/*
    TODO:
*/

#include <iostream>
#include <cmath>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/simulation.h>
#include <cpp_gala/simulation/particle.h>
#include <cpp_gala/simulation/frame.h>

using namespace gala::utils;

namespace gala { namespace simulation {

/*
    Base class
*/
Simulation::Simulation() {
    this->has_ext_potential = false;
    this->has_frame = false;
    this->has_interparticle_interactions = false;
    this->n_dim = 0;

    // subclasses use this to indicate to the integrators that particles may be spawned or destroyed
    this->fixed_n_particles = true;
}

Simulation::Simulation(gala::potential::BasePotential *potential,
                       gala::frame::BaseFrame *frame)
: Simulation() {
    // store potential and initialize
    this->potential = potential;
    this->has_ext_potential = true;
    this->frame = frame;
    this->n_dim = potential->n_dim;

    if (frame != nullptr) {
        this->has_frame = true;

        if (this->frame->n_dim != this->potential->n_dim)
            throw std::invalid_argument("Input frame and potential must have the same n_dim");
    }
}

ssize_t Simulation::get_n_particles() {
    ssize_t n_particles = 0;
    for (auto &pair : this->particles)
        n_particles += pair.second.n_particles;
    return n_particles;
}

std::tuple<std::string, uint32_t> Simulation::add_particle(ParticleCollection pc) {
    /* */

    if (this->particles.count(std::make_tuple(pc.name, pc.ID)) != 0) {
        throw std::invalid_argument(
            "ParticleCollection already exists in this Simulation: Input ParticleCollection must "
            "have a unique name and ID combination.");
    }

    if (this->n_dim == 0) {
        this->n_dim = pc.n_dim;
    } else if (this->n_dim != pc.n_dim) {
        throw std::runtime_error(
            "Input ParticleCollection must have the same n_dim as the simulation");
    }

    if (!pc.massless)
        this->has_interparticle_interactions = true;

    auto key = std::make_tuple(pc.name, pc.ID);
    this->particles.insert(std::make_pair(key, pc));

    return key;
}

void Simulation::get_dwdt(const double t, vector_2d &dwdt) {
    /*
    Compute the phase-space derivative for the current state of the simulation.
    */
    int i, j, k;

    if (this->particles.size() == 0)
        return;

    // Zero out any existing values
    k = 0;
    for (auto &pair : this->particles) {
        for (i=0; i < pair.second.n_particles; i++) {
            for (j=0; j < this->n_dim; j++) {
                // Set position derivative to velocity
                dwdt[k][j] = pair.second.w[i][j + this->n_dim];

                // Set velocity derivative (acceleration) to zero (to be filled below)
                dwdt[k][j + this->n_dim] = 0.;
            }
            k++;
        }
    }

    if (this->has_ext_potential) {
        // Compute the acceleration from the external potential, if set
        k = 0;
        for (auto &pair : this->particles) {
            for (i=0; i < pair.second.n_particles; i++) {
                this->potential->acceleration(&pair.second.w[i][0],
                                              t,
                                              &dwdt[k][this->n_dim]);
                k++;
            }
        }
    }

    if (this->has_frame) {
        // Compute the effective forces from the reference frame
        k = 0;
        for (auto &pair : this->particles) {
            for (i=0; i < pair.second.n_particles; i++) {
                this->frame->get_dwdt(&pair.second.w[i][0],
                                      t,
                                      &dwdt[k][0]);
                k++;
            }
        }
    }

    // Compute the acceleration from all particles
    if (this->has_interparticle_interactions) {
        for (auto &pair2 : this->particles) {
            k = 0;
            for (auto &pair1 : this->particles) {
                for (i=0; i < pair1.second.n_particles; i++) {
                    pair2.second.get_acceleration_at(pair1.second.w[i], t, pair1.second.IDs[i],
                                                     dwdt[k], this->n_dim);
                    k++;
                }
            }
        }
    }

    // TODO: something about computing the extra force on particles...like from dynamical friction
}

vector_2d Simulation::get_dwdt(const double t) {
    vector_2d dwdt(this->get_n_particles(), vector_1d(2 * this->n_dim));
    this->get_dwdt(t, dwdt);
    return dwdt;
}

void Simulation::set_state_w(const vector_2d &w) {
    int k=0;
    for (auto &pair : this->particles) {
        for (int i=0; i < pair.second.n_particles; i++) {
            for (int j=0; j < 2 * this->n_dim; j++) {
                pair.second.w[i][j] = w[k][j];
            }
            k++;
        }
    }
}

void Simulation::get_state_w(vector_2d &w) {
    int k=0;
    for (auto &pair : this->particles) {
        for (int i=0; i < pair.second.n_particles; i++) {
            for (int j=0; j < 2 * this->n_dim; j++) {
                w[k][j] = pair.second.w[i][j];
            }
            k++;
        }
    }
}

vector_2d Simulation::get_state_w() {
    vector_2d state_w(this->get_n_particles(), vector_1d(2 * this->n_dim, NAN));
    this->get_state_w(state_w);
    return state_w;
}

void Simulation::step_callback(const int i, const double t) {
    // Default callback is a no-op
}

}} // namespace: gala::simulation
