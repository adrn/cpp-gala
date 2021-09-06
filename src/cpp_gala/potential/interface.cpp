// Third-party
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

// This package
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/potential/potentialparameter.h>

namespace py = pybind11;
using namespace pybind11::literals;
using namespace gala::potential;

using array_t = py::array_t<double, py::array::c_style | py::array::forcecast>;


array_t density(BasePotential *pot, array_t q, double t) {
    // TODO: make a validate function to check q array and pot n_dim?
    py::buffer_info q_buf = q.request();
    double *q_arr = (double*)q_buf.ptr;
    int q_n_dim = q_buf.shape[1];

    if (q_buf.ndim != 2) {
        throw std::runtime_error("numpy.ndarray n_dim must be 2!");
    } else if (pot->n_dim != q_n_dim) {
        throw std::runtime_error("Input position dimensionality must be the same "
                                 "as the potential dimensionality");
    }

    auto result = array_t(q_buf.shape[0]);
    py::buffer_info result_buf = result.request();
    double *result_arr = (double*)result_buf.ptr;

    for (int i=0; i < q_buf.shape[0]; i++) {
        result_arr[i] = pot->density(&q_arr[q_n_dim * i], t);
    }

    return result;
}


array_t energy(BasePotential *pot, array_t q, double t) {
    // TODO: make a validate function to check q array and pot n_dim?
    py::buffer_info q_buf = q.request();
    double *q_arr = (double*)q_buf.ptr;
    int q_n_dim = q_buf.shape[1];

    if (q_buf.ndim != 2) {
        throw std::runtime_error("numpy.ndarray n_dim must be 2!");
    } else if (pot->n_dim != q_n_dim) {
        throw std::runtime_error("Input position dimensionality must be the same "
                                 "as the potential dimensionality");
    }

    auto result = array_t(q_buf.shape[0]);
    py::buffer_info result_buf = result.request();
    double *result_arr = (double*)result_buf.ptr;

    for (int i=0; i < q_buf.shape[0]; i++) {
        result_arr[i] = pot->energy(&q_arr[q_n_dim * i], t);
    }

    return result;
}


array_t gradient(BasePotential *pot, array_t q, double t) {
    // TODO: make a validate function to check q array and pot n_dim?
    py::buffer_info q_buf = q.request();
    double *q_arr = (double*)q_buf.ptr;
    int q_n_dim = q_buf.shape[1];

    if (q_buf.ndim != 2) {
        throw std::runtime_error("numpy.ndarray n_dim must be 2!");
    } else if (pot->n_dim != q_n_dim) {
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
        pot->gradient(&q_arr[q_n_dim * i], t, &result_arr[q_n_dim * i]);
    }

    return result;
}

array_t acceleration(BasePotential *pot, array_t q, double t) {
    return - gradient(pot, q, t);
}


PYBIND11_MODULE(_potential, mod) {
    /*
        PotentialParameters
    */
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

    /*
        Potentials
    */
    py::class_<BasePotential>(mod, "BasePotential")
        .def(py::init<double, int, vector_1d*>(),
             "G"_a, "n_dim"_a=DEFAULT_n_dim, "q0"_a=py::none());

    // TODO: how much of this boilerplate needs to be copy-pasta'd for each subclass? is there a
    // simpler way?
    py::class_<KeplerPotential, BasePotential>(mod, "KeplerPotential")
        .def("__init__", [](
            BasePotential &self,
            double G,
            BasePotentialParameter* m,
            int n_dim,
            array_t q0) {
                vector_1d q0_vec;
                q0_vec.assign(q0.data(), q0.data() + q0.size());

                if (std::isnan(q0_vec[0])) {
                    new (&self) KeplerPotential(G, m, n_dim);
                } else {
                    new (&self) KeplerPotential(G, m, n_dim, &q0_vec);
                }

            }, "G"_a, "m"_a, "n_dim"_a=DEFAULT_n_dim, "q0"_a=py::none()
        )
        .def_property_readonly("n_dim", [](KeplerPotential &pot) { return pot.n_dim; })
        .def_property_readonly("G", [](KeplerPotential &pot) { return pot.G; })
        .def_property_readonly("q0", [](KeplerPotential &pot) {
            return py::array(pot.q0->size(), pot.q0->data());
        })
        .def("density", &density)
        .def("energy", &energy)
        .def("gradient", &gradient)
        .def("acceleration", &acceleration);

}
