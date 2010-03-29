#include "TFile.h"
#include "TCutG.h"
#include "TROOT.h"
#include "TSystem.h"

void SaveCuts()
{
  TFile *fcuts = new TFile("cut_protons_E_TOF3.root","recreate");
  fcuts->cd();

  gROOT->FindObject("protons_E_TOF3")->Write();
  
  fcuts->Close();


}
