#ifndef _GALA_INTEGRATE_H
#define _GALA_INTEGRATE_H

// Standard
#include <string>
#include <vector>

// Third-party
#include <pybind11/pybind11.h>

// This package
#include <cpp_gala/simulation/simulation.h>
#include <cpp_gala/utils.h>

using namespace gala::utils;

namespace gala { namespace integrate {


typedef void step_callback_t(gala::simulation::Simulation &sim, const int step_i, const double t);


class PYBIND11_EXPORT BaseIntegrator {
    public:
        // Attributes:
        gala::simulation::Simulation sim;
        vector_2d tmp_w;  // temporary container for per-step phase-space positions
        vector_2d tmp_dwdt;  // temporary container for per-step dw/dt

        // Constructors and Destructors:
        BaseIntegrator(gala::simulation::Simulation sim, step_callback_t *step_callback=nullptr);

        // Methods:
        // vector_2d integrate(const vector_1d t);
        // vector_3d integrate_save_all(const vector_1d t);
        void integrate(const vector_1d t, double *result_w);
        void integrate_save_all(const vector_1d t, double *result_w);

        virtual void setup_integrate(const vector_1d &t);
        virtual void step(const double t, const double dt);
        step_callback_t *step_callback;

};

class LeapfrogIntegrator : public BaseIntegrator {
    public:
        // Constructors and Destructors:
        LeapfrogIntegrator(gala::simulation::Simulation sim);

        // Methods:
        void setup_integrate(const vector_1d &t) override;
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
