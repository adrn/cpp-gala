// Standard library
#include <iostream>
#include <cmath>

// This package
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/potential/potentialparameter.h>
#include <cpp_gala/potential/builtin.h>
#include <cpp_gala/utils.h>

using namespace gala::utils;
using namespace gala::potential;

/*
    Kepler
*/
KeplerPotential::KeplerPotential(BasePotentialParameter &M, double G, vector_1d &q0)
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
HernquistPotential::HernquistPotential(BasePotentialParameter &M, BasePotentialParameter &a,
                                       double G, vector_1d &q0)
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
