/*
    TODO:
    - Use templating to enable float versions of these, for speed? May not be worth it now...
*/

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/potential/potentialparameter.h>
#include <iostream>

namespace py = pybind11;
using namespace pybind11::literals;
using namespace gala::potential;


// void test() {
//     StaticPotentialParameter m(1.);
//     KeplerPotential pot(&m, 3);
//     double q[3] = {1., 2., 3.};
//     pot._density(&q[0], 0.);
// }


py::array_t<double> density(BasePotential *pot,
                            py::array_t<double, py::array::c_style> q,
                            double t) {
    // TODO: make a validate function to check q array and pot ndim?
    py::buffer_info q_buf = q.request();
    double *q_arr = (double*)q_buf.ptr;
    int q_ndim = q_buf.shape[1];

    if (q_buf.ndim != 2) {
        throw std::runtime_error("numpy.ndarray ndim must be 2!");
    } else if (pot->m_ndim != q_ndim) {
        throw std::runtime_error("Input position dimensionality must be the same "
                                 "as the potential dimensionality");
    }

    auto result = py::array_t<double>(q_buf.shape[0]);
    py::buffer_info result_buf = result.request();
    double *result_arr = (double*)result_buf.ptr;

    for (int i=0; i < q_buf.shape[0]; i++) {
        result_arr[i] = pot->_density(&q_arr[q_ndim * i], t);
    }

    return result;
}


py::array_t<double> energy(BasePotential *pot,
                           py::array_t<double, py::array::c_style> q,
                           double t) {
    // TODO: make a validate function to check q array and pot ndim?
    py::buffer_info q_buf = q.request();
    double *q_arr = (double*)q_buf.ptr;
    int q_ndim = q_buf.shape[1];

    if (q_buf.ndim != 2) {
        throw std::runtime_error("numpy.ndarray ndim must be 2!");
    } else if (pot->m_ndim != q_ndim) {
        throw std::runtime_error("Input position dimensionality must be the same "
                                 "as the potential dimensionality");
    }

    auto result = py::array_t<double>(q_buf.shape[0]);
    py::buffer_info result_buf = result.request();
    double *result_arr = (double*)result_buf.ptr;

    for (int i=0; i < q_buf.shape[0]; i++) {
        result_arr[i] = pot->_energy(&q_arr[q_ndim * i], t);
    }

    return result;
}


py::array_t<double> gradient(BasePotential *pot,
                             py::array_t<double, py::array::c_style> q,
                             double t) {
    // TODO: make a validate function to check q array and pot ndim?
    py::buffer_info q_buf = q.request();
    double *q_arr = (double*)q_buf.ptr;
    int q_ndim = q_buf.shape[1];

    if (q_buf.ndim != 2) {
        throw std::runtime_error("numpy.ndarray ndim must be 2!");
    } else if (pot->m_ndim != q_ndim) {
        throw std::runtime_error("Input position dimensionality must be the same "
                                 "as the potential dimensionality");
    }

    auto result = py::array_t<double>(q_buf.size);
    result.resize({q_buf.shape[0], q_buf.shape[1]});

    py::buffer_info result_buf = result.request();
    double *result_arr = (double*)result_buf.ptr;

    for (int i=0; i < q_buf.shape[0]; i++) {
        pot->_gradient(&q_arr[q_ndim * i], t, &result_arr[q_ndim * i]);
    }

    return result;
}


PYBIND11_MODULE(_potential, mod) {
    // TODO: do we need to expose this...?
    py::class_<BasePotentialParameter>(mod, "BasePotentialParameter")
        .def("get_value", &BasePotentialParameter::get_value);

    py::class_<StaticPotentialParameter, BasePotentialParameter>(mod, "StaticPotentialParameter")
        .def(py::init<double>(), "val"_a)
        .def("get_value", &StaticPotentialParameter::get_value);

    py::class_<InterpolatedPotentialParameter, BasePotentialParameter>(
            mod, "InterpolatedPotentialParameter")
        .def("__init__",
            [](InterpolatedPotentialParameter &instance,
               py::array_t<double> times,
               py::array_t<double> vals,
               int interp_order) {

                py::buffer_info times_buf = times.request();
                double *times_arr = (double*)times_buf.ptr;

                py::buffer_info vals_buf = vals.request();
                double *vals_arr = (double*)vals_buf.ptr;

                if (times_buf.size != vals_buf.size) {
                    throw std::runtime_error("times and vals arrays must have the same size");
                } else if ((times_buf.ndim != 1) || (vals_buf.ndim != 1)){
                    throw std::runtime_error("times and vals arrays must be 1D arrays");
                }

                new (&instance) InterpolatedPotentialParameter(
                    &times_arr[0], &vals_arr[0], times_buf.size, interp_order);
        })
        .def("get_value", &InterpolatedPotentialParameter::get_value);

    // TODO: do we need to expose this...?
    py::class_<BasePotential>(mod, "BasePotential")
        .def(py::init<double, int>(), "G"_a, "ndim"_a=DEFAULT_NDIM)
        .def("get_ndim", &BasePotential::get_ndim);

    py::class_<KeplerPotential, BasePotential>(mod, "KeplerPotential")
        .def(py::init<double, BasePotentialParameter*, int>(), "G"_a, "m"_a, "ndim"_a=DEFAULT_NDIM)
        .def("get_ndim", &KeplerPotential::get_ndim)
        .def("density", &density)
        .def("energy", &energy)
        .def("gradient", &gradient);

}
