/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Dustin Scriven  contact address: scrivend@tamu.edu                        *
 *                                                                           *
 * Creation Date  : June 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold TNT Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TTNTData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TTNTData)


//////////////////////////////////////////////////////////////////////
TTNTData::TTNTData() {
}



//////////////////////////////////////////////////////////////////////
TTNTData::~TTNTData() {
}



//////////////////////////////////////////////////////////////////////
void TTNTData::Clear() {
  // Energy
  fTNT_E_DetectorNbr.clear();
  fTNT_Energy.clear();
  // Time
  fTNT_T_DetectorNbr.clear();
  fTNT_Time.clear();
}



//////////////////////////////////////////////////////////////////////
void TTNTData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TTNTData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // Energy
  size_t mysize = fTNT_E_DetectorNbr.size();
  cout << "TNT_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fTNT_E_DetectorNbr[i]
         << " Energy: " << fTNT_Energy[i];
  }
  
  // Time
  mysize = fTNT_T_DetectorNbr.size();
  cout << "TNT_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fTNT_T_DetectorNbr[i]
         << " Time: " << fTNT_Time[i];
  }
}
