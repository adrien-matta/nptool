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
#include "TCATSSpectra.h"
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
TCATSSpectra::TCATSSpectra(){
  fNumberOfCats = 0;
  fStripsNumber = 28;

  // An histo of size fEventLoopSize is reset every fEventLoopSize to monitor the
  fEventLoopSize = 5000;
  fEventLoopStep = 100;
  fEventLoopIndex = 0;
}



////////////////////////////////////////////////////////////////////////////////
TCATSSpectra::TCATSSpectra(unsigned int NumberOfCats){
  if (NPOptionManager::getInstance()->GetVerboseLevel() > 0) {
    cout << "************************************************" << endl;
    cout << "TCATSSpectra: Initalising control spectra for " << NumberOfCats << " detectors" << endl; 
    cout << "************************************************" << endl;
  }

  fNumberOfCats = NumberOfCats;
  fStripsNumber = 28;
  // An histo of size fEventLoopSize is reset every fEventLoopSize to monitor the
  fEventLoopSize = 10000;
  fEventLoopStep = 1;
  fEventLoopIndex = 0;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}




////////////////////////////////////////////////////////////////////////////////
TCATSSpectra::~TCATSSpectra(){
}



////////////////////////////////////////////////////////////////////////////////
void TCATSSpectra::InitRawSpectra(){
  TString name;

  for (unsigned int i = 0; i < fNumberOfCats; ++i) {   // loop on number of cats
    // CATS_STRX_Q_RAW
    name = Form("CATS%d_STRX_Q_RAW", i+1);
    AddHisto2D(name, name, fStripsNumber, 1, fStripsNumber+1, 512, 0, 16384, "CATS/RAW/STRQ");

    // CATS_STRY_Q_RAW
    name = Form("CATS%d_STRY_Q_RAW", i+1);
    AddHisto2D(name, name, fStripsNumber, 1, fStripsNumber+1, 512, 0, 16384, "CATS/RAW/STRQ");

    // STRX_MULT
    name = Form("CATS%d_STRX_MULT", i+1);
    AddHisto1D(name, name, fStripsNumber, 1, fStripsNumber+1, "CATS/RAW/MULT");

    // STRY_MULT
    name = Form("CATS%d_STRY_MULT", i+1);
    AddHisto1D(name, name, fStripsNumber, 1, fStripsNumber+1, "CATS/RAW/MULT");
  } // end loop on number of cats
}



////////////////////////////////////////////////////////////////////////////////
void TCATSSpectra::InitPreTreatedSpectra(){
  TString family = "CATS/CAL/STRQ" ;
  TString name;

  for (unsigned int i = 0; i < fNumberOfCats; ++i) {   // loop on number of cats
    // CATS_STRX_Q_CAL
    name = Form("CATS%d_STRX_Q_CAL", i+1);
    AddHisto2D(name, name, fStripsNumber, 1, fStripsNumber+1, 512, 0, 16384, family);

    // CATS_STRY_Q_CAL
    name = Form("CATS%d_STRY_Q_CAL", i+1);
    AddHisto2D(name, name, fStripsNumber, 1, fStripsNumber+1, 512, 0, 16384, family);
   // end loop on number of cats

    // STRX_MULT
    name = Form("CATS%d_STRX_CAL_MULT", i+1);
    AddHisto1D(name, name, fStripsNumber, 1, fStripsNumber+1, "CATS/CAL/MULT");

    // STRY_MULT
    name = Form("CATS%d_STRY_CAL_MULT", i+1);
    AddHisto1D(name, name, fStripsNumber, 1, fStripsNumber+1, "CATS/CAL/MULT");
  } // end loop on number of cats
}

