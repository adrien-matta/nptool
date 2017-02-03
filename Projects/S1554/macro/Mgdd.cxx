void Mgdd(){

  TFile* file = new TFile("../../Outputs/Analysis/S1554_28Mgdd_Phy.root");
  TTree* tree = (TTree*) file->FindObjectAny("PhysicsTree");
  tree->SetAlias("GoodTrifoil","Trifoil.Time>150 && Trifoil.Time<160  && ELab >0") ;
  NPL::Reaction r("28Mg(d,d)28Mg@221.6");
  NPL::Reaction s("28Si(d,d)28Si@221.6");

  // Load Cuts
  fileCut = new TFile("cuts/Cut_Kine_dd.root");
  TCutG* Cut_Kine_dd = (TCutG*) fileCut->FindObjectAny("Cut_Kine_dd");

  // Create Canvas
  TCanvas* c = new TCanvas("Result","Result",1200,1000);
  c->Divide(1,2);
  TH1* h;
  TH1F* h2;

  // Setting Trifoil entry list for fast drawing
  TFile* fileEL = new TFile("macro/GoodTrifoil.root");
  TEntryList* EL = (TEntryList*) fileEL->FindObjectAny("TrifoilEL");
  if(!EL){
    cout << "Trifoil Entry List does not exist, generating : ";
    tree->Draw(">>TrifoilEL","GoodTrifoil","entrylist"); 
    EL = (TEntryList*) gDirectory->FindObjectAny("TrifoilEL");
    cout << " done" << endl;
    EL->SaveAs("macro/GoodTrifoil.root"); 
  }
  cout <<"Setting Trifoil entry list :" ;
  tree->SetEntryList(EL);
  cout << " Done" << endl;

  c->cd(1);
  tree->Draw("ELab:ThetaLab>>hEXT2(1000,0,180,1200,0,60)","ELab>0","colz");

  r.GetKinematicLine3()->Draw("c");
  NPL::Reaction p("28Mg(p,p)28Mg@221.6");
  p.GetKinematicLine3()->Draw("c");
  Cut_Kine_dd->Draw("same");

  gPad->SetLogz();

  h2 = (TH1F*) gDirectory->FindObjectAny("hEXT2");
  h2->GetXaxis()->SetTitle("#theta_{Lab} (deg)");
  h2->GetYaxis()->SetTitle("E_{Lab} (MeV)");
  
  TVirtualPad* pad = c->cd(2);
  pad->Divide(3);

  pad->cd(2);
  tree->Draw("Y_Trifoil:X_Trifoil>>htr(200,-50,50,200,-50,50)","ELab>0 && Cut_Kine_dd","colz") ;   
  pad->cd(1);
  tree->Draw("ELab:ThetaLab>>hEXTA(360,0,180,1200,0,60)","ELab>0 && Sharc.DetectorNumber==12","colz") ;   
  r.SetExcitationHeavy(0);
  r.GetKinematicLine3()->Draw("c");   
  pad->cd(3);
  tree->Draw("ELab:ThetaLab>>hEXTB(360,0,180,1200,0,60)","ELab>0 && Sharc.DetectorNumber==10","colz") ;   
  r.GetKinematicLine3()->Draw("c");  
 }
