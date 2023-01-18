//this is for the demo for standalone bins
 // defines pins numbers


int trigPin = 26;  //D4
int echoPin = 25;  //D3
int led=27;
int red=13;
int green=14;
int blue=12;
int buzzer=33;


// defines variables
long duration;
int distance;


void setup() {
  Serial.begin(9600);
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT);
pinMode(led, OUTPUT);
pinMode(red, OUTPUT);
pinMode(green, OUTPUT);
pinMode(blue, OUTPUT);
pinMode(buzzer, OUTPUT);// Sets the echoPin as an Input
 // Starts the serial communication

  
}

void loop() 
 {
// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

digitalWrite(led,HIGH);
delay(500);
digitalWrite(led, LOW);
delay(500);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor

Serial.print("GARBAGE LEVEL = ");
Serial.println (distance);
delay (50);

//this one takes 25 as the max height of the waste in the bin so the green led goes high
if (distance>=25)
{digitalWrite(green, HIGH);}
else
//this one takes 25 as the min height of thewaste in the bin so the yellow led goes high
{digitalWrite(green, LOW);}
if (distance>=10 & distance<=19)
{digitalWrite(blue, HIGH);}
else
{digitalWrite(blue, LOW);}
//this one takes 25 as the lowest height of the waste in the bin so the Red led goes high
if (distance<10)
{digitalWrite(red, HIGH);
delay(200);
digitalWrite(red, LOW);
delay(200);
//this one takes 25 as the lowest height of the waste in the bin so the Buzzer goes high too
digitalWrite(buzzer, HIGH);
delay(50);
digitalWrite(buzzer, LOW);
delay(50);
}
else

{digitalWrite(red, LOW);
digitalWrite(buzzer, LOW);
}
}
