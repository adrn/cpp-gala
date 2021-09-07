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

BasePotential::BasePotential(double G, int n_dim, vector_1d *q0) {
    this->G = G;
    this->n_dim = n_dim;
    this->q0 = q0;

    for (int i=0; i < n_dim; i++)
        this->tmp_q.push_back(NAN);

    if (q0 == nullptr) {
        this->_q0 = vector_1d(n_dim, 0.);
        this->q0 = &this->_q0;
    }
}

void BasePotential::shift_rotate_q(double *q) {
    for (int i=0; i < this->n_dim; i++) {
        this->tmp_q[i] = q[i] - (*this->q0)[i];
    }

    // TODO: deal with rotation...s
}

// These are the methods that are overridden by subclasses
// Note: If I don't include these, I get a "Symbol not found" error on import of cpp_gala._potential
double BasePotential::_density(double *q, double t) { return NAN; }
double BasePotential::_energy(double *q, double t) { return NAN; }
void BasePotential::_gradient(double *q, double t, double *grad) { }

// These are the methods that should be called by anything that actually needs to use potentials
double BasePotential::density(double *q, double t) {
    this->shift_rotate_q(q);
    return _density(this->tmp_q.data(), t);
}

double BasePotential::energy(double *q, double t) {
    this->shift_rotate_q(q);
    return _energy(this->tmp_q.data(), t);
}

void BasePotential::gradient(double *q, double t, double *grad) {
    this->shift_rotate_q(q);
    _gradient(this->tmp_q.data(), t, grad);
}

void BasePotential::acceleration(double *q, double t, double *acc) {
    std::vector<double> tmp_grad(this->n_dim, 0.);

    gradient(q, t, &tmp_grad[0]);
    for (int i=0; i < this->n_dim; i++) {
        acc[i] = acc[i] - tmp_grad[i];
    }
}


// Potential implementations
KeplerPotential::KeplerPotential(double G, BasePotentialParameter *M, int n_dim,
                                 vector_1d *q0)
: BasePotential(G, n_dim, q0) {
    parameters.insert(std::make_pair("M", M));
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
    return - this->G * parameters["M"]->get_value(t) / r;
}

void KeplerPotential::_gradient(double *q, double t, double *grad) {
    double r = gala::utils::xyz_to_r(q);
    double GM = this->G * parameters["M"]->get_value(t);
    double fac = GM / pow(r, 3);

    grad[0] += fac * q[0];
    grad[1] += fac * q[1];
    grad[2] += fac * q[2];

}


}} // namespace: gala::potential
