/*
    TODO:
*/

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <cpp_gala/integrate/integrate.h>
#include <cpp_gala/simulation/simulation.h>
#include <iostream>

namespace py = pybind11;
using namespace pybind11::literals;
using namespace gala::integrate;

using array_t = py::array_t<double, pybind11::array::c_style | pybind11::array::forcecast>;

PYBIND11_MODULE(_integrate, mod) {
    py::module::import("cpp_gala._simulation");

    // TODO: do we need to expose this...?
    py::class_<BaseIntegrator>(mod, "BaseIntegrator")
        .def(py::init<gala::simulation::Simulation>(), "sim"_a)
        .def_property_readonly("sim", [](BaseIntegrator &self) {
            return self.sim;
        });

    py::class_<LeapfrogIntegrator, BaseIntegrator>(mod, "LeapfrogIntegrator")
        .def(py::init<gala::simulation::Simulation>(), "sim"_a)
        .def("integrate", [](
            LeapfrogIntegrator &self,
            array_t t) {
                // Array of times:
                std::vector<double> t_vec;
                t_vec.assign(t.data(), t.data() + t.size());

                auto w = self.integrate(t_vec);
                // auto w_arr = py::array(w.size(), &w[0][0][0]);
                std::cout << t_vec.size() << " " << self.sim.get_n_bodies() << " " << self.sim.n_dim << "\n";
                // w_arr.resize({t_vec.size(), self.sim.get_n_bodies(), self.sim.n_dim});
                // return w_arr;
                return py::none();
        });

}
