import SALib
from SALib.sample import saltelli
from SALib.analyze import sobol
from SALib.test_functions import Ishigami
import numpy as np
import time

from math import sin, cos, pi


def evaluate_model(x):
    """Гравитационный момент, действующий на МКС, по оси X (в зависимости от углов ориентации)."""
    
    mu = 398600  # гравитационная постоянная Земли
    
    h = 400  # высота орбиты
    Re = 6371  # радуис Земли
    r = h + Re  # радиус орбиты
    
    # моменты инерции
    Ix = 10276978.
    Iy = 31940398.
    Iz = 40019058.
    Ixy = 1084837.
    Ixz = -597098.
    Iyz = 614081.
    
    gamma, psi, theta = x  # крен, рысканье, тангаж
    
    f = 3 * mu / r**3 * cos(theta) * (
        0.5 * (Iy - Iz) * sin(2 * gamma) * cos(theta) \
        - Iyz * cos(2 * gamma) * cos(theta) \
        - (Ixz * cos(gamma) + Ixy * sin(gamma)) * sin(theta)
    )
  
    return f


problem = {
    "num_vars": 3,
    "names": ['gamma', 'psi', 'theta'],
    "bounds": [
        [-pi, pi],
        [-pi, pi],
        [-pi, pi]
    ]
}

t = time.time()
param_values = saltelli.sample(problem, 65536)
print(f"sample generation: {time.time() - t} s")

Y = np.zeros([param_values.shape[0]])
t = time.time()
for i, X in enumerate(param_values):
    Y[i] = evaluate_model(X)
print(f"model evaluation: {time.time() - t} s")

t = time.time()
Si = sobol.analyze(problem, Y)
print(f"SA: {time.time() - t} s")

print("\nSA results:\n")

n_round = 4
print("S1:", Si["S1"].round(n_round))
print("ST:", Si["ST"].round(n_round))
print("x1-x2:", Si['S2'][0, 1].round(n_round))
print("x1-x3:", Si['S2'][0, 2].round(n_round))
print("x2-x3:", Si['S2'][1, 2].round(n_round))
