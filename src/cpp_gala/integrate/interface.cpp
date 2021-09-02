/*
    TODO:
*/

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <cpp_gala/integrate/integrate.h>
#include <cpp_gala/simulation/simulation.h>
#include <iostream>

namespace py = pybind11;
using namespace pybind11::literals;
using namespace gala::integrate;

using array_t = py::array_t<double, pybind11::array::c_style | pybind11::array::forcecast>;

PYBIND11_MODULE(_integrate, mod) {
    py::module::import("cpp_gala._integrate");

    // TODO: do we need to expose this...?
    py::class_<BaseIntegrator>(mod, "BaseIntegrator")
        .def(py::init<gala::simulation::Simulation>());

}
