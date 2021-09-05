#include <iostream>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/simulation.h>
#include <cpp_gala/simulation/body.h>

using namespace gala::utils;

namespace gala { namespace simulation {

// Base class
Simulation::Simulation() {
    // store potential pointer and initialize
    this->has_ext_potential = false;
    this->n_dim = 0;
    this->n_bodies = 0;
    this->state_time = 0.;
}

Simulation::Simulation(gala::potential::BasePotential *potential)
: Simulation() {
    // store potential and initialize
    this->potential = potential;
    this->has_ext_potential = true;
    this->n_dim = potential->n_dim;
}

void Simulation::add_body(BodyCollection body) {
    /*
    TODO: error if the name is the same as an existing body?
    */
    std::string key = body.name;

    if (this->n_dim == 0) {
        this->n_dim = body.n_dim;
    } else if (this->n_dim != body.n_dim) {
        throw std::runtime_error(
            "Input BodyCollection must have the same n_dim as the simulation");
    }
    this->n_bodies += body.n_bodies;

    if (!body.massless)
        this->has_nbody_interaction = true;

    // for (int i=0; i < body.n_bodies; i++) {
    this->state_w.insert(this->state_w.end(), body.w.begin(), body.w.end());
    this->body_ids.insert(this->body_ids.end(), body.ids.begin(), body.ids.end());
    // }

    this->bodies.insert(std::make_pair(key, body));
}

void Simulation::get_dwdt(vector_2d *dwdt) {
    /*
    Compute the phase-space derivative for the current state of the simulation.

    TODO: error if n_bodies == 0?
    */
    int i, j;

    // Zero out any existing values
    for (i=0; i < this->n_bodies; i++) {
        for (j=0; j < this->n_dim; j++) {
            // Set position derivative to velocity
            dwdt->at(i)[j] = this->state_w[i][j + this->n_dim];

            // Set velocity derivative (acceleration) to zero (to be filled below)
            dwdt->at(i)[j + this->n_dim] = 0.;
        }
    }

    if (has_ext_potential) {
        // Compute the acceleration from the external potential, if set
        for (i=0; i < this->state_w.size(); i++)
            this->potential->acceleration(&this->state_w[i][0],
                                          this->state_time,
                                          &dwdt->at(i).at(this->n_dim));
    }

    // Compute the acceleration from all bodies
    for (auto &pair : this->bodies)
        pair.second.get_acceleration_at(this->state_w, this->state_time, this->body_ids, dwdt,
                                        this->n_dim);

    // TODO: something about computing the extra force on bodies...like from dynamical friction
}

vector_2d Simulation::get_dwdt() {
    vector_2d dwdt(this->n_bodies, vector_1d(2 * this->n_dim));
    this->get_dwdt(&dwdt);
    return dwdt;
}

void Simulation::set_state(vector_2d w, double t) {
    int n=0;
    this->state_time = t;
    this->state_w = w;

    for (auto &pair : this->bodies) {
        for (int i=0; i < pair.second.n_bodies; i++) {
            for (int j=0; j < this->n_dim; j++) {
                pair.second.w[i][j] = w[n + i][j];
            }
        }
        n += pair.second.n_bodies;
    }
}

}} // namespace: gala::simulation
