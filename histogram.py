
import numpy as np
import random
import matplotlib.pyplot as plt

values = []
#[values.append(random.expovariate(1/10)) for _ in range(1000)]
[values.append(random.lognormvariate(2,1)) for _ in range(10000)]

#values = [10,20,30,20,10,50,80,100,10,20,30,25,10,40,50]

print(values)

plt.hist(values, bins=10)
plt.show()
