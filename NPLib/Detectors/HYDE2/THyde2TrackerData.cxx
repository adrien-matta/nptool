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
 *             tracker part of the Hyde2 detector.                         *
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

#include "THyde2TrackerData.h"

ClassImp(THyde2TrackerData)

THyde2TrackerData::THyde2TrackerData()
{
   // Default constructor

   Clear();
}



THyde2TrackerData::~THyde2TrackerData()
{
}



void THyde2TrackerData::Clear()
{
   // DSSD
   // (X,E)
   fHYD2Trk_FirstStage_FrontE_DetectorNbr.clear();
   fHYD2Trk_FirstStage_FrontE_StripNbr.clear()  ;
   fHYD2Trk_FirstStage_FrontE_Energy.clear()    ;
   // (X,T)
   fHYD2Trk_FirstStage_FrontT_DetectorNbr.clear()  ;
   fHYD2Trk_FirstStage_FrontT_StripNbr.clear()  ;
   fHYD2Trk_FirstStage_FrontT_Time.clear()      ;
   // (Y,E)
   fHYD2Trk_FirstStage_BackE_DetectorNbr.clear()  ;
   fHYD2Trk_FirstStage_BackE_StripNbr.clear()  ;
   fHYD2Trk_FirstStage_BackE_Energy.clear()    ;
   // (Y,T)
   fHYD2Trk_FirstStage_BackT_DetectorNbr.clear()  ;
   fHYD2Trk_FirstStage_BackT_StripNbr.clear()  ;
   fHYD2Trk_FirstStage_BackT_Time.clear()      ;

   // Second Stage
   // E
   fHYD2Trk_SecondStage_E_DetectorNbr.clear() ;
   fHYD2Trk_SecondStage_E_PadNbr.clear()      ;
   fHYD2Trk_SecondStage_E_Energy.clear()      ;
   // T
   fHYD2Trk_SecondStage_T_DetectorNbr.clear() ;
   fHYD2Trk_SecondStage_T_PadNbr.clear()      ;
   fHYD2Trk_SecondStage_T_Time.clear()        ;

   // Third Stage
   // E
   fHYD2Trk_ThirdStage_E_DetectorNbr.clear()  ;
   fHYD2Trk_ThirdStage_E_PadNbr.clear()      ;
   fHYD2Trk_ThirdStage_E_Energy.clear()       ;
   // T
   fHYD2Trk_ThirdStage_T_DetectorNbr.clear()  ;
   fHYD2Trk_ThirdStage_T_PadNbr.clear()      ;
   fHYD2Trk_ThirdStage_T_Time.clear()         ;
   
   // Fourth Stage
   // E
   fHYD2Trk_FourthStage_E_DetectorNbr.clear()  ;
   fHYD2Trk_FourthStage_E_PadNbr.clear()      ;
   fHYD2Trk_FourthStage_E_Energy.clear()       ;
   // T
   fHYD2Trk_FourthStage_T_DetectorNbr.clear()  ;
   fHYD2Trk_FourthStage_T_PadNbr.clear()      ;
   fHYD2Trk_FourthStage_T_Time.clear()         ;

   // Fifth Stage
   // E
   fHYD2Trk_FifthStage_E_DetectorNbr.clear()  ;
   fHYD2Trk_FifthStage_E_PadNbr.clear()      ;
   fHYD2Trk_FifthStage_E_Energy.clear()       ;
   // T
   fHYD2Trk_FifthStage_T_DetectorNbr.clear()  ;
   fHYD2Trk_FifthStage_T_PadNbr.clear()      ;
   fHYD2Trk_FifthStage_T_Time.clear()         ;

   // Sixth Stage
   // E
   fHYD2Trk_SixthStage_E_DetectorNbr.clear()  ;
   fHYD2Trk_SixthStage_E_PadNbr.clear()      ;
   fHYD2Trk_SixthStage_E_Energy.clear()       ;
   // T
   fHYD2Trk_SixthStage_T_DetectorNbr.clear()  ;
   fHYD2Trk_SixthStage_T_PadNbr.clear()      ;
   fHYD2Trk_SixthStage_T_Time.clear()         ;

}



