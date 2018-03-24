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


	Si_E.Detector.clear();
	Si_E.ThetaStrip.clear();
	Si_E.PhiStrip.clear();
	Si_E.Value.clear();


	CsI_T.Detector.clear();
	CsI_T.Wedge.clear();
	CsI_T.Value.clear();


	Si_T.Detector.clear();
	Si_T.ThetaStrip.clear();
	Si_T.PhiStrip.clear();
	Si_T.Value.clear();
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
  
  // CsI Time
  mysize = CsI_T.Detector.size();
  cout << "AnnularTelescope_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "CsI DetNbr: " << CsI_T.Detector[i]
				 << " CsI WedgeNbr: " << CsI_T.Detector[i]
         << " CsI Time: " << CsI_T.Value[i];
  }

  // Si Energy
	mysize = Si_E.Detector.size();
  cout << "Si:: AnnularTelescope_E_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "DetNbr: " << Si_E.Detector[i]
				 << " Si ThetaStripNbr: " << Si_E.ThetaStrip[i]
				 << " Si PhiStripNbr: " << Si_E.PhiStrip[i]
         << " Si Energy: " << Si_E.Value[i];
  }
  
  // Si Time
  mysize = Si_T.Detector.size();
  cout << "Si:: AnnularTelescope_T_Mult: " << mysize << endl;
 
  for (size_t i = 0 ; i < mysize ; i++){
    cout << "Si DetNbr: " << Si_T.Detector[i]
				 << " Si ThetaStripNbr: " << Si_T.ThetaStrip[i]
				 << " Si PhiStripNbr: " << Si_T.PhiStrip[i]
         << " Si Time: " << Si_T.Value[i];
  }
}
