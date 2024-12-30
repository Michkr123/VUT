#!/usr/bin/python3.10
# coding=utf-8
import pandas as pd
import geopandas as gpd
import matplotlib.pyplot as plt
import contextily as ctx
from shapely.geometry import Point
from sklearn.cluster import DBSCAN
import numpy as np

# Funkce pro vytvoření GeoDataFrame
def make_geo(df_accidents: pd.DataFrame, df_locations: pd.DataFrame, region: str) -> gpd.GeoDataFrame:
    """
    Vytvoří GeoDataFrame ze vstupních DataFrame, nastaví CRS a odstraní neplatné pozice.
    """
    # Spojení dat podle klíče "p1"
    df = df_accidents.merge(df_locations, on="p1")

    # Filtr podle vybraného kraje
    df = df[df['region'] == region]

    # Odstranění nulových a neplatných souřadnic
    df = df[(df['d'] != 0) & (df['e'] != 0)]
    df = df[np.isfinite(df['d']) & np.isfinite(df['e'])]

    # Oprava prohozených souřadnic (d < e)
    swapped = df['d'] < df['e']
    df.loc[swapped, ['d', 'e']] = df.loc[swapped, ['e', 'd']].values

    # Vytvoření GeoDataFrame s EPSG:5514
    gdf = gpd.GeoDataFrame(
        df,
        geometry=[Point(xy) for xy in zip(df['d'], df['e'])],
        crs="EPSG:5514"  # S-JTSK
    )

    return gdf

# Funkce pro vizualizaci geografických dat
def plot_geo(gdf: gpd.GeoDataFrame, fig_location: str = None, show_figure: bool = False):
    """
    Vykreslí mapu s nehodami podle alkoholu (p11 >= 4) ve dvou vybraných měsících.
    """
    print("Starting to plot geographic data...")

    # Filtrování nehod pod vlivem alkoholu
    gdf_alcohol = gdf[gdf['p11'] >= 4]

    # Vybrané měsíce
    months = [1, 7]  # Leden a červenec
    titles = ["Nehody v lednu", "Nehody v červenci"]

    # Vytvoření subgrafů
    fig, axes = plt.subplots(1, 2, figsize=(15, 8), constrained_layout=True)

    for i, month in enumerate(months):
        ax = axes[i]

        # Filtrování podle měsíce
        gdf_month = gdf_alcohol[gdf_alcohol['date'].dt.month == month]

        # Pokud nejsou data
        if gdf_month.empty:
            ax.set_title(f"Žádné nehody v měsíci {month}")
            ax.set_axis_off()
            continue

        # Vykreslení bodů
        gdf_month.plot(ax=ax, markersize=5, color="red", alpha=0.6)

        # Přidání podkladové mapy
        try:
            ctx.add_basemap(ax, crs=gdf_month.crs.to_string(), source=ctx.providers.CartoDB.Positron, zoom=10)
        except Exception as e:
            print(f"Error loading basemap: {e}")

        # Nastavení titulků a os
        ax.set_title(titles[i])
        ax.set_axis_off()

    # Uložení nebo zobrazení grafu
    if fig_location:
        plt.savefig(fig_location, dpi=300)
    if show_figure:
        plt.show()
    plt.close()

    print("Finished plotting geographic data.")

# Funkce pro vizualizaci četnosti nehod (clustery)
def plot_cluster(gdf: gpd.GeoDataFrame, fig_location: str = None, show_figure: bool = False):
    """
    Vykreslí clustery nehod zaviněných zvěří (p10 = 4).
    """
    print("Starting to plot clusters...")

    # Filtrování nehod zaviněných zvěří
    gdf_animals = gdf[gdf['p10'] == 4].copy()

    # Extrakce souřadnic
    coords = np.array([(geom.x, geom.y) for geom in gdf_animals.geometry])

    # Kontrola neplatných hodnot
    coords = coords[np.isfinite(coords).all(axis=1)]

    # Pokud nejsou data
    if len(coords) == 0:
        print("Žádné nehody zaviněné zvěří.")
        return

    print("Applying DBSCAN...")
    # Aplikace DBSCAN
    db = DBSCAN(eps=500, min_samples=10).fit(coords)
    labels = db.labels_

    # Přidání clusterů do GeoDataFrame
    gdf_animals.loc[:, 'cluster'] = labels

    # Vykreslení clusterů
    fig, ax = plt.subplots(figsize=(10, 8))

    for cluster_id in np.unique(labels):
        cluster_points = gdf_animals[gdf_animals['cluster'] == cluster_id]
        if cluster_id != -1:
            hull = cluster_points.geometry.union_all().convex_hull
            gpd.GeoSeries(hull).plot(ax=ax, alpha=0.4, color="blue")
        cluster_points.plot(ax=ax, markersize=5, label=f"Cluster {cluster_id}" if cluster_id != -1 else "Šum", alpha=0.6)

    # Nastavení grafu
    ax.set_title("Clustery nehod zaviněných zvěří")
    ax.set_axis_off()
    plt.legend()

    # Uložení nebo zobrazení grafu
    if fig_location:
        plt.savefig(fig_location, dpi=300)
    if show_figure:
        plt.show()
    plt.close()

    print("Finished plotting clusters.")

# Hlavní část programu
if __name__ == "__main__":
    print("Starting script...")
    
    # Načtení dat
    print("Loading data...")
    df_accidents = pd.read_pickle("accidents.pkl.gz")
    df_locations = pd.read_pickle("locations.pkl.gz")
    print("Data loaded.")
    
    # Výběr kraje
    selected_region = 'STC'  # Vybraný kraj (například Středočeský kraj)

    # Vytvoření GeoDataFrame
    print("Creating GeoDataFrame...")
    gdf = make_geo(df_accidents, df_locations, selected_region)
    print("GeoDataFrame created.")
    
    # Vizualizace geografických dat
    print("Plotting geographic data...")
    plot_geo(gdf, "geo1.png", True)
    print("Geographic data plotted.")
    
    # Vizualizace clusterů
    print("Plotting clusters...")
    plot_cluster(gdf, "geo2.png", True)
    print("Clusters plotted.")
    
    print("Script completed.")
