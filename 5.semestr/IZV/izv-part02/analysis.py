#!/usr/bin/env python3.12
# coding=utf-8

from matplotlib import pyplot as plt
import pandas as pd
import seaborn as sns
import numpy as np
import zipfile

# muzete pridat libovolnou zakladni knihovnu ci knihovnu predstavenou na prednaskach
# dalsi knihovny pak na dotaz

# Ukol 1: nacteni dat ze ZIP souboru
def load_data(filename: str, ds: str) -> pd.DataFrame:
    """
    Nacte data ze ZIP souboru.

    Argumenty:
        filename (str): Název souboru ZIP.
        ds (str): Dataset, který se má načíst.

    Návratová hodnota:
        pd.DataFrame: Kombinovaný datový rámec obsahující data ze všech souborů .xls.
    """
    with zipfile.ZipFile(filename, 'r') as z:
        files = z.namelist()
        data_frames = []
        
        # Čtení všech odpovídajících souborů s koncovkou .xls s dekódováním cp1250
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
    """
    Zpracuje a připraví data pro další analýzu.

    Argumenty:
        df (pd.DataFrame): Vstupní datový rámec.
        verbose (bool): Pokud je True, vypíše velikost datového rámce.

    Návratová hodnota:
        pd.DataFrame: Zpracovaný datový rámec.
    """
    new_df = df.copy()
    # Převod sloupce p2a na formát data "DD.MM.YYYY"
    new_df['date'] = pd.to_datetime(new_df['p2a'], format='%d.%m.%Y')

    # Vytvoření mapy a namapování krajů
    region_map = {0: "PHA", 1: "STC", 2: "JHC", 3: "PLK", 4: "ULK", 5: "HKK", 6: "JHM", 7: "MSK",
                  14: "OLK", 15: "ZLK", 16: "VYS", 17: "PAK", 18: "LBK", 19: "KVK"}
    new_df['region'] = new_df['p4a'].map(region_map)

    # Odstranění duplicitních záznamů
    new_df = new_df.drop_duplicates(subset='p1')

    # Výpočet a výpis velikosti df
    if verbose:
        total_size = new_df.memory_usage(deep=True).sum() / (10**6)
        print(f"new_size={total_size:.1f} MB")
    return new_df

# Ukol 3: počty nehod v jednotlivých regionech podle stavu vozovky
def plot_state(df: pd.DataFrame, fig_location: str = None, show_figure: bool = False):
    """
    Vykreslí počty nehod v jednotlivých regionech podle stavu vozovky.

    Argumenty:
        df (pd.DataFrame): Zpracovaný datový rámec.
        fig_location (str): Cesta pro uložení obrázku.
        show_figure (bool): Pokud je True, zobrazí graf.

    Návratová hodnota:
        -
    """
    # Vytvoření mapy a namapování stavu vozovky
    condition_map = {1: "Suchý povrch", 2: "Suchý povrch", 3: "Mokro", 4: "Bláto", 5: "Náledí, ujetý sníh", 6: "Náledí, ujetý sníh"}
    df['road_condition'] = df['p16'].map(condition_map)

    # Vyfiltrování prázdných záznamů
    filtered_df = df[df['road_condition'].notna()]

    # Seskupení podle krajů a podle stavu vozovky
    grouped_df = filtered_df.groupby(['region', 'road_condition'])
    counts_series = grouped_df.size()
    accident_counts = counts_series.reset_index(name='counts')

    # Vytvoření grafu
    sns.set_theme(style="whitegrid")
    fig, axs = plt.subplots(2, 2, figsize=(15, 10), constrained_layout=True)
    conditions = accident_counts['road_condition'].unique()

    for i, (ax, condition) in enumerate(zip(axs.flatten(), conditions)):
        sns.barplot(data=accident_counts[accident_counts['road_condition'] == condition], x='region', y='counts', ax=ax)
        ax.set_title(condition)
        ax.set_xlabel('')
        ax.set_ylabel('Počet nehod')
        
        if i % 2 == 0:
            ax.set_ylabel('Počet nehod')
        else:
            ax.set_ylabel('')

        if i > 1:
            ax.set_xlabel('Kraj')
        else:
            ax.set_xlabel('')

        for container in ax.containers:
            ax.bar_label(container)

    plt.suptitle('Počet nehod v jednotlivých regionech podle stavu vozovky', fontsize=16)
    
    if fig_location:
        plt.savefig(fig_location)
    
    if show_figure:
        plt.show()
    
    plt.close()

