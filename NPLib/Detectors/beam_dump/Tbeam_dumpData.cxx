/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: valerian  contact address: girardalcindor@ganil.fr                        *
 *                                                                           *
 * Creation Date  : November 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold beam_dump Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "Tbeam_dumpData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(Tbeam_dumpData)


//////////////////////////////////////////////////////////////////////
Tbeam_dumpData::Tbeam_dumpData() {
}



//////////////////////////////////////////////////////////////////////
Tbeam_dumpData::~Tbeam_dumpData() {
}



//////////////////////////////////////////////////////////////////////
void Tbeam_dumpData::Clear() {
  // Energy
  fbeam_dump_E_DetectorNbr.clear();
  fbeam_dump_Energy.clear();
  // Time
  fbeam_dump_T_DetectorNbr.clear();
  fbeam_dump_Time.clear();
}



//////////////////////////////////////////////////////////////////////
void Tbeam_dumpData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [Tbeam_dumpData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // Energy
  size_t mysize = fbeam_dump_E_DetectorNbr.size();
  cout << "beam_dump_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fbeam_dump_E_DetectorNbr[i]
         << " Energy: " << fbeam_dump_Energy[i];
  }
  
  // Time
  mysize = fbeam_dump_T_DetectorNbr.size();
  cout << "beam_dump_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fbeam_dump_T_DetectorNbr[i]
         << " Time: " << fbeam_dump_Time[i];
  }
}
