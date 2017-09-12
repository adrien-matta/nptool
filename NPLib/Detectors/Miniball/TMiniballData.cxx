/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : January 2016                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Miniball Raw data                                        *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TMiniballData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TMiniballData)


//////////////////////////////////////////////////////////////////////
TMiniballData::TMiniballData() {
}



//////////////////////////////////////////////////////////////////////
TMiniballData::~TMiniballData() {
}



//////////////////////////////////////////////////////////////////////
void TMiniballData::Clear() {
  // Energy
  fMiniball_E_DetectorNbr.clear();
  fMiniball_Energy.clear();

  // Angle
  fMiniball_A_DetectorNbr.clear();
  fMiniball_Angle.clear();

  // Time
  fMiniball_T_DetectorNbr.clear();
  fMiniball_Time.clear();
}


//////////////////////////////////////////////////////////////////////
void TMiniballData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TMiniballData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // Energy
  size_t mysize = fMiniball_E_DetectorNbr.size();
  cout << "Miniball_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fMiniball_E_DetectorNbr[i]
         << " Energy: " << fMiniball_Energy[i];
  }
  
  // Angle
  mysize = fMiniball_A_DetectorNbr.size();
  cout << "Miniball_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fMiniball_A_DetectorNbr[i]
         << " Angle: Theta: " << fMiniball_Angle[i].Px()
         << " Angle: Phi: " << fMiniball_Angle[i].Py()
         << " Angle: Alpha: " << fMiniball_Angle[i].Pz();
  }
  

  // Time
  mysize = fMiniball_T_DetectorNbr.size();
  cout << "Miniball_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fMiniball_T_DetectorNbr[i]
         << " Time: " << fMiniball_Time[i];
  }
}
