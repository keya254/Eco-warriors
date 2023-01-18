defmodule Persistence.Repo.Migrations.CreateBins do
  use Ecto.Migration

  def change do
    create table(:smartbins) do
      add :gateway_id, :string, null: false
      add :device_id, :string, null: false
      add :longitude, :string, null: false
      add :latitude, :string, null: false
      add :bin_level, :integer, null: false

      timestamps()
    end
  end
end
