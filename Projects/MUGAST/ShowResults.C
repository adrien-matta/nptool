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
#include "TInitialConditions.h"
#include "TInteractionCoordinates.h"
//#include "NPDetectorManager.h"
#include "NPReaction.h"
using namespace NPL;

void ShowResults(){
   // get tree   
   TFile *f = new TFile("../../Outputs/Analysis/PhysicsTree.root");
   TTree *t = (TTree*) f->Get("PhysicsTree");

   // draw kinematic information
   // canvas
   TCanvas *c1 = new TCanvas("c1", "kinematic information", 600, 600);
   c1->Draw();
   // kinematic line
   TH2F *hk = new TH2F("hk", "hk", 180, 0, 180, 200, 0, 60);
   hk->GetXaxis()->SetTitle("#Theta_{lab} (deg)");
   hk->GetYaxis()->SetTitle("E_{p} (MeV)");
   cout << "counts " << t->Draw("ELab:ThetaLab>>hk","Ex>0&&Ex<6","colz") << endl;
   NPL::Reaction* reaction = new NPL::Reaction();
   reaction->ReadConfigurationFile("28Mg.reaction");
   reaction->GetKinematicLine3()->Draw("c");

  new TCanvas();
  TH1F *hCM = new TH1F("hCM", "hCM", 36, 0, 180); 
  t->Draw("ThetaCM>>hCM","Ex>0&&Ex<6","",2900);
  for(int i = 0 ; i < hCM->GetNbinsX();i++){
    if(hCM->GetBinCenter(i)==37.5 || hCM->GetBinCenter(i)==97.5|| hCM->GetBinCenter(i)==167.5|| hCM->GetBinCenter(i)==42.5){
      hCM->SetBinContent(i,0);
      
      }
    
    }
 gPad->SetLogy();

  TFile* file= new TFile("effMUGAST.root");
  TH1* hOmega = (TH1*)file->FindObjectAny("hDetecThetaCM");
  hOmega->Rebin(5);
  hCM->Sumw2();
  hCM->Divide(hOmega);
  TGraph* g= new TGraph("22.jjj");
  g->Draw("c");
   hCM->Scale(g->Eval(32.5)/hCM->GetBinContent(hCM->FindBin(32.5)));
  TGraph* g2= new TGraph("22.l2");
  g2->Draw("c");
  TGraph* g3= new TGraph("22.l3");
  g3->Draw("c");


}


void CountingRates(Double_t ibeam = 1e5, Double_t ubt = 30){
   // load event generator file
   NPL::Reaction* reaction = new NPL::Reaction();
   reaction->ReadConfigurationFile("28Mg.reaction");
//   reaction->ReadConfigurationFile("11Be_d3He.reaction");
   // get angular distribution
   TH1F *dsig = reaction->GetCrossSectionHist();
   dsig->Draw();
   // calculate total cross section
   Double_t stot = reaction->GetTotalCrossSection();
   cout << "total cross section = " << reaction->GetTotalCrossSection() << " mb" << endl;

   // get target thickness
//   NPL::DetectorManager* myDetector = new NPL::DetectorManager();
//   myDetector->ReadConfigurationFile("MUGAST_Manu.detector");

}
