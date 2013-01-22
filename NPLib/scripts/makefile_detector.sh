#! /bin/bash

# *****************************************************************************
# * Copyright (C) 2009   this file is part of the NPTool Project              *
# *                                                                           *
# * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
# * For the list of contributors see $NPTOOL/Licence/Contributors             *
# *****************************************************************************/

# *****************************************************************************
# * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
# *                                                                           *
# * Creation Date  : January 2013                                             *
# * Last update    :                                                          *
# *---------------------------------------------------------------------------*
# * Decription: This script writes the commands that should be called when    *
# *             compiling individual detectors                                * 
# *             and call it with the argument passed to the script.           *
# *             Two arguments should be given:                                *
# *           + first one is the directory name                               *
# *           + second one is the file name where the messages are redirected *
# *                                                                           *
# *---------------------------------------------------------------------------*
# * Comment:                                                                  *
# *  This script is called by the configure script in $NPTOOL/NPLib           *
# *                                                                           *
# *****************************************************************************


# build message 
echo -e "\t@echo \"Entering $1 directory...\"" >> $2
# execute make command with target specified on command line
echo -e "\tmake --silent -C ./$1" >> $2
# copy header files
echo -e "\tcd $1; cp -f *.h ../include" >> $2
# remove *Dict header files
echo -e "\tcd include; rm *Dict.h" >> $2
# copy library files
echo -e "\tcd $1; cp -f *.so ../lib" >> $2
# deal with mac osx dylib files
#echo "ifeq (\$(findstring macosx, \$(ARCH)), macosx)" >> $2
#echo "\t@echo \"to be done\""
#echo "endif" >> $2
# newline
echo "" >> $2

