/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
 *                                                                           *
 * Creation Date  : June 2016                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold NeutronWall Spectra                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// class header 
#include "TNeutronWallSpectra.h"

// STL
#include <iostream>  
#include <string>
using namespace std;

// NPTool header
#include "NPOptionManager.h"



////////////////////////////////////////////////////////////////////////////////
TNeutronWallSpectra::TNeutronWallSpectra() 
   : fNumberOfDetectors(0) {
  SetName("NeutronWall");
}



////////////////////////////////////////////////////////////////////////////////
TNeutronWallSpectra::TNeutronWallSpectra(unsigned int NumberOfDetectors) {
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TNeutronWallSpectra : Initalizing control spectra for " 
      << NumberOfDetectors << " Detectors" << endl
      << "************************************************" << endl ;
  SetName("NeutronWall");
  fNumberOfDetectors = NumberOfDetectors;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}



////////////////////////////////////////////////////////////////////////////////
TNeutronWallSpectra::~TNeutronWallSpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void TNeutronWallSpectra::InitRawSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy 
    name = "NeutronWall"+NPL::itoa(i+1)+"_ENERGY_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "NeutronWall/RAW");
    // Time 
    name = "NeutronWall"+NPL::itoa(i+1)+"_TIME_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "NeutronWall/RAW");
  } // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TNeutronWallSpectra::InitPreTreatedSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy 
    name = "NeutronWall"+NPL::itoa(i+1)+"_ENERGY_CAL";
    AddHisto1D(name, name, 500, 0, 25, "NeutronWall/CAL");
    // Time
    name = "NeutronWall"+NPL::itoa(i+1)+"_TIME_CAL";
    AddHisto1D(name, name, 500, 0, 25, "NeutronWall/CAL");

  
  }  // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TNeutronWallSpectra::InitPhysicsSpectra() {
  static string name;
  // Kinematic Plot 
  name = "NeutronWall_ENERGY_TIME";
  AddHisto2D(name, name, 500, 0, 500, 500, 0, 50, "NeutronWall/PHY");
}



////////////////////////////////////////////////////////////////////////////////
void TNeutronWallSpectra::FillRawSpectra(TNeutronWallData* RawData) {
  static string name;
  static string family;

  // Energy 
  unsigned int sizeE = RawData->GetMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "NeutronWall"+NPL::itoa(RawData->GetE_DetectorNbr(i))+"_ENERGY_RAW";
    family = "NeutronWall/RAW";

    FillSpectra(family,name ,RawData->Get_Energy(i));
  }

  // Time
  unsigned int sizeT = RawData->GetMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "NeutronWall"+NPL::itoa(RawData->GetT_DetectorNbr(i))+"_TIME_RAW";
    family = "NeutronWall/RAW";

    FillSpectra(family,name ,RawData->Get_Time(i));
  }
}



////////////////////////////////////////////////////////////////////////////////
void TNeutronWallSpectra::FillPreTreatedSpectra(TNeutronWallData* PreTreatedData) {
  static string name;
  static string family;
  
  // Energy 
  unsigned int sizeE = PreTreatedData->GetMultEnergy();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "NeutronWall"+NPL::itoa(PreTreatedData->GetE_DetectorNbr(i))+"_ENERGY_CAL";
    family = "NeutronWall/CAL";

    FillSpectra(family,name ,PreTreatedData->Get_Energy(i));
  }

  // Time
  unsigned int sizeT = PreTreatedData->GetMultTime();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "NeutronWall"+NPL::itoa(PreTreatedData->GetT_DetectorNbr(i))+"_TIME_CAL";
    family = "NeutronWall/CAL";

    FillSpectra(family,name ,PreTreatedData->Get_Time(i));
  }
}



////////////////////////////////////////////////////////////////////////////////
void TNeutronWallSpectra::FillPhysicsSpectra(TNeutronWallPhysics* Physics) {
  static string name;
  static string family;
  family= "NeutronWall/PHY";

  // Energy vs time
  unsigned int sizeE = Physics->NW_Energy.size();
  for(unsigned int i = 0 ; i < sizeE ; i++){
    name = "NeutronWall_ENERGY_TIME";
    FillSpectra(family,name ,Physics->NW_Energy[i],Physics->NW_Time[i]);
  }
}

