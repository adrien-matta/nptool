void Sidd(){
 
  TFile* file = new TFile("../../Outputs/Analysis/S1554_28Sidd_Phy.root");
  TTree* tree = (TTree*) file->FindObjectAny("PhysicsTree");

  NPL::Reaction r("28Si(d,d)28Si@224");
  NPL::Reaction p("28Si(p,p)28Si@224");

  // Setting Trifoil entry list for fast drawing
  // Load Cuts
  TFile* fileCut = new TFile("cuts/Cut_dd.root");
  TCutG* Cut_dd = (TCutG*) fileCut->FindObjectAny("Cut_dd");
  
  TFile* fileCut1 = new TFile("cuts/cut_elgs_det10.root");
  TCutG* Cut_dd10 = (TCutG*) fileCut1->FindObjectAny("cut_elgs_det10");
 
  TFile* fileCut2 = new TFile("cuts/cut_elgs_det12.root");
  TCutG* Cut_dd12 = (TCutG*) fileCut2->FindObjectAny("cut_elgs_det12");
 
  // Create Canvas
  TCanvas* c = new TCanvas("Result","Result",900,900);
  c->Divide(2,2);
  TH1* h;
  TH1F* h2;

  c->cd(1);
  tree->Draw("ELab:ThetaLab>>hEXT4(360,0,180,1200,0,40)","ELab>0 ","colz");

  gPad->SetLogz();
  h2 = (TH1F*) gDirectory->FindObjectAny("hEXT4");
  h2->GetXaxis()->SetTitle("#theta_{Lab} (deg)");
  h2->GetYaxis()->SetTitle("E_{Lab} (MeV)");
  r.GetKinematicLine3()->Draw("c");  
  p.GetKinematicLine3()->Draw("c");  

  c->cd(2);
  tree->Draw("ThetaCM>>hCM(180,0,180)","ELab>0 && (Cut_dd||Sharc.PAD_E<0) && Ex>-0.8 && Ex<0.8","");

  c->cd(3);
  tree->Draw("ELab:ThetaLab>>hEXTA(360,0,180,1200,0,40)","ELab>0 && (Cut_dd||Sharc.PAD_E<0) && Sharc.DetectorNumber==12","colz");
  c->cd(4);
  tree->Draw("ELab:ThetaLab>>hEXTB(360,0,180,1200,0,40)","ELab>0 && (Cut_dd||Sharc.PAD_E<0)&& Sharc.DetectorNumber ==10","colz");

}
