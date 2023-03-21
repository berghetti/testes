#!/bin/python3

import charts
import csv
import sys
import os
import math

def interval_confiance( data ):
    Z = 1.96 # nivel de confiança 95%
    avg = sum(data) / len(data)

    sum_ = 0
    for i in range(len(data)):
        sum_ += math.pow( data[i] - avg, 2 )

    desvio = math.sqrt( sum_ / len(data) )
    margin_error = Z * ( desvio / math.sqrt(len(data) ) ) # intervalo de confiança

    avg = round(avg, 4)
    margin_error = round(margin_error, 4)
    return avg, margin_error

def get_throughput(file, buffer):
    with open(file) as f:
        reader = csv.DictReader(f)
        for row in reader:
            buffer.append(float(row['Throughput']) / 1e6)

def get_latency(file, buffer, col=None):
    with open(file, newline='') as f:
        reader = csv.DictReader(f)
        for row in reader:
            lat = int(float(row[col]))
            if lat > 0:
                buffer.append( lat / 1e3)

def rps_in_file_name(f):
    return int(f.split('_')[1])

def process_folder(folder):
    x = []
    y = []
    yerr = []

    files = os.listdir(folder)
    files = sorted(files, key=rps_in_file_name)

    for file in files:
        f = os.path.join(folder, file)
        if not os.path.isfile(f):
            continue

        print('Reading file \'{}\''.format(file))

        tr = []
        get_throughput(f, tr)
        if len(tr) == 0:
            print('Error file {}. Aborting'.format(file))
            exit(1)

        lat = []
        get_latency(f, lat, sys.argv[1])
        if len(lat) == 0: continue

        avg, error = interval_confiance(lat)
        x.append(tr[0])
        y.append(avg)
        yerr.append(error)

    return x, y, yerr

dfcfs_tr, dfcfs_lat, dfcfs_yerr = process_folder('policys/dfcfs')
dfcfs = {
    'x': dfcfs_tr,
    'y': dfcfs_lat,
    'style': {
        'label': 'd-FCFS',
        'color': 'red',
        #'linestyle': '-',
        #'linewidth': 2.0,
        #'marker': 'o',
        #'markersize': 5.0
    },

    'errorbar': {
        'x': dfcfs_tr,
        'y': dfcfs_lat,
        'yerr': dfcfs_yerr,
        #'ecolor': 'black',
        'elinewidth': 0.3,
        'color': 'red',
        'linestyle': '-',
        'linewidth': 1.0,
        'marker': 'o',
        'markersize': 4.0
    },
}

per_tr, per_lat, per_yerr = process_folder('policys/persephone')
persephone = {
    'x': per_tr,
    'y': per_lat,
    'style': {
        'label': 'Perséphone',
        'color': 'green',
        #'linestyle': '-',
        #'linewidth': 2.0,
        #'marker': 'o',
        #'markersize': 5.0
    },

    'errorbar': {
        'x': per_tr,
        'y': per_lat,
        'yerr': per_yerr,
        #'ecolor': 'black',
        'elinewidth': 0.3,
        'color': 'green',
        'linestyle': '-',
        'linewidth': 1.0,
        'marker': 'o',
        'markersize': 4.0
    },
}

afp_tr, afp_lat, afp_yerr = process_folder('policys/afp')
afp = {
    'x': afp_tr,
    'y': afp_lat,
    'style': {
        'label': 'AFP',
        'color': 'blue',
        #'linestyle': '-',
        #'linewidth': 2.0,
        #'marker': 'o',
        #'markersize': 5.0
    },

    'errorbar': {
        'x': afp_tr,
        'y': afp_lat,
        'yerr': afp_yerr,
        #'ecolor': 'black',
        'elinewidth': 0.3,
        'color': 'blue',
        'linestyle': '-',
        'linewidth': 1.0,
        'marker': 'o',
        'markersize': 4.0
    },
}

config = {
    'datasets': [dfcfs,persephone, afp],
    #'datasets': [dfcfs],
    'xlabel': 'Throughput (MRPS)',
    'ylabel': 'Latência 50% (us)',

    'font': {
        'font.size':15,
        'axes.labelsize': 15,
        'axes.titlesize': 10,
        'xtick.labelsize': 15,
        'ytick.labelsize': 15,
    },

    'grid': {
        'visible' : True,
        'which': 'major',
        'style': {
            'color': '#ccc',
            'linestyle': '-',
            'linewidth': 0.5
        }
    },

    'set_ticks': {
        'xmajor': 2,
        'ymajor': 50,
        'xminor': 1,
        'yminor': 25,
    },

    'title':{
        'label': 'Requisições longas (100 us)',
        'loc':'center',
    },

    'legend': {
        'loc': 'best',
        #'title': 'Políticas',
        'title_fontsize' : 12,
        'fontsize': 10,
    },

    'ylim': [0, 500],
    'xlim': [0, 16],
    'save': 'imgs/{}'.format(sys.argv[2]),
    'show': 'y'
}

#name_save = sys.argv[2]

charts.line(config)
