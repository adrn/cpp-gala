#include <iostream>
#include <cmath>
#include <cpp_gala/integrate/integrate.h>
#include <cpp_gala/simulation/simulation.h>
#include <cpp_gala/util.h>

// TODO: just for printing
#include <pybind11/pybind11.h>
namespace py = pybind11;
using namespace pybind11::literals;

using namespace gala::utils;
using namespace gala::integrate;

/* ------------------------------------------------------------------------------------------------
    Base class

    TODO:
    - Add option to store all vs. store only last timestep w
*/
BaseIntegrator::BaseIntegrator(gala::simulation::Simulation sim) {
    this->sim = sim;

    for (int i=0; i < this->sim.get_n_bodies(); i++) {
        this->tmp_acc.push_back(vector_1d(this->sim.n_dim, NAN));
        this->tmp_w.push_back(vector_1d(this->sim.n_dim, NAN));
    }

    for (const auto &pair : sim.bodies)
        for (int i=0; i < pair.second->n_bodies; i++)
            this->body_ids.push_back(pair.second->ids[i]);

}

// TODO: implement another that returns vector_2d with a flag "save_all" or something??
vector_1d BaseIntegrator::integrate(vector_1d t) {
    int n, i, j, ps_ndim = 2 * this->sim.n_dim;

    if (t.size() < 2)
        throw std::runtime_error("Input time array must have > 1 element.");

    py::print("here A");

    // The return array:
    vector_1d result_w(t.size() * sim.get_n_bodies() * ps_ndim);

    // Store the initial values in the first block of the result array:
    // for (i=0; i < this->sim.get_n_bodies(); i++)
    //     for (j=0; j < ps_ndim; j++)
    //         // result_w[0][i][j] = this->sim.get_w()[i][j];
    py::print("here indeed");
    this->sim.get_w(&result_w[0][0][0]);
    this->sim.get_w(&this->tmp_w[0][0]);

    py::print("here true");
    this->setup_integrate(t[0], t[1] - t[0]);

    py::print("here B");

    for (n=1; n < t.size(); n++) {
        py::print("pre-step");
        this->step(t[n], t[n] - t[n-1]);
        py::print("here C");

        // TODO: add a boolean flag to store all or just the final! as an attribute on the class
        // Store the w vector at this timestep
        for (i=0; i < this->sim.get_n_bodies(); i++)
            for (j=0; j < ps_ndim; j++)
                result_w[n][i][j] = this->tmp_w[i][j];

        py::print("here D");
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
    for (int i=0; i < this->sim.get_n_bodies(); i++)
        this->v_ip1_2.push_back(vector_1d(this->sim.n_dim, NAN));
}

void LeapfrogIntegrator::setup_integrate(double t0, double dt) {
    // First step all of the velocities by 1/2 step to initialize
    py::print("setup_integrate");
    this->sim.get_w_acceleration(&this->tmp_w[0][0], sim.get_n_bodies(), t0, &this->body_ids,
                                 &this->tmp_acc[0][0]);
    py::print("ran get_w_acceleration");

    for (int i=0; i < this->sim.get_n_bodies(); i++)
        for (int j=0; j < this->sim.n_dim; j++)
            this->v_ip1_2[i][j] = this->tmp_w[i][this->sim.n_dim + j] + 0.5 * dt * tmp_acc[i][j];
}

void LeapfrogIntegrator::step(double t, double dt) {
    int i, j, ndim = this->sim.n_dim, ps_ndim = 2 * this->sim.n_dim;

    // full step the positions
    for (i=0; i < this->sim.get_n_bodies(); i++)
        std::cout << "body " << i;
        for (j=0; j < ndim; j++) {
            std::cout << " be " << j << " " << this->tmp_w[i][j];
            this->tmp_w[i][j] = this->tmp_w[i][j] + this->v_ip1_2[i][j] * dt;
            std::cout << " af " << j << " " << this->tmp_w[i][j];
        }
        std::cout << "\n";

    // compute the acceleration at the new positions:
    this->sim.get_w_acceleration(&this->tmp_w[0][0], sim.get_n_bodies(), t, &this->body_ids,
                                 &this->tmp_acc[0][0]);
    std::cout << "after get_w_acc \n";

    // step velocity forward by half step, aligned w/ position, then finish the full step to
    // leapfrog over the positions
    for (i=0; i < this->sim.get_n_bodies(); i++)
        std::cout << "half step velocity body " << i;
        for (j=0; j < ndim; j++) {
            std::cout << " be " << j << " " << this->v_ip1_2[i][j];
            this->tmp_w[i][ndim + j] = this->v_ip1_2[i][j] + 0.5*dt * tmp_acc[i][j];
            this->v_ip1_2[i][j] = this->tmp_w[i][ndim + j] + 0.5*dt * tmp_acc[i][j];
            std::cout << " af " << j << " " << this->v_ip1_2[i][j];
        }
        std::cout << "\n";
}
