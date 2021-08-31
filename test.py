# Test static potential parameter
# from cpp_gala._potential import (KeplerPotential,
#                                  StaticPotentialParameter)
# import numpy as np

# M = StaticPotentialParameter(1.)
# q0 = np.array([0., 0, 0])
# pot = KeplerPotential(1., M, 3, q0=q0)
# # pot = KeplerPotential(1., M, 3)
# print(pot.n_dim, pot.G, pot.q0)

# q = np.array([[2., 0., 0.]])
# print('density', pot.density(q, 0.))
# print('density 2', pot.density(q, 0.))
# print('density 3', pot.density(q, 0.))
# print('gradient', pot.gradient(q, 0.))
# print('gradient 2', pot.gradient(q, 0.))
# print('gradient 3', pot.gradient(q, 0.))
# print('acceleration', pot.acceleration(q, 0.))

# print("final q0", pot.q0)

# import sys
# sys.exit(0)


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
# print(pot.get_n_dim())

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
# print(pot.get_n_dim())

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
# from cpp_gala._simulation import Simulation
# sim = Simulation()

# ---

# from cpp_gala._potential import (KeplerPotential,
#                                  StaticPotentialParameter)
# from cpp_gala._simulation import Simulation, BodyCollection
# import numpy as np

# ext_M = StaticPotentialParameter(1.)
# ext_pot = KeplerPotential(1., ext_M)

# M = StaticPotentialParameter(1.)
# pot = KeplerPotential(1., M)

# # sim = Simulation(ext_pot)
# sim = Simulation()

# # body1 = BodyCollection(
# #     np.array([[1., 0., 0., 0, 0, 0],
# #               [2., 0, 0, 0, 0, 0]]),
# #     potential=pot,
# #     # potential=None,
# #     name="derp1"
# # )

# # body2 = BodyCollection(
# #     np.array([[-1., 0., 0., 0, 0, 0],
# #               [-2., 0, 0, 0, 0, 0]]),
# #     # potential=pot,
# #     potential=None,
# #     name="derp2"
# # )

# body1_w = np.array([[1., 0., 0., 0, 0, 0],
#                     [2., 0, 0, 0, 0, 0]])
# body1 = BodyCollection(
#     body1_w,
#     potential=pot,
#     # potential=None,
#     name="derp1"
# )

# body2_w = np.array([[-1., 0., 0., 0, 0, 0],
#                     [-2., 0, 0, 0, 0, 0]])
# body2 = BodyCollection(
#     body2_w,
#     potential=pot,
#     # potential=None,
#     name="derp2"
# )

# sim.add_body(body1)
# sim.add_body(body2)

# print(sim.n_bodies)
# print(sim.acceleration(body1, 0.))

# import sys
# sys.exit(0)

# -----------------------------------------------------------------------------
# Integrators
#


# -----------------------------------------------------------------------------
# Simulation desired API
#
# from cpp_gala._potential import (KeplerPotential,
#                                  StaticPotentialParameter)
# from cpp_gala._simulation import Simulation, BodyCollection
# import numpy as np

# M = StaticPotentialParameter(1.)
# external_pot = KeplerPotential(1., M)

# """
# Notes:
# - TestParticle() and BodyCollection() can actually be array-like
# """
# sim = Simulation(potential=external_pot)
# sim.add_body(BodyCollection())  # test particle
# sim.add_body(BodyCollection(), name='Sag')  # test particle, named
# sim.add_body(BodyCollection(potential=KeplerPotential()))  # massive body
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
# print(pot.get_n_dim())
