/*
    TODO:
    - Need a composite potential
    - Handle rotation in potentials
*/

// Standard library
#include <iostream>
#include <cmath>

// This package
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/potential/potentialparameter.h>
#include <cpp_gala/utils.h>

using namespace gala::utils;

namespace gala { namespace potential {

// Base class
BasePotential::BasePotential() { }

BasePotential::BasePotential(double G, vector_1d &q0) {
    this->G = G;
    this->n_dim = DEFAULT_n_dim; // override this in subclasses if necessary
    this->q0 = q0;

    for (int i=0; i < this->n_dim; i++)
        this->tmp_q.push_back(NAN);

    if (q0.size() != this->n_dim)
        throw std::invalid_argument("Size of q0 must equal n_dim");
}

void BasePotential::shift_rotate_q(double *q) {
    for (int i=0; i < this->n_dim; i++) {
        this->tmp_q[i] = q[i] - this->q0[i];
    }

    // TODO: deal with rotation...
}

// These are the methods that are overridden by subclasses
// Note: If I don't include these, I get a "Symbol not found" error on import of cpp_gala._potential
double BasePotential::_density(double *q, double t) { return NAN; }
double BasePotential::_energy(double *q, double t) { return NAN; }
void BasePotential::_gradient(double *q, double t, double *grad) { }

// These are the methods that should be called by anything that actually needs to use potentials
double BasePotential::density(double *q, double t) {
    this->shift_rotate_q(q);
    return this->_density(this->tmp_q.data(), t);
}

double BasePotential::energy(double *q, double t) {
    this->shift_rotate_q(q);
    return this->_energy(this->tmp_q.data(), t);
}

void BasePotential::gradient(double *q, double t, double *grad) {
    this->shift_rotate_q(q);
    this->_gradient(this->tmp_q.data(), t, grad);
}

void BasePotential::acceleration(double *q, double t, double *acc) {
    std::vector<double> tmp_grad(this->n_dim, 0.);

    gradient(q, t, &tmp_grad[0]);
    for (int i=0; i < this->n_dim; i++) {
        acc[i] = acc[i] - tmp_grad[i];
    }
}

/* ------------------------------------------------------------------------------------------------
    Potential implementations
*/

/*
    Kepler
*/
KeplerPotential::KeplerPotential(double G, BasePotentialParameter &M, vector_1d &q0)
: BasePotential(G, q0) {
    this->parameters.insert(std::make_pair("M", &M));
}

double KeplerPotential::_density(double *q, double t) {
    double r = gala::utils::xyz_to_r(q);
    if (r == 0) {
        return INFINITY;
    } else {
        return 0.;
    }
}

double KeplerPotential::_energy(double *q, double t) {
    double r = gala::utils::xyz_to_r(q);
    double GM = this->G * this->parameters["M"]->get_value(t);
    return - GM / r;
}

void KeplerPotential::_gradient(double *q, double t, double *grad) {
    double r = gala::utils::xyz_to_r(q);
    double GM = this->G * this->parameters["M"]->get_value(t);
    double fac = GM / pow(r, 2);

    grad[0] += fac * q[0]/r;
    grad[1] += fac * q[1]/r;
    grad[2] += fac * q[2]/r;
}

/*
    Hernquist
*/
HernquistPotential::HernquistPotential(double G,
                                       BasePotentialParameter &M, BasePotentialParameter &a,
                                       vector_1d &q0)
: BasePotential(G, q0) {
    this->parameters.insert(std::make_pair("M", &M));
    this->parameters.insert(std::make_pair("a", &a));
}

double HernquistPotential::_density(double *q, double t) {
    double r = gala::utils::xyz_to_r(q);
    double M = this->parameters["M"]->get_value(t);
    double a = this->G * this->parameters["a"]->get_value(t);
    double rho0 = M / (2 * M_PI * pow(a, 3));
    return rho0 / (r/a * pow(1 + r/a, 3));
}

double HernquistPotential::_energy(double *q, double t) {
    double r = gala::utils::xyz_to_r(q);
    double GM = this->G * this->parameters["M"]->get_value(t);
    double a = this->G * this->parameters["a"]->get_value(t);
    return - GM / (r + a);
}

void HernquistPotential::_gradient(double *q, double t, double *grad) {
    double r = gala::utils::xyz_to_r(q);
    double GM = this->G * this->parameters["M"]->get_value(t);
    double a = this->G * this->parameters["a"]->get_value(t);
    double fac = GM / pow(r + a, 2);

    grad[0] += fac * q[0]/r;
    grad[1] += fac * q[1]/r;
    grad[2] += fac * q[2]/r;
}


}} // namespace: gala::potential
