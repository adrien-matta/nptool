void Results(){
  TFile* File = new TFile("../../Outputs/Analysis/PhysicsTree.root");
  TTree* Tree = (TTree*) File->FindObjectAny("PhysicsTree");

  Tree->Draw("ELab:ThetaLab>>h2(1000,0,180,1000,0,20)","ELab>0","colz");
  new TCanvas();
  Tree->Draw("Ex>>h4(1000)","ELab>0 && Ex>-1000");
  TCanvas* c = new TCanvas("CAS","Control Analyse-Simu");
  c->Divide(2,2);
  c->cd(1);
  Tree->Draw("ELab:Original_ELab>>hcE(1000,0,30,1000,0,30)","","colz");
  TH2* h = (TH2*) gDirectory->FindObjectAny("hcE");
  h->GetXaxis()->SetTitle("Simulated_{} E_{lab}       ");
  h->GetYaxis()->SetTitle("Reconstructed_{} E_{lab}   "); 
  TLine* lineE = new TLine (0,0,30,30);
  lineE->Draw();
  c->cd(2);
  Tree->Draw("ThetaLab:Original_ThetaLab>>hcT(1000,0,180,1000,0,180)","","colz");
  h = (TH2*) gDirectory->FindObjectAny("hcT");
  h->GetXaxis()->SetTitle("Simulated #theta_{lab}   ");
  h->GetYaxis()->SetTitle("Reconstructed #theta_{lab}   ");
  TLine* lineT = new TLine (0,0,180,180);  
  lineT->Draw();
}
