#!/bin/python3

import charts
import csv

overhead = []
short_requests = []
long_requests = []
latency_5 = []
latency_7 = []

with open('persephone_5m.csv', newline='', encoding='utf-8-sig') as f:
  reader = csv.DictReader(f)
  for row in reader:
    overhead.append(int(row['PERSEPHONE_OVERHEAD']))
    latency_5.append(int(float(row['99.9% Tail Latency'])) / 1000)
    short_requests.append(int(row['latency_short 99.9%']) / 1000)
    long_requests.append(int(row['latency_long 99.9%']) / 1000)


with open('persephone_7m.csv', newline='', encoding='utf-8-sig') as f:
  reader = csv.DictReader(f)
  for row in reader:
    # overhead.append(int(float(row['PERSEPHONE_OVERHEAD'])))
    latency_7.append(int(float(row['99.9% Tail Latency'])) / 1000)
    short_requests.append(int(float(row['latency_short 99.9%']) / 1000))
    long_requests.append(int(float(row['latency_long 99.9%']) / 1000))


# https://matplotlib.org/stable/api/_as_gen/matplotlib.axes.Axes.plot.html
latency_5 = {
    'x': overhead,
    'y': latency_5,
    'label': '5 MRPS',
    'color': 'purple',
    'linestyle': '--',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}

latency_7 = {
    'x': overhead,
    'y': latency_7,
    'label': '7 MRPS',
    'color': 'orange',
    'linestyle': '--',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}

config = {
    'datasets': [latency_5, latency_7],
    'xlabel': 'Sobrecarga em nanossegundos',
    'ylabel': 'Latência 99,9% (us)',

    'font': {
        'font.size':15,
        'axes.labelsize': 12,
        'axes.titlesize': 12,
        'xtick.labelsize': 12,
        'ytick.labelsize': 12,
    },

    'grid': {
        'which': 'major',
        'color': '#ccc',
        'linestyle': '-',
        'linewidth': 0.2
    },

    'set_ticks': {
        'xmajor': 10,
        'ymajor': 50,
        'xminor': None,
        'yminor': 25,
    },

    'legend': {
        'loc': 'best',
        #'title': 'Requisições',
        'title_fontsize' : 12,
        'fontsize': 10,
    },

    'ylim': [0, 500],
    'xlim': [min(overhead), 220],  # max(overhead) + 10],
    'save': 'persephone_overhead.pdf',
    'show': 'y'
}

c = charts.line(config)
