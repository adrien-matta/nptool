/*****************************************************************************
 * Copyright (C) 2009-XYEARX   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: XAUTHORX  contact address: XMAILX                        *
 *                                                                           *
 * Creation Date  : XMONTHX XYEARX                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold DETECTORNAME Spectra                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// class header 
#include "TDETECTORNAMESpectra.h"

// STL
#include <iostream>  
#include <string>
using namespace std;

// NPTool header
#include "NPOptionManager.h"



////////////////////////////////////////////////////////////////////////////////
TDETECTORNAMESpectra::TDETECTORNAMESpectra() 
   : fNumberOfDetectors(0) {
  SetName("DETECTORNAME");
}



////////////////////////////////////////////////////////////////////////////////
TDETECTORNAMESpectra::TDETECTORNAMESpectra(unsigned int NumberOfDetectors) {
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TDETECTORNAMESpectra : Initalizing control spectra for " 
      << NumberOfDetectors << " Detectors" << endl
      << "************************************************" << endl ;
  SetName("DETECTORNAME");
  fNumberOfDetectors = NumberOfDetectors;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}



////////////////////////////////////////////////////////////////////////////////
TDETECTORNAMESpectra::~TDETECTORNAMESpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void TDETECTORNAMESpectra::InitRawSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy 
    name = "DETECTORNAME"+NPL::itoa(i+1)+"_ENERGY_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "DETECTORNAME/RAW");
    // Time 
    name = "DETECTORNAME"+NPL::itoa(i+1)+"_TIME_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "DETECTORNAME/RAW");
  } // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TDETECTORNAMESpectra::InitPreTreatedSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy 
    name = "DETECTORNAME"+NPL::itoa(i+1)+"_ENERGY_CAL";
    AddHisto1D(name, name, 500, 0, 25, "DETECTORNAME/CAL");
    // Time
    name = "DETECTORNAME"+NPL::itoa(i+1)+"_TIME_CAL";
    AddHisto1D(name, name, 500, 0, 25, "DETECTORNAME/CAL");

  
  }  // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TDETECTORNAMESpectra::InitPhysicsSpectra() {
  static string name;
  // Kinematic Plot 
  name = "DETECTORNAME_ENERGY_TIME";
  AddHisto2D(name, name, 500, 0, 500, 500, 0, 50, "DETECTORNAME/PHY");
}



////////////////////////////////////////////////////////////////////////////////
void TDETECTORNAMESpectra::FillRawSpectra(TDETECTORNAMEData* RawData) {
  static string name;
  static string family;

  // Energy 
  unsigned int sizeE = RawData->GetMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "DETECTORNAME"+NPL::itoa(RawData->GetE_DetectorNbr(i))+"_ENERGY_RAW";
    family = "DETECTORNAME/RAW";

    FillSpectra(family,name,RawData->Get_Energy(i));
  }

  // Time
  unsigned int sizeT = RawData->GetMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "DETECTORNAME"+NPL::itoa(RawData->GetT_DetectorNbr(i))+"_TIME_RAW";
    family = "DETECTORNAME/RAW";

    FillSpectra(family,name,RawData->Get_Time(i));
  }
}



////////////////////////////////////////////////////////////////////////////////
void TDETECTORNAMESpectra::FillPreTreatedSpectra(TDETECTORNAMEData* PreTreatedData) {
  static string name;
  static string family;
  
  // Energy 
  unsigned int sizeE = PreTreatedData->GetMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "DETECTORNAME"+NPL::itoa(PreTreatedData->GetE_DetectorNbr(i))+"_ENERGY_CAL";
    family = "DETECTORNAME/CAL";

    FillSpectra(family,name,PreTreatedData->Get_Energy(i));
  }

  // Time
  unsigned int sizeT = PreTreatedData->GetMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "DETECTORNAME"+NPL::itoa(PreTreatedData->GetT_DetectorNbr(i))+"_TIME_CAL";
    family = "DETECTORNAME/CAL";

    FillSpectra(family,name,PreTreatedData->Get_Time(i));
  }
}



////////////////////////////////////////////////////////////////////////////////
void TDETECTORNAMESpectra::FillPhysicsSpectra(TDETECTORNAMEPhysics* Physics) {
  static string name;
  static string family;
  family= "DETECTORNAME/PHY";

  // Energy vs time
  unsigned int sizeE = Physics->Energy.size();
  for(unsigned int i = 0 ; i < sizeE ; i++){
    name = "DETECTORNAME_ENERGY_TIME";
    FillSpectra(family,name,Physics->Energy[i],Physics->Time[i]);
  }
}

