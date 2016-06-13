/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
 *                                                                           *
 * Creation Date  : June 2016                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold NeutronWall Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TNeutronWallData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TNeutronWallData)


//////////////////////////////////////////////////////////////////////
TNeutronWallData::TNeutronWallData() {
}



//////////////////////////////////////////////////////////////////////
TNeutronWallData::~TNeutronWallData() {
}



//////////////////////////////////////////////////////////////////////
void TNeutronWallData::Clear() {
  // Energy
  fNeutronWall_E_DetectorNbr.clear();
  fNeutronWall_Energy.clear();
  // Time
  fNeutronWall_T_DetectorNbr.clear();
  fNeutronWall_Time.clear();
}



//////////////////////////////////////////////////////////////////////
void TNeutronWallData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TNeutronWallData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // Energy
  size_t mysize = fNeutronWall_E_DetectorNbr.size();
  cout << "NeutronWall_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fNeutronWall_E_DetectorNbr[i]
         << " Energy: " << fNeutronWall_Energy[i];
  }
  
  // Time
  mysize = fNeutronWall_T_DetectorNbr.size();
  cout << "NeutronWall_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fNeutronWall_T_DetectorNbr[i]
         << " Time: " << fNeutronWall_Time[i];
  }
}
