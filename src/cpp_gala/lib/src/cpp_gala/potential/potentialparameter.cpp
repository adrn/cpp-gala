// Standard
#include <cmath>

// Third-party
#include <gsl/gsl_spline.h>

// This package
#include <cpp_gala/potential/potentialparameter.h>
#include <cpp_gala/utils.h>

using namespace gala::utils;
using namespace gala::potential;
using namespace boost::math;

double BasePotentialParameter::get_value(double t) {
    return NAN;
}

/*
    StaticPotentialParameter
*/
StaticPotentialParameter::StaticPotentialParameter(double val) {
    this->val = val;
}

double StaticPotentialParameter::get_value(double t) {
    return this->val;
}

/*
    EquiInterpPotentialParameter
*/
EquiInterpPotentialParameter::EquiInterpPotentialParameter(vector_1d times, vector_1d vals) {
    this->times = times;
    this->vals = vals;

    // TODO: validate that times is equi-spaced and increasing...

    interpolators::cardinal_cubic_b_spline<double> *_interp =
        new interpolators::cardinal_cubic_b_spline<double>(vals.data(), vals.size(), times[0],
                                                           times[1] - times[0]);
    this->interp = _interp;
}

EquiInterpPotentialParameter::~EquiInterpPotentialParameter() {
    delete interp;
}

double EquiInterpPotentialParameter::get_value(double t) {
    return (*this->interp)(t);
}

/*
    NonEquiInterpPotentialParameter
*/
NonEquiInterpPotentialParameter::NonEquiInterpPotentialParameter(vector_1d times, vector_1d vals) {
    this->times = times;
    this->vals = vals;

    // TODO: validate that times is increasing...

    boost::math::barycentric_rational<double> *_interp =
        new boost::math::barycentric_rational<double>(std::move(times), std::move(vals));
    this->interp = _interp;
}

double NonEquiInterpPotentialParameter::get_value(double t) {
    return (*this->interp)(t);
}

