#include <cmath>
#include <gsl/gsl_spline.h>
#include <cpp_gala/potential/potentialparameter.h>

namespace gala { namespace potential {

double BasePotentialParameter::get_value(double t) {
    return NAN;
}

/*
    StaticPotentialParameter
*/
StaticPotentialParameter::StaticPotentialParameter(double val) {
    m_val = val;
}

double StaticPotentialParameter::get_value(double t) {
    return m_val;
}

/*
    InterpolatedPotentialParameter
*/
InterpolatedPotentialParameter::InterpolatedPotentialParameter(double *times, double *vals,
                                                               int ntimes, int interp_order) {
    m_ntimes = ntimes;

    // GSL stuff:
    m_acc = gsl_interp_accel_alloc();
    if (interp_order == 1) {
        m_spline = gsl_spline_alloc(gsl_interp_linear, ntimes);
    } else if (interp_order == 3) {
        m_spline = gsl_spline_alloc(gsl_interp_cspline, ntimes);
    }

    gsl_spline_init(m_spline, times, vals, ntimes);
}

double InterpolatedPotentialParameter::get_value(double t) {
    double val = gsl_spline_eval(m_spline, t, m_acc);
    return val;
}

InterpolatedPotentialParameter::~InterpolatedPotentialParameter() {
    // free gsl shit
    gsl_spline_free(m_spline);
    gsl_interp_accel_free(m_acc);
}

}} // namespace: gala::potential
