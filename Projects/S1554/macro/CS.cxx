TGraph* thdae;
TGraphErrors* gdd;
double minX = 1e30;
double minC = 1e30;
void NumericalMinimization(const char * minName ,const char *algoName);
double Chi2(const double* parameter);
////////////////////////////////////////////////////////////////////////////////
void CS(){
  TFile* file = new TFile("macro/Kine.root","READ");
  TH2F* h2 = (TH2F*)file->FindObjectAny("hEXT2");

  file = new TFile("macro/28MgddReso.root","READ");
  TH2F* hResodd = (TH2F*)file->FindObjectAny("Kine_2D");

  file = new TFile("macro/28MgppReso.root","READ");
  TH2F* hResopp = (TH2F*)file->FindObjectAny("Kine_2D");

  file = new TFile("SharcEfficiency_28Mgdd.root","READ");
  TH1D* eff = (TH1D*) file->FindObjectAny("SolidAngleCM");

  file = new TFile("Mg28dd_dae.root","READ");
  thdae = (TGraph*) file->FindObjectAny("Graph");

  file = new TFile("Mg28dd_pap.root","READ");
  TGraph* thpap = (TGraph*) file->FindObjectAny("Graph");
  thpap->SetLineColor(kGreen-3);

  file = new TFile("Mg28dd_loh.root","READ");
  TGraph* thloh = (TGraph*) file->FindObjectAny("Graph");
  thloh->SetLineColor(kMagenta-3);




  NPL::Reaction dd("28Mg(d,d)28Mg@222");
  NPL::Reaction pp("28Mg(p,p)28Mg@222");

  TGraph* kdd = dd.GetKinematicLine3();
  TGraph* kpp = pp.GetKinematicLine3();
  TGraph* tdd = dd.GetELabVersusThetaCM();
  TGraph* tpp = pp.GetELabVersusThetaCM();

  const double* xdd= kdd->GetX();
  const double* ydd =  kdd->GetY();
  kdd= new TGraph(kdd->GetN(),ydd,xdd);

  const double* xpp= kpp->GetX();
  const double* ypp =  kpp->GetY();
  kpp= new TGraph(kpp->GetN(),ypp,xpp);

  const double* xdd1= tdd->GetX();
  const double* ydd1 =  tdd->GetY();
  tdd= new TGraph(tdd->GetN()/2,xdd1,ydd1);

  const double* xpp2= tpp->GetX();
  const double* ypp2 =  tpp->GetY();
  tpp= new TGraph(tpp->GetN()/2,xpp2,ypp2);

  TH1D* hdd = new TH1D("hdd","hdd",300,0,180);
  TH1D* hpp = new TH1D("hpp","hpp",300,0,180);

  gdd = new TGraphErrors();
  TGraphErrors* gpp = new TGraphErrors();

  TF1* f = new TF1("f","gaus(0)+gaus(3)+pol1(6)",50,80);
  f->SetNpx(1000);
  f->SetParameter(0,20);

  f->SetParameter(3,20);

//  f->SetParameter(6,3);
//  f->SetParameter(7,-0.01);


    f->SetParameter(6,0);
f->SetParameter(7,0);

  f->SetParLimits(0,0,50);
  f->SetParLimits(3,0,50);


  TF1* g = new TF1("g","gaus(0)",0,90);

  unsigned int step = 200;
  double eps = 30./step;
  cout << "EPSILON = " << eps <<" MeV" <<  endl;
  double E=0;
  TH1D* p;
  TH1D* Rd;
  TH1D* Rp;
  double* r;
  for(unsigned int i = 0 ; i < step ; i++){
    E = i*eps;
    double Td = kdd->Eval(E);
    double Tp = kpp->Eval(E);
    double TCMd = tdd->Eval(E);
    double TCMp = tpp->Eval(E);


    double lTCMd = tdd->Eval(E-eps*0.5);
    double uTCMd = tdd->Eval(E+eps*0.5);

    p = h2->ProjectionX(Form("p%f",TCMd),h2->GetYaxis()->FindBin(E-eps*0.5),h2->GetYaxis()->FindBin(E+eps*0.5));
    if(p->GetEntries()>300){
      p->Rebin(4);
    f->SetParameter(6,0);
f->SetParameter(7,0);


      p->Draw();

      f->FixParameter(1,Td);
      f->FixParameter(4,Tp);

      // Fixing resolution according to Simulation
      Rd = hResodd->ProjectionX(Form("Rd%f",TCMd),hResodd->GetYaxis()->FindBin(E-eps*0.5),hResodd->GetYaxis()->FindBin(E+eps*0.5));
      double Resodd = Rd->GetStdDev(); 
      Rp = hResopp->ProjectionX(Form("Rp%f",TCMd),hResopp->GetYaxis()->FindBin(E-eps*0.5),hResopp->GetYaxis()->FindBin(E+eps*0.5));
      double Resopp = Rp->GetStdDev(); 

      f->FixParameter(2,Resodd);
     // f->SetParLimits(2,Resodd,Resodd); 
       
      
      f->FixParameter(5,Resopp);
     // f->SetParLimits(5,Resopp,Resopp); 
       
      
      double pi,po;
      f->GetParLimits(3,pi,po);
      p->Fit(f,"RBFQ");
      gPad->Update();

      r = f->GetParameters();
      g->SetParameter(0,r[0]);
      g->SetParameter(1,r[1]);
      g->SetParameter(2,r[2]);

      double dd = g->Integral(r[1]-100*r[2],r[1]+100*r[2]);
      double ddo = dd ;
      double dde = sqrt(dd);
      double mynorm = 1;
      double EFF = eff->Integral(eff->FindBin(uTCMd),eff->FindBin(lTCMd))/abs(uTCMd-lTCMd);
      dd = mynorm*dd / eff->GetBinContent(eff->FindBin(TCMd));
      dde = mynorm*dde / eff->GetBinContent(eff->FindBin(TCMd));
      double ddx = 0.5*abs(uTCMd-lTCMd);
      if(dd<1e50 && ddo>10  && abs(Td-Tp)>1){
        hdd->Fill(tdd->Eval(E),dd);
        gdd->SetPoint(gdd->GetN(),TCMd,dd);
        gdd->SetPointError(gdd->GetN()-1,ddx,dde);
      }

      g->SetParameter(0,r[3]);
      g->SetParameter(1,r[4]);
      g->SetParameter(2,r[5]);
      double pp = g->Integral(0,90);
      double ppe = sqrt(pp); 

      hpp->Fill(tpp->Eval(E),pp);
      gpp->SetPoint(gpp->GetN(),TCMp,pp);
      gpp->SetPointError(gpp->GetN()-1,0,ppe);
      p->GetXaxis()->SetRangeUser(40,90);
      gPad->Update();
      gPad->WaitPrimitive();
    }
  }

  new TCanvas();
  gdd->Draw("ap");
  gdd->GetXaxis()->SetTitle("#theta_{CM} (deg)");
  gdd->GetYaxis()->SetTitle("d#sigma/d#Omega (mb/sr)");
  gdd->GetYaxis()->SetRangeUser(1,1e5);
  thdae->Draw("c");
  thpap->Draw("c");
  thloh->Draw("c");

  gPad->SetLogy();
  NumericalMinimization("Minuit2","MIGRAD");
}

