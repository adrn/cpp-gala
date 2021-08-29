#include <iostream>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/body.h>

namespace gala { namespace simulation {

// Base class
Body::Body(gala::potential::BasePotential *potential, double *w, int nbodies,
           std::string name, int ndim) {
    // store potential pointer and initialize
    this->potential = potential;
    this->w = w;
    this->nbodies = nbodies;
    this->name = name;
    this->ndim = ndim;

    // If the potential is NULL, this is a massless particle
    if (potential == NULL) {
        this->massless = true;
    } else {
        this->massless = false;
    }
}

}} // namespace: gala::simulation
