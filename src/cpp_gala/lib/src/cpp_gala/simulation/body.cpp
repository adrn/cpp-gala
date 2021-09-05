#include <iostream>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/body.h>
#include <cpp_gala/utils.h>

using namespace gala::utils;

namespace gala { namespace simulation {

BodyCollection::BodyCollection(vector_2d w,
                               std::string name) {
    // store potential pointer and initialize
    this->w = w;
    this->n_bodies = w.size();
    this->n_dim = w[0].size() / 2;
    this->name = name;
    this->massless = true;

    for (int i=0; i < this->n_bodies; i++)
        this->ids.push_back(name + ":" + std::to_string(i));
}

BodyCollection::BodyCollection(vector_2d w,
                               gala::potential::BasePotential *potential,
                               std::string name)
: BodyCollection::BodyCollection(w, name) {
    this->potential = potential;
    this->massless = false;
}

void BodyCollection::get_acceleration_at(vector_2d &w, double t,
                                         std::vector<std::string> &ids,
                                         vector_2d *acc, int acc_start_idx) {
    /*
    Compute the acceleration from this body for the input positions w
    */
    if (this->massless) {
        return;
    }

    for (int j=0; j < w.size(); j++) {
        for (int i=0; i < this->n_bodies; i++) {
            if ((ids.size() > 0) && (ids[j] == this->ids[i])) {
                continue;
            }

            // the potential has to be centered at each body:
            // TODO: is this assign slow? could make q0 a pointer and replace the pointer?
            this->potential->q0.assign(&this->w[i][0],
                                       &this->w[i][this->n_dim]);
            this->potential->acceleration(&w[j][0], t, &acc->at(j)[acc_start_idx]);
        }
    }
}

void BodyCollection::get_acceleration_at(BodyCollection &body, double t,
                                         vector_2d *acc, int acc_start_idx) {
    /*
    Compute the acceleration from this body collection on all bodies in the input body collection
    */
    this->get_acceleration_at(body.w, t, body.ids, acc);
}

}} // namespace: gala::simulation
