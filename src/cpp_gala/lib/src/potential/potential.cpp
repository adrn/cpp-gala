#include <potential/potential.h>
#include <cmath>
#include <iostream>

namespace gala {
namespace potential {

// Utilties: TODO: move these elsewhere
double xyz_to_r(double *q) {
    return sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2]);
}

/*
    Parameters
*/

double BasePotentialParameter::get_value(double t) {
    return NAN;
}


StaticPotentialParameter::StaticPotentialParameter(double val) {
    m_val = val;
}

double StaticPotentialParameter::get_value(double t) {
    return m_val;
}


/*
    Potentials
*/

// Base class
BasePotential::BasePotential(int ndim) {
    m_ndim = ndim;
}

int BasePotential::get_ndim() const {
    return m_ndim;
}

double BasePotential::_density(double *q, double t) { return NAN; }
double BasePotential::_energy(double *q, double t) { return NAN; }


// Potential implementations
KeplerPotential::KeplerPotential(BasePotentialParameter *GM, int ndim)
: BasePotential(ndim) {
    parameters.insert(std::make_pair("GM", GM));
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
    std::cout << typeid(parameters["GM"]).name() << "\n";
    std::cout << parameters["GM"]->get_value(t) << ", " << r << "\n";
    return - parameters["GM"]->get_value(t) / r;
}


}} // namespace: gala::potential
