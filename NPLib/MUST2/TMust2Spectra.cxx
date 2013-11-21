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
 *  This class holds all the online spectra needed for Must2                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + first version (not complete yet)                                     *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include "TMust2Spectra.h"

#include "TString.h"


ClassImp(TMust2Spectra)
  ////////////////////////////////////////////////////////////////////////////////
  TMust2Spectra::TMust2Spectra(){
    fNumberOfTelescope = 0;
    fStripX=128;
    fStripY=128;
    fPadSili=16;
    fCrystalCsI=16;
  }

////////////////////////////////////////////////////////////////////////////////
TMust2Spectra::TMust2Spectra(unsigned int NumberOfTelescope){
  fNumberOfTelescope = NumberOfTelescope;
  fStripX=128;
  fStripY=128;
  fPadSili=16;
  fCrystalCsI=16;
}


////////////////////////////////////////////////////////////////////////////////
TMust2Spectra::~TMust2Spectra(){
}

////////////////////////////////////////////////////////////////////////////////
void TMust2Spectra::InitRawSpectra(){
  TString name;
  for (Int_t i = 0; i < fNumberOfTelescope; i++) { // loop on number of detectors
    // STRX_E_RAW
    name = Form("MM%d_STRX_E_RAW", i+1);
    AddTH2(name, name, fStripX, 1, fStripX+1, 512, 0, 8192, "MUST2/RAW/STRXE");

    // STRY_E_RAW
    name = Form("MM%d_STRY_E_RAW", i+1);
    AddTH2(name, name, fStripY, 1, fStripY+1, 512, 0, 8192, "MUST2/RAW/STRYE");

    // STRX_T_RAW
    name = Form("MM%d_STRX_T_RAW", i+1);
    AddTH2(name, name, fStripX, 1, fStripX+1, 512, 0, 8192, "MUST2/RAW/STRXT");

    // STRY_T_RAW
    name = Form("MM%d_STRY_T_RAW", i+1);
    AddTH2(name, name, fStripY, 1, fStripY+1, 512, 0, 8192, "MUST2/RAW/STRYT");

    // SILI_E_RAW
    name = Form("MM%d_SILI_E_RAW", i+1);
    AddTH2(name, name, fPadSili, 1, fPadSili+1, 512, 0, 8192, "MUST2/RAW/SILIE");

    // SILI_T_RAW
    name = Form("MM%d_SILI_T_RAW", i+1);
    AddTH2(name, name, fPadSili, 1, fPadSili+1, 512, 0, 8192, "MUST2/RAW/SILIT");

    // CSI_E_RAW
    name = Form("MM%d_CSI_E_RAW", i+1);
    AddTH2(name, name, fCrystalCsI, 1, fCrystalCsI+1, 512, 0, 8192, "MUST2/RAW/CSIE");

    // CSI_T_RAW
    name = Form("MM%d_CSI_T_RAW", i+1);
    AddTH2(name, name, fCrystalCsI, 1, fCrystalCsI+1, 512, 0, 8192, "MUST2/RAW/CSIT");

    // STRX_MULT
    name = Form("MM%d_STRX_MULT", i+1);
    AddTH1(name, name, fStripX, 1, fStripX+1, "MUST2/RAW/MULT");

    // STRY_MULT
    name = Form("MM%d_STRY_MULT", i+1);
    AddTH1(name, name, fStripX, 1, fStripX+1, "MUST2/RAW/MULT");

    // SILI_MULT
    name = Form("MM%d_SILI_MULT", i+1);
    AddTH1(name, name, fPadSili, 1, fPadSili+1, "MUST2/RAW/MULT");

    // CSI_MULT
    name = Form("MM%d_CSI_MULT", i+1);
    AddTH1(name, name, fCrystalCsI, 1, fCrystalCsI+1, "MUST2/RAW/MULT");
  } // end loop on number of detectors
}

