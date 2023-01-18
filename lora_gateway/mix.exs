defmodule LoraGateway.MixProject do
  use Mix.Project

  def project do
    [
      app: :lora_gateway,
      version: "0.1.0",
      elixir: "~> 1.12",
      start_permanent: Mix.env() == :prod,
      deps: deps()
    ]
  end

  # Run "mix help compile.app" to learn about applications.
  def application do
    [
      extra_applications: [:logger],
      mod: {LoraGateway.Application, []}
    ]
  end

  # Run "mix help deps" to learn about dependencies.
  defp deps do
    [
      {:jason, "~>1.4"},
      {:httpoison, "~> 1.8"},
      {:gen_stage, "~> 1.0.0"},
      {:persistence, path: "../persistence"}
    ]
  end
end
