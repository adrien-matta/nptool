void SiCheck2(){
  TFile* file = new TFile("../../Outputs/Analysis/S1554_28Si_Phy.root");
  TTree* tree = (TTree*) file->FindObjectAny("PhysicsTree");
  NPL::Reaction r("28Si(d,p)29Si@222");
  r.SetExcitationHeavy(4.9336);
  TGraph* line1 = r.GetKinematicLine3();
  tree->SetAlias("GoodEvent","ELab>0 && AddBack_DC/1000. >4.6 && AddBack_DC/1000.<5.4 && Ex<6.5");
  new TCanvas();
  tree->Draw(Form("ELab:ThetaLab>>hEX%i(180,0,180,400,0,10)",5)," DetectorNumber == 5 && GoodEvent","colz");
  line1->Draw("c");
 new TCanvas();
  tree->Draw(Form("ELab:ThetaLab>>hEX%i(180,0,180,400,0,10)",6),"DetectorNumber == 6 && GoodEvent","colz");
  line1->Draw("c");
 new TCanvas();
  tree->Draw(Form("ELab:ThetaLab>>hEX%i(180,0,180,400,0,10)",7),"ELab>0 && DetectorNumber == 7  && GoodEvent","colz");
  line1->Draw("c");
 new TCanvas();
  tree->Draw(Form("ELab:ThetaLab>>hEX%i(180,0,180,400,0,10)",8),"ELab>0 && DetectorNumber == 8 && GoodEvent","colz");
  line1->Draw("c");
}


