#!/usr/bin/env python3

from setuptools import find_packages
from setuptools import setup

setup(
    name="PiFan",
    version='0.2',
    description='Raspberry Pi CPU fan management utilites',
    author='Brenden Davidson',
    author_email='davidson.brenden15@gmail.com',
    license='./LICENSE',
    packages=find_packages('src'),
    package_dir={'':'src'}
)