#include <iostream>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/body.h>

namespace gala { namespace simulation {

BodyCollection::BodyCollection(gala::potential::BasePotential *potential,
                               std::vector<std::vector<double>> w,
                               std::string name) {
    // store potential pointer and initialize
    this->potential = potential;
    this->w = w;
    this->n_bodies = w.size();
    this->n_dim = w[0].size() / 2;
    this->name = name;

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
            this->potential->q0.assign(&this->w[i][0],
                                       &this->w[i][this->n_dim]);
            this->potential->acceleration(&body->w[j][0], t, &acc[body->n_dim * j]);
        }
    }
}

}} // namespace: gala::simulation
