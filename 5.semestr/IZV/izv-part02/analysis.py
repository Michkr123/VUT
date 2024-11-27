#!/usr/bin/env python3.12
# coding=utf-8
#%%
from matplotlib import pyplot as plt
import pandas as pd
import seaborn as sns
import numpy as np
import zipfile
import sys

# muzete pridat libovolnou zakladni knihovnu ci knihovnu predstavenou na prednaskach
# dalsi knihovny pak na dotaz

# Ukol 1: nacteni dat ze ZIP souboru
def load_data(filename: str, ds: str) -> pd.DataFrame:
    with zipfile.ZipFile(filename, 'r') as z:
        files = z.namelist()
        data_frames = []
        for file in files:
            if ds in file and file.endswith('.xls'): 
                with z.open(file) as f: 
                    df = pd.read_html(f, encoding='cp1250')[0] 
                    # Smazání nepojmenovaných sloupců 
                    df = df.loc[:, ~df.columns.str.contains('^Unnamed')] 
                    data_frames.append(df) 
        # Spojení datových rámců dohromady 
        combined_df = pd.concat(data_frames, ignore_index=True) 
        return combined_df 



# Ukol 2: zpracovani dat
def parse_data(df: pd.DataFrame, verbose: bool = False) -> pd.DataFrame:
    # Vytvoření nového DataFrame
    new_df = df.copy()

    # Převod sloupce p2a na formát data, bereme v potaz formát "DD.MM.YYYY"
    new_df['date'] = pd.to_datetime(new_df['p2a'], format='%d.%m.%Y')

    # Vytvoření sloupce region
    region_map = {0: "PHA", 1: "STC", 2: "JHC", 3: "PLK", 4: "ULK", 5: "HKK", 6: "JHM", 7: "MSK",
                  14: "OLK", 15: "ZLK", 16: "VYS", 17: "PAK", 18: "LBK", 19: "KVK"}
    new_df['region'] = new_df['p4a'].map(region_map)  # Adjust based on the correct column

    # Odstranění duplikovaných záznamů dle identifikačního čísla (p1)
    new_df = new_df.drop_duplicates(subset='p1')

    # Výpočet a výpis velikosti datového rámce po úpravách
    if verbose:
        total_size = new_df.memory_usage(deep=True).sum() / (10**6)
        print(f"new_size={total_size:.1f} MB")

    return new_df

#%%
# Ukol 3: počty nehod v jednotlivých regionech podle stavu vozovky
def plot_state(df: pd.DataFrame, fig_location: str = None, show_figure: bool = False):
    # Replace numerical values in p16 with descriptive strings
    condition_map = {1: "Suchý povrch", 2: "Suchý povrch", 3: "Mokro", 4: "Bláto", 5: "Náledí, ujetý sníh", 6: "Náledí, ujetý sníh"}
    df['road_condition'] = df['p16'].map(condition_map)
    
    # Filter and count accidents by region and road condition
    accident_counts = df[df['road_condition'].notna()].groupby(['region', 'road_condition']).size().reset_index(name='counts')
    
    # Set up the plot
    sns.set_theme(style="whitegrid")
    fig, axs = plt.subplots(2, 2, figsize=(15, 10), constrained_layout=True)
    conditions = accident_counts['road_condition'].unique()
    
    for ax, condition in zip(axs.flatten(), conditions):
        sns.barplot(data=accident_counts[accident_counts['road_condition'] == condition], x='region', y='counts', ax=ax)
        ax.set_title(f'Stav povrchu vozovky: {condition}')
        ax.set_xlabel('Kraj')
        ax.set_ylabel('Počet nehod')
    
    plt.suptitle('Počet nehod v jednotlivých regionech podle stavu vozovky', fontsize=16)
    
    # Save figure if location provided
    if fig_location:
        plt.savefig(fig_location)
    
    # Show figure if requested
    if show_figure:
        plt.show()
    else:
        plt.close()
