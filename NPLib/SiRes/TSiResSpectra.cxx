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
 *  This class holds all the online spectra needed for SiRes              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + first version (not complete yet)                                     *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// STL
#include <iostream>
#include <cstdlib>
#include <stdexcept>

// NPL
#include "TSiResSpectra.h"
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
TSiResSpectra::TSiResSpectra()
{
  fNumberDetector =  0;
}



////////////////////////////////////////////////////////////////////////////////
TSiResSpectra::TSiResSpectra(unsigned int NumberOfDetectors)
{
  if(NPOptionManager::getInstance()->GetVerboseLevel()>0)
    cout << "************************************************" << endl
      << "TSiResSpectra : Initalising control spectra for " 
      << NumberOfDetectors << " detectorss" << endl
      << "************************************************" << endl ;

  fNumberDetector = NumberOfDetectors;

  InitRawSpectra();
  InitPreTreatedSpectra();
  InitPhysicsSpectra();
}




////////////////////////////////////////////////////////////////////////////////
TSiResSpectra::~TSiResSpectra(){
}



////////////////////////////////////////////////////////////////////////////////
void TSiResSpectra::InitRawSpectra()
{
   string name;
   int NbrBin = 512;
   int MinBin = 0;
   int MaxBin = 16384;
   for(unsigned int j=0; j<fNumberDetector; j++)
   {
	   for (unsigned int i = 0; i < 4; i++) { // loop on channels
	      // SiRes_E_RAW
	      name = Form("SiRes_%d_%d_E_RAW", j+1, i+1);
	      AddHisto1D(name, name, NbrBin, MinBin, MaxBin, "SiRes/RAW/ENERGY");
	   } // end loop on number of detectors

	   // SiRes_E_RAW
	   name = Form("SiRes_%d_E_RAW",j+1);
	   AddHisto2D(name, name, fNumberDetector, 1, fNumberDetector+1, NbrBin, MinBin, MaxBin, "SiRes/RAW/ENERGY");

	   // SiRes_E_RAW
	   name = Form("SiRes_%d_EBack_RAW", j+1);
	   AddHisto1D(name, name, NbrBin, MinBin, MaxBin, "SiRes/RAW/ENERGY");

	   // SiRes_T_RAW
	   name = Form("SiRes_%d_T_RAW",j+1);
	   AddHisto1D(name, name, NbrBin, MinBin, MaxBin, "SiRes/RAW/TIME");

	   // E_RAW_MULT
	   name = Form("SiRes_%d_E_RAW_MULT", j+1);
	   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "SiRes/RAW/MULT");

	   // T_RAW_MULT
	   /*name = Form("SiRes_%d_T_RAW_MULT",j+1);
	   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "SiRes/RAW/MULT");*/
   }
   /*// SiRes_HIT_E_RAW
   name = "SiRes_HITPATTERN_E_RAW";
   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "SiRes/RAW/ENERGY");

   // SiRes_HIT_T_RAW
   name = "SiRes_HITPATTERN_T_RAW";
   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "SiRes/RAW/TIME");*/
}



////////////////////////////////////////////////////////////////////////////////
void TSiResSpectra::InitPreTreatedSpectra()
{
   string name;
   int NbrBin  = 512;
   int MinBinE = 0;
   int MaxBinE = 10;    // MeV
   int MinBinT = 0;
   int MaxBinT = 1;     // us 
   for (unsigned int j = 0; j < fNumberDetector; j++) 
   { 
	   for (unsigned int i = 0; i < 4; i++) { // loop on number of detectors
	      // SiRes_E_CAL
	      name = Form("SiRes_%d_%d_E_CAL", j+1, i+1);
	      AddHisto1D(name, name, NbrBin, MinBinE, MaxBinE, "SiRes/CAL/ENERGY");

	   } // end loop on number of detectors

	   // SiRes_E_CAL
	   name = Form("SiRes_%d_EBack_CAL", j+1);
	   AddHisto1D(name, name, NbrBin, MinBinE, MaxBinE, "SiRes/CAL/ENERGY");

	   // SiRes_T_CAL
	   name = Form("SiRes_%d_T_CAL",j+1);
	   AddHisto1D(name, name, NbrBin, MinBinT, MaxBinT, "SiRes/CAL/TIME");

	   // E_CAL_MULT
	   name = Form("SiRes_%d_E_CAL_MULT", j+1);
	   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "SiRes/CAL/MULT");

	   // T_CAL_MULT
	   /*name = Form("SiRes_%d_T_CAL_MULT", j+1);
	   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "SiRes/CAL/MULT");*/
   }
   /*// SiRes_HIT_E_CAL
   name = "SiRes_HITPATTERN_E_CAL";
   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "SiRes/CAL/ENERGY");

   // SiRes_HIT_T_CAL
   name = "SiRes_HITPATTERN_T_CAL";
   AddHisto1D(name, name, fNumberDetector, 1, fNumberDetector+1, "SiRes/CAL/TIME");*/


}



