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
 *  This class hold DETECTORNAME Raw data                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// NPL
#include "TDETECTORNAMESpectra.h"
#include "NPOptionManager.h"
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"
#ifdef NP_SYSTEM_OF_UNITS_H
using namespace NPUNITS;
#endif

// STL
#include <stdexcept>
#include <iostream>  
#include <cstdlib>
#include <string>
using namespace std;


////////////////////////////////////////////////////////////////////////////////
TDETECTORNAMESpectra::TDETECTORNAMESpectra(){
  SetName("DETECTORNAME");
  fNumberOfDetector = 0;
  fStripFront=24;
  fStripBack=48;
}

////////////////////////////////////////////////////////////////////////////////
TDETECTORNAMESpectra::TDETECTORNAMESpectra(unsigned int NumberOfDetector){
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TDETECTORNAMESpectra : Initalising control spectra for " 
      << NumberOfDetector << " Detectors" << endl
      << "************************************************" << endl ;
  SetName("DETECTORNAME");
  fNumberOfDetector = NumberOfDetector;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}

////////////////////////////////////////////////////////////////////////////////
TDETECTORNAMESpectra::~TDETECTORNAMESpectra(){
}

////////////////////////////////////////////////////////////////////////////////
void TDETECTORNAMESpectra::InitRawSpectra(){
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetector; i++) { // loop on number of detectors
    name = "DETECTORNAMERaw"+NPL::itoa(i+1);
    // STR_FRONT_E_RAW
    name = "DETECTORNAME"+NPL::itoa(i+1)+"_ENERGY_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "DETECTORNAME/RAW")->Draw("colz");
  } // end loop on number of detectors
}

////////////////////////////////////////////////////////////////////////////////
void TDETECTORNAMESpectra::InitPreTreatedSpectra(){
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetector; i++) { // loop on number of detectors
    // STR_FRONT_E_CAL
    name = "DETECTORNAME"+NPL::itoa(i+1)+"_ENERGY_CAL";
    AddHisto1D(name, name, 500, 0, 25, "DETECTORNAME/CAL");
  }  // end loop on number of detectors
}

////////////////////////////////////////////////////////////////////////////////
void TDETECTORNAMESpectra::InitPhysicsSpectra(){
  static string name;
  // Kinematic Plot 
  name = "DETECTORNAME_ENERGY_TIME";
  AddHisto2D(name, name,500,-500,0,500,0,50,"DETECTORNAME/PHY");
}

////////////////////////////////////////////////////////////////////////////////
void TDETECTORNAMESpectra::FillRawSpectra(TDETECTORNAMEData* RawData){
  static string name;
  static string family;

  // Energy 
  unsigned int sizeE = RawData->GetMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "DETECTORNAME"+NPL::itoa(RawData->GetE_DetectorNbr(i))+"_ENERGY_RAW";
    family = "DETECTORNAME/RAW";

    GetHisto(family,name)
      -> Fill(RawData->GetE_DetectorNbr(i), 
          RawData->Get_Energy(i));
  }

  // Time
  unsigned int sizeT = RawData->GetMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "DETECTORNAME"+NPL::itoa(RawData->GetT_DetectorNbr(i))+"_TIME_RAW";
    family = "DETECTORNAME/RAW";

    GetHisto(family,name)
      -> Fill(RawData->GetT_DetectorNbr(i), 
          RawData->Get_Time(i));
  }
}

////////////////////////////////////////////////////////////////////////////////
void TDETECTORNAMESpectra::FillPreTreatedSpectra(TDETECTORNAMEData* PreTreatedData){
  static string name;
  static string family;
  
  // Energy 
  unsigned int sizeE = PreTreatedData->GetMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "DETECTORNAME"+NPL::itoa(PreTreatedData->GetE_DetectorNbr(i))+"_ENERGY_RAW";
    family = "DETECTORNAME/CAL";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetE_DetectorNbr(i), 
          PreTreatedData->Get_Energy(i));
  }

  // Time
  unsigned int sizeT = PreTreatedData->GetMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "DETECTORNAME"+NPL::itoa(PreTreatedData->GetT_DetectorNbr(i))+"_TIME_RAW";
    family = "DETECTORNAME/CAL";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetT_DetectorNbr(i), 
          PreTreatedData->Get_Time(i));
  }
}

////////////////////////////////////////////////////////////////////////////////
void TDETECTORNAMESpectra::FillPhysicsSpectra(TDETECTORNAMEPhysics* Physics){
  static string name;
  static string family;
  family= "DETECTORNAME/PHY";

  // Energy vs time
  unsigned int sizeE = Physics->Energy.size();
  for(unsigned int i = 0 ; i < sizeE ; i++){
    name = "DETECTORNAME_ENERGY_TIME";
    GetHisto(family,name)-> Fill(Physics->Energy[i],Physics->Time[i]);
  }
}

