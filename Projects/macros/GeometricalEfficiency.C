/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 22/07/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *    + This macro calculates the geometrical efficiency for a given geometry*
 *      NPSimulation should have been run with an isotropic source event     *
 *      generator.                                                           *
 *                                                                           *
 *    + Use in a ROOT session:                                               *
 *      .x GeometricalEfficiency.C("FileToAnalyse")                          *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// ROOT headers
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TF1.h"
#include "TString.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"
#include "TCanvas.h"

// NPTOOL headers
#include "TInitialConditions.h"
#include "TInteractionCoordinates.h"

// C++ headers
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;


void GeometricalEfficiency(const char * fname = "myResult.root"){
  // Open output ROOT file from NPTool simulation run
  TString path = gSystem->Getenv("NPTOOL");
  path += "/Outputs/Simulation/";
  TString inFileName = fname;
  if (!inFileName.Contains("root")) inFileName += ".root";
  TFile *inFile = new TFile(path + inFileName);
  TTree *tree   = (TTree*) inFile->Get("SimulatedTree");
  
  // Connect the branches of the TTree and activate then if necessary
  // TInitialConditions branch
  TInitialConditions *initCond = 0;
  tree->SetBranchAddress("InitialConditions", &initCond);
  tree->SetBranchStatus("InitialConditions", true);
  // TInteractionCoordinates branch
  TInteractionCoordinates *interCoord = 0;
  tree->SetBranchAddress("InteractionCoordinates", &interCoord);
  tree->SetBranchStatus("InteractionCoordinates", true);
  
  // Prepare histograms
  TH1F *hDetecTheta   = new TH1F("hDetecTheta",   "DetecTheta",    180, 0, 180);
  TH1F *hDetecThetaCM = new TH1F("hDetecThetaCM", "hDetecThetaCM", 180, 0, 180);
  TH1F *hEmittTheta   = new TH1F("hEmittTheta",   "EmittTheta",    180, 0, 180);
  TH1F *hEmittThetaCM = new TH1F("hEmittThetaCM", "hEmittThetaCM", 180, 0, 180);
  
  // Read the TTree
  int nentries = tree->GetEntries();
  for (int i = 0; i < nentries; i++) {
    tree->GetEntry(i);
    // Fill histos
    hEmittTheta->Fill(initCond->GetThetaLab_WorldFrame(0));
    hEmittThetaCM->Fill(initCond->GetThetaCM(0));

    if (interCoord->GetDetectedMultiplicity() > 0){
      hDetecTheta->Fill(initCond->GetThetaLab_WorldFrame(0));
      hDetecThetaCM->Fill(initCond->GetThetaCM(0));
    }
  }

   hEmittTheta->Sumw2();
    hEmittThetaCM->Sumw2();
      hDetecTheta->Sumw2();
      hDetecThetaCM->Sumw2();
   

   TCanvas *c0 = new TCanvas("c0", "Distrib",800,800);
  hEmittTheta->Draw("");
  hDetecTheta->Draw("same");
  // efficiency in lab frame in %
  TCanvas *c = new TCanvas("c", "efficiency",800,800);
  c->SetTopMargin(0.01);
  c->SetRightMargin(0.03);
  TH1F *hEfficiency = new TH1F("hEfficiency", "Efficiency", 180, 0, 90);
  hEfficiency->Divide(hDetecTheta, hEmittTheta, 1, 1);
  hEfficiency->GetXaxis()->SetTitle("#Theta (deg)");
  hEfficiency->GetYaxis()->SetTitle("#epsilon");
  hEfficiency->Draw();

  TCanvas* c4 = new TCanvas("c4", "CM Frame",800,800);
  c4->SetTopMargin(0.01);
  c4->SetRightMargin(0.03);
  TH1F* SolidACM = new TH1F(*hDetecThetaCM);
  SolidACM->Sumw2();
  TF1* C = new TF1("C",Form("1./(2*%f*sin(x*%f/180.)*1*%f/180)",M_PI,M_PI,M_PI),0,90);
  SolidACM->Divide(hEmittThetaCM);
  SolidACM->Divide(C,1);
  SolidACM->Draw();
  SolidACM->GetXaxis()->SetTitle("#theta_{CM} (deg)");
  SolidACM->GetYaxis()->SetTitle("d#Omega (sr) ");
  TF1* f = new TF1("f",Form("2 * %f * sin(x*%f/180.) *1*%f/180.",M_PI,M_PI,M_PI),0,90);
  f->Draw("SAME");
  c4->Update();

}
