#include <iostream>

#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"

#include "TInitialConditions.h"
#include "TInteractionCoordinates.h"

void GeometricalEfficiency(const char * fname = "Efficiency_10000")
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
   tree->SetBranchStatus("InteractionCoordinates", 1);

   // Prepare histograms
   TH1F *hDetecTheta = new TH1F("hDetecTheta", "DetecTheta", 180, 0, 180);
   TH1F *hEmittTheta = new TH1F("hEmittTheta", "EmittTheta", 180, 0, 180);

   // Read the TTree
   Int_t nentries = tree->GetEntries();
   cout << "TTree contains " << nentries << " events" << endl;
   for (Int_t i = 0; i < nentries; i++) {
      if (i%1000 == 0) cout << "Entry " << i << endl;
      tree->GetEntry(i);
      // Fill histos
      hEmittTheta->Fill(initCond->GetICEmittedAngleThetaLabWorldFrame(0));
      if (interCoord->GetDetectedMultiplicity() > 0)
         hDetecTheta->Fill(interCoord->GetDetectedAngleTheta(0));
   }

   // Compute relative efficiency in %
   TH1F *efficiency = new TH1F("hEfficiency", "Efficiency", 180, 0, 180);
//   efficiency->SetTitle("Efficiency GASPARD;#Theta [deg];#epsilon [%]");
   efficiency->SetTitle("Efficiency e530;#Theta [deg];#epsilon [%]");
   efficiency->Divide(hDetecTheta,hEmittTheta,100);
   efficiency->SetMaximum(110);
   efficiency->Draw();
}
