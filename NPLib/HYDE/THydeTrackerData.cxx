/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 10/06/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class stores the results of the G4 simulation for the    *
 *             tracker part of the Hyde detector.                         *
 *             The format mimics what was used for the GANIL experiments     *
 *             after conversion of the raw data with GRU. Ask                *
 *             N. de Sereville for more informations.                        *
 *             This class derives from TObject (ROOT) and its aim is to be   *
 *             stored in the output TTree of the G4 simulation               *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
using namespace std;

#include "THydeTrackerData.h"

ClassImp(THydeTrackerData)

THydeTrackerData::THydeTrackerData()
{
   // Default constructor

   Clear();
}



THydeTrackerData::~THydeTrackerData()
{
}



void THydeTrackerData::Clear()
{
   // DSSD
   // (X,E)
   fHYDTrk_FirstStage_FrontE_DetectorNbr.clear();
   fHYDTrk_FirstStage_FrontE_StripNbr.clear()   ;
   fHYDTrk_FirstStage_FrontE_Energy.clear()     ;
   // (X,T)
   fHYDTrk_FirstStage_FrontT_DetectorNbr.clear();
   fHYDTrk_FirstStage_FrontT_StripNbr.clear()   ;
   fHYDTrk_FirstStage_FrontT_Time.clear()       ;
   // (Y,E)
   fHYDTrk_FirstStage_BackE_DetectorNbr.clear() ;
   fHYDTrk_FirstStage_BackE_StripNbr.clear()    ;
   fHYDTrk_FirstStage_BackE_Energy.clear()      ;
   // (Y,T)
   fHYDTrk_FirstStage_BackT_DetectorNbr.clear() ;
   fHYDTrk_FirstStage_BackT_StripNbr.clear()    ;
   fHYDTrk_FirstStage_BackT_Time.clear()        ;

   // Second Stage
   // E
   fHYDTrk_SecondStage_E_DetectorNbr.clear() ;
   fHYDTrk_SecondStage_E_PadNbr.clear()      ;
   fHYDTrk_SecondStage_E_Energy.clear()      ;
   // T
   fHYDTrk_SecondStage_T_DetectorNbr.clear() ;
   fHYDTrk_SecondStage_T_PadNbr.clear()      ;
   fHYDTrk_SecondStage_T_Time.clear()        ;

   // Third Stage
   // E
   fHYDTrk_ThirdStage_E_DetectorNbr.clear()  ;
   fHYDTrk_ThirdStage_E_PadNbr.clear()       ;
   fHYDTrk_ThirdStage_E_Energy.clear()       ;
   // T
   fHYDTrk_ThirdStage_T_DetectorNbr.clear()  ;
   fHYDTrk_ThirdStage_T_PadNbr.clear()       ;
   fHYDTrk_ThirdStage_T_Time.clear()         ;
}



void THydeTrackerData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // DSSD
   // (X,E)
   cout << "HYDTrk_FirstStage_FrontE_Mult = " << fHYDTrk_FirstStage_FrontE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYDTrk_FirstStage_FrontE_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fHYDTrk_FirstStage_FrontE_DetectorNbr[i] << " Strip: " << fHYDTrk_FirstStage_FrontE_StripNbr[i] << " Energy: " << fHYDTrk_FirstStage_FrontE_Energy[i] << endl;
   // (X,T)
   cout << "HYDTrk_FirstStage_FrontT_Mult = " << fHYDTrk_FirstStage_FrontT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYDTrk_FirstStage_FrontT_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fHYDTrk_FirstStage_FrontT_DetectorNbr[i] << " Strip: " << fHYDTrk_FirstStage_FrontT_StripNbr[i] << " Time: " << fHYDTrk_FirstStage_FrontT_Time[i] << endl;
   // (Y,E)
   cout << "HYDTrk_FirstStage_BackE_Mult = " << fHYDTrk_FirstStage_BackE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYDTrk_FirstStage_BackE_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fHYDTrk_FirstStage_BackE_DetectorNbr[i] << " Strip: " << fHYDTrk_FirstStage_BackE_StripNbr[i] << " Energy: " << fHYDTrk_FirstStage_BackE_Energy[i] << endl;
   // (Y,T)
   cout << "HYDTrk_FirstStage_BackT_Mult = " << fHYDTrk_FirstStage_BackT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYDTrk_FirstStage_BackT_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fHYDTrk_FirstStage_BackT_DetectorNbr[i] << " Strip: " << fHYDTrk_FirstStage_BackT_StripNbr[i] << " Time: " << fHYDTrk_FirstStage_BackT_Time[i] << endl;

   // Second Stage
   // Energy
   cout << "HYDTrk_SecondStage_E_Mult = " << fHYDTrk_SecondStage_E_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYDTrk_SecondStage_E_DetectorNbr.size(); i++)
      cout << "Det: " << fHYDTrk_SecondStage_E_DetectorNbr[i] << " Pad: " << fHYDTrk_SecondStage_E_PadNbr[i] << " Energy: " << fHYDTrk_SecondStage_E_Energy[i] << endl;
   // Time
   cout << "HYDTrk_SecondStage_T_Mult = " << fHYDTrk_SecondStage_T_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYDTrk_SecondStage_T_DetectorNbr.size(); i++)
      cout << "Det: " << fHYDTrk_SecondStage_T_DetectorNbr[i] << " Pad: " << fHYDTrk_SecondStage_T_PadNbr[i] << " Time: " << fHYDTrk_SecondStage_T_Time[i] << endl;

   // Third Stage
   // Energy
   cout << "HYDTrk_ThirdStage_E_Mult = " << fHYDTrk_ThirdStage_E_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYDTrk_ThirdStage_E_DetectorNbr.size(); i++)
      cout << "Det: " << fHYDTrk_ThirdStage_E_DetectorNbr[i] << " Pad: " << fHYDTrk_ThirdStage_E_PadNbr[i] << " Energy: " << fHYDTrk_ThirdStage_E_Energy[i] << endl;
   // Time
   cout << "HYDTrk_ThirdStage_T_Mult = " << fHYDTrk_ThirdStage_T_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYDTrk_ThirdStage_T_DetectorNbr.size(); i++)
      cout << "Det: " << fHYDTrk_ThirdStage_T_DetectorNbr[i] << " Pad: " << fHYDTrk_ThirdStage_T_PadNbr[i] << " Time: " << fHYDTrk_ThirdStage_T_Time[i] << endl;
}
