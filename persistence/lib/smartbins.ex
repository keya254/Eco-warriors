defmodule Persistence.Smartbins do
  use Ecto.Schema

  import Ecto.Changeset

  @smartbins_fields ~w[device_id gateway_id]a

  schema "smartbins" do
    field :device_id, :string
    field :gateway_id, :string
    field :longitude, :string
    field :latitude, :string
    field :bin_level, :integer

    timestamps()
  end

  def add_bin_changeset(smart_bin) do
    %__MODULE__{}
    |> cast(smart_bin, @smartbins_fields)
    |> validate_required(@smartbins_fields)
    |> unique_constraint(:device_id)
  end

end
