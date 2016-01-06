#!/bin/bash

# exit on error, and verbose
set -ev

#####################################################################
# Test that npanalylsis can run
#####################################################################
npanalysis -h


#####################################################################
# Example 1
#####################################################################
# Compile
cd $NPTOOL/Examples/Example1
cmake ./
if [ -e Makefile ] 
then
  make
else
  ninja
fi

# Check whether simulation file exist or not
if [ -e $NPTOOL/NPData/Example1_Simu.root ] 
then
   # Moving reference simulation file to Outputs directory
   cp $NPTOOL/NPData/Example1_Simu.root $NPTOOL/Outputs/Simulation/Example1.root
   # Run npanalysis
   npanalysis -R RunToTreat.txt -O Example1
   # Run the ShowResult ROOT macro
   echo ".q" | root -b -l ShowResult.cxx
else
   # Case where reference simulation file does not exist
   echo "Warning!!! file $NPTOOL/NPData/Example1_Simu.root does not exist"
   echo "Go to the $NPTOOL directory and install it using the following"
   echo "command:"
   echo "git clone https://github.com/adrien-matta/NPData"
fi


#####################################################################
# Example 2
#####################################################################
# Compile Example2
cd $NPTOOL/Examples/Example2
cmake ./
if [ -e Makefile ] 
then
  make
else
  ninja
fi

# Check whether simulation file exist or not
if [ -e $NPTOOL/NPData/Example2_Simu.root ] 
then
   # Moving reference simulation file to Outputs directory
   cp $NPTOOL/NPData/Example2_Simu.root $NPTOOL/Outputs/Simulation/Example2.root
   # Run npanalysis
   npanalysis -R RunToTreat.txt -O Example2
   # Run the ShowResult ROOT macro
   echo ".q" | root -b -l ShowResult.cxx
else
   # Case where reference simulation file does not exist
   echo "Warning!!! file $NPTOOL/NPData/Example1_Simu.root does not exist"
   echo "Go to the $NPTOOL directory and install it using the following"
   echo "command:"
   echo "git clone https://github.com/adrien-matta/NPData"
fi
