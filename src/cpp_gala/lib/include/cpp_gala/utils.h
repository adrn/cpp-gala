#ifndef _GALA_UTILS_H
#define _GALA_UTILS_H

#include <string>
#include <cmath>

namespace gala { namespace utils {

inline double xyz_to_r(double *q) {
    return sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2]);
}

using vector_3d = std::vector<std::vector<std::vector<double>>>;
using vector_2d = std::vector<std::vector<double>>;
using vector_1d = std::vector<double>;

inline int i2d(int i, int j, int ni, int nj) {
    return j + i * nj;
}

inline int i3d(int i, int j, int k, int ni, int nj, int nk) {
    return k + nk * (j + i * nj);
}

}} // namespace: gala::utils

#endif
