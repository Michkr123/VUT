#!/usr/bin/env python3
"""
IZV Project Part 1
Author: xmicha94

This script contains the functions required to download data from a specific URL,
parse the information, and generate visual graphs based on mathematical functions.
Detailed project requirements are available on the e-learning platform.

You may use any standard library or libraries introduced in lectures.
"""

from bs4 import BeautifulSoup
import requests
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter
from typing import List, Dict, Any

def distance(a: np.ndarray, b: np.ndarray) -> np.ndarray:
    """
    Spocita euklidovskou vzdalenost mezi dvěma body.
    Arumenty:
        a (np.ndarray): První pole bodů.
        b (np.ndarray): Druhé pole bodů.
    Návratová hodnota:
        np.ndarray: Euklidovská vzdálenost mezi body.
    """
    return np.sqrt(np.sum(np.power(b - a, 2), axis=1))


def generate_graph(a: List[float], show_figure: bool = False, save_path: str | None = None):
    """
    Generování sinusových grafů s různými koeficienty.
    Argumenty:
        a (List[float]): List koeficientů.
        show_figure (bool): Pravda - graf bude zobrazen, nepravda - graf nebude zobrazen.
        save_path (str, optional): Cesta pro uložení obrázu. Není-li daná, obrázek se neuloží.
    """
    x = np.linspace(0, 6 * np.pi, 1000)
    a = np.array(a).reshape(-1, 1)
    y_values = np.power(a, 2) * np.sin(x)

    ax = plt.gca()
    ax.xaxis.set_major_formatter(FuncFormatter(format_func))
    ax.set_xticks(np.arange(0, 6.5 * np.pi, 0.5 * np.pi))
    ax.set_xlim(0, 6 * np.pi)
    ax.set_yticks([-40, -20, 0, 20, 40])

    for i, coeff in enumerate(a.flatten()):
        ax.plot(x, y_values[i], label=f"$y_{int(coeff)}(x)$")
        ax.fill_between(x, y_values[i], alpha=0.1)

    plt.xlabel("X")
    plt.ylabel("$f_{a}(x)$")
    ax.legend(loc="upper center", bbox_to_anchor=(0.5, 1.15), ncol=3)

    if save_path:
        plt.savefig(save_path)

    if show_figure:
        plt.show()

    plt.close()


def generate_sinus(show_figure: bool = False, save_path: str | None = None):
    """
    Pokročilé generování sinusových signálů.
    Argumenty:
        show_figure (bool): Pravda - graf bude zobrazen, false - graf nebude zobrazen.
        save_path (str, optional): Cesta pro uložení obrázu. Není-li dána, obrázek se neuloží.
    """
    x = np.linspace(0, 100, 5000)
    fig, ax = plt.subplots(3, 1, figsize=(10, 8))
    y1 = 0.5 * np.cos(np.pi * x / 50)
    y2 = 0.25 * (np.sin(np.pi * x) + np.sin(1.5 * np.pi * x))
    y3 = y1 + y2

    xticks = np.arange(0, 101, 25)
    yticks = np.arange(-0.8, 0.81, 0.4)

    ax[0].plot(x, y1, color='b')
    ax[0].set_xlim(0, 100)
    ax[0].set_xticks(xticks)
    ax[0].set_ylim(-0.8, 0.8)
    ax[0].set_yticks(yticks)
    ax[0].set_ylabel("$f_{1}(t)$")

    ax[1].plot(x, y2, color='b')
    ax[1].set_xlim(0, 100)
    ax[1].set_xticks(xticks)
    ax[1].set_ylim(-0.8, 0.8)
    ax[1].set_yticks(yticks)
    ax[1].set_ylabel("$f_{2}(t)$")

    for i in range(len(x) - 1):
        color = 'green' if y3[i] >= y1[i] else 'red' if x[i] <= 50 else 'orange'
        ax[2].plot(x[i:i+2], y3[i:i+2], color=color)

    ax[2].set_xlim(0, 100)
    ax[2].set_xticks(xticks)
    ax[2].set_ylim(-0.8, 0.8)
    ax[2].set_yticks(yticks)
    ax[2].set_ylabel("$f_{1}(t) + f_{2}(t)$")

    if save_path:
        plt.savefig(save_path)

    if show_figure:
        plt.show()

    plt.close()


def download_data() -> Dict[str, List[Any]]:
    """
    Stažení tabulky z dané URL.
    Návratová hodnota:
        Dict[str, List[Any]]: Slovník se seznamem míst, zeměpisnou šířkou, délkou a nadmořskou výškou.
    """
    url = "https://ehw.fit.vutbr.cz/izv/st_zemepis_cz.html"
    response = requests.get(url)
    response.encoding = 'utf-8'
    response.raise_for_status()
    page_content = response.text

    soup = BeautifulSoup(page_content, "html.parser")
    tables = soup.body.find_all("table")
    if len(tables) < 2:
        raise ValueError("Less than two tables found in the body.")

    rows = tables[1].find_all("tr")
    data = {"positions": [], "lats": [], "longs": [], "heights": []}

    for row in rows[1:]:
        cells = row.find_all("td")
        if len(cells) >= 7:
            position = cells[0].get_text(strip=True)
            lat = float(cells[2].get_text(strip=True).replace(",", ".").replace("°", ""))
            long = float(cells[4].get_text(strip=True).replace(",", ".").replace("°", ""))
            height = float(cells[6].get_text(strip=True).replace(",", "."))

            data["positions"].append(position)
            data["lats"].append(lat)
            data["longs"].append(long)
            data["heights"].append(height)

    return data


def format_func(value: float, tick_number: int) -> str:
    """
    Formátovací funkce pro označení osy x pro násobky pí.
    Argumenty:
        value (float): Hodnota na ose X.
        tick_number (int): Index značky.

    Návratová hodnota:
        str: Formátovaný řetězec pro označení dané značky.
    """
    N = int(np.round(2 * value / np.pi))
    if 0 <= N < 3:
        return {0: "0", 1: rf"$\frac{{{N}}}{{2}}\pi$", 2: r"$\pi$"}[N]
    return rf"${N // 2}\pi$" if N % 2 == 0 else rf"$\frac{{{N}}}{{2}}\pi$"


if __name__ == "__main__":
    generate_graph([7, 4, 3])
    generate_sinus()
