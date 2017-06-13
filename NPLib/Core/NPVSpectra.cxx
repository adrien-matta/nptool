/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/****************************************************************************
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
#include "NPVSpectra.h"
#include "NPOptionManager.h"
#include "NPSpectraServer.h"
#include "RootOutput.h"

// ROOT
#include "TFile.h"
#include "TDirectory.h"
#include "TH2.h"

//STL
#include <iostream>
#include <cstdlib>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
VSpectra::VSpectra(){
  m_name="no_name";
}

////////////////////////////////////////////////////////////////////////////////
TH1* VSpectra::AddHisto1D(string name, string title, Int_t nbinsx, Double_t xlow, Double_t xup, string family){
  // create histo
  TH1 *hist = new TH1D(name.c_str(), title.c_str(), nbinsx, xlow, xup);

  string index= family+"/"+name;

  // fill map
  fMapHisto[index] = hist;

  // Add it to the server
  NPL::SpectraServer::getInstance()->AddSpectra(hist);
 
  return hist;
}

////////////////////////////////////////////////////////////////////////////////
TH1* VSpectra::AddHisto2D(string name, string title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup, string family){
  // create histo
  TH1 *hist = new TH2D(name.c_str(), title.c_str(), nbinsx, xlow, xup, nbinsy, ylow, yup);

  string index= family+"/"+name;

  // fill map
  fMapHisto[index] = hist;
  
  // Add it to the server
  NPL::SpectraServer::getInstance()->AddSpectra(hist);

  return hist;
}

////////////////////////////////////////////////////////////////////////////////
TH1* VSpectra::GetSpectra(const string& FamilyAndName){
  map< string , TH1*>::iterator it;
  it = fMapHisto.find(FamilyAndName);

  if(it == fMapHisto.end()){
    cout << "ERROR : the folowing Histo has been requested and does not exist: " << FamilyAndName << endl ;
    exit(1);
  }

  else
    return it->second; 

  return it->second;
}
////////////////////////////////////////////////////////////////////////////////
void VSpectra::FillSpectra(const string& familyAndname, double valx){
  static NPL::SpectraServer* serv = NPL::SpectraServer::getInstance();
   serv->FillSpectra( GetSpectra(familyAndname)->GetName(),valx);
}
////////////////////////////////////////////////////////////////////////////////
void VSpectra::FillSpectra(const string& familyAndname, double valx,double valy){
  static NPL::SpectraServer* serv = NPL::SpectraServer::getInstance();
   serv->FillSpectra( GetSpectra(familyAndname)->GetName(),valx,valy);
}
////////////////////////////////////////////////////////////////////////////////
void VSpectra::FillSpectra(const string& family,const string& name, double valx){
  static NPL::SpectraServer* serv = NPL::SpectraServer::getInstance();
  serv->FillSpectra(name,valx);
}
////////////////////////////////////////////////////////////////////////////////
void VSpectra::FillSpectra(const string& family,const string& name, double valx, double valy){
  static NPL::SpectraServer* serv = NPL::SpectraServer::getInstance();
  serv->FillSpectra(name,valx,valy);
}

////////////////////////////////////////////////////////////////////////////////
TH1* VSpectra::GetSpectra(const string& family,const string& name){
  string index = family + "/" + name;
  return GetSpectra(index);
}

////////////////////////////////////////////////////////////////////////////////
void VSpectra::WriteSpectra(string filename){
   TFile* f = NULL; 
   if (filename != "VOID") {
      f = new TFile(filename.c_str(), "RECREATE");
   }
   else { // default case is output root file
      f = RootOutput::getInstance()->GetFile();
      f->cd();
   }

   // creating top directory for spectra
   string dirname = "ControlSpectra";
   TDirectory *dir = (TDirectory*) f->Get(dirname.c_str());
   if (!dir) {
      dir = f->mkdir(dirname.c_str());
   }
   dir->cd();

   // creating sub directory for each detector spectra
   TDirectory *dirdet = (TDirectory*) dir->Get(m_name.c_str());
   if (!dirdet) {
      dirdet = dir->mkdir(m_name.c_str());
   }
   dirdet->cd();

   // write all histos
   map< string, TH1* >::iterator it;
   for (it=fMapHisto.begin(); it!=fMapHisto.end(); ++it) {
      it->second->Write();
   }

   // Go back to root directory
   f->cd();

   // close file
   if (filename != "VOID") {
      f->Close();
   }
}
