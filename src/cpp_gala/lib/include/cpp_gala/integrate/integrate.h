#ifndef _GALA_INTEGRATE_H
#define _GALA_INTEGRATE_H

#include <string>
#include <vector>
#include <cpp_gala/simulation/simulation.h>

namespace gala { namespace integrate {

using vector_3d = std::vector<std::vector<std::vector<double>>>;
using vector_2d = std::vector<std::vector<double>>;
using vector_1d = std::vector<double>;

// TODO: should we use pybind11_export here instead?
class __attribute__((visibility("default"))) BaseIntegrator {
    public:
        // Attributes:
        gala::simulation::Simulation sim;

        // Constructors and Destructors:
        BaseIntegrator(gala::simulation::Simulation sim);

        // Methods:
        vector_3d integrate(vector_1d t);
        virtual void setup_integrate(double t0, double dt);
        virtual void step(double t, double dt);

    private:
        vector_2d tmp_w;
        vector_2d tmp_acc;
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


// class GSLIntegrator : public BaseIntegrator {
//     public:
//         // Constructors:
//         //TODO: GSLIntegrator(); ...

//         // Methods:
//         void step(double *w0, double t, double dt, double *result_w) override;
//         void integrate(double *t, int n_t, double *result_w) override;
// };

}} // namespace: gala::integrate

#endif
