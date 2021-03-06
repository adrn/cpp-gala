#include <iostream>
#include <cmath>
#include <cpp_gala/integrate/integrate.h>
#include <cpp_gala/simulation/simulation.h>
#include <cpp_gala/utils.h>

#include <boost/numeric/odeint.hpp>
#include <cpp_gala/extern/nested_range_algebra.hpp>
#include <cpp_gala/extern/vector_vector_resize.hpp>

// TODO: remove! just for printing...s
#include <pybind11/pybind11.h>
namespace py = pybind11;
using namespace pybind11::literals;

using namespace boost::numeric;

using namespace gala::utils;
using namespace gala::integrate;


/* ------------------------------------------------------------------------------------------------
    Base class

    TODO:
    - Redundant methods below! integrate/integrate_save_all...

*/
BaseIntegrator::BaseIntegrator(gala::simulation::Simulation &sim) {
    this->sim = &sim;
}

void BaseIntegrator::integrate(const vector_1d t, double *result_w) {
    int i, j, k, idx, ps_ndim = 2 * this->sim->n_dim;
    int ntimes = t.size();
    int nparticles = this->sim->get_n_particles();

    // Call any custom setup needed before starting to step the integrator:
    this->setup_integrate(t);
    this->sim->step_callback(0, t[0]);

    for (int n=1; n < ntimes; n++) {
        this->step(t[n-1], t[n] - t[n-1]);
        this->sim->step_callback(n, t[n]);
    }

    // Store the w vector at the final timestep
    k = 0;
    for (auto &pair : this->sim->particles) {
        for (i=0; i < pair.second.n_particles; i++) {
            for (j=0; j < ps_ndim; j++) {
                idx = i2d(k, j, nparticles, ps_ndim);
                result_w[idx] = pair.second.w[i][j];
            }
            k++;
        }
    }
}

void BaseIntegrator::integrate_save_all(const vector_1d t, double *result_w) {
    int i, j, k, idx, ps_ndim = 2 * this->sim->n_dim;
    int ntimes = t.size();
    int nparticles = this->sim->get_n_particles();

    // Store the initial values in the first block of the result array:
    k = 0;
    for (auto &pair : this->sim->particles) {
        for (i=0; i < pair.second.n_particles; i++) {
            for (j=0; j < ps_ndim; j++)
                result_w[j + ps_ndim * k] = pair.second.w[i][j];
            k++;
        }
    }

    // Call any custom setup needed before starting to step the integrator:
    this->setup_integrate(t);
    this->sim->step_callback(0, t[0]);

    for (int n=1; n < ntimes; n++) {
        this->step(t[n-1], t[n] - t[n-1]);
        this->sim->step_callback(n, t[n]);

        // Store the w vector at this timestep
        k = 0;
        for (auto &pair : this->sim->particles) {
            for (i=0; i < pair.second.n_particles; i++) {
                for (j=0; j < ps_ndim; j++) {
                    idx = i3d(n, k, j, ntimes, nparticles, ps_ndim);
                    result_w[idx] = pair.second.w[i][j];
                }
                k++;
            }
        }
    }
}

// vector_2d BaseIntegrator::integrate(const vector_1d t) {
//     // Call any custom setup needed before starting to step the integrator:
//     this->setup_integrate(t);

//     for (int n=1; n < t.size(); n++)
//         this->step(t[n-1], t[n] - t[n-1]);

//     return this->sim->state_w;
// }

// vector_3d BaseIntegrator::integrate_save_all(const vector_1d t) {
//     // The return array:
//     vector_3d result_w;

//     // Store the initial values in the first block of the result array:
//     result_w.push_back(this->sim->state_w);

//     // Call any custom setup needed before starting to step the integrator:
//     this->setup_integrate(t);

//     for (int n=1; n < t.size(); n++) {
//         this->step(t[n-1], t[n] - t[n-1]);

//         // Store the w vector at this timestep
//         result_w.push_back(this->sim->state_w);
//     }

//     return result_w;
// }

