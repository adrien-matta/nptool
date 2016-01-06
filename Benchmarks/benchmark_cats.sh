#!/bin/bash

# exit on error, and verbose
set -ev

# cats benchmark 
cd $NPTOOL/Benchmarks/cats
time npanalysis -D benchmark_cats.detector -C calibration.txt -R RunToTreat.txt -O benchmark_cats 
root -l ShowResults.C
