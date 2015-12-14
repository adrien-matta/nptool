void Show(){
TF1* f = new TF1("f","sin(x*3.14159/180.)",0,180);
TFile* ResultTreeFile= new TFile("../../Outputs/Analysis/74Kr_g.root");
TTree* ResultTree = (TTree*) ResultTreeFile->FindObjectAny("ResultTree");
ResultTree->Draw("ThetaCM>>h(36,0,180)","MUST2.Si_E@.size()==0","",ResultTree->GetEntries()*0.03);
TH1F* h = (TH1F*) gDirectory->FindObjectAny("h");
h->Sumw2();
TFile* Feff = new TFile("Eff74Kr.root");
TH1F* eff = (TH1F*) Feff->FindObjectAny("hDetecThetaCM");
eff->Sumw2();
eff->Rebin(5);
double hnorm = h->Integral();
h->Divide(eff);
h->Divide(f);
h->SetMarkerColor(kBlack);
h->SetLineColor(kBlack);

h->Draw();

/*TFile* ResultTreeFile2= new TFile("../../Outputs/Analysis/74Kr_f.root");
TTree* ResultTree2 = (TTree*) ResultTreeFile2->FindObjectAny("ResultTree");
ResultTree2->Draw("ThetaCM>>h2(36,0,180)","MUST2.Si_E@.size()==0","same");
TH1F* h2 = (TH1F*) gDirectory->FindObjectAny("h2");
h2->Sumw2();
h2->Scale(hnorm/h2->Integral());
h2->Divide(eff);
h2->SetMarkerColor(kRed);
h2->SetLineColor(kRed);
h2->Draw("same");*/



TGraph* g = new TGraph("../../Inputs/CrossSection/CS_74Krdp_g.txt");
double* x = g->GetX();
double* y = g->GetY();
double norm = h->GetBinContent(h->FindBin(37.5))/g->Eval(37.5);

for(unsigned int i = 0 ; i < g->GetN() ; i++){
 g->SetPoint(i,x[i],y[i]*norm);
}
g->SetLineColor(kAzure+7);
g->Draw("c");

TGraph* g2 = new TGraph("../../Inputs/CrossSection/CS_74Krdp_p.txt");
x = g2->GetX();
y = g2->GetY();
norm = h->GetBinContent(h->FindBin(37.5))/g2->Eval(37.5);

for(unsigned int i = 0 ; i < g2->GetN() ; i++){
 g2->SetPoint(i,x[i],y[i]*norm);
}
g2->SetLineColor(kOrange+7);
g2->Draw("c");

TGraph* g3 = new TGraph("../../Inputs/CrossSection/CS_74Krdp_f.txt");
x = g3->GetX();
y = g3->GetY();
norm = h->GetBinContent(h->FindBin(37.5))/g3->Eval(37.5);

for(unsigned int i = 0 ; i < g3->GetN() ; i++){
 g3->SetPoint(i,x[i],y[i]*norm);
}
g3->SetLineColor(kGreen-3);
g3->Draw("c");

gPad->SetLogy();
}
