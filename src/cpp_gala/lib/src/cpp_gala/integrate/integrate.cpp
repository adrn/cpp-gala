#include <iostream>
#include <cmath>
#include <cpp_gala/integrate/integrate.h>
#include <cpp_gala/simulation/simulation.h>

namespace gala { namespace integrate {

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
}

// TODO: implement another that returns vector_2d with a flag "save_all" or something??
vector_3d BaseIntegrator::integrate(vector_1d t) {
    int n, i, j, ps_ndim = 2 * this->sim.n_dim;

    if (t.size() < 2)
        throw std::runtime_error("Input time array must have > 1 element.");

    // The return array:
    vector_3d result_w(t.size(), vector_2d(sim.get_n_bodies(), vector_1d(ps_ndim, NAN)));

    // Store the initial values in the first block of the result array:
    for (i=0; i < this->sim.get_n_bodies(); i++)
        for (j=0; j < ps_ndim; j++)
            // result_w[0][i][j] = this->sim.get_w()[i][j];
            this->sim.get_w(&result_w[0][i][j])
    this->sim.get_w(&this->tmp_w[0][0])

    this->setup_integrate(t[0], t[1] - t[0]);

    for (n=1; n < t.size(); n++) {
        this->step(t[n], t[n] - t[n-1]);

        // TODO: add a boolean flag to store all or just the final! as an attribute on the class
        // Store the w vector at this timestep
        for (i=0; i < this->sim.get_n_bodies(); i++)
            for (j=0; j < ps_ndim; j++)
                result_w[n][i][j] = this->tmp_w[i][j];
    }

    return result_w;
}

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
    this->sim.get_acceleration(t0, &this->tmp_acc[0][0]); // TODO: not sure this pointer works

    for (int i=0; i < this->sim.get_n_bodies(); i++)
        for (int j=0; j < this->sim.n_dim; j++)
            this->v_ip1_2[i][j] = this->tmp_w[i][this->sim.n_dim + j] + 0.5 * dt * tmp_acc[i][j];
}

void LeapfrogIntegrator::step(double t, double dt) {
    int i, j, ndim = this->sim.n_dim, ps_ndim = 2 * this->sim.n_dim;

    // full step the positions
    for (i=0; i < this->sim.get_n_bodies(); i++)
        for (j=0; j < ndim; j++)
            this->tmp_w[i][j] = this->tmp_w[i][j] + this->v_ip1_2[i][j] * dt;

    // compute the acceleration at the new positions:
    this->sim.get_w_acceleration(&this->tmp_w[0][0], t, &this->body_ids, &this->tmp_acc[0][0]);

    // step velocity forward by half step, aligned w/ position, then finish the full step to
    // leapfrog over the positions
    for (i=0; i < this->sim.get_n_bodies(); i++)
        for (j=0; j < ndim; j++)
            this->tmp_w[i][ndim + j] = this->v_ip1_2[i][j] + 0.5*dt * tmp_acc[i][j];
            this->v_ip1_2[i][j] = this->tmp_w[i][ndim + j] + 0.5*dt * tmp_acc[i][j];
}

}} // namespace: gala::integrate
