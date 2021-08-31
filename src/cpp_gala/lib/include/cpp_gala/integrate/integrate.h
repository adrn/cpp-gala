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
        BaseIntegrator(Simulation *sim);
        ~BaseIntegrator();

        // Methods:
        virtual void step(double *w0, double t, double dt, double *result_w);
        virtual void integrate(double *t, int n_t, double *result_w);
};


class LeapfrogIntegrator : public BaseIntegrator {
    public:
        // Constructors and Destructors:
        LeapfrogIntegrator(Simulation *sim);

        // Methods:
        void step(double *w0, double t, double dt, double *result_w) override;
        void integrate(double *t, int n_t, double *result_w) override;

    private:
        std::vector<double> v_ip1_2;
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
