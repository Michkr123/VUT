#!/usr/bin/python3.10
# coding=utf-8
import pandas as pd
import geopandas
import matplotlib.pyplot as plt
import contextily
from shapely.geometry import Point
from sklearn.cluster import KMeans
import numpy as np

def make_geo(df_accidents: pd.DataFrame, df_locations: pd.DataFrame, region: str) -> geopandas.GeoDataFrame:
    """
    Vytvoří GeoDataFrame z poskytnutých dat o nehodách a lokalitách.

    Argumenty:
        df_accidents (pd.DataFrame): DataFrame obsahující data o nehodách.
        df_locations (pd.DataFrame): DataFrame obsahující data o lokalitách.
        region (str): Region, podle kterého se filtrují nehody.

    Návratová hodnota:
        gpd.GeoDataFrame: GeoDataFrame s lokalitami nehod a příslušnými daty.
    """
    df = df_accidents.merge(df_locations, on="p1")
    df = df[df['region'] == region]
    # Odstranění neplatných nebo nulových souřadnic
    df = df[(df['d'] != 0) & (df['e'] != 0)]
    df = df[np.isfinite(df['d']) & np.isfinite(df['e'])]

    # Prohození nevhodných souřadnic
    swapped = df['d'] < df['e']
    df.loc[swapped, ['d', 'e']] = df.loc[swapped, ['e', 'd']].values

    # Vytvoření GeoDataFrame
    gdf = geopandas.GeoDataFrame(
        df,
        geometry=[Point(xy) for xy in zip(df['d'], df['e'])],
        crs="EPSG:5514"
    )
    return gdf

def plot_geo(gdf: geopandas.GeoDataFrame, fig_location: str = None, show_figure: bool = False):
    """
    Vykreslí geografická data ukazující nehody v lednu a červenci.

    Argumenty:
        gdf (gpd.GeoDataFrame): GeoDataFrame obsahující data o nehodách.
        fig_location (str, optional): Cesta k souboru pro uložení obrázku.
        show_figure (bool, optional): Zda zobrazit obrázek.
    """
    # Filtrování nehod s vlivem alkoholu
    gdf_alcohol = gdf[gdf['p11'] >= 4]
    months = [1, 7]
    titles = ["Nehody v lednu", "Nehody v červenci"]

    fig, axes = plt.subplots(1, 2, figsize=(11, 8), constrained_layout=True)

    for i, month in enumerate(months):
        ax = axes[i]
        gdf_month = gdf_alcohol[gdf_alcohol['date'].dt.month == month]

        # V daném měsíci není žádná nehoda
        if gdf_month.empty:
            ax.set_title(f"Žádné nehody v měsíci {month}")
            ax.set_axis_off()
            continue

        # Vykreslení nehod a mapy
        gdf_month.plot(ax=ax, markersize=5, color="red", alpha=0.6)
        try:
            contextily.add_basemap(ax, crs=gdf_month.crs.to_string(), source=contextily.providers.CartoDB.Positron, zoom=10)
        except Exception:
            pass

        ax.set_title(titles[i])
        ax.set_axis_off()

    if fig_location:
        plt.savefig(fig_location, dpi=300)
    if show_figure:
        plt.show()
    plt.close()

def plot_cluster(gdf: geopandas.GeoDataFrame, fig_location: str = None, show_figure: bool = False):
    """
    Vykreslí shlukovaná data o nehodách pomocí KMeans shlukování.

    Argumenty:
        gdf (gpd.GeoDataFrame): GeoDataFrame obsahující data o nehodách.
        fig_location (str, optional): Cesta k souboru pro uložení obrázku.
        show_figure (bool, optional): Zda zobrazit obrázek.
    """
    # Filtrování nehod způsobených zvěří
    gdf_animals = gdf[gdf['p10'] == 4].copy()

    # Extrakce souřadnic
    coords = np.array([(geom.x, geom.y) for geom in gdf_animals.geometry if geom])
    if len(coords) == 0:
        return

    # Počet clusterů
    n_clusters = 8
    if len(coords) < n_clusters:
        n_clusters = len(coords)

    # Shlukování metodou KMeans
    kmeans = KMeans(n_clusters=n_clusters, random_state=69)
    labels = kmeans.fit_predict(coords)
    gdf_animals['cluster'] = labels

    gdf_animals = gdf_animals.to_crs(epsg=3857)

    # Počet nehod v jednotlivých clusterech
    cluster_accident_counts = gdf_animals.groupby('cluster').size()
    gdf_animals['accident_count'] = gdf_animals['cluster'].map(cluster_accident_counts)

    fig, ax = plt.subplots(figsize=(11, 8))

    gdf_animals.plot(ax=ax, color='red', markersize=5, alpha=0.6, label="Nehody")

    for cluster_id in range(n_clusters):
        cluster_points = gdf_animals[gdf_animals['cluster'] == cluster_id]
        if cluster_points.empty:
            continue

        hull = cluster_points.geometry.union_all().convex_hull
        if hull.is_empty:
            continue

        # Barevné odlišení clusterů
        accident_count = cluster_accident_counts[cluster_id]
        color = plt.cm.viridis(accident_count / cluster_accident_counts.max())
        geopandas.GeoSeries(hull).plot(ax=ax, alpha=0.4, color=color, edgecolor='black')

    try:
        contextily.add_basemap(ax, crs=gdf_animals.crs.to_string(), source=contextily.providers.CartoDB.Positron, zoom=10)
    except Exception:
        pass

    # Barevná škála podle počtu nehod
    sm = plt.cm.ScalarMappable(cmap='viridis', norm=plt.Normalize(vmin=cluster_accident_counts.min(), vmax=cluster_accident_counts.max()))
    sm._A = []
    cbar = fig.colorbar(sm, ax=ax, orientation='horizontal', fraction=0.03, pad=0.04)
    cbar.set_label("Počet nehod v úseku")

    ax.set_title("Nehody v JHM kraji zaviněné lesní zvěří")
    ax.set_axis_off()

    plt.tight_layout()

    if fig_location:
        plt.savefig(fig_location, dpi=300)
    if show_figure:
        plt.show()
    plt.close()

if __name__ == "__main__":
    """
    Hlavní vstupní bod pro skript. Načte data, vytvoří GeoDataFrame a vygeneruje grafy.
    """
    df_accidents = pd.read_pickle("accidents.pkl.gz")
    df_locations = pd.read_pickle("locations.pkl.gz")

    selected_region = 'JHM'
    gdf = make_geo(df_accidents, df_locations, selected_region)

    plot_geo(gdf, "geo1.png", True)
    plot_cluster(gdf, "geo2.png", True)
