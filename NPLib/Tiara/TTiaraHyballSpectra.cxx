/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : dec 2013                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds all the online spectra needed for Tiara/Hyball          *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + first version (not complete yet)                                     *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPL
#include "TTiaraHyballSpectra.h"
#include "NPOptionManager.h"
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"
#ifdef NP_SYSTEM_OF_UNITS_H
using namespace NPUNITS;
#endif

// ROOT
#include "TString.h"
#include "TDirectory.h"
#include "TFile.h"

////////////////////////////////////////////////////////////////////////////////
TTiaraHyballSpectra::TTiaraHyballSpectra(){
  if (NPOptionManager::getInstance()->GetVerboseLevel() > 0) {
    cout << "************************************************" << endl;
    cout << "TTiaraHyballSpectra: Initalising control spectra" << endl; 
    cout << "************************************************" << endl;
  }

  fRingsNumber   = 16;
  fSectorsNumber =  8;
  fWedgesNumber  =  6;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}



////////////////////////////////////////////////////////////////////////////////
TTiaraHyballSpectra::~TTiaraHyballSpectra(){
}

////////////////////////////////////////////////////////////////////////////////
void TTiaraHyballSpectra::InitRawSpectra(){
  TString name;

  // HIT_RING_RAW
  name = "HYB_HIT_RING_RAW";
  AddHisto1D(name, name, fWedgesNumber*fRingsNumber, 1, fWedgesNumber*fRingsNumber+1, "TIARA/HYBALL/RAW/HIT");

  // HIT_SECTOR_RAW
  name = "HYB_HIT_SECT_RAW";
  AddHisto1D(name, name, fWedgesNumber*fSectorsNumber, 1, fWedgesNumber*fSectorsNumber+1, "TIARA/HYBALL/RAW/HIT");

  // RING_E_RAW
  name = "HYB_RING_E_RAW";
  AddHisto2D(name, name, fWedgesNumber*fRingsNumber, 1, fWedgesNumber*fRingsNumber+1, 512, 0, 16384, "TIARA/HYBALL/RAW/ENERGY");

  // SECTOR_E_RAW
  name = "HYB_SECT_E_RAW";
  AddHisto2D(name, name, fWedgesNumber*fSectorsNumber, 1, fWedgesNumber*fSectorsNumber+1, 512, 0, 16384, "TIARA/HYBALL/RAW/ENERGY");

  // MULT
  for (unsigned int i = 0; i < fWedgesNumber; ++i) {   // loop on number of wedges
    // RING_RAW_MULT
    name = Form("HYB_W%d_RING_E_RAW_MULT", i+1);
    AddHisto1D(name, name, fRingsNumber, 1, fRingsNumber+1, "TIARA/HYBALL/RAW/MULT");

    // SECTOR_RAW_MULT
    name = Form("HYB_W%d_SECT_E_RAW_MULT", i+1);
    AddHisto1D(name, name, fSectorsNumber, 1, fSectorsNumber+1, "TIARA/HYBALL/RAW/MULT");
  } // end loop on number of wedges
}



////////////////////////////////////////////////////////////////////////////////
void TTiaraHyballSpectra::InitPreTreatedSpectra(){
  TString name;

  // HIT_RING_CAL
  name = "HYB_HIT_RING_CAL";
  AddHisto1D(name, name, fWedgesNumber*fRingsNumber, 1, fWedgesNumber*fRingsNumber+1, "TIARA/HYBALL/CAL/HIT");

  // HIT_SECTOR_CAL
  name = "HYB_HIT_SECT_CAL";
  AddHisto1D(name, name, fWedgesNumber*fSectorsNumber, 1, fWedgesNumber*fSectorsNumber+1, "TIARA/HYBALL/CAL/HIT");

  // RING_E_CAL
  name = "HYB_RING_E_CAL";
  AddHisto2D(name, name, fWedgesNumber*fRingsNumber, 1, fWedgesNumber*fRingsNumber+1, 500, 0, 50, "TIARA/HYBALL/CAL/ENERGY");

  // SECTOR_E_CAL
  name = "HYB_SECT_E_CAL";
  AddHisto2D(name, name, fWedgesNumber*fSectorsNumber, 1, fWedgesNumber*fSectorsNumber+1, 500, 0, 50, "TIARA/HYBALL/CAL/ENERGY");

  // MULT
  for (unsigned int i = 0; i < fWedgesNumber; ++i) {   // loop on number of wedges
    // RING_CAL_MULT
    name = Form("HYB_W%d_RING_E_CAL_MULT", i+1);
    AddHisto1D(name, name, fRingsNumber, 1, fRingsNumber+1, "TIARA/HYBALL/CAL/MULT");

    // SECTOR_CAL_MULT
    name = Form("HYB_W%d_SECT_E_CAL_MULT", i+1);
    AddHisto1D(name, name, fSectorsNumber, 1, fSectorsNumber+1, "TIARA/HYBALL/CAL/MULT");
  } // end loop on number of wedges
}



