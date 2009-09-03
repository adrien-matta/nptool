{

	gROOT->Reset();
	gStyle->SetOptStat(1);
	gStyle->SetPalette(51,0);
	//gStyle->SetPalette(1);
	///////////////////////
	///////////////////////	
TFile *file0 = TFile::Open("./Result/myResult.root");

   TCanvas* cTOF = new TCanvas("cTOF","Time of Flight" ,10,10,800,800);
	hTOF1234->Draw("COLZ"); 
	
}
