#ifndef _GALA_POTENTIALPARAMETER_H
#define _GALA_POTENTIALPARAMETER_H

// Standard
#include <string>

// Third-party
#include <pybind11/pybind11.h>
#include <boost/math/interpolators/barycentric_rational.hpp>
#include <boost/math/interpolators/cardinal_cubic_b_spline.hpp>

// This package
#include <cpp_gala/utils.h>

using namespace gala::utils;
using namespace boost::math;

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

class EquiInterpPotentialParameter : public BasePotentialParameter {

    public:
        // Attributes:
        vector_1d times;
        vector_1d vals;
        interpolators::cardinal_cubic_b_spline<double> *interp;

        // Constructors and Destructuor:
        EquiInterpPotentialParameter(vector_1d times, vector_1d vals);
        ~EquiInterpPotentialParameter();

        // Methods:
        double get_value(double t) override;

};

class NonEquiInterpPotentialParameter : public BasePotentialParameter {

    public:
        // Attributes:
        vector_1d times;
        vector_1d vals;
        barycentric_rational<double> *interp; // TODO: this is an arbitrary choice of interpolator

        // Constructors and Destructuor:
        NonEquiInterpPotentialParameter(vector_1d times, vector_1d vals);

        // Methods:
        double get_value(double t) override;

};


}} // namespace: gala::potential

#endif
