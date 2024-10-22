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

    a = np.array(a).reshape(-1, 1)
    y_values = np.power(a, 2) * np.sin(x)

    ax = plt.gca()
    ax.xaxis.set_major_formatter(FuncFormatter(format_func))

    xticks = np.arange(0, 6.5 * np.pi, 0.5 * np.pi)
    ax.set_xticks(xticks)
    ax.set_xlim(0,6*np.pi)
    yticks = np.arange(-50, 50)
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
    x = np.linspace(0, 100, 1000)
    fig, ax = plt.subplots(3, 1, figsize=(10, 8))
    y1 = 0.5 * np.cos(np.pi * x / 50)
    y2 = 0.25 * (np.sin(np.pi * x) + np.sin(3/2 * np.pi * x))
    y3 = y1 + y2

    xticks = np.arange(0, 101, 25)
    yticks = np.arange(-0.8, 0.81, 0.4)


    # First subplot: Sine function
    ax[0].plot(x, y1, label='Sine', color='b')
    ax[0].set_xlim(0,100)
    ax[0].set_xticks(xticks)
    ax[0].set_ylim(-0.8, 0.8)
    ax[0].set_yticks(yticks)
    ax[0].tick_params(axis='x', labelbottom=False)
    ax[0].set_ylabel("$f_{1}(t)$")

    # Second subplot: Cosine function
    ax[1].plot(x, y2, label='Cosine', color='b')
    ax[1].set_xlim(0,100)
    ax[1].set_xticks(xticks)
    ax[1].set_ylim(-0.8, 0.8)
    ax[1].set_yticks(yticks)
    ax[1].tick_params(axis='x', labelbottom=False)
    ax[1].set_ylabel("$f_{2}(t)$")



    # Third subplot: Tangent function
    ax[2].plot(x, y3, label='Tangent', color='g')
    ax[2].set_xlim(0,100)
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
    pass

def format_func(value, tick_number):
    N = int(np.round(2 * value / np.pi))
    if N>=0 and N < 3:
        return {0:"0", 1:f"$\\frac{{{N}}}{{2}}\pi$", 2: r"$\pi$"}[N]
    elif N % 2 > 0:
        return f"$\\frac{{{N}}}{{2}}\pi$"

    else:
        return f"${N // 2}\pi$"

if __name__ == "__main__":
    generate_graph([7, 4, 3])
    generate_sinus()
