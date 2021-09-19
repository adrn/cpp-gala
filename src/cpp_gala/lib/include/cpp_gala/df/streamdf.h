#ifndef _GALA_STREAMDF_H
#define _GALA_STREAMDF_H

// Standard
#include <vector>
#include <string>

// Third-party
#include <pybind11/pybind11.h>

// This package
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/particle.h>
#include <cpp_gala/utils.h>

using namespace gala::utils;

namespace gala { namespace df {


class PYBIND11_EXPORT BaseStreamDF {

    public:
        // Attributes:
        bool lead;
        bool trail;
        int seed;

        // Constructors:
        BaseStreamDF(bool lead, bool trail, int seed=0);

        // Methods:
        virtual vector_2d sample(gala::potential::BasePotential &potential,
                                 gala::simulation::ParticleCollection &progenitor,
                                 std::size_t n_particles,
                                 double t);
};

class StreaklineStreamDF : public BaseStreamDF {
    public:
        // Constructors and Destructors:
        StreaklineStreamDF(bool lead, bool trail, int seed=0);

        // Methods:
        vector_2d sample(gala::potential::BasePotential &potential,
                         gala::simulation::ParticleCollection &progenitor,
                         std::size_t n_particles,
                         double t) override;
};




}} // namespace: gala::df

#endif