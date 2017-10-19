/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Greg Christian  contact address: gchristian@tamu.edu                        *
 *                                                                           *
 * Creation Date  : October 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold MDM Spectra                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// class header 
#include "TMDMSpectra.h"

// STL
#include <iostream>  
#include <string>
using namespace std;

// NPTool header
#include "NPOptionManager.h"



////////////////////////////////////////////////////////////////////////////////
TMDMSpectra::TMDMSpectra() 
   : fNumberOfDetectors(0) {
  SetName("MDM");
}



////////////////////////////////////////////////////////////////////////////////
TMDMSpectra::TMDMSpectra(unsigned int NumberOfDetectors) {
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TMDMSpectra : Initalizing control spectra for " 
      << NumberOfDetectors << " Detectors" << endl
      << "************************************************" << endl ;
  SetName("MDM");
  fNumberOfDetectors = NumberOfDetectors;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}



////////////////////////////////////////////////////////////////////////////////
TMDMSpectra::~TMDMSpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void TMDMSpectra::InitRawSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy 
    name = "MDM"+NPL::itoa(i+1)+"_ENERGY_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "MDM/RAW");
    // Time 
    name = "MDM"+NPL::itoa(i+1)+"_TIME_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "MDM/RAW");
  } // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TMDMSpectra::InitPreTreatedSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy 
    name = "MDM"+NPL::itoa(i+1)+"_ENERGY_CAL";
    AddHisto1D(name, name, 500, 0, 25, "MDM/CAL");
    // Time
    name = "MDM"+NPL::itoa(i+1)+"_TIME_CAL";
    AddHisto1D(name, name, 500, 0, 25, "MDM/CAL");

  
  }  // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TMDMSpectra::InitPhysicsSpectra() {
  static string name;
  // Kinematic Plot 
  name = "MDM_ENERGY_TIME";
  AddHisto2D(name, name, 500, 0, 500, 500, 0, 50, "MDM/PHY");
}



////////////////////////////////////////////////////////////////////////////////
void TMDMSpectra::FillRawSpectra(TMDMData* RawData) {
# if 0
  static string name;
  static string family;

  // Energy 
  unsigned int sizeE = RawData->GetMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "MDM"+NPL::itoa(RawData->GetE_DetectorNbr(i))+"_ENERGY_RAW";
    family = "MDM/RAW";

    FillSpectra(family,name,RawData->Get_Energy(i));
  }

  // Time
  unsigned int sizeT = RawData->GetMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "MDM"+NPL::itoa(RawData->GetT_DetectorNbr(i))+"_TIME_RAW";
    family = "MDM/RAW";

    FillSpectra(family,name,RawData->Get_Time(i));
  }
#endif
}



////////////////////////////////////////////////////////////////////////////////
void TMDMSpectra::FillPreTreatedSpectra(TMDMData* PreTreatedData) {
#if 0
  static string name;
  static string family;
  
  // Energy 
  unsigned int sizeE = PreTreatedData->GetMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "MDM"+NPL::itoa(PreTreatedData->GetE_DetectorNbr(i))+"_ENERGY_CAL";
    family = "MDM/CAL";

    FillSpectra(family,name,PreTreatedData->Get_Energy(i));
  }

  // Time
  unsigned int sizeT = PreTreatedData->GetMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "MDM"+NPL::itoa(PreTreatedData->GetT_DetectorNbr(i))+"_TIME_CAL";
    family = "MDM/CAL";

    FillSpectra(family,name,PreTreatedData->Get_Time(i));
  }
#endif
}



////////////////////////////////////////////////////////////////////////////////
void TMDMSpectra::FillPhysicsSpectra(TMDMPhysics* Physics) {
#if 0
  static string name;
  static string family;
  family= "MDM/PHY";

  // Energy vs time
  unsigned int sizeE = Physics->Energy.size();
  for(unsigned int i = 0 ; i < sizeE ; i++){
    name = "MDM_ENERGY_TIME";
    FillSpectra(family,name,Physics->Energy[i],Physics->Time[i]);
  }
#endif
}

