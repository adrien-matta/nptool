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

  static string name;
  for (unsigned int i = 0; i < fNumberOfDetector; i++) { // loop on number of detectors
    name = "SharcRaw"+NPL::itoa(i+1);
    TCanvas* c1 = new TCanvas(name.c_str(),name.c_str());
    c1->Divide(3,2);
    int i1 = 0;  
    // STR_FRONT_E_RAW
    c1->cd(++i1);
    name = "SHARC"+NPL::itoa(i+1)+"_STR_FRONT_E_RAW";
    AddHisto2D(name, name, fStripFront, 1, fStripFront+1, 5000, 0, 1.5e6, "SHARC/RAW/STR_FRONT_E")->Draw("colz");

    // STR_BACK_E_RAW
    c1->cd(++i1);
    name = "SHARC"+NPL::itoa(i+1)+"_STR_BACK_E_RAW";
    AddHisto2D(name, name, fStripBack, 1, fStripBack+1, 5000, 0, 1.5e6, "SHARC/RAW/STR_BACK_E")->Draw("colz");

    // STR_FRONT_EMAX_RAW
    name = "SHARC"+NPL::itoa(i+1)+"_STR_FRONT_EMAX_RAW";
    AddHisto2D(name, name, fStripFront, 1, fStripFront+1, 5000, 0, 1.5e6, "SHARC/RAW/STR_FRONT_EMAX");

    // STR_BACK_EMAX_Raw
    name = "SHARC"+NPL::itoa(i+1)+"_STR_BACK_EMAX_RAW";
    AddHisto2D(name, name, fStripBack, 1, fStripBack+1, 5000, 0, 1.5e6, "SHARC/RAW/STR_BACK_EMAX");

    // PAD_E_RAW
    c1->cd(++i1);
    name = "SHARC"+NPL::itoa(i+1)+"_PAD_E_RAW";
    AddHisto1D(name, name, 500, 0, 2500, "SHARC/RAW/PAD_E")->Draw("");

    // STR_FRONT_RAW_MULT
    c1->cd(++i1);
    name = "SHARC"+NPL::itoa(i+1)+"_STR_FRONT_RAW_MULT";
    AddHisto1D(name, name, fStripFront, 1, fStripFront+1, "SHARC/RAW/MULT")->Draw("");
    gPad->SetLogy();

    // STR_BACK_RAW_MULT
    c1->cd(++i1);
    name = "SHARC"+NPL::itoa(i+1)+"_STR_BACK_RAW_MULT";
    AddHisto1D(name, name, fStripFront, 1, fStripFront+1, "SHARC/RAW/MULT")->Draw("");
    gPad->SetLogy();

    // PAD_RAW_MULT
    c1->cd(++i1);
    name = "SHARC"+NPL::itoa(i+1)+"_PAD_RAW_MULT";
    AddHisto1D(name, name, fNumberOfDetector, 1, fNumberOfDetector+1, "SHARC/RAW/MULT")->Draw("");
    gPad->SetLogy();

    AddCanvas(c1);  
  } // end loop on number of detectors
}

////////////////////////////////////////////////////////////////////////////////
void TSharcSpectra::InitPreTreatedSpectra(){
  static string name;
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

    // Front-Back Energy Correlation
      name = "SHARC"+NPL::itoa(i+1)+"_FB_COR";
      AddHisto2D(name, name,500,0,25,500,0,25, "SHARC/CAL/FB"); 

  }  // end loop on number of detectors
}

