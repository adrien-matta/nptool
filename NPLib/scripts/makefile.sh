# *****************************************************************************
# * Copyright (C) 2009   this file is part of the NPTool Project              *
# *                                                                           *
# * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
# * For the list of contributors see $NPTOOL/Licence/Contributors             *
# *****************************************************************************/

# *****************************************************************************
# * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
# *                                                                           *
# * Creation Date  : 16/11/10                                                 *
# * Last update    :                                                          *
# *---------------------------------------------------------------------------*
# * Decription: This script loops on all subdirectories containing a Makefile *
# *             and call it with the argument passed to the script.           *
# *             Supported arguments are:                                      *
# *               + no arguments: compile everything                          *
# *               + clean: remove temporary files but not shared library      *
# *               + distclean: remove temporary files and shared library      *
# *                                                                           *
# *---------------------------------------------------------------------------*
# * Comment:                                                                  *
# *                                                                           *
# *                                                                           *
# *****************************************************************************

#! /bin/bash

if [ $# = 0 ] ; then
   echo " + Building detector libraries....."
else
   echo " + Cleaning $2 libraries....."
fi ;

# read .detector_libs or .core_libs file created by the configure script
file=".""$2_libs"
if [ $# = 0 ] ; then
   file=".detector_libs"
fi ;
read -r detectorlibs < "$file" 

# loop recursively on Makefile files in sub-directories
for file in */Makefile
do
   # remove "Makefile" string from file name
   name=${file%\/*}
   # file name in lower case
   lname=$(echo "$name"  | tr '[A-Z]' '[a-z]')
   # only build defined detector libraries
   if echo "$detectorlibs" | grep -q "$lname" ; then
      # print informations
      echo "\tEntering $name directory..."
      # add "-C ./" pattern at the beginning of the name
      cmd="-C ./$name"
      # execute make command with target specified on command line
      make --silent $1 $cmd
   fi ;
done
