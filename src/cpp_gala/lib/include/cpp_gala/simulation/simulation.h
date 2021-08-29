/*
- Simulation class that allows specifying "bodies", potentials attached to bodies, and interactions between bodies
- Pass integrator to simulation?
*/

#ifndef _GALA_SIMULATION_H
#define _GALA_SIMULATION_H

#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/body.h>

namespace gala { namespace simulation {

class Simulation {

    public:
        // Attributes:
        gala::potential::BasePotential *potential;
        std::map<std::string, Body*> bodies;

        // Constructors:
        Simulation(gala::potential::BasePotential *potential);
        Simulation() : Simulation(NULL) {};

        // Methods:
        void add_body(Body *body);
        int get_nbodies();

};

}} // namespace: gala::simulation

#endif
