#include <iostream>
#include <cmath>
#include <cpp_gala/integrate/integrate.h>
#include <cpp_gala/simulation/simulation.h>

namespace gala { namespace integrate {

/*
    Base class
*/
BaseIntegrator::BaseIntegrator() { }

BaseIntegrator::~BaseIntegrator() { }

void BaseIntegrator::integrate(Simulation *sim, double *t, int n_t, double *result_w) {
    /*
    Parameters
    ----------
    *sim
    *t
        Pointer to
    n_t
    *result_w
        Pointer to the array to store the result. If "store_all" is set to true, this array should have size equal to (n_t, sim->n_bodies, sim->potential->n_dim)
    */
}

/*
    Leapfrog
*/
LeapfrogIntegrator::LeapfrogIntegrator() { }

void LeapfrogIntegrator::step(Simulation *sim, double t0, double dt, double *w) {

}

}} // namespace: gala::integrate
