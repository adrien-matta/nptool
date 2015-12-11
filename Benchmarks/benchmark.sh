#!/bin/bash

# exit on error, and verbose
set -ev

# cats benchmark 
cd $NPTOOL/Benchmarks/cats
time npanalysis -D benchmark_cats.detector -C calibration.txt -R RunToTreat.txt -O benchmark_cats 
echo ".q" | root -b -l ShowResult.cxx
