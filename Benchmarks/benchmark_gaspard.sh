#!/bin/bash

# exit on error, and verbose
set -ev

# cats benchmark 
cd $NPTOOL/Benchmarks/gaspard
time npsimulation -D benchmark_gaspard.detector -E 132Sndp_benchmark.reaction -O benchmark_gaspard -B batch.mac
root -l ShowResults.C
