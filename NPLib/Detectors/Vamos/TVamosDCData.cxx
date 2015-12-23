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

#include "TVamosDCData.h"


ClassImp(TVamosDCData)

TVamosDCData::TVamosDCData()
{
   // Default constructor
   Clear();
}



TVamosDCData::~TVamosDCData()
{
}



void TVamosDCData::Clear()
{
   // Strips from row 1
   fVamos_DC_Strip1_DetNbr.clear();
   fVamos_DC_Strip1_StripNbr.clear();
   fVamos_DC_Strip1_Energy.clear();
   // Strips from row 2
   fVamos_DC_Strip2_DetNbr.clear();
   fVamos_DC_Strip2_StripNbr.clear();
   fVamos_DC_Strip2_Energy.clear();
   // Efil
   fVamos_DC_Efil_DetNbr.clear();
   fVamos_DC_Efil_Energy.clear();
   // Efil PileUp
   fVamos_DC_Efil_PileUp_DetNbr.clear();
   fVamos_DC_Efil_PileUp_Energy.clear();
   // Tfil
   fVamos_DC_Tfil_DetNbr.clear();
   fVamos_DC_Tfil_Time.clear();
}



void TVamosDCData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // Strips from row 1
   cout << "Vamos_DC_Strip1_Mult = " << fVamos_DC_Strip1_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fVamos_DC_Strip1_DetNbr.size(); i++)
      cout << "Det_Strip1: " << fVamos_DC_Strip1_DetNbr[i] << " Strip_Strip1: " << fVamos_DC_Strip1_StripNbr[i] << " Energy_Strip1: " << fVamos_DC_Strip1_Energy[i] << endl;
   // Strips from row 2
   cout << "Vamos_DC_Strip2_Mult = " << fVamos_DC_Strip2_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fVamos_DC_Strip2_DetNbr.size(); i++)
      cout << "Det_Strip2: " << fVamos_DC_Strip2_DetNbr[i] << " Strip_Strip2: " << fVamos_DC_Strip2_StripNbr[i] << " Energy_Strip2: " << fVamos_DC_Strip1_Energy[i] << endl;

   // Efil
   cout << "Vamos_DC_Efil_Mult = " << fVamos_DC_Efil_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fVamos_DC_Efil_DetNbr.size(); i++)
      cout << "DetNbr: " << fVamos_DC_Efil_DetNbr[i] << " Energy: " << fVamos_DC_Efil_Energy[i] << endl;

   // Efil PileUp
   cout << "Vamos_DC_Efil_PileUp_Mult = " << fVamos_DC_Efil_PileUp_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fVamos_DC_Efil_PileUp_DetNbr.size(); i++)
      cout << "DetNbr: " << fVamos_DC_Efil_PileUp_DetNbr[i] << " Energy: " << fVamos_DC_Efil_PileUp_Energy[i] << endl;

   // Tfil
   cout << "Vamos_DC_Tfil_Mult = " << fVamos_DC_Tfil_DetNbr.size() << endl;
   for (UShort_t i = 0; i < fVamos_DC_Tfil_DetNbr.size(); i++)
      cout << "DetNbr: " << fVamos_DC_Tfil_DetNbr[i] << " Time: " << fVamos_DC_Tfil_Time[i] << endl;
}
