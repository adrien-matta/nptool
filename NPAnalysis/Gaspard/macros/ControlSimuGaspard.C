#include <iostream>

#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"

#include "TGaspardTrackerData.h"
#include "TInitialConditions.h"
#include "TInteractionCoordinates.h"

void ControlSimuGaspard(const char * fname = "mySimul")
{
   // Open output ROOT file from NPTool simulation run
   TString path = gSystem->Getenv("NPTOOL");
   path += "/Outputs/";
   TString inFileName = fname;
   if (!inFileName.Contains("root")) inFileName += ".root";
   TFile *inFile = new TFile(path + inFileName);
   TTree *tree   = (TTree*) inFile->Get("EventTree");

   // Connect the branches of the TTree and activate then if necessary
   // TGaspardTrackerData branch
   TGaspardTrackerData *gpdTrkData = 0;
   tree->SetBranchAddress("GASPARD", &gpdTrkData);
   tree->SetBranchStatus("GASPARD", 0);
   // TInitialConditions branch
   TInitialConditions *initCond = 0;
   tree->SetBranchAddress("InitialConditions", &initCond);
   tree->SetBranchStatus("InitialConditions", 1);
   // TInteractionCoordinates branch
   TInteractionCoordinates *interCoord = 0;
   tree->SetBranchAddress("InteractionCoordinates", &interCoord);
   tree->SetBranchStatus("InteractionCoordinates", 0);

   // Prepare histograms
   TH1F *hIncidentTheta  = new TH1F("hIncidentTheta",  "Incident Theta", 180, 0, 180);
   TH1F *hIncidentPhi    = new TH1F("hIncidentPhi",    "Incident Phi", 360, 0, 360);
   TH1F *hEmittedThetaIF = new TH1F("hEmittedThetaIF", "Emitted Theta (incident frame)", 180, 0, 180);
   TH1F *hEmittedPhiIF   = new TH1F("hEmittedPhiIF",   "Emitted Phi (incident frame)", 360, 0, 360);
   TH1F *hEmittedThetaWF = new TH1F("hEmittedThetaWF", "Emitted Theta (laboratory frame)", 180, 0, 180);
   TH1F *hEmittedPhiWF   = new TH1F("hEmittedPhiWF",   "Emitted Phi (laboratory frame)", 360, 0, 360);
   TH1F *hControlPhi     = new TH1F("hControlPhi",     "IncidentPhi + EmittedPhi", 360, 0, 360);

   // Read the TTree
   Int_t nentries = tree->GetEntries();
   cout << "TTree contains " << nentries << " events" << endl;
   for (Int_t i = 0; i < nentries; i++) {
      if (i%1000 == 0) cout << "Entry " << i << endl;
      tree->GetEntry(i);
      // Fill histos
      hIncidentTheta->Fill(initCond->GetICIncidentAngleTheta(0));
      hIncidentPhi->Fill(initCond->GetICIncidentAnglePhi(0));
      hEmittedThetaIF->Fill(initCond->GetICEmittedAngleThetaLabIncidentFrame(0));
      hEmittedPhiIF->Fill(initCond->GetICEmittedAnglePhiIncidentFrame(0));
      hEmittedThetaWF->Fill(initCond->GetICEmittedAngleThetaLabWorldFrame(0));
      hEmittedPhiWF->Fill(initCond->GetICEmittedAnglePhiWorldFrame(0));
      // Control histo
      Double_t phi_control = initCond->GetICIncidentAnglePhi(0) + initCond->GetICEmittedAnglePhiIncidentFrame(0);
      if (phi_control > 360) phi_control -= 360;
      hControlPhi->Fill(phi_control);
   }

   // Display histograms
   TCanvas *canvas = new TCanvas("canvas", "Angles");
   canvas->Divide(2,3);
   canvas->cd(1);
   hIncidentTheta->Draw();
   canvas->cd(2);
   hIncidentPhi->Draw();
   canvas->cd(3);
   hEmittedThetaIF->Draw();
   canvas->cd(4);
   hEmittedPhiIF->Draw();
   canvas->cd(5);
   hEmittedThetaWF->Draw();
   canvas->cd(6);
   hEmittedPhiWF->Draw();
   hControlPhi->SetLineColor(kRed);
   hControlPhi->Draw("same");
}
