/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Greg Christian  contact address: gchristian@tamu.edu                        *
 *                                                                           *
 * Creation Date  : October 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold MDM Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TMDMData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TMDMData)


//////////////////////////////////////////////////////////////////////
TMDMData::TMDMData() {
}



//////////////////////////////////////////////////////////////////////
TMDMData::~TMDMData() {
}



//////////////////////////////////////////////////////////////////////
void TMDMData::Clear() {
  // Energy
  fMDM_X_DetectorNbr.clear();
  fMDM_Xpos.clear();
  // Time
  fMDM_Y_DetectorNbr.clear();
  fMDM_Ypos.clear();
}



//////////////////////////////////////////////////////////////////////
void TMDMData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TMDMData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // X - position
  size_t mysize = fMDM_X_DetectorNbr.size();
  cout << "MDM_X_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fMDM_X_DetectorNbr[i]
         << " X position: " << fMDM_Xpos[i];
  }
  
  // Y - position
  mysize = fMDM_Y_DetectorNbr.size();
  cout << "MDM_Y_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fMDM_Y_DetectorNbr[i]
         << " Y position: " << fMDM_Ypos[i];
  }
}
