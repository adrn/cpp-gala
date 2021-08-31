#include <iostream>
#include <cmath>
#include <cpp_gala/integrate/integrate.h>
#include <cpp_gala/simulation/simulation.h>

namespace gala { namespace integrate {

/*
    Base class

    TODO:
    - Add setup_integrate method and implement a general integrate that calls it
    - Add option to store all vs. store only last timestep w
*/
BaseIntegrator::BaseIntegrator() { }

BaseIntegrator::~BaseIntegrator() { }

void BaseIntegrator::integrate(Simulation *sim, double *w0, double *t, int n_t, double *result_w) {
    /*
    Parameters
    ----------
    *sim
        The simulation to integrate.
    *w0
        The initial conditions.
    *t
        Pointer to the array of times.
    n_t
        The number of time elements in the array of times.
    *result_w
        Pointer to the array to store the result. If "store_all" is set to true, this array should
        have shape equal to (n_t, sim->n_bodies, sim->potential->n_dim).
    */

    // TODO: implement a constructor flag, like "assert_equal_dt" to enforce equal timesteps for
    // leapfrog or other methods that might require it.
}

/*
    Leapfrog
*/
LeapfrogIntegrator::LeapfrogIntegrator(Simulation *sim) {
    for (int i=0; i < sim->potential->n_dim; i++)
        this->v_ip1_2.push_back(NAN);
}

void LeapfrogIntegrator::step(double *w0, double t, double dt, double *result_w) {

}

void LeapfrogIntegrator::integrate(double *t, int n_t, double *result_w) {

    // First step all of the velocities by 1/2 step to initialize
    sim->get_acceleration()
    for (int i=0; i < sim->potential->n_dim; i++)
        this->v_ip1_2

}

}} // namespace: gala::integrate
