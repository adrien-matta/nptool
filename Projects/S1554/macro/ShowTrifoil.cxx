
void ShowTrifoil(){

  TFile* file = new TFile("~/Desktop/S1554/Root/good/data35216_001.root");
  TTree* tree = (TTree*) file->FindObjectAny("DataS1554");
  TTrifoilData* tr = new TTrifoilData;
  tree->SetBranchAddress("Trifoil",&tr);

  string user;
  int i  = 0 ;

  while(cin >> user ){
    if(user == "q")
      break;

    tree->GetEntry(i++);
    if(tr->GetMultiplicity()>0){
      tr->GetWaveform(0)->Draw();
      gPad->Update();  
    }
  }

}
