#include <pybind11/pybind11.h>
#include "potential/potential.h"

namespace py = pybind11;
using namespace pybind11::literals;
using namespace gala::potential;

PYBIND11_MODULE(_potential, m) {
    py::class_<BasePotential>(m, "BasePotential")
        .def(py::init<int>(), "ndim"_a=3)
        .def("get_ndim", &BasePotential::get_ndim);
}
