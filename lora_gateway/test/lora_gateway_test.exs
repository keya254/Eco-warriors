defmodule LoraGatewayTest do
  use ExUnit.Case
  doctest LoraGateway

  test "greets the world" do
    assert LoraGateway.hello() == :world
  end
end
