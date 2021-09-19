/*
    TODO:
*/

#include <iostream>
#include <cmath>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/simulation.h>
#include <cpp_gala/simulation/particle.h>
#include <cpp_gala/simulation/frame.h>
#include <cpp_gala/simulation/mockstream.h>
#include <cpp_gala/integrate/integrate.h>

using namespace gala::utils;
using namespace gala::simulation;


MockStreamSimulation::MockStreamSimulation(gala::potential::BasePotential *potential,
                                           gala::df::BaseStreamDF &df,
                                           ParticleCollection &progenitor,
                                           gala::frame::BaseFrame *frame)
: Simulation(potential, frame) {
    // Validate input external potential and frame:
    // TODO: require external potential to exist
    if ((potential->n_dim != 3) || ((frame != nullptr) && (frame->n_dim != 3)))
        throw std::invalid_argument("Input potential and frame must be 3D");

    this->n_dim = 3;
    this->n_particles = 0;

    // Add progenitor as the 0th particle associated with the simulation:
    this->prog_key = this->add_particle(progenitor);
    this->df = &df;

    this->stream_particles.reset(new ParticleCollection("stream"));
    this->stream_particles->n_dim = this->n_dim; // HACK
    this->stream_key = this->add_particle((*this->stream_particles));
}

void MockStreamSimulation::step_callback(const int step_i, const double t) {
    // TODO: implement way of deciding when to release - new class for StreamParticleReleaser?
    ParticleCollection *stream_pc = &this->particles.find(this->stream_key)->second;
    uint32_t nparticles = 1;  // TODO HACK - should be saved on DF?
    uint32_t init_size = stream_pc->n_particles;

    // TODO: determine whether we should release at this timestep, etc...
    auto particle_w = this->df->sample((*this->potential),
                                       this->particles.find(this->prog_key)->second,
                                       nparticles,
                                       t);
    std::cout << "particle_w size " << particle_w.size() << std::endl;

    std::cout << "(pre) stream " << stream_pc->name << " particles size " << stream_pc->n_particles << std::endl;
    stream_pc->add_particles(particle_w);
    std::cout << "(post) stream " << stream_pc->name << " particles size " << stream_pc->n_particles << std::endl;

    for (int i=init_size; i < init_size + particle_w.size(); i++) {
        std::cout << "particle i= " << i << std::endl;

        this->state_w.push_back(stream_pc->w[i]);
        this->particle_potentials.push_back(stream_pc->potential);
        this->particle_IDs.push_back(stream_pc->IDs[i]);
    }

    this->n_particles += particle_w.size();

}