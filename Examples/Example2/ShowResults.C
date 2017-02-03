#include"NPReaction.h"

////////////////////////////////////////////////////////////////////////////////
TChain*  LoadChain(){
TChain* chain = new TChain("PhysicsTree");
chain->Add("../../Outputs/Analysis/Example2.root");
return chain;
}

////////////////////////////////////////////////////////////////////////////////
void ShowResults(){
gROOT->SetStyle("nptool"); 
gROOT->ForceStyle();
TChain* chain = LoadChain();

TCanvas* c1 = new TCanvas("Example2","Example2",0,0,800,800);
c1->Divide(2,2);
// Kinematic Line
c1->cd(1);
chain->Draw("ELab:ThetaLab>>hKine(180,0,180,400,0,20)","ELab>0","colz");
TH2* hKine = (TH2*) gDirectory->FindObjectAny("hKine");
hKine->GetXaxis()->SetTitle("#theta_{lab} (deg)");
hKine->GetYaxis()->SetTitle("E_{lab} (MeV)");

NPL::Reaction r("28Mg(d,p)29Mg@222");
r.SetExcitationHeavy(0);
TGraph* Kine1 = r.GetKinematicLine3(); 
Kine1->Draw("c");

r.SetExcitationHeavy(1.1);
TGraph* Kine2 = r.GetKinematicLine3(); 
Kine2->Draw("c");

r.SetExcitationHeavy(1.4);
TGraph* Kine3 = r.GetKinematicLine3(); 
Kine3->Draw("c");

r.SetExcitationHeavy(2.3);
TGraph* Kine4 = r.GetKinematicLine3(); 
Kine4->Draw("c");

r.SetExcitationHeavy(4.4);
TGraph* Kine5 = r.GetKinematicLine3(); 
Kine5->Draw("c");

// Ex vs Theta Lab
c1->cd(2);
chain->Draw("Ex:ThetaLab>>hExT(90,0,180,500,-1,7)","ELab>0","colz");
TH2* hExT = (TH2*) gDirectory->FindObjectAny("hExT");
hExT->GetXaxis()->SetTitle("#theta_{lab} (deg)");
hExT->GetYaxis()->SetTitle("E_{29Mg} (MeV)");

TLine* line1 = new TLine (0,0,180,0);
line1->Draw();

TLine* line2 = new TLine (0,1.1,180,1.1);
line2->Draw();

TLine* line3 = new TLine (0,1.4,180,1.4);
line3->Draw();

TLine* line4 = new TLine (0,2.3,180,2.3);
line4->Draw();

TLine* line5 = new TLine (0,4.4,180,4.4);
line5->Draw();

// Excitation Energy
c1->cd(3);
chain->Draw("Ex>>hEx(500,-1,7)","ELab>0","");
TH2* hEx = (TH2*) gDirectory->FindObjectAny("hEx");
hEx->GetXaxis()->SetTitle("E_{29Mg} (MeV)");
hEx->GetYaxis()->SetTitle("counts / 16 keV");

}