////////////////////////////////////////////////////////////////////////////////
void TCATSSpectra::InitPhysicsSpectra(){
  TString family = "CATS/PHY/QSUM" ;
  TString name;

  for (unsigned int i = 0; i < fNumberOfCats; ++i) {   // loop on number of cats
    name = Form("CATS%d_QSUM_STRMAX_X_CAL", i+1);
    AddHisto2D(name, name, fStripsNumber, 1, fStripsNumber+1, 512, 0, 16384, family);

    name = Form("CATS%d_QSUM_STRMAX_Y_CAL", i+1);
    AddHisto2D(name, name, fStripsNumber, 1, fStripsNumber+1, 512, 0, 16384, family);
  } // end loop on number of cats

  family = "CATS/PHY/CTRL";
  for (unsigned int i = 0; i < fNumberOfCats; ++i) {   // loop on number of cats
    name = Form("CATS%d_QMEAN_TIME", i+1);
    AddHisto1D(name, name, fEventLoopSize,0,fEventLoopSize,family); 
    fEventLoopQSum.push_back(0);
  } // end loop on number of cats

  family = "CATS/PHY/POS";
  for (unsigned int i = 0; i < fNumberOfCats; ++i) {   // loop on number of cats
    name = Form("CATS%d_POS", i+1);
    AddHisto2D(name, name,100,-50,50,100,-50,50,family); 
  } // end loop on number of cats

  name = "TARGET_POS";
  AddHisto2D(name, name,100,-50,50,100,-50,50,family); 


}

////////////////////////////////////////////////////////////////////////////////
void TCATSSpectra::FillRawSpectra(TCATSData* RawData){
  TString name;
  TString family;

  // CATS_STRX_Q_RAW
  for (unsigned int i = 0; i < RawData->GetCATSMultX(); ++i) {   // loop on vector
    family = "CATS/RAW/STRQ";
    name   = Form("CATS%d_STRX_Q_RAW", RawData->GetCATSDetX(i));
    GetHisto(family, name) -> Fill(RawData->GetCATSStripX(i), RawData->GetCATSChargeX(i)); 
  } // end loop on vector

  // CATS_STRY_Q_RAW
  for (unsigned int i = 0; i < RawData->GetCATSMultY(); ++i) {   // loop on vector
    family = "CATS/RAW/STRQ";
    name   = Form("CATS%d_STRY_Q_RAW", RawData->GetCATSDetY(i));
    GetHisto(family, name) -> Fill(RawData->GetCATSStripY(i), RawData->GetCATSChargeY(i)); 
  } // end loop on vector

  // STRX_MULT
  int myMULT[fStripsNumber];
  for (unsigned int i = 0; i < fNumberOfCats; i++) myMULT[i] = 0;
  for (unsigned int i = 0; i < RawData->GetCATSMultX(); i++) myMULT[RawData->GetCATSDetX(i)-1] += 1;

  for (unsigned int i = 0; i < fNumberOfCats; i++) {
    name   = Form("CATS%d_STRX_MULT", i+1);
    family = "CATS/RAW/MULT";
    GetHisto(family,name) -> Fill(myMULT[i]);
  }

  // STRY_MULT
  for (unsigned int i = 0; i < fNumberOfCats; i++) myMULT[i] = 0;
  for (unsigned int i = 0; i < RawData->GetCATSMultY(); i++) myMULT[RawData->GetCATSDetY(i)-1] += 1;

  for (unsigned int i = 0; i < fNumberOfCats; i++) {
    name   = Form("CATS%d_STRY_MULT", i+1);
    family = "CATS/RAW/MULT";
    GetHisto(family,name) -> Fill(myMULT[i]);
  }
}



