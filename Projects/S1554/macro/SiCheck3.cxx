void SiCheck3(){
  
  TFile* file = new TFile("../../../Outputs/Analysis/S1554_28Si_Phy.root");
  TTree* tree = (TTree*) file->FindObjectAny("PhysicsTree");

  TCanvas* c = new TCanvas("Result","Result",900,900);
  c->Divide(4,4);

  string hist,cond;
  for(unsigned int i = 0 ; i < 16 ; i ++){
    c->cd(i+1);
    hist = Form("Ex:Gamma_Energy/1000.>>h%i(2000,0,10,300,-5,15)",i+1);
    cond = Form("ELab>0 && ThetaLab>90 && Clover_Number==%i",i+1); 
    tree->Draw(hist.c_str(),cond.c_str(),"colz");
    gPad->SetLogz(); 
  }


}
