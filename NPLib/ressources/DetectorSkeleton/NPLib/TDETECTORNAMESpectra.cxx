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
  fStripFront=24;
  fStripBack=48;

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
    name = "DETECTORNAME"+NPL::itoa(i+1)+"_STR_FRONT_E_RAW";
    AddHisto2D(name, name, fStripFront, 1, fStripFront+1, 5000, 0, 1.5e6, "DETECTORNAME/RAW/STR_FRONT_E")->Draw("colz");
  } // end loop on number of detectors
}

////////////////////////////////////////////////////////////////////////////////
void TDETECTORNAMESpectra::InitPreTreatedSpectra(){
  static string name;
  for (unsigned int i = 0; i < fNumberOfDetector; i++) { // loop on number of detectors
    // STR_FRONT_E_CAL
    name = "DETECTORNAME"+NPL::itoa(i+1)+"_STR_FRONT_E_CAL";
    AddHisto2D(name, name, fStripFront, 1, fStripFront+1, 500, 0, 25, "DETECTORNAME/CAL/STR_FRONT_E");
  }  // end loop on number of detectors
}

////////////////////////////////////////////////////////////////////////////////
void TDETECTORNAMESpectra::InitPhysicsSpectra(){
  static string name;
  // Kinematic Plot 
  name = "DETECTORNAME_THETA_E";
  AddHisto2D(name, name,360,0,180,500,0,50,"DETECTORNAME/PHY");
}

////////////////////////////////////////////////////////////////////////////////
void TDETECTORNAMESpectra::FillRawSpectra(TDETECTORNAMEData* RawData){
  static string name;
  static string family;

  // Energy 
  unsigned int mysize = RawData->GetMultiplicityFront();
  for (unsigned int i = 0; i < mysize; i++) {
    name = "DETECTORNAME"+NPL::itoa(RawData->Get_DetectorNbr(i))+"_ENERGY_RAW";
    family = "DETECTORNAME/RAW/ENERGY";

    GetHisto(family,name)
      -> Fill(RawData->Get_DetectorNbr(i), 
          RawData->Get_Energy(i));
  }

  // Energy Multiplicity
  int myMULT[fNumberOfDetector];
  for( unsigned int i = 0; i < fNumberOfDetector; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < RawData->GetMultiplicityFront();i++){
    myMULT[RawData->Get_DetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfDetector; i++){
    name = "DETECTORNAME"+NPL::itoa(i+1)+"_RAW_MULT";
    family= "DETECTORNAME/RAW/MULT";
    GetHisto(family,name)
      -> Fill(myMULT[i]);
  }

}

////////////////////////////////////////////////////////////////////////////////
void TDETECTORNAMESpectra::FillPreTreatedSpectra(TDETECTORNAMEData* PreTreatedData){
  static string name ;
  static string family;

  // STR_FRONT_E
  unsigned int mysize = PreTreatedData->GetMultiplicityFront();
  for (unsigned int i = 0; i < mysize; i++) {
    name = "DETECTORNAME"+NPL::itoa(PreTreatedData->Get_DetectorNbr(i))+"_STR_FRONT_E_CAL";
    family = "DETECTORNAME/CAL/STR_FRONT_E";

    GetHisto(family,name)
      -> Fill(PreTreatedData->Get_StripNbr(i), 
          PreTreatedData->Get_Energy(i));
  }

  // STR_FRONT MULT
  int myMULT[fNumberOfDetector];
  for( unsigned int i = 0; i < fNumberOfDetector; i++)
    myMULT[i] = 0 ; 

  mysize = PreTreatedData->GetMultiplicityFront(); 
  for(unsigned int i = 0 ; i < mysize ;i++){
    myMULT[PreTreatedData->Get_DetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfDetector; i++){

    name = "DETECTORNAME"+NPL::itoa(i+1)+"_STR_FRONT_CAL_MULT";
    family= "DETECTORNAME/CAL/MULT";
    GetHisto(family,name)
      -> Fill(myMULT[i]);
  }

}

////////////////////////////////////////////////////////////////////////////////
void TDETECTORNAMESpectra::FillPhysicsSpectra(TDETECTORNAMEPhysics* Physics){
  static string name;
  static string family;
  family= "DETECTORNAME/PHY";

  // Kine plot
  unsigned int mysize = Physics->Strip_E.size();
  for(unsigned int i = 0 ; i < mysize ; i++){
    double Theta = Physics->GetPositionOfInteraction(i).Angle(TVector3(0,0,1));
    Theta = Theta/deg;
    double Etot=Physics->Strip_E[i];

    if(Physics->PAD_E[i]>0){
      name = "DETECTORNAME_PAD_E_E";
      Etot += Physics->PAD_E[i];
      GetHisto(family,name)->Fill(Physics->PAD_E[i],Physics->Strip_E[i]);
      name = "DETECTORNAME"+NPL::itoa(Physics->DetectorNumber[i])+"_PAD_E_E";
      GetHisto(family,name)->Fill(Physics->PAD_E[i],Physics->Strip_E[i]);

    }
    name = "DETECTORNAME_THETA_E";
    GetHisto(family,name)-> Fill(Theta,Etot);
  }
}

