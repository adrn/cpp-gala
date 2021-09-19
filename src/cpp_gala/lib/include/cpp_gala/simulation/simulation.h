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
        gala::potential::BasePotential *potential;
        gala::frame::BaseFrame *frame;
        std::map<std::tuple<std::string, uint32_t>, ParticleCollection> particles;

        bool has_ext_potential;
        bool has_frame;
        bool has_interparticle_interactions;
        bool fixed_n_particles;

        // Constructors:
        Simulation(gala::potential::BasePotential *potential,
                   gala::frame::BaseFrame *frame = nullptr);
        Simulation();

        // Methods:
        std::tuple<std::string, uint32_t> add_particle(ParticleCollection pc);
        ssize_t get_n_particles();

        // Used by integrators:
        void get_dwdt(const double t, vector_2d &dwdt);
        vector_2d get_dwdt(const double t);

        void set_state_w(const vector_2d &w);
        void get_state_w(vector_2d &w);
        vector_2d get_state_w();

        virtual void step_callback(const int i, const double t);
};

}} // namespace: gala::simulation

#endif
