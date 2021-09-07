#ifndef _GALA_BUILTIN_H
#define _GALA_BUILTIN_H

// Standard
#include <string>
#include <map>
#include <vector>

// Third-party
#include <pybind11/pybind11.h>

// This package
#include <cpp_gala/potential/potentialparameter.h>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/utils.h>

using namespace gala::utils;
using namespace gala::potential;

class KeplerPotential : public BasePotential {

    public:
        // Constructors:
        KeplerPotential(BasePotentialParameter &M,
                        double G,
                        vector_1d &q0);

        // Methods::
        double _density(double *q, double t) override;
        double _energy(double *q, double t) override;
        void _gradient(double *q, double t, double *grad) override;
        // void _hessian(double *q, double t, double *hess) override;
};

class HernquistPotential : public BasePotential {

    public:
        // Constructors:
        HernquistPotential(BasePotentialParameter &M,
                           BasePotentialParameter &a,
                           double G,
                           vector_1d &q0);

        // Methods::
        double _density(double *q, double t) override;
        double _energy(double *q, double t) override;
        void _gradient(double *q, double t, double *grad) override;
        // void _hessian(double *q, double t, double *hess) override;
};

#endif
