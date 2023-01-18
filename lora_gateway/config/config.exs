use Mix.Config

config :persistence, ecto_repos: [Persistence.Repo]

config :persistence, Persistence.Repo,
  database: "smartbins",
  hostname: "localhost",
  password: "postgres",
  username: "postgres"

config :lora_gateway, :persistent_fn, &Persistence.process_bin/1

