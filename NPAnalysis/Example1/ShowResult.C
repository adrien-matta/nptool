#include"NPReaction.h"

TCutG* ETOF=NULL;
TCutG* EDE=NULL;
TChain* chain=NULL ;
TCanvas* c1 = NULL;

////////////////////////////////////////////////////////////////////////////////
void LoadCuts(){
TFile* File_ETOF = new TFile("cuts/ETOF.root","READ");
ETOF = (TCutG*) File_ETOF->FindObjectAny("ETOF");

TFile* File_EDE = new TFile("cuts/EDE.root","READ");
EDE= (TCutG*) File_EDE->FindObjectAny("EDE");
}

////////////////////////////////////////////////////////////////////////////////
void LoadChain(){
chain = new TChain("ResultTree");
chain->Add("../../Outputs/Analysis/Example1.root");
}

////////////////////////////////////////////////////////////////////////////////
void ShowResult(){
LoadChain();
LoadCuts();

c1 = new TCanvas("Example1","Example1",0,0,600,600);
c1->Divide(2,2);

// Light Particle ID //
// E-DE
c1->cd(1);
chain->Draw("SSSD.Energy:MUST2.Si_E>>hIDE(1000,0,35,1000,0,5)","MUST2.CsI_E<0 && MUST2.TelescopeNumber<5","colz");
EDE->Draw("same");

// E-TOF
c1->cd(2);
chain->Draw("-MUST2.Si_T:SSSD.Energy+MUST2.Si_E>>hIDT(1000,0,35,1000,-15,0)","MUST2.CsI_E<0 && MUST2.TelescopeNumber<5","colz");
ETOF->Draw("same");

// Kinematical Line //
c1->cd(3);
chain->Draw("ELab:ThetaLab>>hKine(1000,0,90,400,0,40)","MUST2.CsI_E<0 && MUST2.TelescopeNumber<5 && EDE && ETOF","colz");

NPL::Reaction r("11Li(d,3He)10He@553");
TGraph* Kine = r.GetKinematicLine3(); 
Kine->SetLineWidth(2);
Kine->SetLineColor(kOrange-3);
Kine->Draw("c");

// Excitation Energy //
c1->cd(4);
int bin=100;
double Emin = -10;
double Emax = 10;

chain->Draw(Form("Ex>>hEx(%d,%f,%f)",bin,Emin,Emax),"MUST2.CsI_E<0 && MUST2.TelescopeNumber<5 && EDE && ETOF");
TH1F* hEx = (TH1F*) gDirectory->FindObjectAny("hEx");
hEx->GetYaxis()->SetTitle(Form("counts / %d keV",(int) (1000*(Emax-Emin)/bin)));
hEx->GetXaxis()->SetTitle("E_{10He}");
hEx->SetFillStyle(1001);
hEx->SetLineColor(kAzure+7);
hEx->SetFillColor(kAzure+7);
}
