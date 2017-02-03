void SiCheck(){

  TFile* file = new TFile("../../Outputs/Analysis/S1554_28Si_Phy.root");
  TTree* tree = (TTree*) file->FindObjectAny("PhysicsTree");

  int current=16;
  TCanvas* c;
  for(unsigned int i = 35090 ; i < 35300 ; i++){

    if(current == 16){
      c = new TCanvas(Form("Check%i",i),Form("Check%i",i),900,900);
      c->Divide(4,4);
      current = 1;
    }
    c->cd(current++);
    tree->Draw(Form("ELab:ThetaLab>>hK%i(180,0,180,150,0,10)",i),Form("ELab>0&&RunNumber == %i",i),"colz");
    gPad->SetLogz();
    c->Update(); 
  }
}
