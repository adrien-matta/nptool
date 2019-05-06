/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Mohamad Moukaddam  contact address: mohamad.moukaddam@iphc.cnrs.fr                        *
 *                                                                           *
 * Creation Date  : November 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Pyramid Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TPyramidData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TPyramidData)


//////////////////////////////////////////////////////////////////////
TPyramidData::TPyramidData() {
}



//////////////////////////////////////////////////////////////////////
TPyramidData::~TPyramidData() {
}



//////////////////////////////////////////////////////////////////////
void TPyramidData::Clear() {
  // Energy
  fPyramid_E_DetectorNbr.clear();
  fPyramid_Energy.clear();
  // Time
  fPyramid_T_DetectorNbr.clear();
  fPyramid_Time.clear();
}



//////////////////////////////////////////////////////////////////////
void TPyramidData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TPyramidData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // Energy
  size_t mysize = fPyramid_E_DetectorNbr.size();
  cout << "Pyramid_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fPyramid_E_DetectorNbr[i]
         << " Energy: " << fPyramid_Energy[i];
  }
  
  // Time
  mysize = fPyramid_T_DetectorNbr.size();
  cout << "Pyramid_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fPyramid_T_DetectorNbr[i]
         << " Time: " << fPyramid_Time[i];
  }
}
