void Results(TString name="R1_test.root"){
  
gStyle->SetPalette(55); // dark rainbow
//gStyle->SetPalette(53); // sunset

  TFile* File = new TFile("../../Outputs/Analysis/"+name);
  TTree* Tree = (TTree*) File->FindObjectAny("PhysicsTree");

  TCanvas* c = new TCanvas("Barrel","Barrel");
  c->Divide(2,2);  
  c->cd(1);
  Tree->Draw("(TiaraBarrel.UpStream_E+TiaraBarrel.DownStream_E):TiaraBarrel.Strip_N+(TiaraBarrel.DetectorNumber-1)*4>>h1(35,0,35,1000,0,6500)","","colz");
  c->cd(2);
  Tree->Draw("TiaraBarrel.UpStream_E+TiaraBarrel.DownStream_E>>h2(1000,0,6500)","","");
  c->cd(3);
  Tree->Draw("TiaraBarrel.Strip_E:TiaraBarrel.Strip_N+(TiaraBarrel.DetectorNumber-1)*4>>h3(35,0,35,1000,0,6500)","","colz");
  c->cd(4);
  Tree->Draw("TiaraBarrel.Strip_E>>h4(1000,0,6500)","","");

  TCanvas* c2 = new TCanvas("UvsD","Upstream vs Downstream");
  Tree->Draw("TiaraBarrel.UpStream_E:TiaraBarrel.DownStream_E>>h5(1000,0,6500,1000,0,6500)","","colz");

  TCanvas* c3 = new TCanvas("PvsUD","Position vs Upstream+Downstream");
  c3->Divide(2,1); 
  c3->cd(1);
  Tree->Draw("TiaraBarrel.Strip_Pos:TiaraBarrel.DownStream_E+TiaraBarrel.UpStream_E>>h6(500,3500,6500,200,-1,+1)","","colz");
  c3->cd(2);
  Tree->Draw("TiaraBarrel.Strip_Pos:TiaraBarrel.Strip_E>>h7(500,3500,6500,200,-1,+1)","","colz");

/*
  Tree->Draw("ELab:ThetaLab>>h2(1000,0,180,1000,0,20)","ELab>0","colz");
  Tree->Draw("Ex>>h4(1000,-2,5)","ELab>0 && Ex>-1000 && ThetaLab>164");
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
*/
}
