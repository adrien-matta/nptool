void MgCheck(){
 
  TFile* file = new TFile("../../Outputs/Analysis/S1554_28Mg_Phy.root");
  TTree* tree = (TTree*) file->FindObjectAny("PhysicsTree");

  tree->SetAlias("GoodTrifoil","((Trifoil.Time>150 && Trifoil.Time<160 && RunNumber>35110)||(Trifoil.Time>50 && Trifoil.Time<60 && RunNumber<35110)) &&ELab > 0 && Sharc.Strip_E>0.4") ;
  TCanvas* c = new TCanvas("Mg","Mg",1500,1000);
  tree->Draw("Ex>>h(200,-10,10)","GoodTrifoil &&  ThetaLab>90 ","");
  TH1* h = (TH1*) gDirectory->FindObjectAny("h");
  h->GetXaxis()->SetTitle("E_{29Mg} (MeV)");
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
}
