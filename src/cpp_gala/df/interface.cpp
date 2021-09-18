// Third-party
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

// This package
#include <cpp_gala/df/streamdf.h>
#include <cpp_gala/potential/potential.h>
#include <cpp_gala/simulation/simulation.h>
#include <cpp_gala/simulation/particle.h>
#include <cpp_gala/utils.h>

namespace py = pybind11;
using namespace pybind11::literals;
using namespace gala::df;

using array_t = py::array_t<double, py::array::c_style | py::array::forcecast>;

PYBIND11_MODULE(_df, mod) {
    py::module::import("cpp_gala._simulation");
    py::module::import("cpp_gala._potential");

    // TODO: do we need to expose this...?
    py::class_<BaseStreamDF>(mod, "BaseStreamDF")
        .def(py::init<bool, bool, int>(), "lead"_a, "trail"_a, "seed"_a)
        .def("sample", [](
            BaseStreamDF &self,
            gala::potential::BasePotential &potential,
            gala::simulation::ParticleCollection &progenitor,
            uint32_t n_particles,
            double t) {
                auto result_vec = self.sample(potential, progenitor, n_particles, t);

                // Array to return:
                auto result_arr = array_t(result_vec.size() * 2 * potential.n_dim);
                for (int i=0; i < result_vec.size(); i++) {
                    for (int j=0; j < 2 * potential.n_dim; j++) {
                        result_arr.mutable_at(j + 2 * potential.n_dim * i) = result_vec[i][j];
                    }
                }
                result_arr.resize({int(result_vec.size()), 2 * potential.n_dim});

                return result_arr;
            },
        "potential"_a, "progenitor"_a, "n_particles"_a, "t"_a);

    py::class_<StreaklineStreamDF, BaseStreamDF>(mod, "StreaklineStreamDF")
        .def(py::init<bool, bool, int>(), "lead"_a, "trail"_a, "seed"_a);

}
