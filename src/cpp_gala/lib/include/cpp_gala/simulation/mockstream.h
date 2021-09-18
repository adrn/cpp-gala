#ifndef _GALA_MOCKSTREAM_H
#define _GALA_MOCKSTREAM_H

// Standard
#include <vector>
#include <string>

// This package
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/particle.h>
#include <cpp_gala/simulation/frame.h>
#include <cpp_gala/simulation/simulation.h>
#include <cpp_gala/utils.h>

using namespace gala::utils;
using namespace gala::simulation;


class MockStreamSimulation : public Simulation {

    public:
        // Attributes:
        BaseStreamDF *df;
        ParticleCollection stream_particles;
        std::tuple<std::string, uint32_t> prog_key;

        // Constructors:
        MockStreamSimulation(
            gala::potential::BasePotential *potential,
            BaseStreamDF *df,
            ParticleCollection *progenitor,
            gala::frame::BaseFrame *frame=nullptr);

};

#endif