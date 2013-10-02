#include <iostream>

#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"
#include "TVector3.h"
#include "TCanvas.h"
#include "TMath.h"

#include "TGaspardTrackerData.h"
#include "TInitialConditions.h"
#include "TInteractionCoordinates.h"

void TestAngleReconstruction(const char * fname = "mySimul")
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
   tree->SetBranchStatus("InteractionCoordinates", 1);

   // Prepare histograms
   // Angles
   TH1F *hThetDetecTargetCenter = new TH1F("hThetDetecTargetCenter", "Theta angle w.r.t target center", 180, 0, 180);
   TH1F *hThetDetecInteraction  = new TH1F("hThetDetecInteraction",  "Theta angle w.r.t interaction",   180, 0, 180);
   TH1F *hThetDetecInterBeam    = new TH1F("hThetDetecInterBeam",    "Theta angle w.r.t interaction AND beam",   180, 0, 180);
   // Difference of Angles
   TH1F *hDThetDetecInteraction  = new TH1F("hDThetDetecInteraction", "Effect of interaction point",                    100, -10, 10);
   TH1F *hDThetDetecInterBeam    = new TH1F("hDThetDetecInterBeam",   "Effect of interaction point AND beam direction", 100, -10, 10);

   // Read the TTree
   Int_t nentries = tree->GetEntries();
   cout << "TTree contains " << nentries << " events" << endl;
   for (Int_t i = 0; i < nentries; i++) {
      if (i%1000 == 0) cout << "Entry " << i << endl;
      tree->GetEntry(i);
      // reconstruct angles
      if (interCoord->GetDetectedMultiplicity() > 0) {
         // detected vertex
         Double_t xdetec = interCoord->GetDetectedPositionX(0);
         Double_t ydetec = interCoord->GetDetectedPositionY(0);
         Double_t zdetec = interCoord->GetDetectedPositionZ(0);
         // interaction vertex
         Double_t xinter = initCond->GetICPositionX(0);
         Double_t yinter = initCond->GetICPositionY(0);
         Double_t zinter = initCond->GetICPositionZ(0);
         // incident beam direction
         Double_t theta_beam = initCond->GetICIncidentAngleTheta(0);
         Double_t phi_beam   = initCond->GetICIncidentAnglePhi(0);
         TVector3 Beam_Direction;
         Beam_Direction.SetMagThetaPhi(1, theta_beam * TMath::Pi()/180., phi_beam * TMath::Pi()/180.);
         // vectors
         TVector3 Detected_wrt_TargetCenter(xdetec, ydetec, zdetec);
         TVector3 Interaction_wrt_TargetCenter(xinter, yinter, zinter);
         TVector3 Detected_wrt_Interaction = Detected_wrt_TargetCenter - Interaction_wrt_TargetCenter;
         // calculate angles
         Double_t theta_Detected_wrt_TargetCenter = Detected_wrt_TargetCenter.Theta();
         Double_t theta_Detected_wrt_Interaction  = Detected_wrt_Interaction.Theta();
         Detected_wrt_Interaction = Detected_wrt_Interaction.Unit();
         Double_t cos_theta_Detected_wrt_Interaction_And_Beam = Beam_Direction.Dot(Detected_wrt_Interaction);
         Double_t theta_Detected_wrt_Interaction_And_Beam = TMath::ACos(cos_theta_Detected_wrt_Interaction_And_Beam);
         // fill histos
         hThetDetecTargetCenter->Fill(theta_Detected_wrt_TargetCenter * 180./TMath::Pi());
         hThetDetecInteraction->Fill(theta_Detected_wrt_Interaction * 180./TMath::Pi());
         hThetDetecInterBeam->Fill(theta_Detected_wrt_Interaction_And_Beam * 180./TMath::Pi());
         hDThetDetecInteraction->Fill((theta_Detected_wrt_Interaction - theta_Detected_wrt_TargetCenter) * 180./TMath::Pi());
         hDThetDetecInterBeam->Fill((theta_Detected_wrt_Interaction_And_Beam - theta_Detected_wrt_TargetCenter) * 180./TMath::Pi());
      }
   }

   // display histograms
   TCanvas *canvas = new TCanvas("canvas", "Comparison of theta angles", 900, 900);
   canvas->Divide(2,3);
   canvas->cd(1);
   hThetDetecTargetCenter->Draw();
   canvas->cd(3);
   hThetDetecInteraction->Draw();
   canvas->cd(4);
   hDThetDetecInteraction->Draw();
   canvas->cd(5);
   hThetDetecInterBeam->Draw();
   canvas->cd(6);
   hDThetDetecInterBeam->Draw();
}
