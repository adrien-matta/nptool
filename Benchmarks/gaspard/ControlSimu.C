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

void ControlSimu(const char * fname = "benchmark_gaspard")
{
   // Open output ROOT file from NPTool simulation run
   TString path = gSystem->Getenv("NPTOOL");
   path += "/Outputs/Simulation/";
   TString inFileName = fname;
   if (!inFileName.Contains("root")) inFileName += ".root";
   TFile *inFile = new TFile(path + inFileName);
   if (!inFile->IsOpen()) exit(1);
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
   TH2F *hEmittedETheta  = new TH2F("hEmittedETheta",  "Kinematic line", 180, 0, 180, 800, 0, 80);


   // for emitted particle but gated on interaction in detector
   TH2F *hEmittedETheta_detected  = new TH2F("hEmittedETheta_detected",  "Kinematic line (interaction in det.)",180, 0, 180, 800, 0, 80);
   TH1F *hEmittedThetaIF_detected  = new TH1F("hEmittedThetaIF_detected",  "Light Ejectile Theta in reaction frame (interaction in det.)",180, 0, 180);
   TH1F *hEmittedThetaCM_detected  = new TH1F("hEmittedThetaCM_detected",  "Light Ejectile ThetaCM (interaction in det.)",180, 0, 180);

   // Read the TTree
   Int_t nentries = tree->GetEntries();
   cout <<endl << " TTree contains " << nentries << " events" << endl;
   clock_t begin=clock();
   clock_t end=begin;

   for (Int_t i = 0; i < nentries; i++) {
      if (i%10000 == 0 && i!=0)  {
         cout.precision(5);
         end = clock();
         double TimeElapsed = (end-begin) / CLOCKS_PER_SEC;
         double percent = (double)i/nentries ;
         double TimeToWait = (TimeElapsed/percent) - TimeElapsed;
         cout  << "\r Progression:" << percent*100 << " % \t | \t Remaining time : ~" <<  TimeToWait <<"s"<< flush;
      }
      else if (i==nentries-1)  cout << "\r Progression:" << " 100% " <<endl;
      
      // Get entry
      tree->GetEntry(i);

      // Fill histos
      // incident beam
      hEmittanceXY     -> Fill(initCond->GetIncidentPositionX(), initCond->GetIncidentPositionY());
      hIncidentZ       -> Fill(initCond->GetIncidentPositionZ());
      hEmittanceXTheta -> Fill(initCond->GetIncidentPositionX(), initCond->GetIncidentEmittanceThetaX()-90);
      hEmittanceYPhi   -> Fill(initCond->GetIncidentPositionY(), initCond->GetIncidentEmittancePhiY()-90);
      hIncidentTheta   -> Fill(initCond->GetIncidentEmittanceTheta());
      hIncidentPhi     -> Fill(initCond->GetIncidentEmittancePhi());
            
      // ejected particle
      hEmittedThetaCM  -> Fill(initCond->GetThetaCM(0));
      hEmittedThetaIF  -> Fill(initCond->GetThetaLab_IncidentFrame(0));
      hEmittedThetaWF  -> Fill(initCond->GetThetaLab_WorldFrame(0));
      hEmittedETheta   -> Fill(initCond->GetThetaLab_IncidentFrame(0), initCond->GetKineticEnergy(0));

      if( interCoord->GetDetectedMultiplicity()>0 ) {
            hEmittedETheta_detected   -> Fill(initCond->GetThetaLab_IncidentFrame(0), initCond->GetKineticEnergy(0));
            hEmittedThetaIF_detected   -> Fill(initCond->GetThetaLab_IncidentFrame(0));
            hEmittedThetaCM_detected   -> Fill(initCond->GetThetaCM(0));
      }
      //hEmittedPhiIF    -> Fill(initCond->GetICEmittedAnglePhiIncidentFrame(0));
      //hEmittedPhiWF    -> Fill(initCond->GetICEmittedAnglePhiWorldFrame(0));
      //
      // Control histo
      // Double_t phi_control = initCond->GetIncidentAnglePhi(0) + initCond->GetICEmittedAnglePhiIncidentFrame(0);
      // if (phi_control > 360) phi_control -= 360;
      // hControlPhi->Fill(phi_control);
   }


   // Display histograms
   // 
 
   TCanvas *canvas1 = new TCanvas("canvas1", "Incident beam properties from EventGenerator", 700,1000);
   canvas1->Divide(2,3);

   canvas1->cd(1);
   hEmittanceXTheta->Draw("colz");
   hEmittanceXTheta->SetXTitle("X (mm)");
   hEmittanceXTheta->SetYTitle("#Theta_{X} (deg.)");

   canvas1->cd(2);
   hEmittanceYPhi->Draw("colz");
   hEmittanceYPhi->SetXTitle("Y (mm)");
   hEmittanceYPhi->SetYTitle("#Phi_{Y} (deg.)"); 

   canvas1->cd(3);
   hIncidentTheta->Draw();

   canvas1->cd(4);
   hIncidentPhi->Draw();

   canvas1->cd(5);
   hEmittanceXY->Draw("colz");
   TEllipse *target = new TEllipse(0,0,7.5,7.5);
   target->SetFillStyle(0000);
   target->SetLineStyle(2);
   target->Draw("same");
   TEllipse *targetCenter = new TEllipse(1.0,2.0,0.2,0.2);
   targetCenter->Draw("same");
 
   canvas1->cd(6);
   hIncidentZ->Draw();
   hIncidentZ->SetXTitle("Z {mm}");

/*
   canvas1->cd();
   TPad *pad2 = new TPad("pad2","",0,0,1,1);
   pad2->SetFillStyle(4000); //will be transparent
   pad2->Draw();
   pad2->cd();
   TLatex *   tex = new TLatex(0.2241379,0.2886654,"Reference");
   tex->SetTextColor(17);
   tex->SetTextSize(0.1840081);
   tex->SetTextAngle(43.20207);
   tex->SetLineWidth(2);
   tex->Draw();
   pad2->Update();
   pad2->Modified();
*/ 

   // Display histograms
   TCanvas *canvas2 = new TCanvas("canvas2", "Emitted particle properties",700,700);
   canvas2->Divide(2,2);

   canvas2->cd(1);
   hEmittedThetaCM->SetXTitle("#Theta_{c.m.}");
   hEmittedThetaCM->Draw();
   hEmittedThetaCM_detected->Draw("same");
   hEmittedThetaCM_detected->SetLineColor(kRed);

   canvas2->cd(2);
   hEmittedETheta->Draw("colz");
   hEmittedETheta_detected->Draw("same");
   hEmittedETheta_detected->SetMarkerColor(kRed);
   hEmittedETheta->SetYTitle("E [MeV]");
   hEmittedETheta->SetXTitle("#Theta_{lab}");
   NPL::Reaction r("132Sn(d,p)133Sn@1320");
   r.SetExcitationHeavy(0.0);
   TGraph* Kine = r.GetKinematicLine3(); 
   Kine->SetLineWidth(1);
   Kine->SetLineStyle(2);
   Kine->SetLineColor(kOrange-3);
   Kine->Draw("csame");

   canvas2->cd(3);
   hEmittedThetaIF->Draw();
   hEmittedThetaIF->SetXTitle("#Theta_{lab}");
   hEmittedThetaIF_detected->Draw("same");
   hEmittedThetaIF_detected->SetLineColor(kRed);

   canvas2->cd(4);
   TH1F *hEfficiency = new TH1F("hEfficiency", "Efficiency", 180, 0, 180);
   hEfficiency->Divide(hEmittedThetaIF_detected, hEmittedThetaIF, 100, 1);
   hEfficiency->GetXaxis()->SetTitle("#Theta (deg)");
   hEfficiency->GetYaxis()->SetTitle("#epsilon (%)");
   hEfficiency->SetLineColor(kBlack);
   hEfficiency->Draw();


/*
   canvas2->cd();
   TPad *pad3 = new TPad("pad3","",0,0,1,1);
   pad3->SetFillStyle(4000); //will be transparent
   pad3->Draw();
   pad3->cd();
   TLatex *   tex2 = new TLatex(0.2241379,0.2886654,"Reference");
   tex2->SetTextColor(17);
   tex2->SetTextSize(0.1840081);
   tex2->SetTextAngle(43.20207);
   tex2->SetLineWidth(2);
   tex2->Draw();
   pad3->Update();
   pad3->Modified();
*/

   TFile *referenceFile = new TFile("reference.root");
   TCanvas* canvas1_ref = (TCanvas*) referenceFile->FindObjectAny("canvas1");
   TCanvas* canvas2_ref = (TCanvas*) referenceFile->FindObjectAny("canvas2");
   canvas1_ref->SetName("canvas1_ref");
   canvas2_ref->SetName("canvas2_ref");
   canvas1_ref->Draw();
   canvas2_ref->Draw();
 
/*
   TFile *outFile = new TFile("reference.root","RECREATE");
    canvas1->Write();
    canvas2->Write();
    outFile->Close();
*/
}
