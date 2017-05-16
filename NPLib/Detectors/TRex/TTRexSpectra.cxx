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
 *  This class hold TRex Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// class header 
#include "TTRexSpectra.h"

// STL
#include <iostream>  
#include <string>
using namespace std;

// NPTool header
#include "NPOptionManager.h"



////////////////////////////////////////////////////////////////////////////////
TTRexSpectra::TTRexSpectra() 
   : fNumberOfDetectors(0) {
  SetName("TRex");
}



////////////////////////////////////////////////////////////////////////////////
TTRexSpectra::TTRexSpectra(unsigned int NumberOfDetectors) {
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TTRexSpectra : Initalizing control spectra for " 
      << NumberOfDetectors << " Detectors" << endl
      << "************************************************" << endl ;
  SetName("TRex");
  fNumberOfDetectors = NumberOfDetectors;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}



////////////////////////////////////////////////////////////////////////////////
TTRexSpectra::~TTRexSpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void TTRexSpectra::InitRawSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy 
    name = "TRex"+NPL::itoa(i+1)+"_ENERGY_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "TRex/RAW");
    // Time 
    name = "TRex"+NPL::itoa(i+1)+"_TIME_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "TRex/RAW");
  } // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TTRexSpectra::InitPreTreatedSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy 
    name = "TRex"+NPL::itoa(i+1)+"_ENERGY_CAL";
    AddHisto1D(name, name, 500, 0, 25, "TRex/CAL");
    // Time
    name = "TRex"+NPL::itoa(i+1)+"_TIME_CAL";
    AddHisto1D(name, name, 500, 0, 25, "TRex/CAL");

  
  }  // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TTRexSpectra::InitPhysicsSpectra() {
  static string name;
  // Kinematic Plot 
  name = "TRex_ENERGY_TIME";
  AddHisto2D(name, name, 500, 0, 500, 500, 0, 50, "TRex/PHY");
}



////////////////////////////////////////////////////////////////////////////////
void TTRexSpectra::FillRawSpectra(TTRexData* RawData) {
  static string name;
  static string family;

  // Energy 
  unsigned int sizeE = RawData->GetFrontMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "TRex"+NPL::itoa(RawData->GetFrontE_DetectorNbr(i))+"_ENERGY_RAW";
    family = "TRex/RAW";

    FillSpectra(family,name,RawData->GetFront_Energy(i));
  }

  // Time
  unsigned int sizeT = RawData->GetFrontMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "TRex"+NPL::itoa(RawData->GetFrontT_DetectorNbr(i))+"_TIME_RAW";
    family = "TRex/RAW";

    FillSpectra(family,name,RawData->GetFront_Time(i));
  }
}



////////////////////////////////////////////////////////////////////////////////
void TTRexSpectra::FillPreTreatedSpectra(TTRexData* PreTreatedData) {
  static string name;
  static string family;
  
  // Energy 
  unsigned int sizeE = PreTreatedData->GetFrontMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "TRex"+NPL::itoa(PreTreatedData->GetFrontE_DetectorNbr(i))+"_ENERGY_CAL";
    family = "TRex/CAL";

    FillSpectra(family,name,PreTreatedData->GetFront_Energy(i));
  }

  // Time
  unsigned int sizeT = PreTreatedData->GetFrontMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "TRex"+NPL::itoa(PreTreatedData->GetFrontT_DetectorNbr(i))+"_TIME_CAL";
    family = "TRex/CAL";

    FillSpectra(family,name,PreTreatedData->GetFront_Time(i));
  }
}



////////////////////////////////////////////////////////////////////////////////
void TTRexSpectra::FillPhysicsSpectra(TTRexPhysics* Physics) {
  static string name;
  static string family;
  family= "TRex/PHY";

  // Energy vs time
  unsigned int sizeE = Physics->Energy.size();
  for(unsigned int i = 0 ; i < sizeE ; i++){
    name = "TRex_ENERGY_TIME";
    FillSpectra(family,name,Physics->Energy[i],Physics->Time[i]);
  }
}

