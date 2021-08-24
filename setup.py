from glob import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

# sorted(glob("src/cpp_gala/potential/*.cpp")),
ext_modules = [
    Pybind11Extension(
        "cpp_gala.potential._test",
        ["src/cpp_gala/potential/test.cpp"],
        cxx_std=11
    ),
]

setup(
    ext_modules=ext_modules
)
