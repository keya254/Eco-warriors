//Adding Wazidev Libraries
#include <WaziDev.h>
#include <xlpp.h>
#include <Base64.h>

//Adding GPS and Software Serial Library
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//Declaring pin A0 as recieve and A1 as Transmit
static const int RXPin = A0, TXPin = A1;

//Setting GPS communication Speed to default of NEO-M8 board
static const uint32_t GPSBaud = 9600;

//This is the fixed location's GPS Coordinates(this can be the initial location of waste collection and sorting center)
const double fixedlat = -1.287593;
const double fixedlon = 36.764682;

//Creating GPS Object
TinyGPSPlus gps;

//Activating serial communication
SoftwareSerial ss(RXPin, TXPin);

// NwkSKey (Network Session Key) and Appkey (AppKey) are used for securing LoRaWAN transmissions.
// You need to copy them from/to your LoRaWAN server or gateway.
// You need to configure also the devAddr. DevAddr need to be different for each devices!!
// Copy'n'paste the DevAddr (Device Address): 26011D91
unsigned char devAddr[4] = {0x26, 0x01, 0x1D, 0x91};

// Copy'n'paste the key to your Wazigate: 23158D3BBC31E6AF670D195B5AED5525
unsigned char appSkey[16] = {0x23, 0x15, 0x8D, 0x3B, 0xBC, 0x31, 0xE6, 0xAF, 0x67, 0x0D, 0x19, 0x5B, 0x5A, 0xED, 0x55, 0x25};

// Copy'n'paste the key to your Wazigate: 23158D3BBC31E6AF670D195B5AED5525
unsigned char nwkSkey[16] = {0x23, 0x15, 0x8D, 0x3B, 0xBC, 0x31, 0xE6, 0xAF, 0x67, 0x0D, 0x19, 0x5B, 0x5A, 0xED, 0x55, 0x25};

//Creating Wazidev object
WaziDev wazidev;

void setup()
{
  Serial.begin(38400);

  //Start listening for GPS Data
  ss.begin(GPSBaud);

  //Start LoRa Communications
  wazidev.setupLoRaWAN(devAddr, appSkey, nwkSkey);
}

//Create XLPP object
XLPP xlpp(120);

void loop(void)
{
  //When GPS data is available.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  //If there's an error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected: check wiring.");
    while (true);
  }

  // 2.
  // Send payload with LoRaWAN.
  Serial.print("LoRaWAN send ... ");
  uint8_t e = wazidev.sendLoRaWAN(xlpp.buf, xlpp.len);
  if (e != 0)
  {
    Serial.print("Err ");
    Serial.println(e);
    delay(6000);
    return;
  }
  Serial.println("OK");

  // 3.
  // Receive LoRaWAN message (waiting for 6 seconds only).
  Serial.print("LoRa receive ... ");
  uint8_t offs = 0;
  long startSend = millis();
  e = wazidev.receiveLoRaWAN(xlpp.buf, &xlpp.offset, &xlpp.len, 6000);
  long endSend = millis();
  if (e != 0)
  {
    if (e == ERR_LORA_TIMEOUT) {
      Serial.println("nothing received");
    }
    else
    {

      Serial.print("Err ");
      Serial.println(e);
    }
    delay(6000);
    return;
  }
  Serial.println("OK");

  //Printing Activities on Serial Monitor
  Serial.print("Time On Air: ");
  Serial.println(endSend - startSend);
  Serial.print("LoRa SNR: ");
  Serial.println(wazidev.loRaSNR);
  Serial.print("LoRa RSSI: ");
  Serial.println(wazidev.loRaRSSI);
  Serial.print("LoRaWAN frame size: ");
  Serial.println(xlpp.offset + xlpp.len);
  Serial.print("LoRaWAN payload len: ");
  Serial.println(xlpp.len);
  Serial.print("Payload: ");
  char payload[100];
  base64_decode(payload, xlpp.getBuffer(), xlpp.len);
  Serial.println(payload);
  Serial.println(" ");

  //delay(5000);
}

void displayInfo() {
  //Checking for number of Valid Satelites in sight
  if (gps.satellites.isValid()) {
    Serial.print("Sats: ");
    Serial.print(gps.satellites.value());
  } else {
    Serial.println(" INVALID SATELITE");
  }

  //Checking GPS location coordinates
  float newlat, newlon, distance;
  if (gps.location.isValid()) {
    Serial.print(" Location: ");
    newlat = gps.location.lat();
    newlon = gps.location.lng();
    distance = calcDistance(fixedlat, fixedlon, newlat, newlon);
    Serial.print(newlat, 6);
    Serial.print(" ");
    Serial.print(newlon, 6);
    Serial.print(" ");
    Serial.print(distance);
    Serial.print("m");
  } else {
    Serial.println(" INVALID LOCATION");
  }

  //Checking Altitude/Elivation above sea level
  float alt;
  if (gps.altitude.isValid() ) {
    Serial.print(" Altitude: ");
    alt = gps.altitude.meters();
    Serial.println(alt);
  } else {
    Serial.println(" INVALID ALTITUDE");
  }

  //1
  // Create xlpp payload.
  xlpp.reset();
  xlpp.addGPS(1, newlat, newlon, alt);
  xlpp.addAnalogInput(1, distance);

  //wait 1 second between checks
  //delay(1000);
}

//Calculating distance between fixed coordinates and new coordinates
float calcDistance(float lat1, float lon1, float lat2, float lon2) {
  float dlon, dlat, a, c;
  float dist = 0.0;
  dlon = dtor(lon2 - lon1);
  dlat = dtor(lat2 - lat1);
  a = pow(sin(dlat / 2), 2) + cos(dtor(lat1)) * cos(dtor(lat2)) * pow(sin(dlon / 2), 2);
  c = 2 * atan2(sqrt(a), sqrt(1 - a));

  dist = 6378140.0f * c;  //radius of the earth (6378140 meters) in feet 20925656.2
  return (dist + 0.5);
}

//Convert degrees to radians
float dtor(float fdegrees) {
  return (fdegrees * PI / 180);
}
