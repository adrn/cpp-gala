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
                                           gala::df::BaseStreamDF *df,
                                           ParticleCollection *progenitor,
                                           gala::frame::BaseFrame *frame)
: Simulation(potential, frame) {
    // Validate input external potential and frame:
    // TODO: require external potential to exist
    if ((potential->n_dim != 3) || ((frame != nullptr) && (frame->n_dim != 3)))
        throw std::invalid_argument("Input potential and frame must be 3D")

    this->n_dim = 3;
    this->n_particles = 0;

    // Add progenitor as the 0th particle associated with the simulation:
    this->prog_key = this->add_particle(progenitor);
}

void MockStreamSimulation::stream_step_callback(const int step_i, const double t) {
    // TODO: implement way of deciding when to release - new class for StreamParticleReleaser?
    vector_2d particle_vec;
    ParticleCollection *pc = &sim.particles.find(sim.stream_key)->second;
    uint32_t nparticles = 1;  // TODO HACK

    // TODO: determine whether we should release at this timestep, etc...
    particle_vec = sim.df->sample(sim.potential,
                                  sim.particles.find(sim.prog_key)->second,
                                  nparticles,
                                  t);

    for (int i=0; i < particle_vec.size(); i++) {
        auto ptcl_id = std::make_tuple(pc->ID, pc->n_particles + i);

        sim.state_w.push_back(particle_vec[i]);
        pc->w.push_back(particle_vec[i]);
        pc->IDs.push_back(ptcl_id);

        sim.particle_potentials.push_back(pc->potential);
        sim.particle_IDs.push_back(ptcl_id);
    }

    sim.n_particles += particle_vec.size();
    pc->n_particles += particle_vec.size();

}