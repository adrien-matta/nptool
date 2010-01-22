{

	gROOT->Reset();
	gStyle->SetOptStat(1);
	//gStyle->SetPalette(51,0);
	gStyle->SetPalette(1);
	///////////////////////
	///////////////////////	
TFile *file0 = TFile::Open("./Result/myResult.root");

/*   TCanvas* cTOF = new TCanvas("cTOF","Time of Flight" ,10,10,800,800);
	cTOF->Divide(3,2);
	
	cTOF->cd(1);
	hTOF1->Draw("COLZ"); 
	
	cTOF->cd(2);
	hTOF2->Draw("COLZ"); 
	
	cTOF->cd(3);
	hTOF3->Draw("COLZ"); 
	
	cTOF->cd(4);
	hTOF4->Draw("COLZ"); 
	
	cTOF->cd(5);
	hTOF5->Draw("COLZ"); */
	
	double Pi = 3.141592654 	;
	double DegToRad = Pi/180.   ; // 2Pi/360 = Pi/180

/*	vector<double> relative_error;
	double NumberOfHitX=0;
	cTheta2D = new TCanvas("cTheta2d","Cross Section" ,100,100,900,900);
	hThetaLabCM->Draw("COLZ");*/
	cTheta = new TCanvas("cTheta","Cross Section" ,100,100,900,900);
	hTheta->Draw();
/*	ifstream efficFile;
	
	efficFile.open("./efficiency/20cm2.efficiency");
	string EffString, ThetaString;
	double xxx = hThetaLabCM->GetNbinsY();
	
	for(Int_t i = 1 ; i < hThetaLabCM->GetNbinsX() ; i++)
		{
			// Calculate Relative error	
			NumberOfHitX=hThetaLabCM->Integral(i, i, 1, xxx );			
			if(NumberOfHitX!=0)relative_error.push_back( ( sqrt(NumberOfHitX) ) / (NumberOfHitX) );
			else {relative_error.push_back(0) ;}
		}
		
	for(Int_t j = 1 ; j < hThetaLabCM->GetNbinsY() ; j++)
		{
			//Efficacity correction
			efficFile >> ThetaString >> EffString;
			double Eff = atoi(EffString.c_str());
			for(Int_t i = 1 ; i < hThetaLabCM->GetNbinsX() ; i++)
				{
				if (Eff!=0)
					{
					double pipo =  (hThetaLabCM->GetCellContent(i,j))/(Eff/100) ;
					hThetaLabCM->SetCellContent( i, j, pipo );
					}
				else hThetaLabCM->SetCellContent( i, j, 0 );
				}
		}
		
		
		TH1D* hCrossSection = new TH1D();
		hCrossSection = hThetaLabCM->ProjectionX("Cross Section", 0, -1, "")	;
		
		
		for(Int_t i = 1 ; i < hCrossSection->GetNbinsX() ; i++)
			{
				// Calculate Error		
				NumberOfHitX=hCrossSection->GetBinContent(i);			
				double error=NumberOfHitX*relative_error[i];
				hCrossSection->SetBinError(i,error);
			}
	
	hCrossSection->Draw();
	
/*	ofstream efficFile;
	efficFile.open("20cm.efficiency");

	for(Int_t i = 1 ; i < hTheta->GetNbinsX() ; i++)
		{
		
		Double_t Flux = 100000*sin(hTheta->GetBinCenter(i)*DegToRad)*2*DegToRad/2; 
		
			if(Flux!=0)
			{	
			Double_t Efficiency = ( ( Flux - hTheta->GetBinContent(i) ) / Flux ) * 100 ;
			
			hTheta->SetBinContent(i, 100-Efficiency ) ;
			
	        efficFile << hTheta->GetBinCenter(i) << " " << 100-Efficiency << endl;
			}
			
			else hTheta->SetBinContent(i, 0 );
		}
	
	
		//hTheta->Draw();*/

	//efficiency correction and error bar calculation
/*	ifstream efficFile;
	efficFile.open("./efficiency/20cmCM.efficiency");
	string EffString, ThetaString;
	double Eff, relative_error, error, toto ;

	for(Int_t i = 1 ; i < hThetaCM->GetNbinsX() ; i++)
		{
		
			efficFile >> ThetaString >> EffString;	
			Eff = atoi(EffString.c_str());
			if(hThetaCM->GetBinContent(i)!=0)relative_error = sqrt(hThetaCM->GetBinContent(i))/(hThetaCM->GetBinContent(i));
						
			toto=hThetaCM->GetBinContent(i)*(100-Eff)	;
			hThetaCM->SetBinContent(i, toto) ;
			
			error=hThetaCM->GetBinContent(i)*relative_error;
	        hThetaCM->SetBinError(i,error);
		}*/
	

	
	
	//hThetaCM->Draw();

/*	string Path="/home/Adrien/Desktop/geant/Simulation/Data/CrossSection/11Li(d,3He)10He.txt";
	ifstream CrossSectionFile;
	CrossSectionFile.open(Path.c_str());
	if( CrossSectionFile.is_open() ) 
		cout << " file " << Path << " loading " << endl;
	else{
		cout << " Error, no file " << Path << " found" << endl; return;}
	
	
	//Normalisation:
	Int_t Maximum_Bin 		= hCrossSection->GetMaximumBin()			;
	Double_t Maximum_theta 	= hCrossSection->GetBinCenter(Maximum_Bin)	;
	Double_t Bin_Width		= hCrossSection->GetBinWidth(Maximum_Bin)	;
	Double_t Maximum  		= hCrossSection->GetBinContent(Maximum_Bin)	;
	
	Double_t Normalisation_Factor=0;
	
	
	
	Double_t ThetaVal=0	;
	Double_t CSVal=0	;
	TMarker marker		;
	string theta,CS		;
	
	while( !CrossSectionFile.eof() &&  ThetaVal<60)	
	{
	CrossSectionFile >> theta >> CS ;
	
	ThetaVal=atof(theta.c_str())	;
	
	if(ThetaVal>(Maximum_theta-Bin_Width/4) && ThetaVal<(Maximum_theta+Bin_Width/4) ) 
		{
		CSVal=atof(CS.c_str())	;
		Normalisation_Factor =	Maximum/CSVal;
		} 	
	}
	CrossSectionFile.close();
	CrossSectionFile.open(Path.c_str());

	//Reading Cross Section
	
	ThetaVal=0;
	while( !CrossSectionFile.eof() &&  ThetaVal<60)	
	{
	
	CrossSectionFile >> theta >> CS ;
	CSVal=atof(CS.c_str())			;
	ThetaVal=atof(theta.c_str())	; 	
	marker->DrawMarker(ThetaVal,CSVal*Normalisation_Factor)		;
	}
	
	
	
/*	
	//Normalisation:
	Double_t Maximum_Bin 			= hTheta->GetMaximumBin()			 ;
	Double_t Maximum				= hTheta->GetBinContent(Maximum_Bin) ;
		
	Double_t Normalisation_Factor	= Maximum/3.;
	
	
	
	//Reading Cross Section
	Double_t ThetaVal=0	;
	Double_t CSVal=0	;
	TMarker marker		;
	string theta,CS		;
	
	while( !CrossSectionFile.eof() &&  ThetaVal<60)	
	{
	
	CrossSectionFile >> theta >> CS ;
	CSVal=atof(CS.c_str())			;
	ThetaVal=atof(theta.c_str())	; 
	
	ThetaVal=0.000479597*pow(ThetaVal,3)-0.0670771*pow(ThetaVal,2)+3.08149*ThetaVal-0.339958 ;
	
	marker->DrawMarker(ThetaVal,CSVal*Normalisation_Factor)		;
	}
//	cTheta->SetLogy(1);
	
	
	
	
	/*
	cKine = new TCanvas("cKine","Kinematics lines" ,10,10,800,800);
	hKine->Draw("COLZ");*/
	
/*	cKineInit = new TCanvas("cKineInit","Initial Kinematics lines" ,100,100,600,600);
	hKineInit->Draw("COLZ");
	
	cEDE = new TCanvas("EDE","EDE add Strip" ,100,100,600,600);
	hEDEAddStrip->Draw("COLZ");
/*	cG = new TCanvas("cG","Strip position",500,100,800,600);
	cG->Divide(3,2);

	cG->cd(1);
	Agraph2D->Draw("P0")	;
	cG->cd(2);
	Bgraph2D->Draw("P0")	;
	cG->cd(3);
	Cgraph2D->Draw("P0")	;
	cG->cd(4);
	Dgraph2D->Draw("P0")	;
	cG->cd(5);
	Egraph2D->Draw("P0")	;
	
	cG2 = new TCanvas("cG2","all Strip position",500,100,800,600);
	TOTgraph2D->Draw("P0")	;
	*/
	cH = new TCanvas("cH","Hit density",500,100,1000,800);
	cH->Divide(3,2);
	
	cH->cd(1);
	hHIT4->Draw("COLZ");
	cH->cd(2);
	hHIT2->Draw("COLZ");
	cH->cd(3);
	hHIT5->Draw("COLZ");
	cH->cd(4);
	hHIT1->Draw("COLZ");
	cH->cd(5);
	hHIT3->Draw("COLZ");
    cH->cd(6);
    hThetaHeavy->Draw();
    
/*	cH->cd(5);
	hXZ->Draw("COLZ");
	cH->cd(6);
	hXY->Draw("COLZ");
	
	cEDE = new TCanvas("cEDE","EDE indentification Spectra",500,100,1000,800);
	cEDE->Divide(3,2);
	
	cEDE->cd(1);
	hEDE1->Draw("COLZ");
	cEDE->cd(2);
	hEDE2->Draw("COLZ");
	cEDE->cd(3);
	hEDE3->Draw("COLZ");
	cEDE->cd(4);
	hEDE4->Draw("COLZ");
	cEDE->cd(5);
	hEDE5->Draw("COLZ");
	cEDE->cd(6);
	TH2F* hEDET= new TH2F("hEDET","",4000,-1,600, 400, -1, 25)	; 
	hEDET->Add(hEDE1);
	hEDET->Add(hEDE2);
	hEDET->Add(hEDE3);
	hEDET->Add(hEDE4);
	hEDET->Draw("CLOZ");*/
	
	cEx = new TCanvas("cEx","Excitation Energy" ,100,100,600,600);
	hEx->Draw();
	/*cE = new TCanvas("cE","Light Energy" ,500,100,1000,800);
	cE->Divide(2);
	cE->cd(1);
	hE1234->Draw();
	cE->cd(2);
	hE5->Draw();*/
}
