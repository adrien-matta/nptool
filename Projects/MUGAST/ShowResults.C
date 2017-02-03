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

void ShowResults()
{
   // get tree   
   TFile *f = new TFile("../../Outputs/Analysis/PhysicsTree.root");
   TTree *t = (TTree*) f->Get("PhysicsTree");

   // draw kinematic information
   // canvas
   TCanvas *c1 = new TCanvas("c1", "kinematic information", 600, 600);
   c1->Draw();
   // kinematic line
   TH2F *hk = new TH2F("hk", "hk", 90, 90, 180, 200, 0, 12);
   hk->GetXaxis()->SetTitle("#Theta_{lab} (deg)");
   hk->GetYaxis()->SetTitle("E_{p} (MeV)");
   t->Draw("ELab:ThetaLab>>hk");
   // inset
   TPad *pad = new TPad("pad1", "excitation energy", 0.45, 0.45, 0.87, 0.87);
   pad->Draw();
   pad->cd();
   // excitation energy
   TH1F *hx = new TH1F("hx", "hx", 150, 5, 8);
   hx->SetXTitle("E_{X} (MeV)");
   hx->SetYTitle("counts / (20 keV)");
   t->Draw("Ex>>hx");
}


void CountingRates(Double_t ibeam = 1e5, Double_t ubt = 30)
{
   // load event generator file
   NPL::Reaction* reaction = new NPL::Reaction();
   reaction->ReadConfigurationFile("30Pdp.reaction");
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
