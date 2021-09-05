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
        vector_2d tmp_dwdt;  // temporary container for per-step dw/dt

        // Constructors and Destructors:
        BaseIntegrator(gala::simulation::Simulation sim);

        // Methods:
        vector_3d integrate(const vector_1d t);
        virtual void setup_integrate(const double t0, const double dt);
        virtual void step(const double t, const double dt);
};

class LeapfrogIntegrator : public BaseIntegrator {
    public:
        // Constructors and Destructors:
        LeapfrogIntegrator(gala::simulation::Simulation sim);

        // Methods:
        void setup_integrate(const double t0, const double dt) override;
        void step(const double t, const double dt) override;

    private:
        vector_2d v_ip1_2;
};

class BoostIntegrator : public BaseIntegrator {
    public:
        // Constructors and Destructors:
        BoostIntegrator(gala::simulation::Simulation sim, std::string choice, int sub_steps=0);

        // Methods:
        // void setup_integrate(double t0, double dt) override;
        void step(const double t, const double dt) override;

        template <typename T>
        void step_worker(T stepper, const double t, const double dt);
        void step_rk4(const double t, const double dt);
        void step_rk78(const double t, const double dt);
        void step_dopri5(const double t, const double dt);

        template <const int sub_steps>
        void step_adm(const double t, const double dt);

    private:
        int sub_steps;
        std::string choice;
};

}} // namespace: gala::integrate

#endif
