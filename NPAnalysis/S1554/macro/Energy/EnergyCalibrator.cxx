void EnergyCalibrator(){
  TFile* file = new TFile("~/Desktop/nptool/Outputs/Analysis/AlphaHistoS1554.root");
  NPL::CalibrationSource* cs1 = new NPL::CalibrationSource();
  NPL::EnergyLoss* el = new NPL::EnergyLoss("alpha_Al.G4table","G4Table",10);

  ofstream calfile("SharcCalibration.txt");
  ofstream deadfile("DeadLayer.txt");

  if(!deadfile.is_open() || !calfile.is_open())
    exit(1);

  vector<double> coeff;

  for(unsigned int det = 0 ; det < 12 ; det++){
    cout << "Detector " << det+1 << endl;
    TH2* h2d = (TH2*) file->FindObjectAny(Form("SHARC%i_STR_FRONT_EMAX_RAW",det+1));
    if(h2d){
      cout << "Front" << endl;
      for(unsigned int i = 1 ; i < h2d->GetNbinsX() ; i++){
        TH1D* h = h2d->ProjectionY("px",i,i);
        if(h){
          NPL::CalibrationSource* cs = new NPL::CalibrationSource();
          cs->Set_ThreeAlphaSource();

          NPL::SiliconCalibrator* mycal = new NPL::SiliconCalibrator();
          double dist = mycal->SimpleCalibration(h,cs,el,coeff,0,500000);

          gPad->Update();
          if(dist>0){
            calfile << Form("SHARC_D%i_STRIP_FRONT%i_E ",det+1,i) << coeff[0] << " " << coeff[1] << endl; 
          }

          delete h;
          delete mycal;
          delete cs;
        }
      }
    } 
    h2d = (TH2*) file->FindObjectAny(Form("SHARC%i_STR_BACK_EMAX_RAW",det+1));
    if(h2d){
      cout << "Back" << endl;
      for(unsigned int i = 1 ; i < h2d->GetNbinsX() ; i++){
        TH1D* h = h2d->ProjectionY("px",i,i);
        if(h){
          NPL::CalibrationSource* cs = new NPL::CalibrationSource();
          cs->Set_ThreeAlphaSource();

          NPL::SiliconCalibrator* mycal = new NPL::SiliconCalibrator();
          double dist = mycal->SimpleCalibration(h,cs,el,coeff,0,500000);
          
          gPad->Update();
          if(dist>0){
            calfile << Form("SHARC_D%i_STRIP_BACK%i_E ",det+1,i) << coeff[0] << " " << coeff[1] << endl; 
          }

          delete h;
          delete mycal;
          delete cs;
        }
      }
    } 
  }
}

