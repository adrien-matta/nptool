/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : march 2011                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds all the online spectra needed for Exogam                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + first version (not complete yet)                                     *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// NPL
#include "TExogamSpectra.h"
#include "NPOptionManager.h"
#include "NPGlobalSystemOfUnits.h"
#include "NPPhysicalConstants.h"
#ifdef NP_SYSTEM_OF_UNITS_H
using namespace NPUNITS;
#endif

// ROOT
#include "string.h"
#include "TDirectory.h"
#include "TFile.h"


////////////////////////////////////////////////////////////////////////////////
TExogamSpectra::TExogamSpectra(){
  fNumberOfClover = 0;
  fnbinsRaw=16384;
  fbinRawMin=0;
  fbinRawMax=16384;
  fnbinsCal=5000;
  fbinCalMin=0;
  fbinCalMax=5000;

}

////////////////////////////////////////////////////////////////////////////////
TExogamSpectra::TExogamSpectra(unsigned int NumberOfClover){
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TExogamSpectra : Initalising control spectra for " 
      << NumberOfClover << " Clover" << endl
      << "************************************************" << endl ;

  fNumberOfClover = NumberOfClover;
  fNumberOfSegments=16;
  fNumberOfCores=4;
  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}

////////////////////////////////////////////////////////////////////////////////
TExogamSpectra::~TExogamSpectra(){
}

////////////////////////////////////////////////////////////////////////////////
void TExogamSpectra::InitRawSpectra(){
  string name;
  for (unsigned int i = 0; i < fNumberOfClover; i++) { // loop on number of detectors
   for (unsigned int j = 0; j < fNumberOfCores; j++) { // loop on number of cores

    	name = Form("ExogamEnergyRaw_Clover%d_ECC%d", i+1, j+1);
    	AddHisto1D(name, name, fnbinsRaw, fbinRawMin, fbinRawMax, "Exogam/RAW");
    	name = Form("ExogamTimeRaw_Clover%d_ECC%d", i+1, j+1);
    	AddHisto1D(name, name, fnbinsRaw, fbinRawMin, fbinRawMax, "Exogam/RAW");

  	for (unsigned int k = 0; k < fNumberOfSegments; k++) { // loop on number of segments
    	name = Form("ExogamEnergyRaw_Clover%d_ECC%d_GOCCE%d", i+1, j+1, k+1);
    	AddHisto1D(name, name, fnbinsRaw, fbinRawMin, fbinRawMax, "Exogam/RAW");
    	name = Form("ExogamEnergyDC_Clover%d_ECC%d_GOCCE%d", i+1, j+1, k+1);
    	AddHisto1D(name, name, fnbinsRaw, fbinRawMin, fbinRawMax, "Exogam/RAW");
    }
   }
  } // end loop on number of detectors
  
}

////////////////////////////////////////////////////////////////////////////////
void TExogamSpectra::InitPreTreatedSpectra()
{
  string name;
  for (unsigned int i = 0; i < fNumberOfClover; i++) { // loop on number of detectors
   for (unsigned int j = 0; j < fNumberOfCores; j++) { // loop on number of cores
    	name = Form("ExogamEnergyCal_Clover%d_ECC%d", i+1, j+1);
    	AddHisto1D(name, name, fnbinsRaw, fbinRawMin, fbinRawMax, "Exogam/Cal");
    	name = Form("ExogamTimeCal_Clover%d_ECC%d", i+1, j+1);
    	AddHisto1D(name, name, fnbinsRaw, fbinRawMin, fbinRawMax, "Exogam/Cal");

  	for (unsigned int k = 0; k < fNumberOfSegments; k++) { // loop on number of segments
    	name = Form("ExogamEnergyCal_Clover%d_ECC%d_GOCCE%d", i+1, j+1, k+1);
    	AddHisto1D(name, name, fnbinsRaw, fbinRawMin, fbinRawMax, "Exogam/Cal");
    	name = Form("ExogamTimeCal_Clover%d_ECC%d_GOCCE%d", i+1, j+1, k+1);
    	AddHisto1D(name, name, fnbinsRaw, fbinRawMin, fbinRawMax, "Exogam/Cal");
    }
   }
  } // end loop on number of detectors
}

////////////////////////////////////////////////////////////////////////////////
void TExogamSpectra::InitPhysicsSpectra(){
  string name;
  name = "ExogamEnergyAddBack";
  AddHisto1D(name, name, fnbinsRaw, fbinRawMin, fbinRawMax, "Exogam/DC");

}



