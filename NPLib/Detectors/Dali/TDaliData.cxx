/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Elidiano Tronchin  contact address: elidiano.tronchin@studenti.unipd.it                        *
 *                                                                           *
 * Creation Date  : septembre 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Dali Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TDaliData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TDaliData)


//////////////////////////////////////////////////////////////////////
TDaliData::TDaliData() {
}



//////////////////////////////////////////////////////////////////////
TDaliData::~TDaliData() {
}



//////////////////////////////////////////////////////////////////////
void TDaliData::Clear() {
  // Energy
  fDali_E_DetectorNbr.clear();
  fDali_Energy.clear();
  // Time
  fDali_T_DetectorNbr.clear();
  fDali_Time.clear();
}



//////////////////////////////////////////////////////////////////////
void TDaliData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TDaliData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // Energy
  size_t mysize = fDali_E_DetectorNbr.size();
  cout << "Dali_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fDali_E_DetectorNbr[i]
         << " Energy: " << fDali_Energy[i];
  }
  
  // Time
  mysize = fDali_T_DetectorNbr.size();
  cout << "Dali_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fDali_T_DetectorNbr[i]
         << " Time: " << fDali_Time[i];
  }
}
