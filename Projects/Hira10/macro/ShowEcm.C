TChain* chain1=NULL ;
TChain* chain2=NULL ;

TCanvas* c1 = NULL;
TH1F* heff = NULL;

////////////////////////////////////////////////////////////////////////////////
void LoadChain(){
    chain1 = new TChain("PhysicsTree");
    chain1->Add("/Users/pierremorfouace/Physics/NPTool/nptool/Outputs/Analysis/hiraU_protonpbuu_multi.root");
    
    chain2 = new TChain("PhysicsTree");
    //chain2->Add("/Users/pierremorfouace/Physics/NPTool/nptool/Outputs/Analysis/hiraU_pid_nucl.root");
    //chain2->Add("/Users/pierremorfouace/Physics/NPTool/nptool/Outputs/Analysis/hiraU_flat_p_nucl.root");
    //chain2->Add("/Users/pierremorfouace/Physics/NPTool/nptool/Outputs/Analysis/hiraU_protonsource_nucl.root");
    chain2->Add("/Users/pierremorfouace/Physics/NPTool/nptool/Outputs/Analysis/test.root");
}

////////////////////////////////////////////////////////////////////////////////
void LoadEfficiency(){
    //TFile* f1 = new TFile("/Users/pierremorfouace/Physics/NPTool/nptool/Projects/Hira_upgrade/Efficiency/Efficiency_HiraU_flat.root");
    
    TFile* f1 = new TFile("/Users/pierremorfouace/Physics/NPTool/nptool/Projects/Hira_upgrade/Efficiency_HiraU.root");
    heff = (TH1F*) f1->FindObjectAny("heff_CM");
    //heff->Sumw2();
}

////////////////////////////////////////////////////////////////////////////////
void ShowEcm()
{
    gROOT->SetStyle("nptool");
    LoadChain();
    LoadEfficiency();
    
    TFile *f1 = new TFile("/Users/pierremorfouace/Physics/NPTool/nptool/Inputs/EventGenerator/sn112e120_p_energyCM.root");
    TH1F* hecm_pbuu_sn112 = (TH1F*) f1->FindObjectAny("henergy");
    hecm_pbuu_sn112->SetMarkerColor(2);
    hecm_pbuu_sn112->SetLineColor(2);
    hecm_pbuu_sn112->Rebin(1);
    hecm_pbuu_sn112->Scale(62);
    
    
    /*chain1->Draw("ECM>>hECM1(50,0,300)","InitialEnergy>0","E1");
    TH1F* hECM1 = (TH1F*) gDirectory->FindObjectAny("hECM1");
    hECM1->SetMarkerColor(4);
    hECM1->SetLineColor(4);
    hECM1->SetMarkerStyle(8);*/
    //hECM1->Sumw2();
    
    chain2->Draw("ECM>>hECM2(50,0,300)","Particle==1 && ELab<200","E1");
    TH1F* hECM2 = (TH1F*) gDirectory->FindObjectAny("hECM2");
    hECM2->SetMarkerColor(2);
    hECM2->SetLineColor(2);
    hECM2->SetMarkerStyle(8);
    //hECM2->Sumw2();

    chain2->Draw("ECM_initial>>hECMi(50,0,300)","","E1");
    TH1F* hECMi = (TH1F*) gDirectory->FindObjectAny("hECMi");
    hECMi->SetMarkerColor(1);
    hECMi->SetLineColor(1);
    hECMi->SetMarkerStyle(8);
    hECMi->Scale(1);
    
    TH1F *hECM_correct = new TH1F("hECM_correct","hECM_correct",50,0,300);
    hECM_correct->Divide(hECM2,heff,1,1);
    hECM_correct->SetMarkerColor(8);
    hECM_correct->SetLineColor(8);
    hECM_correct->SetMarkerStyle(8);
    hECM_correct->Scale(1.3);

   
    
    //c1 = new TCanvas("c1","c1",800,600);
    //c1->SetLogy();
    
    //hECM1->Draw("E1same");
    
    hECMi->Draw("E1");
    hECM_correct->Draw("E1same");
    hECM2->Draw("E1same");
    hecm_pbuu_sn112->Draw("E1same");
    
}