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
# * Decription: This script moves all the header files (*.h) from all         *
# *             directories to the lib/ directory.                            *
# *---------------------------------------------------------------------------*
# * Comment:                                                                  *
# *                                                                           *
# *                                                                           *
# *****************************************************************************

#! /bin/bash

echo " + Copying header files to the include directory....."

# previously, clean include directory
rm -f include/*.h

# loop recursively on header files in all directories
for file in */*.h
do
   # copy header files to the include directory
   cp -f $file include/
done

# remove include file with *Dict*
rm -f include/*Dict*
