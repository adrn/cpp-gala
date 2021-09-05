#ifndef _GALA_FORCE_H
#define _GALA_FORCE_H

#include <string>
#include <gala/potential/potential.h>
#include <gala/potential/particle.h>

namespace gala { namespace simulation {

class Force {
    /*
    Parameters
    ----------
    massless : bool
        Short-circuit flag for massless particles.
    n_dim : int
        Phase-space dimensionality (e.g., 3 for 3D positions / 6D phase-space).
    m_n_particles : int
        The number of particles represented by this object.
    w :
        The instantaneous phase-space coordinates of the ptcl.
    */
    public:
        // Attributes:

        // Methods:
        void get_acceleration(Particle *ptcl, double *w, double t);

};

}} // namespace: gala::simulation

#endif
