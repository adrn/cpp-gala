#ifndef _GALA_BODY_H
#define _GALA_BODY_H

#include <string>
#include <cpp_gala/potential/potential.h>

namespace gala { namespace simulation {

class BodyCollection {
    /*
    Parameters
    ----------
    massless
        Short-circuit flag for massless bodies.
    n_dim
        Phase-space dimensionality (e.g., 3 for 3D positions / 6D phase-space).
    n_bodies
        The number of bodies represented by this object.
    w
        The instantaneous phase-space coordinates of the body.
    */
    public:
        // Attributes:
        bool massless;
        int n_dim;
        int n_bodies;
        double *w;
        gala::potential::BasePotential *potential;
        std::string name;

        // Constructors:
        BodyCollection(gala::potential::BasePotential *potential, double *w, int n_bodies=1,
                       std::string name="", int n_dim=DEFAULT_n_dim);

        // Methods:
        void get_acceleration(BodyCollection *body, double t, double *acc);
        // void register_extra_force();
        // TODO: figure this out, but will enable things like dynamical friction

};

}} // namespace: gala::simulation

#endif
