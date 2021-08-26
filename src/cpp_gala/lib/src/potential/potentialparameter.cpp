#include <cmath>
#include <potential/potentialparameter.h>

namespace gala { namespace potential {

double BasePotentialParameter::get_value(double t) {
    return NAN;
}

StaticPotentialParameter::StaticPotentialParameter(double val) {
    m_val = val;
}

double StaticPotentialParameter::get_value(double t) {
    return m_val;
}

}} // namespace: gala::potential
