/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: A. MATTA  contact address: a.matta@surrey.ac.uk          *
 *                                                                           *
 * Creation Date  : dec 2013                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds all the online spectra needed for TiaraBarrel           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPL
#include "TTiaraBarrelSpectra.h"
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
TTiaraBarrelSpectra::TTiaraBarrelSpectra(){
  fNumberOfDetector= 0;
  fInnerBarrelStrip=4;
  fOuterBarrelStrip=4;
}

////////////////////////////////////////////////////////////////////////////////
TTiaraBarrelSpectra::TTiaraBarrelSpectra(unsigned int NumberOfDetector){
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TTiaraBarrelSpectra : Initalising control spectra for " 
      << NumberOfDetector << " Detectors" << endl
      << "************************************************" << endl ;

  fNumberOfDetector= NumberOfDetector;
  fInnerBarrelStrip=4;
  fOuterBarrelStrip=4;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}

////////////////////////////////////////////////////////////////////////////////
TTiaraBarrelSpectra::~TTiaraBarrelSpectra(){
}

////////////////////////////////////////////////////////////////////////////////
void TTiaraBarrelSpectra::InitRawSpectra(){
  TString name;
  TString BaseFamily = "TIARA/BARREL/RAW/";

  //// HIT ////
  // Inner Barrel
  // Hit Up Stream
  name = "INNER_BARREL_US_HIT_RAW";
  AddHisto1D(name, name, fNumberOfDetector*fInnerBarrelStrip, 1, fNumberOfDetector*fInnerBarrelStrip+1, BaseFamily+"HIT");
  //Hit Down Stream
  name = "INNER_BARREL_DS_HIT_RAW";
  AddHisto1D(name, name, fNumberOfDetector*fInnerBarrelStrip, 1, fNumberOfDetector*fInnerBarrelStrip+1, BaseFamily+"HIT");
  // Hit Back
  name = "INNER_BARREL_BACK_HIT_RAW";
  AddHisto1D(name, name, fNumberOfDetector, 1, fNumberOfDetector+1, BaseFamily+"HIT");

  // Outer Barrel
  //Hit Strip
  name = "OUTER_BARREL_STRIP_HIT_RAW";
  AddHisto1D(name, name, fNumberOfDetector*fOuterBarrelStrip, 1, fNumberOfDetector*fOuterBarrelStrip+1, BaseFamily+"HIT");

  //// E ////
  // Inner Barrel
  // E Up Stream
  name = "INNER_BARREL_US_E_RAW";
  AddHisto2D(name, name, fNumberOfDetector*fInnerBarrelStrip, 1, fNumberOfDetector*fInnerBarrelStrip+1, 512,0,16384,BaseFamily+"E");
  //E Down Stream
  name = "INNER_BARREL_DS_E_RAW";
  AddHisto2D(name, name, fNumberOfDetector*fInnerBarrelStrip, 1, fNumberOfDetector*fInnerBarrelStrip+1,512,0,16384, BaseFamily+"E");
  //E Back
  name = "INNER_BARREL_BACK_E_RAW";
  AddHisto2D(name, name, fNumberOfDetector, 1, fNumberOfDetector+1,512,0,16384, BaseFamily+"E");

  // Outer Barrel
  //E Strip
  name = "OUTER_BARREL_STRIP_E_RAW";
  AddHisto2D(name, name, fNumberOfDetector*fOuterBarrelStrip, 1, fNumberOfDetector*fOuterBarrelStrip+1,512,0,16384, BaseFamily+"E");

  //// SUM ////
  // Inner Barrel
  // Sum Up Stream
  name = "INNER_BARREL_STRIP_SUM_RAW";
  AddHisto2D(name, name, fNumberOfDetector*fInnerBarrelStrip, 1, fNumberOfDetector*fInnerBarrelStrip+1, 1024,0,32768,BaseFamily+"SUM");

}

////////////////////////////////////////////////////////////////////////////////
void TTiaraBarrelSpectra::InitPreTreatedSpectra(){
}

////////////////////////////////////////////////////////////////////////////////
void TTiaraBarrelSpectra::InitPhysicsSpectra(){
}

