#!/bin/bash

# exit on error and verbose
set -ev

# go to nps
cd $NPTOOL/NPSimulation
cmake ./
if [ -e Makefile ] 
then
  make
else
  ninja
fi
# minimal run
npsimulation -h
