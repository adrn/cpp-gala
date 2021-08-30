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

using array_t = py::array_t<double, pybind11::array::c_style | pybind11::array::forcecast>;


// void test() {
//     StaticPotentialParameter m(1.);
//     KeplerPotential pot(&m, 3);
//     double q[3] = {1., 2., 3.};
//     pot._density(&q[0], 0.);
// }


array_t density(BasePotential *pot, array_t q, double t) {
    // TODO: make a validate function to check q array and pot ndim?
    py::buffer_info q_buf = q.request();
    double *q_arr = (double*)q_buf.ptr;
    int q_ndim = q_buf.shape[1];

    if (q_buf.ndim != 2) {
        throw std::runtime_error("numpy.ndarray ndim must be 2!");
    } else if (pot->ndim != q_ndim) {
        throw std::runtime_error("Input position dimensionality must be the same "
                                 "as the potential dimensionality");
    }

    auto result = array_t(q_buf.shape[0]);
    py::buffer_info result_buf = result.request();
    double *result_arr = (double*)result_buf.ptr;

    for (int i=0; i < q_buf.shape[0]; i++) {
        result_arr[i] = pot->density(&q_arr[q_ndim * i], t);
    }

    return result;
}


array_t energy(BasePotential *pot, array_t q, double t) {
    // TODO: make a validate function to check q array and pot ndim?
    py::buffer_info q_buf = q.request();
    double *q_arr = (double*)q_buf.ptr;
    int q_ndim = q_buf.shape[1];

    if (q_buf.ndim != 2) {
        throw std::runtime_error("numpy.ndarray ndim must be 2!");
    } else if (pot->ndim != q_ndim) {
        throw std::runtime_error("Input position dimensionality must be the same "
                                 "as the potential dimensionality");
    }

    auto result = array_t(q_buf.shape[0]);
    py::buffer_info result_buf = result.request();
    double *result_arr = (double*)result_buf.ptr;

    for (int i=0; i < q_buf.shape[0]; i++) {
        result_arr[i] = pot->energy(&q_arr[q_ndim * i], t);
    }

    return result;
}


array_t gradient(BasePotential *pot, array_t q, double t) {
    // TODO: make a validate function to check q array and pot ndim?
    py::buffer_info q_buf = q.request();
    double *q_arr = (double*)q_buf.ptr;
    int q_ndim = q_buf.shape[1];

    if (q_buf.ndim != 2) {
        throw std::runtime_error("numpy.ndarray ndim must be 2!");
    } else if (pot->ndim != q_ndim) {
        throw std::runtime_error("Input position dimensionality must be the same "
                                 "as the potential dimensionality");
    }

    auto result = array_t(q_buf.size);
    result.resize({q_buf.shape[0], q_buf.shape[1]});

    py::buffer_info result_buf = result.request();
    double *result_arr = (double*)result_buf.ptr;

    // Zero out the values:
    for (int i=0; i < q_buf.size; i++)
        result_arr[i] = 0.;

    for (int i=0; i < q_buf.shape[0]; i++) {
        pot->gradient(&q_arr[q_ndim * i], t, &result_arr[q_ndim * i]);
    }

    return result;
}

array_t acceleration(BasePotential *pot, array_t q, double t) {
    return - gradient(pot, q, t);
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
        .def("__init__", [](
            InterpolatedPotentialParameter &self,
            array_t times,
            array_t vals,
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

                new (&self) InterpolatedPotentialParameter(
                    &times_arr[0], &vals_arr[0], times_buf.size, interp_order);
            }
        )
        .def("get_value", &InterpolatedPotentialParameter::get_value);

    // TODO: do we need to expose this...?
    py::class_<BasePotential>(mod, "BasePotential")
        .def(py::init<double, int, double*>(), "G"_a, "ndim"_a=DEFAULT_NDIM, "q0"_a=NULL);

    // TODO: how much of this boilerplate needs to be copy-pasta'd for each subclass? is there a
    // simpler way?
    py::class_<KeplerPotential, BasePotential>(mod, "KeplerPotential")
        .def("__init__", [](
            BasePotential &self,
            double G,
            BasePotentialParameter* m,
            int ndim,
            array_t q0) {
                py::buffer_info q0_buf = q0.request();
                double *q0_arr = (double*)q0_buf.ptr;

                if (std::isnan(q0_arr[0])) {
                    new (&self) KeplerPotential(G, m, ndim);
                } else {
                    new (&self) KeplerPotential(G, m, ndim, &q0_arr[0]);
                }

            }, "G"_a, "m"_a, "ndim"_a=DEFAULT_NDIM, "q0"_a=py::none()
        )
        .def_property_readonly("ndim", [](KeplerPotential &pot) { return pot.ndim; })
        .def_property_readonly("G", [](KeplerPotential &pot) { return pot.G; })
        .def_property_readonly("q0", [](KeplerPotential &pot) {
            return py::array(pot.ndim, pot.q0);
        })
        .def("density", &density)
        .def("energy", &energy)
        .def("gradient", &gradient)
        .def("acceleration", &acceleration);

}
