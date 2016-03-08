void Mg(){

  TFile* file = new TFile("../../Outputs/Analysis/S1554_28Mgdp_Phy.root");
  TTree* tree = (TTree*) file->FindObjectAny("PhysicsTree");

  file = new TFile("cuts/Cut_pp.root");
  TCutG* Cut_pp = (TCutG*) file->FindObjectAny("Cut_pp");

  tree->SetAlias("GoodTrifoil","(Trifoil.Time>150 && Trifoil.Time<160 && RunNumber>35110) && ELab > 0 ") ;
  tree->SetAlias("GoodProton","ThetaLab>90 && Sharc.Strip_E>0.8");
  TCanvas* c = new TCanvas("Mg","Mg");
  c->Divide(3,3);

  // Setting Trifoil entry list for fast drawing
  TFile* fileEL = new TFile("macro/GoodTrifoil.root");
  TEntryList* EL = (TEntryList*) fileEL->FindObjectAny("TrifoilEL");
  if(!EL){
    cout << "Trifoil Entry List does not exist, generating : ";
    tree->Draw(">>TrifoilEL","GoodTrifoil","entrylist"); 
    EL = (TEntryList*) gDirectory->FindObjectAny("TrifoilEL");
    cout << " done" << endl;
    EL->SaveAs("macro/GoodTrifoil.root"); 
  }

  cout <<"Setting Trifoil entry list :" ;
  tree->SetEntryList(EL);
  cout << " Done" << endl;


  c->cd(1);
  tree->Draw("Ex:AddBack_DC/1000.>>h2(2000,0,8,200,-1,8)","ThetaLab>90 && GoodProton","colz");
  TH1* h2 = (TH1*) gDirectory->FindObjectAny("h2");
  h2->GetXaxis()->SetTitle("E_{#gamma} (MeV)");
  h2->GetYaxis()->SetTitle("E_{29Mg} (MeV)");
  gPad->SetLogz(); 
  TLine* line = new TLine(0,0,8,8);
  line -> Draw("c"); 

  c->cd(2);
  tree->Draw("Ex>>h(200,-1,8)","GoodTrifoil && GoodProton  ","colz");
  TH1* h = (TH1*) gDirectory->FindObjectAny("h");
  h->GetXaxis()->SetTitle("E_{29Mg} (MeV)");
  h->GetYaxis()->SetTitle("counts/40 keV");

  int maximum = h->GetMaximum();
  TLine* state1= new TLine(0,0,0,maximum);
  state1->Draw("l");

  TLine* state2= new TLine(0.054,0,0.054,maximum);
  state2->Draw("l");

  TLine* state3= new TLine(1.095,0,1.095,maximum);
  state3->Draw("l");

  TLine* state4= new TLine(1.431,0,1.431,maximum);
  state4->Draw("l");

  TLine* state5= new TLine(2.266,0,2.266,maximum);
  state5->Draw("l");

  TLine* state6= new TLine(2.5,0,2.5,maximum);
  state6->Draw("l");

  TLine* state7= new TLine(3.2,0,3.2,maximum);
  state7->Draw("l");

  TLine* state8= new TLine(4.43,0,4.43,maximum);
  state8->Draw("l");

  c->cd(3);
  tree->Draw("ELab:ThetaLab>>hK(180,90,180,900,0,14)"," GoodProton","colz");
  NPL::Reaction r("28Mg(d,p)29Mg@222");
  r.GetKinematicLine3()->Draw("c");

  NPL::Reaction r2("28Mg(d,d)28Mg@222");
  r2.GetKinematicLine3()->Draw("c");

  NPL::Reaction r3("28Mg(p,p)28Mg@222");
  r3.GetKinematicLine3()->Draw("c");


  TH2* hk = (TH2*) gDirectory->FindObjectAny("hK");
  hk->GetXaxis()->SetTitle("#theta_{Lab} (deg)");
  hk->GetYaxis()->SetTitle("E_{Lab} (MeV)");

  c->cd(4);
  tree->Draw("AddBack_DC/1000>>hDC(300,0,3)","ELab>0&& ThetaLab>90 && GoodProton && Ex > 0.5 && Ex<2","");
  gPad->SetLogz();
  h = (TH1*) gDirectory->FindObjectAny("hDC");
  h->GetXaxis()->SetTitle("E_{#gamma} (MeV)");
  h->GetYaxis()->SetTitle("counts / 10 keV");

  c->cd(5);
  tree->Draw("Ex>>h11(50,-1,8)","GoodProton && AddBack_DC/1000. > 1.0 && AddBack_DC/1000. < 1.06 ","");
  h = (TH1*) gDirectory->FindObjectAny("h11");
  h->GetXaxis()->SetTitle("E_{29Mg} (MeV)");
  tree->Draw("Ex>>h12(50,-1,8)","GoodProton && AddBack_DC/1000. > 0.3 && AddBack_DC/1000. < 0.36 ","same");
  h = (TH1*) gDirectory->FindObjectAny("h12");
  h->SetFillColor(kBlack);
  h->SetLineColor(kBlack);
  h->SetFillStyle(3004);

  c->cd(6);
  tree->Draw("AddBack_DC/1000>>hDC2(300,0,3)","ELab>0&& ThetaLab>90 && GoodProton && Ex > 2 && Ex<3","");
  gPad->SetLogz();
  h = (TH1*) gDirectory->FindObjectAny("hDC2");
  h->GetXaxis()->SetTitle("E_{#gamma} (MeV)");
  h->GetYaxis()->SetTitle("counts / 10 keV");


  c->cd(7);
  tree->Draw("Ex>>h13(50,-1+0.1,8+0.1)","GoodProton && AddBack_DC/1000. > 2.2 && AddBack_DC/1000. < 2.28 ","");
  h = (TH1*) gDirectory->FindObjectAny("h11");
  h->GetXaxis()->SetTitle("E_{29Mg} (MeV)");

  tree->Draw("Ex>>h14(50,-1+0.1,8+0.1)","GoodProton && AddBack_DC/1000. > 2.35 && AddBack_DC/1000. < 2.65 ","same");
  h = (TH1*) gDirectory->FindObjectAny("h14");
  h->SetFillColor(kBlack);
  h->SetLineColor(kBlack);
  h->SetFillStyle(3004);
}

