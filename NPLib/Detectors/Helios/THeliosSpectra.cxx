/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : octobre 2016                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Helios Spectra                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// class header 
#include "THeliosSpectra.h"

// STL
#include <iostream>  
#include <string>
using namespace std;

// NPTool header
#include "NPOptionManager.h"



////////////////////////////////////////////////////////////////////////////////
THeliosSpectra::THeliosSpectra() 
   : fNumberOfDetectors(0) {
  SetName("Helios");
}



////////////////////////////////////////////////////////////////////////////////
THeliosSpectra::THeliosSpectra(unsigned int NumberOfDetectors) {
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "THeliosSpectra : Initalizing control spectra for " 
      << NumberOfDetectors << " Detectors" << endl
      << "************************************************" << endl ;
  SetName("Helios");
  fNumberOfDetectors = NumberOfDetectors;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}



////////////////////////////////////////////////////////////////////////////////
THeliosSpectra::~THeliosSpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void THeliosSpectra::InitRawSpectra() {
}


////////////////////////////////////////////////////////////////////////////////
void THeliosSpectra::InitPreTreatedSpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void THeliosSpectra::InitPhysicsSpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void THeliosSpectra::FillRawSpectra(THeliosData* RawData) {
}



////////////////////////////////////////////////////////////////////////////////
void THeliosSpectra::FillPreTreatedSpectra(THeliosData* PreTreatedData) {
}



////////////////////////////////////////////////////////////////////////////////
void THeliosSpectra::FillPhysicsSpectra(THeliosPhysics* Physics) {
}

