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
        uint16_t n_dim;
        double G; // value of G in the unit system

        // Constructors and Destructors:
        BasePotential();

        // Methods:
        virtual void set_q0(vector_1d &q0);

        virtual double density(double *q, double t);
        virtual double energy(double *q, double t);
        virtual void gradient(double *q, double t, double *grad);
        void acceleration(double *q, double t, double *grad);
        // void _hessian(double *q, double t, double *hess);

        // To be overridden by subclasses:
        virtual double _density(double *q, double t);
        virtual double _energy(double *q, double t);
        virtual void _gradient(double *q, double t, double *grad);

};

// Note: could change PYBIND11_EXPORT to __attribute__((visibility("default"))) if we want to remove
// pybind11 dependence in here
class PYBIND11_EXPORT BaseBuiltinPotential : public BasePotential {

    public:
        // Attributes:
        vector_1d q0; // the origin of the potential
        std::map<std::string, BasePotentialParameter*> parameters;
        vector_1d tmp_q; // used for storing shifted/rotated q values

        // Constructors and Destructors:
        BaseBuiltinPotential();
        BaseBuiltinPotential(double G, vector_1d &q0);

        // Methods:
        void shift_rotate_q(double *q);
        void set_q0(vector_1d &q0) override;

        double density(double *q, double t) override;
        double energy(double *q, double t) override;
        void gradient(double *q, double t, double *grad) override;
        // void hessian(double *q, double t, double *hess);
};

class CompositePotential : public BasePotential {
    public:
        // Attributes:
        std::map<std::string, BasePotential*> potentials;
        uint16_t n_potentials;

        // Constructors and Destructors:
        CompositePotential();

        // Methods:
        void add_potential(std::string name, BaseBuiltinPotential &pot);
        void set_q0(vector_1d &q0) override;

        double density(double *q, double t) override;
        double energy(double *q, double t) override;
        void gradient(double *q, double t, double *grad) override;
        // void acceleration(double *q, double t, double *grad) override;
        // void _hessian(double *q, double t, double *hess);
};

}} // namespace: gala::potential

#endif
