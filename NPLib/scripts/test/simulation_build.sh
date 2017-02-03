#!/bin/bash

# exit on error and verbose
set -ev

# go to nps
cd $NPTOOL/NPSimulation

# Check if ninja or ninja-build is there, otherwise fall back on make
if hash ninja-build 2>/dev/null; then
  cmake -GNinja ./
  ninja-build install
elif hash ninja 2>/dev/null; then
  cmake -GNinja ./
  ninja install
else
  cmake ./
  make install
fi

# minimal run
npsimulation -h
