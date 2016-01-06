#!/bin/bash

# exit on error and verbose
set -ev

# go to nps
cd $NPTOOL/Examples/Example2
npsimulation -D Example2.detector -E Example2.reaction -O Example2 -B 28Mg.mac
npanalysis --last-sim -O Example2
echo ".q" | root -b -l ShowResult.C
