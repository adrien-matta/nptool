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
# * Decription: This script creates symbolic links from .so to .dylib in the  *
# *             lib/ directory.                                               *
# *                                                                           *
# *---------------------------------------------------------------------------*
# * Comment:                                                                  *
# *                                                                           *
# *                                                                           *
# *****************************************************************************

#! /bin/bash

# move to lib directory
cd lib

# loop recursively on header files in all directories
for file in *.so
do
   # build dylib file
   name=${file%\.*}
   name="$name.dylib"
   # make symbolic link from .so to .dylib
   ln -sf $file $name  
done
