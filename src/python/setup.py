# setup.py (with automatic dependency tracking)
from setuptools import setup

setup(
    name='matlib',
    version='1.0',
    packages=['matlib'],
    setup_requires=['cffi>=1.0.0'],
    cffi_modules=['build.py:ffi'],
    install_requires=['cffi>=1.0.0'],
)
