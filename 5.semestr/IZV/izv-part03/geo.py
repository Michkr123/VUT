#!/usr/bin/python3.10
# coding=utf-8
# %%%
import pandas as pd
import geopandas
import matplotlib.pyplot as plt
import contextily
import sklearn.cluster
import numpy as np

def make_geo(df_accidents: pd.DataFrame, df_locations: pd.DataFrame) -> geopandas.GeoDataFrame:
    """
    Vytvoří GeoDataFrame ze vstupních DataFrame a odstraní neplatné pozice.
    - Prohozené souřadnice d < e opraví.
    - Vynechá nulové souřadnice (d, e).
    - Zkontroluje, že body jsou v České republice.
    
    Args:
        df_accidents (pd.DataFrame): DataFrame obsahující informace o nehodách.
        df_locations (pd.DataFrame): DataFrame obsahující informace o lokalitách.

    Returns:
        geopandas.GeoDataFrame: GeoDataFrame s platnými pozicemi nehod.
    """
    import geopandas as gpd
    from shapely.geometry import Point

    # Sloučení DataFrame podle společného klíče (např. id nehody)
    df = df_accidents.merge(df_locations, on="p1")

    # Vynecháme řádky, kde jsou souřadnice d nebo e nulové
    df = df[(df['d'] != 0) & (df['e'] != 0)]

    # Oprava prohozených souřadnic (d < e)
    swapped = df['d'] < df['e']
    df.loc[swapped, ['d', 'e']] = df.loc[swapped, ['e', 'd']].values

    # Vytvoříme GeoDataFrame
    gdf = gpd.GeoDataFrame(
        df,
        geometry=[Point(xy) for xy in zip(df['d'], df['e'])],
        crs="EPSG:4326"  # WGS84
    )

    # Transformace souřadnic na S-JTSK (EPSG:5514)
    gdf = gdf.to_crs(epsg=5514)

    # Volitelně: Zkontrolujte, že body jsou v České republice
    # (např. bounding box pro Českou republiku v S-JTSK)
    bbox_cz = {
        "minx": -950000, "maxx": -400000,
        "miny": -1250000, "maxy": -900000
    }
    gdf = gdf[(gdf.geometry.x >= bbox_cz["minx"]) & (gdf.geometry.x <= bbox_cz["maxx"]) &
              (gdf.geometry.y >= bbox_cz["miny"]) & (gdf.geometry.y <= bbox_cz["maxy"])]

    return gdf

def plot_geo(gdf: geopandas.GeoDataFrame, fig_location: str = None, show_figure: bool = False):
    """
    Vykreslete graf obsahující dvě podgrafy s pozicemi nehod ve vybraném kraji,
    které byly zaviněny pod vlivem alkoholu (p11 >= 4).

    Args:
        gdf (geopandas.GeoDataFrame): GeoDataFrame obsahující informace o nehodách.
        fig_location (str, optional): Cesta k uložení grafu. Default je None.
        show_figure (bool, optional): Zda zobrazit graf. Default je False.
    """
    import matplotlib.pyplot as plt
    import contextily as ctx

    # Filtrujeme nehody pod vlivem alkoholu (p11 >= 4)
    gdf_alcohol = gdf[gdf['p11'] >= 4]

    # Vybereme dvě náhodné měsíce pro analýzu
    months = [1, 7]  # Leden a Červenec
    titles = ["Nehody v lednu", "Nehody v červenci"]

    # Vytvoříme subgrafy
    fig, axes = plt.subplots(1, 2, figsize=(15, 8), constrained_layout=True)

    for i, month in enumerate(months):
        ax = axes[i]

        # Filtrujeme nehody pro konkrétní měsíc
        gdf_month = gdf_alcohol[gdf_alcohol['date'].dt.month == month]

        # Kontrola, zda gdf_month není prázdný
        if gdf_month.empty:
            ax.set_title(f"Žádné nehody v měsíci {month}")
            ax.set_axis_off()
            continue

        # Vykreslení bodů
        gdf_month.plot(ax=ax, markersize=5, color="red", alpha=0.6)

        # Přidání podkladové mapy
        ctx.add_basemap(ax, crs=gdf_month.crs.to_string(), source=ctx.providers.Stamen.TonerLite)

        # Nastavení os a nadpisu
        ax.set_title(titles[i])
        ax.set_xlim(gdf_month.geometry.x.min() - 1000, gdf_month.geometry.x.max() + 1000)
        ax.set_ylim(gdf_month.geometry.y.min() - 1000, gdf_month.geometry.y.max() + 1000)
        ax.set_axis_off()

    # Uložení nebo zobrazení grafu
    if fig_location:
        plt.savefig(fig_location, dpi=300)
    if show_figure:
        plt.show()
    plt.close()

