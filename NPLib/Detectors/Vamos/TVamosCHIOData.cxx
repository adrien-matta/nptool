/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : July 2009                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds VAMOS Raw data                                          *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
using namespace std;

#include "TVamosCHIOData.h"


ClassImp(TVamosCHIOData)

TVamosCHIOData::TVamosCHIOData()
{
   // Default constructor
   Clear();
}



TVamosCHIOData::~TVamosCHIOData()
{
}



void TVamosCHIOData::Clear()
{
   // A
   fVamos_CHIO_A_PadNbr.clear();
   fVamos_CHIO_A_Energy.clear();
   // B
   fVamos_CHIO_B_PadNbr.clear();
   fVamos_CHIO_B_Energy.clear();
   // C
   fVamos_CHIO_C_PadNbr.clear();
   fVamos_CHIO_C_Energy.clear();
}



void TVamosCHIOData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // A
   cout << "Vamos_CHIO_MultA = " << fVamos_CHIO_A_PadNbr.size() << endl;
   for (UShort_t i = 0; i < fVamos_CHIO_A_PadNbr.size(); i++)
      cout << "PadNbrA: " << fVamos_CHIO_A_PadNbr[i] << " EnergyA: " << fVamos_CHIO_A_Energy[i] << endl;
   // B
   cout << "Vamos_CHIO_MultB = " << fVamos_CHIO_B_PadNbr.size() << endl;
   for (UShort_t i = 0; i < fVamos_CHIO_B_PadNbr.size(); i++)
      cout << "PadNbrB: " << fVamos_CHIO_B_PadNbr[i] << " EnergyB: " << fVamos_CHIO_B_Energy[i] << endl;
   // C
   cout << "Vamos_CHIO_MultC = " << fVamos_CHIO_C_PadNbr.size() << endl;
   for (UShort_t i = 0; i < fVamos_CHIO_C_PadNbr.size(); i++)
      cout << "PadNbrC: " << fVamos_CHIO_C_PadNbr[i] << " EnergyC: " << fVamos_CHIO_C_Energy[i] << endl;
}