////////////////////////////////////////////////////////////////////////////////
void TSharcSpectra::InitPhysicsSpectra(){
  static string name;
  // Kinematic Plot 
  name = "SHARC_THETA_E";
  AddHisto2D(name, name,360,0,180,500,0,50,"SHARC/PHY");

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
  static string index;

  // STR_FRONT_E 
  unsigned int mysize = RawData->GetMultiplicityFront();
  double EFMAX = 0 ;
  int SFMAX = 0;
  int DFMAX = 0 ;
  for (unsigned int i = 0; i < mysize; i++) {
    index = "SHARC/RAW/STR_FRONT_E/SHARC"+NPL::itoa(RawData->GetFront_DetectorNbr(i))+"_STR_FRONT_E_RAW";
    if(RawData->GetFront_Energy(i) > EFMAX){
      EFMAX = RawData->GetFront_Energy(i);
      SFMAX = RawData->GetFront_StripNbr(i);
      DFMAX = RawData->GetFront_DetectorNbr(i);
    }
    
    GetHisto(index)
      -> Fill(RawData->GetFront_StripNbr(i), 
          RawData->GetFront_Energy(i));
  }
 
  if(DFMAX!=0){
    index = "SHARC/RAW/STR_FRONT_EMAX/SHARC"+NPL::itoa(DFMAX)+"_STR_FRONT_EMAX_RAW";
    GetHisto(index)-> Fill(SFMAX, EFMAX);
  }
 
  // STR_BACK_E
  mysize = RawData->GetMultiplicityBack();
  double EBMAX = 0 ;
  int SBMAX = 0;
  int DBMAX = 0 ;
 
  for (unsigned int i = 0; i < mysize; i++) {
     index = "SHARC/RAW/STR_BACK_E/SHARC"+NPL::itoa( RawData->GetBack_DetectorNbr(i) )+"_STR_BACK_E_RAW";
    if(RawData->GetBack_Energy(i) > EBMAX){
      EBMAX = RawData->GetBack_Energy(i);
      SBMAX = RawData->GetBack_StripNbr(i);
      DBMAX = RawData->GetBack_DetectorNbr(i);
    }
   
    GetHisto(index)
      -> Fill(RawData->GetBack_StripNbr(i),
          RawData->GetBack_Energy(i));
  }
 
  if(DBMAX!=0){
    index = "SHARC/RAW/STR_BACK_EMAX/SHARC"+NPL::itoa(DBMAX)+"_STR_BACK_EMAX_RAW";
    GetHisto(index)-> Fill(SBMAX, EBMAX);
  }

  // PAD_E
  mysize = RawData->GetMultiplicityPAD();
  for (unsigned int i = 0; i < mysize; i++) {
    index = "SHARC/RAW/PAD_E/SHARC"+NPL::itoa(RawData->GetPAD_DetectorNbr(i))+"_PAD_E_RAW";

    GetHisto(index)
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
    index = "SHARC/RAW/MULT/SHARC"+NPL::itoa(i+1)+"_STR_FRONT_RAW_MULT";
    GetHisto(index)
      -> Fill(myMULT[i]);
  }

  // STR_BACK MULT
  for( unsigned int i = 0; i < fNumberOfDetector; i++)
    myMULT[i] = 0 ; 

  mysize = RawData->GetMultiplicityBack();
  for(unsigned int i = 0 ; i < mysize;i++){
    myMULT[RawData->GetBack_DetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfDetector; i++){
    index= "SHARC/RAW/MULT/SHARC"+NPL::itoa(i+1)+"_STR_BACK_RAW_MULT";
    
    GetHisto(index)
      -> Fill(myMULT[i]);
  }

  // PAD MULT
  for( unsigned int i = 0; i < fNumberOfDetector; i++)
    myMULT[i] = 0 ; 
  mysize = RawData->GetMultiplicityPAD();
  for(unsigned int i = 0 ; i < mysize ;i++){
    myMULT[RawData->GetPAD_DetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfDetector; i++){
  index="SHARC/RAW/MULT/SHARC"+NPL::itoa(i+1)+"_PAD_RAW_MULT";
    GetHisto(index)
      -> Fill(myMULT[i]);
  }

}

////////////////////////////////////////////////////////////////////////////////
void TSharcSpectra::FillPreTreatedSpectra(TSharcData* PreTreatedData){
  static string index;

  // Front-Back
  unsigned int mysizeF = PreTreatedData->GetMultiplicityFront();
  unsigned int mysizeB = PreTreatedData->GetMultiplicityBack();

  for (unsigned int i = 0; i < mysizeF; i++) {
    for (unsigned int j = 0; j < mysizeB; j++) {
      if(PreTreatedData->GetFront_DetectorNbr(i)==PreTreatedData->GetBack_DetectorNbr(j)){
        index="SHARC/CAL/FB/SHARC"+NPL::itoa(PreTreatedData->GetFront_DetectorNbr(i))+"_FB_COR";

      GetHisto(index)
        -> Fill(PreTreatedData->GetFront_Energy(i),
                PreTreatedData->GetBack_Energy(j) );
      }
    }
  } 

  // STR_FRONT_E
  unsigned int mysize = PreTreatedData->GetMultiplicityFront();
  for (unsigned int i = 0; i < mysize; i++) {
    index = "SHARC/CAL/STR_FRONT_E/SHARC"+NPL::itoa(PreTreatedData->GetFront_DetectorNbr(i))+"_STR_FRONT_E_CAL";

    GetHisto(index)
      -> Fill(PreTreatedData->GetFront_StripNbr(i), 
          PreTreatedData->GetFront_Energy(i));
  }
  // STR_BACK_E
  mysize = PreTreatedData->GetMultiplicityBack();
  for (unsigned int i = 0; i < mysize; i++) {
   index = "SHARC/CAL/STR_BACK_E/SHARC"+NPL::itoa( PreTreatedData->GetBack_DetectorNbr(i))+"_STR_BACK_E_CAL";

    GetHisto(index)
      -> Fill(PreTreatedData->GetBack_StripNbr(i), 
          PreTreatedData->GetBack_Energy(i));
  }
  // PAD_E
  mysize = PreTreatedData->GetMultiplicityPAD();
  for (unsigned int i = 0; i < mysize ; i++) {
    index= "SHARC/CAL/PAD_E/SHARC"+NPL::itoa(PreTreatedData->GetPAD_DetectorNbr(i))+"_PAD_E_CAL";

    GetHisto(index)
      -> Fill(PreTreatedData->GetPAD_Energy(i));
  }

  // STR_FRONT MULT
  int myMULT[fNumberOfDetector];
  for( unsigned int i = 0; i < fNumberOfDetector; i++)
    myMULT[i] = 0 ; 

  mysize = PreTreatedData->GetMultiplicityFront(); 
  for(unsigned int i = 0 ; i < mysize ;i++){
    myMULT[PreTreatedData->GetFront_DetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfDetector; i++){
    index= "SHARC/CAL/MULT/SHARC"+NPL::itoa(i+1)+"_STR_FRONT_CAL_MULT";
    GetHisto(index)
      -> Fill(myMULT[i]);
  }

  // STR_BACK MULT
  for( unsigned int i = 0; i < fNumberOfDetector; i++)
    myMULT[i] = 0 ; 

  mysize = PreTreatedData->GetMultiplicityBack();
  for(unsigned int i = 0 ; i < mysize ;i++){
    myMULT[PreTreatedData->GetBack_DetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfDetector; i++){
    index= "SHARC/CAL/MULT/SHARC"+NPL::itoa(i+1)+"_STR_BACK_CAL_MULT";
    GetHisto(index)
      -> Fill(myMULT[i]);
  }

  // PAD MULT
  for( unsigned int i = 0; i < fNumberOfDetector; i++)
    myMULT[i] = 0 ; 

  mysize =  PreTreatedData->GetMultiplicityPAD();
  for(unsigned int i = 0 ; i < mysize ;i++){
    myMULT[PreTreatedData->GetPAD_DetectorNbr(i)-1] += 1;  
  }

  for( unsigned int i = 0; i < fNumberOfDetector; i++){
    index = "SHARC/CAL/MULT/SHARC"+NPL::itoa(i+1)+"_PAD_CAL_MULT";
    GetHisto(index)
      -> Fill(myMULT[i]);
  }

  //E-PAD ID
  mysize = PreTreatedData->GetMultiplicityFront();
  unsigned int mysizePAD = PreTreatedData->GetMultiplicityPAD();
  for (unsigned int i = 0; i < mysize ; i++) {
    for (unsigned int j = 0; j < mysizePAD; j++) {

      if(PreTreatedData->GetFront_DetectorNbr(i) == PreTreatedData->GetPAD_DetectorNbr(j)){ 
        index = "SHARC/CAL/ID/SHARC"+NPL::itoa(PreTreatedData->GetFront_DetectorNbr(i))+"_PAD_CAL_ID";

        GetHisto(index)
          -> Fill(PreTreatedData->GetPAD_Energy(j), 
              PreTreatedData->GetFront_Energy(i));
      }
    }
  }


}

////////////////////////////////////////////////////////////////////////////////
void TSharcSpectra::FillPhysicsSpectra(TSharcPhysics* Physics){
  static string index;

  // Kine plot
  unsigned int mysize = Physics->Strip_E.size();
  for(unsigned int i = 0 ; i < mysize ; i++){
    double Theta = Physics->GetPositionOfInteraction(i).Angle(TVector3(0,0,1));
    Theta = Theta/deg;
    double Etot=Physics->Strip_E[i];

    if(Physics->PAD_E[i]>0){
      index = "SHARC/PHY/SHARC_PAD_E_E";
      Etot += Physics->PAD_E[i];
      GetHisto(index)->Fill(Physics->PAD_E[i],Physics->Strip_E[i]);
      index = "SHARC/PHY/SHARC"+NPL::itoa(Physics->DetectorNumber[i])+"_PAD_E_E";
      GetHisto(index)->Fill(Physics->PAD_E[i],Physics->Strip_E[i]);

    }
    index = "SHARC/PHY/SHARC_THETA_E";
    GetHisto(index)-> Fill(Theta,Etot);
  }
}

