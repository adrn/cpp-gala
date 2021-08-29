#include <iostream>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/simulation.h>
#include <cpp_gala/simulation/body.h>

namespace gala { namespace simulation {

// Base class
Simulation::Simulation(potential::BasePotential *potential) {
    // store potential pointer and initialize
    this->potential = potential;
}

void Simulation::add_body(Body *body) {
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


}} // namespace: gala::simulation
