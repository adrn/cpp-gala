#include <iostream>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/body.h>

namespace gala { namespace simulation {

BodyCollection::BodyCollection(gala::potential::BasePotential *potential, double *w, int n_bodies,
                               std::string name, int n_dim) {
    // store potential pointer and initialize
    this->potential = potential;
    this->w = w;
    this->n_bodies = n_bodies;
    this->name = name;
    this->n_dim = n_dim;

    // If the potential is NULL, this is a massless particle
    if (potential == nullptr) {
        this->massless = true;
    } else {
        this->massless = false;
    }
}

void BodyCollection::get_acceleration(BodyCollection *body, double t, double *acc) {
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

    for (int j=0; j < body->n_bodies; j++) {
        for (int i=0; i < this->n_bodies; i++) {
            if ((body->name == this->name) && (i == j)) {
                continue;
            }

            // the potential has to be centered at each body:
            this->potential->q0 = &this->w[2 * this->n_dim * i];
            this->potential->acceleration(&body->w[2 * body->n_dim * j], t, &acc[body->n_dim * j]);
        }
    }
}

}} // namespace: gala::simulation
