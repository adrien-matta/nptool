#include"NPReaction.h"

TCutG* ETOF=NULL;
TCutG* EDE=NULL;
TChain* chain=NULL ;
TCanvas* c1 = NULL;


////////////////////////////////////////////////////////////////////////////////
void LoadChain(){
chain = new TChain("PhysicsTree");
chain->Add("../../Outputs/Analysis/Example2.root");
}

////////////////////////////////////////////////////////////////////////////////
void ShowResult(){
LoadChain();

c1 = new TCanvas("Example2","Example2",0,0,800,800);
c1->Divide(2,2);

c1->cd(1);
chain->Draw("ELab:ThetaLab>>hKine(180,0,180,400,0,20)","ELab>0","colz");
TH2* hKine = (TH2*) gDirectory->FindObjectAny("hKine");
hKine->GetXaxis()->SetTitle("#theta_{lab} (deg)");
hKine->GetYaxis()->SetTitle("E_{lab} (MeV)");


NPL::Reaction r("28Mg(d,p)29Mg@222");
r.SetExcitationHeavy(0);
TGraph* Kine1 = r.GetKinematicLine3(); 
Kine1->SetLineWidth(2);
Kine1->SetLineColor(kOrange-3);
Kine1->Draw("c");

r.SetExcitationHeavy(1.1);
TGraph* Kine2 = r.GetKinematicLine3(); 
Kine2->SetLineWidth(2);
Kine2->SetLineColor(kOrange-3);
Kine2->Draw("c");

r.SetExcitationHeavy(1.4);
TGraph* Kine3 = r.GetKinematicLine3(); 
Kine3->SetLineWidth(2);
Kine3->SetLineColor(kOrange-3);
Kine3->Draw("c");

r.SetExcitationHeavy(2.3);
TGraph* Kine4 = r.GetKinematicLine3(); 
Kine4->SetLineWidth(2);
Kine4->SetLineColor(kOrange-3);
Kine4->Draw("c");

r.SetExcitationHeavy(4.4);
TGraph* Kine5 = r.GetKinematicLine3(); 
Kine5->SetLineWidth(2);
Kine5->SetLineColor(kOrange-3);
Kine5->Draw("c");




c1->cd(2);
chain->Draw("Ex:ThetaLab>>hExT(90,0,180,500,-1,7)","ELab>0","colz");
TH2* hExT = (TH2*) gDirectory->FindObjectAny("hExT");
hExT->GetXaxis()->SetTitle("#theta_{lab} (deg)");
hExT->GetYaxis()->SetTitle("E_{29Mg} (MeV)");

TLine* line1 = new TLine (0,0,180,0);
line1->SetLineWidth(2);
line1->SetLineColor(kOrange-3);
line1->Draw();

TLine* line2 = new TLine (0,1.1,180,1.1);
line2->SetLineWidth(2);
line2->SetLineColor(kOrange-3);
line2->Draw();

TLine* line3 = new TLine (0,1.4,180,1.4);
line3->SetLineWidth(2);
line3->SetLineColor(kOrange-3);
line3->Draw();


TLine* line4 = new TLine (0,2.3,180,2.3);
line4->SetLineWidth(2);
line4->SetLineColor(kOrange-3);
line4->Draw();

TLine* line5 = new TLine (0,4.4,180,4.4);
line5->SetLineWidth(2);
line5->SetLineColor(kOrange-3);
line5->Draw();



c1->cd(3);
chain->Draw("Ex>>hEx(500,-1,7)","ELab>0","");
TH2* hEx = (TH2*) gDirectory->FindObjectAny("hEx");
hEx->GetYaxis()->SetTitle("counts / 16 keV");
hEx->GetXaxis()->SetTitle("E_{29Mg} (MeV)");



}
