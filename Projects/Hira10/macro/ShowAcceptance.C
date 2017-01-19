TChain* chain;

/////////////////////////////////////////////////
void LoadChain(){
    chain = new TChain("PhysicsTree");
    chain->Add("/Users/pierremorfouace/Physics/NPTool/nptool/Outputs/Analysis/hira_acceptance.root");
    return;
}

/////////////////////////////////////////////////
void ShowAcceptance(){
    gROOT->SetStyle("pierre_style");
    
    LoadChain();
    
    TCanvas *c1 = new TCanvas("c1","c1",800,800);
    c1->cd();
    chain->Draw("ThetaLab:ECM>>h1(220,0,220,180,0,180)","","colz");
    TH2F *h1 = (TH2F*) gDirectory->FindObjectAny("h1");
    h1->GetXaxis()->SetTitle("E_{c.m.} (MeV)");
    h1->GetYaxis()->SetTitle("#theta_{lab} (deg)");
    h1->GetXaxis()->CenterTitle();
    h1->GetYaxis()->CenterTitle();
    h1->Draw("colz");
    
    TCanvas *c2 = new TCanvas("c2","c2",800,800);
    c2->cd();
    chain->Draw("ThetaCM:ECM>>h2(220,0,220,180,0,180)","","colz");
    TH2F *h2 = (TH2F*) gDirectory->FindObjectAny("h2");
    h2->GetXaxis()->SetTitle("E_{c.m.} (MeV)");
    h2->GetYaxis()->SetTitle("#theta_{c.m.} (deg)");
    h2->GetXaxis()->CenterTitle();
    h2->GetYaxis()->CenterTitle();
    h2->Draw("colz");
    
    
}
