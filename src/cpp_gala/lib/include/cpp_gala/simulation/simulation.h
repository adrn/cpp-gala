/*
- Simulation class that allows specifying "bodies", potentials attached to bodies, and interactions between bodies
- Pass integrator to simulation?
*/

#ifndef _GALA_SIMULATION_H
#define _GALA_SIMULATION_H

#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/body.h>

namespace gala { namespace simulation {

// TODO: should we use pybind11_export here instead?
class __attribute__((visibility("default"))) Simulation {

    public:
        // Attributes:
        gala::potential::BasePotential *potential;
        std::map<std::string, BodyCollection*> bodies;

        // Constructors:
        Simulation(gala::potential::BasePotential *potential);
        Simulation() : Simulation(NULL) {};

        // Methods:
        void add_body(BodyCollection *body);
        int get_n_bodies();
        void get_body_acceleration(BodyCollection *body, double t, double *acc);
};

}} // namespace: gala::simulation

#endif
