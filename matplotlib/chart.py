#!/bin/python3

import matplotlib.pyplot as plt
from matplotlib.ticker import (MultipleLocator, AutoMinorLocator)

#plt.style.use('seaborn-v0_8-dark-palette')

class chart:
    def __init__(self):
        self.fig, self.ax = plt.subplots(1, 1, dpi=200)

    def plot(self):
        pass

    def set_x_major_tick(self, v):
        self.ax.xaxis.set_major_locator(MultipleLocator(v))

    def set_y_major_tick(self, v):
        self.ax.yaxis.set_major_locator(MultipleLocator(v))

    def set_x_minor_tick(self, v):
        self.ax.xaxis.set_minor_locator(MultipleLocator(v))

    def set_y_minor_tick(self, v):
        self.ax.yaxis.set_minor_locator(MultipleLocator(v))


    def show(self):
        plt.tight_layout()
        plt.show()


class line(chart):
    def __init__(self, config):
        super().__init__()
        self.config = config
        self.datasets = config['datasets']
        self.grid = config['grid']

    def plot(self):
        self.ax.set_xlabel(config['xlabel'])
        self.ax.set_ylabel(config['ylabel'])

        self.ax.set_ylim(self.config['ylim'])
        self.ax.set_xlim(self.config['xlim'])

        self.ax.grid(True,
                     which=self.grid['which'],
                     color=self.grid['color'],
                     linestyle=self.grid['linestyle'],
                     linewidth=self.grid['linewidth'])

        #self.ax.grid()
        for dataset in self.datasets:
            self.ax.plot(dataset['x'], dataset['y'],
                         label=dataset['label'],
                         c=dataset['color'],
                         ls=dataset['linestyle'],
                         lw=dataset['linewidth'],
                         marker=dataset['marker'],
                         ms=dataset['markersize'])


        plt.legend(loc='best')

persephone_overhead = [
    100,
    110,
    120,
    130,
    140,
    150,
    160,
    170,
    180,
    190,
    200,
    210,
    220,
    230,
    240,
    250
]

persephone_short_requests = [
6459,
6780,
8240,
6687,
9031,
5476,
5019,
5133,
8149,
19462,
385703,
4943322,
9321835,
13090722,
16891300,
20108280
]


persephone_long_requests = [
130080,
147254,
156784,
132249,
157379,
130730,
132724,
121510,
147269,
149056,
485202,
5029937,
9413319,
13151323,
16974862,
20167409
]

#convert to microseconds
persephone_short_requests = [v / 1000 for v in persephone_short_requests]
persephone_long_requests = [v / 1000 for v in persephone_long_requests]

#https://matplotlib.org/stable/api/_as_gen/matplotlib.axes.Axes.plot.html
latency_short = {
    'x' : persephone_overhead,
    'y' : persephone_short_requests,
    'label' : 'Requisições curtas',
    'color' : 'purple',
    'linestyle' : '-',
    'linewidth' : 2.0,
    'marker': 'o',
    'markersize' : 5.0
}

latency_long = {
    'x' : persephone_overhead,
    'y' : persephone_long_requests,
    'label' : 'Requisições longas',
    'color' : 'orange',
    'linestyle' : '-',
    'linewidth' : 2.0,
    'marker': 'o',
    'markersize' : 5.0
}

config = {
    'datasets' : [ latency_short, latency_long ],
    'xlabel' : 'Sobrecarga em nanossegundos',
    'ylabel' : 'latência 99,9% (us)',

    'grid' : {
        'which' : 'major',
        'color': '#ccc',
        'linestyle' : '-',
        'linewidth' : 0.2
    },

    'ylim' : [0, 500],
    'xlim' : [min(persephone_overhead) - 10, max(persephone_overhead) + 10]
}

c = line( config )
c.set_x_major_tick(10)
c.set_y_major_tick(50)
c.plot()
c.show()
