/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Daniel Cox  contact address: daniel.m.cox@jyu.fi                        *
 *                                                                           *
 * Creation Date  : January 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Spede Spectra                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// class header 
#include "TSpedeSpectra.h"

// STL
#include <iostream>  
#include <string>
using namespace std;

// NPTool header
#include "NPOptionManager.h"



////////////////////////////////////////////////////////////////////////////////
TSpedeSpectra::TSpedeSpectra() 
   : fNumberOfDetectors(0) {
  SetName("Spede");
}



////////////////////////////////////////////////////////////////////////////////
TSpedeSpectra::TSpedeSpectra(unsigned int NumberOfDetectors) {
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TSpedeSpectra : Initalizing control spectra for " 
      << NumberOfDetectors << " Detectors" << endl
      << "************************************************" << endl ;
  SetName("Spede");
  fNumberOfDetectors = NumberOfDetectors;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}



////////////////////////////////////////////////////////////////////////////////
TSpedeSpectra::~TSpedeSpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void TSpedeSpectra::InitRawSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
	  for (unsigned int j = 0; j < fNumberOfPixels; j++) { // loop on number of pixels
		  // Energy 
		  name = "Spede"+NPL::itoa(i+1)+"_ENERGY_RAW";
		  AddHisto2D(name, name, fNumberOfPixels, 1, fNumberOfPixels, 4096, 0, 16384,"Spede/RAW");
		  // Time 
		  name = "Spede"+NPL::itoa(i+1)+"_TIME_RAW";
		  AddHisto2D(name, name, fNumberOfPixels, 1, fNumberOfPixels, 4096, 0, 16384,"Spede/RAW");
	  } // end loop on number of detectors
  }
}



////////////////////////////////////////////////////////////////////////////////
void TSpedeSpectra::InitPreTreatedSpectra() {
	static string name;
	for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
		for (unsigned int j = 0; j < fNumberOfPixels; j++) { // loop on number of pixels
			// Energy 
			name = "Spede"+NPL::itoa(i+1)+"_ENERGY_CAL";
			AddHisto2D(name, name, fNumberOfPixels, 1, fNumberOfPixels, 4096, 0, 25,"Spede/CAL");
			// Time
			name = "Spede"+NPL::itoa(i+1)+"_TIME_CAL";
			AddHisto2D(name, name, fNumberOfPixels, 1, fNumberOfPixels, 4096, 0, 25,"Spede/CAL");

		}
	}  // end loop on number of detectors
}

////////////////////////////////////////////////////////////////////////////////
void TSpedeSpectra::InitPhysicsSpectra() {
  static string name;
  // Kinematic Plot 
  name = "Spede_ENERGY_TIME";
  AddHisto2D(name, name, 500, 0, 500, 500, 0, 50, "Spede/PHY");
}



////////////////////////////////////////////////////////////////////////////////
void TSpedeSpectra::FillRawSpectra(TSpedeData* RawData) {
  static string name;
  static string family;

  // Energy 
  unsigned int sizeE = RawData->GetMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "Spede"+NPL::itoa(RawData->GetE_DetectorNbr(i))+"_ENERGY_RAW";
    family = "Spede/RAW";

    FillSpectra(family,name,RawData->GetE_PixelNbr(i), RawData->Get_Energy(i));
  }

  // Time
  unsigned int sizeT = RawData->GetMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "Spede"+NPL::itoa(RawData->GetT_DetectorNbr(i))+"_TIME_RAW";
    family = "Spede/RAW";

    FillSpectra(family,name,RawData->GetT_PixelNbr(i), RawData->Get_Time(i));
  }
}

////////////////////////////////////////////////////////////////////////////////
void TSpedeSpectra::FillPreTreatedSpectra(TSpedeData* PreTreatedData) {
  static string name;
  static string family;
  
  // Energy 
  unsigned int sizeE = PreTreatedData->GetMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "Spede"+NPL::itoa(PreTreatedData->GetE_DetectorNbr(i))+"_ENERGY_CAL";
    family = "Spede/CAL";

    FillSpectra(family,name,PreTreatedData->GetE_PixelNbr(i), PreTreatedData->Get_Energy(i));
  }

  // Time
  unsigned int sizeT = PreTreatedData->GetMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "Spede"+NPL::itoa(PreTreatedData->GetT_DetectorNbr(i))+"_TIME_CAL";
    family = "Spede/CAL";

    FillSpectra(family,name,PreTreatedData->GetT_PixelNbr(i), PreTreatedData->Get_Time(i));
  }
}



////////////////////////////////////////////////////////////////////////////////
void TSpedeSpectra::FillPhysicsSpectra(TSpedePhysics* Physics) {
  static string name;
  static string family;
  family= "Spede/PHY";

  // Energy vs time
  unsigned int sizeE = Physics->Energy.size();
  for(unsigned int i = 0 ; i < sizeE ; i++){
    name = "Spede_ENERGY_TIME";
    FillSpectra(family,name,Physics->Energy[i],Physics->Time[i]);
  }
}

