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

dfcfs_troughput = []
dfcfs_slowdown = []
get_data('policys/dfcfs.csv', dfcfs_troughput, dfcfs_slowdown)

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
    'label': 'A.F.P',
    'color': 'blue',
    'linestyle': '-',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}

dfcfs = {
    'x': dfcfs_troughput,
    'y': dfcfs_slowdown,
    'label': 'd-FCFS',
    'color': 'red',
    'linestyle': '-',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}

config = {
    'datasets': [dfcfs, persephone, fap],
    'xlabel': 'Throughput (MRPS)',
    'ylabel': 'Slowdown 99,9%',

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
        'xminor': None,
        'ymajor': 10,
        'yminor': 5,
    },

    'legend': {
        'loc': 'upper center',
        #'title': 'Requisições',
        'title_fontsize' : 12,
        'fontsize': 10,
    },


    'ylim': [0, 100],
    # 'xlim': [min(overhead), 220],  # max(overhead) + 10],
    'save': 'policys_slowdown.pdf',
    'show': 'y'
}

c = charts.line(config)
