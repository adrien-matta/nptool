
void EnergyLoss();
void Reaction();
TCanvas* c = new TCanvas("PhysicsBench","Physics Benchmark ",1000,500);

////////////////////////////////////////////////////////////////////////////////
void physics(){
  c->Divide(2,1);
  EnergyLoss();
  Reaction();
}
////////////////////////////////////////////////////////////////////////////////
void EnergyLoss(){
  NPL::EnergyLoss EL("test.G4table","G4Table",100);
  TH1F* h = new TH1F("hEL","hEL",1000,-1,1);
  clock_t begin = clock(); 
  for (unsigned int i = 0 ; i < 10000 ; i++){
    double E  = i;
    double ES = EL.Slow(E,10*micrometer,60*deg);
    double EF = EL.EvaluateInitialEnergy(ES,10*micrometer,60*deg);
    h->Fill((E-EF)/eV);
  } 
  clock_t end = clock();
  double time = end-begin;
  time = time/CLOCKS_PER_SEC;
  cout << " ***** Energy Loss performance : 10000 cycle done in " << time*1000 << "ms *****" << endl; 
  c->cd(1);
  h->Draw("");
  gPad->SetLogy();
  h->GetXaxis()->SetTitle("Energy error on Slow/Eval cycle (eV)");
}
////////////////////////////////////////////////////////////////////////////////
void Reaction() {
  NPL::Reaction r("28Si(d,p)29Si@280");
  double E4,T4,E3,T3;
  TH1F* h = new TH1F("hE","hE",1000,-1,1);
  clock_t begin = clock(); 
  for (unsigned int i = 0 ; i < 10000 ; i++){
    r.SetThetaCM(i*deg*180./10000);
    r.KineRelativistic(T3,E3,T4,E4);
    double E = r.ReconstructRelativistic(E3,T3);
    h->Fill(E/eV);
  } 
  clock_t end = clock();
  double time = end-begin;
  time = time/CLOCKS_PER_SEC;
  cout << " ***** Reaction performance : 10000 cycle done in " << time*1000 << "ms *****" << endl; 

  c->cd(2);
  h->Draw("");
  h->GetXaxis()->SetTitle("Excitation error on Generated/Reconstructed cycle (eV)");
}

