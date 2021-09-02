#include <iostream>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/simulation.h>
#include <cpp_gala/simulation/body.h>

using namespace gala::utils;

namespace gala { namespace simulation {

// Base class
Simulation::Simulation(gala::potential::BasePotential *potential) {
    // store potential pointer and initialize
    this->potential = potential;
    if (potential != nullptr) {
        this->n_dim = potential->n_dim;
    } else {
        this->n_dim = 0;
    }
}

void Simulation::add_body(BodyCollection *body) {
    /*
    TODO: check body->n_dim against this->potential->n_dim?
    TODO: error if the name is the same as an existing body
    */
    std::string key;

    if (body->name == "") {
        key = "body[" + std::to_string(this->get_n_bodies()) + "]";
    } else {
        key = body->name;
    }

    if (this->n_dim == 0) {
        this->n_dim = body->n_dim;
    } else if (this->n_dim != body->n_dim) {
        throw std::runtime_error(
            "Input BodyCollection must have the same n_dim as the simulation");
    }

    this->bodies.insert(std::make_pair(key, body));
}

int Simulation::get_n_bodies() {
    /*
    Return the total number of bodies added to the simulation.
    */
    int n_bodies = 0;
    for (const auto &pair : this->bodies)
        n_bodies += pair.second->n_bodies;
    return n_bodies;
}

void Simulation::get_body_acceleration(BodyCollection *body, double t,
                                       vector_2d *acc) {
    /*
    Compute the acceleration on a body given all other elements (bodies, external potentials, and
    forces) in the simulation.
    */
    int i;

    // Zero out any existing acceleration values
    for (i=0; i < body->n_bodies; i++)
        acc->at(i).assign(body->n_dim, 0.);

    if (this->potential != NULL) {
        // Compute the acceleration from the simulation external potential, if set
        for (i=0; i < body->n_bodies; i++)
            this->potential->acceleration(&body->w[i][0],
                                          t,
                                          &acc->at(i)[0]);
    }

    // Compute the acceleration from all other bodies
    for (const auto &pair : this->bodies)
        pair.second->get_acceleration(body, t, acc);

    // TODO: Compute acceleration from additional forces registered to this body

}

void Simulation::get_w_acceleration(vector_2d *w, double t,
                                    std::vector<std::string> *ids, vector_2d *acc) {
    /*
    Compute the acceleration at the input phase-space position from all bodies and external
    potentials in the simulation.
    */
    int i;

    // Zero out any existing acceleration values
    for (i=0; i < w->size(); i++)
        acc->at(i).assign(this->n_dim, 0.);

    if (this->potential != NULL) {
        // Compute the acceleration from the simulation external potential, if set
        for (i=0; i < w->size(); i++)
            this->potential->acceleration(&w->at(i)[0],
                                          t,
                                          &acc->at(i)[0]);
    }

    // Compute the acceleration from all bodies
    for (const auto &pair : this->bodies)
        pair.second->get_acceleration(w, t, ids, acc);
}

// void Simulation::get_acceleration(double t, vector_2d *acc) {
//     int n = 0;
//     for (const auto &pair : this->bodies) {
//         this->get_body_acceleration(pair.second, t, &acc[n * this->n_dim]);
//         n += pair.second->n_bodies;
//     }
// }

vector_2d Simulation::get_w() {
    int i, j, ps_ndim = 2 * this->n_dim, n = 0;
    vector_2d w(this->get_n_bodies(), vector_1d(ps_ndim, 0.));
    for (const auto &pair : this->bodies) {
        for (i=0; i < pair.second->n_bodies; i++) {
            for (j=0; j < ps_ndim; j++) {
                w[n + i][j] = pair.second->w[i][j];
            }
        }
        n += pair.second->n_bodies;
    }

    return w;
}

}} // namespace: gala::simulation
