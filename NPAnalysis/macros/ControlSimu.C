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

#include <iostream>

#include "TROOT.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"
#include "TH2F.h"

#include "TInitialConditions.h"
#include "TInteractionCoordinates.h"

void ControlSimu(const char * fname = "mySimul")
{
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
   tree->SetBranchStatus("InitialConditions", 1);
   // TInteractionCoordinates branch
   TInteractionCoordinates *interCoord = 0;
   tree->SetBranchAddress("InteractionCoordinates", &interCoord);
   tree->SetBranchStatus("InteractionCoordinates", 0);

   // Declare histograms
   // for incident beam
   TH2F *hEmittanceXY     = new TH2F("hEmittanceXY",     "Position on Target (x, y)", 200, -10, 10, 200, -10, 10);
   TH2F *hEmittanceXTheta = new TH2F("hEmittanceXTheta", "Emittance (x, #theta)",     200, -10, 10, 200, -10, 10);
   TH2F *hEmittanceYPhi   = new TH2F("hEmittanceYPhi",   "Emittance (y, #phi)",       200, -10, 10, 200, -10, 10);
   TH1F *hIncidentTheta   = new TH1F("hIncidentTheta",   "Incident Theta",            100,   0, 10);
   TH1F *hIncidentPhi     = new TH1F("hIncidentPhi",     "Incident Phi",              360,   0, 360);
   TH1F *hIncidentZ       = new TH1F("hIncidentZ",       "z-position of interaction", 200,  -5, 5);

   // for emitted particle
   TH1F *hEmittedThetaCM = new TH1F("hEmittedThetaCM", "Emitted Theta CM",               180, 0, 180);
   TH1F *hEmittedThetaIF = new TH1F("hEmittedThetaIF", "Emitted Theta (reaction frame)", 180, 0, 180);
   TH1F *hEmittedPhiIF   = new TH1F("hEmittedPhiIF",   "Emitted Phi (reaction frame)",   360, 0, 360);
   TH1F *hEmittedThetaWF = new TH1F("hEmittedThetaWF", "Emitted Theta (detector frame)", 180, 0, 180);
   TH1F *hEmittedPhiWF   = new TH1F("hEmittedPhiWF",   "Emitted Phi (detector frame)",   360, 0, 360);
   TH1F *hControlPhi     = new TH1F("hControlPhi",     "IncidentPhi + EmittedPhi",       360, 0, 360);
   TH2F *hEmittedETheta  = new TH2F("hEmittedETheta",  "Kinematic line",                 180, 0, 180, 200, 0, 150);

   // Read the TTree
   Int_t nentries = tree->GetEntries();
   cout << "TTree contains " << nentries << " events" << endl;
   for (Int_t i = 0; i < nentries; i++) {
      if (i%10000 == 0) cout << "Entry " << i << endl;
      tree->GetEntry(i);

      // Fill histos
      // incident beam
      hEmittanceXY     -> Fill(initCond->GetICPositionX(0), initCond->GetICPositionY(0));
      hIncidentZ       -> Fill(initCond->GetICPositionZ(0));
      hEmittanceXTheta -> Fill(initCond->GetICPositionX(0), initCond->GetICIncidentEmittanceTheta(0));
      hEmittanceYPhi   -> Fill(initCond->GetICPositionY(0), initCond->GetICIncidentEmittancePhi(0));
      hIncidentTheta   -> Fill(initCond->GetICIncidentAngleTheta(0));
      hIncidentPhi     -> Fill(initCond->GetICIncidentAnglePhi(0));
      // ejected particle
      hEmittedThetaCM  -> Fill(initCond->GetICEmittedAngleThetaCM(0));
      hEmittedThetaIF  -> Fill(initCond->GetICEmittedAngleThetaLabIncidentFrame(0));
      hEmittedPhiIF    -> Fill(initCond->GetICEmittedAnglePhiIncidentFrame(0));
      hEmittedThetaWF  -> Fill(initCond->GetICEmittedAngleThetaLabWorldFrame(0));
      hEmittedPhiWF    -> Fill(initCond->GetICEmittedAnglePhiWorldFrame(0));
      hEmittedETheta   -> Fill(initCond->GetICEmittedAngleThetaLabIncidentFrame(0), initCond->GetICEmittedEnergy(0));

      // Control histo
      Double_t phi_control = initCond->GetICIncidentAnglePhi(0) + initCond->GetICEmittedAnglePhiIncidentFrame(0);
      if (phi_control > 360) phi_control -= 360;
      hControlPhi->Fill(phi_control);
   }

   // Display histograms
   TCanvas *canvas1 = new TCanvas("canvas1", "Incident beam properties");
   canvas1->Divide(2,3);
   canvas1->cd(1);
   hEmittanceXTheta->Draw("colz");
   canvas1->cd(2);
   hEmittanceYPhi->Draw("colz");
   canvas1->cd(3);
   hIncidentTheta->Draw();
   canvas1->cd(4);
   hIncidentPhi->Draw();
   canvas1->cd(5);
   hEmittanceXY->Draw("colz");
   canvas1->cd(6);
   hIncidentZ->Draw();

   // Display histograms
   TCanvas *canvas2 = new TCanvas("canvas2", "Emitted particle properties");
   canvas2->Divide(2,3);
   canvas2->cd(1);
   hEmittedThetaCM->Draw();
   canvas2->cd(2);
   hEmittedETheta->Draw();
   canvas2->cd(3);
   hEmittedThetaIF->Draw();
   canvas2->cd(4);
   hEmittedPhiIF->Draw();
   canvas2->cd(5);
   hEmittedThetaWF->Draw();
   canvas2->cd(6);
   hEmittedPhiWF->Draw();
//   hControlPhi->SetLineColor(kRed);
//   hControlPhi->Draw("same");

   // Display histograms
   TCanvas *canvasxi32 = new TCanvas("canvas3", "Emitted particle properties", 300, 600);
   canvas3->Divide(1,3);
   canvas3->cd(1);
   hEmittedThetaCM->SetXTitle("#Theta_{c.m.}");
   hEmittedThetaCM->Draw();
   canvas3->cd(2);
   hEmittedThetaWF->Draw();
   hEmittedThetaWF->SetXTitle("#Theta_{lab}");
   canvas3->cd(3);
   hEmittedETheta->SetXTitle("#Theta_{lab}");
   hEmittedETheta->SetYTitle("E [MeV]");
   hEmittedETheta->Draw();
}
