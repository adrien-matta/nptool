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
	fAnnularTelescope_CsI_E_DetectorNbr.clear();
	fAnnularTelescope_CsI_E_WedgeNbr.clear();
	fAnnularTelescope_CsI_Energy.clear();


	fAnnularTelescope_Si_E_DetectorNbr.clear();
	fAnnularTelescope_Si_E_ThetaStripNbr.clear();
	fAnnularTelescope_Si_E_PhiStripNbr.clear();
	fAnnularTelescope_Si_Energy.clear();


	fAnnularTelescope_CsI_T_DetectorNbr.clear();
	fAnnularTelescope_CsI_T_WedgeNbr.clear();
	fAnnularTelescope_CsI_Time.clear();


	fAnnularTelescope_Si_T_DetectorNbr.clear();
	fAnnularTelescope_Si_T_ThetaStripNbr.clear();
	fAnnularTelescope_Si_T_PhiStripNbr.clear();
	fAnnularTelescope_Si_Time.clear();
}



//////////////////////////////////////////////////////////////////////
void TAnnularTelescopeData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TAnnularTelescopeData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // CsI Energy
  size_t mysize = fAnnularTelescope_CsI_E_DetectorNbr.size();
  cout << "AnnularTelescope_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fAnnularTelescope_CsI_E_DetectorNbr[i]
				 << " CsI WedgeNbr: " << fAnnularTelescope_CsI_E_WedgeNbr[i]
         << " CsI Energy: " << fAnnularTelescope_CsI_Energy[i];
  }
  
  // CsI Time
  mysize = fAnnularTelescope_CsI_T_DetectorNbr.size();
  cout << "AnnularTelescope_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "CsI DetNbr: " << fAnnularTelescope_CsI_T_DetectorNbr[i]
				 << " CsI WedgeNbr: " << fAnnularTelescope_CsI_T_DetectorNbr[i]
         << " CsI Time: " << fAnnularTelescope_CsI_Time[i];
  }

  // Si Energy
	mysize = fAnnularTelescope_Si_E_DetectorNbr.size();
  cout << "Si:: AnnularTelescope_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << fAnnularTelescope_Si_E_DetectorNbr[i]
				 << " Si ThetaStripNbr: " << fAnnularTelescope_Si_E_ThetaStripNbr[i]
				 << " Si PhiStripNbr: " << fAnnularTelescope_Si_E_PhiStripNbr[i]
         << " Si Energy: " << fAnnularTelescope_Si_Energy[i];
  }
  
  // Si Time
  mysize = fAnnularTelescope_Si_T_DetectorNbr.size();
  cout << "Si:: AnnularTelescope_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "Si DetNbr: " << fAnnularTelescope_Si_T_DetectorNbr[i]
				 << " Si ThetaStripNbr: " << fAnnularTelescope_Si_T_ThetaStripNbr[i]
				 << " Si PhiStripNbr: " << fAnnularTelescope_Si_T_PhiStripNbr[i]
         << " Si Time: " << fAnnularTelescope_Si_Time[i];
  }
}
