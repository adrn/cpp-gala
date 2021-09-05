#include <iostream>
#include <cmath>
#include <cpp_gala/integrate/integrate.h>
#include <cpp_gala/simulation/simulation.h>
#include <cpp_gala/utils.h>

#include <boost/numeric/odeint.hpp>
#include <cpp_gala/extern/nested_range_algebra.hpp>
#include <cpp_gala/extern/nested_range_algebra.hpp>

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
    - Add option to store all vs. store only last timestep w
*/
BaseIntegrator::BaseIntegrator(gala::simulation::Simulation sim) {
    this->sim = sim;

    for (int i=0; i < this->sim.n_particles; i++) {
        this->tmp_w.push_back(vector_1d(2 * this->sim.n_dim, NAN));
        this->tmp_dwdt.push_back(vector_1d(2 * this->sim.n_dim, NAN));
    }

}

// TODO: implement another that returns vector_2d with a flag "save_all" or something??
vector_3d BaseIntegrator::integrate(vector_1d t) {
    if (t.size() < 2)
        throw std::runtime_error("Input time array must have > 1 element.");

    // The return array:
    vector_3d result_w;

    // Store the initial values in the first block of the result array:
    this->tmp_w = sim.state_w;
    result_w.push_back(this->sim.state_w);

    // Call any custom setup needed before starting to step the integrator:
    this->setup_integrate(t[0], t[1] - t[0]);

    for (int n=1; n < t.size(); n++) {
        this->step(t[n], t[n] - t[n-1]);

        // TODO: add a boolean flag to store all or just the final! as an attribute on the class
        // Store the w vector at this timestep
        result_w.push_back(sim.state_w);
        // for (i=0; i < this->sim.n_particles; i++)
        //     for (j=0; j < ps_ndim; j++)
        //         result_w[n][i][j] = this->tmp_w[i][j];
    }

    return result_w;
}

// These are the methods that are overridden by subclasses
// Note: If I don't include these, I get a "Symbol not found" error on import of cpp_gala._integrate
void BaseIntegrator::setup_integrate(double t0, double dt) { }
void BaseIntegrator::step(double t, double dt) { }


/* ------------------------------------------------------------------------------------------------
    Leapfrog
*/
LeapfrogIntegrator::LeapfrogIntegrator(gala::simulation::Simulation sim)
: BaseIntegrator(sim) {
    for (int i=0; i < this->sim.n_particles; i++)
        this->v_ip1_2.push_back(vector_1d(this->sim.n_dim, NAN));
}

void LeapfrogIntegrator::setup_integrate(double t0, double dt) {
    // First step all of the velocities by 1/2 step to initialize
    this->sim.get_dwdt(&this->tmp_dwdt);

    for (int i=0; i < this->sim.n_particles; i++)
        for (int j=0; j < this->sim.n_dim; j++) {
            this->v_ip1_2[i][j] = this->sim.state_w[i][this->sim.n_dim + j]
                + dt/2 * this->tmp_dwdt[i][this->sim.n_dim + j];
        }
}

void LeapfrogIntegrator::step(double t, double dt) {
    int i, j;

    // Evolve the positions forward by a full step, using the retarded velocities
    for (i=0; i < this->sim.n_particles; i++)
        for (j=0; j < this->sim.n_dim; j++)
            this->tmp_w[i][j] += dt * this->v_ip1_2[i][j];

    // Set the simulation state where now the velocity values are 1/2 step behind
    this->sim.set_state(this->tmp_w, t);

    // Compute the acceleration at the new positions:
    this->sim.get_dwdt(&this->tmp_dwdt);

    // Evolve the velocity forward first by a half step, snapshot the time-aligned position and
    // velocity, then finish the full step to leapfrog over the positions
    for (i=0; i < this->sim.n_particles; i++) {
        for (j=0; j < this->sim.n_dim; j++) {
            this->tmp_w[i][this->sim.n_dim + j] = this->v_ip1_2[i][j]
                + dt/2 * this->tmp_dwdt[i][this->sim.n_dim + j];
            this->v_ip1_2[i][j] = this->tmp_w[i][this->sim.n_dim + j]
                + dt/2 * this->tmp_dwdt[i][this->sim.n_dim + j];
        }
    }

    // Set the simulation state: the position and velocity should be synced, so ready for output
    this->sim.set_state(this->tmp_w, t);
}


/* ------------------------------------------------------------------------------------------------
    Boost
*/

// BoostIntegrator::BaseIntegrator(gala::simulation::Simulation sim, std::string choice, int steps)
// : BaseIntegrator(sim) {
//     /*
//     choice
//         Can be one of:
//             rk4 (4th order Runge-Kutta)
//             dopri5 (5th rder Runge-Kutta)
//             rk78 (8th order Runge-Kutta)
//             adm (Adam-Bashforth-M)
//     steps
//         The number of sub-steps to take (used in the Adam-Bashforth method).
//     */
//     this->steps = steps;
//     this->choice = choice;
// }

