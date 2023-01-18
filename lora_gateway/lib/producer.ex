defmodule LoraGateway.Producer do
  alias LoraGateway.FetchData
  @endpoints [
    "https://api.waziup.io/api/v2/devices/6370822068f31908efc4f23f", 
    "https://api.waziup.io/api/v2/devices/636fb26968f31907211dee5e"
  ]

  use GenStage

  def start_link(endpoints) do
    GenStage.start_link(__MODULE__, endpoints, name: __MODULE__)
  end

  def init(endpoints) do
    { :producer, endpoints }
  end

  def handle_demand(demand, endpoints) when demand > 0 do
    endpoints = @endpoints 
    sensor_data = Enum.map(endpoints, &FetchData.get_sensor_data/1)

    {:noreply, sensor_data, endpoints}
  end
end