////////////////////////////////////////////////////////////////////////////////
void TTiaraBarrelSpectra::FillRawSpectra(TTiaraBarrelData* RawData){
  TString name;
  TString family;
  TString BaseFamily = "TIARA/BARREL/RAW/";

  // INNER_BARREL_US_HIT_RAW
  for (unsigned int i = 0; i < RawData->GetFrontUpstreamEMult(); i++) {
    name = "INNER_BARREL_US_HIT_RAW";
    family = BaseFamily+"HIT";

    GetHisto(family,name)
      ->Fill((RawData->GetFrontUpstreamEDetectorNbr(i)-1)*fInnerBarrelStrip
          +RawData->GetFrontUpstreamEStripNbr(i));

    family = BaseFamily+"E";
    name = "INNER_BARREL_US_E_RAW";
    GetHisto(family,name)
      ->Fill((RawData->GetFrontUpstreamEDetectorNbr(i)-1)*fInnerBarrelStrip
          +RawData->GetFrontUpstreamEStripNbr(i),RawData->GetFrontUpstreamEEnergy(i));
  }
  // INNER_BARREL_DS_HIT_RAW
  for (unsigned int i = 0; i < RawData->GetFrontDownstreamEMult(); i++) {
    name = "INNER_BARREL_DS_HIT_RAW";
    family = BaseFamily+"HIT";

    GetHisto(family,name)
      ->Fill((RawData->GetFrontDownstreamEDetectorNbr(i)-1)*fInnerBarrelStrip
          +RawData->GetFrontDownstreamEStripNbr(i));

    name = "INNER_BARREL_DS_E_RAW";
    family = BaseFamily+"E";
    GetHisto(family,name)
      ->Fill((RawData->GetFrontDownstreamEDetectorNbr(i)-1)*fInnerBarrelStrip
          +RawData->GetFrontDownstreamEStripNbr(i),RawData->GetFrontDownstreamEEnergy(i));
  }

  // INNER_BARREL_BACK_HIT_RAW
  for (unsigned int i = 0; i < RawData->GetBackEMult(); i++) {
    name = "INNER_BARREL_BACK_HIT_RAW";
    family = BaseFamily+"HIT";

    GetHisto(family,name)
      ->Fill((RawData->GetBackEDetectorNbr(i)-1)*fInnerBarrelStrip);

    name = "INNER_BARREL_BACK_E_RAW";
    family = BaseFamily+"E";
    GetHisto(family,name)
      ->Fill((RawData->GetBackEDetectorNbr(i)-1)*fInnerBarrelStrip, RawData->GetBackEEnergy(i));
  }

  // OUTER_BARREL_STRIP_HIT_RAW
  for (unsigned int i = 0; i < RawData->GetOuterEMult(); i++) {
    name = "OUTER_BARREL_STRIP_HIT_RAW";
    family = BaseFamily+"HIT";

    GetHisto(family,name)
      ->Fill((RawData->GetOuterEDetectorNbr(i)-1)*fInnerBarrelStrip
          +RawData->GetOuterEStripNbr(i));
    
    family = BaseFamily+"E";
    name = "OUTER_BARREL_STRIP_E_RAW";
    GetHisto(family,name)
      ->Fill((RawData->GetOuterEDetectorNbr(i)-1)*fInnerBarrelStrip
          +RawData->GetOuterEStripNbr(i),RawData->GetOuterEEnergy(i));
  }

  // INNER_BARREL_US_HIT_RAW
  for (unsigned int i = 0; i < RawData->GetFrontUpstreamEMult(); i++) {
    name = "INNER_BARREL_STRIP_SUM_RAW";
    family = BaseFamily+"SUM";
    for (unsigned int j = 0; j < RawData->GetFrontDownstreamEMult(); j++) {
      int channelUP = (RawData->GetFrontDownstreamEDetectorNbr(i)-1)*fInnerBarrelStrip
        +RawData->GetFrontDownstreamEStripNbr(i) ;
      int channelDW = (RawData->GetFrontDownstreamEDetectorNbr(j)-1)*fInnerBarrelStrip
        +RawData->GetFrontDownstreamEStripNbr(j) ;

      if(channelUP==channelDW)
        GetHisto(family,name)
          ->Fill(channelUP,RawData->GetFrontDownstreamEEnergy(i)+RawData->GetFrontDownstreamEEnergy(i));
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
void TTiaraBarrelSpectra::FillPreTreatedSpectra(TTiaraBarrelData* PreTreatedData){
}

////////////////////////////////////////////////////////////////////////////////
void TTiaraBarrelSpectra::FillPhysicsSpectra(TTiaraBarrelPhysics* Physics){
}

////////////////////////////////////////////////////////////////////////////////
TH1* TTiaraBarrelSpectra::AddHisto1D(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, TString family){
  // create histo
  TH1 *hist = new TH1D(name, title, nbinsx, xlow, xup);

  vector<TString> index ;
  index.push_back(family);
  index.push_back(name);

  // fill map
  fMapHisto[index]=hist;

  return hist;
}

////////////////////////////////////////////////////////////////////////////////
TH1* TTiaraBarrelSpectra::AddHisto2D(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString family){
  // create histo
  TH1 *hist = new TH2D(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup);

  vector<TString> index ;
  index.push_back(family);
  index.push_back(name);

  // fill map
  fMapHisto[index]=hist;

  return hist;
}

////////////////////////////////////////////////////////////////////////////////
TH1* TTiaraBarrelSpectra::GetHisto(TString family, TString name){
  vector<TString> index ;
  index.push_back(family);
  index.push_back(name);
  // fill map
  return fMapHisto.at(index);
}

////////////////////////////////////////////////////////////////////////////////
void TTiaraBarrelSpectra::WriteHisto(TString filename){
  TFile* f=NULL; 

  if(filename!="VOID"){
    f = new TFile(filename,"RECREATE");
  }

  map< vector<TString>, TH1* >::iterator it;

  for (it=fMapHisto.begin(); it!=fMapHisto.end(); ++it){
    it->second->Write();
  }

  if(filename!="VOID"){
    f->Close();
    delete f;
  }

}
///////////////////////////////////////////////////////////////////////////////
void TTiaraBarrelSpectra::CheckSpectra(){
  map< vector<TString>, TH1* >::iterator it;
  Color_t ok_color = kTeal+9;
  Color_t warning_color = kOrange+8;   warning_color *= 1;
  Color_t bad_color = kRed;            bad_color *= 1;

  for (it=fMapHisto.begin(); it!=fMapHisto.end(); ++it){
    it->second->SetLineColor(ok_color);
    it->second->SetFillStyle(1001);
    it->second->SetFillColor(ok_color);
  }

}
