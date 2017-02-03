#!/bin/bash

# exit on error and verbose
set -ev

# Run basic test
$NPTOOL/NPLib/scripts/test/basic.sh
# Run compilation of npsimulation
$NPTOOL/NPLib/scripts/test/simulation_build.sh
# Run npsimulation on Example2 and then analysis on Example2
$NPTOOL/NPLib/scripts/test/simulation_run.sh
