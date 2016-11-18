/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Mohamad Moukaddam  contact address: m.moukaddam@surrey.ac.uk                        *
 *                                                                           *
 * Creation Date  : November 2016                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold FPDTamu Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TFPDTamuData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TFPDTamuData)


//////////////////////////////////////////////////////////////////////
TFPDTamuData::TFPDTamuData() {
}



//////////////////////////////////////////////////////////////////////
TFPDTamuData::~TFPDTamuData() {
}



//////////////////////////////////////////////////////////////////////
void TFPDTamuData::Clear() {
  // Energy
  fFPDTamu_E_DetectorNbr.clear();
  fFPDTamu_Energy.clear();
  // Time
  fFPDTamu_T_DetectorNbr.clear();
  fFPDTamu_Time.clear();
}



//////////////////////////////////////////////////////////////////////
void TFPDTamuData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TFPDTamuData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // Energy
  size_t mysize = fFPDTamu_E_DetectorNbr.size();
  cout << "FPDTamu_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fFPDTamu_E_DetectorNbr[i]
         << " Energy: " << fFPDTamu_Energy[i];
  }
  
  // Time
  mysize = fFPDTamu_T_DetectorNbr.size();
  cout << "FPDTamu_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fFPDTamu_T_DetectorNbr[i]
         << " Time: " << fFPDTamu_Time[i];
  }
}
