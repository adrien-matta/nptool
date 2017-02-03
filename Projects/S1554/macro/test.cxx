{
   c0 = new TCanvas("c1","multigraph L3",200,10,700,500);
  // c0->SetFrameFillColor(30);
   TMultiGraph *mg = new TMultiGraph();
   TGraph *gr1 = new TGraph(); gr1->SetLineColor(kBlue);
   TGraph *gr2 = new TGraph(); gr2->SetLineColor(kRed);
   TGraph *gr3 = new TGraph(); gr3->SetLineColor(kGreen);
   TGraph *gr4 = new TGraph(); gr4->SetLineColor(kOrange);
   Double_t dx = 6.28/100;
   Double_t x  = -3.14;
   for (int i=0; i<=100; i++) {
      x = x+dx;
      gr1->SetPoint(i,x,2.*TMath::Sin(x));
      gr2->SetPoint(i,x,TMath::Cos(x));
      gr3->SetPoint(i,x,TMath::Cos(x*x));
      gr4->SetPoint(i,x,TMath::Cos(x*x*x));
   }
   mg->Add(gr4); gr4->SetTitle("Cos(x*x*x)"); gr4->SetLineWidth(3);
   mg->Add(gr3); gr3->SetTitle("Cos(x*x)")  ; gr3->SetLineWidth(3);
   mg->Add(gr2); gr2->SetTitle("Cos(x)")    ; gr2->SetLineWidth(3);
   mg->Add(gr1); gr1->SetTitle("2*Sin(x)")  ; gr1->SetLineWidth(3);

   mg->Draw("a  l3d");
   return c0;
}
