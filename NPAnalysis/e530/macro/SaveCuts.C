#include "TFile.h"
#include "TCutG.h"
#include "TROOT.h"
#include "TSystem.h"

void SaveCuts()
{
  TFile *fcuts = new TFile("cut_ground_state_Jo.root","recreate");
  fcuts->cd();

  gROOT->FindObject("ground_state_Jo")->Write();
  
  fcuts->Close();


}