////////////////////////////////////////////////////////////////////////////////
void TSiResSpectra::InitPhysicsSpectra()
{
   string name;

   int NbrBin  = 512;
   int MinBinE = 0;
   int MaxBinE = 10;
   int MinBinT = 0;
   int MaxBinT = 1;
   for (unsigned int j = 0; j < fNumberDetector; j++) 
   { 
	   // Energy-Time Correlation
	   for (unsigned int i = 0 ; i < 4 ; i++) { // loop on number of detectors
	      name = Form("SiRes_%d_%d_E_TOF", j+1, i+1);
	      AddHisto2D(name, name, NbrBin, MinBinE, MaxBinE, NbrBin, MinBinT, MaxBinT, "SiRes/PHY"); 
	   }

	   // EBack-TOF:
	   name = Form("SiRes_%d_EBack_TOF", j+1);
	   AddHisto2D(name, name, NbrBin, MinBinE, MaxBinE, NbrBin, MinBinT, MaxBinT, "SiRes/PHY");

	   // E-TOF:
	   name = Form("SiRes_%d_E_TOF", j+1);
	   AddHisto2D(name, name, NbrBin, MinBinE, MaxBinE, NbrBin, MinBinT, MaxBinT, "SiRes/PHY");
   }
}



////////////////////////////////////////////////////////////////////////////////
void TSiResSpectra::FillRawSpectra(TSiResData* RawData)
{
   string name;
   string family;
   for (unsigned int j = 0; j < fNumberDetector; j++) 
   { 
  
	   // E_RAW
	   for (unsigned int i = 0; i < RawData->GetEnergyMult(); i++) {
	      name   = Form("SiRes_%d_%d_E_RAW", j+1, RawData->GetEChannelNumber(i));
	      family = "SiRes/RAW/ENERGY";
	      GetHisto(family,name) -> Fill(RawData->GetEEnergy(i));
	   }


	   // SiRes_E_RAW
	   name = Form("SiRes_%d_E_RAW",j+1);
	   family = "SiRes/RAW/ENERGY";
	   for (unsigned int i = 0; i < RawData->GetEnergyMult(); i++) {
	      GetHisto(family,name) -> Fill(RawData->GetEChannelNumber(i), RawData->GetEEnergy(i));
	   }

	   // SiRes_E_RAW
	   name = Form("SiRes_%d_EBack_RAW", j+1);
	   family = "SiRes/RAW/ENERGY";
	   GetHisto(family,name) -> Fill(RawData->GetEEnergyBack(j));

	   // SiRes_T_RAW
	   name = Form("SiRes_%d_T_RAW", j+1);
	   family = "SiRes/RAW/TIME";
	   GetHisto(family,name) -> Fill(RawData->GetTTime(j));
	   

	   // SiRes_HIT_E_RAW
	   /*name = "SiRes_HITPATTERN_E_RAW";
	   family = "SiRes/RAW/ENERGY";
	   for (unsigned int i = 0; i < RawData->GetEnergyMult(); i++) {
	      GetHisto(family,name) -> Fill(RawData->GetENumber(i));
	   }

	   // SiRes_HIT_T_RAW
	   name = "SiRes_HITPATTERN_T_RAW";
	   family = "SiRes/RAW/TIME";
	   for (unsigned int i = 0; i < RawData->GetTimeMult(); i++) {
	      GetHisto(family,name) -> Fill(RawData->GetTNumber(i));
	   }*/

	   // E_RAW_MULT
	   name = Form("SiRes_%d_E_RAW_MULT", j+1);
	   family = "SiRes/RAW/MULT";
	   GetHisto(family,name) -> Fill(RawData->GetEnergyMult());

	   // T_RAW_MULT
	   /*name = Form("SiRes_%d_T_RAW_MULT", j+1);
	   family = "SiRes/RAW/MULT";
	   GetHisto(family,name) -> Fill(RawData->GetTimeMult());*/
   }

}



