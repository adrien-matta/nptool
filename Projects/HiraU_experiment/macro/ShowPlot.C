TChain *chain1;

//chain1=NULL;

void LoadChain(){
	chain1 = new TChain("PhysicsTree");
	//chain1->Add("/Users/pierremorfouace/Physics/NPTool/nptool/Outputs/Analysis/hira_upgrade.root");
    //chain1->Add("/Users/pierremorfouace/Physics/NPTool/nptool/Outputs/Analysis/hiraU_cal6.root");
    chain1->Add("/Users/pierremorfouace/Physics/NPTool/nptool.new/Outputs/Analysis/dumb.root");
}

void ShowPlot()
{
    gROOT->SetStyle("pierre_style");
	LoadChain();

	chain1->Draw("PhiLabMB:ThetaLabMB>>hMB(180,0,180,360,-180,180)","","");
	TH2F* hMB = (TH2F*) gDirectory->FindObjectAny("hMB");
    hMB->GetXaxis()->SetTitle("#theta_{lab} (deg)");
    hMB->GetYaxis()->SetTitle("#phi_{lab} (deg)");
    hMB->GetXaxis()->CenterTitle();
    hMB->GetYaxis()->CenterTitle();
    hMB->GetXaxis()->SetRangeUser(0,90);
    hMB->GetYaxis()->SetRangeUser(-200,200);
    hMB->SetMarkerColor(2);
    hMB->SetMarkerSize(0.1);

	chain1->Draw("PhiLabHira:ThetaLabHira>>hHira(180,0,180,360,-180,180)","","");
	TH2F* hHira = (TH2F*) gDirectory->FindObjectAny("hHira");
    hHira->GetXaxis()->SetTitle("#theta_{lab} (deg)");
    hHira->GetYaxis()->SetTitle("#phi_{lab} (deg)");
    hHira->GetXaxis()->CenterTitle();
    hHira->GetYaxis()->CenterTitle();
	hHira->SetMarkerColor(4);
    hHira->SetMarkerSize(0.1);
    
    /*chain1->Draw("PhiLabVW:ThetaLabVW>>hVW(180,0,180,360,-180,180)","","");
    TH2F* hVW = (TH2F*) gDirectory->FindObjectAny("hVW");
    hVW->SetMarkerColor(9);
    hVW->SetMarkerSize(0.2);*/

    chain1->Draw("PhiLabFA:ThetaLabFA>>hFA(180,0,180,360,-180,180)","","");
    TH2F* hFA = (TH2F*) gDirectory->FindObjectAny("hFA");
    hFA->SetMarkerColor(8);
    hFA->SetMarkerSize(0.1);

    TCanvas *c1 = new TCanvas("c1","c1",600,600);
    c1->cd();
	hMB->Draw();
	hHira->Draw("same");
    hFA->Draw("same");
    //hVW->Draw("same");
    
    /*TCanvas *c2 = new TCanvas("c2","c2",600,600);
    c2->cd();
    chain1->Draw("Y:X>>hXY(400,-200,200,500,-300,200)","","");
    TH2F* hXY = (TH2F*) gDirectory->FindObjectAny("hXY");
    hXY->SetMarkerColor(4);
    hXY->SetMarkerSize(0.1);
    hXY->GetXaxis()->SetTitle("X (mm)");
    hXY->GetYaxis()->SetTitle("Y (mm)");
    hXY->GetXaxis()->CenterTitle();
    hXY->GetYaxis()->CenterTitle();
    hXY->Draw();*/
    

}
