void Show(){

TFile* file = new TFile("../../Outputs/Analysis/PhysicsTree.root","READ");
TTree* PhysicsTree = (TTree*) file->FindObjectAny("PhysicsTree");
NPL::Reaction r("17N(d,p)18N@93");
TH1* h;

TCanvas* c ;
c = new TCanvas();
PhysicsTree->Draw("ELab:ThetaLab>>h2(360,0,180,1000,0,30)","Ex>-1000","colz");
r.GetKinematicLine3()->Draw("c");
h = (TH1*) gDirectory->FindObjectAny("h2");
h->GetXaxis()->SetTitle("E_{Lab} (MeV)");
h->GetYaxis()->SetTitle("#theta_{Lab} (deg)");
c->SaveAs("~/Dropbox/Presentation\ and\ Poster/ISOLDE_2016/image/Kine.pdf");

c = new TCanvas();
PhysicsTree->Draw("Ex>>h3(900,-2,7)","Ex>-1000","colz");
h = (TH1*) gDirectory->FindObjectAny("h3");
h->GetXaxis()->SetTitle("E_{18N} (MeV)");
h->GetYaxis()->SetTitle("counts / 10 keV");
c->SaveAs("~/Dropbox/Presentation\ and\ Poster/ISOLDE_2016/image/Ex.pdf");

c = new TCanvas();
PhysicsTree->Draw("EDC>>h4(2000,0,2)","Ex>-1000","colz");
h = (TH1*) gDirectory->FindObjectAny("h4");
h->GetXaxis()->SetTitle("E_{#gamma} DC (MeV)");
h->GetYaxis()->SetTitle("counts / 1 keV");
gPad->SetLogy();
c->SaveAs("~/Dropbox/Presentation\ and\ Poster/ISOLDE_2016/image/EDC.pdf");

c = new TCanvas();
PhysicsTree->Draw("Ex:EDC>>h1(1000,0,2.5,1000,-1,2.5)","Ex>-1000","colz");
TLine* line = new TLine(0,0,2.5,2.5);
line->Draw();
h = (TH1*) gDirectory->FindObjectAny("h1");
h->GetYaxis()->SetTitle("E_{18N} (MeV)");
h->GetXaxis()->SetTitle("E_{#gamma} DC (MeV)");
c->SaveAs("~/Dropbox/Presentation\ and\ Poster/ISOLDE_2016/image/ExEg.pdf");
TFile* fcut = new TFile("cut.root","READ");
TCutG* cut = (TCutG*) fcut->FindObjectAny("CUTG");
cut->Draw();
cut->SetLineWidth(3);
c->SaveAs("~/Dropbox/Presentation\ and\ Poster/ISOLDE_2016/image/ExEg2.pdf");
delete cut;
fcut = new TFile("cut2.root","READ");
cut = (TCutG*) fcut->FindObjectAny("CUTG2");
cut->Draw();
cut->SetLineWidth(3);
c->SaveAs("~/Dropbox/Presentation\ and\ Poster/ISOLDE_2016/image/ExEg3.pdf");
delete cut;
fcut = new TFile("cut3.root","READ");
cut = (TCutG*) fcut->FindObjectAny("CUTG3");
cut->Draw();
cut->SetLineWidth(3);
c->SaveAs("~/Dropbox/Presentation\ and\ Poster/ISOLDE_2016/image/ExEg4.pdf");


c = new TCanvas();
cout << PhysicsTree->Draw("ThetaCM>>h5(36,0,180)","Ex>-1000 && EDC > 0.6 && EDC < 0.65","colz") << endl;
h = (TH1*) gDirectory->FindObjectAny("h5");
h->GetYaxis()->SetTitle("d#sigma/d#Omega (mb/sr)");
h->GetXaxis()->SetTitle("#theta_{CM}(deg)");
TFile* feff = new TFile("TRex_eff4.root","READ");
TH1* eff = (TH1*) feff->FindObjectAny("hDetecThetaCM");
eff->Rebin(5);
h->Divide(eff);

TGraph* g = new TGraph("CS/18N_4.txt");
TGraph* g0 = new TGraph("CS/18N_5.txt");
g0->Draw("c");
g0->SetLineColor(kAzure+7);
g->SetLineWidth(3);
g0->SetLineWidth(3);

double norm = h->GetBinContent(h->FindBin(12))/g->Eval(12);
h->Scale(1/norm);
h->GetYaxis()->SetRangeUser(1e-2,350);

h->SetLineColor(kOrange+7);
g->Draw("c");
gPad->SetLogy();
TLatex *   tex = new TLatex(86.5,110,"600 < E_{#gamma} < 650 keV");
tex->Draw();
leg = new TLegend(0.5,0.6,1,0.85);
leg->SetBorderSize(0);
leg->SetFillStyle(0);
leg->SetHeader("742 keV (3^{- }) state");
leg->AddEntry(g,"L=2","l");
leg->AddEntry(g0,"L=0","l");
leg->Draw();
c->SaveAs("~/Dropbox/Presentation\ and\ Poster/ISOLDE_2016/image/CS_742.pdf");

c = new TCanvas();
cout << PhysicsTree->Draw("ThetaCM>>h6(36,0,180)","Ex>-1000 && EDC > 0.7 && EDC < 0.8","colz") << endl;
h = (TH1*) gDirectory->FindObjectAny("h6");
h->GetYaxis()->SetTitle("d#sigma/d#Omega (mb/sr)");
h->GetXaxis()->SetTitle("#theta_{CM}(deg)");

//feff = new TFile("TRex_eff6.root","READ");
//eff = (TH1*) feff->FindObjectAny("hDetecThetaCM");
//eff->Rebin(5);
h->Divide(eff);
h->SetLineColor(kOrange+7);

g2 = new TGraph("CS/18N_5.txt");
g3 = new TGraph("CS/18N_4.txt");
g3->SetLineColor(kAzure+7);
norm = h->GetBinContent(h->FindBin(12))/g2->Eval(12);
h->Scale(1/norm);
h->GetYaxis()->SetRangeUser(1e-2,350);

g2->Draw("c");
g2->SetLineWidth(3);
g3->Draw("c");
g3->SetLineWidth(3);
gPad->SetLogy();
tex = new TLatex(86.5,110,"700 < E_{#gamma} < 800 keV");
tex->Draw();
leg = new TLegend(0.5,0.6,1,0.85);
leg->SetHeader("747 keV (1^{- }) state");
leg->SetBorderSize(0);
leg->SetFillStyle(0);

leg->AddEntry(g2,"L=0","l");
leg->AddEntry(g3,"L=2","l");
leg->Draw();
c->SaveAs("~/Dropbox/Presentation\ and\ Poster/ISOLDE_2016/image/CS_747.pdf");

c = new TCanvas();
cout << PhysicsTree->Draw("ThetaCM>>h7(36,0,180)","Ex>-1000 && EDC > 1.7 && EDC < 1.8","colz") << endl;
h = (TH1*) gDirectory->FindObjectAny("h7");
h->GetYaxis()->SetTitle("d#sigma/d#Omega (mb/sr)");
h->GetXaxis()->SetTitle("#theta_{CM}(deg)");

feff = new TFile("TRex_eff6.root","READ");
eff = (TH1*) feff->FindObjectAny("hDetecThetaCM");
eff->Rebin(5);
h->Divide(eff);
h->SetLineColor(kOrange+7);

g2 = new TGraph("CS/18N_6.txt");
g3 = new TGraph("CS/18N_4.txt");
g3->SetLineColor(kAzure+7);
norm = h->GetBinContent(h->FindBin(12))/g2->Eval(12);
h->Scale(1/norm);
h->GetYaxis()->SetRangeUser(1e-2,350);

g2->Draw("c");
g2->SetLineWidth(3);
g3->Draw("c");
g3->SetLineWidth(3);
gPad->SetLogy();
tex = new TLatex(86.5,110,"1700 < E_{#gamma} < 1800 keV");
tex->Draw();
leg = new TLegend(0.5,0.6,1,0.85);
leg->SetHeader("1735 keV (1^{- }) state");
leg->SetBorderSize(0);
leg->SetFillStyle(0);

leg->AddEntry(g2,"L=0","l");
leg->AddEntry(g3,"L=2","l");
leg->Draw();
c->SaveAs("~/Dropbox/Presentation\ and\ Poster/ISOLDE_2016/image/CS_1735.pdf");

}
