/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Greg Christian  contact address: gchristian@tamu.edu                        *
 *                                                                           *
 * Creation Date  : July 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold LightPipe Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TLightPipeData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TLightPipeData)


//////////////////////////////////////////////////////////////////////
TLightPipeData::TLightPipeData() {
}



//////////////////////////////////////////////////////////////////////
TLightPipeData::~TLightPipeData() {
}



//////////////////////////////////////////////////////////////////////
void TLightPipeData::Clear() {
  // Energy
  fLightPipe_E_DetectorNbr.clear();
  fLightPipe_Energy.clear();
  // Time
  fLightPipe_T_DetectorNbr.clear();
  fLightPipe_Time.clear();
}



//////////////////////////////////////////////////////////////////////
void TLightPipeData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TLightPipeData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // Energy
  size_t mysize = fLightPipe_E_DetectorNbr.size();
  cout << "LightPipe_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fLightPipe_E_DetectorNbr[i]
         << " Energy: " << fLightPipe_Energy[i];
  }
  
  // Time
  mysize = fLightPipe_T_DetectorNbr.size();
  cout << "LightPipe_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fLightPipe_T_DetectorNbr[i]
         << " Time: " << fLightPipe_Time[i];
  }
}
