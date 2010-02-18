{
	gROOT->Reset();
	gStyle->SetOptStat(1);
	gStyle->SetPalette(51,0);
	///////////////////////
	///////////////////////	
	
		double Pi = 3.141592654 	;

///Usefull myAngleInRad * RadToDeg = myAngle In deg :p
	double DegToRad = Pi/180.   ; // 2Pi/360 = Pi/180
	double RadToDeg = 180./Pi   ; // 360/2Pi = 180/Pi
	
TFile *file0 = TFile::Open("./Result/thetaCM.root");

	cCM = new TCanvas("cCm" , "Cross Section (CM)" , 100 , 100 , 900, 900) ;
	hThetaCM->Draw();
	
	vector<double> relative_error;
	double NumberOfHitX=0;
	ifstream efficFile;
	efficFile.open("./17cm.efficiency");
	string EffString, ThetaString;
	double xxx = hThetaLabCM->GetNbinsY();
	
	for(Int_t i = 1 ; i < hThetaLabCM->GetNbinsX() ; i++)
		{
			// Calculate Relative error	
			NumberOfHitX=hThetaLabCM->Integral(i, i, 1, xxx );			
			if(NumberOfHitX!=0)relative_error.push_back( 1. / ( sqrt(NumberOfHitX) ) );
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
					double pipo = (hThetaLabCM->GetCellContent(i,j))/(Eff) ;
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
				hCrossSection->SetBinContent(i, 
				NumberOfHitX=hCrossSection->GetBinContent(i)/(sin(hCrossSection->GetBinCenter(i)*DegToRad) ));
				double error=NumberOfHitX*relative_error[i];
				hCrossSection->SetBinError(i,error);
			}
	
	hCrossSection->Draw();
	
	string Path="../Data/CrossSection/11Li(d,3He)10He.txt";
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
	
	Double_t Normalisation_Factor = 0	;
	Double_t ThetaVal = 0				;
	Double_t CSVal = 0					;
	TMarker marker						;
	string theta,CS						;
	
	while( !CrossSectionFile.eof() &&  ThetaVal<60)	
	{
	CrossSectionFile >> theta >> CS ;
	
	ThetaVal=atof(theta.c_str())	;
	
	if(ThetaVal>(Maximum_theta-Bin_Width/1000) && ThetaVal<(Maximum_theta+Bin_Width/1000) ) 
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
	marker->DrawMarker(ThetaVal,CSVal*Normalisation_Factor	)	;
	}
	
}
