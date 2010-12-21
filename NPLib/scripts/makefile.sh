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

# loop recursively on Makefile files in sub-directories
for file in */Makefile
do
   # remove "Makefile" string from file name
   name=${file%\/*}
   # print informations
   echo "Entering $name directory..."
   # add "-C ./" pattern at the beginning of the name
   cmd="-C ./$name"
   # execute make command with target specified on command line
   make --silent $1 $cmd
done
