#!/usr/bin/env python
# Copyright (2019) Cobalt Speech and Language Inc.

from setuptools import setup

setup(
    name='luna',
    python_requires='>=3.5.0',
    description='This client library is designed to support the Cobalt API for speech synthesis with Luna',
    author='Cobalt Speech and Language Inc.',
    maintainer_email='tech@cobaltspeech.com',
    url='https://github.com/cobaltspeech/sdk-luna',
    packages=["luna"],
    version_format='{tag}.dev{commits}+{sha}',
    setup_requires=['setuptools-git-version'],
    install_requires=[
        'grpcio-tools'
    ]
)
