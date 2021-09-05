/*
    TODO:
    - Use templating to enable float versions of these, for speed? May not be worth it now...
*/

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/simulation.h>
#include <cpp_gala/simulation/particle.h>
#include <cpp_gala/utils.h>
#include <iostream>

namespace py = pybind11;
using namespace pybind11::literals;
using namespace gala::simulation;
using namespace gala::utils;

using array_t = py::array_t<double, py::array::c_style | py::array::forcecast>;

PYBIND11_MODULE(_simulation, mod) {
    py::module::import("cpp_gala._potential");

    py::class_<ParticleCollection>(mod, "ParticleCollection")
        .def("__init__", [](
            ParticleCollection &self,
            array_t w,
            gala::potential::BasePotential *potential,
            std::string name) {
                if (w.ndim() != 2){
                    throw std::runtime_error(
                        "Input ParticleCollection phase-space coordinates w must be a 2D array");
                }

                vector_2d w_vec(w.shape(0), vector_1d(w.shape(1)));
                for (int i=0; i < w.shape(0); i++)
                    w_vec[i].assign(w.data(i), w.data(i) + w.shape(1));

                new (&self) ParticleCollection(w_vec, potential, name);
            }, "w"_a, "potential"_a = NULL, "name"_a = ""
        )
        .def_property_readonly("n_particles", [](ParticleCollection &pc) {
            return pc.n_particles;
        })
        .def_property_readonly("n_dim", [](ParticleCollection &pc) {
            return pc.n_dim;
        })
        .def_property_readonly("name", [](ParticleCollection &pc) {
            return pc.name;
        });

    py::class_<Simulation>(mod, "Simulation")
        .def(py::init<>())
        .def(py::init<gala::potential::BasePotential*>(), "potential"_a)
        .def("add_particle", &Simulation::add_particle)
        .def_property_readonly("n_particles", [](Simulation &self){ return self.n_particles; })
        .def_property_readonly("n_dim", [](Simulation &self){ return self.n_dim; })
        .def_property_readonly("state_t", [](Simulation &self){ return self.state_time; })
        .def_property_readonly("state_w", [](Simulation &self) {
                auto w_vec = self.state_w;

                // Array to return:
                auto w = array_t(self.n_particles * 2 * self.n_dim);
                for (int i=0; i < self.n_particles; i++) {
                    for (int j=0; j < 2 * self.n_dim; j++) {
                        w.mutable_at(j + 2 * self.n_dim * i) = w_vec[i][j];
                    }
                }
                w.resize({self.n_particles, 2 * self.n_dim});

                return w;
            }
        )
        .def("get_dwdt", [](
            Simulation &self) {
                auto dwdt_vec = self.get_dwdt();

                // Array to return:
                auto dwdt = array_t(self.n_particles * 2 * self.n_dim);
                for (int i=0; i < self.n_particles; i++) {
                    for (int j=0; j < 2 * self.n_dim; j++) {
                        dwdt.mutable_at(j + 2 * self.n_dim * i) = dwdt_vec[i][j];
                    }
                }
                dwdt.resize({self.n_particles, 2 * self.n_dim});

                return dwdt;
            }
        );
}
