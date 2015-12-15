TChain* chain=NULL;


////////////////////////////////////////////////////////////////////////////////
void LoadChain()
{
    chain = new TChain("PhysicsTree");
    chain->Add("../../Outputs/Analysis/benchmark_cats.root");
}

////////////////////////////////////////////////////////////////////////////////
void ShowResult()
{
   // load chain from result
   LoadChain();

   // draw canvas
   TCanvas *c1 = new TCanvas("Mask CATS E644 experiment", "Mask CATS E644 experiment", 1200, 600);
   c1->Divide(2,1);
   c1->Draw();

   // draw results from benchmark
   c1->cd(1);
   chain->Draw("PositionY[0]:PositionX[0]>>h(600,-30,30,600,-30,30)", "", "colz");
   TLatex *texO = new TLatex(-17.5, 22, "Obtained");
   texO->Draw();

   // draw results from reference result
   c1->cd(2);
   TFile* ref = new TFile("reference.root", "READ");
   TH2* href = (TH2*) ref->FindObjectAny("href");
   href->Draw("colz");
   TLatex *texR = new TLatex(-17.5,22, "Reference");
   texR->Draw();
}



