defmodule LoraGateway.Application do
  @moduledoc false

  use Application

  @impl true
  def start(_type, _args) do
    children = [
      LoraGateway.Producer,
      LoraGateway.Consumer
    ]

    opts = [strategy: :one_for_one, name: LoraGateway.Supervisor]
    Supervisor.start_link(children, opts)
  end
end
