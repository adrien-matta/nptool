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
 *  This class hold Spede Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TSpedeData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TSpedeData)


//////////////////////////////////////////////////////////////////////
TSpedeData::TSpedeData() {
}



//////////////////////////////////////////////////////////////////////
TSpedeData::~TSpedeData() {
}



//////////////////////////////////////////////////////////////////////
void TSpedeData::Clear() {
  // Energy
  fSpede_E_DetectorNbr.clear();
  fSpede_E_PixelNbr.clear();
  fSpede_Energy.clear();
  // Time
  fSpede_T_DetectorNbr.clear();
  fSpede_T_PixelNbr.clear();
  fSpede_Time.clear();
}



//////////////////////////////////////////////////////////////////////
void TSpedeData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TSpedeData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // Energy
  size_t mysize = fSpede_E_DetectorNbr.size();
  cout << "Spede_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fSpede_E_DetectorNbr[i]
         <<" PixNbr: " << fSpede_E_PixelNbr[i]
         <<" Energy: " << fSpede_Energy[i]
		 <<"\n";
  }
  
  // Time
  mysize = fSpede_T_DetectorNbr.size();
  cout << "Spede_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fSpede_T_DetectorNbr[i]
		 <<" PixNbr: " << fSpede_T_PixelNbr[i]
         <<" Time: "   << fSpede_Time[i]
		 <<"\n";
  }
}
