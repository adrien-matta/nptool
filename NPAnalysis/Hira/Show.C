void InitChain();
TChain *chain;
TGraph *kin;

void Show()
{
    gStyle->SetOptStat(0);
    InitChain();
    
    TCanvas* mainC0  = new TCanvas("Hira XY", "Hira XY" , 800,600);
    TCanvas* mainC1  = new TCanvas("PID", "PID" , 800,600);
    TCanvas* mainC2  = new TCanvas("Kinematics", "Kinematics" , 800,600);
    TCanvas* mainC3  = new TCanvas("Phi-Theta", "Phi-Theta" , 800,600);
    TCanvas* mainC4  = new TCanvas("BeamSpot", "BeamSpot" , 800,600);

    mainC0->cd();
    chain->Draw("Y:X>>h0(300,-300,300,200,-200,200)","","colz");
    TH2F* h0 = (TH2F*)gDirectory->FindObjectAny("h0");
    h0->GetXaxis()->SetTitle("X (mm)");
    h0->GetYaxis()->SetTitle("Y (mm)");
    h0->SetTitle("");
    
    mainC1->cd();
    chain->Draw("E_ThickSi:E_CsI>>h1(1000,0,200,1000,0,25)","","colz");
    TH2F* h1 = (TH2F*)gDirectory->FindObjectAny("h1");
    h1->GetXaxis()->SetTitle("E_{CsI} (MeV)");
    h1->GetYaxis()->SetTitle("E_{Si} (MeV)");
    h1->SetTitle("");
    
    mainC2->cd();
    chain->Draw("ELab:ThetaLab>>h2(1000,0,50,1000,0,200)","","colz");
    h2->SetMinimum(1);
    TH2F* h2 = (TH2F*)gDirectory->FindObjectAny("h2");
    h2->GetXaxis()->SetTitle("#theta_{lab} (deg)");
    h2->GetYaxis()->SetTitle("E (MeV)");
    h2->SetTitle("");
    
    //NPL::Reaction *r = new NPL::Reaction("34Ar(p,d)33Ar@2380");
    NPL::Reaction *r = new NPL::Reaction("46Ar(p,d)45Ar@3220");
    kin = r->GetKinematicLine3();
    
    kin->SetLineColor(2);
    //kin->Draw("plsame");
    
    mainC3->cd();
    chain->Draw("PhiLab:ThetaLab>>h3(100,0,50,720,-180,180)","","colz");
    TH2F* h3 = (TH2F*)gDirectory->FindObjectAny("h3");
    h3->GetXaxis()->SetTitle("#theta_{lab} (deg)");
    h3->GetYaxis()->SetTitle("#phi_{lab} (MeV)");
    h3->SetTitle("");
    
    mainC4->cd();
    chain->Draw("fIC_Incident_Position_Y:fIC_Incident_Position_X>>h4(1000,-10,10,100,-10,10)","","colz");
    TH2F* h4 = (TH2F*)gDirectory->FindObjectAny("h4");
    h4->GetXaxis()->SetTitle("X_{beam} (mm)");
    h4->GetYaxis()->SetTitle("Y_{beam} (mm)");
    h4->SetTitle("");

    return;
}


//////////////////////////////////////////////////
void InitChain()
{
    if(chain!=NULL){delete chain;}
    chain = new TChain("AnalysedTree","");
    
    chain->Add("/Users/pierremorfouace/Physics/NPTool/nptool/Outputs/Analysis/46Ar_pd.root");
    
    return;
}