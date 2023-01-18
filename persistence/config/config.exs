use Mix.Config

config :persistence, ecto_repos: [Persistence.Repo]

config :persistence, Persistence.Repo,
  database: "smartbins",
  username: "postgres",
  password: "postgres",
  hostname: "localhost"
