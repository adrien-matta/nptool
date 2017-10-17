/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Daniel Cox  contact address: daniel.m.cox@jyu.fi                        *
 *                                                                           *
 * Creation Date  : January 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Jurogam Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TJurogamData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TJurogamData)


//////////////////////////////////////////////////////////////////////
TJurogamData::TJurogamData() {
}



//////////////////////////////////////////////////////////////////////
TJurogamData::~TJurogamData() {
}



//////////////////////////////////////////////////////////////////////
void TJurogamData::Clear() {
  // Energy
  fJurogam_E_DetectorNbr.clear();
  fJurogam_Energy.clear();
  // Time
  fJurogam_T_DetectorNbr.clear();
  fJurogam_Time.clear();
  // BGO
  // Energy
  fJurogamBGO_E_DetectorNbr.clear();
  fJurogamBGO_Energy.clear();
  // Time
  fJurogamBGO_T_DetectorNbr.clear();
  fJurogamBGO_Time.clear();
}



//////////////////////////////////////////////////////////////////////
void TJurogamData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TJurogamData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // Energy
  size_t mysize = fJurogam_E_DetectorNbr.size();
  cout << "Jurogam_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fJurogam_E_DetectorNbr[i]
         << " Energy: " << fJurogam_Energy[i];
  }
  
  // Time
  mysize = fJurogam_T_DetectorNbr.size();
  cout << "Jurogam_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fJurogam_T_DetectorNbr[i]
         << " Time: " << fJurogam_Time[i];
  }
  // BGO
  // Energy
  mysize = fJurogamBGO_E_DetectorNbr.size();
  cout << "Jurogam_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fJurogamBGO_E_DetectorNbr[i]
         << " Energy: " << fJurogamBGO_Energy[i];
  }
  
  // Time
  mysize = fJurogamBGO_T_DetectorNbr.size();
  cout << "Jurogam_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fJurogamBGO_T_DetectorNbr[i]
         << " Time: " << fJurogamBGO_Time[i];
  }
}
