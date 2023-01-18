defmodule LoraGateway.Consumer do
  use GenStage

  def start_link(_initial) do
    GenStage.start_link(__MODULE__, :state_doesnt_matter)
  end

  def init(_state) do
    opts = [{LoraGateway.Producer, min_demand: 0, max_demand: 10}]
    { :consumer, :state_ignore, subscribe_to: opts}
  end

  def handle_events(sensor_data, _from, state) do
    Enum.map(sensor_data, fn x -> parse_sensor_data(x) |> IO.inspect end)
    
    {:noreply, [], state}
  end

  defp parse_sensor_data(data) do
    %{
      gateway_id: data["gateway_id"],
      device_id: data["id"],
      longitude: get_long(data["sensors"]),
      latitude: get_lat(data["sensors"]),
      get_bin_level: get_bin_level(data["sensors"])
    }
  end

    defp get_lat([_analog_input, gps_location]) do
      gps_location["value"]["value"]["latitude"]
  end

  defp get_long([_analog_input, gps_location]) do
      gps_location["value"]["value"]["longitude"]
  end

  defp get_bin_level([analog_input, _]) do
    analog_input["value"]["value"]
  end
end
