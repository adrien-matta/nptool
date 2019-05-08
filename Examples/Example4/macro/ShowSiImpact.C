TChain* chain=NULL;

TH2F* h[20];

TCutG* cut_p=NULL;

///////////////////////////////////////////////
void LoadCut()
{
  TFile* File_cutp = new TFile("cut/cut_p.root","READ");
  cut_p = (TCutG*) File_cutp->FindObjectAny("cut_p");
}

///////////////////////////////////////////////
void LoadChain()
{
  chain = new TChain("PhysicsTree");
  chain->Add("../../Outputs/Analysis/Example4.root");
}

////////////////////////////////////////////////
void ShowSiImpact()
{
  LoadCut();
  LoadChain();

  for(int i=0; i<20; i++){
    TString token1 = Form("SiPosZ:SiPosY>>h%d(200,-200,200,200,0,300)",i);
    TString token2 = Form("SiNumber==%d && cut_p",i);
    chain->Draw(token1,token2);
    h[i] = (TH2F*)gDirectory->FindObjectAny(Form("h%d",i));
    h[i]->SetMarkerStyle(7);

    h[i]->SetMarkerColor(kAzure+i);
    if(i>9)h[i]->SetMarkerColor(kAzure+i-10);
  }

  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  c1->cd();
  h[0]->Draw();
  for(int i=1; i<20; i++){
    h[i]->Draw("same");
  }
}
