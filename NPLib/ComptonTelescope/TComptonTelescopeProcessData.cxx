/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : July 2012                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds the raw data for a Compton telescope prototype made of  *
 *  a silicon tracker detector and a LaBr3 calorimeter                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
using namespace std;

#include "TComptonTelescopeProcessData.h"

ClassImp(TComptonTelescopeProcessData)

TComptonTelescopeProcessData::TComptonTelescopeProcessData()
{
   // Default constructor
   Clear();
}



TComptonTelescopeProcessData::~TComptonTelescopeProcessData()
{
}



void TComptonTelescopeProcessData::Clear()
{
   // DSSSD
   // (X,E)
   fCT_Tracker_Compton_TowerNbr.clear();
   fCT_Tracker_Compton_DetectorNbr.clear();
   fCT_Tracker_Compton_Energy.clear();
}



void TComptonTelescopeProcessData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event: Compton Telescope Process XXXXXXXXXXXXXXXXX" << endl;

   // DSSSD
   // (X,E)
   cout << "CT_Tracker_Compton_Mult = " << fCT_Tracker_Compton_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fCT_Tracker_Compton_DetectorNbr.size(); i++)
      cout << "TowNbr: " << fCT_Tracker_Compton_TowerNbr[i] << " DetNbr: " << fCT_Tracker_Compton_DetectorNbr[i] << " Energy: " << fCT_Tracker_Compton_Energy[i] << endl;
}
