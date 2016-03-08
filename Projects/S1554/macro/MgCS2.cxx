void Scale(TGraph* g , TGraphErrors* ex);
TGraph* TWOFNR(double E, double J0, double J, double n, double l, double j);
TH1* Region(double Emin, double Emax,TH2* h, TCanvas* c, int pad, TH2* Eff);
double ToMininize(const double* parameter);
TGraph* FindNormalisation(TGraph* cs1, TGraph* cs2, TH1* hexp);

TGraph* g1;
TGraph* g2;
TH1* current;

////////////////////////////////////////////////////////////////////////////////
void MgCS2(){
  file = new TFile("SharcEfficiency_29Mg.root");
  TH2* Eff = (TH2*) file->FindObjectAny("SolidAngleCM_2D");

  Goodfile = new TFile("GoodExThetaCM.root");
  TH2D* goodEx = (TH2D*) Goodfile->FindObjectAny("hexcmG");

  Badfile = new TFile("BadExThetaCM.root");
  TH2D* badEx = (TH2D*) Badfile->FindObjectAny("hexcmB");

  TH2D* h = goodEx;
  badEx->Scale(7./(145-1+300-160));
  h->Sumw2();
  h->SetMarkerSize(0.5);
  h->SetMarkerColor(kAzure+7);
  badEx->Sumw2(); 
  h->GetYaxis()->SetTitle("E_{29Mg} (MeV)");
  h->GetXaxis()->SetTitle("#theta_{CM} (deg)");
  h->Add(badEx,-1);
  Eff->Sumw2();
  h->Scale(0.010048*10/7.37);
  h->Divide(Eff);

  TH1D* p;
  int nn=0;
  // Full Ex
  p =  h->ProjectionY(Form("p%i",nn++),h->GetXaxis()->FindBin(0.),h->GetXaxis()->FindBin(50.));
  p->Draw();
  
  new TCanvas();
  h->Draw("colz");
  // CS
  TCanvas* c = new TCanvas("CS","CS",1000,1000);
  c->Divide(2,2);

  // Region 
  current = Region(-0.1,0.4,h,c,nn++,Eff);
  g1 = TWOFNR(0.000, 0, 0.5, 1, 0, 0.5);
  g2 = TWOFNR(0.054, 0, 1.5, 0, 2, 1.5);
  g1->SetLineStyle(2);g1->Draw("c");
  g2->SetLineStyle(3);g2->Draw("c");
  FindNormalisation(g1,g2,current)->Draw("c"); 
  
  current = Region(0.6,1.8,h,c,nn++,Eff);
  g1 = TWOFNR(1.095, 0, 1.5, 1, 1, 1.5);
  g2 = TWOFNR(1.431, 0, 3.5, 0, 3, 3.5);
  g1->SetLineStyle(2);g1->Draw("c");
  g2->SetLineStyle(3);g2->Draw("c");
  FindNormalisation(g1,g2,current)->Draw("c"); 
  
  current = Region(2,3,h,c,nn++,Eff);
  g1 = TWOFNR(2.266, 0, 0.5, 1, 1, 0.5);
  g2 = TWOFNR(2.500, 0, 1.5, 0, 2, 1.5);
  g1->SetLineStyle(2);g1->Draw("c");
  g2->SetLineStyle(3);g2->Draw("c");
  FindNormalisation(g1,g2,current)->Draw("c"); 
 
  current = Region(3,4.8,h,c,nn++,Eff);
  g1 = TWOFNR(3.2, 0, 2.5, 0, 2, 2.5);
  g2 = TWOFNR(3.2, 0, 3.5, 0, 3, 3.5);
  g1->SetLineStyle(2);g1->Draw("c");
  g2->SetLineStyle(3);g2->Draw("c");
  FindNormalisation(g1,g2,current)->Draw("c"); 


}
////////////////////////////////////////////////////////////////////////////////
TH1* Region(double Emin, double Emax,TH2* h, TCanvas* c, int pad , TH2* Eff){
  c->cd(pad);
  TH1D* p =  h->ProjectionX(Form("p%i",pad),h->GetYaxis()->FindBin(Emin),h->GetYaxis()->FindBin(Emax));
  p->Draw();
  gPad->SetLogy();
//  p->GetYaxis()->SetRangeUser(1e1,1e5);
  return p;
}

