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
 *  This class hold Sage Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TSageData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TSageData)


//////////////////////////////////////////////////////////////////////
TSageData::TSageData() {
}



//////////////////////////////////////////////////////////////////////
TSageData::~TSageData() {
}



//////////////////////////////////////////////////////////////////////
void TSageData::Clear() {
  // Energy
  fSage_E_DetectorNbr.clear();
  fSage_E_PixelNbr.clear();
  fSage_Energy.clear();
  // Time
  fSage_T_DetectorNbr.clear();
  fSage_T_PixelNbr.clear();
  fSage_Time.clear();
}



//////////////////////////////////////////////////////////////////////
void TSageData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TSageData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // Energy
  size_t mysize = fSage_E_DetectorNbr.size();
  cout << "Sage_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fSage_E_DetectorNbr[i]
         << "PixNbr: " << fSage_E_PixelNbr[i]
         <<" Energy: " << fSage_Energy[i];
  }
  
  // Time
  mysize = fSage_T_DetectorNbr.size();
  cout << "Sage_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fSage_T_DetectorNbr[i]
		 << "PixNbr: " << fSage_T_PixelNbr[i]
         << " Time: " << fSage_Time[i];
  }
}