////////////////////////////////////////////////////////////////////////////////
void TMust2Spectra::InitPreTreatedSpectra()
{
  TString name;
  for (Int_t i = 0; i < fNumberOfTelescope; i++) { // loop on number of detectors
    // STRX_E_CAL
    name = Form("MM%d_STRX_E_CAL", i+1);
    AddTH2(name, name, fStripX, 1, fStripX+1, 500, 0, 50, "MUST2/CAL/STRXE");

    // STRY_E_CAL
    name = Form("MM%d_STRY_E_CAL", i+1);
    AddTH2(name, name, fStripY, 1, fStripY+1, 500, 0, 50, "MUST2/CAL/STRYE");

    // STRX_T_CAL
    name = Form("MM%d_STRX_T_CAL", i+1);
    AddTH2(name, name, fStripX, 1, fStripX+1, 500, 0, 500, "MUST2/CAL/STRXT");

    // STRY_T_CAL
    name = Form("MM%d_STRY_T_CAL", i+1);
    AddTH2(name, name, fStripY, 1, fStripY+1, 500, 0, 500, "MUST2/CAL/STRYT");

    // SILI_E_CAL
    name = Form("MM%d_SILI_E_CAL", i+1);
    AddTH2(name, name, fPadSili, 1, fPadSili+1, 500, 0, 50, "MUST2/CAL/SILIE");

    // SILI_T_CAL
    name = Form("MM%d_SILI_T_CAL", i+1);
    AddTH2(name, name, fPadSili, 1, fPadSili+1, 500, 0, 50, "MUST2/CAL/SILIT");

    // CSI_E_CAL
    name = Form("MM%d_CSI_E_CAL", i+1);
    AddTH2(name, name, fCrystalCsI, 1, fCrystalCsI+1, 500, 0, 50, "MUST2/CAL/CSIE");

    // CSI_T_CAL
    name = Form("MM%d_CSI_T_CAL", i+1);
    AddTH2(name, name, fCrystalCsI, 1, fCrystalCsI+1, 500, 0, 50, "MUST2/CAL/CSIT");

    // STRX_MULT
    name = Form("MM%d_STRX_MULT", i+1);
    AddTH1(name, name, fStripX, 1, fStripX+1, "MUST2/CAL/MULT");

    // STRY_MULT
    name = Form("MM%d_STRY_MULT", i+1);
    AddTH1(name, name, fStripX, 1, fStripX+1, "MUST2/CAL/MULT");

    // SILI_MULT
    name = Form("MM%d_SILI_MULT", i+1);
    AddTH1(name, name, fPadSili, 1, fPadSili+1, "MUST2/CAL/MULT");

    // CSI_MULT
    name = Form("MM%d_CSI_MULT", i+1);
    AddTH1(name, name, fCrystalCsI, 1, fCrystalCsI+1, "MUST2/CAL/MULT");

  }  // end loop on number of detectors
}


////////////////////////////////////////////////////////////////////////////////
void TMust2Spectra::FillRawSpectra(TMust2Data* RawData){
  TString name;
  TString family;

  // STRX_E 
  for (Int_t i = 0; i < RawData->GetMMStripXEMult(); i++) {
    name = Form("MM%d_STRX_E_RAW", RawData->GetMMStripXEDetectorNbr(i) );
    family = "MUST2/RAW/STRXE";

    GetHisto(family,name)
      -> Fill(RawData->GetMMStripXEStripNbr(i), 
          RawData->GetMMStripXEEnergy(i) - 8192);
  }

  // STRY_E
  for (Int_t i = 0; i < RawData->GetMMStripYEMult(); i++) {
    name = Form("MM%d_STRY_E_RAW", RawData->GetMMStripYEDetectorNbr(i) );
    family = "MUST2/RAW/STRYE";

    GetHisto(family,name)
      -> Fill(RawData->GetMMStripYEStripNbr(i),
         8192 - RawData->GetMMStripYEEnergy(i));
  }

  // STRX_T
  for (Int_t i = 0; i < RawData->GetMMStripXTMult(); i++) {
    name = Form("MM%d_STRX_T_RAW", RawData->GetMMStripXTDetectorNbr(i) );
    family = "MUST2/RAW/STRXT";

    GetHisto(family,name)
      -> Fill(RawData->GetMMStripXTStripNbr(i),
          RawData->GetMMStripXTTime(i) - 8192);
  }
  // STRY_T
  for (Int_t i = 0; i < RawData->GetMMStripYTMult(); i++) {
    name = Form("MM%d_STRY_T_RAW", RawData->GetMMStripYTDetectorNbr(i) );
    family = "MUST2/RAW/STRYT";

    GetHisto(family,name)
      -> Fill(RawData->GetMMStripYTStripNbr(i),
          8192 - RawData->GetMMStripYTTime(i));
  }

  // SILI_E
  for (Int_t i = 0; i < RawData->GetMMSiLiEMult(); i++) {
    name = Form("MM%d_SILI_E_RAW", RawData->GetMMSiLiEDetectorNbr(i) );
    family = "MUST2/RAW/SILIE";

    GetHisto(family,name)
      -> Fill(RawData->GetMMSiLiEPadNbr(i),
          RawData->GetMMSiLiEEnergy(i));
  }

  // SILI_T
  for (Int_t i = 0; i < RawData->GetMMSiLiTMult(); i++) {
    name = Form("MM%d_SILI_T_RAW", RawData->GetMMSiLiTDetectorNbr(i) );
    family = "MUST2/RAW/SILIT";

    GetHisto(family,name)
      -> Fill(RawData->GetMMSiLiTPadNbr(i),
          RawData->GetMMSiLiTTime(i));
  }

  // CSI_E
  for (Int_t i = 0; i < RawData->GetMMCsIEMult(); i++) {
    name = Form("MM%d_CSI_E_RAW", RawData->GetMMCsIEDetectorNbr(i) );
    family = "MUST2/RAW/CSIE";

    GetHisto(family,name)
      -> Fill(RawData->GetMMCsIECristalNbr(i), 
          RawData->GetMMCsIEEnergy(i));
  }

  // CSI_T
  for (Int_t i = 0; i < RawData->GetMMCsITMult(); i++) {
    name = Form("MM%d_CSI_T_RAW", RawData->GetMMCsITDetectorNbr(i) );
    family = "MUST2/RAW/CSIT";

    GetHisto(family,name)
      -> Fill(RawData->GetMMCsITCristalNbr(i), 
          RawData->GetMMCsITTime(i));
  }

}

