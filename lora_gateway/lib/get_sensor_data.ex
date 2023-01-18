defmodule LoraGateway.FetchData do

  def get_sensor_data(endpoint) do
    {:ok, data} = HTTPoison.get!(endpoint).body |> Jason.decode
    data
  end
end 
