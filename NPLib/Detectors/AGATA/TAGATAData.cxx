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
 *  This class hold AGATA Raw data                                        *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TAGATAData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TAGATAData)


//////////////////////////////////////////////////////////////////////
TAGATAData::TAGATAData() {
}



//////////////////////////////////////////////////////////////////////
TAGATAData::~TAGATAData() {
}



//////////////////////////////////////////////////////////////////////
void TAGATAData::Clear() {
  // Energy
  fAGATA_E_DetectorNbr.clear();
  fAGATA_Energy.clear();

  // Angle
  fAGATA_A_DetectorNbr.clear();
  fAGATA_Angle.clear();

  // Time
  fAGATA_T_DetectorNbr.clear();
  fAGATA_Time.clear();
}


//////////////////////////////////////////////////////////////////////
void TAGATAData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TAGATAData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // Energy
  size_t mysize = fAGATA_E_DetectorNbr.size();
  cout << "AGATA_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fAGATA_E_DetectorNbr[i]
         << " Energy: " << fAGATA_Energy[i];
  }
  
  // Angle
  mysize = fAGATA_A_DetectorNbr.size();
  cout << "AGATA_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fAGATA_A_DetectorNbr[i]
         << " Angle: " << fAGATA_Angle[i];
  }
  

  // Time
  mysize = fAGATA_T_DetectorNbr.size();
  cout << "AGATA_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fAGATA_T_DetectorNbr[i]
         << " Time: " << fAGATA_Time[i];
  }
}
