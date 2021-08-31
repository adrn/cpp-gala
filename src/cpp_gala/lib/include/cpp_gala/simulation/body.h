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
        std::vector<std::vector<double>> w;
        int n_dim;
        int n_bodies;
        gala::potential::BasePotential *potential;
        std::string name;

        // Constructors:
        BodyCollection(gala::potential::BasePotential *potential,
                       std::vector<std::vector<double>> w,
                       std::string name="");

        // Methods:
        void get_acceleration(BodyCollection *body, double t, double *acc);
        // void register_extra_force();
        // TODO: figure this out, but will enable things like dynamical friction

};

}} // namespace: gala::simulation

#endif
