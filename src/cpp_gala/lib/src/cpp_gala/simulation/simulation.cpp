/*
    TODO:
    - Simulation should have a frame (to enable rotating frames)
*/

#include <iostream>
#include <cmath>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/simulation.h>
#include <cpp_gala/simulation/particle.h>
#include <cpp_gala/simulation/frame.h>

using namespace gala::utils;

namespace gala { namespace simulation {

// Base class
Simulation::Simulation() {
    this->has_ext_potential = false;
    this->has_interparticle_interactions = false;
    this->n_dim = 0;
    this->n_particles = 0;
    this->state_time = NAN;
}

Simulation::Simulation(gala::potential::BasePotential *potential, gala::frame::BaseFrame *frame)
: Simulation() {
    // store potential and initialize
    this->potential = potential;
    this->has_ext_potential = true;
    this->frame = frame;
    this->n_dim = potential->n_dim;

    // TODO: validate frame and potential same n_dim
}

void Simulation::add_particle(ParticleCollection pc) {
    /*
    TODO: error if the name is the same as an existing particle?
    TODO: validate, can't add if frame present!
    */

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
    this->n_particles += pc.n_particles;

    if (!pc.massless)
        this->has_interparticle_interactions = true;

    this->state_w.insert(this->state_w.end(), pc.w.begin(), pc.w.end());
    for (int i=0; i < pc.n_particles; i++) {
        this->particle_potentials.push_back(pc.potential);
        this->particle_IDs.push_back(pc.IDs[i]);
    }

    this->particles.insert(std::make_pair(std::make_tuple(pc.name, pc.ID), pc));
}

void Simulation::get_dwdt(vector_2d *dwdt) {
    /*
    Compute the phase-space derivative for the current state of the simulation.

    TODO: error if n_particles == 0?
    */
    int i, j;

    // Zero out any existing values
    for (i=0; i < this->n_particles; i++) {
        for (j=0; j < this->n_dim; j++) {
            // Set position derivative to velocity
            (*dwdt)[i][j] = this->state_w[i][j + this->n_dim];

            // Set velocity derivative (acceleration) to zero (to be filled below)
            (*dwdt)[i][j + this->n_dim] = 0.;
        }
    }

    if (this->has_ext_potential) {
        // Compute the acceleration from the external potential, if set
        for (i=0; i < this->n_particles; i++)
            this->potential->acceleration(&this->state_w[i][0],
                                          this->state_time,
                                          &(*dwdt)[i][this->n_dim]);
    }

    if (this->frame != nullptr) {
        // Compute the effective forces from the reference frame
        for (i=0; i < this->n_particles; i++)
            this->frame->get_dwdt(&this->state_w[i][0],
                                  this->state_time,
                                  &(*dwdt)[i][this->n_dim]);
    }

    // Compute the acceleration from all particles
    if (this->has_interparticle_interactions) {
        for (auto &pair : this->particles)
            pair.second.get_acceleration_at(this->state_w, this->state_time, this->particle_IDs,
                                            dwdt, this->n_dim);
    }

    // TODO: something about computing the extra force on particles...like from dynamical friction
}

vector_2d Simulation::get_dwdt() {
    vector_2d dwdt(this->n_particles, vector_1d(2 * this->n_dim));
    this->get_dwdt(&dwdt);
    return dwdt;
}

void Simulation::set_state(const vector_2d &w, const double t) {
    int n=0;
    this->state_time = t;
    this->state_w = w;

    for (auto &pair : this->particles) {
        for (int i=0; i < pair.second.n_particles; i++) {
            for (int j=0; j < this->n_dim; j++) {
                pair.second.w[i][j] = w[n + i][j];
            }
        }
        n += pair.second.n_particles;
    }
}

}} // namespace: gala::simulation


/*
    Idea for speed, if we need it, to avoid copying data around every set_state() call.
    - When add_particle(), add the potential for that PC to a list and have a lookup table to go
      from particle index to its potential. Need to do the same for Forces but not our problem just
      yet.
    - Could also turn the vector stored in Particle into an array state_w, access that as a 2D
      array instead of as a vector_2d
    - Also: using the do_step() interface to Boost integration may be slower than integrate with an
      observer function that stores the results...
*/