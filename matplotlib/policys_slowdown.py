#!/bin/python3

import charts
import csv


def get_data(file, tr, slowdown):
  with open(file, newline='') as f:
    reader = csv.DictReader(f)
    for row in reader:
      tr.append(float(row['Throughput']) / 1e6)
      slowdown.append(float(row['Slowdown99.9%']))


persephone_troughput = []
persephone_slowdown = []
get_data('policys/persephone.csv', persephone_troughput, persephone_slowdown)

fap_troughput = []
fap_slowdown = []
get_data('policys/fap_simple.csv', fap_troughput, fap_slowdown)

persephone = {
    'x': persephone_troughput,
    'y': persephone_slowdown,
    'label': 'Persephone',
    'color': 'green',
    'linestyle': '-',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}

fap = {
    'x': fap_troughput,
    'y': fap_slowdown,
    'label': 'F.A.P',
    'color': 'blue',
    'linestyle': '-',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}
config = {
    'datasets': [persephone, fap],
    'xlabel': 'Throughput (MRPS)',
    'ylabel': 'Slowdown 99,9%',

    'grid': {
        'which': 'major',
        'color': '#ccc',
        'linestyle': '-',
        'linewidth': 0.2
    },

    'set_ticks': {
        'xmajor': 1,
        'xminor': 0.5,
        'ymajor': 10,
        'yminor': 5,
    },

    'ylim': [0, 50],
    # 'xlim': [min(overhead), 220],  # max(overhead) + 10],
    'save': 'policys_slowdown.png',
    'show': 'y'
}

c = charts.line(config)
