#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "potential/potential.h"

namespace py = pybind11;
using namespace pybind11::literals;
using namespace gala::potential;

// TODO: implement density function that takes a numpy array and does the thing _density
// py::array_t<double, py::array::c_style

PYBIND11_MODULE(_potential, m) {
    py::class_<BasePotential>(m, "BasePotential")
        .def(py::init<int>(), "ndim"_a=3)
        .def("get_ndim", &BasePotential::get_ndim);
}
