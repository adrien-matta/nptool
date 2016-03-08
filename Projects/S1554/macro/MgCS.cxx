void Scale(TGraph* g , TGraphErrors* ex);
TGraph* TWOFNR(double E, double J0, double J, double n, double l, double j);

////////////////////////////////////////////////////////////////////////////////
void MgCS(){


  file = new TFile("SharcEfficiency_29Mg.root");
  TH2* Eff = (TH2*) file->FindObjectAny("SolidAngleCM_2D");

  //TCanvas* c0 = new TCanvas("Mg0","Mg0"); 

  Goodfile = new TFile("GoodExThetaCM.root");
  TH2D* goodEx = (TH2D*) Goodfile->FindObjectAny("hexcmG");

  Badfile = new TFile("BadExThetaCM.root");
  TH2D* badEx = (TH2D*) Badfile->FindObjectAny("hexcmB");

  TH2D* h = goodEx;
  badEx->Scale(7./(145-1+300-160));
  //  h->Sumw2();
  h->SetMarkerSize(0.5);
  h->SetMarkerColor(kAzure+7);
  //  badEx->Sumw2(); 
//  h->Draw("colz");
  h->GetYaxis()->SetTitle("E_{29Mg} (MeV)");
  h->GetXaxis()->SetTitle("#theta_{CM} (deg)");
  h->GetXaxis()->SetRangeUser(-1,50);

  vector<double> E,W;
  E.push_back(0.0);
  E.push_back(0.054);
//  E.push_back(0.590);

  E.push_back(1.094);
  E.push_back(1.431);
//  E.push_back(1.638);

  E.push_back(2.266);
  E.push_back(2.500);
//  E.push_back(2.8);
  E.push_back(3.223);
  E.push_back(3.674);
  E.push_back(3.985);
  E.push_back(4.280);
  E.push_back(4.43);


  string formula="[0]+[1]*x+";

  for (unsigned int i = 0 ; i < E.size() ; i++){
    //formula+=Form("gaus(%i)",i*3);
    formula+=Form("[%i]*exp( -(x-[5]-[%i])*(x-[5]-[%i])/(([2]+[3]*x+[4]*x*x)*([2]+[3]*x+[4]*x*x)))",i*2+6,i*2+6+1,i*2+6+1);
    if(i!=E.size()-1)
      formula+="+";
    W.push_back(1.1*(0.144206+0.00644644*E[i]+0.0006305*E[i]*E[i]));
  }

  TGraph* g_shift = new TGraph();
  vector<TGraph*> g_width;
  for (unsigned int i = 0 ; i < E.size() ; i++){
    g_width.push_back(new TGraph);
  }
  TF1* bck = new TF1("bck","[0]+[1]*x",-6,-1);     
  bck->SetLineColor(kBlack);
  TF1* func = new TF1("func",formula.c_str(),-2,5);
  func->FixParameter(0,0);
  func->FixParameter(1,0);

  func->FixParameter(2,0.226217);
  func->FixParameter(3,0.00294652);
  func->FixParameter(4,0);
  func->FixParameter(5,-0.055);

  //  func->SetParLimits(2,0.1,0.3);
  //  func->SetParLimits(3,0,1);
  //  func->SetParLimits(4,0,1e-31);
  //  func->SetParLimits(5,-0.06,0.06);

  func->SetNpx(500);

  double Energy;
  for (unsigned int i = 0 ; i < E.size() ; i++){
    func->SetParameter(i*2+6,10);
    func->SetParLimits(i*2+6,0,100);
    func->FixParameter(i*2+6+1,E[i]);
  }

  //////////
  vector<TGraphErrors*> CS;
  CS.resize(E.size(),NULL);

  for(unsigned int i = 0 ; i < CS.size() ; i++){
    CS[i]=new TGraphErrors();
  }

  const double* par;
  unsigned int nbin = 10;
  double minCM = 2;
  double maxCM = 42;
  double step = (maxCM-minCM)/nbin;
  Eff->RebinX(step);  


  TH1D* p; TH1D* bbb; 
  double ThetaCM;
  int pad = 1;

  TCanvas* c4 = new TCanvas("Fit","Fit",1800,1000);
  c4->Divide(4,2,1e-4,1e-4);
  c4->cd(1);
  for(unsigned int i = 0 ; i < nbin ; i++){
    ThetaCM = minCM+i*step; 
    p = h->ProjectionY(Form("p%i",i),h->GetXaxis()->FindBin(ThetaCM),h->GetXaxis()->FindBin(ThetaCM+step));
    p->Rebin(2);
    // Background 
    bbb = badEx->ProjectionY(Form("b%i",i),badEx->GetXaxis()->FindBin(ThetaCM),badEx->GetXaxis()->FindBin(ThetaCM+step));
    bbb->Rebin(2);
    // Substract the Background
    p->Add(bbb,-1);
    // Presentation  
    p->SetMarkerSize(0.5);
    p->SetMarkerColor(kAzure+7);
    bbb->SetMarkerSize(0.1);
    bbb->SetLineColor(kOrange+7);
    bbb->SetFillColor(kOrange+7);
    bbb->SetFillStyle(3004);

    p->GetYaxis()->SetRangeUser(0,30);
    p->GetXaxis()->SetRangeUser(-1,6);

    // Limit the heigh of the gaussian to the bin at the state position
    for(unsigned int n = 0 ; n < CS.size() ; n++){ 
      func->SetParameter(n*2+6,p->GetBinContent(p->FindBin(func->GetParameter(n*2+6+1))));
      double integral = p->Integral(p->FindBin(E[n]-2*W[n]),p->FindBin(E[n]+2*W[n]));
      if(integral >0)
        func->SetParLimits(n*2+6,0, integral+1e-20);
      else
        func->SetParLimits(n*2+6,0, 10);  
    }
    c4->cd(pad++);
    p->Draw();
    //    p->Fit(bck,"RBFQ");
    bbb->Draw("same");

    //   func->FixParameter(0,bck->GetParameter(0));
    //   func->FixParameter(1,bck->GetParameter(1));
    p->Fit(func,"RBFQ+"); 
    par = func->GetParameters();
    TLatex* tex = new TLatex(2,25,Form("%.1f deg",ThetaCM+0.5*step));
    tex->Draw();
    // bck->Draw("same");

    g_shift->SetPoint(g_shift->GetN(),ThetaCM,func->GetParameter(5)); 
    if(p->Integral(p->FindBin(-1),p->FindBin(6))>-1000 ){
      for(unsigned int n = 0 ; n < CS.size() ; n++){
        double SolidAngle = Eff->ProjectionX("px",Eff->GetYaxis()->FindBin(E[n]-W[n]),Eff->GetYaxis()->FindBin(E[n]+W[n]))->Interpolate(ThetaCM);
        double Width = func->GetParameter(2)+func->GetParameter(3)*E[n]+func->GetParameter(4)*E[n]*E[n];
        g_width[n]->SetPoint(g_width[n]->GetN(),ThetaCM,Width);
        CS[n]->SetPoint(CS[n]->GetN(),ThetaCM+0.5*step,par[n*2+6]/SolidAngle);
        CS[n]->SetPointError(CS[n]->GetN()-1,0,func->GetParError(n*2+6)/SolidAngle);
      }
    }
    else{
      pad= pad-1;
    }
  }

  // Draw Width of each state vs ThetaCM
  c4->cd(pad++);
  const UInt_t Number = 2;
  Double_t Red[Number]    = { 0,  0   };
  Double_t Green[Number]  = { 0,  0.8 };
  Double_t Blue[Number]   = { 0,  1.00 };

  Double_t Length[Number] = { 0,1.00 };
  Int_t nb=g_width.size() ;
  Int_t color =  TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);

  for(unsigned int n = 0 ; n < g_width.size() ; n++){
    g_width[n]->SetMarkerColor(color++) ;
    g_width[n]->SetMarkerSize(0.4);
    if(n==0){
      g_width[n]->Draw("ap");
      g_width[n]->GetYaxis()->SetRangeUser(0.3,1);
    }
    else
      g_width[n]->Draw("p");  
  }

  // Draw the shift of Ex spectrum for each ThetaCM
  c4->cd(pad++);
  g_shift->SetLineColor(kAzure+7);
  g_shift->Draw("acp");



  TCanvas* c = new TCanvas("Mg","Mg",2000,2000*2./5.);
  c->Divide(5,2,0.00001,0.00001);


  for(unsigned int i = 0 ; i < CS.size() ; i++){
    c->cd(i+1);

    CS[i]->Draw("ap");
    gPad->SetLogy();

    TGraph* gs = TWOFNR(E[i], 0,0.5,1,0, 0.5);
    TGraph* gp = TWOFNR(E[i], 0,1.5,1,1, 1.5);
    TGraph* gd = TWOFNR(E[i], 0,1.5,0,2, 1.5);
    TGraph* gf = TWOFNR(E[i], 0,3.5,0,3, 3.5);
    TGraph* gg = TWOFNR(E[i], 0,4.5,0,4, 4.5);

    Scale(gs,CS[i]);
    Scale(gp,CS[i]);
    Scale(gd,CS[i]);
    Scale(gf,CS[i]);
    Scale(gg,CS[i]);

    gs->SetLineColor(kGreen);
    gp->SetLineColor(kBlue);
    gd->SetLineColor(kRed);
    gf->SetLineColor(kBlack);
    gg->SetLineColor(kAzure+7);

    if(i!=0){
      gp->Draw("c");
      gd->Draw("c");
      gf->Draw("c");
      gs->Draw("c");
      gg->Draw("c");
      TLegend* leg = new TLegend(0.7,0.6,1,0.9);
      leg->SetBorderSize(0);
      leg->SetFillStyle(0);
      leg->AddEntry(gs,"L=0","l");
      leg->AddEntry(gp,"L=1","l");
      leg->AddEntry(gd,"L=2","l");
      leg->AddEntry(gf,"L=3","l");
      leg->AddEntry(gg,"L=4","l");
      leg->Draw();
    }

    else{
      gs->Draw("c"); 
      gd->Draw("c"); 
      TGraph* gm = new TGraph(*gs);
      double* XX = gm->GetX();
      double* YY = gm->GetY();
      double* ZZ = gd->GetY();
      for(int n = 0 ; n < gs->GetN() ; n++)
        gm->SetPoint(n,XX[n],YY[n]+ZZ[n]);
      Scale(gm,CS[i]);
      gm->SetLineStyle(2);
      gm->SetLineColor(kGreen-3);
      gm->Draw("c");
      TLegend* leg = new TLegend(0.7,0.6,1,0.9);
      leg->SetBorderSize(0);
      leg->SetFillStyle(0);
      leg->AddEntry(gs,"L=0","l");
      leg->AddEntry(gd,"L=2","l");
      leg->AddEntry(gm,"L=2+0","l");
      leg->Draw();
    }

    TLatex* tex = new TLatex(10,1.7e3,Form("%.2f MeV",E[i]));
    tex->Draw();
    CS[i]->GetYaxis()->SetRangeUser(1,1e4);
    CS[i]->GetXaxis()->SetRangeUser(0,40);
  }
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
  if( QValue < 0 )
    Front_Input << 0.01 << std::endl;
  else
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

  TGraph* CS = new TGraph("22.jjj");
  return CS;
}
