////////////////////////////////////////////////////////////////////////////////
void makeMgCS(){
  TFile* file = new TFile("../../Outputs/Analysis/S1554_28Mgdp_Phy.root");
  TTree* tree = (TTree*) file->FindObjectAny("PhysicsTree");

  tree->SetAlias("GoodEvent","(RunNumber>35110 && ELab > 0)");
  tree->SetAlias("GoodTrifoil","(Trifoil.Time@.size()==1 && Trifoil.Time>145 && Trifoil.Time<160)") ;
  tree->SetAlias("BadTrifoil","(Trifoil.Time@.size()==1 && (Trifoil.Time<145 || Trifoil.Time>160) && Trifoil.Time<300) ") ;
  tree->SetAlias("GoodProton","(ThetaLab>90 &&( (Sharc.Strip_E>0.8&&ThetaCM<10)||(Sharc.Strip_E>1 &&ThetaCM>10) ))");

  TCanvas* c0 = new TCanvas("Mg0","Mg0",800,800*2./5.); 
  c0->Divide(5,2);
  c0->cd(1);
  tree->Draw("Ex:ThetaCM>>hexcmG(72,0,180,400,-8,8)","GoodTrifoil && GoodProton && GoodEvent","colz");
  TH2D* h = (TH2D*) gDirectory->FindObjectAny("hexcmG");
  h->GetYaxis()->SetTitle("GOOD E_{29Mg} (MeV)");
  h->GetXaxis()->SetTitle("GOOD #theta_{CM} (deg)");
  h->GetXaxis()->SetRangeUser(-1,50);
  h->SaveAs("GoodExThetaCM.root");
  
  c0->cd(2);
  tree->Draw("Ex:ThetaCM>>hexcmB(72,0,180,400,-8,8)","BadTrifoil && GoodProton && GoodEvent","colz");
  h = (TH2D*) gDirectory->FindObjectAny("hexcmB");
  h->GetYaxis()->SetTitle("BAD E_{29Mg} (MeV)");
  h->GetXaxis()->SetTitle("BAD #theta_{CM} (deg)");
  h->GetXaxis()->SetRangeUser(-1,50);
 // h->Scale(7./(145-1+300-160));
  h->SaveAs("BadExThetaCM.root");

  c0->cd(3);
  tree->Draw("Ex:ThetaCM>>hexcmB2(72,0,180,400,-8,8)","Trifoil.Time@.size()==0 && GoodProton && GoodEvent","colz");
  h = (TH2D*) gDirectory->FindObjectAny("hexcmB2");
  h->GetYaxis()->SetTitle("BAD E_{29Mg} (MeV)");
  h->GetXaxis()->SetTitle("BAD #theta_{CM} (deg)");
  h->GetXaxis()->SetRangeUser(-1,50);
 // h->Scale(7./(145-1+300-160));
  h->SaveAs("BadExThetaCM2.root");

/*
  c0->cd(4);
  tree->Draw("Ex>>hexG(400,-8,8)","GoodTrifoil && GoodProton && GoodEvent","");
  TH1D* hh = (TH1D*) gDirectory->FindObjectAny("hexG");
  h->GetXaxis()->SetTitle("E_{29Mg} (MeV)");
  tree->Draw("Ex>>hexB(400,-8,8)","BadTrifoil && GoodProton && GoodEvent","same");
  hh = (TH1D*) gDirectory->FindObjectAny("hexB");
  hh->SetFillColor(kOrange+7);
  hh->SetLineColor(kOrange+7); 
  hh->SetFillStyle(3004); 
//  hh->Scale(7./(145-1+300-160));

  c0->cd(5);
  tree->Draw("Trifoil.Time>>htt(300,0,300)","GoodProton && GoodEvent","");  
  tree->Draw("Trifoil.Time>>hgt(300,0,300)","GoodTrifoil && GoodProton && GoodEvent ","same");  
  tree->Draw("Trifoil.Time>>hbt(300,0,300)","BadTrifoil && GoodProton && GoodEvent ","same");  

  TH1D* hgt = (TH1D*) gDirectory->FindObjectAny("hgt");
  hgt->SetFillColor(kOrange+7);
  hgt->SetLineColor(kOrange+7);   
  hgt->SetFillStyle(3004); 
  TH1D* hbt = (TH1D*) gDirectory->FindObjectAny("hbt");
  hbt->SetFillColor(kGreen-3);
  hbt->SetLineColor(kGreen-3);   
  hbt->SetFillStyle(3004); 


  
  c0->cd(6);
  tree->Draw("Trifoil.Time:ThetaLab>>hte(360,0,360,300,0,300)","GoodProton && GoodEvent","colz");  
  TH1D* hte = (TH1D*) gDirectory->FindObjectAny("hte");
  hte->GetXaxis()->SetTitle("Sharc E (MeV)");
  hte->GetYaxis()->SetTitle("Trifoil Time (a.u.)");

  c0->cd(7);
  tree->Draw("Ex>>hexSi(400,-8,8)","Trifoil.Time@.size()==0 && GoodProton && GoodEvent",""); 
  hgt = (TH1D*) gDirectory->FindObjectAny("hexSi"); 
  TH1D* hhh = new TH1D(*hh);
  hhh->Scale(hgt->GetEntries()/hhh->GetEntries());
  hhh->Draw("same");

  c0->cd(8);
  tree->Draw("Ex>>hexSi2(400,-8,8)","Trifoil.Time@.size()==0 && GoodProton && GoodEvent && AddBack_DC/1000.>4 && AddBack_DC/1000.<6",""); 

 // new TCanvas();
//  tree->Draw("AddBack_DC>>hGAL(2000,0,8000)","GoodTrifoil && Ex<0 && Ex>-1000 && ThetaLab>90");
new TCanvas();
//  tree->Draw("Ex:ThetaCM>>hexcmG(90,0,180,400,-8,8)","GoodTrifoil && GoodProton && GoodEvent","colz");
  tree->Draw("Ex","GoodTrifoil && GoodProton && GoodEvent && AddBack_DC/1000.> 1 && AddBack_DC/1000.<1.2 && Ex>-1000","");
  */
}
