#!/usr/bin/env python
# Copyright (2019) Cobalt Speech and Language Inc.

from setuptools import setup

setup(
    name='cobalt-luna',
    python_requires='>=3.5.0',
    description='Client SDK library for Cobalt\'s Luna engine.',
    author='Cobalt Speech and Language Inc.',
    author_email='tech@cobaltspeech.com',
    url='https://github.com/cobaltspeech/sdk-luna',
    packages=["luna"],
    version_config={
        "version_format": "{tag}.dev{sha}"
    },
    setup_requires=['better-setuptools-git-version'],
    install_requires=['grpcio-tools']
)
