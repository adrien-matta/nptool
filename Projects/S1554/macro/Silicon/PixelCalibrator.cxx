void PixelCalibrator(){
  TFile* file = new TFile("PixelHistos.root");
  NPL::CalibrationSource* cs1 = new NPL::CalibrationSource();
  NPL::EnergyLoss* el = new NPL::EnergyLoss("alpha_Al.G4table","G4Table",10);
  TH1* h ;

  NPL::CalibrationSource* cs = new NPL::CalibrationSource();
  cs->Set_ThreeAlphaSource();
  NPL::SiliconCalibrator* mycal = new NPL::SiliconCalibrator();


  ofstream calfile("SharcPixelCalibration.txt");
  ofstream deadfile("DeadLayer.txt");
  if(!deadfile.is_open() || !calfile.is_open())
    exit(1);

  vector<double> coeff;
  for(unsigned int d = 0 ; d < 12 ; d++){
    TH2F* DeadLayer = new TH2F(Form("d%d",d+1), Form("d%d",d+1), 24,1,24,48,1,48);
    for(unsigned int f = 0 ; f < 24 ; f++){
      for(unsigned int b = 0 ; b < 48 ; b++){
        h = (TH1*) file->FindObjectAny(Form("h%d_%d_%d",d+1,f+1,b+1));
        h->Rebin(3);
        cout << "\r " << d+1 << " " << f+1 << " " << b+1 << "    " << flush ;
        double deadlayer = mycal->ZeroExtrapolation(h,cs,el,coeff,0,5000,150000,300000);
        if(deadlayer>-1){
          gPad->Update();
          deadfile << Form("SHARC_D%i_STRIP_FRONT%i_BACK%i_DEADLAYER ",d+1,f+1,b+1) << deadlayer <<endl;
          calfile << Form("SHARC_D%i_STRIP_FRONT%i_BACK%i_E ",d+1,f+1,b+1) << coeff[0] << " " << coeff[1] << endl; 
          DeadLayer->Fill(f,b,deadlayer+0.0001);
        }
        else if(deadlayer == -300)
          cout << d+1 << " " << f+1 << " " << b+1 << " fail peak find" << endl;
      
      }    
    }
  }

  TCanvas* c = new TCanvas();

  c->Divide(3,4);
  
  for(unsigned int d = 0 ; d < 12 ; d++){
    c->cd(d+1);
     TH2F* DeadLayer = (TH2F*) gDirectory->FindObjectAny(Form("d%d",d+1));
     DeadLayer->Draw("colz");
  }
}
