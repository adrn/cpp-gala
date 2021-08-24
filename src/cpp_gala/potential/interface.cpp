#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "potential/potential.h"

namespace py = pybind11;
using namespace pybind11::literals;
using namespace gala::potential;


py::array_t<double> density(BasePotential pot,
                            py::array_t<double, py::array::c_style> q,
                            double t) {
    py::buffer_info buf = q.request();

    if (buf.ndim != 2) {
        throw std::runtime_error("numpy.ndarray ndim must be 2!");
    }

    auto result = py::array_t<double>(buf.size);

    // TODO: fill in the result array...

    return result;
}


PYBIND11_MODULE(_potential, m) {
    py::class_<BasePotential>(m, "BasePotential")
        .def(py::init<int>(), "ndim"_a=3)
        .def("get_ndim", &BasePotential::get_ndim);

    py::class_<KeplerPotential>(m, "KeplerPotential")
        .def(py::init<int>(), "ndim"_a=3)
        .def("get_ndim", &KeplerPotential::get_ndim)
        .def("density", &density);

}