# Ukol4: alkohol a následky v krajích
def plot_alcohol(df: pd.DataFrame, df_consequences: pd.DataFrame, fig_location: str = None, show_figure: bool = False):
    """
    Vykreslí počet nehod pod vlivem alkoholu v jednotlivých regionech podle úrovně zranění.

    Argumenty:
        df (pd.DataFrame): Datový rámec s nehodami.
        df_consequences (pd.DataFrame): Datový rámec s následky nehod.
        fig_location (str): Cesta pro uložení obrázku.
        show_figure (bool): Pokud je True, zobrazí graf.

    Návratová hodnota:
        -
    """
    # Spojení dvou df na základě indexu p1
    merged_df = pd.merge(df, df_consequences, on='p1', suffixes=('_accident', '_consequence'))
    
    # Filtrování nehod zahrnujících alkohol (p11 >= 3)
    alcohol_df = merged_df[merged_df['p11'] >= 3]
    
    # Odstranění záznamů bez času
    alcohol_df = alcohol_df[alcohol_df['p2a'].notna()]
    
    # Určení zraněných osob
    alcohol_df['person'] = alcohol_df['p59a'].apply(lambda x: 'řidič' if x == 1 else 'spolujezdec')
    
    # Vytvoření mapy a namapování úrovně zranění
    injury_levels = {1: 'Usmrcení', 2: 'Těžké zranění', 3: 'Lehké zranění', 4: 'Bez zranění'}
    alcohol_df['injury_level'] = alcohol_df['p59g'].map(injury_levels).fillna('Bez zranění')
    
    # Počet zranění podle kraje, osoby a úrovně zranění
    injury_counts = alcohol_df.groupby(['region', 'person', 'injury_level']).size().reset_index(name='counts')
    
    # Vytvoření grafu
    sns.set_theme(style="whitegrid")
    fig, axs = plt.subplots(2, 2, figsize=(15, 10))
    injury_levels_sorted = ['Bez zranění', 'Lehké zranění', 'Těžké zranění', 'Usmrcení']
    
    for i, (ax, injury_level) in enumerate(zip(axs.flatten(), injury_levels_sorted)):
        sns.barplot(
            data=injury_counts[injury_counts['injury_level'] == injury_level],
            x='region', y='counts', hue='person', ax=ax, palette='muted'        )
        ax.set_title(injury_level)
        if i > 1:
            ax.set_xlabel('Kraj')
        else:
            ax.set_xlabel('')

        if i % 2 == 0:
            ax.set_ylabel('Počet nehod')
        else:
            ax.set_ylabel('')
        
        ax.tick_params(axis='x', labelsize=8)
        
        for container in ax.containers:
            ax.bar_label(container, fontsize=6)

    handles, labels = axs[0, 0].get_legend_handles_labels()
    fig.legend(handles, labels, loc='center right', bbox_to_anchor=(0.8, 0.475), ncol=1)
    
    # Odstranění legend
    for ax in axs.flatten():
        ax.legend().set_visible(False)
    
    plt.suptitle('Počet nehod pod vlivem alkoholu v jednotlivých regionech podle úrovně zranění', fontsize=16, x=0.4)
    fig.tight_layout(rect=[0, 0, 0.7, 1])
    
    if fig_location:
        plt.savefig(fig_location, bbox_inches='tight')
    
    if show_figure:
        plt.show()
    plt.close(fig)

