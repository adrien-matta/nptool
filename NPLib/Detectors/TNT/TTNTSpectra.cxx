/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Dustin Scriven  contact address: scrivend@tamu.edu                        *
 *                                                                           *
 * Creation Date  : July 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold TNT Spectra                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// class header 
#include "TTNTSpectra.h"

// STL
#include <iostream>  
#include <string>
using namespace std;

// NPTool header
#include "NPOptionManager.h"



////////////////////////////////////////////////////////////////////////////////
TTNTSpectra::TTNTSpectra() 
   : fNumberOfDetectors(0) {
  SetName("TNT");
}



////////////////////////////////////////////////////////////////////////////////
TTNTSpectra::TTNTSpectra(unsigned int NumberOfDetectors) {
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TTNTSpectra : Initalizing control spectra for " 
      << NumberOfDetectors << " Detectors" << endl
      << "************************************************" << endl ;
  SetName("TNT");
  fNumberOfDetectors = NumberOfDetectors;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}



////////////////////////////////////////////////////////////////////////////////
TTNTSpectra::~TTNTSpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void TTNTSpectra::InitRawSpectra() {
  cout << "check11" << endl;
  static string name;
  cout << "test11" << endl;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy 
    cout << "check12" << endl;
    name = "TNT"+NPL::itoa(i+1)+"_ENERGY_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "TNT/RAW");
    cout << "test12" << endl;
    // Time 
    cout << "check13" << endl;
    name = "TNT"+NPL::itoa(i+1)+"_TIME_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "TNT/RAW");
    cout << "test13" << endl;
  } // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TTNTSpectra::InitPreTreatedSpectra() {
  cout << "check14" << endl;
  static string name;
  cout << "test14" << endl;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy 
    cout << "check15" << endl;
    name = "TNT"+NPL::itoa(i+1)+"_ENERGY_CAL";
    AddHisto1D(name, name, 500, 0, 25, "TNT/CAL");
    cout << "test15" << endl;
    // Time
    cout << "check16" << endl;
    name = "TNT"+NPL::itoa(i+1)+"_TIME_CAL";
    AddHisto1D(name, name, 500, 0, 25, "TNT/CAL");
    cout << "test16" << endl;
  
  }  // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TTNTSpectra::InitPhysicsSpectra() {
  cout << "check17" << endl;
  static string name;
  cout << "test17" << endl;
  // Kinematic Plot 
  cout << "check18" << endl;
  name = "TNT_ENERGY_TIME";
  AddHisto2D(name, name, 500, 0, 500, 500, 0, 50, "TNT/PHY");
  cout << "test18" << endl;
}



////////////////////////////////////////////////////////////////////////////////
void TTNTSpectra::FillRawSpectra(TTNTData* RawData) {
  cout << "check19" << endl;
  static string name;
  static string family;

  // Energy 
  unsigned int sizeE = RawData->GetMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "TNT"+NPL::itoa(RawData->GetE_DetectorNbr(i))+"_ENERGY_RAW";
    family = "TNT/RAW";

    FillSpectra(family,name,RawData->Get_Energy(i));
  }

  // Time
  unsigned int sizeT = RawData->GetMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "TNT"+NPL::itoa(RawData->GetT_DetectorNbr(i))+"_TIME_RAW";
    family = "TNT/RAW";

    FillSpectra(family,name,RawData->Get_Time(i));
  }
  cout << "test19" << endl;
}



////////////////////////////////////////////////////////////////////////////////
void TTNTSpectra::FillPreTreatedSpectra(TTNTData* PreTreatedData) {
  cout << "check20" << endl; 
  static string name;
  static string family;
  
  // Energy 
  unsigned int sizeE = PreTreatedData->GetMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "TNT"+NPL::itoa(PreTreatedData->GetE_DetectorNbr(i))+"_ENERGY_CAL";
    family = "TNT/CAL";

    FillSpectra(family,name,PreTreatedData->Get_Energy(i));
  }

  // Time
  unsigned int sizeT = PreTreatedData->GetMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "TNT"+NPL::itoa(PreTreatedData->GetT_DetectorNbr(i))+"_TIME_CAL";
    family = "TNT/CAL";

    FillSpectra(family,name,PreTreatedData->Get_Time(i));
  }
  cout << "test20" << endl;
}



////////////////////////////////////////////////////////////////////////////////
void TTNTSpectra::FillPhysicsSpectra(TTNTPhysics* Physics) {
  cout << "test21" << endl;
  static string name;
  static string family;
  family= "TNT/PHY";

  // Energy vs time
  unsigned int sizeE = Physics->Energy.size();
  for(unsigned int i = 0 ; i < sizeE ; i++){
    name = "TNT_ENERGY_TIME";
    FillSpectra(family,name,Physics->Energy[i],Physics->Time[i]);
  }
  cout << "test21" << endl;
}

