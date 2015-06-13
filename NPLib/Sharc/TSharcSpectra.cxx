/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 ****************************************************************************/

/*****************************************************************************
 * Original Author: A. Matta                 address: a.matta@surrey.ac.uk   *
 *                                                                           *
 * Creation Date  : June 2015                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds all the online spectra needed for Sharc                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// NPL
#include "TSharcSpectra.h"
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
TSharcSpectra::TSharcSpectra(){
  SetName("Sharc");
  fNumberOfDetector = 0;
  fStripFront=24;
  fStripBack=48;
}

////////////////////////////////////////////////////////////////////////////////
TSharcSpectra::TSharcSpectra(unsigned int NumberOfDetector){
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TSharcSpectra : Initalising control spectra for " 
      << NumberOfDetector << " Detectors" << endl
      << "************************************************" << endl ;
  SetName("Sharc");
  fNumberOfDetector = NumberOfDetector;
  fStripFront=24;
  fStripBack=48;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}

////////////////////////////////////////////////////////////////////////////////
TSharcSpectra::~TSharcSpectra(){
}

////////////////////////////////////////////////////////////////////////////////
void TSharcSpectra::InitRawSpectra(){
  TCanvas* c1 = new TCanvas("SharcRaw","Sharc Raw");
  TCanvas* c2 = new TCanvas("SharcRawPAD","Sharc Raw PAD");

  c1->Divide(fNumberOfDetector,4);
  c2->Divide(fNumberOfDetector,2);
  string name;
  for (unsigned int i = 0; i < fNumberOfDetector; i++) { // loop on number of detectors
    // STR_FRONT_E_RAW
    c1->cd(i+2);
    name = "SHARC"+NPL::itoa(i+1)+"_STR_FRONT_E_RAW";
    AddHisto2D(name, name, fStripFront, 1, fStripFront+1, 512, 0, 8192, "SHARC/RAW/STR_FRONT_E")->Draw("colz");

    // STR_BACK_E_RAW
    c1->cd(i+3);
    name = "SHARC"+NPL::itoa(i+1)+"_STR_BACK_E_RAW";
    AddHisto2D(name, name, fStripBack, 1, fStripBack+1, 512, 0, 8192, "SHARC/RAW/STR_BACK_E")->Draw("colz");

    // PAD_E_RAW
    c2->cd(i+5);
    name = "SHARC"+NPL::itoa(i+1)+"_PAD_E_RAW";
    AddHisto1D(name, name, 512, 0, 16384, "SHARC/RAW/PAD_E")->Draw("");

    // STR_FRONT_RAW_MULT
    c1->cd(i+4);
    name = "SHARC"+NPL::itoa(i+1)+"_STR_FRONT_RAW_MULT";
    AddHisto1D(name, name, fStripFront, 1, fStripFront+1, "SHARC/RAW/MULT")->Draw("");

    // STR_BACK_RAW_MULT
    c1->cd(i+5);
    name = "SHARC"+NPL::itoa(i+1)+"_STR_BACK_RAW_MULT";
    AddHisto1D(name, name, fStripFront, 1, fStripFront+1, "SHARC/RAW/MULT")->Draw("");

    // PAD_RAW_MULT
    c2->cd(i+2);
    name = "SHARC"+NPL::itoa(i+1)+"_PAD_RAW_MULT";
    AddHisto1D(name, name, fNumberOfDetector, 1, fNumberOfDetector+1, "SHARC/RAW/MULT")->Draw("");

  } // end loop on number of detectors
  AddCanvas(c1);  
  AddCanvas(c2);
}

////////////////////////////////////////////////////////////////////////////////
void TSharcSpectra::InitPreTreatedSpectra(){
  string name;
  for (unsigned int i = 0; i < fNumberOfDetector; i++) { // loop on number of detectors
    // STR_FRONT_E_CAL
    name = "SHARC"+NPL::itoa(i+1)+"_STR_FRONT_E_CAL";
    AddHisto2D(name, name, fStripFront, 1, fStripFront+1, 500, 0, 25, "SHARC/CAL/STR_FRONT_E");

    // STR_BACK_E_CAL
    name = "SHARC"+NPL::itoa(i+1)+"_STR_BACK_E_CAL";
    AddHisto2D(name, name, fStripBack, 1, fStripBack+1, 500, 0, 25, "SHARC/CAL/STR_BACK_E");

    // PAD_E_CAL
    name = "SHARC"+NPL::itoa(i+1)+"_PAD_E_CAL";
    AddHisto1D(name, name, 100, 0, 50, "SHARC/CAL/PAD_E");

    // STR_FRONT_CAL_MULT
    name = "SHARC"+NPL::itoa(i+1)+"_STR_FRONT_CAL_MULT";
    AddHisto1D(name, name, fStripFront, 1, fStripFront+1, "SHARC/CAL/MULT");

    // STR_BACK_CAL_MULT
    name = "SHARC"+NPL::itoa(i+1)+"_STR_BACK_CAL_MULT";
    AddHisto1D(name, name, fStripFront, 1, fStripFront+1, "SHARC/CAL/MULT");

    // PAD_CAL_MULT
    name = "SHARC"+NPL::itoa(i+1)+"_PAD_CAL_MULT";
    AddHisto1D(name, name, fNumberOfDetector, 1, fNumberOfDetector+1, "SHARC/CAL/MULT");
   
    // PAD_CAL_ID 
     name = "SHARC"+NPL::itoa(i+1)+"_PAD_CAL_ID";
     AddHisto2D(name, name,100,0,50,500,0,50, "SHARC/CAL/ID");
  }  // end loop on number of detectors
}

////////////////////////////////////////////////////////////////////////////////
void TSharcSpectra::InitPhysicsSpectra(){
  string name;
  // Kinematic Plot 
  name = "SHARC_THETA_E";
  AddHisto2D(name, name,360,0,180,500,0,50,"SHARC/PHY");

  // X-Y Energy Correlation
  for (unsigned int i = 0 ; i < fNumberOfDetector ; i++) { // loop on number of detectors
    name = "SHARC"+NPL::itoa(i+1)+"_XY_COR";
    AddHisto2D(name, name,500,0,25,500,0,25, "SHARC/PHY"); 
  }

  // ID Plot
  // PAD-DE:
  name = "SHARC_PAD_E_E";
  AddHisto1D(name, name,500,0,25,"SHARC/PHY");

  for (unsigned int i = 0; i < fNumberOfDetector; i++) { // loop on number of detectors
    // PAD-DE:
    name = "SHARC"+NPL::itoa(i+1)+"_PAD_E_E";
    AddHisto2D(name, name,100,0,100,500,0,25,"SHARC/PHY");
  }
}



////////////////////////////////////////////////////////////////////////////////
void TSharcSpectra::FillRawSpectra(TSharcData* RawData){
  string name;
  string family;

  // STR_FRONT_E 
  for (unsigned int i = 0; i < RawData->GetMultiplicityFront(); i++) {
    name = "SHARC"+NPL::itoa(RawData->GetFront_DetectorNbr(i))+"_STR_FRONT_E_RAW";
    family = "SHARC/RAW/STR_FRONT_E";

    GetHisto(family,name)
      -> Fill(RawData->GetFront_StripNbr(i), 
          RawData->GetFront_Energy(i) - 8192);
  }

  // STR_BACK_E
  for (unsigned int i = 0; i < RawData->GetMultiplicityBack(); i++) {
    name = "SHARC"+NPL::itoa( RawData->GetBack_DetectorNbr(i) )+"_STR_BACK_E_RAW";
    family = "SHARC/RAW/STR_BACK_E";

    GetHisto(family,name)
      -> Fill(RawData->GetBack_StripNbr(i),
          RawData->GetBack_Energy(i));
  }

  // PAD_E
  for (unsigned int i = 0; i < RawData->GetMultiplicityPAD(); i++) {
    name = "SHARC"+NPL::itoa(RawData->GetPAD_DetectorNbr(i))+"_PAD_E_RAW";
    family = "SHARC/RAW/PAD_E";

    GetHisto(family,name)
      -> Fill(RawData->GetPAD_Energy(i));
  }

  // STR_FRONT MULT
  int myMULT[fNumberOfDetector];
  for( unsigned int i = 0; i < fNumberOfDetector; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < RawData->GetMultiplicityFront();i++){
    myMULT[RawData->GetFront_DetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfDetector; i++){

    name = "SHARC"+NPL::itoa(i+1)+"_STR_FRONT_RAW_MULT";
    family= "SHARC/RAW/MULT";
    GetHisto(family,name)
      -> Fill(myMULT[i]);
  }

  // STR_BACK MULT
  for( unsigned int i = 0; i < fNumberOfDetector; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < RawData->GetMultiplicityBack();i++){
    myMULT[RawData->GetBack_DetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfDetector; i++){
    name = "SHARC"+NPL::itoa(i+1)+"_STR_BACK_RAW_MULT";
    family= "SHARC/RAW/MULT";
    GetHisto(family,name)
      -> Fill(myMULT[i]);
  }

  // PAD MULT
  for( unsigned int i = 0; i < fNumberOfDetector; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < RawData->GetMultiplicityPAD();i++){
    myMULT[RawData->GetPAD_DetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfDetector; i++){
    name = "SHARC"+NPL::itoa(i+1)+"_PAD_RAW_MULT";
    family= "SHARC/RAW/MULT";
    GetHisto(family,name)
      -> Fill(myMULT[i]);
  }

}

////////////////////////////////////////////////////////////////////////////////
void TSharcSpectra::FillPreTreatedSpectra(TSharcData* PreTreatedData){
  string name ;
  string family;
  // STR_FRONT_E
  for (unsigned int i = 0; i < PreTreatedData->GetMultiplicityFront(); i++) {
    name = "SHARC"+NPL::itoa(PreTreatedData->GetFront_DetectorNbr(i))+"_STR_FRONT_E_CAL";
    family = "SHARC/CAL/STR_FRONT_E";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetFront_StripNbr(i), 
          PreTreatedData->GetFront_Energy(i));
  }
  // STR_BACK_E
  for (unsigned int i = 0; i < PreTreatedData->GetMultiplicityBack(); i++) {
    name = "SHARC"+NPL::itoa( PreTreatedData->GetBack_DetectorNbr(i))+"_STR_BACK_E_CAL";
    family = "SHARC/CAL/STR_BACK_E";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetBack_StripNbr(i), 
          PreTreatedData->GetBack_Energy(i));
  }
  // PAD_E
  for (unsigned int i = 0; i < PreTreatedData->GetMultiplicityPAD (); i++) {
    name = "SHARC"+NPL::itoa(PreTreatedData->GetPAD_DetectorNbr(i))+"_PAD_E_CAL";
    family = "SHARC/CAL/PAD_E";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetPAD_Energy(i));
  }
  
  // STR_FRONT MULT
  int myMULT[fNumberOfDetector];
  for( unsigned int i = 0; i < fNumberOfDetector; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < PreTreatedData->GetMultiplicityFront();i++){
    myMULT[PreTreatedData->GetFront_DetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfDetector; i++){

    name = "SHARC"+NPL::itoa(i+1)+"_STR_FRONT_CAL_MULT";
    family= "SHARC/CAL/MULT";
    GetHisto(family,name)
      -> Fill(myMULT[i]);
  }

  // STR_BACK MULT
  for( unsigned int i = 0; i < fNumberOfDetector; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < PreTreatedData->GetMultiplicityBack();i++){
    myMULT[PreTreatedData->GetBack_DetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfDetector; i++){
    name = "SHARC"+NPL::itoa(i+1)+"_STR_BACK_CAL_MULT";
    family= "SHARC/CAL/MULT";
    GetHisto(family,name)
      -> Fill(myMULT[i]);
  }

  // PAD MULT
  for( unsigned int i = 0; i < fNumberOfDetector; i++)
    myMULT[i] = 0 ; 

  for(unsigned int i = 0 ; i < PreTreatedData->GetMultiplicityPAD();i++){
    myMULT[PreTreatedData->GetPAD_DetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfDetector; i++){
    name = "SHARC"+NPL::itoa(i+1)+"_PAD_CAL_MULT";
    family= "SHARC/CAL/MULT";
    GetHisto(family,name)
      -> Fill(myMULT[i]);
  }

  //E-PAD ID
  family = "SHARC/CAL/ID";
  for (unsigned int i = 0; i < PreTreatedData->GetMultiplicityFront(); i++) {
   for (unsigned int j = 0; j < PreTreatedData->GetMultiplicityPAD(); j++) {
    
    if(PreTreatedData->GetFront_DetectorNbr(i) == PreTreatedData->GetPAD_DetectorNbr(j)){ 
      name = "SHARC"+NPL::itoa(PreTreatedData->GetFront_DetectorNbr(i))+"_PAD_CAL_ID";
    
      GetHisto(family,name)
        -> Fill(PreTreatedData->GetPAD_Energy(j), 
          PreTreatedData->GetFront_Energy(i));
      }
    }
  }


}

////////////////////////////////////////////////////////////////////////////////
void TSharcSpectra::FillPhysicsSpectra(TSharcPhysics* Physics){
  string name;
  string family= "SHARC/PHY";
    
  // Kine plot
  for(unsigned int i = 0 ; i < Physics->Strip_E.size(); i++){
    double Theta = Physics->GetPositionOfInteraction(i).Angle(TVector3(0,0,1));
    Theta = Theta/deg;
    double Etot=Physics->Strip_E[i];

    if(Physics->PAD_E[i]>0){
      name = "SHARC_PAD_E_E";
      Etot += Physics->PAD_E[i];
      GetHisto(family,name)->Fill(Physics->PAD_E[i],Physics->Strip_E[i]);
      name = "SHARC"+NPL::itoa(Physics->DetectorNumber[i])+"_PAD_E_E";
      GetHisto(family,name)->Fill(Physics->PAD_E[i],Physics->Strip_E[i]);

    }
    name = "SHARC_THETA_E";
    GetHisto(family,name)-> Fill(Theta,Etot);
  }
}