# Ukol 5: Druh nehody (srážky) v čase
def plot_type(df: pd.DataFrame, fig_location: str = None, show_figure: bool = False):
    """
    Vykreslí druh nehod (srážek) v jednotlivých regionech v čase.

    Argument:
        df (pd.DataFrame): Datový rámec s nehodami.
        fig_location (str): Cesta pro uložení obrázku.
        show_figure (bool): Pokud je True, zobrazí graf.

    Návratová hodnota:
        -
    """
    # Získání seznamu unikátních regionů
    unique_regions = df['region'].unique() 

    # Náhodný výběr čtyř regionů
    selected_regions = np.random.choice(unique_regions, 4, replace=False)

    # Vytvoření mapy a namapování druhů srážek
    collision_types = {
        1: 's jedoucím nekolejovým vozidlem', 2: 's vozidlem zaparkovaným', 3: 's pevnou překážkou', 4: 's chodcem',
        5: 's lesní zvěří', 6: 's domácím zvířetem', 7: 's vlakem', 8: 's tramvají'
    }
    df['collision_type'] = df['p6'].map(collision_types)

    # Filtrování pro vybrané regiony    
    df_filtered = df[df['region'].isin(selected_regions)]

    # Kontingenční tabulka pro získání počtu srážek za den pro každý region a druh srážky    
    pivot_df = df_filtered.pivot_table(
        index=['date', 'region'], 
        columns='collision_type', 
        values='p1', 
        aggfunc='count', 
        fill_value=0
    ).reset_index()
    
    # Seskupení podle měsíce pro každý region a druh srážky    
    pivot_df['date'] = pd.to_datetime(pivot_df['date'])
    monthly_data = pivot_df.groupby(['region', pd.Grouper(key='date', freq='MS')]).sum().reset_index()
    
    # Vytvoření grafu
    colors = ['red', 'blue', 'green', 'orange', 'purple', 'brown', 'gray', 'pink']
    sns.set_theme(style="white")
    fig, axs = plt.subplots(2, 2, figsize=(20, 10), sharex=True)
    regions_sorted = sorted(selected_regions)
    
    for i, (ax, region) in enumerate(zip(axs.flatten(), regions_sorted)):
        region_data = monthly_data[monthly_data['region'] == region]
        region_data = region_data.set_index('date')
        region_data = region_data.loc['2023-01-01':'2024-9-29']
        
        region_data.plot(ax=ax, title=f'Kraj: {region}', color=colors, linewidth=2)
        ax.set_xlabel('')
        if i % 2 == 0:
            ax.set_ylabel('Počet nehod')
        else:
            ax.set_ylabel('')
        
    # Odstranění legend a přidání jedné společné
    for ax in axs.flatten():
        ax.get_legend().remove()
    handles, labels = axs[0, 0].get_legend_handles_labels()
    fig.legend(handles, labels, loc='center right', bbox_to_anchor=(0.8, 0.5), title='Druh srážky', ncol=1)
    
    plt.suptitle('Počet nehod v jednotlivých regionech podle druhu srážky', fontsize=16, x=0.4)
    fig.tight_layout(rect=[0, 0, 0.64, 1])
    
    if fig_location:
        plt.savefig(fig_location, bbox_inches='tight')
    if show_figure:
        plt.show()
    plt.close(fig)


if __name__ == "__main__":
    # zde je ukazka pouziti, tuto cast muzete modifikovat podle libosti
    # skript nebude pri testovani pousten primo, ale budou volany konkreni
    # funkce.
    
    df = load_data("data_23_24.zip", "nehody")
    df_consequences = load_data("data_23_24.zip", "nasledky")
    df2 = parse_data(df, True)

    plot_state(df2, "01_state.png")
    plot_alcohol(df2, df_consequences, "02_alcohol.png", True)
    plot_type(df2, "03_type.png")


# Poznamka:
# pro to, abyste se vyhnuli castemu nacitani muzete vyuzit napr
# VS Code a oznaceni jako bunky (radek #%%% )
# Pak muzete data jednou nacist a dale ladit jednotlive funkce
# Pripadne si muzete vysledny dataframe ulozit nekam na disk (pro ladici
# ucely) a nacitat jej naparsovany z disku