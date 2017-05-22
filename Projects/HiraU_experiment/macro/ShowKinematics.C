TChain *chain1;

//chain1=NULL;

void LoadChain(){
	chain1 = new TChain("PhysicsTree");
    chain1->Add("/Users/pierremorfouace/Physics/NPTool/nptool/Outputs/Analysis/test.root");
}

void ShowKinematics()
{
    gROOT->SetStyle("pierre_style");
	LoadChain();

	chain1->Draw("ELab:ThetaLabHira>>h(500,0,30,1000,20,70)","","");
	TH2F* h = (TH2F*) gDirectory->FindObjectAny("h");
    h->GetXaxis()->SetTitle("#theta_{lab} (deg)");
    h->GetYaxis()->SetTitle("E_{lab} (MeV)");
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->GetXaxis()->SetRangeUser(0,30);
    h->GetYaxis()->SetRangeUser(40,65);
    h->SetMarkerColor(2);
    h->SetMarkerSize(0.1);
    h->Draw("colz");
    
    
    NPL::Reaction *r1 = new NPL::Reaction("1H(12C,1H)12C@58.62");
    g1 = r1->GetKinematicLine3();
    g1->SetLineWidth(1);
    g1->SetLineColor(2);
    g1->Draw("lsame");
    
    NPL::Reaction *r2 = new NPL::Reaction("1H(1H,1H)1H@58.62");
    g2 = r2->GetKinematicLine3();
    g2->SetLineWidth(1);
    g2->SetLineColor(2);
    g2->Draw("lsame");

}
