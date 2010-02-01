void DisplayInputCrossSection()
{
   // Path to cross-section files
   TString path = gSystem->Getenv("NPTOOL");
   path += "/Inputs/CrossSection/";

   // Read cross-sections 132Sn(d,p)
/*   TGraph *gr1 = new TGraph(path + "132Sndp_5A_MeV_3p3_ZR_FRC.lis");
   TGraph *gr2 = new TGraph(path + "132Sndp_5A_MeV_2f7_ZR_FRC.lis");
   TGraph *gr3 = new TGraph(path + "132Sndp_10A_MeV_3p3_ZR_FRC.lis");
   TGraph *gr4 = new TGraph(path + "132Sndp_10A_MeV_2f7_ZR_FRC.lis");*/
   // Jacques
   TGraph *gr1 = ReadCrossSection("132Sndp_5A_MeV_3p3_ZR_FRC.lis");
   TGraph *gr2 = ReadCrossSection("132Sndp_5A_MeV_2f7_ZR_FRC.lis");
   TGraph *gr3 = ReadCrossSection("132Sndp_10A_MeV_3p3_ZR_FRC.lis");
   TGraph *gr4 = ReadCrossSection("132Sndp_10A_MeV_2f7_ZR_FRC.lis");
   // Didier
   TGraph *gr5 = new TGraph(path + "sn132dp_gs_10AMeV.txt");

   // Read cross-section 134Sn(p,t)
   // Didier
   TGraph *grpt1 = ReadCrossSectionPT("CS_Ep10MeV_sn134pt_gs_1h9demi.dat");
   TGraph *grpt2 = ReadCrossSectionPT("CS_Ep15MeV_sn134pt_gs_1h9demi.dat");
   TGraph *grpt3 = ReadCrossSectionPT("CS_Ep20MeV_sn134pt_gs_1h9demi.dat");

   // Read cross-section 132Sn(d,d)
   // Angel
   TGraph *grdd = new TGraph(path + "132Sndd_10A_MeV_ruth.dat");
   TGraph *grpp = new TGraph(path + "132Snpp_10A_MeV_ruth.dat");

   // Draw cross-sections
   TCanvas *can = new TCanvas("can");
   can->SetLogy();
   can->Draw();
//   TH2F *hframe = new TH2F("hframe", "^{2}H(^{132}Sn,p)^{133}Sn", 180, 0, 180, 100, 1e-2, 100);
//   TH2F *hframe = new TH2F("hframe", "", 180, 0, 180, 100, 1e-2, 100);
//   TH2F *hframe = new TH2F("hframe", "^{1}H(^{134}Sn,t)^{132}Sn_{g.s.}", 180, 0, 180, 100, 1e-8, 1e-5);
//   TH2F *hframe = new TH2F("hframe", "", 180, 0, 180, 100, 1e-8, 1e-5);
   TH2F *hframe = new TH2F("hframe", "", 180, 0, 180, 100, 1e-3, 10);
   hframe->Draw();
   hframe->SetXTitle("#Theta_{c.m.} [deg]");
//   hframe->SetYTitle("d#sigma/d#Omega [mb/sr]");
   hframe->SetYTitle("d#sigma/d#Omega / (d#sigma/d#Omega)_{R}");
//   hframe->SetYTitle("#propto d#sigma/d#Omega");
/*   grpt1->SetLineColor(kRed);      grpt1->Draw("l");
   grpt2->SetLineColor(kMagenta);  grpt2->Draw("l");
   grpt3->SetLineColor(kBlue);     grpt3->Draw("l");*/
/*   gr1->SetLineColor(kRed);  gr1->SetLineStyle(2); gr1->Draw("l");
   gr2->SetLineColor(kRed);  gr2->Draw("l");
   gr3->SetLineColor(kBlue); gr3->SetLineStyle(2); gr3->Draw("l");
   gr4->SetLineColor(kBlue); gr4->Draw("l");*/
//   gr5->Draw("l");
//   grdd->Draw("l");
   grpp->Draw("l");

   // TLegend
   TLegend *leg = new TLegend(0.50, 0.64, 0.82, 0.84);
//   leg->AddEntry(grdd, "10 MeV/u", "l");
   leg->AddEntry(grpp, "10 MeV/u", "l");
/*   TLegend *leg = new TLegend(0.50, 0.64, 0.82, 0.84);
   leg->AddEntry(grpt1, "1h9/2 10 MeV/u", "l");
   leg->AddEntry(grpt2, "1h9/2 15 MeV/u", "l");
   leg->AddEntry(grpt3, "1h9/2 20 MeV/u", "l");*/
/*   TLegend *leg = new TLegend(0.16, 0.17, 0.48, 0.37);
   leg->AddEntry(gr1, "3p3/2 5 MeV/u", "l");
   leg->AddEntry(gr2, "2f7/2 5 MeV/u", "l");
   leg->AddEntry(gr3, "3p3/2 10 MeV/u", "l");
   leg->AddEntry(gr4, "2f7/2 10 MeV/u", "l");*/
   leg->SetBorderSize(1);
   leg->Draw();
   
/*   TMultiGraph *mgr = new TMultiGraph();
   mgr->Add(gr1, "lp");
   mgr->Add(gr2, "lp");
   mgr->Add(gr3, "lp");
   mgr->Add(gr4, "lp");
   mgr->Draw("a*");*/
//   gr1->Draw("alp");
}



TGraph* ReadCrossSection(const char* fname)
{
   // Path to cross-section files
   TString path = gSystem->Getenv("NPTOOL");
   path += "/Inputs/CrossSection/";

   // Open file
   ifstream fich;
   fich.open(path + fname);
   if (!fich) cout << "Probleme d'ouverture dans le fichier " << fname << endl;

   // Read file
   Double_t angle, sigma;
   Int_t nlines = 0;
   TGraph *gr = new TGraph();
   while (fich >> angle >> sigma) {
      gr->SetPoint(nlines++, angle, sigma * 15);	// 15: fm^2 -> mb + D0^2
   }

   // Close file
   fich.close();

   // TGraph name
   gr->SetTitle(fname);

   // test pour savoir si on a bien rempli le TGraph
   if (gr->GetN() == 0)
      cout << "Mauvaise lecture du fichier --> probablement mauvais format" << endl;

   return gr;
}



TGraph* ReadCrossSectionPT(const char* fname)
{
   // Path to cross-section files
   TString path = gSystem->Getenv("NPTOOL");
   path += "/Inputs/CrossSection/";

   // Open file
   ifstream fich;
   fich.open(path + fname);
   if (!fich) cout << "Probleme d'ouverture dans le fichier " << fname << endl;

   // Read file
   Double_t angle, sigma, dum;
   Int_t nlines = 0;
   TGraph *gr = new TGraph();
//   while (fich >> angle >> sigma >> dum >> dum >> dum >> dum >> dum >> dum >> dum >> dum >> dum) {
   while (fich >> angle >> sigma) {
      gr->SetPoint(nlines++, angle, sigma);
   }

   // Close file
   fich.close();

   // TGraph name
   gr->SetTitle(fname);

   // test pour savoir si on a bien rempli le TGraph
   if (gr->GetN() == 0)
      cout << "Mauvaise lecture du fichier --> probablement mauvais format" << endl;

   return gr;
}
