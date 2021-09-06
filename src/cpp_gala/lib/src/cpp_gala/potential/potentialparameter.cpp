// Standard
#include <cmath>

// Third-party
#include <gsl/gsl_spline.h>

// This package
#include <cpp_gala/potential/potentialparameter.h>

using namespace gala::potential;

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
    InterpolatedPotentialParameter
*/
InterpolatedPotentialParameter::InterpolatedPotentialParameter(double *times, double *vals,
                                                               int ntimes, int interp_order) {
    this->ntimes = ntimes;

    // GSL stuff:
    this->acc = gsl_interp_accel_alloc();
    if (interp_order == 1) {
        this->spline = gsl_spline_alloc(gsl_interp_linear, ntimes);
    } else if (interp_order == 3) {
        this->spline = gsl_spline_alloc(gsl_interp_cspline, ntimes);
    }

    gsl_spline_init(this->spline, times, vals, ntimes);
}

double InterpolatedPotentialParameter::get_value(double t) {
    double val = gsl_spline_eval(this->spline, t, this->acc);
    return val;
}

InterpolatedPotentialParameter::~InterpolatedPotentialParameter() {
    // free gsl shit
    gsl_spline_free(this->spline);
    gsl_interp_accel_free(this->acc);
}