////////////////////////////////////////////////////////////////////////////////
void TExogamSpectra::FillRawSpectra(TExogamData* RawData){
  string name;
  string family;

  // Energy and Time RAw 
  for (unsigned int i = 0; i < RawData->GetECCEMult(); i++) {
    name = Form("ExogamEnergyRaw_Clover%d_ECC%d", RawData->GetECCEClover(i)+1,RawData->GetECCECristal(i)+1);
    family = "Exogam/RAW";

    GetHisto(family,name)
      -> Fill(RawData->GetECCEEnergy(i));

    name = Form("ExogamTimeRaw_Clover%d_ECC%d", RawData->GetECCTClover(i)+1,RawData->GetECCTCristal(i)+1);
    family = "Exogam/RAW";

    GetHisto(family,name)
      -> Fill(RawData->GetECCTTime(i));
    
    name = Form("ExogamEnergyRaw_Clover%d_ECC%d_GOCCE%d", RawData->GetECCEClover(i)+1,RawData->GetECCECristal(i)+1,RawData->GetGOCCEESegment(i)+1);
    family = "Exogam/RAW";

    GetHisto(family,name)
      -> Fill(RawData->GetGOCCEEEnergy(i));
    }


}

////////////////////////////////////////////////////////////////////////////////
void TExogamSpectra::FillPreTreatedSpectra(TExogamData* PreTreatedData){
  string name ;
  string family;
  // Energy and Time RAw 
  for (unsigned int i = 0; i < PreTreatedData->GetECCEMult(); i++) {
    name = Form("ExogamEnergyCal_Clover%d_ECC%d", PreTreatedData->GetECCEClover(i)+1,PreTreatedData->GetECCECristal(i)+1);
    family = "Exogam/Cal";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetECCEEnergy(i));

    name = Form("ExogamTimeCal_Clover%d_ECC%d", PreTreatedData->GetECCTClover(i)+1,PreTreatedData->GetECCTCristal(i)+1);
    family = "Exogam/Cal";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetECCTTime(i));
    
    name = Form("ExogamEnergyCal_Clover%d_ECC%d_GOCCE%d", PreTreatedData->GetECCEClover(i)+1,PreTreatedData->GetECCECristal(i)+1,PreTreatedData->GetGOCCEESegment(i)+1);
    family = "Exogam/Cal";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetGOCCEEEnergy(i));
    }
}

////////////////////////////////////////////////////////////////////////////////
void TExogamSpectra::FillPhysicsSpectra(TExogamPhysics* Physics){
  string name;
  string family= "Exogam/PHY";
  // Doppler Correct and Add Back
  name = "ExogamEnergyAddBack";
  family = "Exogam/DC";
  for (unsigned int i = 0; i < Physics->DopplerCorrectedEnergy.size(); i++) {
    GetHisto(family,name)
      -> Fill(Physics->DopplerCorrectedEnergy[i]);
  }

}
////////////////////////////////////////////////////////////////////////////////
TH1* TExogamSpectra::AddHisto1D(string name, string title, Int_t nbinsx, Double_t xlow, Double_t xup, string family){
  // create histo
  TH1 *hist = new TH1D(name.c_str(), title.c_str(), nbinsx, xlow, xup);

  vector<string> index ;
  index.push_back(family);
  index.push_back(name);

  // fill map
  fMapHisto[index]=hist;

  return hist;
}

////////////////////////////////////////////////////////////////////////////////
TH1* TExogamSpectra::AddHisto2D(string name, string title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, string family){
  // create histo
  TH1 *hist = new TH2D(name.c_str(), title.c_str(), nbinsx, xlow, xup, nbinsy, ylow, yup);

  vector<string> index ;
  index.push_back(family);
  index.push_back(name);

  // fill map
  fMapHisto[index]=hist;

  return hist;
}

////////////////////////////////////////////////////////////////////////////////
TH1* TExogamSpectra::GetHisto(string family, string name){
  vector<string> index ;
  index.push_back(family);
  index.push_back(name);

  // fill map
  return fMapHisto.at(index);
}

////////////////////////////////////////////////////////////////////////////////
void TExogamSpectra::WriteHisto(string filename){
  TFile* f=NULL; 

  if(filename!="VOID"){
    f = new TFile(filename.c_str(),"RECREATE");
  }

  map< vector<string>, TH1* >::iterator it;

  for (it=fMapHisto.begin(); it!=fMapHisto.end(); ++it){
    it->second->Write();
  }

  if(filename!="VOID"){
    f->Close();
    delete f;
  }

}
///////////////////////////////////////////////////////////////////////////////
void TExogamSpectra::CheckSpectra(){
map< vector<string>, TH1* >::iterator it;
  Color_t ok_color = kTeal+9;
  Color_t warning_color = kOrange+8;   warning_color *= 1;
  Color_t bad_color = kRed;            bad_color *= 1;

  for (it=fMapHisto.begin(); it!=fMapHisto.end(); ++it){
    it->second->SetLineColor(ok_color);
    it->second->SetFillStyle(1001);
    it->second->SetFillColor(ok_color);
  }

}
