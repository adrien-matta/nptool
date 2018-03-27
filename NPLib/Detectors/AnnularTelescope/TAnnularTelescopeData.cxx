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
	CsI_E_Detector.clear();
	CsI_E_Wedge.clear();
	CsI_E_Energy.clear();

	CsI_T_Detector.clear();
	CsI_T_Wedge.clear();
	CsI_T_Time.clear();

	Si_Theta_E_Detector.clear();
	Si_Theta_E_Strip.clear();
	Si_Theta_E_Energy.clear();

	Si_Theta_T_Detector.clear();
	Si_Theta_T_Strip.clear();
	Si_Theta_T_Time.clear();

	Si_Phi_E_Detector.clear();
	Si_Phi_E_Strip.clear();
	Si_Phi_E_Energy.clear();

	Si_Phi_T_Detector.clear();
	Si_Phi_T_Strip.clear();
	Si_Phi_T_Time.clear();
}



//////////////////////////////////////////////////////////////////////
void TAnnularTelescopeData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TAnnularTelescopeData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // CsI Energy
  size_t mysize = CsI_E_Detector.size();
  cout << "AnnularTelescope_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << CsI_E_Detector[i]
				 << " CsI WedgeNbr: " << CsI_E_Wedge[i]
         << " CsI Energy: " << CsI_E_Energy[i];
  }
	cout << "\n";
  
  // CsI Time
  mysize = CsI_T_Detector.size();
  cout << "AnnularTelescope_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "CsI DetNbr: " << CsI_T_Detector[i]
				 << " CsI WedgeNbr: " << CsI_T_Detector[i]
         << " CsI Time: " << CsI_T_Time[i];
  }
	cout << "\n";
	
  // Si Energy
	mysize = Si_Theta_E_Detector.size();
  cout << "Si:: AnnularTelescope_ThetaE_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << Si_Theta_E_Detector[i]
				 << " Si ThetaStripNbr: " << Si_Theta_E_Strip[i]
         << " Si ThetaStripEnergy: " << Si_Theta_E_Energy[i];
  }
	cout << "\n";
  
  // Si Time
	mysize = Si_Theta_T_Detector.size();
  cout << "Si:: AnnularTelescope_ThetaT_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << Si_Theta_T_Detector[i]
				 << " Si ThetaStripNbr: " << Si_Theta_T_Strip[i]
         << " Si ThetaStripTime: " << Si_Theta_T_Time[i];
  }
	cout << "\n";

  // Si Energy (phi)
	mysize = Si_Phi_E_Detector.size();
  cout << "Si:: AnnularTelescope_PhiE_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << Si_Phi_E_Detector[i]
				 << " Si PhiStripNbr: " << Si_Phi_E_Strip[i]
         << " Si PhiStripEnergy: " << Si_Phi_E_Energy[i];
  }
	cout << "\n";
  
  // Si Time (phi)
	mysize = Si_Phi_T_Detector.size();
  cout << "Si:: AnnularTelescope_PhiT_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << Si_Phi_T_Detector[i]
				 << " Si PhiStripNbr: " << Si_Phi_T_Strip[i]
         << " Si PhiStripTime: " << Si_Phi_T_Time[i];
  }
	cout << "\n";
}
