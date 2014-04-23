# *****************************************************************************
# * Copyright (C) 2014   this file is part of the NPTool Project              *
# *                                                                           *
# * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
# * For the list of contributors see $NPTOOL/Licence/Contributors             *
# *****************************************************************************/

# *****************************************************************************
# * Original Author: A. Matta  contact address: a.matta@surrey.ac.uk          *
# *                                                                           *
# * Creation Date  : 23/4/2014                                                *
# * Last update    :                                                          *
# *---------------------------------------------------------------------------*
# * Decription: This script edit the user .rootlogon.C file to load NPTool    *
# * Libraries.                                                                *
# *                                                                           *
# *---------------------------------------------------------------------------*
# * Comment:                                                                  *
# *                                                                           *
# *                                                                           *
# *****************************************************************************

#! /bin/bash

# test if the ~/.rootlogon.C file exist, if no create it
if [ -f ~/.rootlogon.C ];
then
  echo 'File .rootlogon.C already exist: attempt to modified it'
  # Check if NPTOOL stuff is already in
  SEARCH=$(grep NPTOOL ~/test.C)
  echo $SEARCH
  if [ ! -n "$SEARCH" ];
  then
    echo 'Appending missing line to the file'
    sed -i -e 's#{#{\
  TString NPLPath = gSystem->Getenv(\"NPTOOL\");\
  gROOT->ProcessLine(Form(\".x %s/NPLib/scripts/NPToolLogon.C+\", NPLPath.Data()));\
#g' ~/.rootlogon.C 
  else
    echo 'Some NPTool Lib already loading in .rootlogon.C : please check the file'
  fi

else
  echo 'File .rootlogon.C was created'
  cp scripts/rootlogon_basic.C ~/.rootlogon.C
fi





