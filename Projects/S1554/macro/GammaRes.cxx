
void GammaRes(){
  TGraph* Res = new TGraph();
  TLorentzVector GammaLV; 
  TVector3 position;
  TVector3 beta(0,0,0.10) ;
  for(unsigned int i = 0 ; i < 1000 ; i++){

    double E = i*10;
    position =TVector3(0,110+45,0);
    position.SetMag(1);
    GammaLV.SetPx(E*position.X());
    GammaLV.SetPy(E*position.Y());
    GammaLV.SetPz(E*position.Z());
    GammaLV.SetE(E);
    GammaLV.Boost(-beta);
    double E1 = GammaLV.Energy();
    position =TVector3(0,110+45,33.4*2);
    position.SetMag(1);
    GammaLV.SetPx(E*position.X());
    GammaLV.SetPy(E*position.Y());
    GammaLV.SetPz(E*position.Z());
    GammaLV.SetE(E);
    GammaLV.Boost(-beta);
    double E2 = GammaLV.Energy();
    
    Res->SetPoint(Res->GetN(),E,abs(E1-E2)*0.5);
  }

  Res->Draw("al");
  cout << Res->Eval(1000)<<endl ;
  cout << Res->Eval(5000)<<endl ;

}
