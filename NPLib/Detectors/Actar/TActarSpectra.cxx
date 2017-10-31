/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
 *                                                                           *
 * Creation Date  : September 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Actar Spectra                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// class header
#include "TActarSpectra.h"

// STL
#include <iostream>
#include <string>
using namespace std;

// NPTool header
#include "NPOptionManager.h"



////////////////////////////////////////////////////////////////////////////////
TActarSpectra::TActarSpectra()
   : fNumberOfDetectors(0) {
  SetName("Actar");
}



////////////////////////////////////////////////////////////////////////////////
TActarSpectra::TActarSpectra(unsigned int NumberOfDetectors) {
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TActarSpectra : Initalizing control spectra for "
      << NumberOfDetectors << " Detectors" << endl
      << "************************************************" << endl ;
  SetName("Actar");
  fNumberOfDetectors = NumberOfDetectors;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}



////////////////////////////////////////////////////////////////////////////////
TActarSpectra::~TActarSpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void TActarSpectra::InitRawSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Charge
    name = "Actar"+NPL::itoa(i+1)+"_CHARGE_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "Actar/RAW");
    // Time
    name = "Actar"+NPL::itoa(i+1)+"_TIME_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "Actar/RAW");
  } // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TActarSpectra::InitPreTreatedSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy
    name = "Actar"+NPL::itoa(i+1)+"_CHARGE_CAL";
    AddHisto1D(name, name, 500, 0, 25, "Actar/CAL");
    // Time
    name = "Actar"+NPL::itoa(i+1)+"_TIME_CAL";
    AddHisto1D(name, name, 500, 0, 25, "Actar/CAL");


  }  // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TActarSpectra::InitPhysicsSpectra() {
  static string name;
  // Kinematic Plot
  name = "Actar_CHARGE_TIME";
  AddHisto2D(name, name, 500, 0, 500, 500, 0, 50, "Actar/PHY");
}



////////////////////////////////////////////////////////////////////////////////
void TActarSpectra::FillRawSpectra(TActarData* RawData) {
  static string name;
  static string family;

  // Charge
  unsigned int sizeE = RawData->GetMultCharge();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "Actar"+NPL::itoa(RawData->Get_PadNumber(i))+"_CHARGE_RAW";
    family = "Actar/RAW";

    //GetHisto(family,name) -> Fill(RawData->Get_Charge(i));
  }

  // Time
  unsigned int sizeT = RawData->GetMultCharge();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "Actar"+NPL::itoa(RawData->Get_PadNumber(i))+"_TIME_RAW";
    family = "Actar/RAW";

    //GetHisto(family,name) -> Fill(RawData->Get_Time(i));
  }
}



////////////////////////////////////////////////////////////////////////////////
void TActarSpectra::FillPreTreatedSpectra(TActarData* PreTreatedData) {
  static string name;
  static string family;

  // Energy
  unsigned int sizeE = PreTreatedData->GetMultCharge();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "Actar"+NPL::itoa(PreTreatedData->Get_PadNumber(i))+"_ENERGY_CAL";
    family = "Actar/CAL";

    //GetHisto(family,name) -> Fill(PreTreatedData->Get_Charge(i));
  }

  // Time
  unsigned int sizeT = PreTreatedData->GetMultCharge();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "Actar"+NPL::itoa(PreTreatedData->Get_PadNumber(i))+"_TIME_CAL";
    family = "Actar/CAL";

    //GetHisto(family,name) -> Fill(PreTreatedData->Get_Time(i));
  }
}



////////////////////////////////////////////////////////////////////////////////
void TActarSpectra::FillPhysicsSpectra(TActarPhysics* Physics) {
  static string name;
  static string family;
  family= "Actar/PHY";

  // Energy vs time
  unsigned int sizeE = Physics->PadCharge.size();
  for(unsigned int i = 0 ; i < sizeE ; i++){
    name = "Actar_ENERGY_TIME";
    //GetHisto(family,name) -> Fill(Physics->Charge[i],Physics->Time[i]);
  }
}