// These are the methods that are overridden by subclasses
// Note: If I don't include these, I get a "Symbol not found" error on import of cpp_gala._integrate
void BaseIntegrator::setup_integrate(const vector_1d &t) {
    if (t.size() < 2)
        throw std::runtime_error("Input time array must have > 1 element.");

    this->tmp_w = this->sim->get_state_w();
    this->tmp_dwdt = this->sim->get_dwdt(t[0]);
}

void BaseIntegrator::step(const double t, const double dt) { }


/* ------------------------------------------------------------------------------------------------
    Leapfrog
*/
LeapfrogIntegrator::LeapfrogIntegrator(gala::simulation::Simulation &sim)
: BaseIntegrator(sim) {
    for (int i=0; i < sim.get_n_particles(); i++)
        this->v_ip1_2.push_back(vector_1d(sim.n_dim, NAN));
}

void LeapfrogIntegrator::setup_integrate(const vector_1d &t) {
    BaseIntegrator::setup_integrate(t);
    // In BaseIntegrator::setup_integrate, the initial state_w and dwdt is stored in tmp_w and
    // tmp_dwdt, so we can use them below to initialize the array of velocities (1/2 step forward)

    double dt = t[1] - t[0];

    // First step all of the velocities by 1/2 step to initialize
    for (int i=0; i < this->tmp_w.size(); i++)
        for (int j=0; j < this->sim->n_dim; j++) {
            this->v_ip1_2[i][j] = this->tmp_w[i][this->sim->n_dim + j]
                + dt/2 * this->tmp_dwdt[i][this->sim->n_dim + j];
        }
}

void LeapfrogIntegrator::step(const double t, const double dt) {
    int i, j;

    // TODO: if (this->sim->fixed_n_particles == false), need to resize and recompute
    // tmp_w, tmp_dwdt, v_ip1_2 at the beginning of each step
    if (this->sim->fixed_n_particles == false) {
        this->tmp_w = this->sim->get_state_w();
        this->tmp_dwdt = this->sim->get_dwdt(t);
        for (int i=0; i < this->tmp_w.size(); i++)
            for (int j=0; j < this->sim->n_dim; j++) {
                this->v_ip1_2[i][j] = this->tmp_w[i][this->sim->n_dim + j]
                    + dt/2 * this->tmp_dwdt[i][this->sim->n_dim + j];
            }
    }

    // print_vector_2d(this->tmp_w);
    // print_vector_2d(this->tmp_dwdt);
    // print_vector_2d(this->v_ip1_2);

    // Evolve the positions forward by a full step, using the retarded velocities
    for (i=0; i < this->tmp_w.size(); i++)
        for (j=0; j < this->sim->n_dim; j++)
            this->tmp_w[i][j] += dt * this->v_ip1_2[i][j];

    // Set the simulation state where now the velocity values are 1/2 step behind
    this->sim->set_state_w(this->tmp_w);

    // Compute the acceleration at the new positions:
    this->sim->get_dwdt(t, this->tmp_dwdt);

    // Evolve the velocity forward first by a half step, snapshot the time-aligned position and
    // velocity, then finish the full step to leapfrog over the positions
    for (i=0; i < this->tmp_w.size(); i++) {
        for (j=0; j < this->sim->n_dim; j++) {
            this->tmp_w[i][this->sim->n_dim + j] = this->v_ip1_2[i][j]
                + dt/2 * this->tmp_dwdt[i][this->sim->n_dim + j];
            this->v_ip1_2[i][j] = this->tmp_w[i][this->sim->n_dim + j]
                + dt/2 * this->tmp_dwdt[i][this->sim->n_dim + j];
        }
    }

    // Set the simulation state: the position and velocity should be synced, so ready for output
    this->sim->set_state_w(this->tmp_w);
}


/* ------------------------------------------------------------------------------------------------
    Boost
*/

BoostIntegrator::BoostIntegrator(gala::simulation::Simulation &sim,
                                 std::string choice, int sub_steps)
