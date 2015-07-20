void Show(){
  
  TFile* file = new TFile("../../Outputs/Analysis/S1554_Si.root");
  TTree* tree = (TTree*) file->FindObjectAny("PhysicsTree");

  TCanvas* c = new TCanvas("Result","Result",600,600);
  c->Divide(2,2);

  c->cd(1);
  tree->Draw("Ex:AddBack_DC/1000.>>h(500,0,10,500,-5,10)","ELab>0 && ThetaLab>90","colz");
 
  c->cd(2);
  tree->Draw("Ex>>hE(150,-5,10)","ELab>0 && ThetaLab>90");
  
  c->cd(3);
  tree->Draw("ThetaCM>>hCM(36,0,180)","ThetaLab>90 && Ex>-0.6 && Ex< 0.2","");
  //tree->Draw("ThetaCM>>hCMb(36,0,180)","ThetaLab>90 && Ex>-0.8 && Ex< 0.2","same");

  TH1F* hCM = (TH1F*) gDirectory->FindObjectAny("hCM");
  //TH1F* hCMb = (TH1F*) gDirectory->FindObjectAny("hCMb");
 // hCMb->SetMarkerColor(kGreen-3);
//  hCMb->Sumw2();

  TFile* effFile = new TFile("SharcEff.root");
  hCM->Sumw2();
  TH1F* eff = (TH1F*) effFile->FindObjectAny("hDetecThetaCM");
  eff->Sumw2();
  eff->Rebin(5);
  hCM->Divide(eff);
  hCM->GetXaxis()->SetRangeUser(0,60);
  gPad->SetLogy();

  TGraph* g = new TGraph("22.si");
  g->Draw("c"); 
  double normal = hCM->GetBinContent(hCM->FindBin(8))/g->Eval(8);
  TF1* f = new TF1("f",Form("%f",normal),0,180);
   hCM->Divide(f);
  /*double normalb = hCM->GetBinContent(hCM->FindBin(8))/hCMb->GetBinContent(hCMb->FindBin(8));
  TF1* fb = new TF1("fb",Form("%f",1./normalb),0,180);
  hCMb->Divide(fb); 
*/
  c->cd(4);
  tree->Draw("ThetaCM>>hCM2(36,0,180)","ThetaLab>90 && Trifoil.Time>50 && Trifoil.Time<65 && Ex>4 && Ex< 5","");
  TH1F* hCM2 = (TH1F*) gDirectory->FindObjectAny("hCM2"); 
  hCM2->Sumw2();
  hCM2->Divide(eff);              
  hCM2->GetXaxis()->SetRangeUser(0,60);  
  gPad->SetLogy();  

  TGraph* g2 = new TGraph("22.sie");
  g2->Draw("c"); 
  normal = hCM2->GetBinContent(hCM->FindBin(8))/g2->Eval(8);
  TF1* f2 = new TF1("f2",Form("%f",normal),0,180);
  hCM2->Divide(f2);

}
