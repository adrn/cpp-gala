#ifndef _GALA_FRAME_H
#define _GALA_FRAME_H

// Standard
#include <string>
#include <map>
#include <vector>

// Third-party
#include <pybind11/pybind11.h>

// This package
#include <cpp_gala/utils.h>

using namespace gala::utils;

namespace gala { namespace frame {

// Note: could change PYBIND11_EXPORT to __attribute__((visibility("default"))) if we want to remove
// pybind11 dependence in here
class PYBIND11_EXPORT BaseFrame {

    public:
        // Attributes:
        uint16_t n_dim;

        // Constructors and Destructors:
        BaseFrame(uint16_t n_dim);

        // Methods:
        virtual double energy(double *w, double t);
        virtual void get_dwdt(double *w, double t, double *dwdt);
        // virtual void hessian(double *q, double t, double *hess);

};

class StaticFrame : public BaseFrame {

    public:
        // Constructors and Destructors:
        StaticFrame(uint16_t n_dim);

};

class ConstantRotatingFrame : public BaseFrame {

    public:
        // Attributes:
        vector_1d Omega;

        // Constructors and Destructors:
        ConstantRotatingFrame(vector_1d &Omega);

        // Methods:
        double energy(double *w, double t) override;
        void get_dwdt(double *w, double t, double *grad) override;

};

}} // gala::frame

#endif
