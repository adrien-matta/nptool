/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : january 2011                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *     This class holds the raw data storage for the W1 detector from Micron *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include "TW1Data.h"

#include <iostream>
using namespace std;

ClassImp(TW1Data)



TW1Data::TW1Data()
{
   // Default constructor
   Clear();
}



TW1Data::~TW1Data()
{
}



void TW1Data::Clear()
{
   // (Front, E)
   fW1_FrontE_DetectorNbr.clear();
   fW1_FrontE_StripNbr.clear();
   fW1_FrontE_Energy.clear();
   // (Front, T)
   fW1_FrontT_DetectorNbr.clear();
   fW1_FrontT_StripNbr.clear();
   fW1_FrontT_Time.clear();

   // (Back, E)
   fW1_BackE_DetectorNbr.clear();
   fW1_BackE_StripNbr.clear();
   fW1_BackE_Energy.clear();
   // (Back, T)
   fW1_BackT_DetectorNbr.clear();
   fW1_BackT_StripNbr.clear();
   fW1_BackT_Time.clear();
}



void TW1Data::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TW1Data::Dump()] XXXXXXXXXXXXXXXXX" << endl;

   // (Front, E)
   cout << "W1_FrontE_Mult = " << fW1_FrontE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fW1_FrontE_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fW1_FrontE_DetectorNbr[i] << " Strip: " << fW1_FrontE_StripNbr[i] << " Energy: " << fW1_FrontE_Energy[i] << endl;
   // (Front, T)
   cout << "W1_FrontT_Mult = " << fW1_FrontT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fW1_FrontT_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fW1_FrontT_DetectorNbr[i] << " Strip: " << fW1_FrontT_StripNbr[i] << " Energy: " << fW1_FrontT_Time[i] << endl;

   // (Back, E)
   cout << "W1_BackE_Mult = " << fW1_BackE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fW1_BackE_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fW1_BackE_DetectorNbr[i] << " Strip: " << fW1_BackE_StripNbr[i] << " Energy: " << fW1_BackE_Energy[i] << endl;
   // (Back, T)
   cout << "W1_BackT_Mult = " << fW1_BackT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fW1_BackT_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fW1_BackT_DetectorNbr[i] << " Strip: " << fW1_BackT_StripNbr[i] << " Energy: " << fW1_BackT_Time[i] << endl;
}
