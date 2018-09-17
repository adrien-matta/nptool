TChain* chain=NULL;

TH2F* hpid;
TH2F* hkine;
TH2F* hev;
TH1F* he;

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
void ShowResult()
{
  LoadCut();
  LoadChain();

  TCanvas* c1 = new TCanvas("c1","c1",1200,1200);
  c1->Divide(2,2);
  // PID //
  c1->cd(1);
  chain->Draw("DE/60.:ESi>>hpid(500,0,15,500,0.2,20)","","colz");
  hpid = (TH2F*) gDirectory->FindObjectAny("hpid");
  hpid->GetXaxis()->SetTitle("E_{Si} (MeV)");
  hpid->GetXaxis()->CenterTitle();
  hpid->GetYaxis()->SetTitle("#DeltaE (a.u.)");
  hpid->GetYaxis()->CenterTitle();

  cut_p->SetLineWidth(2);
  cut_p->SetLineColor(2);
  cut_p->Draw("same");
  // Kinematics //
  c1->cd(2);
  chain->Draw("ELab:ThetaLab>>hkine(100,0,50,200,0,15)","cut_p","colz");
  hkine = (TH2F*) gDirectory->FindObjectAny("hkine");
  hkine->GetXaxis()->SetTitle("#theta_{lab} (deg)");
  hkine->GetXaxis()->CenterTitle();
  hkine->GetYaxis()->SetTitle("E_{lab} (MeV)");
  hkine->GetYaxis()->CenterTitle();

  NPL::Reaction* r1 = new NPL::Reaction("18O(p,p)18O@50");
  TGraph* g1;
  g1=r1->GetKinematicLine3();
  g1->SetLineWidth(2);
  g1->SetLineColor(2);
  g1->Draw("lsame");

  // Vertex-Ex //
  c1->cd(3);
  chain->Draw("Ex:XVertex>>hev(150,0,300,200,-5,5)","cut_p","colz");
  hev = (TH2F*) gDirectory->FindObjectAny("hev");
  hev->GetXaxis()->SetTitle("X_{vertex} (mm)");
  hev->GetXaxis()->CenterTitle();
  hev->GetYaxis()->SetTitle("E*_{18O} (MeV)");
  hev->GetYaxis()->CenterTitle();
  // Ex //
  c1->cd(4);
  chain->Draw("Ex>>he(200,-5,5)","cut_p");
  he = (TH1F*) gDirectory->FindObjectAny("he");
  he->GetXaxis()->SetTitle("E*_{18O} (MeV)");
  he->GetXaxis()->CenterTitle();
}
