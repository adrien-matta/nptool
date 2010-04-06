#include "TFile.h"
#include "TCutG.h"
#include "TROOT.h"
#include "TSystem.h"

void SaveCuts()
{
  TFile *fcuts = new TFile("cut_e_tof_jo.root","recreate");
  fcuts->cd();

  gROOT->FindObject("protons_E_TOF_Jo")->Write();
  
  fcuts->Close();


}
