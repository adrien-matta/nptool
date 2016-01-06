void NIM(){
  TCanvas* c = new TCanvas("c","c",900,900);


//  TFile* fexp = new TFile("Nana_exp.root","READ");
//  TH1* hexp = (TH1*) fexp->FindObjectAny("Detector3");
  TFile* fexp = new TFile("Eu_one_det.root","READ");
  TTree* treex = (TTree*) fexp->FindObjectAny("Tree"); 
//  treex->Draw("1000*(0.00193276+fNANA_LaBr3_EnergyLong*0.000175521+fNANA_LaBr3_EnergyLong*fNANA_LaBr3_EnergyLong*-3.88985e-10) >>Detector3x(4096,0,4096)","","");
   treex->Draw("fNANA_LaBr3_EnergyLong >>Detector3x(4096,0,4096)","","");

  TH1* hexp = (TH1*) fexp->FindObjectAny("Detector3x");

  TFile* fbgd = new TFile("Eu_1hr_bg.root","READ");
  TTree* treeb = (TTree*) fbgd->FindObjectAny("Tree"); 
 treeb->Draw("fNANA_LaBr3_EnergyLong>>Detector3(4096,0,4096)","","same");
 TH1* hbgd = (TH1*) fbgd->FindObjectAny("Detector3");

  double min2= 1700;
  double max2 = 3500;
  double normalisation_bgd = hexp->Integral(hexp->FindBin(min2),hexp->FindBin(max2))/hbgd->Integral(hbgd->FindBin(min2),hbgd->FindBin(max2));
  cout << "Normalisation = " << normalisation_bgd << endl;
 hbgd->Scale(normalisation_bgd);
 hexp->Add(hbgd,-1);

  TFile* fsim = new TFile("../../Outputs/Analysis/Eu_1hr.root","READ");
  TTree* tree = (TTree*) fsim->FindObjectAny("PhysicsTree"); 
 
  tree->Draw("LaBr_E*1000.>>sum2(4096,0,4096)","","E1 same");

  TH1* hsim = (TH1*) fsim->FindObjectAny("sum2");
  double min1= 400;
  double max1 = 1400;
  double normalisation = hexp->Integral(hexp->FindBin(min1),hexp->FindBin(max1))/hsim->Integral(hsim->FindBin(min1),hsim->FindBin(max1));
  cout << "Normalisation = " << normalisation << endl;
  hsim->Scale(normalisation);
 
  hsim->SetLineColor(kOrange-3);
  hsim->SetMarkerSize(0.7);
  hexp->Draw();
  hexp->GetXaxis()->SetTitle("LaBr3 Energy (keV)");
  hexp->GetYaxis()->SetTitle("Counts per 4 keV");
  hsim->Draw("same");
  hbgd->SetFillColor(kRed);
  hbgd->SetLineColor(kRed);
  hbgd->Draw("same");
 // hexp_o->SetFillColor(kGreen);
 // hexp_o->SetLineColor(kGreen);
  //hexp_o->Draw("same"); 
  
  gPad->SetLogy();
  }
