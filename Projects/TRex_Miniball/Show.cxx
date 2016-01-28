void Show(){

TFile* file = new TFile("../../Outputs/Analysis/PhysicsTree.root","READ");
TTree* PhysicsTree = (TTree*) file->FindObjectAny("PhysicsTree");
NPL::Reaction r("17N(d,p)18N@93");


PhysicsTree->Draw("Ex:EDC>>h1(1000,0,2.5,1000,-1,2.5)","Ex>-1000","colz");
new TCanvas();
 PhysicsTree->Draw("ELab:ThetaLab>>h2","Ex>-1000","colz");
r.GetKinematicLine3()->Draw("c");

new TCanvas();
PhysicsTree->Draw("Ex>>h3(1000,-10,10)","Ex>-1000","colz");

new TCanvas();
PhysicsTree->Draw("EDC>>h3(2500,0,2.5)","Ex>-1000","colz");


}
