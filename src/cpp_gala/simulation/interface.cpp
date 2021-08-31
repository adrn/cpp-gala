/*
    TODO:
    - Use templating to enable float versions of these, for speed? May not be worth it now...
*/

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/simulation.h>
#include <cpp_gala/simulation/body.h>
#include <iostream>

namespace py = pybind11;
using namespace pybind11::literals;
using namespace gala::simulation;


PYBIND11_MODULE(_simulation, mod) {
    py::module::import("cpp_gala._potential");

    py::class_<BodyCollection>(mod, "BodyCollection")
        .def("__init__", [](
            BodyCollection &self,
            py::array_t<double> w,
            gala::potential::BasePotential *potential,
            std::string name,
            int ndim) {

                py::buffer_info w_buf = w.request();
                double *w_arr = (double*)w_buf.ptr;

                if (w_buf.ndim != 2){
                    throw std::runtime_error(
                        "Input BodyCollection phase-space coordinates w must be a 2D array");
                }

                new (&self) BodyCollection(
                    potential, &w_arr[0], w_buf.shape[0], name, ndim);
            }, "w"_a, "potential"_a = NULL, "name"_a = "", "ndim"_a = DEFAULT_NDIM
        )
        .def_property_readonly("nbodies", [](BodyCollection &body) {
            return body.nbodies;
        })
        .def_property_readonly("ndim", [](BodyCollection &body) {
            return body.ndim;
        })
        .def_property_readonly("name", [](BodyCollection &body) {
            return body.name;
        });

    py::class_<Simulation>(mod, "Simulation")
        .def(py::init<>())
        .def(py::init<gala::potential::BasePotential*>(), "potential"_a)
        .def("add_body", &Simulation::add_body)
        .def_property_readonly("nbodies", &Simulation::get_nbodies)
        .def("acceleration", [](
            Simulation &self,
            BodyCollection *body,
            double t) {
                // Array to return:
                auto acc = py::array_t<double>(body->nbodies * body->ndim);
                acc.resize({body->nbodies, body->ndim});

                py::buffer_info acc_buf = acc.request();
                double *acc_arr = (double*)acc_buf.ptr;

                self.get_acceleration(body, t, acc_arr);

                return acc;
            }
        );
}
