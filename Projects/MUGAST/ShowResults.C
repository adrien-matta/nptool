void ShowResults()
{
   // get tree   
   TFile *f = new TFile("../../Outputs/Analysis/PhysicsTree.root");
   TTree *t = (TTree*) f->Get("PhysicsTree");

   // draw 
   TCanvas *c1 = new TCanvas("c1", "kinematic information", 600, 600);
   c1->Draw();
   TH2F *hk = new TH2F("hk", "hk", 90, 90, 180, 200, 0, 12);
   hk->GetXaxis()->SetTitle("#Theta_{lab} (deg)");
   hk->GetYaxis()->SetTitle("E_{p} (MeV)");
   t->Draw("ELab:ThetaLab>>hk");
   // inset
   TPad *pad = new TPad("pad1", "excitation energy", 0.45, 0.45, 0.87, 0.87);
   pad->Draw();
   pad->cd();
   TH1F *hx = new TH1F("hx", "hx", 150, 5, 8);
   hx->SetXTitle("E_{X} (MeV)");
   hx->SetYTitle("counts / (20 keV)");
   t->Draw("Ex>>hx");
}
