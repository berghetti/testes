#!/bin/python3

import charts
import csv


def get_data(file, tr, short, long):
  with open(file, newline='') as f:
    reader = csv.DictReader(f)
    for row in reader:
      tr.append(float(row['Throughput']) / 1e6)
      short.append(float(row['latency_short 99.9%']) / 1e3)
      long.append(float(row['latency_long 99.9%']) / 1e3)


persephone_troughput = []
persephone_short = []
persephone_long = []
get_data('policys/persephone.csv', persephone_troughput,
         persephone_short, persephone_long)

fap_troughput = []
fap_short = []
fap_long = []
get_data('policys/fap_simple.csv', fap_troughput, fap_short, fap_long)

persephone_s = {
    'x': persephone_troughput,
    'y': persephone_short,
    'label': 'Curtas (Persephone)',
    'color': 'green',
    'linestyle': '-',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}

persephone_l = {
    'x': persephone_troughput,
    'y': persephone_long,
    'label': 'Longas (Persephone)',
    'color': 'green',
    'linestyle': '--',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}

fap_s = {
    'x': fap_troughput,
    'y': fap_short,
    'label': 'Curtas (F.A.P)',
    'color': 'blue',
    'linestyle': '-',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}

fap_l = {
    'x': fap_troughput,
    'y': fap_long,
    'label': 'Longas (F.A.P)',
    'color': 'blue',
    'linestyle': '--',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}
config = {
    'datasets': [persephone_s, persephone_l, fap_s, fap_l],
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
        'loc': 'best',
        'title': 'Requisições',
        'title_fontsize' : 12,
        'fontsize': 10,
    },

    'ylim': [0, 500],
    # 'xlim': [min(overhead), 220],  # max(overhead) + 10],
    'save': 'policys_short_long.png',
    'show': 'y'
}

c = charts.line(config)
