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
 *  This class hold Helios2 Raw data                                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "THelios2Data.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(THelios2Data)


//////////////////////////////////////////////////////////////////////
THelios2Data::THelios2Data() {
}

//////////////////////////////////////////////////////////////////////
THelios2Data::~THelios2Data() {
}

//////////////////////////////////////////////////////////////////////
void THelios2Data::Clear() {
  fHelios2_EBack_DetectorNbr.clear();
  fHelios2_EBack_Energy.clear();
  fHelios2_EUp_DetectorNbr.clear();
  fHelios2_EUp_Energy.clear();
  fHelios2_EDw_DetectorNbr.clear();
  fHelios2_EDw_Energy.clear();
 
  fHelios2_TBack_DetectorNbr.clear();
  fHelios2_TBack_Time.clear();
  fHelios2_TUp_DetectorNbr.clear();
  fHelios2_TUp_Time.clear();
  fHelios2_TDw_DetectorNbr.clear();
  fHelios2_TDw_Time.clear();
}

//////////////////////////////////////////////////////////////////////
void THelios2Data::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [THelios2Data::Dump()] XXXXXXXXXXXXXXXXX" << endl;
  
  // E Back
  size_t mysize = fHelios2_EBack_DetectorNbr.size();
  cout << "Helios2_EBack_Mult: " << mysize << endl;

  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fHelios2_EBack_DetectorNbr[i]
      << " Energy: " << fHelios2_EBack_Energy[i];
  }

  // E Up
  mysize = fHelios2_EUp_DetectorNbr.size();
  cout << "Helios2_EUp_Mult: " << mysize << endl;

  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fHelios2_EUp_DetectorNbr[i]
      << " Energy: " << fHelios2_EUp_Energy[i];
  }

  // E Dw
  mysize = fHelios2_EDw_DetectorNbr.size();
  cout << "Helios2_EDw_Mult: " << mysize << endl;

  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fHelios2_EDw_DetectorNbr[i]
      << " Energy: " << fHelios2_EDw_Energy[i];
  }

  // T Back
  mysize = fHelios2_TBack_DetectorNbr.size();
  cout << "Helios2_TBack_Mult: " << mysize << endl;

  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fHelios2_TBack_DetectorNbr[i]
      << " Time: " << fHelios2_TBack_Time[i];
  }

  // T Up
  mysize = fHelios2_TUp_DetectorNbr.size();
  cout << "Helios2_TUp_Mult: " << mysize << endl;

  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fHelios2_TUp_DetectorNbr[i]
      << " Time: " << fHelios2_TUp_Time[i];
  }

  // T Dw
  mysize = fHelios2_TDw_DetectorNbr.size();
  cout << "Helios2_TDw_Mult: " << mysize << endl;

  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fHelios2_TDw_DetectorNbr[i]
      << " Time: " << fHelios2_TDw_Time[i];
  }

}
