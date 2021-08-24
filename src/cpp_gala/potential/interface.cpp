#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <potential/potential.h>
#include <iostream>

namespace py = pybind11;
using namespace pybind11::literals;
using namespace gala::potential;


void test() {
    KeplerPotential pot(3);
    double q[3] = {1., 2., 3.};
    pot._density(&q[0], 0.);
}


py::array_t<double> density(BasePotential *pot,
                            py::array_t<double, py::array::c_style> q,
                            double t) {
    py::buffer_info q_buf = q.request();
    double *q_arr = (double*)q_buf.ptr;

    if (q_buf.ndim != 2) {
        throw std::runtime_error("numpy.ndarray ndim must be 2!");
    }

    auto result = py::array_t<double>(q_buf.shape[0]);
    py::buffer_info result_buf = result.request();
    double *result_arr = (double*)result_buf.ptr;

    for (int i=0; i < q_buf.shape[0]; i++) {
        result_arr[i] = pot->_density(&q_arr[i], t);
    }

    return result;
}


PYBIND11_MODULE(_potential, m) {
    py::class_<BasePotential>(m, "BasePotential")
        .def(py::init<int>(), "ndim"_a=3)
        .def("get_ndim", &BasePotential::get_ndim);

    py::class_<KeplerPotential, BasePotential>(m, "KeplerPotential")
        .def(py::init<int>(), "ndim"_a=3)
        .def("get_ndim", &KeplerPotential::get_ndim)
        .def("density", &density);

}
