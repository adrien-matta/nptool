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

#Target name
lname=$(printf "$1\n"  | tr '[A-Z]' '[a-z]')
printf  "$lname: " >> $2
#Dependancies
printf "FillIncludeDir \n" >> $2
# build message 
printf  "\t@echo \"Entering $1 directory...\"\n" >> $2
# execute make command with target specified on command line
printf  "\tmake --silent -C ./$1\n" >> $2
printf  "\tcp ./$1/*.so lib/\n" >> $2
printf "\n" >> $2

