void Si(){
  
  TFile* file = new TFile("../../Outputs/Analysis/S1554_28Sidp_Phy.root");
  TTree* tree = (TTree*) file->FindObjectAny("PhysicsTree");
  tree->SetAlias("GoodTiming","Gamma_Time-Sharc.Strip_T>-1000 && Gamma_Time-Sharc.Strip_T<1000") ;
  NPL::Reaction r("28Si(d,p)29Si@221.623");
  NPL::Reaction d("28Si(d,d)28Si@221.623");
  NPL::Reaction p("28Si(p,p)28Si@221.623");

  TCanvas* c = new TCanvas("Result","Result",1000,1000);
  c->Divide(3,3);
  TH1* h;

//  c->cd(1);
  tree->Draw("Ex:AddBack_DC/1000.>>h2(2000,0,8,300,0,8)","ELab>0 && ThetaLab>90&&GoodTiming && AddBack_DC@.size()==1","colz");
  h = (TH1*) gDirectory->FindObjectAny("h2");
  h->GetXaxis()->SetTitle("E_{#gamma} (MeV)");
  h->GetYaxis()->SetTitle("E_{29Si} (MeV)");
  gPad->SetLogz(); 
  TLine* line = new TLine(0,0,8,8);
  line -> Draw("c"); 
return;
  c->cd(2);
  tree->Draw("Ex:ThetaLab>>hEXT(180,0,180,300,-5,8)","ELab>0","colz");
  gPad->SetLogz();
  h = (TH1*) gDirectory->FindObjectAny("hEXT");
  h->GetXaxis()->SetTitle("#theta_{Lab} (deg)");
  h->GetYaxis()->SetTitle("E_{29Si} (MeV)");

  c->cd(3);
  tree->Draw("ELab:ThetaLab>>hEXT2(360,0,180,1200,0,20)","ELab>0","colz");
  gPad->SetLogz();
  r.GetKinematicLine3()->Draw("c");
  r.SetExcitationHeavy(4.9336);
  r.GetKinematicLine3()->Draw("c"); 
  d.GetKinematicLine3()->Draw("c"); 
  p.GetKinematicLine3()->Draw("c"); 
  
  h = (TH1*) gDirectory->FindObjectAny("hEXT2");
  h->GetXaxis()->SetTitle("#theta_{Lab} (deg)");
  h->GetYaxis()->SetTitle("E_{Lab} (MeV)");

  c->cd(4);
  tree->Draw("AddBack_DC/1000>>hDC(800,0,8)","ELab>0&& ThetaLab>90 && Ex>0.8 && Ex<1.6 && AddBack_Theta >110","");
  gPad->SetLogz();
  h = (TH1*) gDirectory->FindObjectAny("hDC");
  h->GetXaxis()->SetTitle("E_{#gamma} (MeV)");
  h->GetYaxis()->SetTitle("counts / 10 keV");

  c->cd(5);
  tree->Draw("AddBack_DC/1000>>hDC2(1600,0,8)","ELab>0&& ThetaLab>90 && Ex>3 && Ex<5.5 && AddBack_Theta >110 && AddBack_Theta < 130","");

  gPad->SetLogz();
  h = (TH1*) gDirectory->FindObjectAny("hDC2");
  h->GetYaxis()->SetTitle("E_{#gamma} (MeV)");
  h->GetXaxis()->SetTitle("#theta_{gamma}");
}
