{
	gROOT->Reset();
	gStyle->SetOptStat(1);
	gStyle->SetPalette(51,0);
	///////////////////////
	///////////////////////	
TFile *file0 = TFile::Open("./Result/myResult.root");
	
	double Pi = 3.141592654 	;
	double DegToRad = Pi/180.   ; // 2Pi/360 = Pi/180

	
	ofstream efficFile;
	efficFile.open("12cm.efficiency");

	for(Int_t i = 1 ; i < hTheta->GetNbinsX() ; i++)
		{	
		Double_t Flux = 1000000./2. * sin(hTheta->GetBinCenter(i)*DegToRad) * (2*DegToRad) ;  
		
			if(Flux!=0)
			{	
			Double_t Efficiency = 100*( (hTheta->GetBinContent(i)) / Flux );
			hTheta->SetBinContent(i, Efficiency ) ;
			}
			
			else hTheta->SetBinContent(i, 0 );
			
			efficFile << hTheta->GetBinCenter(i) << " " << Efficiency << endl;
		}
		
	cEff = new TCanvas("cEff","Efficiency" ,100,100,600,600);
	hTheta->Draw();
	
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
	
}