def plot_cluster(gdf: geopandas.GeoDataFrame, fig_location: str = None, show_figure: bool = False):
    """
    Vykreslete nehody zaviněné zvěří (p10=4) ve vybraném kraji s vyznačenými oblastmi 
    podbarvenými podle četnosti nehod v dané oblasti.

    Args:
        gdf (geopandas.GeoDataFrame): GeoDataFrame obsahující informace o nehodách.
        fig_location (str, optional): Cesta k uložení grafu. Default je None.
        show_figure (bool, optional): Zda zobrazit graf. Default je False.
    """
    import matplotlib.pyplot as plt
    import contextily as ctx
    from sklearn.cluster import DBSCAN
    from shapely.geometry import MultiPoint
    from geopandas import GeoDataFrame

    # Filtrujeme nehody zaviněné zvěří (p10 = 4)
    gdf_animals = gdf[gdf['p10'] == 4]

    # Extrakce souřadnic pro shlukování
    coords = np.array(list(zip(gdf_animals.geometry.x, gdf_animals.geometry.y)))

    # Použití DBSCAN pro shlukování (vhodné pro geografická data)
    db = DBSCAN(eps=500, min_samples=10).fit(coords)
    gdf_animals['cluster'] = db.labels_

    # Vytvoření GeoDataFrame pro shluky
    clusters = gdf_animals[gdf_animals['cluster'] != -1]
    grouped = clusters.groupby('cluster')
    polygons = []

    for cluster_id, group in grouped:
        points = MultiPoint(list(group.geometry))
        polygons.append(points.convex_hull)

    cluster_gdf = GeoDataFrame({'geometry': polygons, 'cluster': grouped.groups.keys()}, crs=gdf.crs)

    # Vykreslení grafu
    fig, ax = plt.subplots(1, 1, figsize=(12, 8))

    # Vykreslení polygonů shluků
    cluster_gdf.plot(ax=ax, color='red', alpha=0.4, edgecolor='black', legend=True)

    # Vykreslení bodů nehod
    gdf_animals.plot(ax=ax, markersize=5, color='blue', alpha=0.6, label='Nehody')

    # Přidání podkladové mapy
    ctx.add_basemap(ax, crs=gdf.crs.to_string(), source=ctx.providers.Stamen.TonerLite)

    # Nastavení os a zobrazení legendy
    ax.set_title("Nehody zaviněné zvěří")
    ax.set_xlim(gdf_animals.geometry.x.min() - 1000, gdf_animistry.x.max() + 1000)
    ax.set_ylim(gdf_animals.geometry.y.min() - 1000, gdf_animals.geometry.y.max() + 1000)
    ax.set_axis_off()
    ax.legend()

    # Uložení nebo zobrazení grafu
    if fig_location:
        plt.savefig(fig_location, dpi=300)
    if show_figure:
        plt.show()
    plt.close()

if __name__ == "__main__":
    # zde muzete delat libovolne modifikace
    df_accidents = pd.read_pickle("accidents.pkl.gz")
    df_locations = pd.read_pickle("locations.pkl.gz")
    gdf = make_geo(df_accidents, df_locations)

    plot_geo(gdf, "geo1.png", True)
    plot_cluster(gdf, "geo2.png", True)

    # testovani splneni zadani
    import os
    #assert os.path.exists("geo1.png")
    #assert os.path.exists("geo2.png")
