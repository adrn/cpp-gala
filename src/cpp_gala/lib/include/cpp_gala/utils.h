#ifndef _GALA_UTILS_H
#define _GALA_UTILS_H

#include <string>
#include <cmath>

namespace gala { namespace utils {

/*
    Type definitions
*/
using vector_3d = std::vector<std::vector<std::vector<double>>>;
using vector_2d = std::vector<std::vector<double>>;
using vector_1d = std::vector<double>;

/*
    Coordinate system stuff
*/
inline double xyz_to_r(double *q) {
    return sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2]);
}

inline double xyz_to_r(vector_1d &q) {
    return sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2]);
}

inline vector_1d xyz_to_r(vector_2d &q) {
    vector_1d r;
    for (int i=0; i < q.size(); i++)
        r.push_back(sqrt(q[i][0]*q[i][0] + q[i][1]*q[i][1] + q[i][2]*q[i][2]));
    return r;
}


/*
    Indexing
*/
inline int i2d(int i, int j, int ni, int nj) {
    return j + i * nj;
}

inline int i3d(int i, int j, int k, int ni, int nj, int nk) {
    return k + nk * (j + i * nj);
}

}} // namespace: gala::utils

#endif
