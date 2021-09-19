#ifndef _GALA_BODY_H
#define _GALA_BODY_H

// Standard
#include <vector>
#include <string>

// Third-party
#include <pybind11/pybind11.h>

// This package
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/utils.h>

using namespace gala::utils;

namespace gala { namespace simulation {

class PYBIND11_EXPORT ParticleCollection {
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
        The instantaneous phase-space coordinates
    */
    private:
        static uint32_t _count;

    public:
        // Attributes:
        bool massless;
        vector_2d w;
        int n_dim;
        int n_particles;
        gala::potential::BasePotential *potential;
        std::string name;
        uint32_t ID;
        std::vector<std::tuple<uint32_t, uint32_t>> IDs;

        // Constructors:
        ParticleCollection() { ID = ++_count; }
        ParticleCollection(std::string name);
        ParticleCollection(vector_2d w, std::string name);
        ParticleCollection(vector_2d w,
                           gala::potential::BasePotential *potential,
                           std::string name);

        // Methods:
        void add_particles(vector_2d &w);
        void get_acceleration_at(ParticleCollection &pc, const double t,
                                 vector_2d *acc, int acc_start_idx=0);
        void get_acceleration_at(vector_2d &w, const double t,
                                 std::vector<std::tuple<uint32_t, uint32_t>> &IDs,
                                 vector_2d *acc, int acc_start_idx=0);
        vector_2d get_x();
        vector_2d get_v();
        // TODO: figure this out, but will enable things like dynamical friction
        // void register_extra_force();

};

}} // namespace: gala::simulation

#endif
