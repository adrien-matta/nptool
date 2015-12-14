void AddGraphPoint(TGraph* g, double x , double y){
  g->Set(g->GetN()+1);
  g->SetPoint(g->GetN()-1,x,y);
}
///////////////////////
void EnergyCalibrator(){
  TFile* file = new TFile("~/Desktop/nptool/Outputs/Analysis/S1554_Alpha_Histos.root");
  NPL::CalibrationSource* cs1 = new NPL::CalibrationSource();
  NPL::EnergyLoss* el = new NPL::EnergyLoss("alpha_Al.G4table","G4Table",10);

  ofstream calfile("SharcCalibration.txt");
  ofstream deadfile("DeadLayer.txt");
  ofstream configfile("ConfigSharc.dat");
  configfile << "ConfigSharc" << endl;

  if(!deadfile.is_open() || !calfile.is_open())
    exit(1);

  vector<double> coeff;
  
  vector<TGraph*> gcoeff_F, goff_F , gcoeff_B, goff_B,dist_F,dist_B;
  
  for(unsigned int det = 0 ; det < 12 ; det++){
    gcoeff_F.push_back(new TGraph());
    goff_F.push_back(new TGraph());
    gcoeff_B.push_back(new TGraph());
    goff_B.push_back(new TGraph());
    dist_F.push_back(new TGraph());
    dist_B.push_back(new TGraph());
  }
  TCanvas* c1 = new TCanvas("Coeff Front","Coeff Front");
  c1->Divide(4,4);
  
  TCanvas* c2 = new TCanvas("Off Front","Off Front");
  c2->Divide(4,4);
 
  TCanvas* c3 = new TCanvas("Coeff Back","Coeff Back");
  c3->Divide(4,4);
  
  TCanvas* c4 = new TCanvas("Off Back","Off Back");
  c4->Divide(4,4);

  TCanvas* c5 = new TCanvas("Dist Front","Dist Front");
  c5->Divide(4,4);

  TCanvas* c6 = new TCanvas("Dist Back","Dist Back");
  c6->Divide(4,4);


  
  TCanvas* current = new TCanvas(); 
  for(unsigned int det = 0 ; det < 12 ; det++){
    configfile << "%" << Form(" Detector %i",det+1) << endl;
    cout << "Detector " << det+1 << endl;
    TH2* h2d = (TH2*) file->FindObjectAny(Form("SHARC%i_STR_FRONT_EMAX_RAW",det+1));
    if(h2d){
      cout << "Front" << endl;
      for(unsigned int i = 1 ; i < h2d->GetNbinsX()+1 ; i++){
        TH1D* h = h2d->ProjectionY("px",i,i);
        if(h){
          NPL::CalibrationSource* cs = new NPL::CalibrationSource();
          cs->Set_ThreeAlphaSource();

          NPL::SiliconCalibrator* mycal = new NPL::SiliconCalibrator();
          double dist = mycal->SimpleCalibration(h,cs,el,coeff,100000,500000);

          gPad->Update();
          if(dist>0){
            calfile << Form("SHARC_D%i_STRIP_FRONT%i_E ",det+1,i) << coeff[0] << " " << coeff[1] << endl; 
            AddGraphPoint(gcoeff_F[det],i,coeff[1]);
            AddGraphPoint(goff_F[det],i,coeff[0]);
            AddGraphPoint(dist_F[det],i,dist);
          }
          else if(det+1!=9 && det+1!=11){
            // Desable the strip
            if(!(det+1<5 && i>16))
              configfile << Form("  DISABLE_CHANNEL SH%iSTRF%i",det+1,i)<<endl;
          }

          delete h;
          delete mycal;
          delete cs;
        }
      }
    }
    c1->cd(det+1);
    gcoeff_F[det]->Draw("ap");
    c2->cd(det+1);
    goff_F[det]->Draw("ap");
    c5->cd(det+1);
    dist_F[det]->Draw("ap");
    current->cd();
    h2d = (TH2*) file->FindObjectAny(Form("SHARC%i_STR_BACK_EMAX_RAW",det+1));
    if(h2d){
      cout << "Back" << endl;
      for(unsigned int i = 1 ; i < h2d->GetNbinsX()+1 ; i++){
        TH1D* h = h2d->ProjectionY("px",i,i);
        if(h){
          NPL::CalibrationSource* cs = new NPL::CalibrationSource();
          cs->Set_ThreeAlphaSource();

          NPL::SiliconCalibrator* mycal = new NPL::SiliconCalibrator();
          double dist = mycal->SimpleCalibration(h,cs,el,coeff,100000,500000);
          gPad->Update();
          if(dist>0){
            calfile << Form("SHARC_D%i_STRIP_BACK%i_E ",det+1,i) << coeff[0] << " " << coeff[1] << endl; 
            if(coeff[1]==-1)
              cout << dist << endl;
            
            AddGraphPoint(gcoeff_B[det],i,coeff[1]);
            AddGraphPoint(goff_B[det],i,coeff[0]);
            AddGraphPoint(dist_B[det],i,dist);
          }
          else if(det+1!=9 && det+1!=11){
            // Desable the strip
            if(!(det+1<5 && i>24))
              configfile << Form("  DISABLE_CHANNEL SH%iSTRB%i",det+1,i)<<endl;
          }


          delete h;
          delete mycal;
          delete cs;
        }
      }
    }
    c3->cd(det+1);
    gcoeff_B[det]->Draw("ap");
    c4->cd(det+1);
    goff_B[det]->Draw("ap");
    c6->cd(det+1);
    dist_B[det]->Draw("ap");
    current->cd();

  }
  


}