// // Built from example here:
// // https://github.com/headmyshoulder/odeint-v2/blob/master/examples/2d_lattice/spreading.cpp

// template <typename T>
// vector_3d BoostIntegrator::integrate_worker(T stepper, vector_2d w, vector_1d t) {
//     vector_3d all_w;

//     //[ integrate_const_loop
//     double dt;
//     all_w.push_back(w);

//     for (int i=1; i < t.size(); i++) {
//         dt = t[i] - t[i-1];
//         stepper.do_step([this](const vector_2d &w, vector_2d &dw, const double t) {
//             // TODO: this currently just gets the 3-acceleration, but need to compute the 6-acc
//             this->sim.get_w_acceleration(&w, t, &this->particle_ids, &dw);
//         }, this->tmp_w, t[i-1], dt);
//         all_w.push_back(this->tmp_w);
//     }
//     return all_w;
// }

// // List of steppers here:
// // https://www.boost.org/doc/libs/1_63_0/libs/numeric/odeint/doc/html/boost_numeric_odeint/odeint_in_detail/steppers.html

// // ---
// vector_3d BoostIntegrator::integrate_rk4(vector_2d w, vector_1d t) {
//     // https://www.boost.org/doc/libs/1_60_0/libs/numeric/odeint/doc/html/boost/numeric/odeint/runge_kutta4.html
//     typedef boost::numeric::odeint::runge_kutta4 <
//         vector_2d, double, vector_2d, double,
//         nested_range_algebra <boost::numeric::odeint::range_algebra>,
//         boost::numeric::odeint::default_operations
//     > stepper_type_rk4;

//     stepper_type_rk4 stepper;
//     return integrate_worker(stepper, w, t);
// }

// vector_3d BoostIntegrator::integrate_dopri5(vector_2d w, vector_1d t) {
//     // https://www.boost.org/doc/libs/1_60_0/libs/numeric/odeint/doc/html/boost/numeric/odeint/runge_kutta_dopri5.html
//     typedef boost::numeric::odeint::runge_kutta_dopri5<
//         vector_2d, double, vector_2d, double,
//         nested_range_algebra< boost::numeric::odeint::range_algebra >,
//         boost::numeric::odeint::default_operations
//     > stepper_type_dopri5;

//     stepper_type_dopri5 stepper;
//     return integrate_worker(stepper, w, t);
// }

// vector_3d BoostIntegrator::integrate_rk78(vector_2d w, vector_1d t) {
//     // https://www.boost.org/doc/libs/1_60_0/libs/numeric/odeint/doc/html/boost/numeric/odeint/runge_kutta_fehlberg78.html
//     typedef boost::numeric::odeint::runge_kutta_fehlberg78 <
//         vector_2d, double, vector_2d, double,
//         nested_range_algebra <boost::numeric::odeint::range_algebra>,
//         boost::numeric::odeint::default_operations
//     > stepper_type_rk78;

//     stepper_type_rk78 stepper;
//     return integrate_worker(stepper, w, t);
// }

// template <const int steps>
// vector_3d BoostIntegrator::integrate_adm(vector_2d w, vector_1d t) {
//     // https://www.boost.org/doc/libs/1_60_0/libs/numeric/odeint/doc/html/boost/numeric/odeint/adams_bashforth_moulton.html
//     typedef boost::numeric::odeint::adams_bashforth_moulton <
//         steps,
//         vector_2d, double, vector_2d, double,
//         nested_range_algebra <boost::numeric::odeint::range_algebra>,
//         boost::numeric::odeint::default_operations
//     > stepper_type_adm;

//     stepper_type_adm stepper;
//     return integrate_worker(stepper, w, t);
// }

// vector_3d BoostIntegrator::integrate(vector_1d t) {
//     vector_3d nothing;

//     if (this->choice == "rk4") {
//         return integrate_rk4(w, t);
//     } else if (this->choice == "rk78") {
//         return integrate_rk78(w, t);
//     } else if (this->choice == "dopri5") {
//         return integrate_dopri5(w, t);
//     } else if (this->choice == "adm") {
//         switch (this->steps) {
//             case 2:
//                 return integrate_adm<2>(w, t);
//             case 4:
//                 return integrate_adm<4>(w, t);
//             case 6:
//                 return integrate_adm<6>(w, t);
//             case 8:
//                 return integrate_adm<8>(w, t);
//             default:
//                 throw std::invalid_argument("Invalid setting of steps");
//                 return nothing;
//         }
//     } else {
//         throw std::invalid_argument("Invalid integrator choice");
//         return nothing;
//     }

// }
