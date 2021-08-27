#ifndef _GALA_POTENTIALPARAMETER_H
#define _GALA_POTENTIALPARAMETER_H

#include <string>
#include <gsl/gsl_spline.h>

namespace gala { namespace potential {

class BasePotentialParameter {

    public:
        // Attributes:
        std::string const m_name;

        // Methods:
        virtual double get_value(double t);

};

class StaticPotentialParameter : public BasePotentialParameter {

    public:
        // Attributes:
        double m_val;

        // Constructors:
        // TODO: implement int, long, double, array constructors!
        StaticPotentialParameter(double val);

        // Methods:
        double get_value(double t) override;

};

class InterpolatedPotentialParameter : public BasePotentialParameter {

    public:
        // Attributes:
        double *m_times;
        double *m_vals;
        int m_ntimes;

        // GSL stuff:
        gsl_interp_accel *m_acc;
        gsl_spline *m_spline;

        // Constructors and Destructuor:
        // TODO: implement int, long, double, array constructors!
        InterpolatedPotentialParameter(double *times, double *vals, int ntimes, int interp_order);
        ~InterpolatedPotentialParameter();

        // Methods:
        double get_value(double t) override;

};

}} // namespace: gala::potential

#endif