////////////////////////////////////////////////////////////////////////////////
double Chi2(TGraph* g , TH1* h){
 double Chi2 = 0 ;
 double chi;

  for(int i = 1 ; i < h->GetNbinsX() ; i++){
    if(h->GetBinContent(i)>0)  {
      chi = (h->GetBinContent(i) - g->Eval(h->GetBinCenter(i)) ) / (h->GetBinError(i));
      Chi2 +=chi*chi;
    }
  }

 return Chi2;
}
////////////////////////////////////////////////////////////////////////////////
double ToMininize(const double* parameter){
static int f = 0 ;
  TGraph* g = new TGraph();
  double* X = g1->GetX();
  double* Y = g1->GetY();
  for(int i = 0 ; i < g1->GetN() ; i++){
    g->SetPoint(g->GetN(),X[i],parameter[0]*Y[i] + parameter[1]*g2->Eval(X[i]) );
  }
  double chi2  = Chi2(g,current);  
  return chi2;
}
////////////////////////////////////////////////////////////////////////////////
void Scale(TGraph* g , TGraphErrors* ex){
  double scale;
  double mean = 0 ;
  double* eX = ex->GetX();
  double* eY = ex->GetY();
  double totalW = 0;
  double W = 0;
  for(int i = 0 ; i < ex->GetN() ; i++){
    if(eY[i]>1 && eY[i] <1e4){
      // Incremental Error weighted average
      W = 1./ex->GetErrorY(i);
      scale = eY[i]/g->Eval(eX[i]);
      totalW +=W;
      mean = mean + W*(scale - mean)/(totalW);
    }
  }

  double* x = g->GetX();
  double* y = g->GetY();
  for(unsigned int i = 0 ; i < g->GetN() ; i++)
    g->SetPoint(i,x[i],y[i]*mean);
}
////////////////////////////////////////////////////////////////////////////////
TGraph* TWOFNR(double E, double J0, double J, double n, double l, double j){
  double BeamEnergy =  8;

  NPL::Reaction r("28Mg(d,p)29Mg@224");
  r.SetExcitationHeavy(E);
  double QValue = r.GetQValue();

  std::ofstream Front_Input("in.front");
  Front_Input << "jjj" << std::endl;
  Front_Input << "pipo" << std::endl;
  Front_Input << 2 << std::endl;
  Front_Input << BeamEnergy << std::endl;
  Front_Input << 28 << " " << 12 << std::endl;
  Front_Input << 1 << std::endl;
  Front_Input << 1 << std::endl;
  Front_Input << "0 0 0" << std::endl;
  Front_Input << l << " " << j << std::endl;
  Front_Input << n << std::endl;
  Front_Input << 2 << std::endl;

  // unbound case:
//  if( QValue < 0 )
//    Front_Input << 0.01 << std::endl;
//  else
    Front_Input << QValue << std::endl; 

  Front_Input << 1 << std::endl;
  Front_Input << J0 << std::endl;
  Front_Input << 1 << std::endl;
  Front_Input << 5 << std::endl;
  Front_Input << 1 << std::endl;
  Front_Input << J << std::endl;
  Front_Input << 1 << std::endl;
  Front_Input << 2 << std::endl;
  Front_Input << 2 << std::endl;
  Front_Input << 1 << std::endl;
  Front_Input << 1 << std::endl;
  Front_Input << 1.25 << " " << 0.65 << std::endl;
  Front_Input << 6.0 << std::endl;
  Front_Input << 0 << std::endl;
  Front_Input << 0 << std::endl;

  Front_Input.close() ;

  system("(exec FRONT < in.front &> /dev/null)"); 
  system("(exec echo tran.jjj | TWOFNR &> /dev/null)");
 // system("exec FRONT < in.front"); 
 // system("(exec echo tran.jjj | TWOFNR)");


  TGraph* CS = new TGraph("22.jjj");
  return CS;
}

////////////////////////////////////////////////////////////////////////////////
TGraph* FindNormalisation(TGraph* cs1, TGraph* cs2, TH1* hexp){
  // Set global variable
  g1 = cs1;
  g2 = cs2;
  current = hexp;

  const char* minName ="Minuit";const char* algoName="Fumili2";
  ROOT::Math::Minimizer* min =
    ROOT::Math::Factory::CreateMinimizer(minName, algoName);
  min->SetValidError(true);

  // Number of parameter
  mysize = 2;

  // create funciton wrapper for minimizer
  // a IMultiGenFunction type
  ROOT::Math::Functor f(&ToMininize,mysize);
  min->SetFunction(f);
  
  double* parameter = new double[mysize];
  for(unsigned int i = 0 ; i < mysize ; i++){
    parameter[i] = 0.5;
    char name[4];
    sprintf(name,"S%d",i+1);
    min->SetLimitedVariable(i,name,parameter[i],0.1,0,1000);
  }
  
  // do the minimization
  min->Minimize();
  const double *xs = min->X();
  const double *err = min->Errors(); 

  for(int i = 0  ; i < mysize ; i++){
    cout << Form("S%d=",i+1) << xs[i] <<"("<<err[i] << ") "; 
  }
  cout << endl;
  // Return the Fitted CS
  TGraph* g = new TGraph(); 
  double* X = cs1->GetX();
  double* Y = cs1->GetY();
  for(int i = 0 ; i < cs1->GetN() ; i++){
    g->SetPoint(g->GetN(),X[i],xs[0]*Y[i] + xs[1]*cs2->Eval(X[i]) );
  }
  return g;
}

