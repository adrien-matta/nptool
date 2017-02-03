/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
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
 *             tracker part of the Gaspard detector.                         *
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

#include "TGaspardTrackerData.h"

ClassImp(TGaspardTrackerData)

////////////////////////////////////////////////////////////////////////////////
TGaspardTrackerData::TGaspardTrackerData(){
}


////////////////////////////////////////////////////////////////////////////////
TGaspardTrackerData::~TGaspardTrackerData(){
}


////////////////////////////////////////////////////////////////////////////////
void TGaspardTrackerData::Clear(){
   // DSSD
   // (X,E)
   fGPDTrk_FirstStage_FrontE_DetectorNbr.clear();
   fGPDTrk_FirstStage_FrontE_StripNbr.clear()  ;
   fGPDTrk_FirstStage_FrontE_Energy.clear()    ;
   // (X,T)
   fGPDTrk_FirstStage_FrontT_DetectorNbr.clear()  ;
   fGPDTrk_FirstStage_FrontT_StripNbr.clear()  ;
   fGPDTrk_FirstStage_FrontT_Time.clear()      ;
   // (Y,E)
   fGPDTrk_FirstStage_BackE_DetectorNbr.clear()  ;
   fGPDTrk_FirstStage_BackE_StripNbr.clear()  ;
   fGPDTrk_FirstStage_BackE_Energy.clear()    ;
   // (Y,T)
   fGPDTrk_FirstStage_BackT_DetectorNbr.clear()  ;
   fGPDTrk_FirstStage_BackT_StripNbr.clear()  ;
   fGPDTrk_FirstStage_BackT_Time.clear()      ;

   // Second Stage
   // E
   fGPDTrk_SecondStage_E_DetectorNbr.clear() ;
   fGPDTrk_SecondStage_E_PadNbr.clear()      ;
   fGPDTrk_SecondStage_E_Energy.clear()      ;
   // T
   fGPDTrk_SecondStage_T_DetectorNbr.clear() ;
   fGPDTrk_SecondStage_T_PadNbr.clear()      ;
   fGPDTrk_SecondStage_T_Time.clear()        ;

   // Third Stage
   // E
   fGPDTrk_ThirdStage_E_DetectorNbr.clear()  ;
   fGPDTrk_ThirdStage_E_PadNbr.clear()      ;
   fGPDTrk_ThirdStage_E_Energy.clear()       ;
   // T
   fGPDTrk_ThirdStage_T_DetectorNbr.clear()  ;
   fGPDTrk_ThirdStage_T_PadNbr.clear()      ;
   fGPDTrk_ThirdStage_T_Time.clear()         ;
}


////////////////////////////////////////////////////////////////////////////////
void TGaspardTrackerData::Dump() const{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // DSSD
   // (X,E)
   cout << "GPDTrk_FirstStage_FrontE_Mult = " << fGPDTrk_FirstStage_FrontE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fGPDTrk_FirstStage_FrontE_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fGPDTrk_FirstStage_FrontE_DetectorNbr[i] << " Strip: " << fGPDTrk_FirstStage_FrontE_StripNbr[i] << " Energy: " << fGPDTrk_FirstStage_FrontE_Energy[i] << endl;
   // (X,T)
   cout << "GPDTrk_FirstStage_FrontT_Mult = " << fGPDTrk_FirstStage_FrontT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fGPDTrk_FirstStage_FrontT_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fGPDTrk_FirstStage_FrontT_DetectorNbr[i] << " Strip: " << fGPDTrk_FirstStage_FrontT_StripNbr[i] << " Time: " << fGPDTrk_FirstStage_FrontT_Time[i] << endl;
   // (Y,E)
   cout << "GPDTrk_FirstStage_BackE_Mult = " << fGPDTrk_FirstStage_BackE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fGPDTrk_FirstStage_BackE_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fGPDTrk_FirstStage_BackE_DetectorNbr[i] << " Strip: " << fGPDTrk_FirstStage_BackE_StripNbr[i] << " Energy: " << fGPDTrk_FirstStage_BackE_Energy[i] << endl;
   // (Y,T)
   cout << "GPDTrk_FirstStage_BackT_Mult = " << fGPDTrk_FirstStage_BackT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fGPDTrk_FirstStage_BackT_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fGPDTrk_FirstStage_BackT_DetectorNbr[i] << " Strip: " << fGPDTrk_FirstStage_BackT_StripNbr[i] << " Time: " << fGPDTrk_FirstStage_BackT_Time[i] << endl;

   // Second Stage
   // Energy
   cout << "GPDTrk_SecondStage_E_Mult = " << fGPDTrk_SecondStage_E_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fGPDTrk_SecondStage_E_DetectorNbr.size(); i++)
      cout << "Det: " << fGPDTrk_SecondStage_E_DetectorNbr[i] << " Pad: " << fGPDTrk_SecondStage_E_PadNbr[i] << " Energy: " << fGPDTrk_SecondStage_E_Energy[i] << endl;
   // Time
   cout << "GPDTrk_SecondStage_T_Mult = " << fGPDTrk_SecondStage_T_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fGPDTrk_SecondStage_T_DetectorNbr.size(); i++)
      cout << "Det: " << fGPDTrk_SecondStage_T_DetectorNbr[i] << " Pad: " << fGPDTrk_SecondStage_T_PadNbr[i] << " Time: " << fGPDTrk_SecondStage_T_Time[i] << endl;

   // Third Stage
   // Energy
   cout << "GPDTrk_ThirdStage_E_Mult = " << fGPDTrk_ThirdStage_E_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fGPDTrk_ThirdStage_E_DetectorNbr.size(); i++)
      cout << "Det: " << fGPDTrk_ThirdStage_E_DetectorNbr[i] << " Pad: " << fGPDTrk_ThirdStage_E_PadNbr[i] << " Energy: " << fGPDTrk_ThirdStage_E_Energy[i] << endl;
   // Time
   cout << "GPDTrk_ThirdStage_T_Mult = " << fGPDTrk_ThirdStage_T_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fGPDTrk_ThirdStage_T_DetectorNbr.size(); i++)
      cout << "Det: " << fGPDTrk_ThirdStage_T_DetectorNbr[i] << " Pad: " << fGPDTrk_ThirdStage_T_PadNbr[i] << " Time: " << fGPDTrk_ThirdStage_T_Time[i] << endl;
}
