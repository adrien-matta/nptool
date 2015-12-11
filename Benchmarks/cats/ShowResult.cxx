TChain* chain=NULL;
TCanvas* c1=NULL;
void LoadChain();

void ShowResult()
{
    LoadChain();
    c1 = new TCanvas("Mask CATS E644 experiment","Mask CATS E644 experiment",1200,600);
    c1->Divide(2,1);
    c1->cd(1);
    // Y versus X for CATS1 detector
    chain->Draw("PositionY[0]:PositionX[0]>>h(600,-30,30,600,-30,30)","","colz");
    TLatex *   texO = new TLatex(-17.5,22,"Obtained");
    texO->Draw();
    
    c1->cd(2);
    TFile* ref = new TFile("reference.root","READ");
    TH2* href = (TH2*) ref->FindObjectAny("href");
    href->Draw("colz");
    TLatex *   texR = new TLatex(-17.5,22,"Reference");
    texR->Draw();
    
}


////////////////////////////////////////////////////////////////////////////////
void LoadChain(){
    chain = new TChain("PhysicsTree");
    chain->Add("../../Outputs/Analysis/benchmark_cats.root");
}
