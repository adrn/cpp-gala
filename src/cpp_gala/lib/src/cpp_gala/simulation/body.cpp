#include <iostream>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/body.h>

namespace gala { namespace simulation {

// TODO: rename Body -> BodyCollection
Body::Body(gala::potential::BasePotential *potential, double *w, int nbodies,
           std::string name, int ndim) {
    // store potential pointer and initialize
    this->potential = potential;
    this->w = w;
    this->nbodies = nbodies;
    this->name = name;
    this->ndim = ndim;

    // If the potential is NULL, this is a massless particle
    if (potential == nullptr) {
        this->massless = true;
    } else {
        this->massless = false;
    }
}

void Body::get_acceleration(Body *body, double t, double *acc) {
    /*
    Compute the acceleration from this body at a single position `w`

    Parameters
    ----------
    w
    t
    acc
        The output acceleration array

    */

    if (this->massless) {
        return;
    }

    for (int j=0; j < body->nbodies; j++) {
        for (int i=0; i < this->nbodies; i++) {
            if ((body->name == this->name) && (i == j)) {
                continue;
            }

            // the potential has to be centered at each body:
            this->potential->q0 = &this->w[2 * this->ndim * i];
            this->potential->acceleration(&body->w[2 * body->ndim * j], t, &acc[body->ndim * j]);
        }
    }
}

}} // namespace: gala::simulation
