#ifndef _GALA_UTILS_H
#define _GALA_UTILS_H

#include <string>
#include <cmath>

namespace gala { namespace utils {

double xyz_to_r(double *q) {
    return sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2]);
}

}} // namespace: gala::utils

#endif
