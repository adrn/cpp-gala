#include <iostream>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/potential/potentialparameter.h>
#include <cpp_gala/utils.h>

namespace gala { namespace potential {

// Base class
BasePotential::BasePotential(double G, int ndim) {
    this->G = G;
    this->ndim = ndim;
}

int BasePotential::get_ndim() const { return this->ndim; }
double BasePotential::get_G() const { return this->G; }

double BasePotential::_density(double *q, double t) { return NAN; }
double BasePotential::_energy(double *q, double t) { return NAN; }
void BasePotential::_gradient(double *q, double t, double *grad) { }


// Potential implementations
KeplerPotential::KeplerPotential(double G, BasePotentialParameter *M, int ndim)
: BasePotential(G, ndim) {
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
    grad[0] = fac * q[0];
    grad[1] = fac * q[1];
    grad[2] = fac * q[2];
}


}} // namespace: gala::potential
