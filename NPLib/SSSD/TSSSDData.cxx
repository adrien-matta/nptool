/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : febuary 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold SSSD Raw data                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
using namespace std;

#include "TSSSDData.h"

ClassImp(TSSSDData)

TSSSDData::TSSSDData()
{
   // Default constructor

   // SSSD
   // Energy
   fSSSD_StripE_DetectorNbr.clear();
   fSSSD_StripE_StripNbr.clear();
   fSSSD_StripE_Energy.clear();
   // Time
   fSSSD_StripT_DetectorNbr.clear();
   fSSSD_StripT_StripNbr.clear();
   fSSSD_StripT_Time.clear();

}

TSSSDData::~TSSSDData()
{}

void TSSSDData::Clear()
{
   // DSSD
   // Energy
   fSSSD_StripE_DetectorNbr.clear();
   fSSSD_StripE_StripNbr.clear();
   fSSSD_StripE_Energy.clear(); 
   // Time
   fSSSD_StripT_DetectorNbr.clear();
   fSSSD_StripT_StripNbr.clear();
   fSSSD_StripT_Time.clear();
}



void TSSSDData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // SSSD
   // Energy
   cout << "SSSD_StripE_Mult = " << fSSSD_StripE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fSSSD_StripE_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fSSSD_StripE_DetectorNbr[i] << " Strip: " << fSSSD_StripE_StripNbr[i] << " Energy: " << fSSSD_StripE_Energy[i] << endl;
   // Time
   cout << "SSSD_StripXT_Mult = " << fSSSD_StripT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fSSSD_StripT_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fSSSD_StripT_DetectorNbr[i] << " Strip: " << fSSSD_StripT_StripNbr[i] << " Time: " << fSSSD_StripT_Time[i] << endl;
  
}
