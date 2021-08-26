from glob import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

# sorted(glob("src/cpp_gala/potential/*.cpp")),
ext_modules = [
    Pybind11Extension(
        "cpp_gala._potential",
        ["src/cpp_gala/potential/interface.cpp",
         "src/cpp_gala/lib/src/potential/potential.cpp",
         "src/cpp_gala/lib/src/potential/potentialparameter.cpp"],
        cxx_std=11,
        include_dirs=['src/cpp_gala/lib/include']
    ),
]

setup(
    ext_modules=ext_modules
)
