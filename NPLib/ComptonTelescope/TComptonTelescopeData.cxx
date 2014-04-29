/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
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

#include "TComptonTelescopeData.h"

ClassImp(TComptonTelescopeData)

TComptonTelescopeData::TComptonTelescopeData()
{
   // Default constructor
   Clear();
}



TComptonTelescopeData::~TComptonTelescopeData()
{
}



void TComptonTelescopeData::Clear()
{
   // DSSSD
   // (X,E)
   fCT_Tracker_FrontE_TowerNbr.clear();
   fCT_Tracker_FrontE_DetectorNbr.clear();
   fCT_Tracker_FrontE_StripNbr.clear();
   fCT_Tracker_FrontE_Energy.clear();
   // (X,T)
   fCT_Tracker_FrontT_TowerNbr.clear();
   fCT_Tracker_FrontT_DetectorNbr.clear();
   fCT_Tracker_FrontT_StripNbr.clear();
   fCT_Tracker_FrontT_Time.clear();
   // (Y,E)
   fCT_Tracker_BackE_TowerNbr.clear();   
   fCT_Tracker_BackE_DetectorNbr.clear();
   fCT_Tracker_BackE_StripNbr.clear();
   fCT_Tracker_BackE_Energy.clear();
   // (Y,T)
   fCT_Tracker_BackT_TowerNbr.clear();   
   fCT_Tracker_BackT_DetectorNbr.clear();
   fCT_Tracker_BackT_StripNbr.clear();
   fCT_Tracker_BackT_Time.clear();

   // Calorimeter E
   fCT_Calorimeter_E_TowerNbr.clear();
   fCT_Calorimeter_E_DetectorNbr.clear();
   fCT_Calorimeter_E_ChannelNbr.clear() ;
   fCT_Calorimeter_E_Energy.clear();
   // Calorimeter T
   fCT_Calorimeter_T_TowerNbr.clear();
   fCT_Calorimeter_T_DetectorNbr.clear();
   fCT_Calorimeter_T_ChannelNbr.clear();
   fCT_Calorimeter_T_Time.clear();
}



void TComptonTelescopeData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event: Compton Telescope XXXXXXXXXXXXXXXXX" << endl;

   // DSSSD
   // (X,E)
   cout << "CT_Tracker_FrontE_Mult = " << fCT_Tracker_FrontE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fCT_Tracker_FrontE_DetectorNbr.size(); i++)
      cout << "TowNbr: " << fCT_Tracker_FrontE_TowerNbr[i] << " DetNbr: " << fCT_Tracker_FrontE_DetectorNbr[i] << " Strip: " << fCT_Tracker_FrontE_StripNbr[i] << " Energy: " << fCT_Tracker_FrontE_Energy[i] << endl;
   // (X,T)
   cout << "CT_Tracker_FrontT_Mult = " << fCT_Tracker_FrontT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fCT_Tracker_FrontT_DetectorNbr.size(); i++)
      cout << "TowNbr: " << fCT_Tracker_FrontT_TowerNbr[i] << " DetNbr: " << fCT_Tracker_FrontT_DetectorNbr[i] << " Strip: " << fCT_Tracker_FrontT_StripNbr[i] << " Time: " << fCT_Tracker_FrontT_Time[i] << endl;
   // (Y,E)
   cout << "CT_Tracker_BackE_Mult = " << fCT_Tracker_BackE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fCT_Tracker_BackE_DetectorNbr.size(); i++)
      cout << "TowNbr: " << fCT_Tracker_BackE_TowerNbr[i] << " DetNbr: " << fCT_Tracker_BackE_DetectorNbr[i] << " Strip: " << fCT_Tracker_BackE_StripNbr[i] << " Energy: " << fCT_Tracker_BackE_Energy[i] << endl;
   // (Y,T)
   cout << "CT_Tracker_BackT_Mult = " << fCT_Tracker_BackT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fCT_Tracker_BackT_DetectorNbr.size(); i++)
      cout << "TowNbr: " << fCT_Tracker_BackT_TowerNbr[i] << " DetNbr: " << fCT_Tracker_BackT_DetectorNbr[i] << " Strip: " << fCT_Tracker_BackT_StripNbr[i] << " Time: " << fCT_Tracker_BackT_Time[i] << endl;

   // Calorimeter
   // Energy
   cout << "CT_Calorimeter_E_Mult = " << fCT_Calorimeter_E_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fCT_Calorimeter_E_DetectorNbr.size(); i++)
      cout << "TowNbr: " << fCT_Calorimeter_E_TowerNbr[i] << " Det: " << fCT_Calorimeter_E_DetectorNbr[i] << " Pad: " << fCT_Calorimeter_E_ChannelNbr[i] << " Energy: " << fCT_Calorimeter_E_Energy[i] << endl;
   // Time
   cout << "CT_Calorimeter_T_Mult = " << fCT_Calorimeter_T_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fCT_Calorimeter_T_DetectorNbr.size(); i++)
      cout << "TowNbr: " << fCT_Calorimeter_E_TowerNbr[i] << " Det: " << fCT_Calorimeter_T_DetectorNbr[i] << " Pad: " << fCT_Calorimeter_T_ChannelNbr[i] << " Time: " << fCT_Calorimeter_T_Time[i] << endl;
}
