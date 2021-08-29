/*
    TODO:
    - Use templating to enable float versions of these, for speed? May not be worth it now...
*/

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/simulation.h>
#include <iostream>

namespace py = pybind11;
using namespace pybind11::literals;
using namespace gala::simulation;


PYBIND11_MODULE(_simulation, mod) {

    // TODO: do we need to expose this...?
    py::class_<Simulation>(mod, "Simulation")
        .def(py::init<gala::potential::BasePotential*>(), "potential"_a=NULL)
        .def("add_body", &Simulation::add_body)
        .def_property_readonly("nbodies", &Simulation::get_nbodies);

}