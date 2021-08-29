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

    py::class_<Simulation>(mod, "Simulation")
        .def(py::init<>())
        .def(py::init<gala::potential::BasePotential*>(), "potential"_a)
        .def("add_body", &Simulation::add_body)
        .def_property_readonly("nbodies", &Simulation::get_nbodies);

    py::class_<Body>(mod, "Body")
        .def("__init__", [](
            Body &instance,
            gala::potential::BasePotential *potential,
            py::array_t<double> w,
            std::string name,
            int ndim) {

                py::buffer_info w_buf = w.request();
                double *w_arr = (double*)w_buf.ptr;

                if (w_buf.ndim != 2){
                    throw std::runtime_error(
                        "Input Body phase-space coordinates w must be a 2D array");
                }

                new (&instance) Body(
                    potential, &w_arr[0], w_buf.shape[0], name, ndim);
            }, "potential"_a, "w"_a, "name"_a = "", "ndim"_a = DEFAULT_NDIM
        )
        .def_property_readonly("nbodies", [](Body &body) {
            return body.nbodies;
        })
        .def_property_readonly("ndim", [](Body &body) {
            return body.ndim;
        })
        .def_property_readonly("name", [](Body &body) {
            return body.name;
        });

}
