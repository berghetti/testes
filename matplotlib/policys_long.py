#!/bin/python3

import charts
import csv


def get_data_throughput(file, buff ):
  with open(file, newline='') as f:
    reader = csv.DictReader(f)
    for row in reader:
      buff.append(float(row['Throughput']) / 1e6)

def get_data_lat(file, buff):
  with open(file, newline='') as f:
    reader = csv.DictReader(f)
    for row in reader:
      buff.append(int(float(row['latency_long 99.9%'])) / 1000)

dfcfs_troughput = []
dfcfs_lat = []
get_data_throughput('policys/dfcfs.csv', dfcfs_troughput,)
get_data_lat('policys/dfcfs.csv', dfcfs_lat)

persephone_troughput = []
persephone_lat = []
get_data_throughput('policys/persephone.csv', persephone_troughput)
get_data_lat('policys/persephone.csv', persephone_lat)

fap_troughput = []
fap_lat = []
get_data_throughput('policys/afp.csv', fap_troughput)
get_data_lat('policys/afp.csv', fap_lat)

dfcfs = {
    'x': dfcfs_troughput,
    'y': dfcfs_lat,
    'label': 'd-FCFS',
    'color': 'red',
    'linestyle': '-',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}

persephone = {
    'x': persephone_troughput,
    'y': persephone_lat,
    'label': 'Persephone',
    'color': 'green',
    'linestyle': '-',
    'linewidth': 2.0,
    'marker': 'o',
    'markersize': 5.0
}

fap = {
    'x': fap_troughput,
    'y': fap_lat,
    'label': 'AFP',
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
        'axes.titlesize': 10,
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
        'xmajor': 2,
        'ymajor': 50,
        'xminor': 1,
        'yminor': 25,
    },

    'legend': {
        'loc': 'best',
        #'title': 'Políticas',
        'title_fontsize' : 12,
        'fontsize': 10,
    },

    'title':{
        #'label': 'Requisições longas (100 us)',
        #'loc': 'center'
    },

    'ylim': [0, 500],
    # 'xlim': [min(overhead), 220],  # max(overhead) + 10],
    'save': 'imgs/policys_long.png',
    #'show': 'y'
}

c = charts.line(config)
