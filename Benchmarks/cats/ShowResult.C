TChain* chain=NULL;
TCanvas* c1=NULL;
void LoadChain();

void ShowResult()
{
    LoadChain();
 
    c1 = new TCanvas("Mask CATS E644 experiment","Mask CATS E644 experiment",600,600);
    c1->cd();
    // Y versus X for CATS1 detector
    chain->Draw("PositionY[0]:PositionX[0]>>h(600,-30,30,600,-30,30)","","colz");
}


////////////////////////////////////////////////////////////////////////////////
void LoadChain(){
    chain = new TChain("PhysicsTree");
//    chain->Add("../../Outputs/Analysis/Analysed_test_cats_mask_e644.root");
    chain->Add("../../Outputs/Analysis/PhysicsTree.root");
}
