/*****************************************************************************
 * Copyright (C) 2009-XYEARX   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: XAUTHORX  contact address: XMAILX                        *
 *                                                                           *
 * Creation Date  : XMONTHX XYEARX                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold DETECTORNAME Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TDETECTORNAMEData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TDETECTORNAMEData)


//////////////////////////////////////////////////////////////////////
TDETECTORNAMEData::TDETECTORNAMEData() {
}



//////////////////////////////////////////////////////////////////////
TDETECTORNAMEData::~TDETECTORNAMEData() {
}



//////////////////////////////////////////////////////////////////////
void TDETECTORNAMEData::Clear() {
  // Energy
  fDETECTORNAME_E_DetectorNbr.clear();
  fDETECTORNAME_Energy.clear();
  // Time
  fDETECTORNAME_T_DetectorNbr.clear();
  fDETECTORNAME_Time.clear();
}



//////////////////////////////////////////////////////////////////////
void TDETECTORNAMEData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TDETECTORNAMEData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // Energy
  size_t mysize = fDETECTORNAME_E_DetectorNbr.size();
  cout << "DETECTORNAME_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fDETECTORNAME_E_DetectorNbr[i]
         << " Energy: " << fDETECTORNAME_Energy[i];
  }
  
  // Time
  mysize = fDETECTORNAME_T_DetectorNbr.size();
  cout << "DETECTORNAME_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fDETECTORNAME_T_DetectorNbr[i]
         << " Time: " << fDETECTORNAME_Time[i];
  }
}
