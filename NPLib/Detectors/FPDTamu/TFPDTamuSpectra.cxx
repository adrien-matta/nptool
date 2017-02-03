/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Mohamad Moukaddam                                        *
 * contact address: m.moukaddam@surrey.ac.uk                                 *
 *                                                                           *
 * Creation Date  : November 2016                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold FPDTamu Spectra                                          *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// class header 
#include "TFPDTamuSpectra.h"

// STL
#include <iostream>  
#include <string>
using namespace std;

// NPTool header
#include "NPOptionManager.h"



////////////////////////////////////////////////////////////////////////////////
TFPDTamuSpectra::TFPDTamuSpectra() 
   : fNumberOfDetectors(0) {
  SetName("FPDTamu");
}



////////////////////////////////////////////////////////////////////////////////
TFPDTamuSpectra::TFPDTamuSpectra(unsigned int NumberOfDetectors) {
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TFPDTamuSpectra : Initalizing control spectra for " 
      << NumberOfDetectors << " Detectors" << endl
      << "************************************************" << endl ;
  SetName("FPDTamu");
  fNumberOfDetectors = NumberOfDetectors;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}



////////////////////////////////////////////////////////////////////////////////
TFPDTamuSpectra::~TFPDTamuSpectra() {
}



////////////////////////////////////////////////////////////////////////////////
void TFPDTamuSpectra::InitRawSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy 
    name = "FPDTamu"+NPL::itoa(i+1)+"_ENERGY_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "FPDTamu/RAW");
    // Time 
    name = "FPDTamu"+NPL::itoa(i+1)+"_TIME_RAW";
    AddHisto1D(name, name, 4096, 0, 16384, "FPDTamu/RAW");
  } // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TFPDTamuSpectra::InitPreTreatedSpectra() {
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetectors; i++) { // loop on number of detectors
    // Energy 
    name = "FPDTamu"+NPL::itoa(i+1)+"_ENERGY_CAL";
    AddHisto1D(name, name, 500, 0, 25, "FPDTamu/CAL");
    // Time
    name = "FPDTamu"+NPL::itoa(i+1)+"_TIME_CAL";
    AddHisto1D(name, name, 500, 0, 25, "FPDTamu/CAL");

  
  }  // end loop on number of detectors
}



////////////////////////////////////////////////////////////////////////////////
void TFPDTamuSpectra::InitPhysicsSpectra() {
  static string name;
  // Kinematic Plot 
  name = "FPDTamu_ENERGY_TIME";
  AddHisto2D(name, name, 500, 0, 500, 500, 0, 50, "FPDTamu/PHY");
}