#%%
# Ukol4: alkohol a následky v krajích
def plot_alcohol(df: pd.DataFrame, df_consequences: pd.DataFrame, fig_location: str = None, show_figure: bool = False):
    # Merge the dataframes on index p1
    merged_df = pd.merge(df, df_consequences, on='p1', suffixes=('_accident', '_consequence'))
    
    # Filter for accidents involving alcohol (p11 >= 3)
    alcohol_df = merged_df[merged_df['p11'] >= 3]
    
    # Filter out rows where the time is unknown
    alcohol_df = alcohol_df[alcohol_df['p2a'].notna()]
    
    # Determine if the person injured is a driver or passenger
    alcohol_df['person'] = alcohol_df['p59a'].apply(lambda x: 'řidič' if x == 1 else 'spolujezdec')
    
    # Determine the level of injury
    injury_levels = {1: 'Usmrcení', 2: 'Těžké zranění', 3: 'Lehké zranění', 4: 'Bez zranění'}
    alcohol_df['injury_level'] = alcohol_df['p59g'].map(injury_levels).fillna('Bez zranění')
    
    # Count the number of injuries by region, person, and injury level
    injury_counts = alcohol_df.groupby(['region', 'person', 'injury_level']).size().reset_index(name='counts')
    
    # Set up the plot
    sns.set_theme(style="whitegrid")
    fig, axs = plt.subplots(2, 2, figsize=(15, 10))
    injury_levels_sorted = ['Bez zranění', 'Lehké zranění', 'Těžké zranění', 'Usmrcení']
    
    for ax, injury_level in zip(axs.flatten(), injury_levels_sorted):
        sns.barplot(
            data=injury_counts[injury_counts['injury_level'] == injury_level],
            x='region', y='counts', hue='person', ax=ax, palette='muted'
        )
        ax.set_title(f'Úroveň zranění: {injury_level}')
        ax.set_xlabel('')
        ax.set_ylabel('Počet následků')

    # Move the legend outside of the plots
    handles, labels = axs[0, 0].get_legend_handles_labels()
    fig.legend(handles, labels, loc='upper center', title='Osoba', bbox_to_anchor=(0.5, -0.05), ncol=2)
    
    # Remove individual legends
    for ax in axs.flatten():
        ax.legend().set_visible(False)
    
    plt.suptitle('Počet nehod pod vlivem alkoholu v jednotlivých regionech podle úrovně zranění', fontsize=16)
    fig.tight_layout(rect=[0, 0.03, 1, 0.95])  # Adjust layout to make space for the legend
    
    # Save figure if location provided
    if fig_location:
        plt.savefig(fig_location, bbox_inches='tight')
    
    # Show figure if requested
    if show_figure:
        plt.show()
    plt.close(fig)  # Ensure the figure is closed to end the program properly


# Ukol 5: Druh nehody (srážky) v čase
def plot_type(df: pd.DataFrame, fig_location: str = None,
              show_figure: bool = False):
    pass


if __name__ == "__main__":
    # zde je ukazka pouziti, tuto cast muzete modifikovat podle libosti
    # skript nebude pri testovani pousten primo, ale budou volany konkreni
    # funkce.
    
    df = load_data("data_23_24.zip", "nehody")
    df_consequences = load_data("data_23_24.zip", "nasledky")
    df2 = parse_data(df, True)
    
    # plot_state(df2, "01_state.png")
    plot_alcohol(df2, df_consequences, "02_alcohol.png", True)
    plot_type(df2, "03_type.png")


# Poznamka:
# pro to, abyste se vyhnuli castemu nacitani muzete vyuzit napr
# VS Code a oznaceni jako bunky (radek #%%% )
# Pak muzete data jednou nacist a dale ladit jednotlive funkce
# Pripadne si muzete vysledny dataframe ulozit nekam na disk (pro ladici
# ucely) a nacitat jej naparsovany z disku