////////////////////////////////////////////////////////////////////////////////
void TMust2Spectra::FillPreTreatedSpectra(TMust2Data* PreTreatedData){
  TString name ;
  TString family;
  // STRX_E
  for (Int_t i = 0; i < PreTreatedData->GetMMStripXEMult(); i++) {
    name = Form("MM%d_STRX_E_CAL", PreTreatedData->GetMMStripXEDetectorNbr(i) );
    family = "MUST2/CAL/STRIPXE";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetMMStripXEStripNbr(i), 
          PreTreatedData->GetMMStripXEEnergy(i));
  }
  // STRY_E
  for (Int_t i = 0; i < PreTreatedData->GetMMStripYEMult(); i++) {
    name = Form("MM%d_STRY_E_CAL", PreTreatedData->GetMMStripYEDetectorNbr(i) );
    family = "MUST2/CAL/STRYE";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetMMStripYEStripNbr(i), 
          PreTreatedData->GetMMStripYEEnergy(i));
  }
  // STRX_T
  for (Int_t i = 0; i < PreTreatedData->GetMMStripXTMult(); i++) {
    name = Form("MM%d_STRX_T_CAL", PreTreatedData->GetMMStripXTDetectorNbr(i) );
    family = "MUST2/CAL/STRXT";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetMMStripXTStripNbr(i), 
          PreTreatedData->GetMMStripXTTime(i));
  }
  // STRY_T
  for (Int_t i = 0; i < PreTreatedData->GetMMStripYTMult(); i++) {
   name = Form("MM%d_STRY_T_CAL", PreTreatedData->GetMMStripYTDetectorNbr(i) );
    family = "MUST2/CAL/STRYT";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetMMStripYTStripNbr(i), 
          PreTreatedData->GetMMStripYTTime(i));
  }
  // SILI_E
  for (Int_t i = 0; i < PreTreatedData->GetMMSiLiEMult(); i++) {
    name = Form("MM%d_SILI_E_CAL", PreTreatedData->GetMMSiLiEDetectorNbr(i) );
    family = "MUST2/CAL/SILIET";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetMMSiLiEPadNbr(i), 
          PreTreatedData->GetMMSiLiEEnergy(i));
  }
  // SILI_T
  for (Int_t i = 0; i < PreTreatedData->GetMMSiLiTMult(); i++) {
    name = Form("MM%d_SILI_T_CAL", PreTreatedData->GetMMSiLiTDetectorNbr(i) );
    family = "MUST2/CAL/SILIT";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetMMSiLiTPadNbr(i), 
          PreTreatedData->GetMMSiLiTTime(i));
  }
  // CSI_E
  for (Int_t i = 0; i < PreTreatedData->GetMMCsIEMult(); i++) {
    name = Form("MM%d_CSI_E_CAL", PreTreatedData->GetMMCsIEDetectorNbr(i) );
    family = "MUST2/CAL/CSIE";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetMMCsIECristalNbr(i),   
          PreTreatedData->GetMMCsIEEnergy(i));
  }
  // CSI_T
  for (Int_t i = 0; i < PreTreatedData->GetMMCsITMult(); i++) {
    name = Form("MM%d_CSI_T_CAL", PreTreatedData->GetMMCsITDetectorNbr(i) );
    family = "MUST2/CAL/CSIT";

    GetHisto(family,name)
      -> Fill(PreTreatedData->GetMMCsITCristalNbr(i), 
          PreTreatedData->GetMMCsITTime(i));
  }
}

////////////////////////////////////////////////////////////////////////////////
void TMust2Spectra::FillPhysicsSpectra(TMust2Physics* Physics){

}

////////////////////////////////////////////////////////////////////////////////
TH1* TMust2Spectra::AddTH1(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, TString family){
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
TH1* TMust2Spectra::AddTH2(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString family){
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
TH1* TMust2Spectra::GetHisto(TString family, TString name){
  vector<TString> index ;
  index.push_back(family);
  index.push_back(name);

  // fill map
  return fMapHisto.at(index);
}
