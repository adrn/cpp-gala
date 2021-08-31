#include <iostream>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/simulation.h>
#include <cpp_gala/simulation/body.h>

namespace gala { namespace simulation {

// Base class
Simulation::Simulation(gala::potential::BasePotential *potential) {
    // store potential pointer and initialize
    this->potential = potential;
}

void Simulation::add_body(Body *body) {
    /*
    TODO: check body->ndim against this->potential->ndim?
    TODO: error if the name is the same as an existing body
    */
    std::string key;

    if (body->name == "") {
        key = "body[" + std::to_string(this->get_nbodies()) + "]";
    } else {
        key = body->name;
    }
    this->bodies.insert(std::make_pair(key, body));
}

int Simulation::get_nbodies() {
    return this->bodies.size();
}

void Simulation::get_acceleration(Body *body, double t, double *acc) {
    /*
    Compute the acceleration on a body given all other elements (bodies, external potentials, and
    forces) in the simulation.
    */
    int i, j;

    // Zero out any existing acceleration values
    for (i=0; i < (body->nbodies * body->ndim); i++) {
        acc[i] = 0.;
    }

    if (this->potential != NULL) {
        // Loop over each object in the input body:
        for (int i=0; i < body->nbodies; i++) {
            // Compute the acceleration from the simulation external potential, if set
            this->potential->acceleration(&body->w[2 * body->ndim * i],
                                          t,
                                          &acc[body->ndim * i]);

            // Acceleration from forces registered to this body:
            // TODO:

        }
    }

    // Compute the acceleration from all other bodies
    for (const auto &pair : this->bodies) {
        pair.second->get_acceleration(body, t, acc);
    }

    //return
}


}} // namespace: gala::simulation
