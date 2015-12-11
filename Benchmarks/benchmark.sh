#!/bin/bash

# exit on error, and verbose
set -ev

# can run the app
npanalysis -h

# cats benchmark 
cd $NPTOOL/Benchmarks/cats
npanalysis -D benchmark_cats.detector -C calibration.txt -R RunToTreat.txt -O benchmark_cats 
echo ".q" | root -b -l ShowResult.C
