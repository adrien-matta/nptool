# *****************************************************************************
# * Copyright (C) 2016   this file is part of the NPTool Project              *
# *                                                                           *
# * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
# * For the list of contributors see $NPTOOL/Licence/Contributors             *
# *****************************************************************************/

# *****************************************************************************
# * Original Author: A. Matta  contact address: a.matta@surrey.ac.uk          *
# *                                                                           *
# * Creation Date  : 23/4/2014                                                *
# * Last update    :                                                          *
# *---------------------------------------------------------------------------*
# * Decription: This script edit the user .rootlogon.C file to load NPTool    *
# * Libraries.                                                                *
# *                                                                           *
# *---------------------------------------------------------------------------*
# * Comment:                                                                  *
# *                                                                           *
# *                                                                           *
# *****************************************************************************

#! /bin/bash
# save the current folder information
FOLDER=${PWD}

# simulation
if [ "$1" = "s" ]; then
  cd $NPTOOL/NPSimulation
  if [ -f "build.ninja" ]; then
    ninja install
  elif [ -f "Makefile" ]; then
    make install
  else
    cmake ./
    make install
  fi

# lib
elif [ "$1" = "l" ]; then
  cd $NPTOOL/NPLib
  if [ -f "build.ninja" ]; then
    ninja install
  elif [ -f "Makefile" ]; then
    make install
  else
    cmake ./
    make install
  fi

# all
elif [ "$1" = "a" ]; then
  cd $NPTOOL/NPSimulation
  if [ -f "build.ninja"  ]; then
    ninja install
  elif [ -f "Makefile" ]; then
    make install
  else
    cmake ./
    make install
  fi

  cd $NPTOOL/NPLib
  if [ -f "build.ninja"  ]; then
    ninja install
  elif [ -f "Makefile" ]; then
    make install
  else
    cmake ./
    make install
  fi

  # current folder 
  cd $FOLDER
  echo "Folder is $PWD"
  if [ -f "build.ninja"  ]; then
    ninja 
  elif [ -f "Makefile" ]; then
    make 
  else
    echo "no build in current folder"
  fi

else
  echo "Unexpected argument: $1"
  echo "Expecting : s (simulation) , l (library) or a (all)"
fi

# return to original folder
cd $FOLDER

