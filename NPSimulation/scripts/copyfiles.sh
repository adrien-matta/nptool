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
# * Decription: This script loops on all subdirectories and make a symbolic   *
# *             link of headers and source files to the include and src       *
# *             directories, respectively                                     *
# *             Files are copied *only* if the detector has been selected     *
# *             during the NPLib configuration step                           *
# *                                                                           *
# *---------------------------------------------------------------------------*
# * Comment:                                                                  *
# *                                                                           *
# *                                                                           *
# *****************************************************************************

#! /bin/bash

# read .detector_libs file created by the $NPLib/configure script
if [ $# = 0 ] ; then
   file="../NPLib/.detector_libs"
fi ;
read -r detectorlibs < "$file" 

# loop on all sub-directories
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
         echo " + Copying files from $ldir directory..."
         # enter directory
         cd $dir
         # loop on files in directory
	 for file in *
	 do
	    # get file extension
	    ext=${file##*.}
            # test if this is a header file
	    if [ $ext = hh ]; then
	       path="../$dir/$file"
	       ln -s $path ../include
	    # test if this is a source file
	    elif [ $ext = cc ]; then
	       path="../$dir/$file"
	       ln -s $path ../src
	    fi ;
	 done
         # go back to top directory
         cd ../
      fi ;
   fi
done
