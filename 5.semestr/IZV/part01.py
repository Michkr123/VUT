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
    ax.xaxis.set_major_formatter(FuncFormatter(format_func))

    xticks = np.arange(0, 6.5 * np.pi, 0.5 * np.pi)
    ax.set_xticks(xticks)
    ax.set_xlim(0,6*np.pi)
    yticks = np.arange(-50, 50)
    ax.set_yticks([-40, -20, 0, 20, 40])
    #ax.set_yticks(yticks)


    for coeff in a:
        plt.plot(x, np.power(coeff, 2) * np.sin(x), label=f"$y_{coeff}(x)$")
        plt.fill_between(x, np.power(coeff, 2) * np.sin(x), alpha=0.1)

    plt.xlabel("X")
    plt.ylabel("$f_{a}(x)$")
    ax.legend(loc="upper center", bbox_to_anchor=(0.5, 1.15), ncol=3)

    if save_path:
        plt.savefig(save_path)

    if show_figure:
        plt.show()

    plt.close()


def generate_sinus(show_figure: bool = False, save_path: str | None = None):
    pass


def download_data() -> Dict[str, List[Any]]:
    pass

#def format_func(value, tick_number):
#    N = int(np.round(2 * value / np.pi))
#
#    if (N == 0) :
#        return fr"${N}$"
#    elif (N % 1 == 0):
#        return f"${N}\pi$"
#    else :
#        return f"$\frac{{N}}{2}\pi$"

def format_func(value, tick_number):
    N = int(np.round(2 * value / np.pi))
    if N>=0 and N < 3:
        return {0:"0", 1: r"$\pi/2$", 2: r"$\pi$"}[N]
    elif N % 2 > 0:
        return f"${N}\pi/2$"
    else:
        return f"${N // 2}\pi$"

if __name__ == "__main__":
    generate_graph([7, 4, 3])
    generate_sinus()
