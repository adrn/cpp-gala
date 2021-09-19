// Third-party
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

// This package
#include <cpp_gala/integrate/integrate.h>
#include <cpp_gala/simulation/simulation.h>
#include <cpp_gala/utils.h>

namespace py = pybind11;
using namespace pybind11::literals;
using namespace gala::integrate;

using array_t = py::array_t<double, py::array::c_style | py::array::forcecast>;

PYBIND11_MODULE(_integrate, mod) {
    py::module::import("cpp_gala._simulation");

    // TODO: do we need to expose this...?
    py::class_<BaseIntegrator>(mod, "BaseIntegrator")
        .def(py::init<gala::simulation::Simulation&>(), "sim"_a)
        .def_property_readonly("sim", [](BaseIntegrator &self) {
            return self.sim;
        });

    py::class_<LeapfrogIntegrator, BaseIntegrator>(mod, "LeapfrogIntegrator")
        .def(py::init<gala::simulation::Simulation&>(), "sim"_a)
        .def("integrate", [](
            LeapfrogIntegrator &self,
            array_t t,
            bool save_all) {
                // Array of times:
                std::vector<double> t_vec;
                t_vec.assign(t.data(), t.data() + t.size());

                int nparticles = self.sim->get_n_particles();

                array_t w;
                if (save_all == true) {
                    w = array_t({(int)t_vec.size(),
                                 nparticles,
                                 2 * self.sim->n_dim});
                } else {
                    w = array_t({nparticles,
                                 2 * self.sim->n_dim});
                }

                py::buffer_info w_buf = w.request();
                double *w_ptr = (double*)w_buf.ptr;

                // Actually do the integration:
                if (save_all == true)
                    self.integrate_save_all(t_vec, w_ptr);
                else
                    self.integrate(t_vec, w_ptr);

                return w;

        }, "t"_a, "save_all"_a = true);

    py::class_<BoostIntegrator, BaseIntegrator>(mod, "BoostIntegrator")
        .def(py::init<gala::simulation::Simulation&, std::string, int>(),
             "sim"_a, "choice"_a, "sub_steps"_a=0)
        .def("integrate", [](
            BoostIntegrator &self,
            array_t t,
            bool save_all) {
                // Array of times:
                std::vector<double> t_vec;
                t_vec.assign(t.data(), t.data() + t.size());

                int nparticles = self.sim->get_n_particles();

                array_t w;
                if (save_all == true) {
                    w = array_t({(int)t_vec.size(),
                                 nparticles,
                                 2 * self.sim->n_dim});
                } else {
                    w = array_t({nparticles,
                                 2 * self.sim->n_dim});
                }

                py::buffer_info w_buf = w.request();
                double *w_ptr = (double*)w_buf.ptr;

                // Actually do the integration:
                if (save_all == true)
                    self.integrate_save_all(t_vec, w_ptr);
                else
                    self.integrate(t_vec, w_ptr);

                return w;

        }, "t"_a, "save_all"_a = true);

}
