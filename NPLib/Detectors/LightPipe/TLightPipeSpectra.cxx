/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Greg Christian  contact address: gchristian@tamu.edu                        *
 *                                                                           *
 * Creation Date  : July 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold LightPipe Spectra                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// class header 
#include "TLightPipeSpectra.h"

// STL
#include <iostream>  
#include <string>
using namespace std;

// NPTool header
#include "NPOptionManager.h"



////////////////////////////////////////////////////////////////////////////////
TLightPipeSpectra::TLightPipeSpectra() 
   : fNumberOfDetectors(0) {
  SetName("LightPipe");
}



////////////////////////////////////////////////////////////////////////////////
TLightPipeSpectra::TLightPipeSpectra(unsigned int NumberOfDetectors) {
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TLightPipeSpectra : Initalizing control spectra for " 
      << NumberOfDetectors << " Detectors" << endl
      << "************************************************" << endl ;
  SetName("LightPipe");
  fNumberOfDetectors = NumberOfDetectors;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}



////////////////////////////////////////////////////////////////////////////////
TLightPipeSpectra::~TLightPipeSpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void TLightPipeSpectra::InitRawSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy 
    name = "LightPipe"+NPL::itoa(i+1)+"_ENERGY_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "LightPipe/RAW");
    // Time 
    name = "LightPipe"+NPL::itoa(i+1)+"_TIME_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "LightPipe/RAW");
  } // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TLightPipeSpectra::InitPreTreatedSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy 
    name = "LightPipe"+NPL::itoa(i+1)+"_ENERGY_CAL";
    AddHisto1D(name, name, 500, 0, 25, "LightPipe/CAL");
    // Time
    name = "LightPipe"+NPL::itoa(i+1)+"_TIME_CAL";
    AddHisto1D(name, name, 500, 0, 25, "LightPipe/CAL");

  
  }  // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TLightPipeSpectra::InitPhysicsSpectra() {
  static string name;
  // Kinematic Plot 
  name = "LightPipe_ENERGY_TIME";
  AddHisto2D(name, name, 500, 0, 500, 500, 0, 50, "LightPipe/PHY");
}



////////////////////////////////////////////////////////////////////////////////
void TLightPipeSpectra::FillRawSpectra(TLightPipeData* RawData) {
  // static string name;
  // static string family;

  // // Energy 
  // unsigned int sizeE = RawData->GetMultEnergy();
  // for (unsigned int i = 0; i < sizeE; i++) {
  //   name = "LightPipe"+NPL::itoa(RawData->GetE_DetectorNbr(i))+"_ENERGY_RAW";
  //   family = "LightPipe/RAW";

  //   FillSpectra(family,name,RawData->Get_Energy(i));
  // }

  // // Time
  // unsigned int sizeT = RawData->GetMultTime();
  // for (unsigned int i = 0; i < sizeT; i++) {
  //   name = "LightPipe"+NPL::itoa(RawData->GetT_DetectorNbr(i))+"_TIME_RAW";
  //   family = "LightPipe/RAW";

  //   FillSpectra(family,name,RawData->Get_Time(i));
  // }
}



////////////////////////////////////////////////////////////////////////////////
void TLightPipeSpectra::FillPreTreatedSpectra(TLightPipeData* PreTreatedData) {
  // static string name;
  // static string family;
  
  // // Energy 
  // unsigned int sizeE = PreTreatedData->GetMultEnergy();
  // for (unsigned int i = 0; i < sizeE; i++) {
  //   name = "LightPipe"+NPL::itoa(PreTreatedData->GetE_DetectorNbr(i))+"_ENERGY_CAL";
  //   family = "LightPipe/CAL";

  //   FillSpectra(family,name,PreTreatedData->Get_Energy(i));
  // }

  // // Time
  // unsigned int sizeT = PreTreatedData->GetMultTime();
  // for (unsigned int i = 0; i < sizeT; i++) {
  //   name = "LightPipe"+NPL::itoa(PreTreatedData->GetT_DetectorNbr(i))+"_TIME_CAL";
  //   family = "LightPipe/CAL";

  //   FillSpectra(family,name,PreTreatedData->Get_Time(i));
  // }
}



////////////////////////////////////////////////////////////////////////////////
void TLightPipeSpectra::FillPhysicsSpectra(TLightPipePhysics* Physics) {
  // static string name;
  // static string family;
  // family= "LightPipe/PHY";

  // // Energy vs time
  // unsigned int sizeE = Physics->Energy.size();
  // for(unsigned int i = 0 ; i < sizeE ; i++){
  //   name = "LightPipe_ENERGY_TIME";
  //   FillSpectra(family,name,Physics->Energy[i],Physics->Time[i]);
  // }
}

