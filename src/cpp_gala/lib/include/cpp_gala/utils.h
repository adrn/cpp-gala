#ifndef _GALA_UTILS_H
#define _GALA_UTILS_H

#include <string>
#include <cmath>
#include <iostream>

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
inline double norm(double *q, int size) {
    double val = 0;
    for (int i=0; i < size; i++)
        val += q[i] * q[i];
    return sqrt(val);
}

inline double norm(vector_1d &q) {
    double val = 0;
    for (int i=0; i < q.size(); i++)
        val += q[i] * q[i];
    return sqrt(val);
}

inline vector_1d norm(vector_2d &q) {
    vector_1d r;
    for (int i=0; i < q.size(); i++)
        r.push_back(norm(q[i])); // sqrt(q[i][0]*q[i][0] + q[i][1]*q[i][1] + q[i][2]*q[i][2]));
    return r;
}

inline vector_1d cross(vector_1d &w) {
    vector_1d z(3, NAN);
    z[0] = w[1] * w[5] - w[2] * w[4];
    z[1] = -w[0] * w[5] + w[2] * w[3];
    z[2] = w[0] * w[4] - w[1] * w[3];
    return z;
}

inline vector_1d cross(vector_1d &x, vector_1d &y) {
    vector_1d z(3, NAN);
    z[0] = x[1] * y[2] - x[2] * y[1];
    z[1] = -x[0] * y[2] + x[2] * y[0];
    z[2] = x[0] * y[1] - x[1] * y[0];
    return z;
}

inline vector_1d get_unit_vector(vector_1d &vec) {
    vector_1d uvec;
    double R = norm(vec);
    for (int i=0; i < vec.size(); i++)
        uvec.push_back(vec[i] / R);
    return uvec;
}

inline vector_1d get_unit_vector(vector_1d &vec, std::size_t begin, std::size_t end) {
    vector_1d uvec;
    double R = norm(&vec[begin], end - begin);
    for (int i=begin; i < end; i++)
        uvec.push_back(vec[i] / R);
    return uvec;
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

template<typename T>
std::vector<T> slice(std::vector<T> const &v, int m, int n)
{
    auto first = v.cbegin() + m;
    auto last = v.cbegin() + n;

    std::vector<T> vec(first, last);
    return vec;
}

/*
    Dev stuff
*/

inline void print_vector_1d(vector_1d &vec) {
    std::string s = "[";
    for (int i=0; i < vec.size(); i++) {
        s.append(std::to_string(vec[i]));
        if (i != (vec.size()-1))
            s.append(", ");
    }
    s.append("]");
    std::cout << s << std::endl;
}

inline void print_vector_2d(vector_2d &vec) {
    std::cout << "[" << std::endl;
    for (int i=0; i < vec.size(); i++) {
        std::cout << "    ";
        print_vector_1d(vec[i]);
    }
    std::cout << "]" << std::endl;
}

}} // namespace: gala::utils

#endif
