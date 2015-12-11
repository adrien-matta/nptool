
void PixelMaker(){

  // Get the Tree
  TFile* infile = new TFile("../../../../Outputs/Analysis/S1554_Alpha_Phy.root","READ");
  TTree* tree = (TTree*) infile->FindObjectAny("PhysicsTree");
  TSharcPhysics* Sharc = new TSharcPhysics();
  tree->SetBranchAddress("Sharc",&Sharc);
  
  // Create the histogram
  TFile* outfile = new TFile("PixelHistos.root","RECREATE");
  for(unsigned int d = 0 ; d < 12 ; d++){
    for(unsigned int f = 0 ; f < 24 ; f++){
      for(unsigned int b = 0 ; b < 48 ; b++){
        new TH1F(Form("h%d_%d_%d",d+1,f+1,b+1),Form("h%d_%d_%d",d+1,f+1,b+1),3000,0,300000);
      }
    }
  }
  cout << "initial write" << endl;
  outfile->Flush();


  unsigned long nentries = tree->GetEntries();
  unsigned int d = 0;
  unsigned int f = 0;
  unsigned int b = 0; 
  double E = 0;
  TH1* histo;
  for(unsigned long i = 0 ; i < nentries ; i++){
    if(i%1000000==0){
      cout << "\r Progress " << i << "/" << nentries << " (" << (double)i/nentries*100. << "%)          "  << flush;
    }
    tree->GetEntry(i);
    for(unsigned int i = 0 ; i < Sharc->EventMultiplicity;i++){
      d = Sharc->DetectorNumber[i]; 
      f = Sharc->Strip_Front[i]; 
      b = Sharc->Strip_Back[i]; 
      E = Sharc->Strip_Front_RawE[i];
      histo = (TH1*) outfile->FindObjectAny(Form("h%d_%d_%d",d,f,b));
       histo->Fill(E);
    }
  }

  cout << "Saving Histo" << endl;
  outfile->Write();
  outfile->Close();
  infile->Close();

}
