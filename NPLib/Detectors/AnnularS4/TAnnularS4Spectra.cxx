/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
 *                                                                           *
 * Creation Date  : January 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold AnnularS4 Spectra                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// class header 
#include "TAnnularS4Spectra.h"

// STL
#include <iostream>  
#include <string>
using namespace std;

// NPTool header
#include "NPOptionManager.h"



////////////////////////////////////////////////////////////////////////////////
TAnnularS4Spectra::TAnnularS4Spectra() 
   : fNumberOfDetectors(0) {
  SetName("AnnularS4");
}



////////////////////////////////////////////////////////////////////////////////
TAnnularS4Spectra::TAnnularS4Spectra(unsigned int NumberOfDetectors) {
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TAnnularS4Spectra : Initalizing control spectra for " 
      << NumberOfDetectors << " Detectors" << endl
      << "************************************************" << endl ;
  SetName("AnnularS4");
  fNumberOfDetectors = NumberOfDetectors;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}



////////////////////////////////////////////////////////////////////////////////
TAnnularS4Spectra::~TAnnularS4Spectra() {
}



////////////////////////////////////////////////////////////////////////////////
void TAnnularS4Spectra::InitRawSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy 
    name = "AnnularS4"+NPL::itoa(i+1)+"_ENERGY_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "AnnularS4/RAW");
    // Time 
    name = "AnnularS4"+NPL::itoa(i+1)+"_TIME_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "AnnularS4/RAW");
  } // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TAnnularS4Spectra::InitPreTreatedSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy 
    name = "AnnularS4"+NPL::itoa(i+1)+"_ENERGY_CAL";
    AddHisto1D(name, name, 500, 0, 25, "AnnularS4/CAL");
    // Time
    name = "AnnularS4"+NPL::itoa(i+1)+"_TIME_CAL";
    AddHisto1D(name, name, 500, 0, 25, "AnnularS4/CAL");

  
  }  // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TAnnularS4Spectra::InitPhysicsSpectra() {
  static string name;
  // Kinematic Plot 
  name = "AnnularS4_ENERGY_TIME";
  AddHisto2D(name, name, 500, 0, 500, 500, 0, 50, "AnnularS4/PHY");
}



////////////////////////////////////////////////////////////////////////////////
void TAnnularS4Spectra::FillRawSpectra(TAnnularS4Data* RawData) {
  /*static string name;
  static string family;

  // Energy 
  unsigned int sizeE = RawData->GetMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "AnnularS4"+NPL::itoa(RawData->GetE_DetectorNbr(i))+"_ENERGY_RAW";
    family = "AnnularS4/RAW";

    GetHisto(family,name) -> Fill(RawData->Get_Energy(i));
  }

  // Time
  unsigned int sizeT = RawData->GetMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "AnnularS4"+NPL::itoa(RawData->GetT_DetectorNbr(i))+"_TIME_RAW";
    family = "AnnularS4/RAW";

    GetHisto(family,name) -> Fill(RawData->Get_Time(i));
  }*/
}



////////////////////////////////////////////////////////////////////////////////
void TAnnularS4Spectra::FillPreTreatedSpectra(TAnnularS4Data* PreTreatedData) {
  /*static string name;
  static string family;
  
  // Energy 
  unsigned int sizeE = PreTreatedData->GetMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "AnnularS4"+NPL::itoa(PreTreatedData->GetE_DetectorNbr(i))+"_ENERGY_CAL";
    family = "AnnularS4/CAL";

    GetHisto(family,name) -> Fill(PreTreatedData->Get_Energy(i));
  }

  // Time
  unsigned int sizeT = PreTreatedData->GetMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "AnnularS4"+NPL::itoa(PreTreatedData->GetT_DetectorNbr(i))+"_TIME_CAL";
    family = "AnnularS4/CAL";

    GetHisto(family,name) -> Fill(PreTreatedData->Get_Time(i));
  }*/
}



////////////////////////////////////////////////////////////////////////////////
void TAnnularS4Spectra::FillPhysicsSpectra(TAnnularS4Physics* Physics) {
  /*static string name;
  static string family;
  family= "AnnularS4/PHY";

  // Energy vs time
  unsigned int sizeE = Physics->Energy.size();
  for(unsigned int i = 0 ; i < sizeE ; i++){
    name = "AnnularS4_ENERGY_TIME";
    GetHisto(family,name) -> Fill(Physics->Energy[i],Physics->Time[i]);
  }*/
}

