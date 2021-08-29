# Test static potential parameter
# from cpp_gala._potential import (KeplerPotential,
#                                  StaticPotentialParameter)
# import numpy as np

# M = StaticPotentialParameter(1.)
# pot = KeplerPotential(1., M, 3)
# print(pot.get_ndim())

# q = np.array([[1., 2., 3.]])
# print(pot.density(q, 0.))


# -----------------------------------------------------------------------------
# Test interpolated potential parameter
#
# from cpp_gala._potential import (KeplerPotential,
#                                  InterpolatedPotentialParameter)
# import numpy as np

# ts = np.linspace(0, 1., 8)
# # vals = np.ones_like(ts)
# vals = 0.1 * ts**2 + 1.
# M = InterpolatedPotentialParameter(ts, vals, 1)
# pot = KeplerPotential(1., M, 3)
# print(pot.get_ndim())

# q = np.array([[1., 2., 3.]])
# print(pot.energy(q, 0.))

# energy = []
# tts = np.linspace(0, 1, 128)
# for tt in tts:
#     energy.append(pot.energy(q, tt)[0])

# import matplotlib.pyplot as plt
# plt.figure(figsize=(5, 5))
# plt.plot(tts, energy)
# plt.show()

# -----------------------------------------------------------------------------
# Test and time against Gala

# import gala.potential as gp

# gala_pot = gp.KeplerPotential(1.)

# from cpp_gala._potential import KeplerPotential, StaticPotentialParameter
# import numpy as np
# import time

# M = StaticPotentialParameter(1.)
# pot = KeplerPotential(1., M, 3)
# print(pot.get_ndim())

# rng = np.random.default_rng(42)
# # q = np.array([[1., 2., 3.]])
# # q = rng.random(size=(2, 3))
# q = rng.random(size=(100_000, 3))
# ntrials = 1

# print("q", q)
# print("r", np.sqrt(np.sum(q**2, axis=1)))

# for funcname in ['density', 'energy', 'gradient']:
#     print(funcname)
#     cpp_vals = getattr(pot, funcname)(q, 0.)
#     gala_vals = getattr(gala_pot, '_' + funcname)(q, np.array([0.]))
#     print("max. difference:", np.max(np.abs(cpp_vals - gala_vals)), "\n")

#     t0 = time.time()
#     for n in range(ntrials):
#         getattr(pot, funcname)(q, 0.)
#     cpp_time = (time.time() - t0) / ntrials

#     t0 = time.time()
#     for n in range(ntrials):
#         getattr(gala_pot, '_' + funcname)(q, np.array([0.]))
#     gala_time = (time.time() - t0) / ntrials

#     print(f"{cpp_time=}\n{gala_time=}\n")

# -----------------------------------------------------------------------------
# Simulation tmp testing
#
# TODO: Note that this doesn't work on its own and I don't know why...
# from cpp_gala._simulation import Simulation  # , Body
# sim = Simulation()

# ---

from cpp_gala._potential import (KeplerPotential,
                                 StaticPotentialParameter)
from cpp_gala._simulation import Simulation, Body
import numpy as np

M = StaticPotentialParameter(1.)
pot = KeplerPotential(1., M)

# sim = Simulation(pot)
sim = Simulation()

# body = Body(pot, np.array([[1., 2., 3.]]))
body = Body(pot, np.array([[1., 2., 3.]]), "derp")
print(body.name, body.ndim)

sim.add_body(body)


# -----------------------------------------------------------------------------
# Simulation desired API
#
# from cpp_gala._potential import (KeplerPotential,
#                                  StaticPotentialParameter)
# from cpp_gala._simulation import Simulation, Body
# import numpy as np

# M = StaticPotentialParameter(1.)
# external_pot = KeplerPotential(1., M)

# """
# Notes:
# - TestParticle() and Body() can actually be array-like
# """
# sim = Simulation(potential=external_pot)
# sim.add_body(Body())  # test particle
# sim.add_body(Body(), name='Sag')  # test particle, named
# sim.add_body(Body(potential=KeplerPotential()))  # massive body
# # sim.add_body(MockStream())
# state = sim.run()  # TODO: snapshot options, save_timesteps, ...

# state[0]
# state[1]
# state['Sag']


# ts = np.linspace(0, 1., 8)
# # vals = np.ones_like(ts)
# vals = 0.1 * ts**2 + 1.
# M = InterpolatedPotentialParameter(ts, vals, 1)
# pot = KeplerPotential(1., M, 3)
# print(pot.get_ndim())
