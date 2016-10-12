

void Results(){
  TFile* File = new TFile("../../Outputs/Analysis/PhysicsTree.root");
  TTree* Tree = (TTree*) File->FindObjectAny("PhysicsTree");

  Tree->Draw("ELab:ThetaLab>>h2(1000,0,180,1000,0,20)","ELab>0","colz");
  new TCanvas();
  Tree->Draw("Ex>>h4(1000)","ELab>0 && Ex>-1000");

}
