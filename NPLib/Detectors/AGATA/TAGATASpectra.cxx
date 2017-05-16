/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta  contact address: a.matta@surrey.ac.uk                        *
 *                                                                           *
 * Creation Date  : January 2016                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold AGATA Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// class header 
#include "TAGATASpectra.h"

// STL
#include <iostream>  
#include <string>
using namespace std;

// NPTool header
#include "NPOptionManager.h"



////////////////////////////////////////////////////////////////////////////////
TAGATASpectra::TAGATASpectra() 
   : fNumberOfDetectors(0) {
  SetName("AGATA");
}



////////////////////////////////////////////////////////////////////////////////
TAGATASpectra::TAGATASpectra(unsigned int NumberOfDetectors) {
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TAGATASpectra : Initalizing control spectra for " 
      << NumberOfDetectors << " Detectors" << endl
      << "************************************************" << endl ;
  SetName("AGATA");
  fNumberOfDetectors = NumberOfDetectors;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}



////////////////////////////////////////////////////////////////////////////////
TAGATASpectra::~TAGATASpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void TAGATASpectra::InitRawSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy 
    name = "AGATA"+NPL::itoa(i+1)+"_ENERGY_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "AGATA/RAW");
    // Time 
    name = "AGATA"+NPL::itoa(i+1)+"_TIME_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "AGATA/RAW");

  
  } // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TAGATASpectra::InitPreTreatedSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy 
    name = "AGATA"+NPL::itoa(i+1)+"_ENERGY_CAL";
    AddHisto1D(name, name, 500, 0, 25, "AGATA/CAL");
    // Time
    name = "AGATA"+NPL::itoa(i+1)+"_TIME_CAL";
    AddHisto1D(name, name, 500, 0, 25, "AGATA/CAL");

  
  }  // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TAGATASpectra::InitPhysicsSpectra() {
  static string name;
  // Kinematic Plot 
  name = "AGATA_ENERGY_TIME";
  AddHisto2D(name, name, 500, -500, 0, 500, 0, 50, "AGATA/PHY");
}



////////////////////////////////////////////////////////////////////////////////
void TAGATASpectra::FillRawSpectra(TAGATAData* RawData) {
  static string name;
  static string family;

  // Energy 
  unsigned int sizeE = RawData->GetMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "AGATA"+NPL::itoa(RawData->GetE_DetectorNbr(i))+"_ENERGY_RAW";
    family = "AGATA/RAW";

    FillSpectra(family,name,RawData->GetE_DetectorNbr(i), 
                                  RawData->Get_Energy(i));
  }

  // Time
  unsigned int sizeT = RawData->GetMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "AGATA"+NPL::itoa(RawData->GetT_DetectorNbr(i))+"_TIME_RAW";
    family = "AGATA/RAW";

    FillSpectra(family,name,RawData->GetT_DetectorNbr(i), 
                                  RawData->Get_Time(i));
  }
}



////////////////////////////////////////////////////////////////////////////////
void TAGATASpectra::FillPreTreatedSpectra(TAGATAData* PreTreatedData) {
  static string name;
  static string family;
  
  // Energy 
  unsigned int sizeE = PreTreatedData->GetMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "AGATA"+NPL::itoa(PreTreatedData->GetE_DetectorNbr(i))+"_ENERGY_CAL";
    family = "AGATA/CAL";

    FillSpectra(family,name,PreTreatedData->GetE_DetectorNbr(i), 
                                  PreTreatedData->Get_Energy(i));
  }

  // Time
  unsigned int sizeT = PreTreatedData->GetMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "AGATA"+NPL::itoa(PreTreatedData->GetT_DetectorNbr(i))+"_TIME_CAL";
    family = "AGATA/CAL";

    FillSpectra(family,name,PreTreatedData->GetT_DetectorNbr(i), 
                                  PreTreatedData->Get_Time(i));
  }
}



////////////////////////////////////////////////////////////////////////////////
void TAGATASpectra::FillPhysicsSpectra(TAGATAPhysics* Physics) {
  static string name;
  static string family;
  family= "AGATA/PHY";

  // Energy vs time
  unsigned int sizeE = Physics->Energy.size();
  for(unsigned int i = 0 ; i < sizeE ; i++){
    name = "AGATA_ENERGY_TIME";
    FillSpectra(family,name,Physics->Energy[i],Physics->Time[i]);
  }
}

