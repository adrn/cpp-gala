from glob import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

# sorted(glob("src/cpp_gala/potential/*.cpp")),
ext_modules = [
    Pybind11Extension(
        "cpp_gala._potential",
        ["src/cpp_gala/potential/interface.cpp",
         "src/cpp_gala/lib/src/cpp_gala/potential/potential.cpp",
         "src/cpp_gala/lib/src/cpp_gala/potential/potentialparameter.cpp"],
        cxx_std=11,
        include_dirs=['src/cpp_gala/lib/include'],
        libraries=['gsl']
    ),
    Pybind11Extension(
        "cpp_gala._simulation",
        ["src/cpp_gala/simulation/interface.cpp",
         "src/cpp_gala/lib/src/cpp_gala/simulation/simulation.cpp",
         "src/cpp_gala/lib/src/cpp_gala/simulation/body.cpp"],
        cxx_std=11,
        include_dirs=['src/cpp_gala/lib/include'],
        libraries=['gsl']
    ),
    Pybind11Extension(
        "cpp_gala._integrate",
        ["src/cpp_gala/integrate/interface.cpp",
         "src/cpp_gala/lib/src/cpp_gala/integrate/integrate.cpp"],
        cxx_std=11,
        include_dirs=['src/cpp_gala/lib/include'],
        libraries=['gsl']
    ),
]

setup(
    ext_modules=ext_modules
)