////////////////////////////////////////////////////////////////////////////////
void TFPDTamuSpectra::FillRawSpectra(TFPDTamuData* RawData) {
  static string name;
  static int side; // Left 0 or Right 1
  static string family;

  //DeltaE ionisation chamber
  // Energy 
  unsigned int sizeE = RawData->Get_Delta_Energy_Mult();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "FPDTamu"+NPL::itoa(RawData->Get_Delta_E_DetectorNbr(i))+"_ENERGY_RAW";
    family = "FPDTamu/Delta/RAW";

    GetHisto(family,name) -> Fill(RawData->Get_Delta_Energy(i));
  }
  // Time
  unsigned int sizeT = RawData->Get_Delta_Time_Mult();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "FPDTamu"+NPL::itoa(RawData->Get_Delta_T_DetectorNbr(i))+"_TIME_RAW";
    family = "FPDTamu/Delta/RAW";

    GetHisto(family,name) -> Fill(RawData->Get_Delta_Time(i));
  }

  // Avalanche Resistive Wire
  // Energy 
  sizeE = RawData->Get_AWire_Energy_Mult();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "FPDTamu"+NPL::itoa(RawData->Get_AWire_E_DetectorNbr(i))+"_ENERGY_RAW";
    family = "FPDTamu/AWire/RAW";
    side = RawData->Get_AWire_E_DetectorSide(i);
	if (side==0)
    	GetHisto(family,name+"_Left") -> Fill(RawData->Get_AWire_Energy(i));
	else
    	GetHisto(family,name+"_Right") -> Fill(RawData->Get_AWire_Energy(i));
  }
  // Time
  sizeT = RawData->Get_AWire_Time_Mult();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "FPDTamu"+NPL::itoa(RawData->Get_AWire_T_DetectorNbr(i))+"_TIME_RAW";
    family = "FPDTamu/AWire/RAW";
    side = RawData->Get_AWire_T_DetectorSide(i);
	if (side==0)
    	GetHisto(family,name+"_Left") -> Fill(RawData->Get_AWire_Time(i));
	else
    	GetHisto(family,name+"_Right") -> Fill(RawData->Get_AWire_Time(i));
  }
    
  // MicroMega Plate
  // Energy 
  sizeE = RawData->Get_Micro_Energy_Mult();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "FPDTamu"+NPL::itoa(RawData->Get_Micro_E_RowNbr(i))+"_"
    				+NPL::itoa(RawData->Get_Micro_E_ColNbr(i))+"_ENERGY_RAW";
    family = "FPDTamu/Micro/RAW";

    GetHisto(family,name) -> Fill(RawData->Get_Micro_Energy(i));
  }
  // Time
  sizeT = RawData->Get_Micro_Time_Mult();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "FPDTamu"+NPL::itoa(RawData->Get_Micro_T_RowNbr(i))+"_"
    				+NPL::itoa(RawData->Get_Micro_T_ColNbr(i))+"_TIME_RAW";
    family = "FPDTamu/Micro/RAW";

    GetHisto(family,name) -> Fill(RawData->Get_Micro_Time(i));
  }

  // Plastic Scintillator
  // Energy 
  sizeE = RawData->Get_Plast_Energy_Mult();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "FPDTamu_ENERGY_RAW";
    family = "FPDTamu/Plast/RAW";
    side = RawData->Get_Plast_E_DetectorSide(i);
	if (side==0)
    	GetHisto(family,name+"_Left") -> Fill(RawData->Get_Plast_Energy(i));
	else
    	GetHisto(family,name+"_Right") -> Fill(RawData->Get_Plast_Energy(i));
  }
  // Time
  sizeT = RawData->Get_Plast_Time_Mult();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "FPDTamu_TIME_RAW";
    family = "FPDTamu/Plast/RAW";
    side = RawData->Get_Plast_T_DetectorSide(i);
	if (side==0)
    	GetHisto(family,name+"_Left") -> Fill(RawData->Get_Plast_Time(i));
	else
    	GetHisto(family,name+"_Right") -> Fill(RawData->Get_Plast_Time(i));
  }
  

}



