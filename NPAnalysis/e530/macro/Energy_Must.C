/////////////////////////////////////////////////////////////
//
//    Read a CATS run and display some spectra             //
//
/////////////////////////////////////////////////////////////


#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TCutG.h"
#include "TMust2Physics.h"


#include <iostream>

using namespace std;

void Energy_Must(const char* frun = "/60Fe_AnalysedData")
{
  // open the ROOT file to process
  TString path  = gSystem->Getenv("NPTOOL");
  path         += "/Outputs/Analysis/";

  TString inFileName = frun;
  TFile *inFile = new TFile(path + inFileName + ".root");
  inFile->ls();
  
  TTree *tree   = (TTree*) inFile->Get("Analysed_Data");
  
  //connect TMust2Physics Branch
  TMust2Physics *M2 = new TMust2Physics();
  TBranch *brMust2 = tree->GetBranch("MUST2");
  brMust2 ->SetAddress(&M2);

  
  // prepare output histograms 
  TH2F* Si_Ex_Si_Ey         = new TH2F("Ey vs Ex","Ey vs Ex",1000,0,30000,1000,0,30000);
  TH1F* Si_Ex              = new TH1F("Ex","Ex",1000,0,30000);
  TH1F* Si_Ey              = new TH1F("Ey","Ey",1000,0,30000);
  TH1F* Si_E               = new TH1F("E","Max(Ey,Ex)",1000,0,30000);

  TFile *FCuts = new TFile("correl_Ex_Ey.root");
  TCutG *correl = (TCutG*) FCuts->Get("correlation");

      
  // read the data
  
  int nentries = tree->GetEntries();
  cout << "TTree contains " << nentries << " events" << endl;
  

  for (int i = 0; i < nentries; i++) {
    //for (int i = 0; i < 100; i++) {
    if (i%10000 == 0) cout << "Entry " << i << endl;
    //////////////////////
    // read Cats branch //
    //////////////////////
    tree ->GetEntry(i);

    for(unsigned int hit = 0; hit < M2 -> Si_E.size() ; hit ++)
      {
	Si_E  -> Fill( M2 -> Si_E  [hit]);
	Si_Ex -> Fill( M2 -> Si_E_X[hit]);
	Si_Ey -> Fill( M2 -> Si_E_Y[hit]);
	//	Si_Ex_Si_Ey -> Fill( M2 -> Si_E_X[hit], M2 -> Si_E_Y[hit]);

	if(correl -> IsInside(M2 -> Si_E_X[hit], M2 -> Si_E_Y[hit]))
	  Si_Ex_Si_Ey -> Fill( M2 -> Si_E_X[hit], M2 -> Si_E_Y[hit]);

      }
    
    
  }   // loop over the events
    
  cout << M2->Check_1 << " " << M2->Check1 << " " << M2->Check2 << endl;
  cout << M2 ->compt_Match_XY << endl;

  TCanvas *c1 = new TCanvas("c","E ",900,900);
  c1->Divide(2,2);
  c1->cd(1);
  Si_E->Draw();
  c1->cd(2);
  Si_Ex->Draw();
  c1->cd(3);
  Si_Ey->Draw();
  c1->cd(4);
  // Si_Ex_Si_Ey->Draw("colz");

  TCanvas *c2 = new TCanvas("c2","Exy ",900,900);
  Si_Ex_Si_Ey->Draw("colz");
}
