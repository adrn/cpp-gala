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

void Simulation::add_body(BodyCollection *body) {
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
    /*
    Return the total number of bodies added to the simulation.
    */
    int nbodies = 0;
    for (const auto &pair : this->bodies)
        nbodies += pair.second->nbodies;
    return nbodies;
}

void Simulation::get_acceleration(BodyCollection *body, double t, double *acc) {
    /*
    Compute the acceleration on a body given all other elements (bodies, external potentials, and
    forces) in the simulation.
    */
    int i;

    // Zero out any existing acceleration values
    for (i=0; i < (body->nbodies * body->ndim); i++)
        acc[i] = 0.;

    if (this->potential != NULL) {
        // Compute the acceleration from the simulation external potential, if set
        for (i=0; i < body->nbodies; i++)
            this->potential->acceleration(&body->w[2 * body->ndim * i],
                                          t,
                                          &acc[body->ndim * i]);
    }

    // Compute the acceleration from all other bodies
    for (const auto &pair : this->bodies)
        pair.second->get_acceleration(body, t, acc);


    // TODO: Compute acceleration from additional forces registered to this body

}


}} // namespace: gala::simulation
