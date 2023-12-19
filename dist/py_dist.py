#!/bin/python3

from random import *
import numpy as np
from math import sqrt, log

rate = 1000
mean = (1.0/rate) * 10**6
alpha = 1.0 + mean / (mean - 1.0)
xm = mean * (alpha - 1) / alpha

seed(42)

print('alpha: {}, xm: {} mean: {}'.format(alpha, xm, mean))

def my_pareto(alpha):
   """Pareto distribution.  alpha is the shape parameter."""
   # Jain, pg. 495

   u = 1.0 - random()
   return u ** (-1.0 / alpha)

#for _ in range(10):
#    print(paretovariate(alpha) * xm)
#    #print(my_pareto(alpha) * xm)


# Desired request rate (requests per second)
desired_rate = 1000

# Calculate mean and variance of the lognormal distribution for the desired rate
mu = np.log(desired_rate / np.sqrt(1 + (desired_rate ** 2)))
sigma = np.sqrt(np.log(1 + (desired_rate ** 2)) - 2 * np.log(desired_rate))

print(f"Calculated standard deviation (sigma): {sigma:.4f}")
print(f"Calculated mean (mu): {mu:.4f}")
for _ in range(10):
    print(lognormvariate(mu, sigma))

sigma2 = sqrt(2*(log(mean) - log(mean/2)))
mu2 = (log(mean) - (sigma2 * sigma2) / 2)

print()
print(f"Calculated standard deviation (sigma2): {sigma2:.4f}")
print(f"Calculated mean (mu2): {mu2:.4f}")
for _ in range(10):
    print(lognormvariate(mu2, sigma2))
