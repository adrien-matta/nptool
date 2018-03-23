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
 *  This class hold AnnularTelescope Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include "TAnnularTelescopeData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

ClassImp(TAnnularTelescopeData)


//////////////////////////////////////////////////////////////////////
TAnnularTelescopeData::TAnnularTelescopeData() {
	Clear();
}



//////////////////////////////////////////////////////////////////////
TAnnularTelescopeData::~TAnnularTelescopeData() {
}



//////////////////////////////////////////////////////////////////////
void TAnnularTelescopeData::Clear() {
  // Energy
  fAnnularTelescope_CsI_E_DetectorNbr.clear();
  fAnnularTelescope_CsI_Energy.clear();
  // Time
  fAnnularTelescope_CsI_T_DetectorNbr.clear();
  fAnnularTelescope_CsI_Time.clear();
}



//////////////////////////////////////////////////////////////////////
void TAnnularTelescopeData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TAnnularTelescopeData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // Energy
  size_t mysize = fAnnularTelescope_CsI_E_DetectorNbr.size();
  cout << "AnnularTelescope_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fAnnularTelescope_CsI_E_DetectorNbr[i]
         << " Energy: " << fAnnularTelescope_CsI_Energy[i];
  }
  
  // Time
  mysize = fAnnularTelescope_CsI_T_DetectorNbr.size();
  cout << "AnnularTelescope_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fAnnularTelescope_CsI_T_DetectorNbr[i]
         << " Time: " << fAnnularTelescope_CsI_Time[i];
  }
}
