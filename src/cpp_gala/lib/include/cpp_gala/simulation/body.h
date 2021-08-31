#ifndef _GALA_BODY_H
#define _GALA_BODY_H

#include <string>
#include <cpp_gala/potential/potential.h>

namespace gala { namespace simulation {

class Body {
    /*
    Parameters
    ----------
    massless : bool
        Short-circuit flag for massless bodies.
    ndim : int
        Phase-space dimensionality (e.g., 3 for 3D positions / 6D phase-space).
    nbodies : int
        The number of bodies represented by this object.
    w :
        The instantaneous phase-space coordinates of the body.
    */
    public:
        // Attributes:
        bool massless;
        int ndim;
        int nbodies;
        double *w;
        gala::potential::BasePotential *potential;
        std::string name;

        // Constructors:
        Body(gala::potential::BasePotential *potential, double *w, int nbodies=1,
             std::string name="", int ndim=DEFAULT_NDIM);

        // Methods:
        void get_acceleration(Body *body, double t, double *acc);
        // void register_extra_force();
        // TODO: figure this out, but will enable things like dynamical friction

};

}} // namespace: gala::simulation

#endif
