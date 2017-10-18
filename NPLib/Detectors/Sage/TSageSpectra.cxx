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
 *  This class hold Sage Spectra                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// class header 
#include "TSageSpectra.h"

// STL
#include <iostream>  
#include <string>
using namespace std;

// NPTool header
#include "NPOptionManager.h"



////////////////////////////////////////////////////////////////////////////////
TSageSpectra::TSageSpectra() 
   : fNumberOfDetectors(0) {
  SetName("Sage");
}



////////////////////////////////////////////////////////////////////////////////
TSageSpectra::TSageSpectra(unsigned int NumberOfDetectors) {
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TSageSpectra : Initalizing control spectra for " 
      << NumberOfDetectors << " Detectors" << endl
      << "************************************************" << endl ;
  SetName("Sage");
  fNumberOfDetectors = NumberOfDetectors;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}



////////////////////////////////////////////////////////////////////////////////
TSageSpectra::~TSageSpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void TSageSpectra::InitRawSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
	  for (unsigned int j = 0; j < fNumberOfPixels; j++) { // loop on number of pixels
		  // Energy 
		  name = "Sage"+NPL::itoa(i+1)+"_ENERGY_RAW";
		  AddHisto2D(name, name, fNumberOfPixels, 1, fNumberOfPixels, 4096, 0, 16384,"Sage/RAW");
		  // Time 
		  name = "Sage"+NPL::itoa(i+1)+"_TIME_RAW";
		  AddHisto2D(name, name, fNumberOfPixels, 1, fNumberOfPixels, 4096, 0, 16384,"Sage/RAW");
	  } // end loop on number of detectors
  }
}



////////////////////////////////////////////////////////////////////////////////
void TSageSpectra::InitPreTreatedSpectra() {
	static string name;
	for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
		for (unsigned int j = 0; j < fNumberOfPixels; j++) { // loop on number of pixels
			// Energy 
			name = "Sage"+NPL::itoa(i+1)+"_ENERGY_CAL";
			AddHisto2D(name, name, fNumberOfPixels, 1, fNumberOfPixels, 4096, 0, 25,"Sage/CAL");
			// Time
			name = "Sage"+NPL::itoa(i+1)+"_TIME_CAL";
			AddHisto2D(name, name, fNumberOfPixels, 1, fNumberOfPixels, 4096, 0, 25,"Sage/CAL");

		}
	}  // end loop on number of detectors
}

////////////////////////////////////////////////////////////////////////////////
void TSageSpectra::InitPhysicsSpectra() {
  static string name;
  // Kinematic Plot 
  name = "Sage_ENERGY_TIME";
  AddHisto2D(name, name, 500, 0, 500, 500, 0, 50, "Sage/PHY");
}



////////////////////////////////////////////////////////////////////////////////
void TSageSpectra::FillRawSpectra(TSageData* RawData) {
  static string name;
  static string family;

  // Energy 
  unsigned int sizeE = RawData->GetMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "Sage"+NPL::itoa(RawData->GetE_DetectorNbr(i))+"_ENERGY_RAW";
    family = "Sage/RAW";

    FillSpectra(family,name, RawData->GetE_PixelNbr(i), RawData->Get_Energy(i));
  }

  // Time
  unsigned int sizeT = RawData->GetMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "Sage"+NPL::itoa(RawData->GetT_DetectorNbr(i))+"_TIME_RAW";
    family = "Sage/RAW";

    FillSpectra(family,name, RawData->GetT_PixelNbr(i), RawData->Get_Time(i));
  }
}

////////////////////////////////////////////////////////////////////////////////
void TSageSpectra::FillPreTreatedSpectra(TSageData* PreTreatedData) {
  static string name;
  static string family;
  
  // Energy 
  unsigned int sizeE = PreTreatedData->GetMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "Sage"+NPL::itoa(PreTreatedData->GetE_DetectorNbr(i))+"_ENERGY_CAL";
    family = "Sage/CAL";

    FillSpectra(family,name,PreTreatedData->GetE_PixelNbr(i), PreTreatedData->Get_Energy(i));
  }

  // Time
  unsigned int sizeT = PreTreatedData->GetMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "Sage"+NPL::itoa(PreTreatedData->GetT_DetectorNbr(i))+"_TIME_CAL";
    family = "Sage/CAL";

    FillSpectra(family,name,PreTreatedData->GetT_PixelNbr(i), PreTreatedData->Get_Time(i));
  }
}



////////////////////////////////////////////////////////////////////////////////
void TSageSpectra::FillPhysicsSpectra(TSagePhysics* Physics) {
  static string name;
  static string family;
  family= "Sage/PHY";

  // Energy vs time
  unsigned int sizeE = Physics->Energy.size();
  for(unsigned int i = 0 ; i < sizeE ; i++){
    name = "Sage_ENERGY_TIME";
    FillSpectra(family,name, Physics->Energy[i],Physics->Time[i]);
  }
}

