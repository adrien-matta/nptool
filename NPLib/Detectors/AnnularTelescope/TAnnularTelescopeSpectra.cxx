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
 *  This class hold AnnularTelescope Spectra                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// class header 
#include "TAnnularTelescopeSpectra.h"

// STL
#include <iostream>  
#include <string>
using namespace std;

// NPTool header
#include "NPOptionManager.h"



////////////////////////////////////////////////////////////////////////////////
TAnnularTelescopeSpectra::TAnnularTelescopeSpectra() 
   : fNumberOfDetectors(0) {
  SetName("AnnularTelescope");
}



////////////////////////////////////////////////////////////////////////////////
TAnnularTelescopeSpectra::TAnnularTelescopeSpectra(unsigned int NumberOfDetectors) {
  // if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
  //   cout << "************************************************" << endl
  //     << "TAnnularTelescopeSpectra : Initalizing control spectra for " 
  //     << NumberOfDetectors << " Detectors" << endl
  //     << "************************************************" << endl ;
  SetName("AnnularTelescope");
  fNumberOfDetectors = NumberOfDetectors;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}



////////////////////////////////////////////////////////////////////////////////
TAnnularTelescopeSpectra::~TAnnularTelescopeSpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void TAnnularTelescopeSpectra::InitRawSpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void TAnnularTelescopeSpectra::InitPreTreatedSpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void TAnnularTelescopeSpectra::InitPhysicsSpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void TAnnularTelescopeSpectra::FillRawSpectra(TAnnularTelescopeData* RawData) {
}



////////////////////////////////////////////////////////////////////////////////
void TAnnularTelescopeSpectra::FillPreTreatedSpectra(TAnnularTelescopeData* PreTreatedData) {
}



////////////////////////////////////////////////////////////////////////////////
void TAnnularTelescopeSpectra::FillPhysicsSpectra(TAnnularTelescopePhysics* Physics) {
}

