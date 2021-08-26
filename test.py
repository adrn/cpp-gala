# from cpp_gala._potential import KeplerPotential, BasePotential
# import numpy as np

# pot = KeplerPotential(3)
# print(pot.get_ndim())

# print(issubclass(KeplerPotential, BasePotential))
# print(isinstance(pot, BasePotential))

# q = np.array([[1., 2., 3.]])
# print(pot.density(q, 0.))

import gala.potential as gp

gala_pot = gp.KeplerPotential(1.)

from cpp_gala._potential import KeplerPotential, StaticPotentialParameter
import numpy as np
import time

M = StaticPotentialParameter(1.)
pot = KeplerPotential(1., M, 3)
print(pot.get_ndim())

rng = np.random.default_rng(42)
# q = np.array([[1., 2., 3.]])
# q = rng.random(size=(2, 3))
q = rng.random(size=(100_000, 3))
ntrials = 1

print("q", q)
print("r", np.sqrt(np.sum(q**2, axis=1)))

for funcname in ['density', 'energy', 'gradient']:
    print(funcname)
    cpp_vals = getattr(pot, funcname)(q, 0.)
    gala_vals = getattr(gala_pot, '_' + funcname)(q, np.array([0.]))
    print("max. difference:", np.max(np.abs(cpp_vals - gala_vals)), "\n")

    t0 = time.time()
    for n in range(ntrials):
        getattr(pot, funcname)(q, 0.)
    cpp_time = (time.time() - t0) / ntrials

    t0 = time.time()
    for n in range(ntrials):
        getattr(gala_pot, '_' + funcname)(q, np.array([0.]))
    gala_time = (time.time() - t0) / ntrials

    print(f"{cpp_time=}\n{gala_time=}\n")
