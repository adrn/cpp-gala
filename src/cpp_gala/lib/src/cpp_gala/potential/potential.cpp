/*
    TODO:
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

/*
    Base mixin class
*/

BasePotential::BasePotential() { }

// These are the methods that are overridden by subclasses
// Note: If I don't include these, I get a "Symbol not found" error on import of cpp_gala._potential
void BasePotential::set_q0(vector_1d &q0) {}

double BasePotential::_density(double *q, double t) { return NAN; }
double BasePotential::_energy(double *q, double t) { return NAN; }
void BasePotential::_gradient(double *q, double t, double *grad) {}

double BasePotential::density(double *q, double t) { return NAN; }
double BasePotential::energy(double *q, double t) { return NAN; }
void BasePotential::gradient(double *q, double t, double *grad) {}
void BasePotential::acceleration(double *q, double t, double *acc) {
    std::vector<double> tmp_grad(this->n_dim, 0.);

    gradient(q, t, &tmp_grad[0]);
    for (int i=0; i < this->n_dim; i++) {
        acc[i] = acc[i] - tmp_grad[i];
    }
}


/*
    Base class
*/
BaseBuiltinPotential::BaseBuiltinPotential() { }

BaseBuiltinPotential::BaseBuiltinPotential(double G, vector_1d &q0) {
    this->G = G;
    this->n_dim = DEFAULT_n_dim; // override this in subclasses if necessary
    this->q0 = q0;

    for (int i=0; i < this->n_dim; i++)
        this->tmp_q.push_back(NAN);

    if (q0.size() != this->n_dim)
        throw std::invalid_argument("Size of q0 must equal n_dim");
}

void BaseBuiltinPotential::shift_rotate_q(double *q) {
    for (int i=0; i < this->n_dim; i++)
        this->tmp_q[i] = q[i] - this->q0[i];

    // TODO: deal with rotation...
}

void BaseBuiltinPotential::set_q0(vector_1d &q0) {
    this->q0 = q0;
}

// These are the methods that should be called by anything that actually needs to use potentials
double BaseBuiltinPotential::density(double *q, double t) {
    this->shift_rotate_q(q);
    return this->_density(this->tmp_q.data(), t);
}

double BaseBuiltinPotential::energy(double *q, double t) {
    this->shift_rotate_q(q);
    return this->_energy(this->tmp_q.data(), t);
}

void BaseBuiltinPotential::gradient(double *q, double t, double *grad) {
    this->shift_rotate_q(q);
    this->_gradient(this->tmp_q.data(), t, grad);
}

/*
    Composite Potential
*/
CompositePotential::CompositePotential() {
    this->n_potentials = 0;
    this->G = 0.;
    this->n_dim = 0;
}

void CompositePotential::add_potential(std::string name, BaseBuiltinPotential &pot) {
    // TODO: Also store G, check against any added pot.G
    if (this->potentials.count(name) != 0) {
        throw std::invalid_argument(
            "Potential '" + name + "' already exists in this CompositePotential: Potential "
            "components must have a unique name per CompositePotential instance.");
    }

    if (this->n_potentials == 0) {
        this->n_dim = pot.n_dim;
        this->G = pot.G;
    } else if (this->n_dim != pot.n_dim) {
        throw std::runtime_error(
            "Input Potential must have the same n_dim as other potentials in this "
            "CompositePotential instance.");
    } else if (this->G != pot.G) {
        throw std::runtime_error(
            "Input Potential must have the unit system (i.e. value of G) as other potentials in "
            "this CompositePotential instance.");
    }

    this->potentials.insert(std::make_pair(name, &pot));
    this->n_potentials++;
}

void CompositePotential::set_q0(vector_1d &q0) {
    for (auto &pair : this->potentials)
        pair.second->set_q0(q0);
}

double CompositePotential::density(double *q, double t) {
    double val = 0.;
    for (auto &pair : this->potentials)
        val += pair.second->density(q, t);
    return val;
}

double CompositePotential::energy(double *q, double t) {
    double val = 0.;
    for (auto &pair : this->potentials)
        val += pair.second->energy(q, t);
    return val;
}

void CompositePotential::gradient(double *q, double t, double *grad) {
    for (auto &pair : this->potentials)
        pair.second->gradient(q, t, grad);
}

}} // namespace: gala::potential
