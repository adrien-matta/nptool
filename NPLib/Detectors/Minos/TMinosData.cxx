/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Elidiano Tronchin  contact address: tronchin@lpccaen.in2p3.fr                        *
 *                                                                           *
 * Creation Date  : October 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Minos Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TMinosData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TMinosData)


//////////////////////////////////////////////////////////////////////
TMinosData::TMinosData() {
}



//////////////////////////////////////////////////////////////////////
TMinosData::~TMinosData() {
}



//////////////////////////////////////////////////////////////////////
void TMinosData::Clear() {
  // Energy
  fMinos_E_DetectorNbr.clear();
  fMinos_Energy.clear();
  // Time
  fMinos_T_DetectorNbr.clear();
  fMinos_Time.clear();
}



//////////////////////////////////////////////////////////////////////
void TMinosData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TMinosData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // Energy
  size_t mysize = fMinos_E_DetectorNbr.size();
  cout << "Minos_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fMinos_E_DetectorNbr[i]
         << " Energy: " << fMinos_Energy[i];
  }
  
  // Time
  mysize = fMinos_T_DetectorNbr.size();
  cout << "Minos_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fMinos_T_DetectorNbr[i]
         << " Time: " << fMinos_Time[i];
  }
}
