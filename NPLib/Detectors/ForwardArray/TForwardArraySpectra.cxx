/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
 *                                                                           *
 * Creation Date  : May 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold ForwardArray Spectra                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// class header 
#include "TForwardArraySpectra.h"

// STL
#include <iostream>  
#include <string>
using namespace std;

// NPTool header
#include "NPOptionManager.h"



////////////////////////////////////////////////////////////////////////////////
TForwardArraySpectra::TForwardArraySpectra() 
   : fNumberOfDetectors(0) {
  SetName("ForwardArray");
}



////////////////////////////////////////////////////////////////////////////////
TForwardArraySpectra::TForwardArraySpectra(unsigned int NumberOfDetectors) {
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TForwardArraySpectra : Initalizing control spectra for " 
      << NumberOfDetectors << " Detectors" << endl
      << "************************************************" << endl ;
  SetName("ForwardArray");
  fNumberOfDetectors = NumberOfDetectors;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}



////////////////////////////////////////////////////////////////////////////////
TForwardArraySpectra::~TForwardArraySpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void TForwardArraySpectra::InitRawSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy 
    name = "ForwardArray"+NPL::itoa(i+1)+"_ENERGY_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "ForwardArray/RAW");
    // Time 
    name = "ForwardArray"+NPL::itoa(i+1)+"_TIME_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "ForwardArray/RAW");
  } // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TForwardArraySpectra::InitPreTreatedSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy 
    name = "ForwardArray"+NPL::itoa(i+1)+"_ENERGY_CAL";
    AddHisto1D(name, name, 500, 0, 25, "ForwardArray/CAL");
    // Time
    name = "ForwardArray"+NPL::itoa(i+1)+"_TIME_CAL";
    AddHisto1D(name, name, 500, 0, 25, "ForwardArray/CAL");

  
  }  // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TForwardArraySpectra::InitPhysicsSpectra() {
  static string name;
  // Kinematic Plot 
  name = "ForwardArray_ENERGY_TIME";
  AddHisto2D(name, name, 500, 0, 500, 500, 0, 50, "ForwardArray/PHY");
}



////////////////////////////////////////////////////////////////////////////////
void TForwardArraySpectra::FillRawSpectra(TForwardArrayData* RawData) {
  static string name;
  static string family;

  // Energy 
  unsigned int sizeE = RawData->GetMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "ForwardArray"+NPL::itoa(RawData->GetE_DetectorNbr(i))+"_ENERGY_RAW";
    family = "ForwardArray/RAW";

    //GetHisto(family,name) -> Fill(RawData->Get_Energy(i));
  }

  // Time
  unsigned int sizeT = RawData->GetMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "ForwardArray"+NPL::itoa(RawData->GetT_DetectorNbr(i))+"_TIME_RAW";
    family = "ForwardArray/RAW";

    //GetHisto(family,name) -> Fill(RawData->Get_Time(i));
  }
}



////////////////////////////////////////////////////////////////////////////////
void TForwardArraySpectra::FillPreTreatedSpectra(TForwardArrayData* PreTreatedData) {
  static string name;
  static string family;
  
  // Energy 
  unsigned int sizeE = PreTreatedData->GetMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "ForwardArray"+NPL::itoa(PreTreatedData->GetE_DetectorNbr(i))+"_ENERGY_CAL";
    family = "ForwardArray/CAL";

    //GetHisto(family,name) -> Fill(PreTreatedData->Get_Energy(i));
  }

  // Time
  unsigned int sizeT = PreTreatedData->GetMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "ForwardArray"+NPL::itoa(PreTreatedData->GetT_DetectorNbr(i))+"_TIME_CAL";
    family = "ForwardArray/CAL";

    //GetHisto(family,name) -> Fill(PreTreatedData->Get_Time(i));
  }
}



////////////////////////////////////////////////////////////////////////////////
void TForwardArraySpectra::FillPhysicsSpectra(TForwardArrayPhysics* Physics) {
  static string name;
  static string family;
  family= "ForwardArray/PHY";

  // Energy vs time
  unsigned int sizeE = Physics->Energy.size();
  for(unsigned int i = 0 ; i < sizeE ; i++){
    name = "ForwardArray_ENERGY_TIME";
    //GetHisto(family,name) -> Fill(Physics->Energy[i],Physics->Time[i]);
  }
}

