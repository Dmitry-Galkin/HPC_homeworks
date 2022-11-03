""" Расчет числа e. """

from time import time
from numba import njit
import ctypes


def calculation_e_simple(n_iter, N=100):
    """
    Специально расчет факториала не оптимизирован, 
    чтобы суммарный расчет времени побольше занимал.
    """
    
    exp = 0
    
    for _ in range(N):
        e = 0
        for n in range(n_iter):
            f = 1
            for i in range(2, n + 1):
                f *= i
            e += 1 / f
        exp += e
    return exp / N


@njit(nogil=True, cache=True)
def calculation_e_njit(n_iter, N=100):
    """
    Специально расчет факториала не оптимизирован, 
    чтобы суммарный расчет времени побольше занимал.
    """
    
    exp = 0
    
    for _ in range(N):
        e = 0
        for n in range(n_iter):
            f = 1
            for i in range(2, n + 1):
                f *= i
            e += 1 / f
        exp += e
    return exp / N
    
    
def calculation_e_ctypes_simple(n_iter, N=100):
	clibE = ctypes.CDLL("./libE.so")
	clibE.calculation_e_simple.restype = ctypes.c_double
	e = clibE.calculation_e_simple(ctypes.c_int(n_iter), ctypes.c_int(N))
	return e
	
	
def calculation_e_ctypes_omp(n_iter, N=100):
	clibE = ctypes.CDLL("./libE.so")
	clibE.calculation_e_omp.restype = ctypes.c_double
	e = clibE.calculation_e_omp(ctypes.c_int(n_iter), ctypes.c_int(N))
	return e


n_iter = 60
N = 500_000

t = time()
e = calculation_e_simple(n_iter, N)
print(f"simple method -> e = {e}, calculation time: {round(time() - t, 5)} s")

t = time()
e = calculation_e_njit(n_iter, N)
print(f"njit method (1) -> e = {e}, calculation time: {round(time() - t, 5)} s")

t = time()
e = calculation_e_njit(n_iter, N)
print(f"njit method (2) -> e = {e}, calculation time: {round(time() - t, 5)} s")

t = time()
e = calculation_e_ctypes_simple(n_iter, N)
print(f"ctypes method simple -> e = {e}, calculation time: {round(time() - t, 5)} s")

t = time()
e = calculation_e_ctypes_omp(n_iter, N)
print(f"ctypes method with omp -> e = {e}, calculation time: {round(time() - t, 5)} s")
    
