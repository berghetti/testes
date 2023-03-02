#!/bin/python3

import matplotlib.pyplot as plt
from matplotlib.ticker import (MultipleLocator, AutoMinorLocator)

# plt.style.use('seaborn-v0_8-dark-palette')


def font(v):
  plt.rcParams.update(v)


class chart:
  def __init__(self):

    self.fig, self.ax = plt.subplots(1, 1, dpi=200)

  def datasets(self, v):
    pass

  def set_ticks(self, v):
    if v['xmajor']:
      self.ax.xaxis.set_major_locator(MultipleLocator(v['xmajor']))

    if v['ymajor']:
      self.ax.yaxis.set_major_locator(MultipleLocator(v['ymajor']))

    if v['xminor']:
      self.ax.xaxis.set_minor_locator(MultipleLocator(v['xminor']))

    if v['yminor']:
      self.ax.yaxis.set_minor_locator(MultipleLocator(v['yminor']))

  def xlabel(self, v):
    self.ax.set_xlabel(v)

  def ylabel(self, v):
    self.ax.set_ylabel(v)

  def xlim(self, v):
    self.ax.set_xlim(self.config['xlim'])

  def ylim(self, v):
    self.ax.set_ylim(self.config['ylim'])

  def grid(self, v):
    self.ax.grid(True,
                 which=v['which'],
                 color=v['color'],
                 linestyle=v['linestyle'],
                 linewidth=v['linewidth'])

  def legend(self, v):
    plt.legend(**v)

  def save(self, v):
    self.fig.savefig(self.config['save'], bbox_inches='tight')

  def show(self, v):
    if v == 'y' or v == 'Y':
      plt.tight_layout()
      plt.show()


class line(chart):
  def __init__(self, config):
    self.config = config
    if config['font']:
      font(config['font'])
      del config['font']
    super().__init__()

    # call all functions in config dict
    for func, value in self.config.items():
      if not value:
        continue
      # print(func)
      try:
        f = getattr(self, func)
        f(value)
      except:
        print('Error ' + func)
        pass

  def datasets(self, v):
    for dataset in v:
      self.ax.plot(dataset['x'], dataset['y'],
                   label=dataset['label'],
                   c=dataset['color'],
                   ls=dataset['linestyle'],
                   lw=dataset['linewidth'],
                   marker=dataset['marker'],
                   ms=dataset['markersize'])
