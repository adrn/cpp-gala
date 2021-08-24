#include "potential/potential.h"

namespace gala {
namespace potential {

BasePotential::BasePotential(int ndim) {
    m_ndim = ndim;
}

int BasePotential::get_ndim() const {
    return m_ndim;
}

}} // namespace: gala::potential
