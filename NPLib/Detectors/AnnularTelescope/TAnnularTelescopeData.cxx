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
	CsI_E.Detector.clear();
	CsI_E.Wedge.clear();
	CsI_E.Value.clear();

	CsI_T.Detector.clear();
	CsI_T.Wedge.clear();
	CsI_T.Value.clear();

	Si_Theta_E.Detector.clear();
	Si_Theta_E.Strip.clear();
	Si_Theta_E.Value.clear();

	Si_Theta_T.Detector.clear();
	Si_Theta_T.Strip.clear();
	Si_Theta_T.Value.clear();

	Si_Phi_E.Detector.clear();
	Si_Phi_E.Strip.clear();
	Si_Phi_E.Value.clear();

	Si_Phi_T.Detector.clear();
	Si_Phi_T.Strip.clear();
	Si_Phi_T.Value.clear();
}



//////////////////////////////////////////////////////////////////////
void TAnnularTelescopeData::Dump() const {
  // This method is very useful for debuging and worth the dev.
  cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TAnnularTelescopeData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

  // CsI Energy
  size_t mysize = CsI_E.Detector.size();
  cout << "AnnularTelescope_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << CsI_E.Detector[i]
				 << " CsI WedgeNbr: " << CsI_E.Wedge[i]
         << " CsI Energy: " << CsI_E.Value[i];
  }
	cout << "\n";
  
  // CsI Time
  mysize = CsI_T.Detector.size();
  cout << "AnnularTelescope_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "CsI DetNbr: " << CsI_T.Detector[i]
				 << " CsI WedgeNbr: " << CsI_T.Detector[i]
         << " CsI Time: " << CsI_T.Value[i];
  }
	cout << "\n";
	
  // Si Energy
	mysize = Si_Theta_E.Detector.size();
  cout << "Si:: AnnularTelescope_ThetaE_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << Si_Theta_E.Detector[i]
				 << " Si ThetaStripNbr: " << Si_Theta_E.Strip[i]
         << " Si ThetaStripEnergy: " << Si_Theta_E.Value[i];
  }
	cout << "\n";
  
  // Si Time
	mysize = Si_Theta_T.Detector.size();
  cout << "Si:: AnnularTelescope_ThetaT_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << Si_Theta_T.Detector[i]
				 << " Si ThetaStripNbr: " << Si_Theta_T.Strip[i]
         << " Si ThetaStripTime: " << Si_Theta_T.Value[i];
  }
	cout << "\n";

  // Si Energy (phi)
	mysize = Si_Phi_E.Detector.size();
  cout << "Si:: AnnularTelescope_PhiE_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << Si_Phi_E.Detector[i]
				 << " Si PhiStripNbr: " << Si_Phi_E.Strip[i]
         << " Si PhiStripEnergy: " << Si_Phi_E.Value[i];
  }
	cout << "\n";
  
  // Si Time (phi)
	mysize = Si_Phi_T.Detector.size();
  cout << "Si:: AnnularTelescope_PhiT_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << Si_Phi_T.Detector[i]
				 << " Si PhiStripNbr: " << Si_Phi_T.Strip[i]
         << " Si PhiStripTime: " << Si_Phi_T.Value[i];
  }
	cout << "\n";
}