////////////////////////////////////////////////////////////////////////////////
void TTiaraHyballSpectra::InitPhysicsSpectra(){
  TString name;
  // X-Y Impact Matrix
  name = "HYB_IMPACT_MATRIX";
  AddHisto2D(name, name, 500, -150, 150, 500, -150, 150, "TIARA/HYBALL/PHY");

  // Kinematic line
  name = "HYB_THETA_E";
  AddHisto2D(name, name, 360, 0, 180, 500, 0, 50, "TIARA/HYBALL/PHY");

  // Ring v.s. Sector Energy Correlation
  name = "HYB_XY_COR";
  AddHisto2D(name, name, 500, 0, 50, 500, 0, 50, "TIARA/HYBALL/PHY"); 
}



////////////////////////////////////////////////////////////////////////////////
void TTiaraHyballSpectra::FillRawSpectra(TTiaraHyballData* RawData){
  TString name;
  TString family;

  // HIT_RING_RAW
  family = "TIARA/HYBALL/RAW/HIT";
  name = "HYB_HIT_RING_RAW";
  for (unsigned int i = 0; i < RawData->GetRingEMult(); i++) {
    unsigned short channel = (RawData->GetRingEDetectorNbr(i)-1) * fRingsNumber + RawData->GetRingEStripNbr(i);
    GetHisto(family, name) -> Fill(channel);
  }

  // HIT_SECT_RAW
  family = "TIARA/HYBALL/RAW/HIT";
  name = "HYB_HIT_SECT_RAW";
  for (unsigned int i = 0; i < RawData->GetSectorEMult(); i++) {
    unsigned short channel = (RawData->GetSectorEDetectorNbr(i)-1) * fSectorsNumber + RawData->GetSectorEStripNbr(i);
    GetHisto(family, name) -> Fill(channel);
  }

  // RING_E_RAW
  family = "TIARA/HYBALL/RAW/ENERGY";
  name = "HYB_RING_E_RAW";
  for (unsigned int i = 0; i < RawData->GetRingEMult(); i++) {
    unsigned short channel = (RawData->GetRingEDetectorNbr(i)-1) * fRingsNumber + RawData->GetRingEStripNbr(i);
    GetHisto(family, name) -> Fill(channel, RawData->GetRingEEnergy(i));
  }

  // SECTOR_E_RAW
  family = "TIARA/HYBALL/RAW/ENERGY";
  name = "HYB_SECT_E_RAW";
  for (unsigned int i = 0; i < RawData->GetSectorEMult(); i++) {
    unsigned short channel = (RawData->GetSectorEDetectorNbr(i)-1) * fSectorsNumber + RawData->GetSectorEStripNbr(i);
    GetHisto(family, name) -> Fill(channel, RawData->GetSectorEEnergy(i));
}

  // RING_RAW_MULT
  int myMULT[fWedgesNumber];
  for (unsigned int i = 0; i < fWedgesNumber; i++) myMULT[i] = 0;
  for (unsigned int i = 0; i < RawData->GetRingEMult(); i++) myMULT[(RawData->GetRingEDetectorNbr(i)-1)] += 1;

  for (unsigned int i = 0; i < fWedgesNumber; i++) {
    name   = Form("HYB_W%d_RING_E_RAW_MULT", i+1);
    family = "TIARA/HYBALL/RAW/MULT";
    GetHisto(family,name) -> Fill(myMULT[i]);
  }

  // Sector_RAW_MULT
  for (unsigned int i = 0; i < fWedgesNumber; i++) myMULT[i] = 0;
  for (unsigned int i = 0; i < RawData->GetSectorEMult(); i++) myMULT[(RawData->GetSectorEDetectorNbr(i)-1)] += 1;

  for (unsigned int i = 0; i < fWedgesNumber; i++) {
    name   = Form("HYB_W%d_SECT_E_RAW_MULT", i+1);
    family = "TIARA/HYBALL/RAW/MULT";
    GetHisto(family,name) -> Fill(myMULT[i]);
  }
}



