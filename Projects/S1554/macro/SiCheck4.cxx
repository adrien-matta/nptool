void SiCheck4(){
  
  TFile* file = new TFile("../../Outputs/Analysis/S1554_28Si_Phy.root");
  TTree* tree = (TTree*) file->FindObjectAny("PhysicsTree");

  TCanvas* c = new TCanvas("Result","Result",900,900);
  c->Divide(4,4);

  for(unsigned int i = 0 ; i < 16 ; i++){
  c->cd(i+1);
  if(i+1>4)
    tree->Draw(Form("Gamma_Energy/1000.>>h%i(10000,0,10)",i),Form("ELab>0 && ThetaLab>90 && Clover_Number==%i",i+1),"colz");
  c->Update();
  }

}
