// Third-party
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>  // automatic type conversions

// This package
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/potential/potentialparameter.h>
#include <cpp_gala/potential/builtin.h>

namespace py = pybind11;
using namespace pybind11::literals;
using namespace gala::potential;

using array_t = py::array_t<double, py::array::c_style | py::array::forcecast>;

void validate_pot_q(BasePotential &pot, array_t q) {
    if (q.ndim() != 2)
        throw std::runtime_error("numpy.ndarray n_dim must be 2!");
    else if (pot.n_dim != q.shape(1))
        throw std::runtime_error(
            "Input position dimensionality must be the same as the potential dimensionality");
}

vector_1d q0_helper(array_t q0, int n_dim) {
    vector_1d q0_vec(q0.data(), q0.data() + q0.size());

    if (std::isnan(q0_vec[0])) {
        q0_vec.clear();
        for (int i=0; i<n_dim; i++)
            q0_vec.push_back(0.);
    }

    return q0_vec;
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

    py::class_<EquiInterpPotentialParameter, BasePotentialParameter>(
            mod, "EquiInterpPotentialParameter")
        .def("__init__", [](
            EquiInterpPotentialParameter &self,
            array_t times,
            array_t vals) {

                vector_1d times_vec;
                times_vec.assign(times.data(), times.data() + times.size());

                vector_1d vals_vec;
                vals_vec.assign(vals.data(), vals.data() + vals.size());

                if (times_vec.size() != vals_vec.size())
                    throw std::runtime_error("times and vals arrays must have the same size");

                new (&self) EquiInterpPotentialParameter(times_vec, vals_vec);
            }
        )
        .def("get_value", &EquiInterpPotentialParameter::get_value);

    py::class_<NonEquiInterpPotentialParameter, BasePotentialParameter>(
            mod, "NonEquiInterpPotentialParameter")
        .def("__init__", [](
            NonEquiInterpPotentialParameter &self,
            array_t times,
            array_t vals) {

                vector_1d times_vec;
                times_vec.assign(times.data(), times.data() + times.size());

                vector_1d vals_vec;
                vals_vec.assign(vals.data(), vals.data() + vals.size());

                if (times_vec.size() != vals_vec.size())
                    throw std::runtime_error("times and vals arrays must have the same size");

                new (&self) NonEquiInterpPotentialParameter(times_vec, vals_vec);
            }
        )
        .def("get_value", &NonEquiInterpPotentialParameter::get_value);

    /*
        BasePotential
    */
    py::class_<BasePotential>(mod, "BasePotential")
        .def_property_readonly("n_dim", [](BasePotential &pot) { return pot.n_dim; })
        .def_property_readonly("G", [](BasePotential &pot) { return pot.G; })
        .def("density", [](
            BasePotential &self,
            array_t q,
            double t) {
                validate_pot_q(self, q);

                auto result = array_t(q.shape(0));
                double *q_ptr = q.mutable_data();
                double *result_ptr = result.mutable_data();

                for (int i=0; i < q.shape(0); i++)
                    result_ptr[i] = self.density(&q_ptr[self.n_dim * i], t);

                return result;
        })
        .def("energy", [](
            BasePotential &self,
            array_t q,
            double t) {
                validate_pot_q(self, q);

                auto result = array_t(q.shape(0));
                double *q_ptr = q.mutable_data();
                double *result_ptr = result.mutable_data();

                for (int i=0; i < q.shape(0); i++)
                    result_ptr[i] = self.energy(&q_ptr[self.n_dim * i], t);

                return result;
        })
        .def("gradient", [](
            BasePotential &self,
            array_t q,
            double t) {
                validate_pot_q(self, q);

                auto result = array_t(q.size());
                result.resize({q.shape(0), q.shape(1)});

                double *q_ptr = q.mutable_data();
                double *result_ptr = result.mutable_data();

                // Zero out the values:
                for (int i=0; i < result.size(); i++)
                    result_ptr[i] = 0.;

                for (int i=0; i < q.shape(0); i++)
                    self.gradient(&q_ptr[self.n_dim * i], t, &result_ptr[self.n_dim * i]);

                return result;
        })
        .def("acceleration", [](
            BasePotential &self,
            array_t q,
            double t) {
                validate_pot_q(self, q);

                auto result = array_t(q.size());
                result.resize({q.shape(0), q.shape(1)});

                double *q_ptr = q.mutable_data();
                double *result_ptr = result.mutable_data();

                // Zero out the values:
                for (int i=0; i < result.size(); i++)
                    result_ptr[i] = 0.;

                for (int i=0; i < q.shape(0); i++)
                    self.acceleration(&q_ptr[self.n_dim * i], t, &result_ptr[self.n_dim * i]);

                return result;
        });

    /*
        BaseBuiltinPotential
    */
    py::class_<BaseBuiltinPotential, BasePotential>(mod, "BaseBuiltinPotential")
        .def(py::init<double, vector_1d&>(), "G"_a, "q0"_a=py::none())
        .def_property_readonly("q0", [](BaseBuiltinPotential &pot) {
            return py::array(pot.q0.size(), pot.q0.data());
        })
        .def_property_readonly("parameters", [](BaseBuiltinPotential &pot) {
            return pot.parameters;
        });

    /*
        Composite potential
    */
    py::class_<CompositePotential, BasePotential>(mod, "CompositePotential")
        .def(py::init<>())
        .def("add_potential", [](
            CompositePotential &self,
            std::string name,
            BaseBuiltinPotential &pot) {
                self.add_potential(name, pot);
            }
        );

    /*
        Built-in potential classes
    */
    py::class_<KeplerPotential, BaseBuiltinPotential>(mod, "KeplerPotential")
        .def("__init__", [](
            KeplerPotential &self,
            BasePotentialParameter &m,
            double G,
            array_t q0) {
                auto q0_vec = q0_helper(q0, DEFAULT_n_dim);
                new (&self) KeplerPotential(m, G, q0_vec);

            }, "m"_a, "G"_a, "q0"_a=py::none()
        );

    py::class_<HernquistPotential, BaseBuiltinPotential>(mod, "HernquistPotential")
        .def("__init__", [](
            HernquistPotential &self,
            BasePotentialParameter &m,
            BasePotentialParameter &a,
            double G,
            array_t q0) {
                auto q0_vec = q0_helper(q0, DEFAULT_n_dim);
                new (&self) HernquistPotential(m, a, G, q0_vec);

            }, "m"_a, "a"_a, "G"_a, "q0"_a=py::none()
        );

}
