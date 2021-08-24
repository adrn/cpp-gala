#include "potential/potential.h"
#include <cmath>

namespace gala {
namespace potential {

// Utilties: TODO: move these elsewhere
double xyz_to_r(double *q) {
    return sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2]);
}

// Base class
BasePotential::BasePotential(int ndim) {
    m_ndim = ndim;
}

int BasePotential::get_ndim() const {
    return m_ndim;
}

// Potential implementations
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

    // TODO: use parameters to actually evalute this...

    return 0.;
}


}} // namespace: gala::potential
