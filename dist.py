
from random import *
import matplotlib.pyplot as plt


def bimodal(n):
    return randrange(0, n) == 0

MAX_VALUES = 10000

X = 100
values = []

for i in range(MAX_VALUES):
    if (bimodal(200)):
        values.append( X * 500.5 )
    else:
        values.append( X * 0.1 )

print(values)

plt.plot([i for i in range(MAX_VALUES)], values)
plt.show()
