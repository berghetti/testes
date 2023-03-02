#!/bin/python3

import charts
import csv


def get_data(file, tr, l999):
  with open(file, newline='') as f:
    reader = csv.DictReader(f)
    for row in reader:
      tr.append(float(row['Throughput']) / 1e6)
      l999.append(int(float(row['99.9% Tail Latency'])) / 1000)


dfcfs_troughput = []
dfcfc_999 = []
get_data('policys/dfcfs.csv', dfcfs_troughput, dfcfc_999)

persephone_troughput = []
persephone_999 = []

get_data('policys/persephone.csv', persephone_troughput, persephone_999)

fap_troughput = []
fap_999 = []
get_data('policys/fap_simple.csv', fap_troughput, fap_999)

dfcfs = {
    'x': dfcfs_troughput,
    'y': dfcfc_999,
    'label': 'd-FCFS',
    'color': 'red',
    'linestyle': '-',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}

persephone = {
    'x': persephone_troughput,
    'y': persephone_999,
    'label': 'Persephone',
    'color': 'green',
    'linestyle': '-',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}

fap = {
    'x': fap_troughput,
    'y': fap_999,
    'label': 'A.F.P',
    'color': 'blue',
    'linestyle': '-',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}
config = {

    'datasets': [dfcfs, persephone, fap],
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
        'ymajor': 50,
        'xminor': 0.5,
        'yminor': 25,
    },

    'legend': {
        'loc': 'best',
        #'title': 'Políticas',
        'title_fontsize' : 12,
        'fontsize': 10,
    },

    'ylim': [0, 500],
    # 'xlim': [min(overhead), 220],  # max(overhead) + 10],
    'save': 'policys_comparation.pdf',
    'show': 'y'
}

c = charts.line(config)
