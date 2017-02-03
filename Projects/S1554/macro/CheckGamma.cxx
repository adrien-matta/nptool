

void CheckGamma(){
  TFile* file = new TFile("../../Outputs/Analysis/S1554_28Si_Phy.root");
  TTree* tree = (TTree*) file->FindObjectAny("PhysicsTree");
  
  tree->Draw("AddBack_DC/1000>>h6(250,0,8)","AddBack_Theta < 125 &&  AddBack_Theta > 120 ",""); 
  tree->Draw("AddBack_DC/1000>>h1(250,0,8)","AddBack_Theta < 80 &&  AddBack_Theta > 70  ","same"); 
  tree->Draw("AddBack_DC/1000>>h2(250,0,8)","AddBack_Theta < 85 &&  AddBack_Theta > 80 ","same"); 
  tree->Draw("AddBack_DC/1000>>h3(250,0,8)","AddBack_Theta < 100 &&  AddBack_Theta > 90 ","same"); 
  tree->Draw("AddBack_DC/1000>>h4(250,0,8)","AddBack_Theta < 105.5 &&  AddBack_Theta > 104 ","same"); 
  tree->Draw("AddBack_DC/1000>>h5(250,0,8)","AddBack_Theta < 107 &&  AddBack_Theta > 106 ","same"); 
  tree->Draw("AddBack_DC/1000>>h7(250,0,8)","AddBack_Theta < 150 &&  AddBack_Theta > 140 ","same"); 

  TH1* h ;
  for(unsigned int i = 0 ; i < 7 ; i++){
    h = (TH1*) gDirectory->FindObjectAny(Form("h%d",i+1));
    h->SetFillStyle(0);
    h->SetLineColor(kAzure+i+1);
  }



