void DrawEfficiencyMugast(){
  PhysicsTree->Draw("acos(PosZ/sqrt(PosX*PosX+PosY*PosY+PosZ*PosZ))*180./3.141592>>h(180,0,180)","",""); 
  TH1F* h = new TH1F();
  gDirectory->GetObject("h",h);
TF1 *fa2 = new TF1("fa2","sin(x*TMath::Pi()/180.)",0,180);
h->Divide(fa2,1);
}

void DrawImpactMugast_XY(){
  PhysicsTree->Draw("Mugast.PosY:Mugast.PosX","","colz"); 
}
void DrawImpactMugast_XY(int tel){
  PhysicsTree->Draw("Mugast.PosY:Mugast.PosX",Form("Mugast.TelescopeNumber==%d",tel),"colz"); 
}
void DrawImpactMugast_YZ(){
  PhysicsTree->Draw("Mugast.PosY:Mugast.PosZ","",""); 
}
void DrawImpactMugast_XZ(){
  PhysicsTree->Draw("Mugast.PosX:Mugast.PosZ","",""); 
}
void DrawComparisonThetaMG_RealTheta(){
  // Theta rec - Mugast alone
  PhysicsTree->Draw("acos(PosZ/sqrt(PosX*PosX+PosY*PosY+PosZ*PosZ))*180./3.141592>>h(180,0,180)","",""); 
  // Theta rec - combine MG,M2 and CATS
  PhysicsTree->Draw("ThetaLab>>h2(180,0,180)","","same"); 
  TH1F* h2 = new TH1F();
  gDirectory->GetObject("h2",h2);
  h2->SetLineColor(kRed);

}

void DrawPhiAnnular(){
  //PhysicsTree->Draw("atan(Mugast.PosY/Mugast.PosX)*180./3.141592>>h(180,0,180)","",""); 
  PhysicsTree->Draw("Mugast.PosY/Mugast.PosX","Mugast.TelescopeNumber==11",""); 
}
