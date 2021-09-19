#include <iostream>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/particle.h>
#include <cpp_gala/utils.h>

using namespace gala::utils;

namespace gala { namespace simulation {

ParticleCollection::ParticleCollection(std::string name)
: ParticleCollection() {
    this->n_particles = 0;
    this->n_dim = 0;
    this->name = name;
    this->massless = true;
}

ParticleCollection::ParticleCollection(vector_2d w, std::string name)
: ParticleCollection(name) {
    this->add_particles(w);
}

uint32_t ParticleCollection::_count;

ParticleCollection::ParticleCollection(vector_2d w,
                                       gala::potential::BasePotential *potential,
                                       std::string name)
: ParticleCollection::ParticleCollection(w, name) {
    this->potential = potential;
    this->massless = false;
}

void ParticleCollection::add_particles(vector_2d &w) {
    if (w.size() == 0)
        throw std::invalid_argument("Input phase-space position w must have size > 0");

    if (this->n_dim == 0)
        this->n_dim = w[0].size() / 2;
    else if (this->n_dim != (w[0].size() / 2))
        throw std::invalid_argument("Input phase-space position n_dim must match other particles.");

    for (int i=0; i < w.size(); i++) {
        this->w.push_back(w[i]);
        this->IDs.push_back(std::make_tuple(this->ID, i + this->n_particles));
    }

    this->n_particles += w.size();
}

void ParticleCollection::get_acceleration_at(vector_2d &w, const double t,
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
            if ((IDs.size() > 0) && (IDs[j] == this->ID))
                continue;

            // the potential has to be centered at each particle:
            this->potential->set_q0(this->w[i]);
            this->potential->acceleration(&w[j][0], t, &(*acc)[j][acc_start_idx]);
        }
    }
}

void ParticleCollection::get_acceleration_at(ParticleCollection &pc, const double t,
                                             vector_2d *acc, int acc_start_idx) {
    /*
    Compute the acceleration from this particle collection on all particles in the input particle
    collection
    */
    this->get_acceleration_at(pc.w, t, pc.IDs, acc, acc_start_idx);
}

vector_2d ParticleCollection::get_x() {
    vector_2d x;
    for (int i=0; i < this->n_particles; i++)
        x.push_back(slice(this->w[i], 0, this->n_dim));
    return x;
}

vector_2d ParticleCollection::get_v() {
    vector_2d v;
    for (int i=0; i < this->n_particles; i++)
        v.push_back(slice(this->w[i], this->n_dim, 2 * this->n_dim));
    return v;
}

}} // namespace: gala::simulation
