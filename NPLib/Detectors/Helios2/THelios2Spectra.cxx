/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta  contact address: matta@lpccaen.in2p3.fr                        *
 *                                                                           *
 * Creation Date  : octobre 2016                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Helios2 Spectra                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// class header 
#include "THelios2Spectra.h"

// STL
#include <iostream>  
#include <string>
using namespace std;

// NPTool header
#include "NPOptionManager.h"



////////////////////////////////////////////////////////////////////////////////
THelios2Spectra::THelios2Spectra() 
   : fNumberOfDetectors(0) {
  SetName("Helios2");
}



////////////////////////////////////////////////////////////////////////////////
THelios2Spectra::THelios2Spectra(unsigned int NumberOfDetectors) {
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "THelios2Spectra : Initalizing control spectra for " 
      << NumberOfDetectors << " Detectors" << endl
      << "************************************************" << endl ;
  SetName("Helios2");
  fNumberOfDetectors = NumberOfDetectors;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}



////////////////////////////////////////////////////////////////////////////////
THelios2Spectra::~THelios2Spectra() {
}



////////////////////////////////////////////////////////////////////////////////
void THelios2Spectra::InitRawSpectra() {
}


////////////////////////////////////////////////////////////////////////////////
void THelios2Spectra::InitPreTreatedSpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void THelios2Spectra::InitPhysicsSpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void THelios2Spectra::FillRawSpectra(THelios2Data* RawData) {
}



////////////////////////////////////////////////////////////////////////////////
void THelios2Spectra::FillPreTreatedSpectra(THelios2Data* PreTreatedData) {
}



////////////////////////////////////////////////////////////////////////////////
void THelios2Spectra::FillPhysicsSpectra(THelios2Physics* Physics) {
}