////////////////////////////////////////////////////////////////////////////////
void TTiaraHyballSpectra::FillPreTreatedSpectra(TTiaraHyballData* PreTreatedData){
  TString name;
  TString family;

  // HIT_RING_CAL
  family = "TIARA/HYBALL/CAL/HIT";
  name = "HYB_HIT_RING_CAL";
  for (unsigned int i = 0; i < PreTreatedData->GetRingEMult(); i++) {
    unsigned short channel = (PreTreatedData->GetRingEDetectorNbr(i)-1) * fRingsNumber + PreTreatedData->GetRingEStripNbr(i);
    GetHisto(family, name) -> Fill(channel);
  }

  // HIT_SECT_CAL
  family = "TIARA/HYBALL/CAL/HIT";
  name = "HYB_HIT_SECT_CAL";
  for (unsigned int i = 0; i < PreTreatedData->GetSectorEMult(); i++) {
    unsigned short channel = (PreTreatedData->GetSectorEDetectorNbr(i)-1) * fSectorsNumber + PreTreatedData->GetSectorEStripNbr(i);
    GetHisto(family, name) -> Fill(channel);
  }

  // RING_E_CAL
  family = "TIARA/HYBALL/CAL/ENERGY";
  name = "HYB_RING_E_CAL";
  for (unsigned int i = 0; i < PreTreatedData->GetRingEMult(); i++) {
    unsigned short channel = (PreTreatedData->GetRingEDetectorNbr(i)-1) * fRingsNumber + PreTreatedData->GetRingEStripNbr(i);
    GetHisto(family, name) -> Fill(channel, PreTreatedData->GetRingEEnergy(i));
  }

  // SECTOR_E_CAL
  family = "TIARA/HYBALL/CAL/ENERGY";
  name = "HYB_SECT_E_CAL";
  for (unsigned int i = 0; i < PreTreatedData->GetRingEMult(); i++) {
    unsigned short channel = (PreTreatedData->GetRingEDetectorNbr(i)-1) * fSectorsNumber + PreTreatedData->GetSectorEStripNbr(i);
    GetHisto(family, name) -> Fill(channel, PreTreatedData->GetRingEEnergy(i));
  }

  // RING_CAL_MULT
  int myMULT[fWedgesNumber];
  for (unsigned int i = 0; i < fWedgesNumber; i++) myMULT[i] = 0;
  for (unsigned int i = 0; i < PreTreatedData->GetRingEMult(); i++) myMULT[(PreTreatedData->GetRingEDetectorNbr(i)-1)] += 1;

  for (unsigned int i = 0; i < fWedgesNumber; i++) {
    name   = Form("HYB_W%d_RING_E_CAL_MULT", i+1);
    family = "TIARA/HYBALL/CAL/MULT";
    GetHisto(family,name) -> Fill(myMULT[i]);
  }

  // RING_CAL_MULT
  for (unsigned int i = 0; i < fWedgesNumber; i++) myMULT[i] = 0;
  for (unsigned int i = 0; i < PreTreatedData->GetSectorEMult(); i++) myMULT[(PreTreatedData->GetSectorEDetectorNbr(i)-1)] += 1;

  for (unsigned int i = 0; i < fWedgesNumber; i++) {
    name   = Form("HYB_W%d_SECT_E_CAL_MULT", i+1);
    family = "TIARA/HYBALL/CAL/MULT";
    GetHisto(family,name) -> Fill(myMULT[i]);
  }
}



////////////////////////////////////////////////////////////////////////////////
void TTiaraHyballSpectra::FillPhysicsSpectra(TTiaraHyballPhysics* Physics){
  /*   TString name;
       TString family= "TIARA/HYBALL/PHY";
  // X-Y Impact Matrix

  for(unsigned int i = 0 ; i < Physics->Si_E.size(); i++){
  name = "MM_IMPACT_MATRIX";
  double x = Physics->GetPositionOfInteraction(i).x();
  double y = Physics->GetPositionOfInteraction(i).y();
  GetHisto(family,name)-> Fill(x,y);

  name = "MM_THETA_E";
  double Theta = Physics->GetPositionOfInteraction(i).Angle(TVector3(0,0,1));
  Theta = Theta/deg;
  GetHisto(family,name)-> Fill(Theta,Physics->Si_E[i]);

  // STRX_E_CAL
  name = Form("MM%d_XY_COR", Physics->TelescopeNumber[i]);
  GetHisto(family,name)-> Fill(Physics->Si_EX[i],Physics->Si_EY[i]);
  }*/

}



////////////////////////////////////////////////////////////////////////////////
TH1* TTiaraHyballSpectra::AddHisto1D(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, TString family){
  // create histo
  TH1 *hist = new TH1D(name, title, nbinsx, xlow, xup);

  vector<TString> index;
  index.push_back(family);
  index.push_back(name);

  // fill map
  fMapHisto[index] = hist;

  return hist;
}



////////////////////////////////////////////////////////////////////////////////
TH1* TTiaraHyballSpectra::AddHisto2D(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString family){
  // create histo
  TH1 *hist = new TH2D(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup);

  vector<TString> index;
  index.push_back(family);
  index.push_back(name);

  // fill map
  fMapHisto[index] = hist;

  return hist;
}



////////////////////////////////////////////////////////////////////////////////
TH1* TTiaraHyballSpectra::GetHisto(TString family, TString name){
  vector<TString> index;
  index.push_back(family);
  index.push_back(name);
  TH1* histo ; 
  
  try{
    histo = fMapHisto.at(index); 
  }

  catch(const std::out_of_range& oor){
  cout << "ERROR : the folowing Histo has been requested by THyballSpectra and does not exist: family:" << family << " name: "  << name << endl ;
  exit(1);
  }

  return histo;
}
////////////////////////////////////////////////////////////////////////////////
void TTiaraHyballSpectra::WriteHisto(TString filename){
  TFile* f = NULL; 

  if (filename != "VOID") {
    f = new TFile(filename,"RECREATE");
  }

  map< vector<TString>, TH1* >::iterator it;
  for (it=fMapHisto.begin(); it!=fMapHisto.end(); ++it) {
    it->second->Write();
  }

  if (filename != "VOID") {
    f->Close();
    delete f;
  }
}
