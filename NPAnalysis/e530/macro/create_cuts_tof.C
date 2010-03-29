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
#include "TTriggerData.h"

#include <iostream>

using namespace std;

void create_cuts_tof(const char* frun = "/60Fe_AnalysedData")
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

  // connect the E and Theta Branches
  double E, Theta;
  TBranch      *ELab = tree->GetBranch("ELab");
  ELab->SetAddress(&E);
  TBranch      *ThetaLab = tree->GetBranch("ThetaLab");
  ThetaLab->SetAddress(&Theta);
  
  // prepare output histograms 
  TH1F* tof1          = new TH1F("TOF1","TOF1",600,350,410); 

  //TH2F* E_Theta       = new TH2F("E vs Theta","E vs Theta",60,100,160,500,0,25000);
  TH2F* DE_E          = new TH2F("DE vs E","DE vs E",1000,0,25000,1000,0,25000);
  TH2F* DE_TOF        = new TH2F("E vs TOF","E vs TOF T123",600,350,410,400,0,10000);
  TH2F* DE_TOF1       = new TH2F("E vs TOF1","E vs TOF Tel1",600,350,410,400,0,10000);
  TH2F* DE_TOF2       = new TH2F("E vs TOF2","E vs TOF Tel2",600,350,410,400,0,10000);
  TH2F* DE_TOF3       = new TH2F("E vs TOF Tel3","E vs TOF Tel3",600,350,410,400,0,10000);

  TH1F * SiLi_E       = new TH1F("SiLi_E","SiLi_E",1000,0,30000);
  
  TFile *FCuts = new TFile("cut_protons.root");
  TCutG *cut_protons = (TCutG*) FCuts->Get("protons");

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
	if(M2 -> TelescopeNumber[hit] != 4 ) //&& M2 -> TelescopeNumber[hit] != 3) {
	  {
	    DE_E   -> Fill(M2 -> SiLi_E[hit], M2 -> Si_E[hit]);
	    
	    if(cut_protons -> IsInside(M2 -> SiLi_E[hit], M2 -> Si_E[hit])) 
	      { 
	      }
	    else
	      {
		DE_TOF -> Fill(M2 -> Si_T[hit], M2 -> Si_E[hit]);
		if(M2 -> TelescopeNumber[hit] == 1)        DE_TOF1 -> Fill(M2 -> Si_T[hit], M2 -> Si_E[hit]);
		else if(M2 -> TelescopeNumber[hit] == 2)   DE_TOF2 -> Fill(M2 -> Si_T[hit], M2 -> Si_E[hit]);
		else                                       DE_TOF3-> Fill(M2 -> Si_T[hit], M2 -> Si_E[hit]);
	      }
	  }
      }  
    
  }   // loop over the events
    
  
  TCanvas *c1 = new TCanvas("c","DE E",900,900);
  DE_E->Draw("colz");
  
  
  TCanvas *c2 = new TCanvas("c2","DE TOF",900,900);
  c2->Divide(2,2);
  c2->cd(1);
  DE_TOF1->Draw("colz");
  c2->cd(2);
  DE_TOF2->Draw("colz");
  c2->cd(3);
  DE_TOF3->Draw("colz");
  c2->cd(4);
  DE_TOF->Draw("colz");
  }
