/*
    TODO:
*/

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <cpp_gala/integrate/integrate.h>
#include <cpp_gala/simulation/simulation.h>
#include <cpp_gala/utils.h>
#include <iostream>

namespace py = pybind11;
using namespace pybind11::literals;
using namespace gala::integrate;

using array_t = py::array_t<double, py::array::c_style | py::array::forcecast>;

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

                auto w = array_t({(int)t_vec.size(),
                                  self.sim.n_particles,
                                  2 * self.sim.n_dim});
                py::buffer_info w_buf = w.request();
                double *w_ptr = (double*)w_buf.ptr;

                // Actually do the integration: fills buffer
                self.integrate(t_vec, w_ptr);

                return w;
        });

    py::class_<BoostIntegrator, BaseIntegrator>(mod, "BoostIntegrator")
        .def(py::init<gala::simulation::Simulation, std::string, int>(),
             "sim"_a, "choice"_a, "sub_steps"_a=0)
        .def("integrate", [](
            BoostIntegrator &self,
            array_t t) {
                // Array of times:
                std::vector<double> t_vec;
                t_vec.assign(t.data(), t.data() + t.size());

                // Actually do the integration:
                auto w_vec = self.integrate(t_vec);

                // Copy data to an array:
                auto w_arr = array_t({(int)t_vec.size(),
                                      self.sim.n_particles,
                                      2 * self.sim.n_dim});
                auto w_arr_m = w_arr.mutable_unchecked();

                for (int i=0; i < t_vec.size(); i++) {
                    for (int j=0; j < self.sim.n_particles; j++) {
                        for (int k=0; k < 2 * self.sim.n_dim; k++) {
                            w_arr_m(i, j, k) = w_vec[i][j][k];
                        }
                    }
                }

                return w_arr;
        });

}
