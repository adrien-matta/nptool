void Resolution(){

  TFile* file = new TFile("../../Outputs/Analysis/PhysicsTree.root");
  TTree* tree = (TTree*) file->FindObjectAny("PhysicsTree");

  TCanvas* c = new TCanvas("ExTheta","ExTheta");
  c->Divide(3,3);
  TH2* h ;
  vector<TGraph*>  g;
  TGraph* gt = new TGraph();
  double* pp;


  const double* par;
  unsigned int nbin = 10;
  double minCM = 4;
  double maxCM = 34;
  double step = (maxCM-minCM)/nbin;
  TH1D* p; 
  double ThetaCM;
  g.resize(nbin,NULL);
  int point = 0 ;

  for (unsigned int i = 0 ; i < 7 ; i++){
    c->cd(i+1);
  
    int entries = tree->Draw(Form("Ex:ThetaCM>>h%i(180,0,180,1000,-1,10)",i),Form("Run==%i && Plastic.Energy@.size()==1 && ThetaLab>90&& Sharc.Strip_E > 0.8",i+1));
    h = (TH2*) gDirectory->FindObjectAny(Form("h%i",i));

    for(unsigned int b = 0 ; b < nbin ; b++){
      if(g[b]==NULL)
        g[b]=new TGraph();
  
      ThetaCM = minCM+b*step;  
      p = h->ProjectionY(Form("p%i",b*10+i),h->GetXaxis()->FindBin(ThetaCM),h->GetXaxis()->FindBin(ThetaCM+step));
      if(p->Integral()>0){
        p->Fit("gaus","Q");
        pp = p->GetFunction("gaus")->GetParameters();
        if(pp[2]<0.18)
        g[b]->SetPoint(g[b]->GetN(),pp[1],pp[2]);
        gt->SetPoint(gt->GetN(),pp[1],pp[2]);

      }
    }
  }


  TCanvas* cg = new TCanvas("Res","Res");
  cg->Divide(4,4);
  for(unsigned int i = 0 ; i < nbin; i++){
    cg->cd(i+1);
    g[i]->Draw("ap");
    g[i]->Fit("pol2","F");
  }
   cg->cd(nbin+1);
   gt->Draw("ap");    
   gt->Fit("pol2","F");    
  cg->cd(nbin+2);
   tree->Draw(Form("Ex:ThetaCM>>h%i(180,0,180,1000,-1,10)",1000),"Plastic.Energy@.size()==1 && ThetaLab>90 && Sharc.Strip_E > 0.8","colz");

}
