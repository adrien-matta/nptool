#include"NPReaction.h"


TChain* chain=NULL ;
TCanvas* c1 = NULL;
TCanvas* c2 = NULL;
TCanvas* c3 = NULL;

////////////////////////////////////////////////////////////////////////////////
void LoadChain(){
    chain = new TChain("PhysicsTree");
    chain->Add("../../Outputs/Analysis/hiraU_protonpbuu_nucl.root");
    //chain->Add("../../Outputs/Analysis/hiraU_flat_p_nucl.root");
}

////////////////////////////////////////////////////////////////////////////////
void ShowResults(){
    gROOT->SetStyle("nptool");
    
    LoadChain();

    c1 = new TCanvas("Example1","Example1",800,800);
    c1->Divide(2,2);

    // Light Particle ID //
    // E-DE
    c1->cd(1);
    chain->Draw("E_ThickSi:E_CsI>>hIDE(1000,0,220,1000,0,20)","","colz");
    TH1F* hIDE = (TH1F*) gDirectory->FindObjectAny("hIDE");
    hIDE->GetXaxis()->SetTitle("E_{CsI} (MeV)");
    hIDE->GetYaxis()->SetTitle("E_{Si} (MeV)");

    // TotalEnergy
    int bin=50;
    double Emin = 0;
    double Emax = 300;
    c1->cd(2);
    
    chain->Draw(Form("InitialEnergy_Hira>>hEi(%d,%f,%f)",bin,Emin,Emax),"InitialEnergy>0","E1");
    TH1F* hEi = (TH1F*) gDirectory->FindObjectAny("hEi");
    hEi->GetXaxis()->SetTitle("E_{lab} (MeV)");
    hEi->GetYaxis()->SetTitle("counts");
    hEi->SetMarkerColor(2);
    hEi->SetLineColor(2);
    hEi->Sumw2();
    
    chain->Draw(Form("ELab>>hEd(%d,%f,%f)",bin,Emin,Emax),"InitialEnergy>0","E1same");
    TH1F* hEd = (TH1F*) gDirectory->FindObjectAny("hEd");
    hEd->SetMarkerColor(4);
    hEd->SetLineColor(4);
    hEd->Sumw2();
    
    
    hEi->Draw("E1");
    hEd->Draw("E1same");
    

    c1->cd(3);
    TH1F* heff = new TH1F("heff","heff",bin,Emin,Emax);
    heff->Sumw2();
    heff->GetXaxis()->SetTitle("E_{lab} (MeV)");
    heff->GetYaxis()->SetTitle("Eff (%)");
    heff->GetYaxis()->SetRangeUser(0,100);
    heff->GetXaxis()->SetRangeUser(0,200);
    heff->SetMarkerColor(4);
    heff->SetLineColor(4);

    heff->Divide(hEd,hEi,1,1);
    heff->Draw("E1");
    
    TH1F* heff_nucl = new TH1F("heff_nucl","heff_nucl",bin,Emin,Emax);
    heff_nucl->Sumw2();
    heff_nucl->SetMarkerColor(8);
    heff_nucl->SetLineColor(8);
    

    /*heff->Fit("pol1");

    TF1* f = heff->GetFunction("pol1");
    f->SetLineWidth(2);
    f->SetLineColor(kOrange-3);
    f->SetNpx(1000);*/
    
    c1->cd(4);
    chain->Draw("ELab:ThetaLab>>htheta(160,0,120,500,0,200)","InitialEnergy>0","colz");
    TH1F* htheta = (TH1F*) gDirectory->FindObjectAny("htheta");
    htheta->GetXaxis()->SetTitle("#theta_{lab} (deg)");
    htheta->GetYaxis()->SetTitle("E_{lab} (MeV)");
    
    
    c2 = new TCanvas("CM","CM",800,800);
    c2->Divide(2,1);
    c2->cd(1)->SetLogy();
    chain->Draw("ECM_initial_Hira>>hECMi(50,0,300)","InitialEnergy>0 && InitialEnergy<200","E1");
    TH1F* hECMi = (TH1F*) gDirectory->FindObjectAny("hECMi");
    hECMi->GetXaxis()->SetTitle("E_{CM} (MeV)");
    hECMi->SetMarkerColor(2);
    hECMi->SetLineColor(2);
    hECMi->Sumw2();
    
    chain->Draw("ECM>>hECM(50,0,300)","ELab<200 && ELab>0","E1same");
    TH1F* hECM = (TH1F*) gDirectory->FindObjectAny("hECM");
    hECM->SetMarkerColor(4);
    hECM->SetLineColor(4);
    hECM->Sumw2();

    hECMi->Draw("E1");
    hECM->Draw("E1same");

    c2->cd(2);
    chain->Draw("ECM:ThetaLab>>hECMtheta(500,0,80,500,0,120)","","colz");
    TH1F* hECMtheta = (TH1F*) gDirectory->FindObjectAny("hECMtheta");
    hECMtheta->GetXaxis()->SetTitle("#theta_{lab} (deg)");
    hECMtheta->GetYaxis()->SetTitle("E_{CM} (MeV)");

    
    c3 = new TCanvas("Eff","Eff",0,0,800,800);
    c3->cd();
    TH1F* heff_CM = new TH1F("heff_CM","heff_CM",50,0,300);
    heff_CM->Sumw2();
    heff_CM->GetXaxis()->SetTitle("E_{CM} (MeV)");
    heff_CM->GetYaxis()->SetTitle("Eff");
    heff_CM->Divide(hECM,hECMi,1,1);
    heff_CM->Draw("E1");
    
    heff_CM->SaveAs("Efficiency_HiraU.root");
    
    
}