////////////////////////////////////////////////////////////////////////////////
double Chi2(const double* parameter){
  double Chi2 = 0 ;
  for (int i = 0; i < gdd->GetN() ; i++) {
    if(gdd->GetX()[i]>22 && gdd->GetX()[i]<32 && gdd->GetY()[i]>1){
      double chi = ( thdae->Eval( gdd->GetX()[i] ) - parameter[0]*gdd->GetY()[i]) / (parameter[0]*gdd->GetEY()[i] );
      Chi2 += chi*chi ;
    }
  }
  if(Chi2<minC){
    minC= Chi2;
    minX= parameter[0];
  }
  return Chi2;
}

////////////////////////////////////////////////////////////////////////////////
void NumericalMinimization(const char * minName ,const char *algoName){
  ROOT::Math::Minimizer* min = 
    ROOT::Math::Factory::CreateMinimizer(minName, algoName);

  // set tolerance , etc...
  min->SetMaxFunctionCalls(100000); // for Minuit/Minuit2 
//  min->SetPrecision(1e-6);
//  min->SetValidError(true);
//  min->SetPrintLevel(1);

  // create funciton wrapper for minimizer
  // a IMultiGenFunction type 
  ROOT::Math::Functor f(&Chi2,1); 

  min->SetFunction(f);
  double* parameter = new double[1];
  parameter[0] = 2e-22;
  // Set the free variables to be minimized!
  min->SetLimitedVariable(0,"Normalistation",parameter[0],1e-5,1e-5,1);

  // do the minimization
  min->Minimize(); 

  const double *xs = min->X();
  std::cout << "Minimum = " << xs[0] <<  " +/- " << min->Errors()[0] << std::endl;

  for(int i = 0; i <  gdd->GetN() ; i++ ){
    gdd->SetPoint(i,gdd->GetX()[i],xs[0]*gdd->GetY()[i]);
    gdd->SetPointError(i,gdd->GetEX()[i],xs[0]*gdd->GetEY()[i]);

  }
  return 0;
}
