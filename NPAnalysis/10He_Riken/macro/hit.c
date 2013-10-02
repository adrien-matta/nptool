{
	gROOT->Reset();
	gStyle->SetOptStat(1);
	gStyle->SetPalette(51,0);
	///////////////////////
	///////////////////////	
TFile *file0 = TFile::Open("./Result/myResult.root");
		
	cEff = new TCanvas("cEff","Theta Distribution" ,100,100,600,600);
	hTheta->Draw();
	
	
	cCM = new TCanvas("cCm" , "ThetaCM" , 100 , 100 , 600, 600) ;
	hThetaCM->Draw();
	
	cHit = new TCanvas("cHit","Hit" ,100,100,600,600);
	hXY->Draw("COLZ");
	
	
	cHit2 = new TCanvas("cHit2","Hit" ,100,100,600,600);
	cHit2->Divide(3,2);
	
	cHit2->cd(1);
	hXY1->Draw("COLZ");
	
	cHit2->cd(2);
	hXY2->Draw("COLZ");
	
	cHit2->cd(3);
	hXY3->Draw("COLZ");
	
	cHit2->cd(4);
	hXY4->Draw("COLZ");
	
	cHit2->cd(5);
	hXY5->Draw("COLZ");
	
	cHit2->cd(6);
	hXY6->Draw("COLZ");
	
	cEx = new TCanvas("cEx","Excitation Energy" ,100,100,300,300);
	hEx->Draw();
	
}
