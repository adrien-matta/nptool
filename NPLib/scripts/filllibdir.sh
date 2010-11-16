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
# * Decription: This script moves all the libraries (*.so) in all directories *
# *             to the lib/ directory.                                        *
# *---------------------------------------------------------------------------*
# * Comment:                                                                  *
# *                                                                           *
# *                                                                           *
# *****************************************************************************

#! /bin/bash

# previously, clean lib/ directory
cd lib/
rm -f *
cd ../

# loop recursively on header files in all directories
for file in */*.so
do
   # copy header files to the include directory
   cp -f $file lib/
done
