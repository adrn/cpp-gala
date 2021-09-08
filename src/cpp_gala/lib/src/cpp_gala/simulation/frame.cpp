// Standard library
#include <iostream>
#include <cmath>

// This package
#include <cpp_gala/simulation/frame.h>
#include <cpp_gala/utils.h>

using namespace gala::utils;
using namespace gala::frame;

/*
    Base class
*/
BaseFrame::BaseFrame(uint16_t n_dim) { this->n_dim = n_dim; }

// These are the methods that are overridden by subclasses
// Note: If I don't include these, I get a "Symbol not found" error on import of cpp_gala._potential
double BaseFrame::energy(double *w, double t) { return 0.; }

void BaseFrame::get_dwdt(double *w, double t, double *dwdt) {}

/*
    Static frame
*/
StaticFrame::StaticFrame(uint16_t n_dim) : BaseFrame(n_dim) {}

/*
    ConstantRotating frame
*/
ConstantRotatingFrame::ConstantRotatingFrame(vector_1d &Omega) : BaseFrame(Omega.size()) {
    this->Omega = Omega;
    if (Omega.size() != 3) {
        throw std::runtime_error("Currently, only 3D rotations are supported.");
    }
}

double ConstantRotatingFrame::energy(double *w, double t) {
    vector_1d L(3);
    L[0] = w[1] * w[2+n_dim] - w[2] * w[1+n_dim];
    L[1] = -w[0] * w[2+n_dim] + w[2] * w[0+n_dim];
    L[2] = w[0] * w[1+n_dim] - w[1] * w[0+n_dim];
    return - (Omega[0] * L[0] + Omega[1] * L[1] + Omega[2] * L[2]);
}

void ConstantRotatingFrame::get_dwdt(double *w, double t, double *dwdt) {
    double Cx, Cy, Cz; // used in cross-products below

    // Omega x q
    Cx = Omega[1] * w[2] - Omega[2] * w[1];
    Cy = -Omega[0] * w[2] + Omega[2] * w[0];
    Cz = Omega[0] * w[1] - Omega[1] * w[0];
    dwdt[0] += -Cx;
    dwdt[1] += -Cy;
    dwdt[2] += -Cz;

    // Omega x p
    Cx = Omega[1] * w[2+n_dim] - Omega[2] * w[1+n_dim];
    Cy = -Omega[0] * w[2+n_dim] + Omega[2] * w[0+n_dim];
    Cz = Omega[0] * w[1+n_dim] - Omega[1] * w[0+n_dim];
    dwdt[3] += -Cx;
    dwdt[4] += -Cy;
    dwdt[5] += -Cz;
}
