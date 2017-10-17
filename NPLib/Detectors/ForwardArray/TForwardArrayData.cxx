/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
 *                                                                           *
 * Creation Date  : May 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold ForwardArray Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TForwardArrayData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TForwardArrayData)


//////////////////////////////////////////////////////////////////////
TForwardArrayData::TForwardArrayData() {
}



//////////////////////////////////////////////////////////////////////
TForwardArrayData::~TForwardArrayData() {
}



//////////////////////////////////////////////////////////////////////
void TForwardArrayData::Clear() {
  // Energy
  fForwardArray_E_DetectorNbr.clear();
  fForwardArray_Energy.clear();
  // Time
  fForwardArray_T_DetectorNbr.clear();
  fForwardArray_Time.clear();
}



//////////////////////////////////////////////////////////////////////
void TForwardArrayData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TForwardArrayData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // Energy
  size_t mysize = fForwardArray_E_DetectorNbr.size();
  cout << "ForwardArray_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fForwardArray_E_DetectorNbr[i]
         << " Energy: " << fForwardArray_Energy[i];
  }
  
  // Time
  mysize = fForwardArray_T_DetectorNbr.size();
  cout << "ForwardArray_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fForwardArray_T_DetectorNbr[i]
         << " Time: " << fForwardArray_Time[i];
  }
}
