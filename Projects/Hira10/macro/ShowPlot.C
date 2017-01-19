TChain* chain1=NULL ;
TChain* chain2=NULL ;

TCanvas* c1 = NULL;
TH1F* heff = NULL;

////////////////////////////////////////////////////////////////////////////////
void LoadChain(){
    chain1 = new TChain("PhysicsTree");
    chain1->Add("/Users/pierremorfouace/Physics/NPTool/nptool/Outputs/Analysis/hiraU_protonpbuu_multi.root");
    
    chain2 = new TChain("PhysicsTree");
    chain2->Add("/Users/pierremorfouace/Physics/NPTool/nptool/Outputs/Analysis/hiraU_protonpbuu_nucl.root");
}

////////////////////////////////////////////////////////////////////////////////
void LoadEfficiency(){
    TFile* f1 = new TFile("/Users/pierremorfouace/Physics/NPTool/nptool/Projects/Hira_upgrade/Efficiency/Efficiency_HiraU_flat.root");
    heff = (TH1F*) f1->FindObjectAny("heff_CM");
    //heff->Sumw2();
}

////////////////////////////////////////////////////////////////////////////////
void ShowPlot()
{
    gROOT->SetStyle("nptool");
    LoadChain();
    LoadEfficiency();
    
    TFile *f1 = new TFile("/Users/pierremorfouace/Physics/NPTool/nptool/Inputs/EventGenerator/sn112e120_p_energyCM.root");
    TH1F* hecm_pbuu_sn112 = (TH1F*) f1->FindObjectAny("henergy");
    hecm_pbuu_sn112->SetMarkerColor(2);
    hecm_pbuu_sn112->SetLineColor(2);
    hecm_pbuu_sn112->Rebin(1);
    hecm_pbuu_sn112->Scale(31);
    
    TCanvas *c1 = new TCanvas("c1","c1",1200,600);
    c1->Divide(2,1);
    
    // ELab //
    c1->cd(1);
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd()->SetLogy();               // pad1 becomes the current pad
    
    chain1->Draw("ELab>>hELab1(20,0,200)","InitialEnergy>0","E1");
    TH1F* hELab1 = (TH1F*) gDirectory->FindObjectAny("hELab1");
    hELab1->SetMarkerColor(4);
    hELab1->SetLineColor(4);
    hELab1->SetMarkerStyle(8);
    hELab1->SetName("multi");
    hELab1->GetXaxis()->SetTitle("E_{lab} (MeV)");
    hELab1->GetYaxis()->SetTitle("counts (a.u.)");
    hELab1->GetXaxis()->SetRangeUser(0,220);
    
    chain2->Draw("InitialEnergy_Hira>>hEi(20,0,200)","InitialEnergy>0","E1");
    TH1F* hEi = (TH1F*) gDirectory->FindObjectAny("hEi");
    hEi->SetMarkerColor(1);
    hEi->SetLineColor(1);
    hEi->SetMarkerStyle(8);
    hEi->SetName("normal");
    hEi->GetXaxis()->SetTitle("E_{lab} (MeV)");
    hEi->GetYaxis()->SetTitle("counts (a.u.)");
    hEi->GetXaxis()->SetRangeUser(0,220);
    
    chain2->Draw("ELab>>hELab2(20,0,200)","InitialEnergy_Hira>0","E1");
    TH1F* hELab2 = (TH1F*) gDirectory->FindObjectAny("hELab2");
    hELab2->SetMarkerColor(2);
    hELab2->SetLineColor(2);
    hELab2->SetMarkerStyle(8);
    hELab2->SetName("nucl");
    
    hEi->Draw("E1");
    hELab1->Draw("E1same");
    hELab2->Draw("E1same");
    
    hELab1->GetYaxis()->SetTitleSize(25);
    hELab1->GetYaxis()->SetTitleFont(43);
    hELab1->GetYaxis()->SetTitleOffset(1.55);
    
    
    TLegend *leg = new TLegend(0.6,0.7,0.9,0.95);
    leg->SetBorderSize(1);
    leg->SetLineColor(1);
    leg->AddEntry("normal","Original spectrum","lp");
    leg->AddEntry("multi","Multiple scattering","lp");
    leg->AddEntry("nucl","Nuclear reaction loss","lp");
    leg->Draw();
    
    c1->cd(1);
    TPad *pad2 = new TPad("pad2", "pad2", 0.0, 0.05, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.36);
    pad2->SetGridx();
    pad2->Draw();
    pad2->cd();       // pad2 becomes the current pad
    // Define the ratio plot
    TH1F *hratio_lab = (TH1F*)hELab2->Clone("hratio_lab");
    hratio_lab->Sumw2();
    hratio_lab->Divide(hEi);
    hratio_lab->SetMarkerColor(2);
    hratio_lab->SetLineColor(2);
    hratio_lab->SetMarkerStyle(8);
    hratio_lab->SetMarkerSize(1);
    hratio_lab->GetXaxis()->SetRangeUser(0,220);
    hratio_lab->GetYaxis()->SetRangeUser(0.5,1.05);
    hratio_lab->Draw("E1");
    
    hratio_lab->GetXaxis()->SetTitle("E_{lab} (MeV)");
    hratio_lab->GetXaxis()->CenterTitle();
    hratio_lab->GetYaxis()->SetTitle("ratio");
    hratio_lab->GetYaxis()->SetNdivisions(505);
    hratio_lab->GetYaxis()->SetTitleSize(25);
    hratio_lab->GetYaxis()->SetTitleFont(43);
    hratio_lab->GetYaxis()->SetTitleOffset(1.55);
    hratio_lab->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    hratio_lab->GetYaxis()->SetLabelSize(20);
    
    // X axis ratio plot settings
    hratio_lab->GetXaxis()->SetTitleSize(25);
    hratio_lab->GetXaxis()->SetTitleFont(43);
    hratio_lab->GetXaxis()->SetTitleOffset(4.);
    hratio_lab->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    hratio_lab->GetXaxis()->SetLabelSize(20);
    
    
    TH1F *hratio_lab2 = (TH1F*)hELab1->Clone("hratio_lab");
    hratio_lab2->Sumw2();
    hratio_lab2->Divide(hEi);
    hratio_lab2->SetMarkerColor(4);
    hratio_lab2->SetLineColor(4);
    hratio_lab2->SetMarkerStyle(8);
    hratio_lab2->SetMarkerSize(1);
    hratio_lab2->Draw("E1same");

    
    
    // ECM //
    c1->cd(2);
    TPad *pad3 = new TPad("pad3", "pad3", 0, 0.3, 1, 1.0);
    pad3->SetBottomMargin(0); // Upper and lower plot are joined
    pad3->SetGridx();         // Vertical grid
    pad3->Draw();             // Draw the upper pad: pad1
    pad3->cd()->SetLogy();               // pad1 becomes the current pad
    
    chain1->Draw("ECM>>hECM1(20,0,200)","","E1");
    TH1F* hECM1 = (TH1F*) gDirectory->FindObjectAny("hECM1");
    hECM1->SetMarkerColor(4);
    hECM1->SetLineColor(4);
    hECM1->SetMarkerStyle(8);
    hECM1->GetXaxis()->SetTitle("E_{c.m.} (MeV)");
    hECM1->GetYaxis()->SetTitle("counts (a.u.)");
    hECM1->GetXaxis()->SetRangeUser(0,200);
    
    chain2->Draw("ECM_initial_Hira>>hECMi(20,0,200)","InitialEnergy>0 && InitialEnergy<200","E1");
    TH1F* hECMi = (TH1F*) gDirectory->FindObjectAny("hECMi");
    hECMi->SetMarkerColor(1);
    hECMi->SetLineColor(1);
    hECMi->SetMarkerStyle(8);
    hECMi->SetName("normal");
    hECMi->GetXaxis()->SetTitle("E_{c.m.} (MeV)");
    hECMi->GetYaxis()->SetTitle("counts (a.u.)");
    hECMi->GetXaxis()->SetRangeUser(0,200);
    hECMi->GetYaxis()->SetRangeUser(10,2e5);
    
    chain2->Draw("ECM>>hECM2(20,0,200)","","E1");
    TH1F* hECM2 = (TH1F*) gDirectory->FindObjectAny("hECM2");
    hECM2->SetMarkerColor(2);
    hECM2->SetLineColor(2);
    hECM2->SetMarkerStyle(8);
    //hECM2->Sumw2();

    hECM1->GetYaxis()->SetTitleSize(25);
    hECM1->GetYaxis()->SetTitleFont(43);
    hECM1->GetYaxis()->SetTitleOffset(1.55);
    
    hECMi->Draw("E1");
    hECM1->Draw("E1same");
    hECM2->Draw("E1same");
    
    cout << "Mean value of the energy in the c.m" << endl;
    cout << "Original: " << hECMi->GetMean() << endl;
    cout << "Multiple: " << hECM1->GetMean() << endl;
    cout << "Nuclear: " << hECM2->GetMean() << endl;
    
    c1->cd(2);
    TPad *pad4 = new TPad("pad4", "pad4", 0.0, 0.05, 1, 0.3);
    pad4->SetTopMargin(0);
    pad4->SetBottomMargin(0.36);
    pad4->SetGridx();
    pad4->Draw();
    pad4->cd();       // pad4 becomes the current pad
    // Define the ratio plot
    TH1F *hratio_cm = (TH1F*)hECM2->Clone("hratio_cm");
    hratio_cm->Sumw2();
    hratio_cm->Divide(hECMi);
    hratio_cm->SetMarkerColor(2);
    hratio_cm->SetLineColor(2);
    hratio_cm->SetMarkerStyle(8);
    hratio_cm->SetMarkerSize(1);
    hratio_cm->GetXaxis()->SetRangeUser(0,200);
    hratio_cm->GetYaxis()->SetRangeUser(0.5,1.05);
    hratio_cm->Draw("E1");
    
    hratio_cm->GetXaxis()->SetTitle("E_{c.m.} (MeV)");
    hratio_cm->GetXaxis()->CenterTitle();
    hratio_cm->GetYaxis()->SetTitle("ratio");
    hratio_cm->GetYaxis()->SetNdivisions(505);
    hratio_cm->GetYaxis()->SetTitleSize(25);
    hratio_cm->GetYaxis()->SetTitleFont(43);
    hratio_cm->GetYaxis()->SetTitleOffset(1.55);
    hratio_cm->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    hratio_cm->GetYaxis()->SetLabelSize(20);
    
    // X axis ratio plot settings
    hratio_cm->GetXaxis()->SetTitleSize(25);
    hratio_cm->GetXaxis()->SetTitleFont(43);
    hratio_cm->GetXaxis()->SetTitleOffset(4.);
    hratio_cm->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    hratio_cm->GetXaxis()->SetLabelSize(20);
    
    TH1F *hratio_cm2 = (TH1F*)hECM1->Clone("hratio_cm");
    hratio_cm2->Sumw2();
    hratio_cm2->Divide(hECMi);
    hratio_cm2->SetMarkerColor(4);
    hratio_cm2->SetLineColor(4);
    hratio_cm2->SetMarkerStyle(8);
    hratio_cm2->SetMarkerSize(1);
    hratio_cm2->Draw("E1same");

    
}