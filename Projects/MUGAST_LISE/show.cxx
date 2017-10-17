void show(){

  TFile* file = new TFile("../../Outputs/Analysis/PhysicsTree.root","READ");
  TTree* tree = (TTree*) file->FindObjectAny("PhysicsTree");
  
  tree->Draw("ELab:ThetaLab>>h(360,0,180,360,0,20)","ELab>0","colz");
  NPL::Reaction r;
  r.ReadConfigurationFile("68Nidp.reaction");
  r.GetKinematicLine3()->Draw("c");
  TCanvas*c =new TCanvas();
  c->Divide(2,2);
  c->cd(1);
  tree->Draw("ELab:OriginalELab>>hE(500,0,16,500,0,16)","ELab>0","colz");
  TLine* lineE = new TLine(0,0,16,16);
  lineE->Draw();
  c->cd(2);
  tree->Draw("ThetaLab:OriginalThetaLab>>hT(500,100,180,500,100,180)","ELab>0","colz");
  TLine* lineT = new TLine(100,100,180,180);
  lineT->Draw();
  c->cd(3);
  tree->Draw("Y:X>>hY(300,-150,150,300,-150,150)","ELab>0","colz");
  c->cd(4);
   tree->Draw("Ex>>hEx(300,-10,10)","ELab>0","colz");        

}
