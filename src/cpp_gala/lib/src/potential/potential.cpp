#include <potential/potential.h>
#include <potential/potentialparameter.h>
#include <cmath>
#include <iostream>

namespace gala { namespace potential {

// Utilties: TODO: move these elsewhere
double xyz_to_r(double *q) {
    return sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2]);
}

// Base class
BasePotential::BasePotential(double G, int ndim) {
    m_G = G;
    m_ndim = ndim;
}

int BasePotential::get_ndim() const { return m_ndim; }

double BasePotential::_density(double *q, double t) { return NAN; }
double BasePotential::_energy(double *q, double t) { return NAN; }
void BasePotential::_gradient(double *q, double t, double *grad) { }


// Potential implementations
KeplerPotential::KeplerPotential(double G, BasePotentialParameter *M, int ndim)
: BasePotential(G, ndim) {
    parameters.insert(std::make_pair("M", M));
}

double KeplerPotential::_density(double *q, double t) {
    double r = xyz_to_r(q);
    if (r == 0) {
        return INFINITY;
    } else {
        return 0.;
    }
}

double KeplerPotential::_energy(double *q, double t) {
    double r = xyz_to_r(q);
    return - m_G * parameters["M"]->get_value(t) / r;
}

void KeplerPotential::_gradient(double *q, double t, double *grad) {
    double r = xyz_to_r(q);
    double GM = m_G * parameters["M"]->get_value(t);
    double fac = GM / pow(r, 3);
    grad[0] = fac * q[0];
    grad[1] = fac * q[1];
    grad[2] = fac * q[2];
}


}} // namespace: gala::potential
