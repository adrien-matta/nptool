void InitChain();
void LoadCut();

TChain *chain;
TGraph *kin;

int nentries;

TCutG *cut_kine;


void Show()
{
    gStyle->SetOptStat(0);
    InitChain();
    LoadCut();
    
    nentries = chain->GetEntries();
    
    //**************** Condition ****************//
    TString CondCut = "cut_kine";
    TString CondTot = CondCut;//+"&&"+
    //*******************************************//
    
    TCanvas* mainC0  = new TCanvas("Hira XY", "Hira XY" , 800,600);
    TCanvas* mainC1  = new TCanvas("PID", "PID" , 1200,600);
    TCanvas* mainC2  = new TCanvas("Kinematics", "Kinematics" , 800,600);
    TCanvas* mainC3  = new TCanvas("Phi-Theta", "Phi-Theta" , 800,600);
    TCanvas* mainC4  = new TCanvas("BeamSpot", "BeamSpot" , 800,600);
    TCanvas* mainC5  = new TCanvas("ThetaCM", "ThetaCM" , 800,600);
    TCanvas* mainC6  = new TCanvas("Efficiency", "Efficiency" , 800,600);
    TCanvas* mainC7  = new TCanvas("CrossSection","CrossSection",800,600);
    mainC1->Divide(2,1);

    mainC0->cd();
    chain->Draw("Y:X>>h0(300,-300,300,200,-200,200)","","colz");
    TH2F* h0 = (TH2F*)gDirectory->FindObjectAny("h0");
    h0->GetXaxis()->SetTitle("X (mm)");
    h0->GetYaxis()->SetTitle("Y (mm)");
    h0->SetTitle("");
    
    
    mainC1->cd(1);
    chain->Draw("E_ThickSi:E_CsI>>h1(1000,0,200,1000,0,25)","","colz");
    TH2F* h1 = (TH2F*)gDirectory->FindObjectAny("h1");
    h1->GetXaxis()->SetTitle("E_{CsI} (MeV)");
    h1->GetYaxis()->SetTitle("E_{Si} (MeV)");
    h1->SetTitle("");

    mainC1->cd(2);
    chain->Draw("E_ThinSi:E_ThickSi>>h12(1000,0,25,1000,0,5)","","colz");
    TH2F* h12 = (TH2F*)gDirectory->FindObjectAny("h12");
    h12->GetXaxis()->SetTitle("E_{Si} (MeV)");
    h12->GetYaxis()->SetTitle("#Delta E (MeV)");
    h12->SetTitle("");

    
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
    
    mainC5->cd();
    chain->Draw("ThetaCM>>h5(25,0,100)",CondTot,"E1");
    TH1F* h5 = (TH1F*)gDirectory->FindObjectAny("h5");
    h5->GetXaxis()->SetTitle("#theta_{CM} (deg)");
    h5->SetTitle("");
    
    
    //Efficiency calculation
    mainC6->cd();
    TH1F *Efficiency = new TH1F("Efficiency", "Efficiency", 25, 0, 100);
    Efficiency->SetXTitle("#theta_{CM}");
    Efficiency->SetTitle("Efficiency");
    Efficiency->Add(h5,4*TMath::Pi()/nentries);
    Efficiency->Draw();
    
    // Cross-Section
    mainC7->cd();
    mainC7->SetLogy();
    TH1F *h7 = new TH1F("h7", "h7", 25, 0, 100);
    h7->SetXTitle("#theta_{CM}");
    h7->SetYTitle("d#sigma/d#Omega (mb/sr)");
    h7->SetTitle("");
    h7->Divide(h5,Efficiency,1,nentries/(4*TMath::Pi()));//262000
    h7->Draw();


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



///////////////////////////////////////////////////////////////////////////////
void LoadCut()
{
    TString Cut_Name = "CUT/cut_kine.root";
    TString Object_Name = "cut_kine";
    TFile* f_cut_kine = new TFile(Cut_Name,"read");
    cut_kine = (TCutG*) f_cut_kine->FindObjectAny(Object_Name);
    
    return;
}









