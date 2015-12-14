#!/bin/bash

# exit on error, and verbose
set -ev

# can run the app
npanalysis -h

# Compile Example1 
cd $NPTOOL/Examples/Example1
cmake ./
if [ -e Makefile ] 
then
  make
else
  ninja
fi
# Moving reference test simulation
cp $NPTOOL/Examples/Example1/Example1_Simu.root $NPTOOL/Outputs/Simulation/Example1.root
npanalysis -R RunToTreat.txt -O Example1

echo ".q" | root -b -l ShowResult.cxx

# Compile Example2
cd $NPTOOL/Examples/Example2
cmake ./
if [ -e Makefile ] 
then
  make
else
  ninja
fi
cp $NPTOOL/Examples/Example2/Example2_Simu.root $NPTOOL/Outputs/Simulation/Example2.root
npanalysis -R RunToTreat.txt -O Example2
echo ".q" | root -b -l ShowResult.cxx

