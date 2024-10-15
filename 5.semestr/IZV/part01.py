#!/usr/bin/env python3
"""
IZV cast1 projektu
Autor: xmicha94

Detailni zadani projektu je v samostatnem projektu e-learningu.
Nezapomente na to, ze python soubory maji dane formatovani.

Muzete pouzit libovolnou vestavenou knihovnu a knihovny predstavene na prednasce
"""
from bs4 import BeautifulSoup
import requests
import numpy as np
from numpy.typing import NDArray
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter
from typing import List, Callable, Dict, Any
from math import sin


def distance(a: np.array, b: np.array) -> np.array:
    distance = np.sqrt(np.sum(np.power(b - a, 2), axis=1))   
    return distance

def generate_graph(a: List[float], show_figure: bool = False, save_path: str | None = None):
    x = np.linspace(0, 6 * np.pi, 1000)

    ax = plt.gca()

    for coeff in a:
        plt.plot(x, coeff * np.sin(x), label=f"$y_{coeff}(x)$")
        plt.fill_between(x, coeff * np.sin(x), alpha=0.1)

    ax.xaxis.set_major_formatter(plt.FuncFormatter(format_func))

    # Customize the plot
    plt.xlabel("X")
    plt.ylabel("$f_{a}(x)$")
    ax.legend(loc="upper center", bbox_to_anchor=(0.5, 1.15), ncol=3)

    # Save the figure if a path is provided
    if save_path:
        plt.savefig(save_path)  # Save the figure to the specified path

    # Show the figure if requested
    if show_figure:
        plt.show()  # Display the graph

    plt.close()  # Close the plot to free up memory


def generate_sinus(show_figure: bool = False, save_path: str | None = None):
    pass


def download_data() -> Dict[str, List[Any]]:
    pass


def format_func(value, tick_number):
    N = int(np.round(2 * value / np.pi))
    
    if N >= 0 and N < 6:
        return {0: "0", 1: r"$\frac{\pi}{2}$", 2: r"$\pi$"}.get(N, "")
    
    elif N % 2 > 0:  # For odd N
        return f"${N} \\frac{{\\pi}}{{2}}$"  # Properly escaped for mathtext
    
    else:  # For even N
        return f"${N // 2} \\pi$"  # Properly escaped for mathtext


if __name__ == "__main__":
    generate_graph([7, 4, 3])
    generate_sinus()
