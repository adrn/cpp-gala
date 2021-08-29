#ifndef _GALA_FORCE_H
#define _GALA_FORCE_H

#include <string>
#include <gala/potential/potential.h>
#include <gala/potential/body.h>

namespace gala { namespace simulation {

class Force {
    /*
    Parameters
    ----------
    massless : bool
        Short-circuit flag for massless bodies.
    ndim : int
        Phase-space dimensionality (e.g., 3 for 3D positions / 6D phase-space).
    m_nbodies : int
        The number of bodies represented by this object.
    w :
        The instantaneous phase-space coordinates of the body.
    */
    public:
        // Attributes:

        // Methods:
        void get_acceleration(Body *body, double *w, double t);

};

}} // namespace: gala::simulation

#endif
