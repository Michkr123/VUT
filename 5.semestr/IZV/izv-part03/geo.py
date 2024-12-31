#!/usr/bin/python3.10
# coding=utf-8
import pandas as pd
import geopandas as gpd
import matplotlib.pyplot as plt
import contextily as ctx
from shapely.geometry import Point
from sklearn.cluster import KMeans
import numpy as np

# Funkce pro vytvoření GeoDataFrame
def make_geo(df_accidents: pd.DataFrame, df_locations: pd.DataFrame, region: str) -> gpd.GeoDataFrame:
    """
    Vytvoří GeoDataFrame ze vstupních DataFrame, nastaví CRS a odstraní neplatné pozice.
    """
    df = df_accidents.merge(df_locations, on="p1")
    df = df[df['region'] == region]
    df = df[(df['d'] != 0) & (df['e'] != 0)]
    df = df[np.isfinite(df['d']) & np.isfinite(df['e'])]

    swapped = df['d'] < df['e']
    df.loc[swapped, ['d', 'e']] = df.loc[swapped, ['e', 'd']].values

    gdf = gpd.GeoDataFrame(
        df,
        geometry=[Point(xy) for xy in zip(df['d'], df['e'])],
        crs="EPSG:5514"  # S-JTSK
    )
    return gdf

# Funkce pro vizualizaci geografických dat
def plot_geo(gdf: gpd.GeoDataFrame, fig_location: str = None, show_figure: bool = False):
    print("Starting to plot geographic data...")
    gdf_alcohol = gdf[gdf['p11'] >= 4]
    months = [1, 7]  # Leden a červenec
    titles = ["Nehody v lednu", "Nehody v červenci"]

    # Create a grid layout with equal width for both subplots and make the plot bigger
    fig, axes = plt.subplots(1, 2, figsize=(36, 28), constrained_layout=False)  # Increased figsize

    for i, month in enumerate(months):
        ax = axes[i]
        gdf_month = gdf_alcohol[gdf_alcohol['date'].dt.month == month]

        if gdf_month.empty:
            ax.set_title(f"Žádné nehody v měsíci {month}")
            ax.set_axis_off()
            continue

        gdf_month.plot(ax=ax, markersize=5, color="red", alpha=0.6)
        try:
            ctx.add_basemap(ax, crs=gdf_month.crs.to_string(), source=ctx.providers.CartoDB.Positron, zoom=10)
        except Exception as e:
            print(f"Error loading basemap: {e}")

        ax.set_title(titles[i])
        ax.set_axis_off()

    # Manually adjust the spacing between subplots
    plt.subplots_adjust(wspace=0.1)  # Adjust width space between subplots

    if fig_location:
        plt.savefig(fig_location, dpi=300)
    if show_figure:
        plt.show()
    plt.close()
    print("Finished plotting geographic data.")

def plot_cluster(gdf: gpd.GeoDataFrame, fig_location: str = None, show_figure: bool = False):
    print("Starting to plot clusters...")
    gdf_animals = gdf[gdf['p10'] == 4].copy()
    print(f"Number of animal-caused accidents: {len(gdf_animals)}")

    coords = np.array([(geom.x, geom.y) for geom in gdf_animals.geometry if geom])
    if len(coords) == 0:
        print("No valid data for clustering.")
        return

    # Set the number of clusters to 7
    n_clusters = 8
    if len(coords) < n_clusters:
        print(f"Not enough data for {n_clusters} clusters. Using {len(coords)} clusters instead.")
        n_clusters = len(coords)

    print(f"Applying K-means with {n_clusters} clusters...")
    kmeans = KMeans(n_clusters=n_clusters)
    labels = kmeans.fit_predict(coords)
    gdf_animals['cluster'] = labels

    # Reproject to EPSG:3857 for basemap compatibility
    gdf_animals = gdf_animals.to_crs(epsg=3857)

    # Calculate accident counts per cluster
    cluster_accident_counts = gdf_animals.groupby('cluster').size()
    gdf_animals['accident_count'] = gdf_animals['cluster'].map(cluster_accident_counts)

    fig, ax = plt.subplots(figsize=(10, 8))

    # Plot individual accidents as small red dots
    gdf_animals.plot(ax=ax, color='red', markersize=5, alpha=0.6, label="Accidents")

    # Plot clusters with colors based on accident counts
    for cluster_id in range(n_clusters):
        cluster_points = gdf_animals[gdf_animals['cluster'] == cluster_id]
        if cluster_points.empty:
            print(f"Cluster {cluster_id} is empty.")
            continue

        hull = cluster_points.geometry.unary_union.convex_hull
        if hull.is_empty:
            print(f"Cluster {cluster_id} has no valid hull.")
            continue

        # Use accident count to determine color
        accident_count = cluster_accident_counts[cluster_id]
        color = plt.cm.YlGnBu(accident_count / cluster_accident_counts.max())  # Normalize accident count
        gpd.GeoSeries(hull).plot(ax=ax, alpha=0.4, color=color, edgecolor='black')

    # Add basemap
    try:
        ctx.add_basemap(ax, crs=gdf_animals.crs.to_string(), source=ctx.providers.CartoDB.Positron, zoom=10)
    except Exception as e:
        print(f"Error loading basemap: {e}")

    # Add legend for accident count
    sm = plt.cm.ScalarMappable(cmap='YlGnBu', norm=plt.Normalize(vmin=cluster_accident_counts.min(), vmax=cluster_accident_counts.max()))
    sm._A = []  # Required for ScalarMappable
    cbar = fig.colorbar(sm, ax=ax, orientation='horizontal', fraction=0.03, pad=0.04)
    cbar.set_label("Počet nehod v úseku")

    ax.set_title("Nehody v JHM kraji zaviněné lesní zvěří")
    ax.set_axis_off()

    if fig_location:
        plt.savefig(fig_location, dpi=300)
    if show_figure:
        plt.show()
    plt.close()
    print("Finished plotting clusters.")

# Hlavní část programu
if __name__ == "__main__":
    print("Starting script...")
    print("Loading data...")
    df_accidents = pd.read_pickle("accidents.pkl.gz")
    df_locations = pd.read_pickle("locations.pkl.gz")
    print("Data loaded.")

    selected_region = 'JHM'  # Vybraný kraj (například Středočeský kraj)
    print("Creating GeoDataFrame...")
    gdf = make_geo(df_accidents, df_locations, selected_region)
    print("GeoDataFrame created.")

    print("Plotting geographic data...")
    plot_geo(gdf, "geo1.png", True)
    print("Geographic data plotted.")

    print("Plotting clusters...")
    plot_cluster(gdf, "geo2.png", True)
    print("Clusters plotted.")

    print("Script completed.")
