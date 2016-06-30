/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
 *                                                                           *
 * Creation Date  : June 2016                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Microball Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TMicroballData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TMicroballData)


//////////////////////////////////////////////////////////////////////
TMicroballData::TMicroballData() {
}



//////////////////////////////////////////////////////////////////////
TMicroballData::~TMicroballData() {
}



//////////////////////////////////////////////////////////////////////
void TMicroballData::Clear() {
  // Energy
  fMicroball_E_DetectorNbr.clear();
  fMicroball_Energy.clear();
  // Time
  fMicroball_T_DetectorNbr.clear();
  fMicroball_Time.clear();
}



//////////////////////////////////////////////////////////////////////
void TMicroballData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TMicroballData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // Energy
  size_t mysize = fMicroball_E_DetectorNbr.size();
  cout << "Microball_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fMicroball_E_DetectorNbr[i]
         << " Energy: " << fMicroball_Energy[i];
  }
  
  // Time
  mysize = fMicroball_T_DetectorNbr.size();
  cout << "Microball_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fMicroball_T_DetectorNbr[i]
         << " Time: " << fMicroball_Time[i];
  }
}
