#!/bin/bash

# Script will clone andcompile raytracer.

# Usage example:
#   bash try_raytracer.sh

# PREREQUISITES: bash, git, pkg-config, ...

ROOT_DIR=$(pwd)

# - clone repositories -
if [ ! -d cclib ]
then
  git clone https://github.com/izuzanak/cclib.git
fi

if [ ! -d raytracer ]
then
  git clone https://github.com/izuzanak/raytracer.git
fi

# - compile cclib -
mkdir -p cclib/build
cd cclib/build
cmake .. &&
make -j $(nproc)

cd $ROOT_DIR

# - compile raytracer -
mkdir -p raytracer/build
cd raytracer/build
cmake "-DCCLIB_DIR=${ROOT_DIR}/cclib" .. &&
make -j $(nproc)

cd $ROOT_DIR

# - render scene -
#raytracer/build/rt_demo/rt_demo raytracer/tests/data/rt_demo/scene.json scene.bmp
#raytracer/build/rt_demo/rt_demo raytracer/tests/data/rt_demo/spheres.json spheres.bmp

