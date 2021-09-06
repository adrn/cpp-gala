#include <iostream>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/particle.h>
#include <cpp_gala/utils.h>

using namespace gala::utils;

namespace gala { namespace simulation {

ParticleCollection::ParticleCollection(vector_2d w, std::string name)
: ParticleCollection() {
    // store potential pointer and initialize
    this->w = w;
    this->n_particles = w.size();
    this->n_dim = w[0].size() / 2;
    this->name = name;
    this->massless = true;

    for (int i=0; i < this->n_particles; i++)
        this->IDs.push_back(std::make_tuple(this->ID, i));
}

uint32_t ParticleCollection::_count;

ParticleCollection::ParticleCollection(vector_2d w,
                                       gala::potential::BasePotential *potential,
                                       std::string name)
: ParticleCollection::ParticleCollection(w, name) {
    this->potential = potential;
    this->massless = false;
}

void ParticleCollection::get_acceleration_at(vector_2d &w, double t,
                                             std::vector<std::tuple<uint32_t, uint32_t>> &IDs,
                                             vector_2d *acc, int acc_start_idx) {
    /*
    Compute the acceleration from this particle collection for the input positions w
    */
    if (this->massless) {
        return;
    }

    for (int j=0; j < w.size(); j++) {
        for (int i=0; i < this->n_particles; i++) {
            if ((IDs.size() > 0)
                    && (std::get<0>(IDs[j]) == this->ID)
                    && (std::get<1>(IDs[j]) == std::get<1>(this->IDs[i]))) {
                continue;
            }

            // the potential has to be centered at each particle:
            this->potential->q0 = &this->w[i];
            this->potential->acceleration(&w[j][0], t, &(*acc)[j][acc_start_idx]);
        }
    }
}

void ParticleCollection::get_acceleration_at(ParticleCollection &pc, double t,
                                             vector_2d *acc, int acc_start_idx) {
    /*
    Compute the acceleration from this particle collection on all particles in the input particle
    collection
    */
    this->get_acceleration_at(pc.w, t, pc.IDs, acc, acc_start_idx);
}

}} // namespace: gala::simulation
