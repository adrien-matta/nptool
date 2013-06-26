# *****************************************************************************
# * Copyright (C) 2009   this file is part of the NPTool Project              *
# *                                                                           *
# * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
# * For the list of contributors see $NPTOOL/Licence/Contributors             *
# *****************************************************************************/

# *****************************************************************************
# * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
# *                                                                           *
# * Creation Date  : 26/06/13                                                 *
# * Last update    :                                                          *
# *---------------------------------------------------------------------------*
# * Decription: This script is in charge of deleting the headers and source   *
# *             files associated with any detector found in the $NPS/include  *
# *             and $NPS/src directories.                                     *
# *                                                                           *
# *---------------------------------------------------------------------------*
# * Comment:                                                                  *
# *                                                                           *
# *                                                                           *
# *****************************************************************************

#! /bin/bash

incsrcdir="include src"

# print informations
echo " + Cleaning include directory from detectors' header files..."

# loop recursively on include files in sub-directories
for file in */*.hh
do
   header=$(basename "$file")
   dir=$(dirname "$file")
   # directory name in lower case
   ldir=$(echo "$dir"  | tr '[A-Z]' '[a-z]')
   # delete files associated to all detector libraries
   if ! echo "$incsrcdir" | grep -q "$ldir" ; then
      # enter directory
      cd include
      # delete include files
      rm -f $header
      # go back to top directory
      cd ../
   fi ;
done


# print informations
echo " + Cleaning src directory from detectors' source files..."

# loop recursively on include files in sub-directories
for file in */*.cc
do
   src=$(basename "$file")
   dir=$(dirname "$file")
   # directory name in lower case
   ldir=$(echo "$dir"  | tr '[A-Z]' '[a-z]')
   # only delete files associated to defined detector libraries
   if ! echo "$incsrcdir" | grep -q "$ldir" ; then
      # enter directory
      cd src
      # delete include files
      rm -f $src
      # go back to top directory
      cd ../
   fi ;
done
