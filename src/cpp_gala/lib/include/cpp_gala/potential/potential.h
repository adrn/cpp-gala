#ifndef _GALA_POTENTIAL_H
#define _GALA_POTENTIAL_H

#include <string>
#include <map>
#include <cpp_gala/potential/potentialparameter.h>

#define DEFAULT_NDIM 3

namespace gala { namespace potential {

class __attribute__((visibility("default"))) BasePotential {

    public:
        // Attributes:
        int ndim;  // phase-space dimensionality, 3 in most cases
        double G;  // value of G in the unit system
        std::map<std::string, BasePotentialParameter*> parameters;

        // Constructors:
        BasePotential(double G,
                      int ndim=DEFAULT_NDIM);

        // Methods::
        int get_ndim() const;
        double get_G() const;

        virtual double _density(double *q, double t);
        virtual double _energy(double *q, double t);
        virtual void _gradient(double *q, double t, double *grad);
        // void _hessian(double *q, double t, double *hess);

};

class KeplerPotential : public BasePotential {

    public:
        // Constructors:
        KeplerPotential(double G,
                        BasePotentialParameter *M,
                        int ndim=DEFAULT_NDIM);

        // Methods::
        double _density(double *q, double t) override;
        double _energy(double *q, double t) override;
        void _gradient(double *q, double t, double *grad) override;
        // void _hessian(double *q, double t, double *hess) override;
};

}} // namespace: gala::potential

#endif
