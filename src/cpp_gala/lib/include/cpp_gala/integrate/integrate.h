#ifndef _GALA_INTEGRATE_H
#define _GALA_INTEGRATE_H

#include <string>
#include <vector>
#include <cpp_gala/simulation/simulation.h>
#include <cpp_gala/utils.h>

using namespace gala::utils;

namespace gala { namespace integrate {

// TODO: should we use pybind11_export here instead?
class __attribute__((visibility("default"))) BaseIntegrator {
    public:
        // Attributes:
        gala::simulation::Simulation sim;
        vector_2d tmp_w;  // temporary container for per-step phase-space positions
        vector_2d tmp_acc;  // temporary container for per-step accelerations
        std::vector<std::string> ptcl_ids;  // container for all sim ptcl ids

        // Constructors and Destructors:
        BaseIntegrator(gala::simulation::Simulation sim);

        // Methods:
        vector_3d integrate(vector_1d t);
        virtual void setup_integrate(double t0, double dt);
        virtual void step(double t, double dt);
};


class LeapfrogIntegrator : public BaseIntegrator {
    public:
        // Constructors and Destructors:
        LeapfrogIntegrator(gala::simulation::Simulation sim);

        // Methods:
        void setup_integrate(double t0, double dt) override;
        void step(double t, double dt) override;

    private:
        vector_2d v_ip1_2;
};

class GSLIntegrator : public BaseIntegrator {
    public:
        // Constructors and Destructors:
        GSLIntegrator(gala::simulation::Simulation sim);

        // Methods:
        void setup_integrate(double t0, double dt) override;
        void step(double t, double dt) override;

    // private:
    //     vector_2d v_ip1_2;
};

}} // namespace: gala::integrate

#endif
