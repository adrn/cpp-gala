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

    for (int i=0; i < this->n_bodies; i++)
        this->ids.push_back(name + ":" + std::to_string(i));

    // If the potential is NULL, this is a massless particle
    if (potential == nullptr) {
        this->massless = true;
    } else {
        this->massless = false;
    }
}

void BodyCollection::get_acceleration(double *w, int n_w, double t, std::vector<std::string> *ids,
                                      double *acc) {
    /*
    Compute the acceleration from this body for the input positions w
    */
    int body_i;

    if (this->massless) {
        return;
    }

    for (int j=0; j < n_w; j++) {
        for (int i=0; i < this->n_bodies; i++) {
            if ((ids != nullptr) && (ids->at(j) == this->ids[i])) {
                continue;
            }

            // the potential has to be centered at each body:
            // TODO: is this assign slow? could make it a pointer and replace the pointer?
            this->potential->q0.assign(&this->w[i][0],
                                       &this->w[i][this->n_dim]);
            this->potential->acceleration(&w[j * 2 * this->n_dim], t, &acc[j * this->n_dim]);
        }
    }
}

void BodyCollection::get_acceleration(BodyCollection *body, double t, double *acc) {
    /*
    Compute the acceleration from this body collection on all bodies in the input body collection
    */
    this->get_acceleration(&body->w[0][0], body->n_bodies, t, &body->ids, acc);
}

}} // namespace: gala::simulation
