defmodule Persistence do
  import Ecto.Query
  
  alias __MODULE__.{Repo, Smartbins }

  def process_bin(smartbin) do
    {exists, smartbin} = check_if_exists(smartbin)

    add_or_update(exists, smartbin)
  end

  def check_if_exists(smartbin) do
    exists = (
      from sb in Smartbins, 
      where: sb.device_id == ^smartbin.device_id
    ) |> Repo.exists?()

    { exists, smartbin }
  end
  
  def add_or_update({_exists=true, smartbin}) do
    older_bin = Repo.get_by(Smartbins, device_id: smartbin.device_id)

    Ecto.Changeset.change(
      older_bin,
      longitude: smartbin.longitude,  
      latitude: smartbin.latitude,
      bin_level: smartbin.bin_level
    ) 
    |> Repo.update
    |> process_change

  end

  def add_or_update(_exists, smartbin) do
    %{
      device_id: smartbin.device_id,
      longitude: smartbin.longitude,
      latitude: smartbin.latitude,
      bin_level: smartbin.bin_level
    } 
    |> Smartbins.add_bin_changeset
    |> Repo.insert
    |> process_change
  end

  defp process_change({:ok, smartbin}), do: smartbin
  defp process_change({:error, changeset}), do: IO.inspect changeset.errors

  def get_bins() do
    Smartbins |> Repo.all()
  end
end
