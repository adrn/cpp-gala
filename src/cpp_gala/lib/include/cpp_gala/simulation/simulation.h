/*
- Simulation class that allows specifying "bodies", potentials attached to bodies, and interactions between bodies
- Pass integrator to simulation?
*/

#ifndef _GALA_SIMULATION_H
#define _GALA_SIMULATION_H

#include <string>

namespace gala { namespace simulation {

class BaseSimulation {

    public:
        // Attributes:
        // std::string const m_name;

        // Methods:
        // virtual double get_value(double t);

};

class PotentialSimulation : public BaseSimulation {
    /*
    A simulation with test particles orbiting in an external potential.
    */

    public:
        // Attributes:
        // double m_val;

        // Constructors:
        // TODO: implement int, long, double, array constructors!
        // StaticPotentialParameter(double val);

        // Methods:
        // double get_value(double t) override;

};

}} // namespace: gala::simulation

#endif
