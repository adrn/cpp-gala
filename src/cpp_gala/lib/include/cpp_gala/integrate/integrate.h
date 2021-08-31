#ifndef _GALA_INTEGRATE_H
#define _GALA_INTEGRATE_H

#include <string>
#include <cpp_gala/simulation/simulation.h>

namespace gala { namespace integrate {

// TODO: should we use pybind11_export here instead?
class __attribute__((visibility("default"))) BaseIntegrator {
    public:
        // Attributes:

        // Constructors and Destructors:
        BaseIntegrator();
        ~BaseIntegrator();

        // Methods:
        virtual void step(Simulation *sim, double t0, double dt, double *w);
        void integrate(Simulation *sim, double *t, int n_t, double *result_w);
};


class LeapfrogIntegrator : public BaseIntegrator {
    public:
        // Constructors and Destructors:
        LeapfrogIntegrator();

        // Methods:
        void step(Simulation *sim, double t0, double dt, double *w) override;
};


class GSLIntegrator : public BaseIntegrator {
    public:
        // Constructors:
        //TODO: GSLIntegrator(); ...

        // Methods:
        void step(Simulation *sim, double t0, double dt, double *w) override;
};

}} // namespace: gala::integrate

#endif
