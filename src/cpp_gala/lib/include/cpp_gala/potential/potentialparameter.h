#ifndef _GALA_POTENTIALPARAMETER_H
#define _GALA_POTENTIALPARAMETER_H

// Standard
#include <string>

// Third-party
#include <gsl/gsl_spline.h>
#include <pybind11/pybind11.h>

namespace gala { namespace potential {

class PYBIND11_EXPORT BasePotentialParameter {

    public:
        // Attributes:
        std::string const name;

        // Methods:
        virtual double get_value(double t);

};

class StaticPotentialParameter : public BasePotentialParameter {

    public:
        // Attributes:
        double val;

        // Constructors:
        // TODO: implement int, long, double, array constructors!
        StaticPotentialParameter(double val);

        // Methods:
        double get_value(double t) override;

};

class InterpolatedPotentialParameter : public BasePotentialParameter {

    public:
        // Attributes:
        double *times;
        double *vals;
        int ntimes;

        // GSL stuff:
        gsl_interp_accel *acc;
        gsl_spline *spline;

        // Constructors and Destructuor:
        // TODO: implement int, long, double, array constructors!
        InterpolatedPotentialParameter(double *times, double *vals, int ntimes, int interp_order);
        ~InterpolatedPotentialParameter();

        // Methods:
        double get_value(double t) override;

};

}} // namespace: gala::potential

#endif
