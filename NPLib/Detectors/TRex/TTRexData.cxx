/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta  contact address: a.matta@surrey.ac.uk                        *
 *                                                                           *
 * Creation Date  : January 2016                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold TRex Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TTRexData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TTRexData)


//////////////////////////////////////////////////////////////////////
TTRexData::TTRexData() {
}



//////////////////////////////////////////////////////////////////////
TTRexData::~TTRexData() {
}



//////////////////////////////////////////////////////////////////////
void TTRexData::Clear() {
  // Front 
  // Energy
  fTRexFront_E_DetectorNbr.clear();
  fTRexFront_E_StripNbr.clear();
  fTRexFront_Energy.clear();
  // Time
  fTRexFront_T_DetectorNbr.clear();
  fTRexFront_T_StripNbr.clear();
  fTRexFront_Time.clear();
  
  // Back 
  // Energy
  fTRexBack_E_DetectorNbr.clear();
  fTRexBack_E_StripNbr.clear();
  fTRexBack_Energy.clear();
  // Time
  fTRexBack_T_DetectorNbr.clear();
  fTRexBack_T_StripNbr.clear();
  fTRexBack_Time.clear();

  // PAD 
  // Energy
  fTRexPAD_E_DetectorNbr.clear();
  fTRexPAD_Energy.clear();
  // Time
  fTRexPAD_T_DetectorNbr.clear();
  fTRexPAD_Time.clear();


}



//////////////////////////////////////////////////////////////////////
void TTRexData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TTRexData::Dump()] XXXXXXXXXXXXXXXXX" << endl;
  // Front
  // Energy
  size_t mysize = fTRexFront_E_DetectorNbr.size();
  cout << "TRex_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fTRexFront_E_DetectorNbr[i]
         << " Energy: " << fTRexFront_Energy[i];
  }
  
  // Time
  mysize = fTRexFront_T_DetectorNbr.size();
  cout << "TRex_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fTRexFront_T_DetectorNbr[i]
         << " Time: " << fTRexFront_Time[i];
  }
  // Back
  // Energy
  mysize = fTRexBack_E_DetectorNbr.size();
  cout << "TRex_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fTRexBack_E_DetectorNbr[i]
         << " Energy: " << fTRexBack_Energy[i];
  }
  
  // Time
  mysize = fTRexBack_T_DetectorNbr.size();
  cout << "TRex_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fTRexBack_T_DetectorNbr[i]
         << " Time: " << fTRexBack_Time[i];
  }
  // PAD
  // Energy
  mysize = fTRexPAD_E_DetectorNbr.size();
  cout << "TRex_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fTRexPAD_E_DetectorNbr[i]
         << " Energy: " << fTRexPAD_Energy[i];
  }
  
  // Time
  mysize = fTRexPAD_T_DetectorNbr.size();
  cout << "TRex_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fTRexPAD_T_DetectorNbr[i]
         << " Time: " << fTRexPAD_Time[i];
  }

}
