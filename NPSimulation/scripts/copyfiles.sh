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

# read .detector_libs or .core_libs file created by the configure script
if [ $# = 0 ] ; then
   file="../NPLib/.detector_libs"
fi ;
read -r detectorlibs < "$file" 

# loop recursively on Makefile files in sub-directories
for dir in *
do
   # only treat directories
   if [ -d "$dir" ]; then
      dir=$(basename "$dir")
      # file name in lower case
      ldir=$(echo "$dir"  | tr '[A-Z]' '[a-z]')
      # only copy files associated to defined detector libraries
      if echo "$detectorlibs" | grep -q "$ldir" ; then
         # print informations
         echo "\tCopying files from $ldir directory..."
         # enter directory
         cd $dir
         # copy include files
         cp *.hh ../include
         # copy source files
         cp *.cc ../src
         # go back to top directory
         cd ../
      fi ;
   fi
done
