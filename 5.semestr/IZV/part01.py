#!/usr/bin/env python3
"""
IZV cast1 projektu
Autor: 

Detailni zadani projektu je v samostatnem projektu e-learningu.
Nezapomente na to, ze python soubory maji dane formatovani.

Muzete pouzit libovolnou vestavenou knihovnu a knihovny predstavene na prednasce
"""
from bs4 import BeautifulSoup
import requests
import numpy as np
from numpy.typing import NDArray
import matplotlib.pyplot as plt
from typing import List, Callable, Dict, Any


def distance(a: np.array, b: np.array) -> np.array:
    distance = np.sqrt(np.sum(np.power(b - a, 2), axis=1))   
    return distance

def generate_graph(a: List[float], show_figure: bool = False, save_path: str | None = None):
    x = np.linspace(-10, 10, 100)  # Define the range of x values

    # Generate and plot each function defined by the coefficients in 'a'
    for coeff in a:
        y = coeff * x  # Simple linear function y = ax for each coefficient 'a'
        plt.plot(x, y, label=f"y = {coeff}x")

    # Customize the plot
    plt.title("Generated Graphs")
    plt.xlabel("X-axis")
    plt.ylabel("Y-axis")
    plt.axhline(0, color='black',linewidth=0.5, ls='--')
    plt.axvline(0, color='black',linewidth=0.5, ls='--')
    plt.grid(color = 'gray', linestyle = '--', linewidth = 0.5)
    plt.legend()

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


if __name__ == "__main__":
    generate_graph([7, 4, 3])
    generate_sinus()
