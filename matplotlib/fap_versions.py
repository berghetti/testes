#!/bin/python3

import charts
import csv


def get_data(file, tr, buff):
  with open(file, newline='') as f:
    reader = csv.DictReader(f)
    for row in reader:
      tr.append(float(row['Throughput']) / 1e6)
      # short.append(float(row['latency_short 99.9%']) / 1e3)
      # long.append(float(row['latency_long 99.9%']) / 1e3)
      buff.append(float(row['99.9% Tail Latency']) / 1e3)


fap_troughput = []
fap_999 = []
get_data('policys/fap_simple.csv', fap_troughput, fap_999)

fap2_troughput = []
fap2_999 = []
get_data('policys/fap_vcheck.csv', fap2_troughput, fap2_999)

fap = {
    'x': fap_troughput,
    'y': fap_999,
    'label': 'F.A.P',
    'color': 'green',
    'linestyle': '-',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}

fap_2 = {
    'x': fap2_troughput,
    'y': fap2_999,
    'label': 'F.A.P 2',
    'color': 'blue',
    'linestyle': '--',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}

config = {
    'datasets': [fap, fap_2],
    'xlabel': 'Throughput (MRPS)',
    'ylabel': 'Latência 99,9% (us)',

    'font': {
        'font.size':15,
        'axes.labelsize': 15,
        'axes.titlesize': 15,
        'xtick.labelsize': 15,
        'ytick.labelsize': 15,
    },

    'grid': {
        'which': 'major',
        'color': '#ccc',
        'linestyle': '-',
        'linewidth': 0.2
    },

    'set_ticks': {
        'xmajor': 1,
        'xminor': 0.5,
        'ymajor': 50,
        'yminor': 25,
    },

    'legend': {
        'loc': 'upper left',
        'title': 'Políticas',
        'title_fontsize' : 12,
        'fontsize': 10,
    },

    'ylim': [0, 500],
    # 'xlim': [min(overhead), 220],  # max(overhead) + 10],
    'save': 'fap_versions.png',
    'show': 'y'
}

c = charts.line(config)
