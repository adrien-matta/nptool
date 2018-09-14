TChain* chain=NULL;

TH1F* hvertex;
TH1F* henergy;
TH1F* hangle;

TCutG* cut_p=NULL;

///////////////////////////////////////////////
void LoadCut()
{
  TFile* File_cutp = new TFile("../cut/cut_p.root","READ");
  cut_p = (TCutG*) File_cutp->FindObjectAny("cut_p");
}

///////////////////////////////////////////////
void LoadChain()
{
  chain = new TChain("PhysicsTree");
  chain->Add("../../../Outputs/Analysis/Example4.root");
}

////////////////////////////////////////////////
void CheckReco()
{
  LoadCut();
  LoadChain();

  TCanvas* c1 = new TCanvas("c1","c1",1200,1200);
  c1->Divide(2,2);
  // Vertex //
  c1->cd(1);
  chain->Draw("InitXVertex+127-XVertex>>hvertex(200,-100,100)","cut_p");
  hvertex = (TH1F*) gDirectory->FindObjectAny("hvertex");
  hvertex->GetXaxis()->SetTitle("X_{init}-X_{reco} (mm)");
  hvertex->GetXaxis()->CenterTitle();

  // Angle //
  c1->cd(2);
  chain->Draw("InitTheta3-ThetaLab>>hangle(200,-10,10)","cut_p");
  hangle = (TH1F*) gDirectory->FindObjectAny("hangle");
  hangle->GetXaxis()->SetTitle("#theta_{init}-#theta_{reco} (deg)");
  hangle->GetXaxis()->CenterTitle();

  // Energy //
  c1->cd(3);
  chain->Draw("InitE3-ELab>>henergy(200,-2,2)","cut_p");
  henergy = (TH1F*) gDirectory->FindObjectAny("henergy");
  henergy->GetXaxis()->SetTitle("E_{init}-E_{reco} (MeV)");
  henergy->GetXaxis()->CenterTitle();



}
