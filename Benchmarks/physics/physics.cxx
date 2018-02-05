
void EnergyLoss();
void Reaction();
TCanvas* c = new TCanvas("PhysicsBench","Physics Benchmark ",1500,500);

////////////////////////////////////////////////////////////////////////////////
void physics(){
  c->Divide(3,1);
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
  unsigned int cycles = 10000;
  // test Random Ex generation
    NPL::Reaction r2;
    r2.ReadConfigurationFile("test.reaction");
    TH1F* h2 = new TH1F("hE2","hE2",1000,-1,1);
    double E4,T4,E3,T3,Ex,E;  
    r2.ShootRandomExcitationEnergy();
    clock_t begin = clock(); 
    for (unsigned int i = 0 ; i < cycles ; i++){
      r2.ShootRandomExcitationEnergy();
      Ex = r2.GetExcitation4();
      r2.SetThetaCM(i*deg*180./cycles);
      r2.KineRelativistic(T3,E3,T4,E4);
      E = r2.ReconstructRelativistic(E3,T3)-Ex;
      h2->Fill(E/eV);
    } 
    clock_t end = clock();
    double time = end-begin;
    time = time/CLOCKS_PER_SEC;
    cout << " ***** Reaction performance : " << cycles/time/1000. << " cycles per ms *****" << endl; 
    c->cd(3);
    h2->Draw("");
    h2->GetXaxis()->SetTitle("Excitation error on Generated/Reconstructed cycle (eV)");

  // On fly declaration
  cout << " Reaction declaration without input file / no Ex distribution" << endl;
  NPL::Reaction r("28Si(d,p)29Si@280");
  TH1F* h = new TH1F("hE","hE",1000,-1,1);
  begin = clock(); 
  for (unsigned int i = 0 ; i < cycles ; i++){
    r.SetExcitation4(0);
    Ex = r2.GetExcitation4();
    r.SetThetaCM(i*deg*180./cycles);
    r.KineRelativistic(T3,E3,T4,E4);
    E = r.ReconstructRelativistic(E3,T3);
    h->Fill(E/eV);
  } 
  end = clock();
  time = end-begin;
  time = time/CLOCKS_PER_SEC;
  cout << " ***** Reaction performance :" << cycles/time/1000. << " cycles per ms *****" << endl; 
  c->cd(2);
  h->Draw("");
  h->GetXaxis()->SetTitle("Excitation error on Generated/Reconstructed cycle (eV)");


}