////////////////////////////////////////////////////////////////////////////////
void TCATSSpectra::FillPreTreatedSpectra(TCATSData* PreTreatedData){
  TString name;
  TString family;

  // CATS_STRX_Q_CAL
  for (unsigned int i = 0; i < PreTreatedData->GetCATSMultX(); ++i) {   // loop on vector
    family = "CATS/CAL/STRQ";
    name   = Form("CATS%d_STRX_Q_CAL", PreTreatedData->GetCATSDetX(i));
    GetHisto(family,name) -> Fill(PreTreatedData->GetCATSStripX(i), PreTreatedData->GetCATSChargeX(i)); 
  } // end loop on vector

  // CATS_STRY_Q_CAL
  for (unsigned int i = 0; i < PreTreatedData->GetCATSMultY(); ++i) {   // loop on vector
    family = "CATS/CAL/STRQ";
    name   = Form("CATS%d_STRY_Q_CAL", PreTreatedData->GetCATSDetY(i));
    GetHisto(family,name) -> Fill(PreTreatedData->GetCATSStripY(i), PreTreatedData->GetCATSChargeY(i)); 
  } // end loop on vector

  // STRX_MULT
  int myMULT[fStripsNumber];
  for (unsigned int i = 0; i < fNumberOfCats; i++) myMULT[i] = 0;
  for (unsigned int i = 0; i < PreTreatedData->GetCATSMultX(); i++) myMULT[PreTreatedData->GetCATSDetX(i)-1] += 1;

  for (unsigned int i = 0; i < fNumberOfCats; i++) {
    name   = Form("CATS%d_STRX_CAL_MULT", i+1);
    family = "CATS/CAL/MULT";
    GetHisto(family,name) -> Fill(myMULT[i]);
  }

  // STRY_MULT
  for (unsigned int i = 0; i < fNumberOfCats; i++) myMULT[i] = 0;
  for (unsigned int i = 0; i < PreTreatedData->GetCATSMultY(); i++) myMULT[PreTreatedData->GetCATSDetY(i)-1] += 1;

  for (unsigned int i = 0; i < fNumberOfCats; i++) {
    name   = Form("CATS%d_STRY_CAL_MULT", i+1);
    family = "CATS/CAL/MULT";
    GetHisto(family,name) -> Fill(myMULT[i]);
  }

}

////////////////////////////////////////////////////////////////////////////////
void TCATSSpectra::FillPhysicsSpectra(TCATSPhysics* Physics){
  TString name,family;
  // CATS_STRX_Q_CAL
  for (unsigned int i = 0; i < Physics->DetNumberX.size() ; ++i) {   // loop on vector
    family="CATS/PHY/QSUM";
    name   = Form("CATS%d_QSUM_STRMAX_X_CAL", Physics->DetNumberX[i]);
    GetHisto(family,name) -> Fill(Physics->StripMaxX[i],Physics->QsumX[i]); 
    name   = Form("CATS%d_QSUM_STRMAX_Y_CAL", Physics->DetNumberX[i]);
    GetHisto(family,name) -> Fill(Physics->StripMaxY[i],Physics->QsumY[i]); 
    
  }
  // An histo of size fEventLoopSize is reset every fEventLoopSize to monitor the
  // Keep Track of how many event elapsed
  
  if(Physics->StripMaxX.size()==fNumberOfCats){
    fEventLoopIndex++;
    family = "CATS/PHY/CTRL";

    for (unsigned int i = 0; i < Physics->StripMaxX.size(); ++i) {
      fEventLoopQSum[i]+=Physics->QsumX[i]/1000000.;
      name = Form("CATS%d_QMEAN_TIME",i+1);

      GetHisto(family,name) ->SetBinContent(fEventLoopIndex/fEventLoopStep,fEventLoopQSum[i]/fEventLoopStep);
    }
  }
 
  // Reset the mean every bin 
  if(fEventLoopIndex%fEventLoopStep>fEventLoopStep)
    for (unsigned int i = 0; i < fNumberOfCats; ++i)  
      fEventLoopQSum[i]=0;

  // Restart histo
  if(fEventLoopIndex > fEventLoopSize)
      fEventLoopIndex = 0 ;

 if(Physics->StripMaxX.size()==fNumberOfCats){
    for (unsigned int i = 0; i < Physics->StripMaxX.size(); ++i) {
      family = "CATS/PHY/POS";
      name = Form("CATS%d_POS", i+1);
      GetHisto(family,name) -> Fill(Physics->PositionX[i],Physics->PositionY[i]);
    }

    name = "TARGET_POS";
    GetHisto(family,name)->Fill(Physics->PositionOnTargetX,Physics->PositionOnTargetY);
}

}



////////////////////////////////////////////////////////////////////////////////
TH1* TCATSSpectra::AddHisto1D(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, TString family){
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
TH1* TCATSSpectra::AddHisto2D(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString family){
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
TH1* TCATSSpectra::GetHisto(TString family, TString name){
  vector<TString> index;
  index.push_back(family);
  index.push_back(name);

  return fMapHisto.at(index);
}



////////////////////////////////////////////////////////////////////////////////
void TCATSSpectra::WriteHisto(TString filename){
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
