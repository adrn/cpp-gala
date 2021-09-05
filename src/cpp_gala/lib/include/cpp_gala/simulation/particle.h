#ifndef _GALA_BODY_H
#define _GALA_BODY_H

#include <vector>
#include <string>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/utils.h>

using namespace gala::utils;

namespace gala { namespace simulation {

class ParticleCollection {
    /*
    Parameters
    ----------
    massless
        Short-circuit flag for massless particles.
    n_dim
        Phase-space dimensionality (e.g., 3 for 3D positions / 6D phase-space).
    n_particles
        The number of particles represented by this object.
    w
        The instantaneous phase-space coordinates of the ptcl.
    */
    public:
        // Attributes:
        bool massless;
        vector_2d w;
        std::vector<std::string> ids;
        int n_dim;
        int n_particles;
        gala::potential::BasePotential *potential;
        std::string name;

        // Constructors:
        ParticleCollection(vector_2d w,
                       std::string name);
        ParticleCollection(vector_2d w,
                       gala::potential::BasePotential *potential,
                       std::string name);

        // Methods:
        void get_acceleration_at(ParticleCollection &ptcl, double t,
                                 vector_2d *acc, int acc_start_idx=0);
        void get_acceleration_at(vector_2d &w, double t, std::vector<std::string> &ids,
                                 vector_2d *acc, int acc_start_idx=0);
        // TODO: figure this out, but will enable things like dynamical friction
        // void register_extra_force();

};

}} // namespace: gala::simulation

#endif
