from glob import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

# sorted(glob("src/cpp_gala/potential/*.cpp")),
ext_modules = [
    Pybind11Extension(
        "cpp_gala._potential",
        ["src/cpp_gala/potential/interface.cpp",
         "src/cpp_gala/lib/src/cpp_gala/potential/potential.cpp",
         "src/cpp_gala/lib/src/cpp_gala/potential/potentialparameter.cpp",
         "src/cpp_gala/lib/src/cpp_gala/potential/builtin.cpp",],
        cxx_std=14,
        include_dirs=['src/cpp_gala/lib/include'],
        libraries=['boost_system']
    ),
    Pybind11Extension(
        "cpp_gala._simulation",
        ["src/cpp_gala/simulation/interface.cpp",
         "src/cpp_gala/lib/src/cpp_gala/simulation/simulation.cpp",
         "src/cpp_gala/lib/src/cpp_gala/simulation/particle.cpp"],
        cxx_std=14,
        include_dirs=['src/cpp_gala/lib/include'],
        # libraries=['gsl']
    ),
    Pybind11Extension(
        "cpp_gala._integrate",
        ["src/cpp_gala/integrate/interface.cpp",
         "src/cpp_gala/lib/src/cpp_gala/integrate/integrate.cpp"],
        cxx_std=14,
        include_dirs=['src/cpp_gala/lib/include'],
        libraries=['boost_system']
    ),
]

setup(
    ext_modules=ext_modules
)
