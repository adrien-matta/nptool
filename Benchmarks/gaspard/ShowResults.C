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
 *    + This macro displays everything concerning the incident beam and the  *
 *      emitted particle from NPSimulation                                   *
 *                                                                           *
 *    + Use in a ROOT session:                                               *
 *      .x ControlSimu.C("FileToAnalyse")                                    *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

// ROOT headers
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TFile.h"
#include "TString.h"
#include "TEllipse.h"
#include "TLegend.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"

// nptool headers
#include "TReactionConditions.h"
#include "TInteractionCoordinates.h"
#include "NPReaction.h"
using namespace NPL;

TCanvas* canvas1;
TCanvas* canvas2;

void ShowResults(const char * fname = "benchmark_gaspard"){
  // for the style 
  gStyle->SetOptStat(0);
  gROOT->SetStyle("nptool");     
  gROOT->ForceStyle(true);  

  // Open output ROOT file from NPTool simulation run
  TString path = gSystem->Getenv("NPTOOL");
  path += "/Outputs/Simulation/";
  TString inFileName = fname;
  if (!inFileName.Contains("root")) inFileName += ".root";
  TFile *inFile = new TFile(path + inFileName);
  if (!inFile->IsOpen()) exit(1);
  TTree *tree   = (TTree*) inFile->Get("SimulatedTree");

  // Connect the branches of the TTree and activate then if necessary
  // TReactionConditions branch
  TReactionConditions *reacCond = 0;
  tree->SetBranchAddress("ReactionConditions", &reacCond);
  tree->SetBranchStatus("ReactionConditions", 1);
  
  // TInteractionCoordinates branch
  TInteractionCoordinates *interCoord = 0;
  tree->SetBranchAddress("InteractionCoordinates", &interCoord);
  tree->SetBranchStatus("InteractionCoordinates", 1);

  // Declare histograms
  // for incident beam
  TH2F *hEmittanceXY     = new TH2F("hEmittanceXY",     "Beam Position on Target (x, y)", 200, -10, 10, 200, -10, 10);
  TH2F *hEmittanceXTheta = new TH2F("hEmittanceXTheta", "Beam Emittance (#theta_{x}-90,x)", 200, -10, 10, 200, -2, 2);
  TH2F *hEmittanceYPhi   = new TH2F("hEmittanceYPhi",   "Beam Emittance (#phi_{y}-90,y)", 200, -10, 10, 200, -2, 2);
  TH1F *hIncidentTheta   = new TH1F("hIncidentTheta",   "Beam Incident Theta (Polar angle)", 100,   0, 2);
  TH1F *hIncidentPhi     = new TH1F("hIncidentPhi",     "Beam Incident Phi (Azimuthal angle)", 370,  -185, 185);
  TH1F *hIncidentZ       = new TH1F("hIncidentZ",       "Beam z-position of interaction", 200,  -0.007, 0.007);

  // for emitted particle
  TH1F *hEmittedThetaCM = new TH1F("hEmittedThetaCM", "Light Ejectile Theta CM",               180, 0, 180);
  TH1F *hEmittedThetaIF = new TH1F("hEmittedThetaIF", "Light Ejectile Theta (reaction frame)", 180, 0, 180);
  TH1F *hEmittedPhiIF   = new TH1F("hEmittedPhiIF",   "Light Ejectile Phi (reaction frame)",   360, 0, 360);
  TH1F *hEmittedThetaWF = new TH1F("hEmittedThetaWF", "Light Ejectile Theta (detector frame)", 180, 0, 180);
  TH1F *hEmittedPhiWF   = new TH1F("hEmittedPhiWF",   "Light Ejectile Phi (detector frame)",   360, 0, 360);
  TH1F *hControlPhi     = new TH1F("hControlPhi",     "Light Ejectile IncidentPhi + EmittedPhi", 360, 0, 360);
  TH2F *hEmittedETheta  = new TH2F("hEmittedETheta",  "Kinematic line", 1800, 0, 180, 1800, 0, 60);


  // for emitted particle but gated on interaction in detector
  TH2F *hEmittedETheta_detected  = new TH2F("hEmittedETheta_detected",  "Kinematic line (interaction in det.)",1800, 0, 180, 1800, 0, 60);
  TH1F *hEmittedThetaIF_detected = new TH1F("hEmittedThetaIF_detected", "Light Ejectile Theta in reaction frame (interaction in det.)",180, 0, 180);
  TH1F *hEmittedThetaCM_detected = new TH1F("hEmittedThetaCM_detected", "Light Ejectile ThetaCM (interaction in det.)",180, 0, 180);

  // Read the TTree
  Int_t nentries = tree->GetEntries();
  cout << endl << " TTree contains " << nentries << " events" << endl;

  for (Int_t i = 0; i < nentries; i++) {
    if (i%10000 == 0 && i!=0)  {
      cout.precision(5);
      Double_t percent = (Double_t)i/nentries ;
      cout  << "\r Progression:  " << percent*100 << " %" << flush;
    }
    else if (i==nentries-1)  cout << "\r Progression:" << " 100%" << endl;

    // Get entry
    tree->GetEntry(i);

    // Fill histos
    // incident beam
    hEmittanceXY     -> Fill(reacCond->GetVertexPositionX(), reacCond->GetVertexPositionY());
    hIncidentZ       -> Fill(reacCond->GetVertexPositionZ());
    hEmittanceXTheta -> Fill(reacCond->GetVertexPositionX(), reacCond->GetBeamEmittanceThetaX()-90);
    hEmittanceYPhi   -> Fill(reacCond->GetVertexPositionY(), reacCond->GetBeamEmittancePhiY()-90);
    hIncidentTheta   -> Fill(reacCond->GetBeamEmittanceTheta());
    hIncidentPhi     -> Fill(reacCond->GetBeamEmittancePhi());

    // ejected particle
    hEmittedThetaCM  -> Fill(reacCond->GetThetaCM());
    hEmittedThetaIF  -> Fill(reacCond->GetThetaLab_BeamFrame(0));
    hEmittedThetaWF  -> Fill(reacCond->GetThetaLab_WorldFrame(0));
    hEmittedETheta   -> Fill(reacCond->GetThetaLab_BeamFrame(0), reacCond->GetKineticEnergy(0));

    if (interCoord->GetDetectedMultiplicity() > 0) {
      hEmittedETheta_detected  -> Fill(reacCond->GetThetaLab_BeamFrame(0), reacCond->GetKineticEnergy(0));
      hEmittedThetaIF_detected -> Fill(reacCond->GetThetaLab_BeamFrame(0));
      hEmittedThetaCM_detected -> Fill(reacCond->GetThetaCM());
    }
  }


  // Display beam histograms
  canvas1 = new TCanvas("canvas1", "Incident beam properties from EventGenerator", 700,0,  500,750);
  canvas1->Divide(2,3);

  canvas1->cd(1);
  hEmittanceXTheta->Draw("colz");
  hEmittanceXTheta->SetXTitle("X (mm)");
  hEmittanceXTheta->SetYTitle("#theta_{X} (deg)");

  canvas1->cd(2);
  hEmittanceYPhi->Draw("colz");
  hEmittanceYPhi->SetXTitle("Y (mm)");
  hEmittanceYPhi->SetYTitle("#Phi_{Y} (deg)"); 

  canvas1->cd(3);
  hIncidentTheta->Draw();
  hIncidentTheta->SetXTitle("Incident #theta (deg)");

  canvas1->cd(4);
  hIncidentPhi->Draw();
  hIncidentPhi->SetXTitle("Incident #phi (deg)");
  
  canvas1->cd(5);
  hEmittanceXY->Draw("colz");
  hEmittanceXY->SetXTitle("Position on Target X (mm)");
  hEmittanceXY->SetYTitle("Position on Target Y (mm)"); 
  TEllipse *target = new TEllipse(0,0,7.5,7.5);
  target->SetFillStyle(0000);
  target->SetLineStyle(2);
  target->Draw("same");
  TEllipse *targetCenter = new TEllipse(1.0,2.0,0.2,0.2);
  targetCenter->Draw("same");

  canvas1->cd(6);
  hIncidentZ->Draw();
  hIncidentZ->SetXTitle("Z (mm)");

  // Display detector histograms
  canvas2 = new TCanvas("canvas2", "Emitted particle properties",500,500);
  canvas2->Divide(2,2);

  canvas2->cd(1);
  hEmittedThetaCM->SetXTitle("#theta_{c.m.}");
  hEmittedThetaCM->SetYTitle("counts / 1^{#circ}");
  hEmittedThetaCM->SetLineColor(kBlue-3);
  hEmittedThetaCM->SetFillColor(kBlue-3);
  hEmittedThetaCM->GetYaxis()->SetTitleOffset(1.18);
  hEmittedThetaCM->Draw();
  
  hEmittedThetaCM_detected->SetLineColor(kOrange-3);
  hEmittedThetaCM_detected->SetFillColor(kOrange-3);
  hEmittedThetaCM_detected->Draw("same");
  
  TLegend* leg = new TLegend(0.53, 0.68, 0.81, 0.9);
  leg->AddEntry(hEmittedThetaCM,"Emitted","f");
  leg->AddEntry(hEmittedThetaCM_detected,"Detected","f");
  leg->Draw();

  canvas2->cd(2);
  hEmittedETheta_detected->SetXTitle("#theta_{Lab}");
  hEmittedETheta_detected->SetYTitle("E_{Lab} (MeV)");
  hEmittedETheta_detected->Draw();
  NPL::Reaction r("132Sn(d,p)133Sn@1320");
  TGraph* Kine = r.GetKinematicLine3(); 
  Kine->SetLineWidth(1);
  Kine->SetLineStyle(2);
  Kine->SetLineColor(kOrange-3);
  Kine->Draw("c same");

  canvas2->cd(3);
  hEmittedThetaIF->SetXTitle("#theta_{Lab}");
  hEmittedThetaIF->SetYTitle("counts / 1^{#circ}");
  hEmittedThetaIF->SetLineColor(kBlue-3);
  hEmittedThetaIF->SetFillColor(kBlue-3);
  hEmittedThetaIF->GetYaxis()->SetTitleOffset(1.18);
  hEmittedThetaIF->Draw();

  hEmittedThetaIF_detected->SetLineColor(kOrange-3);
  hEmittedThetaIF_detected->SetFillColor(kOrange-3);
  hEmittedThetaIF_detected->Draw("same");

  canvas2->cd(4);
  TH1F *hEfficiency = new TH1F("hEfficiency", "Efficiency", 180, 0, 180);
  hEfficiency->Divide(hEmittedThetaIF_detected, hEmittedThetaIF, 100, 1);
  hEfficiency->GetXaxis()->SetTitle("#theta (deg)");
  hEfficiency->GetYaxis()->SetTitle("#epsilon (%)");
  hEfficiency->Draw();


  TFile* referenceFile = new TFile("reference.root");
  TCanvas* canvas1_ref = (TCanvas*) referenceFile->FindObjectAny("canvas1_ref");
  canvas1_ref->SetTitle(  Form("FROM reference.root: %s",canvas1_ref->GetTitle()) );
  TCanvas* canvas2_ref = (TCanvas*) referenceFile->FindObjectAny("canvas2_ref");
  canvas2_ref->SetTitle(  Form("FROM reference.root: %s",canvas2_ref->GetTitle()) );
  canvas1_ref->Draw();
  canvas2_ref->Draw();

}


////////////////////////////////////////////////////////////////////////////////
// Use this method to overwrite the reference file only
// DO NOT USE UNLESS YOU WANT TO MAKE A CHANGE TO THE BENCHMARK
void WriteGaspardReference(){
  TFile *outFile = new TFile("reference.root","RECREATE");
  canvas1->SetName("canvas1_ref");
  canvas2->SetName("canvas2_ref");
  canvas1->Write();
  canvas2->Write();
  outFile->Close();
}
