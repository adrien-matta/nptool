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
TCATSSpectra::TCATSSpectra()
{
   fNumberOfCats = 0;
   fStripsNumber = 28;
}



////////////////////////////////////////////////////////////////////////////////
TCATSSpectra::TCATSSpectra(unsigned int NumberOfCats)
{
   if (NPOptionManager::getInstance()->GetVerboseLevel() > 0) {
      cout << "************************************************" << endl;
      cout << "TCATSSpectra: Initalising control spectra for " << NumberOfCats << " detectors" << endl; 
      cout << "************************************************" << endl;
   }

   fNumberOfCats = NumberOfCats;
   fStripsNumber = 28;

   InitRawSpectra();
   InitPreTreatedSpectra();
   InitPhysicsSpectra();
}




////////////////////////////////////////////////////////////////////////////////
TCATSSpectra::~TCATSSpectra()
{
}



////////////////////////////////////////////////////////////////////////////////
void TCATSSpectra::InitRawSpectra()
{
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
      AddHisto1D(name, name, fStripsNumber, 1, fStripsNumber+1, "CATS/RAW/CTRL");

      // STRY_MULT
      name = Form("CATS%d_STRY_MULT", i+1);
      AddHisto1D(name, name, fStripsNumber, 1, fStripsNumber+1, "CATS/RAW/CTRL");
   } // end loop on number of cats
}



////////////////////////////////////////////////////////////////////////////////
void TCATSSpectra::InitPreTreatedSpectra()
{
   TString name;

   for (unsigned int i = 0; i < fNumberOfCats; ++i) {   // loop on number of cats
      // CATS_STRX_Q_CAL
      name = Form("CATS%d_STRX_Q_CAL", i+1);
      AddHisto2D(name, name, fStripsNumber, 1, fStripsNumber+1, 512, 0, 16384, "CATS/CAL/STRQ");

      // CATS_STRY_Q_CAL
      name = Form("CATS%d_STRY_Q_CAL", i+1);
      AddHisto2D(name, name, fStripsNumber, 1, fStripsNumber+1, 512, 0, 16384, "CATS/CAL/STRQ");
   } // end loop on number of cats
}



////////////////////////////////////////////////////////////////////////////////
void TCATSSpectra::InitPhysicsSpectra()
{
/*   TString name;
   // X-Y Impact Matrix
   name = "HYB_IMPACT_MATRIX";
   AddHisto2D(name, name, 500, -150, 150, 500, -150, 150, "TIARA/HYBALL/PHY");

   // Kinematic line
   name = "HYB_THETA_E";
   AddHisto2D(name, name, 360, 0, 180, 500, 0, 50, "TIARA/HYBALL/PHY");

   // Ring v.s. Sector Energy Correlation
   name = "HYB_XY_COR";
   AddHisto2D(name, name, 500, 0, 50, 500, 0, 50, "TIARA/HYBALL/PHY"); */
}



////////////////////////////////////////////////////////////////////////////////
void TCATSSpectra::FillRawSpectra(TCATSData* RawData)
{
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
      family = "CATS/RAW/CTRL";
      GetHisto(family,name) -> Fill(myMULT[i]);
   }

   // STRY_MULT
   for (unsigned int i = 0; i < fNumberOfCats; i++) myMULT[i] = 0;
   for (unsigned int i = 0; i < RawData->GetCATSMultY(); i++) myMULT[RawData->GetCATSDetY(i)-1] += 1;

   for (unsigned int i = 0; i < fNumberOfCats; i++) {
      name   = Form("CATS%d_STRY_MULT", i+1);
      family = "CATS/RAW/CTRL";
      GetHisto(family,name) -> Fill(myMULT[i]);
   }
}



////////////////////////////////////////////////////////////////////////////////
void TCATSSpectra::FillPreTreatedSpectra(TCATSData* PreTreatedData)
{
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
}



////////////////////////////////////////////////////////////////////////////////
void TCATSSpectra::FillPhysicsSpectra(TCATSPhysics* Physics)
{
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
TH1* TCATSSpectra::AddHisto1D(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, TString family)
{
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
TH1* TCATSSpectra::AddHisto2D(TString name, TString title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, TString family)
{
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
TH1* TCATSSpectra::GetHisto(TString family, TString name)
{
   vector<TString> index;
   index.push_back(family);
   index.push_back(name);

   return fMapHisto.at(index);
}



////////////////////////////////////////////////////////////////////////////////
void TCATSSpectra::WriteHisto(TString filename)
{
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
