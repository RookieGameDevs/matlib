from setuptools import setup

setup(
    name='matlib',
    version='0.1+6716ff9',
    packages=['matlib'],
    setup_requires=['cffi>=1.0.0'],
    cffi_modules=['build.py:ffi'],
    install_requires=['cffi>=1.0.0'],

    # metadata for PyPI
    author='Ivan Nikolaev',
    author_email='voidexp@gmail.com',
    description='Python wrapper for matlib C library',
    license='BSD',
    keywords='math matrix vector linear algebra',
    url='https://github.com/RookieGameDevs/matlib')
