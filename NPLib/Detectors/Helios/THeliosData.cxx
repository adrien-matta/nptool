/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : octobre 2016                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Helios Raw data                                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "THeliosData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(THeliosData)


//////////////////////////////////////////////////////////////////////
THeliosData::THeliosData() {
}

//////////////////////////////////////////////////////////////////////
THeliosData::~THeliosData() {
}

//////////////////////////////////////////////////////////////////////
void THeliosData::Clear() {
  fHelios_EBack_DetectorNbr.clear();
  fHelios_EBack_Energy.clear();
  fHelios_EUp_DetectorNbr.clear();
  fHelios_EUp_Energy.clear();
  fHelios_EDw_DetectorNbr.clear();
  fHelios_EDw_Energy.clear();
 
  fHelios_TBack_DetectorNbr.clear();
  fHelios_TBack_Time.clear();
  fHelios_TUp_DetectorNbr.clear();
  fHelios_TUp_Time.clear();
  fHelios_TDw_DetectorNbr.clear();
  fHelios_TDw_Time.clear();
}

//////////////////////////////////////////////////////////////////////
void THeliosData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [THeliosData::Dump()] XXXXXXXXXXXXXXXXX" << endl;
  
  // E Back
  size_t mysize = fHelios_EBack_DetectorNbr.size();
  cout << "Helios_EBack_Mult: " << mysize << endl;

  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fHelios_EBack_DetectorNbr[i]
      << " Energy: " << fHelios_EBack_Energy[i];
  }

  // E Up
  mysize = fHelios_EUp_DetectorNbr.size();
  cout << "Helios_EUp_Mult: " << mysize << endl;

  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fHelios_EUp_DetectorNbr[i]
      << " Energy: " << fHelios_EUp_Energy[i];
  }

  // E Dw
  mysize = fHelios_EDw_DetectorNbr.size();
  cout << "Helios_EDw_Mult: " << mysize << endl;

  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fHelios_EDw_DetectorNbr[i]
      << " Energy: " << fHelios_EDw_Energy[i];
  }

  // T Back
  mysize = fHelios_TBack_DetectorNbr.size();
  cout << "Helios_TBack_Mult: " << mysize << endl;

  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fHelios_TBack_DetectorNbr[i]
      << " Time: " << fHelios_TBack_Time[i];
  }

  // T Up
  mysize = fHelios_TUp_DetectorNbr.size();
  cout << "Helios_TUp_Mult: " << mysize << endl;

  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fHelios_TUp_DetectorNbr[i]
      << " Time: " << fHelios_TUp_Time[i];
  }

  // T Dw
  mysize = fHelios_TDw_DetectorNbr.size();
  cout << "Helios_TDw_Mult: " << mysize << endl;

  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fHelios_TDw_DetectorNbr[i]
      << " Time: " << fHelios_TDw_Time[i];
  }

}