void THyde2TrackerData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // DSSD
   // (X,E)
   cout << "HYD2Trk_FirstStage_FrontE_Mult = " << fHYD2Trk_FirstStage_FrontE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYD2Trk_FirstStage_FrontE_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fHYD2Trk_FirstStage_FrontE_DetectorNbr[i] << " Strip: " << fHYD2Trk_FirstStage_FrontE_StripNbr[i] << " Energy: " << fHYD2Trk_FirstStage_FrontE_Energy[i] << endl;
   // (X,T)
   cout << "HYD2Trk_FirstStage_FrontT_Mult = " << fHYD2Trk_FirstStage_FrontT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYD2Trk_FirstStage_FrontT_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fHYD2Trk_FirstStage_FrontT_DetectorNbr[i] << " Strip: " << fHYD2Trk_FirstStage_FrontT_StripNbr[i] << " Time: " << fHYD2Trk_FirstStage_FrontT_Time[i] << endl;
   // (Y,E)
   cout << "HYD2Trk_FirstStage_BackE_Mult = " << fHYD2Trk_FirstStage_BackE_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYD2Trk_FirstStage_BackE_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fHYD2Trk_FirstStage_BackE_DetectorNbr[i] << " Strip: " << fHYD2Trk_FirstStage_BackE_StripNbr[i] << " Energy: " << fHYD2Trk_FirstStage_BackE_Energy[i] << endl;
   // (Y,T)
   cout << "HYD2Trk_FirstStage_BackT_Mult = " << fHYD2Trk_FirstStage_BackT_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYD2Trk_FirstStage_BackT_DetectorNbr.size(); i++)
      cout << "DetNbr: " << fHYD2Trk_FirstStage_BackT_DetectorNbr[i] << " Strip: " << fHYD2Trk_FirstStage_BackT_StripNbr[i] << " Time: " << fHYD2Trk_FirstStage_BackT_Time[i] << endl;

   // Second Stage
   // Energy
   cout << "HYD2Trk_SecondStage_E_Mult = " << fHYD2Trk_SecondStage_E_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYD2Trk_SecondStage_E_DetectorNbr.size(); i++)
      cout << "Det: " << fHYD2Trk_SecondStage_E_DetectorNbr[i] << " Pad: " << fHYD2Trk_SecondStage_E_PadNbr[i] << " Energy: " << fHYD2Trk_SecondStage_E_Energy[i] << endl;
   // Time
   cout << "HYD2Trk_SecondStage_T_Mult = " << fHYD2Trk_SecondStage_T_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYD2Trk_SecondStage_T_DetectorNbr.size(); i++)
      cout << "Det: " << fHYD2Trk_SecondStage_T_DetectorNbr[i] << " Pad: " << fHYD2Trk_SecondStage_T_PadNbr[i] << " Time: " << fHYD2Trk_SecondStage_T_Time[i] << endl;

   // Third Stage
   // Energy
   cout << "HYD2Trk_ThirdStage_E_Mult = " << fHYD2Trk_ThirdStage_E_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYD2Trk_ThirdStage_E_DetectorNbr.size(); i++)
      cout << "Det: " << fHYD2Trk_ThirdStage_E_DetectorNbr[i] << " Pad: " << fHYD2Trk_ThirdStage_E_PadNbr[i] << " Energy: " << fHYD2Trk_ThirdStage_E_Energy[i] << endl;
   // Time
   cout << "HYD2Trk_ThirdStage_T_Mult = " << fHYD2Trk_ThirdStage_T_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYD2Trk_ThirdStage_T_DetectorNbr.size(); i++)
      cout << "Det: " << fHYD2Trk_ThirdStage_T_DetectorNbr[i] << " Pad: " << fHYD2Trk_ThirdStage_T_PadNbr[i] << " Time: " << fHYD2Trk_ThirdStage_T_Time[i] << endl;

   // Fourth Stage
   // Energy
   cout << "HYD2Trk_FourthStage_E_Mult = " << fHYD2Trk_FourthStage_E_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYD2Trk_FourthStage_E_DetectorNbr.size(); i++)
      cout << "Det: " << fHYD2Trk_FourthStage_E_DetectorNbr[i] << " Pad: " << fHYD2Trk_FourthStage_E_PadNbr[i] << " Energy: " << fHYD2Trk_FourthStage_E_Energy[i] << endl;
   // Time
   cout << "HYD2Trk_FourthStage_T_Mult = " << fHYD2Trk_FourthStage_T_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYD2Trk_FourthStage_T_DetectorNbr.size(); i++)
      cout << "Det: " << fHYD2Trk_FourthStage_T_DetectorNbr[i] << " Pad: " << fHYD2Trk_FourthStage_T_PadNbr[i] << " Time: " << fHYD2Trk_FourthStage_T_Time[i] << endl;

   // Fifth Stage
   // Energy
   cout << "HYD2Trk_FifthStage_E_Mult = " << fHYD2Trk_FifthStage_E_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYD2Trk_FifthStage_E_DetectorNbr.size(); i++)
      cout << "Det: " << fHYD2Trk_FifthStage_E_DetectorNbr[i] << " Pad: " << fHYD2Trk_FifthStage_E_PadNbr[i] << " Energy: " << fHYD2Trk_FifthStage_E_Energy[i] << endl;
   // Time
   cout << "HYD2Trk_FifthStage_T_Mult = " << fHYD2Trk_FifthStage_T_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYD2Trk_FifthStage_T_DetectorNbr.size(); i++)
      cout << "Det: " << fHYD2Trk_FifthStage_T_DetectorNbr[i] << " Pad: " << fHYD2Trk_FifthStage_T_PadNbr[i] << " Time: " << fHYD2Trk_FifthStage_T_Time[i] << endl;

   // Sixth Stage
   // Energy
   cout << "HYD2Trk_SixthStage_E_Mult = " << fHYD2Trk_SixthStage_E_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYD2Trk_SixthStage_E_DetectorNbr.size(); i++)
      cout << "Det: " << fHYD2Trk_SixthStage_E_DetectorNbr[i] << " Pad: " << fHYD2Trk_SixthStage_E_PadNbr[i] << " Energy: " << fHYD2Trk_SixthStage_E_Energy[i] << endl;
   // Time
   cout << "HYD2Trk_SixthStage_T_Mult = " << fHYD2Trk_SixthStage_T_DetectorNbr.size() << endl;
   for (UShort_t i = 0; i < fHYD2Trk_SixthStage_T_DetectorNbr.size(); i++)
      cout << "Det: " << fHYD2Trk_SixthStage_T_DetectorNbr[i] << " Pad: " << fHYD2Trk_SixthStage_T_PadNbr[i] << " Time: " << fHYD2Trk_SixthStage_T_Time[i] << endl;

}
