#ifndef _GALA_POTENTIAL_H
#define _GALA_POTENTIAL_H

// Standard
#include <string>
#include <map>
#include <vector>

// Third-party
#include <pybind11/pybind11.h>

// This package
#include <cpp_gala/potential/potentialparameter.h>
#include <cpp_gala/utils.h>

#define DEFAULT_n_dim 3

using namespace gala::utils;

namespace gala { namespace potential {

// Note: could change PYBIND11_EXPORT to __attribute__((visibility("default"))) if we want to remove
// pybind11 dependence in here
class PYBIND11_EXPORT BasePotential {

    public:
        // Attributes:
        int n_dim; // phase-space dimensionality, 3 in most cases
        double G; // value of G in the unit system
        vector_1d *q0; // the origin of the potential
        std::map<std::string, BasePotentialParameter*> parameters;

        // Constructors and Destructors:
        BasePotential();
        BasePotential(double G,
                      int n_dim=DEFAULT_n_dim,
                      vector_1d *q0=nullptr);

        // Methods::
        void shift_rotate_q(double *q);

        virtual double _density(double *q, double t);
        virtual double _energy(double *q, double t);
        virtual void _gradient(double *q, double t, double *grad);

        double density(double *q, double t);
        double energy(double *q, double t);
        void gradient(double *q, double t, double *grad);
        void acceleration(double *q, double t, double *grad);
        // void _hessian(double *q, double t, double *hess);

    private:
        vector_1d _q0; // used for storing default q0 values
        vector_1d tmp_q; // used for storing shifted/rotated q values

};

class KeplerPotential : public BasePotential {

    public:
        // Constructors:
        KeplerPotential(double G,
                        BasePotentialParameter *M,
                        int n_dim=DEFAULT_n_dim,
                        vector_1d *q0=nullptr);

        // Methods::
        double _density(double *q, double t) override;
        double _energy(double *q, double t) override;
        void _gradient(double *q, double t, double *grad) override;
        // void _hessian(double *q, double t, double *hess) override;
};

}} // namespace: gala::potential

#endif
