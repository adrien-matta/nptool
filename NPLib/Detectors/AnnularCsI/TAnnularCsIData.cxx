/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Greg Christian  contact address: gchristian@tamu.edu                        *
 *                                                                           *
 * Creation Date  : March 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold AnnularCsI Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TAnnularCsIData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TAnnularCsIData)


//////////////////////////////////////////////////////////////////////
TAnnularCsIData::TAnnularCsIData() {
}



//////////////////////////////////////////////////////////////////////
TAnnularCsIData::~TAnnularCsIData() {
}



//////////////////////////////////////////////////////////////////////
void TAnnularCsIData::Clear() {
  // Energy
  fAnnularCsI_E_DetectorNbr.clear();
  fAnnularCsI_Energy.clear();
  // Time
  fAnnularCsI_T_DetectorNbr.clear();
  fAnnularCsI_Time.clear();
}



//////////////////////////////////////////////////////////////////////
void TAnnularCsIData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TAnnularCsIData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // Energy
  size_t mysize = fAnnularCsI_E_DetectorNbr.size();
  cout << "AnnularCsI_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fAnnularCsI_E_DetectorNbr[i]
         << " Energy: " << fAnnularCsI_Energy[i];
  }
  
  // Time
  mysize = fAnnularCsI_T_DetectorNbr.size();
  cout << "AnnularCsI_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fAnnularCsI_T_DetectorNbr[i]
         << " Time: " << fAnnularCsI_Time[i];
  }
}