////////////////////////////////////////////////////////////////////////////////
void TSiResSpectra::FillPreTreatedSpectra(TSiResData* PreTreatedData)
{
   string name;
   string family;
  
   // E_CAL
   for(unsigned int j=0; j<fNumberDetector; j++)
   {
	   for (unsigned int i = 0; i < PreTreatedData->GetEnergyMult(); i++) {
	      name   = Form("SiRes_%d_%d_E_CAL", j+1, PreTreatedData->GetEChannelNumber(i));
	      family = "SiRes/CAL/ENERGY";
	      GetHisto(family,name) -> Fill(PreTreatedData->GetEEnergy(i));
	   }

	   // T_CAL
	   name   = Form("SiRes_%d_T_CAL", j+1);
	   family = "SiRes/CAL/TIME";
	   GetHisto(family,name) -> Fill(PreTreatedData->GetTTime(j));


	   // SiRes_E_CAL
	   name = Form("SiRes_%d_EBack_CAL", j+1);
	   family = "SiRes/CAL/ENERGY";
	   GetHisto(family,name) -> Fill(PreTreatedData->GetEEnergyBack(j));
	   

	   /*
	   // SiRes_HIT_E_CAL
	   name = "SiRes_HITPATTERN_E_CAL";
	   family = "SiRes/CAL/ENERGY";
	   for (unsigned int i = 0; i < PreTreatedData->GetEnergyMult(); i++) {
	      GetHisto(family,name) -> Fill(PreTreatedData->GetENumber(i));
	   }

	   // SiRes_HIT_T_CAL
	   name = "SiRes_HITPATTERN_T_CAL";
	   family = "SiRes/CAL/TIME";
	   for (unsigned int i = 0; i < PreTreatedData->GetTimeMult(); i++) {
	      GetHisto(family,name) -> Fill(PreTreatedData->GetTNumber(i));
	   }*/

	   // E_CAL_MULT
	   name = Form("SiRes_%d_E_CAL_MULT", j+1);
	   family = "SiRes/CAL/MULT";
	   GetHisto(family,name) -> Fill(PreTreatedData->GetEnergyMult());

	   // T_CAL_MULT
	   /*name = Form("SiRes_%d_T_CAL_MULT", j+1);
	   family = "SiRes/CAL/MULT";
	   GetHisto(family,name) -> Fill(PreTreatedData->GetTimeMult());*/
   }
}



////////////////////////////////////////////////////////////////////////////////
void TSiResSpectra::FillPhysicsSpectra(TSiResPhysics* Physics)
{
   string name;
   string family = "SiRes/PHY";

   for (unsigned int j = 0; j < fNumberDetector; j++) 
   { 

	   // Energy-Time Correlation
	   for (unsigned int i = 0 ; i < Physics->DetectorNumber.size(); i++) { // loop on number of detectors
	      name = Form("SiRes_%d_E_TOF", i+1);
	      GetHisto(family,name) -> Fill(Physics->Time[j], Physics->Energy[i]);
	   }
	   
	   // E-TOF:
	   name =Form("SiRes_%d_EBack_TOF", j+1);
	   GetHisto(family,name) -> Fill(Physics->Time[j], Physics->EnergyBack[j]);

	   // E-TOF:
	   name = Form("SiRes_%d_E_TOF", j+1);
	   for (unsigned int i = 0 ; i < Physics->DetectorNumber.size(); i++) { // loop on number of detectors
	      GetHisto(family,name) -> Fill(Physics->Time[j], Physics->Energy[i]);
	   }
   }
}



////////////////////////////////////////////////////////////////////////////////
TH1* TSiResSpectra::AddHisto1D(string name, string title, Int_t nbinsx, Double_t xlow, Double_t xup, string family){
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
TH1* TSiResSpectra::AddHisto2D(string name, string title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, string family){
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
TH1* TSiResSpectra::GetHisto(string& family, string& name){
  vector<string> index;
  index.push_back(family);
  index.push_back(name);
  TH1* histo ; 
  
  try{
    histo = fMapHisto.at(index); 
  }

  catch(const std::out_of_range& oor){
  cout << "ERROR : the folowing Histo has been requested by TSiResSpectra and does not exist: family:" << family << " name: "  << name << endl ;
  exit(1);
  }

  return histo;
}



////////////////////////////////////////////////////////////////////////////////
void TSiResSpectra::WriteHisto(string filename)
{
   TFile *f = NULL; 

   if (filename != "VOID") {
      f = new TFile(filename.c_str(), "RECREATE");
   }

   map< vector<string>, TH1* >::iterator it;
   for (it=fMapHisto.begin(); it!=fMapHisto.end(); ++it) {
      it->second->Write();
   }

   if (filename != "VOID") {
      f->Close();
      delete f;
   }
}



///////////////////////////////////////////////////////////////////////////////
void TSiResSpectra::CheckSpectra()
{
   map< vector<string>, TH1* >::iterator it;
   Color_t ok_color      = kTeal+9;
   Color_t warning_color = kOrange+8;  warning_color *= 1;
   Color_t bad_color     = kRed;       bad_color     *= 1;

   for (it=fMapHisto.begin(); it!=fMapHisto.end(); ++it)
   {
      it->second->SetLineColor(ok_color);
      it->second->SetFillStyle(1001);
      it->second->SetFillColor(ok_color);
   }
}
