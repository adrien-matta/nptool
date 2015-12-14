
void GeMaker(){

  // Get the Tree
  TChain* tree = new TChain("DataS1554");
  tree->Add("Co_root/*.root");
  TTigressData* Tigress = new TTigressData();
  tree->SetBranchStatus("*",false);
  tree->SetBranchStatus("Tigress",true);
  tree->SetBranchAddress("Tigress",&Tigress);
  
  // Create the histogram
  TFile* outfile = new TFile("GeHistos_60Co.root","RECREATE");
  for(unsigned int d = 4 ; d < 16 ; d++){
    for(unsigned int c = 0 ; c < 4 ; c++){
      for(unsigned int s = 0 ; s < 10 ; s++){
        new TH1F(Form("h%d_%d_%d",d+1,c+1,s),Form("h%d_%d_%d",d+1,c+1,s),80000,0,8000000);
      }
    }
  }
  cout << "initial write" << endl;
  outfile->Flush();

  unsigned long nentries = tree->GetEntries();
  unsigned int d = 0;
  unsigned int c = 0;
  unsigned int s = 0; 
  double E = 0;
  TH1* histo;
  for(unsigned long i = 0 ; i < nentries ; i++){
    if(i%1000000==0){
      cout << "\r Progress " << i << "/" << nentries << " (" << (double)i/nentries*100. << "%)          "  << flush;
    }
    tree->GetEntry(i);
    unsigned int mysize = Tigress->GetMultiplicityGe();
    for(unsigned int i = 0 ; i < mysize;i++){
      d = Tigress->GetGeCloverNbr(i); 
      c = Tigress->GetGeCrystalNbr(i); 
      s = Tigress->GetGeSegmentNbr(i); 
      E = Tigress->GetGeEnergy(i);
      histo = (TH1*) outfile->FindObjectAny(Form("h%d_%d_%d",d,c,s));
      histo->Fill(E);
    }
  }

  cout << "Saving Histo" << endl;
  outfile->Write();
  outfile->Close();
}
