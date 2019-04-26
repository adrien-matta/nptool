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
chain = new TChain("PhysicsTree");
chain->Add("../../Outputs/Analysis/Example1.root");
}
////////////////////////////////////////////////////////////////////////////////
void LoadEventList(){
}
////////////////////////////////////////////////////////////////////////////////
void plot_kine(NPL::Reaction r, double Ex,Color_t c,int w, int s){
  r.SetExcitation4(Ex);

  TGraph* g= r.GetKinematicLine3();
  g->SetLineColor(c) ;
  g->SetLineStyle(s) ;
  g->SetLineWidth(w) ;

  g->Draw("c");
  }
////////////////////////////////////////////////////////////////////////////////
void plot_state(double Ex,double max,Color_t c,int w, int s){
  TLine* line = new TLine(Ex,0,Ex,max) ; 
  line->SetLineColor(c) ;
  line->SetLineStyle(s) ;
  line->SetLineWidth(w) ;
  line->Draw();

  }
////////////////////////////////////////////////////////////////////////////////
void Kine(){
// Load stuff
LoadChain();
LoadEventList();
//LoadCuts();

// for Kinematic calculation
NPL::Reaction O17("16O(d,p)17O@96");
NPL::Reaction O16dd("16O(d,d)16O@96");
NPL::Reaction O16pp("16O(p,p)16O@96");
NPL::Reaction O16C12("16O(12C,12C)16O@96");
NPL::Reaction O15("16O(d,t)15O@96");
Color_t color_dp=kOrange+7;
Color_t color_dd=kRed;
Color_t color_pp=kGreen;
Color_t color_dt=kAzure+7;
Color_t color_ca=kBlack;

c1 = new TCanvas("Example1","Example1",0,0,600,600);
c1->Divide(2,2);
// Kinematic Plot
c1->cd(1);
TH2F* hKine = new TH2F("hKine","hKine",1000,0,180,1000,0,40);
hKine->Draw("col");
hKine->GetXaxis()->SetTitle("#Theta_{Lab} (deg)");
hKine->GetYaxis()->SetTitle("E_{Lab} (MeV)");
chain->Draw("ELab:ThetaLab","","col");
// O17
plot_kine(O17,0,color_dp,2,1);
plot_kine(O17,0.87073,color_dp,2,1);
plot_kine(O17,3.05536,color_dp,1,1);
// Sn
plot_kine(O17,4.143,color_dp,1,2);

// O15
plot_kine(O15,0,color_dt,2,1);
//plot_kine(O15,5.183,color_dt,2,1);
//plot_kine(O15,5.241,color_dt,1,1);
//plot_kine(O15,6.176,color_dt,1,1);
// Sp
//plot_kine(O15,7.297,color_dt,1,2);
// Elastic
plot_kine(O16dd,0,color_dd,2,1);
plot_kine(O16pp,0,color_pp,1,1);
plot_kine(O16C12,0,color_ca,1,2);

// Kinematic Plot
c1->cd(2);
int bin = 1000;
double start=-10;
double end =  10;
double binning = (end-start)/bin;
TH1F* hEx = new TH1F("hEx","hEx",bin,start,end);
hEx->FillRandom("gaus", 10000);
hEx->GetXaxis()->SetTitle("E_{17O} (MeV)");
hEx->GetYaxis()->SetTitle(Form("counts / %.0f keV",binning*1000));

hEx->Draw();
double max = hEx->GetMaximum();
plot_state(0,max,color_dp,2,1);
plot_state(0.87073,max,color_dp,2,1);
plot_state(3.05536,max,color_dp,1,1);
plot_state(4.143,max,color_dp,1,2);



}