////////////////////////////////////////////////////////////////////////////////
void TFPDTamuSpectra::FillPreTreatedSpectra(TFPDTamuData* PreTreatedData) {
  static string name;
  static string family;
  static int side; // Left 0 or Right 1

  //DeltaE ionisation chamber
  // Energy 
  unsigned int sizeE = PreTreatedData->Get_Delta_Energy_Mult();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "FPDTamu"+NPL::itoa(PreTreatedData->Get_Delta_E_DetectorNbr(i))+"_ENERGY_CAL";
    family = "FPDTamu/Delta/CAL";

    GetHisto(family,name) -> Fill(PreTreatedData->Get_Delta_Energy(i));
  }
  // Time
  unsigned int sizeT = PreTreatedData->Get_Delta_Time_Mult();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "FPDTamu"+NPL::itoa(PreTreatedData->Get_Delta_T_DetectorNbr(i))+"_TIME_CAL";
    family = "FPDTamu/Delta/CAL";

    GetHisto(family,name) -> Fill(PreTreatedData->Get_Delta_Time(i));
  }

  // Avalanche Resistive Wire
  // Energy 
  sizeE = PreTreatedData->Get_AWire_Energy_Mult();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "FPDTamu"+NPL::itoa(PreTreatedData->Get_AWire_E_DetectorNbr(i))+"_ENERGY_CAL";
	family = "FPDTamu/AWire/CAL";
	side = PreTreatedData->Get_AWire_E_DetectorSide(i);
	if (side==0)
    	GetHisto(family,name+"_Left") -> Fill(PreTreatedData->Get_AWire_Energy(i));
	else    
		GetHisto(family,name+"_Right") -> Fill(PreTreatedData->Get_AWire_Energy(i));
  }
  // Time
  sizeT = PreTreatedData->Get_AWire_Time_Mult();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "FPDTamu"+NPL::itoa(PreTreatedData->Get_AWire_T_DetectorNbr(i))+"_TIME_CAL";
    family = "FPDTamu/AWire/CAL";
	side = PreTreatedData->Get_AWire_T_DetectorSide(i);
	if (side==0)
    	GetHisto(family,name+"_Left") -> Fill(PreTreatedData->Get_AWire_Time(i));
	else    
		GetHisto(family,name+"_Right") -> Fill(PreTreatedData->Get_AWire_Time(i));
  }
    
  // MicroMega Plate
  // Energy 
  sizeE = PreTreatedData->Get_Micro_Energy_Mult();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "FPDTamu"+NPL::itoa(PreTreatedData->Get_Micro_E_RowNbr(i))+"_"
    				+NPL::itoa(PreTreatedData->Get_Micro_E_ColNbr(i))+"_ENERGY_CAL";
    family = "FPDTamu/Micro/CAL";

    GetHisto(family,name) -> Fill(PreTreatedData->Get_Micro_Energy(i));
  }
  // Time
  sizeT = PreTreatedData->Get_Micro_Time_Mult();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "FPDTamu"+NPL::itoa(PreTreatedData->Get_Micro_T_RowNbr(i))+"_"
    				+NPL::itoa(PreTreatedData->Get_Micro_T_ColNbr(i))+"_TIME_CAL";
    family = "FPDTamu/Micro/CAL";

    GetHisto(family,name) -> Fill(PreTreatedData->Get_Micro_Time(i));
  }

  // Plastic Scintillator
  // Energy 
  sizeE = PreTreatedData->Get_Plast_Energy_Mult();
  for (unsigned int i = 0; i < sizeE; i++) {
    name = "FPDTamu_ENERGY_CAL";
    family = "FPDTamu/Plast/CAL";
	side = PreTreatedData->Get_Plast_E_DetectorSide(i);
	if (side==0)
    	GetHisto(family,name+"_Left") -> Fill(PreTreatedData->Get_Plast_Energy(i));
	else 
    	GetHisto(family,name+"_Right") -> Fill(PreTreatedData->Get_Plast_Energy(i));
  }
  // Time
  sizeT = PreTreatedData->Get_Plast_Time_Mult();
  for (unsigned int i = 0; i < sizeT; i++) {
    name = "FPDTamu_TIME_CAL";
    family = "FPDTamu/Plast/CAL";
	side = PreTreatedData->Get_Plast_T_DetectorSide(i);
	if (side==0)
    	GetHisto(family,name+"_Left") -> Fill(PreTreatedData->Get_Plast_Time(i));
	else
    	GetHisto(family,name+"_Right") -> Fill(PreTreatedData->Get_Plast_Time(i));
  }
  
}



////////////////////////////////////////////////////////////////////////////////
void TFPDTamuSpectra::FillPhysicsSpectra(TFPDTamuPhysics* Physics) {
  static string name;
  static string family;
  family= "FPDTamu/PHY";

  // Energy vs time
  unsigned int sizeE = Physics->DeltaEnergy.size();
  for(unsigned int i = 0 ; i < sizeE ; i++){
    name = "FPDTamu_DELTA_ENERGY_TIME";
    GetHisto(family,name) -> Fill(Physics->DeltaEnergy[i],Physics->DeltaTime[i]);
  }
}

