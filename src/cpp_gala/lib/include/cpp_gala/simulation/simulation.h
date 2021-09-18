#ifndef _GALA_SIMULATION_H
#define _GALA_SIMULATION_H

// Standard
#include <vector>
#include <string>

// Third-party
#include <pybind11/pybind11.h>

// This package
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/particle.h>
#include <cpp_gala/simulation/frame.h>
#include <cpp_gala/utils.h>

using namespace gala::utils;

namespace gala { namespace simulation {

class PYBIND11_EXPORT Simulation {

    public:
        // Attributes:
        int n_dim;
        int n_particles;
        gala::potential::BasePotential *potential;
        gala::frame::BaseFrame *frame;
        std::map<std::tuple<std::string, uint32_t>, ParticleCollection> particles;

        bool has_ext_potential;
        bool has_frame;
        bool has_interparticle_interactions;

        // Internal
        std::vector<std::tuple<uint32_t, uint32_t>> particle_IDs;
        std::vector<gala::potential::BasePotential*> particle_potentials;
        vector_2d state_w;
        double state_time;

        // Constructors:
        Simulation(gala::potential::BasePotential *potential,
                   gala::frame::BaseFrame *frame=nullptr);
        Simulation();

        // Methods:
        std::tuple<std::string, uint32_t> add_particle(ParticleCollection pc);

        void get_dwdt(vector_2d *dwdt);
        vector_2d get_dwdt();

        void set_state(const vector_2d &w, const double t);
};

}} // namespace: gala::simulation

#endif