: BaseIntegrator(sim) {
    /*
    choice
        Can be one of:
            rk4 (4th order Runge-Kutta)
            dopri5 (5th order Runge-Kutta)
            rk78 (8th order Runge-Kutta)
            adm (Adam-Bashforth-M)
    sub_steps
        The number of sub-steps to take (used in the Adam-Bashforth method).
    */
    this->sub_steps = sub_steps;
    this->choice = choice;
}

// Built from example here:
// https://github.com/headmyshoulder/odeint-v2/blob/master/examples/2d_lattice/spreading.cpp

// TODO: shit - will this work if w size changes for each step?

template <typename T>
void BoostIntegrator::step_worker(T stepper, const double t, const double dt) {
    stepper.do_step([this](const vector_2d &w, vector_2d &dw, const double t) {
        // TODO: this currently just gets the 3-acceleration, but need to compute the 6-acc
        this->sim->set_state_w(w);
        this->sim->get_dwdt(t, dw);
    }, this->tmp_w, t, dt);
    this->sim->set_state_w(this->tmp_w);
}

// List of steppers here:
// https://www.boost.org/doc/libs/1_63_0/libs/numeric/odeint/doc/html/boost_numeric_odeint/odeint_in_detail/steppers.html

// ---
void BoostIntegrator::step_rk4(const double t, const double dt) {
    // https://www.boost.org/doc/libs/1_60_0/libs/numeric/odeint/doc/html/boost/numeric/odeint/runge_kutta4.html
    typedef odeint::runge_kutta4 <
        vector_2d, double, vector_2d, double,
        nested_range_algebra <odeint::range_algebra>,
        odeint::default_operations
    > stepper_type_rk4;

    stepper_type_rk4 stepper;
    this->step_worker(stepper, t, dt);
}

void BoostIntegrator::step_dopri5(const double t, const double dt) {
    // https://www.boost.org/doc/libs/1_60_0/libs/numeric/odeint/doc/html/boost/numeric/odeint/runge_kutta_dopri5.html
    typedef odeint::runge_kutta_dopri5<
        vector_2d, double, vector_2d, double,
        nested_range_algebra< odeint::range_algebra >,
        odeint::default_operations
    > stepper_type_dopri5;

    stepper_type_dopri5 stepper;
    this->step_worker(stepper, t, dt);
}

void BoostIntegrator::step_rk78(const double t, const double dt) {
    // https://www.boost.org/doc/libs/1_60_0/libs/numeric/odeint/doc/html/boost/numeric/odeint/runge_kutta_fehlberg78.html
    typedef odeint::runge_kutta_fehlberg78 <
        vector_2d, double, vector_2d, double,
        nested_range_algebra <odeint::range_algebra>,
        odeint::default_operations
    > stepper_type_rk78;

    stepper_type_rk78 stepper;
    this->step_worker(stepper, t, dt);
}

template <const int substeps>
void BoostIntegrator::step_adm(const double t, const double dt) {
    // https://www.boost.org/doc/libs/1_60_0/libs/numeric/odeint/doc/html/boost/numeric/odeint/adams_bashforth_moulton.html
    typedef odeint::adams_bashforth_moulton <
        substeps,
        vector_2d, double, vector_2d, double,
        nested_range_algebra <odeint::range_algebra>,
        odeint::default_operations
    > stepper_type_adm;

    stepper_type_adm stepper;
    this->step_worker(stepper, t, dt);
}

void BoostIntegrator::step(const double t, const double dt) {
    if (this->choice == "rk4") {
        step_rk4(t, dt);
    } else if (this->choice == "rk78") {
        step_rk78(t, dt);
    } else if (this->choice == "dopri5") {
        step_dopri5(t, dt);
    } else if (this->choice == "adm") {
        switch (this->sub_steps) {
            case 2:
                step_adm<2>(t, dt);
            case 4:
                step_adm<4>(t, dt);
            case 6:
                step_adm<6>(t, dt);
            case 8:
                step_adm<8>(t, dt);
            default:
                throw std::invalid_argument("Invalid setting of sub_steps");
        }
    } else {
        throw std::invalid_argument("Invalid integrator choice");
    }

}
