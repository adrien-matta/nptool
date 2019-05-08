#include "NPReaction.h"
#include "NPNucleus.h"
TCutG* ETOF=NULL;
TCutG* EDE=NULL;
TChain* chain=NULL ;

////////////////////////////////////////////////////////////////////////////////
void LoadCuts(){
    TFile* File_ETOF = new TFile("cuts/ETOF.root","READ");
    ETOF = (TCutG*) File_ETOF->FindObjectAny("ETOF");
    
    TFile* File_EDE = new TFile("cuts/EDE.root","READ");
    EDE= (TCutG*) File_EDE->FindObjectAny("EDE");
}

////////////////////////////////////////////////////////////////////////////////
void LoadChain(){
    chain = new TChain("PhysicsTree");
    chain->Add("../../Outputs/Analysis/Example1.root");
}

////////////////////////////////////////////////////////////////////////////////
void ShowResults(){
    LoadChain();
    LoadCuts();
    
    TCanvas* c1 = new TCanvas("Detected","Detected",0,0,600,600);
    c1->Divide(2,2);
    
    // Light Particle ID //
    // E-DE
    c1->cd(1);
    chain->Draw("SSSD.Energy:MUST2.Si_E>>hIDE(1000,0,35,1000,0,5)","MUST2.CsI_E<0 && MUST2.TelescopeNumber<5","colz");
    TH1F* hIDE = (TH1F*) gDirectory->FindObjectAny("hIDE");
    hIDE->GetYaxis()->SetTitle("SSSD Energy (MeV)");
    hIDE->GetXaxis()->SetTitle("MUST2 Energy (MeV)");
  
    EDE->Draw("same");
    
    // E-TOF
    c1->cd(2);
    chain->Draw("MUST2.Si_T:SSSD.Energy+MUST2.Si_E>>hIDT(1000,0,35,500,450,500)","MUST2.CsI_E<0 && MUST2.TelescopeNumber<5","colz");
    TH1F* hIDT = (TH1F*) gDirectory->FindObjectAny("hIDT");
    hIDT->GetYaxis()->SetTitle("TOF (ns)");
    hIDT->GetXaxis()->SetTitle("Energy (MeV)");
  

    ETOF->Draw("same");
    
    // Kinematical Line //
    c1->cd(3);
    chain->Draw("ELab:ThetaLab>>h(1000,0,90,1000,0,30)","MUST2.CsI_E<0 && MUST2.TelescopeNumber<5 && EDE && ETOF","colz");
    TH1F* h = (TH1F*) gDirectory->FindObjectAny("h");
    h->GetYaxis()->SetTitle("Energy (MeV)");
    h->GetXaxis()->SetTitle("Theta Lab(deg)");
    
    NPL::Reaction r("11Li(d,3He)10He@553");
    vector<string> v={"8He","n","n"};
    r.GetNucleus4()->DefineMassByThreshold(v);

    r.SetExcitationHeavy(1.4);
    TGraph* Kine = r.GetKinematicLine3();
    Kine->SetLineWidth(2);
    Kine->SetLineColor(kOrange-3);
    Kine->Draw("c");
    
    // Excitation Energy //
    c1->cd(4);
    int bin=50;
    double Emin = -5;
    double Emax = 5;
    
    chain->Draw(Form("Ex>>hEx(%d,%f,%f)",bin,Emin,Emax),"MUST2.CsI_E<0 && MUST2.TelescopeNumber<5 && EDE && ETOF");
    TH1F* hEx = (TH1F*) gDirectory->FindObjectAny("hEx");
    hEx->GetYaxis()->SetTitle(Form("counts / %d keV",(int) (1000*(Emax-Emin)/bin)));
    hEx->GetXaxis()->SetTitle("E_{10He}");
    hEx->SetFillStyle(1001);
    hEx->SetLineColor(kAzure+7);
    hEx->SetFillColor(kAzure+7);
    
    hEx->Fit("gaus");
    
    TF1* f = hEx->GetFunction("gaus");
    f->SetLineWidth(2);
    f->SetLineColor(kOrange-3);
    f->SetNpx(1000);
    
    TCanvas* c2 = new TCanvas("Simulated","Simulated",600,0,600,600);
    c2->Divide(2,2);
    
    c2->cd(1);
    chain->Draw("OriginalELab:OriginalThetaLab>>hS(1000,0,90,1000,0,30)","","col");
    TH1F* hS = (TH1F*) gDirectory->FindObjectAny("hS"); 
    hS->GetYaxis()->SetTitle("Simulated Energy (MeV)");
    hS->GetXaxis()->SetTitle("Simulated Theta Lab (deg)");  
    Kine->Draw("c");
    c2->cd(2);
    chain->Draw("OriginalELab:ELab>>hS2(1000,0,30,1000,0,30)","ELab>0","col");
    TH1F* hS2 = (TH1F*) gDirectory->FindObjectAny("hS2"); 
    hS2->GetYaxis()->SetTitle("Simulated Energy (MeV)");
    hS2->GetXaxis()->SetTitle("Reconstructed Energy (MeV)");  
 
    TLine* lE = new TLine(0,0,30,30);
    lE->Draw();
    c2->cd(3);
    chain->Draw("OriginalThetaLab:ThetaLab>>hS3(1000,0,90,1000,0,90)","ThetaLab>0","col");
    TH1F* hS3 = (TH1F*) gDirectory->FindObjectAny("hS3"); 
    hS3->GetYaxis()->SetTitle("Simulated Theta Lab (deg)");
    hS3->GetXaxis()->SetTitle("Reconstructed Theta Lab (deg)");  
    
    TLine* lT = new TLine(0,0,90,90);
    lT->Draw();
    c2->cd(4);
    chain->Draw("OriginalBeamEnergy:BeamEnergy>>hS4(1000,500,600,1000,500,600)","BeamEnergy>0","col");
    TH1F* hS4 = (TH1F*) gDirectory->FindObjectAny("hS4"); 
    hS4->GetYaxis()->SetTitle("Simulated Beam Energy (MeV)");
    hS4->GetXaxis()->SetTitle("Reconstructed Beam Energy (MeV)");  
  
    TLine* lB = new TLine(500,500,600,600);
    lB->Draw();
    


}
