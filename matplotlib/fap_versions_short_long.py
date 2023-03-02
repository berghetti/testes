#!/bin/python3

import charts
import csv


def get_data(file, tr, short, long, sl):
  with open(file, newline='') as f:
    reader = csv.DictReader(f)
    for row in reader:
      tr.append(float(row['Throughput']) / 1e6)
      short.append(float(row['latency_short 99.9%']) / 1e3)
      long.append(float(row['latency_long 99.9%']) / 1e3)
      sl.append(float(row['Slowdown99.9%']))


fap_troughput = []
fap_short = []
fap_long = []
fap_sl = []
get_data('policys/fap_simple.csv', fap_troughput, fap_short, fap_long, fap_sl)

fap2_troughput = []
fap2_long = []
fap2_short = []
fap2_sl = []
get_data('policys/fap_vcheck.csv', fap2_troughput, fap2_short, fap2_long, fap2_sl)

fap_s = {
    'x': fap_troughput,
    'y': fap_short,
    'label': 'Curtas (F.A.P)',
    'color': 'green',
    'linestyle': '-',
    'linewidth': 2.0,
    'marker': '*',
    'markersize': 5.0
}
fap_l = {
    'x': fap_troughput,
    'y': fap_long,
    'label': 'Longas (F.A.P)',
    'color': 'green',
    'linestyle': '-',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}

fap_sl = {
    'x': fap_troughput,
    'y': fap_sl,
    'label': 'Slowdown (F.A.P)',
    'color': 'red',
    'linestyle': '-',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}

fap2_s = {
    'x': fap2_troughput,
    'y': fap2_short,
    'label': 'Curtas (F.A.P 2)',
    'color': 'blue',
    'linestyle': '--',
    'linewidth': 2.0,
    'marker': '*',
    'markersize': 5.0
}
fap2_l = {
    'x': fap2_troughput,
    'y': fap2_long,
    'label': 'Longas (F.A.P 2)',
    'color': 'blue',
    'linestyle': '--',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}
fap2_sl = {
    'x': fap2_troughput,
    'y': fap2_sl,
    'label': 'Slowdown (F.A.P 2)',
    'color': 'red',
    'linestyle': '--',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}

config = {
    'datasets': [fap_s, fap_l, fap_sl, fap2_s, fap2_l, fap2_sl],
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
    'save': 'fap_versions_short_long.png',
    'show': 'y'
}

c = charts.line(config)
