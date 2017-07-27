// Contain global variable declaration, comment and option
#include "EnergyCalibrator.h"

void AutoCalibration(int Telescope_Start, int Telescope_End)
{
	for(int i = Telescope_Start ; i<=Telescope_End ;i++)
	{
		AlThickness = 0.6*micrometer ;
		SiThickness = 0.0*micrometer ;
		double Al_step = 0.01*micrometer;
		int step_limit = 100;
		int k = 0 ;

		Telescope_Number = i ;
		// Create a folder to Hold all the file from calibration
		ostringstream FolderName;
		FolderName << Experiment << "_" << xy << "_T" << Telescope_Number << "_E";
		main_name = FolderName.str() ;
		TString make_folder = "mkdir ./Calibration/" + main_name ;   
		folder = "./Calibration/" + FolderName.str() ;
		system(make_folder);
		system(make_folder+"/peaks");
		system(make_folder+"/dispersion");
		system(make_folder+"/latex");
		system(make_folder+"/latex/pictures");

		// open the ROOT file to process
		TString path  = "./Histograms/";
		TString inFileName = frun;
		inFile = new TFile(path + inFileName +"_RawMust2Histos.root");

		EnergyCalibrator();
		bool check1=false,check2=false;
		while( !(mean_extrapolation <0.1 && mean_extrapolation >-0.1 ) && k < step_limit )
		{
			if(mean_extrapolation < 0)
			{
				if(xy=="X")
					AlThickness -= Al_step;
				else if(xy=="Y")
					AlThickness += Al_step;

				check1=true;
			}

			else if (mean_extrapolation > 0)
			{
				if(xy=="X")
					AlThickness += Al_step;
				else if(xy=="Y")
					AlThickness -= Al_step;

				check2=true;
			}

			if(check1&&check2)
			{
				Al_step=Al_step/10.;
				check1=false;check2=false;
			}
			latex_file.close();
			EnergyCalibrator();

			cout << " Iteration Results: Al Thickness: " << AlThickness/micrometer << "um | Mean Extrapolation  "  << mean_extrapolation << "Chan. "<< endl ;

			k++;
		}

		LatexSummaryEnder();

		delete Buffer;
		delete Source_branching_ratio;
		delete Source_E;
		delete Source_Sig;
		delete energyX;
		delete errorsX;
		delete energyY;
		delete errorsY;

	}

	return;
}


/////////////////////////////
void DefineSource(TString sourceName)
{
	if(sourceName=="3 alphas")
	{
		NumberOfIsotope = 3 ;
		energyX = new Double_t[NumberOfIsotope]; errorsX = new Double_t[NumberOfIsotope];
		energyY = new Double_t[NumberOfIsotope]; errorsY = new Double_t[NumberOfIsotope];

		/// Information used in the summary
		Source_Number_Peak = 8;
		Source_isotope = new TString[Source_Number_Peak] ;Source_E = new Double_t[Source_Number_Peak] ; Source_Sig = new Double_t[Source_Number_Peak] ; Source_branching_ratio = new Double_t[Source_Number_Peak] ;

		// 244Pu
		Source_isotope[0]="$^{239}$Pu"; Source_E[0]   = 5.15659 ; Source_Sig[0] = 0.00014 ; Source_branching_ratio[0] = 70.77 ;
		Source_isotope[1]="$^{239}$Pu"; Source_E[1]   = 5.14438 ; Source_Sig[1] = 0.00014 ; Source_branching_ratio[1] = 17.11 ;
		Source_isotope[2]="$^{239}$Pu"; Source_E[2]   = 5.1055  ; Source_Sig[2] = 0.00014 ; Source_branching_ratio[2] = 11.94 ;

		// 241Am
		Source_isotope[3]="$^{241}$Am"; Source_E[3]   = 5.48556 ; Source_Sig[3] = 0.00012 ; Source_branching_ratio[3] = 84.8 ;
		Source_isotope[4]="$^{241}$Am"; Source_E[4]   = 5.44280 ; Source_Sig[4] = 0.00012 ; Source_branching_ratio[4] = 13.1 ;
		Source_isotope[5]="$^{241}$Am"; Source_E[5]   = 5.388   ; Source_Sig[5] = 0.00012 ; Source_branching_ratio[5] = 1.66 ;

		// 244Cm
		Source_isotope[6]="$^{244}$Cm"; Source_E[6]   = 5.80477 ; Source_Sig[6] = 0.00005 ; Source_branching_ratio[6] = 76.40 ;
		Source_isotope[7]="$^{244}$Cm"; Source_E[7]   = 5.76264 ; Source_Sig[7] = 0.00005 ; Source_branching_ratio[7] = 23.60 ;

		// Corrected value of main peak used in the fit
		Double_t sig_value = 0.1;
		Double_t alpha1_Sig = sig_value ; Double_t alpha2_Sig = sig_value ; Double_t alpha3_Sig = sig_value ; 

		Double_t alpha1_E , alpha2_E , alpha3_E;

		alpha1_E = EL_Al.Slow(	Source_E[0]*MeV , // Energy of the detected particle
				AlThickness	    , // Target Thickness at 0 degree
				0			          ) ;

		alpha1_E = EL_Si.Slow(	alpha1_E*MeV    , // Energy of the detected particle
				SiThickness	    , // Target Thickness at 0 degree
				0			          ) ;

		alpha2_E = EL_Al.Slow(	Source_E[3]*MeV , // Energy of the detected particle
				AlThickness	    , // Target Thickness at 0 degree
				0			          ) ;

		alpha2_E = EL_Si.Slow(	alpha2_E*MeV    , // Energy of the detected particle
				SiThickness	    , // Target Thickness at 0 degree
				0     		      ) ;		

		alpha3_E = EL_Al.Slow(	Source_E[6]*MeV , // Energy of the detected particle
				AlThickness	    , // Target Thickness at 0 degree
				0			          ) ;

		alpha3_E = EL_Si.Slow(	alpha3_E*MeV    , // Energy of the detected particle
				SiThickness   	, // Target Thickness at 0 degree
				0     		      ) ;		                          

		// X and Y are in a reversed order        
		energyX[0] = alpha1_E   ; energyX[1] = alpha2_E   ; energyX[2] = alpha3_E   ;
		errorsX[0] = alpha1_Sig ; errorsX[1] = alpha2_Sig ; errorsX[2] = alpha3_Sig ;

		energyY[0] = alpha3_E   ; energyY[1] = alpha2_E   ; energyY[2] = alpha1_E   ;
		errorsY[0] = alpha3_Sig ; errorsY[1] = alpha2_Sig ; errorsY[2] = alpha1_Sig ;
	}

	return;  
}


/////////////////////////////
void EnergyCalibrator()
{
	// Set-up the root Style
	gStyle->SetOptTitle();
	gStyle->SetOptTitle();

	DefineSource();

	TString str;
	TString str1;
	TString str2;
	TString strbuff;
	TString strbuff2;
	TString fname;
	TString fname2;
	TString fname3;
	TString hname;

	LatexSummaryHeader(xy);

	// Clear everything
	BadStrip.clear() ;
	sigma_fit = new TH1F("Sigma", "Sigma from fit (channel)", 80, 0,10);
	Dispersion= new TH1F("Dispersion", "Dispersion from Zero Extrapolation (channel)", 40, -20,20);
	ZeroDispersion = new TGraph(128);
	coeff_a = new TGraph(128);
	coeff_b = new TGraph(128);

	ostringstream number ;
	number << Telescope_Number  ;
	CurrentTelescope = Telescope_Number ;
	if (xy == "X"){ 
		//////// Input Files ///////////
		str = "hMM"+number.str()+"_STRX_E";
		//freddy files (different naming)
		//str = "hMM"+number.str()+"_EX";
		/////// Output Files ///////////
		str1 = "Cal_Str_X_E_MM"+number.str();
	}	 
	else if (xy == "Y"){ 	
		//////// Input Files ///////////
		str = "hMM"+number.str()+"_STRY_E";
		//freddy files (different naming)
		//str = "hMM"+number.str()+"_EY";
		/////// Output Files ///////////
		str1 = "Cal_Str_Y_E_MM"+number.str();
	}	 
	else {cout<< "ERROR FOR X or Y PARAMETER"<< endl;}

	fname =  folder + "/peaks/" + str1 + ".peak";
	peaks_file.open( ( (string)fname ).c_str() );

	fname2 = folder + "/" + str1 + ".cal";
	calib_file.open( ( (string)fname2 ).c_str() );

	fname3 = folder + "/" + str1 + ".dispersion";
	dispersion_file.open( ( (string)fname3 ).c_str() );

	Tsummary = new TCanvas((TString)("Telescope"+number.str()+"Summary"), (TString)("Telescope "+number.str()+" Summary"), 700, 700);
	Tsummary->Divide(2,3);
	Buffer  = new TCanvas((TString)("Buffer"), (TString)("Buffer"), 10, 10);
	Buffer->cd(1);

	for (Int_t j = Strip_Start-1; j < Strip_End; j++)
	{
		///// Get the histogram of det i and strip j /////
		CurrentStrip=j+1;
		number.seekp(0);
		number << j+1;
		hname = str+number.str();
		TH1F *hist = (TH1F*) inFile->Get(((string)hname).c_str());

		// Prevent rebinning in Pedestal
		TH1F *histAlpha = (TH1F*)hist->Clone();

		Alpha(histAlpha,
				xy,
				Pedestals(hist));

	//		Tsummary->WaitPrimitive();
		if(xy == "X") 	   histAlpha->GetXaxis()->SetRangeUser(8800,9000);
		else if(xy == "Y") histAlpha->GetXaxis()->SetRangeUser(7300,7500);

		if(j == 127) 
		{ TH1F histAlpha67 = TH1F(*histAlpha);
			Tsummary->cd(2);
			histAlpha67.SetStats(true);
			histAlpha67.SetTitle("Raw Spectrum of strip 128 with gaussian fit");

			histAlpha67.Draw("A");
			Buffer->cd(1);
		}
	}

	Tsummary->cd(1); 
	mean_extrapolation = ZeroDispersion->GetMean(2);
	ZeroDispersion->SetMaximum(mean_extrapolation+30);ZeroDispersion->SetMinimum(mean_extrapolation-30);
	ZeroDispersion->SetTitle("Scattered plot of zero extrapolation dispersion : Ped.+b/a");
	ZeroDispersion->SetMarkerStyle(2);
	ZeroDispersion->Draw("ap");
	//Draw the mean line
	TLine mean_line = TLine(0, ZeroDispersion->GetMean(2), 140, ZeroDispersion->GetMean(2) );
	mean_line.Draw("");

	Tsummary->cd(3); 
	Dispersion->Draw();

	Tsummary->cd(4); 
	sigma_fit->Draw();

	Tsummary->cd(5) ;
	coeff_a->SetMarkerStyle(2);
	coeff_a->SetMaximum(coeff_a->GetMean(2)+0.002);coeff_a->SetMinimum(coeff_a->GetMean(2)-0.002);
	coeff_a->SetTitle("Gain a (MeV/channel)");
	coeff_a->Draw("ap");

	Tsummary->cd(6);
	coeff_b->SetMaximum(coeff_b->GetMean(2)+10);coeff_b->SetMinimum(coeff_b->GetMean(2)-10);
	coeff_b->SetMarkerStyle(2);
	coeff_b->SetTitle("Offset b (MeV)");
	coeff_b->Draw("ap");

	TString filename = Tsummary->GetName();
	Tsummary->SaveAs(filename+".pdf");
	Tsummary->Close();
	system("mv "+filename+".pdf ./" + folder + "/latex/pictures");

	peaks_file.close();
	calib_file.close();
	dispersion_file.close();

	LatexSummaryTelescope();
	delete Tsummary   ;
	delete sigma_fit  ;
	delete Dispersion ;

	Buffer->Close();   
}

/////////////////////////////////
Double_t Pedestals(TH1F *hist)
{

	if(Pedestals_Aligned)
		return 8192 ;

	else
	{
		TF1 *gauss=new TF1("gauss","gaus",0,1024);

		hist->SetAxisRange(7800,8500);

		///// Peak search /////
		TSpectrum *s = new TSpectrum(2,1);
		Int_t nfound =0;
		nfound = s->Search(hist,2," ");

		Float_t *xpeaks = s->GetPositionX();

		Float_t linf =0, lsup =0; 
		Double_t sum=0, mean=0, sigma=0;

		if(nfound != 1 ) 
			cout << "########   PROBLEM Nfound != NAsked !  ########   " << hist->GetName() <<"  Nfound:"<<nfound<<endl;

		else {
			linf = xpeaks[0]-10;
			lsup = xpeaks[0]+10; 
			gauss=new TF1("gauss","gaus",linf,lsup); 
			gauss->SetRange(linf,lsup);
			hist->Fit(gauss,"RQ");

			sum = gauss->GetParameter(0);
			mean = gauss->GetParameter(1);
			sigma = gauss->GetParameter(2);

			if(sigma > 3)
				BadStrip[CurrentStrip] += " Alpha peak to large;" ;

		}

		delete s; delete gauss;
		return (mean) ;
	}


}

/////////////////////////////////
void Alpha(TH1F *hist, TString xy, Double_t Pedestal)
{

	if(xy == "X") 		hist->GetXaxis()->SetRangeUser(8750,9250);
	else if(xy == "Y") 	hist->GetXaxis()->SetRangeUser(7050,7600);

	if(!Finder(hist, xy, mean, sigma )) cout << "On "<< hist->GetName() << endl ;

	// Fit 
	TGraphErrors* gr_MM= new TGraphErrors(4);

	if(method == "ZeroForce")
	{
		a = Calib_ZeroForceMethod((string)xy,gr_MM,Pedestal, mean, sigma);
		b = -Pedestal*a ;
	}

	else if(method == "ZeroExtrapolation")
	{
		Calib_ZeroExtrapolationMethod(hist,(string)xy,gr_MM,Pedestal, mean, sigma,a,b);
	}

}

/////////////////////////////////
bool Finder(TH1F *h, TString xy, Double_t *mean, Double_t *sigma)
{

	/////////////////////////////////////////////////
	//						                                 //
	//	           ALPHA  FINDER		               //
	//						                                 //
	/////////////////////////////////////////////////

	for(int k=0; k<3; k++)
	{
		mean[k]=0;
		sigma[k]=0;
	}

	Double_t resolsig=5;
	Float_t  resolsigTSpec=1;
	Double_t seuil=0.2;
	Int_t npeaks=5;   // maximum number of peak that can be found

	//////// Peak finder

	TSpectrum *s = new TSpectrum(npeaks,resolsigTSpec);

	Int_t nfound = s->Search(h,resolsig,"new",seuil);
	Float_t *xpeaks = s->GetPositionX();

	/// Sort in growing order the array

	if(nfound>1)
	{
		for(Int_t p=0;p<nfound;p++)
		{
			for(Int_t i=0;i<nfound-1;i++)
			{
				if(xpeaks[i]>xpeaks[i+1])
				{
					Float_t varia=xpeaks[i];
					xpeaks[i]=xpeaks[i+1];
					xpeaks[i+1]=varia;
				}	  
			}
		}
	}

	Float_t linf=0, lsup=0; 

	// If 3 peak found
	if(nfound == 3)
	{
		for (Int_t p=0;p<nfound;p++)
		{   
			if(xy == "X")
			{			
				linf = xpeaks[p]-2;
				lsup = xpeaks[p]+8;
			}

			else if (xy == "Y")
			{			
				linf = xpeaks[p]-8;
				lsup = xpeaks[p]+2;
			}

			TF1 *gauss = new TF1("gauss","gaus",linf,lsup);
			h->Fit(gauss,"RQ");
			mean[p] = gauss->GetParameter(1);
			sigma[p]= gauss->GetParameter(2);

			sigma_fit->Fill(gauss->GetParameter(2));
		}
	}

	if(nfound!=3)
	{
		ostringstream numP;
		numP << nfound ;

		BadStrip[CurrentStrip] += " " + numP.str() + " peak(s) found;" ;

		for (Int_t p=0;p<3;p++)
		{
			cout << "attention, nombre de pics different de 3!!!" ;
			mean[p]=-1;
			sigma[p]=-1;
			return false ;
		}
	}

	return true ;
}


/////////////////////////////////
Double_t Calib_ZeroForceMethod(string xy,TGraphErrors *gr,float Pedestal, Double_t *mean, Double_t *sigma)
{  
	Double_t energy[3];
	Double_t errors[3];

	if(xy=="X")
		for(int i = 0 ; i < 3 ; i ++)
		{
			energy[i] = energyX[i];
			errors[i] = errorsX[i];
		}

	if(xy=="Y")
		for(int i = 0 ; i < 3 ; i ++)
		{
			energy[i] = energyY[i];
			errors[i] = errorsY[i];
		}

	gr->SetPoint(0,Pedestal,energy[0]);

	for (Int_t p = 0; p < 3; p++) {
		gr->SetPoint(p, mean[p], energy[p]);
		gr->SetPointError(p, sigma[p], errors[p]);    
	}

	TF1 *f1 = new TF1("f1",Form("[0]*(x-%f)",Pedestal));
	gr->Fit("f1", "Q" );

	a = f1 -> GetParameter(0);

	if (xy == "X")
		calib_file << "MUST2_T" << CurrentTelescope << "_Si_X" << CurrentStrip << "_E " << b << " " << a  << endl ;

	else if (xy == "Y")
		calib_file << "MUST2_T" << CurrentTelescope << "_Si_Y" << CurrentStrip << "_E " << b << " " << a  << endl ;

	delete f1;
	return a ;
}

/////////////////////////////////
Double_t Calib_ZeroExtrapolationMethod(TH1F* hist , string xy,TGraphErrors *gr, float Pedestal, Double_t* mean, Double_t* sigma, Double_t &a , Double_t &b)
{  
	Double_t energy[3];
	Double_t errors[3];

	if(xy=="X")
		for(int i = 0 ; i < 3 ; i ++)
		{
			energy[i] = energyX[i];
			errors[i] = energyX[i];
		}

	if(xy=="Y")
		for(int i = 0 ; i < 3 ; i ++)
		{
			energy[i] = energyY[i];
			errors[i] = energyY[i];
		}

	for (Int_t p = 0; p < 3; p++) {
		gr->SetPoint(p, mean[p], energy[p]);
		gr->SetPointError(p, sigma[p], errors[p]);    
	}

	TF1 *f1 = new TF1("f1","[1]+[0]*x");
	gr->Fit("f1", "Q" );

	a = f1 -> GetParameter(0);
	b = f1 -> GetParameter(1);

	if(RefitWithSatellite)
	{
		Find_Satellites(hist);

		for (Int_t p = 0; p < 3; p++) 
		{
			gr->SetPoint(p, mean[p], energy[p]);
			gr->SetPointError(p, sigma[p], a*sigma[p]);    
		}

		gr->Fit("f1", "Q" );

		a = f1 -> GetParameter(0);
		b = f1 -> GetParameter(1);
	}


	if( (a < 0.009 &&  a > 0.006) || (a > -0.009 &&  a < -0.006) )  
		coeff_a->SetPoint(CurrentStrip,CurrentStrip,a);

	if( (b < -54 && b > -72) || (b > 54 && b < 72) )
		coeff_b->SetPoint(CurrentStrip,CurrentStrip,b);

	// look at the dispersion around Pedestals
	Double_t dispersion = Pedestal + b/a ;
	dispersion_file  << "MUST2_T" << CurrentTelescope << "_Si_X" << CurrentStrip << "_E_Zero_Dispersion " << dispersion << endl ;

	// Condition avoid Mean problem due to a few large value
	if(dispersion<40 && dispersion>-40 )
		ZeroDispersion->SetPoint(CurrentStrip,CurrentStrip,dispersion);

	Dispersion->Fill(dispersion);

	if(dispersion > 30 || dispersion < -30)
	{
		ostringstream disp;
		disp << dispersion ;
		BadStrip[CurrentStrip] += " zero extrapolation too high:" + disp.str() +"channels; ";
	}


	if (xy == "X")
		calib_file << "MUST2_T" << CurrentTelescope << "_Si_X" << CurrentStrip << "_E " << b << " " << a  << endl ;

	else if (xy == "Y")
		calib_file << "MUST2_T" << CurrentTelescope << "_Si_Y" << CurrentStrip << "_E " << b << " " << a  << endl ;

	delete f1;
	return a ;

}

/////////////////////////////////////////
void LatexSummaryHeader(TString xy)
{

	latex_file.open(folder+"/latex/"+main_name+".tex");

	///// Write File Header

	latex_file << "\\documentclass[a4paper,6pt]{article}" << endl ;
	latex_file << "\\usepackage[french]{babel}" << endl ;
	latex_file << "\\usepackage[T1]{fontenc}" << endl ;
	latex_file << "\\usepackage{graphicx}" << endl ;
	latex_file << "\\usepackage{fullpage}" << endl ;
	latex_file << "\\topmargin = 0pt" << endl ;
	latex_file << "\\headsep = 0pt" << endl ;

	// Start Document
	latex_file << "\\begin{document}" << endl ;
	latex_file << "\\title{MUST2 DSSD Energy Calibration Report}" << endl ;
	latex_file << "\\date{}" << endl ;
	latex_file << "\\maketitle" << endl ;

	// Write Report header
	latex_file << "\\section{Calibration Summary}" << endl ;
	latex_file << "\\begin{itemize}" << endl ;
	latex_file << "\t \\item[{\\bf Experiment:}] "<< Experiment << endl ;
	latex_file << "\t \\item[{\\bf Operator:}] "<< Operator << endl ;
	latex_file << "\t \\item[{\\bf App. Date:}] "<< Run_Period << endl ;
	latex_file << "\t \\item[{\\bf Source:}] "<< Source << endl ;
	latex_file << "\t \\item[{\\bf Dead Layer:}] "<< "Al "<< AlThickness/micrometer << "$\\mu$m + Si " << SiThickness/micrometer << "$\\mu$m" << endl ;
	latex_file << "\t \\item[{\\bf Comment:}] "<< Comment << endl ;
	latex_file << "\t \\item[] "<< endl ;
	latex_file << "\t \\item[{\\bf Calibration Method:}] "<< " " << method << " "<< endl ;
	latex_file << "\t \\item[{\\bf Telescope Treated:}] "<<  " " << Telescope_Number << endl ;
	latex_file << "\t \\item[{\\bf Strip Treated:}] "<<  " " << Strip_Start << " to "<< Strip_End << " " << endl ;
	latex_file << "\t \\item[{\\bf DSSD Side:}] "<< " " <<  xy << endl ;

	latex_file << "\\end{itemize}" << endl ;

	latex_file << "\\begin{itemize}" << endl ;
	latex_file << "\t \\item[] "<< endl ;
	latex_file << "\t \\item[] "<< endl ;
	latex_file << "\\end{itemize}" << endl ;

	latex_file << "{\\bf Source Description:} " << endl ;
	latex_file << "\\begin{center}"<<endl ;
	latex_file << "\\begin{tabular}{ | c | c | c | } "<<endl ;
	latex_file << "\\hline "<<endl ;
	latex_file << "Isotope & Original Energy (MeV) & Branching Ratio \\\\ \\hline " << endl ;

	for(int hh = 0 ; hh < Source_Number_Peak ; hh++)
	{
		latex_file << Source_isotope[hh] << " & " << Source_E[hh] << " & " << Source_branching_ratio[hh] << " \\\\ \\hline" << endl;
	}

	latex_file << "\\end{tabular} "<<endl ;
	latex_file << "\\end{center}"<<endl ;

	latex_file <<"\\pagebreak"<<endl ;
}

///
void LatexSummaryEnder()
{
	latex_file << endl <<  "\\end{document}" << endl ;
	latex_file.close();
	// generate the pdf file and clean-up
	system("pdflatex "+folder+"/latex/"+main_name+".tex");
	system("rm -f *.log");
	system("rm -f *.aux");
	system("mv " + main_name+".pdf "+folder  );
}

///
void LatexSummaryTelescope()
{
	/// Write main summary
	latex_file << "\\section{Telescope "<< CurrentTelescope << " }"<<endl ;
	/// List symptomatic strips and reason

	if(BadStrip.size()>0)
	{
		latex_file << " Bad Strip:" << endl ;
		latex_file << "\\begin{center}"<<endl ;
		latex_file << "\\begin{tabular}{ | c | c | } "<<endl ;
		latex_file << "\\hline "<<endl ;
		latex_file << " Strip Number & Problem \\\\ \\hline "<<endl ;
		map<int,string>::iterator it ;
		for(it = BadStrip.begin() ; it!=BadStrip.end() ; it++)
		{
			latex_file << it->first << " & " << it->second <<  " \\\\ \\hline "<<endl ;
		}

		latex_file << "\\end{tabular} "<<endl ;
		latex_file << "\\end{center}"<<endl ;
	}

	else
		latex_file << "Bad Strip : All Strip are ok."<<endl ;

	// Add the Graph
	TString filename = Tsummary->GetName();
	TString path = folder+"/latex/pictures/"+filename+".pdf";

	latex_file <<"\\begin{figure}[htcb!]"<<endl ;
	latex_file <<"\\begin{center}"<<endl ;
	latex_file <<"\\includegraphics[width=0.7\\textwidth]{"+path +"}"<<endl ;
	latex_file <<"\\end{center}"<<endl ;
	latex_file <<"\\end{figure}"<<endl ;

	latex_file <<"\\pagebreak"<<endl ;

	/// add summary graph and image

}


//////// Satellite finder and description of the Peak+Sattelite look-a-like function
void Find_Satellites(TH1F *h)
{

	if(mean[0]==0 && mean[1]==0 && mean[2]==0) { cout << "pas de pics ---> pas de satellites!" << endl;}

	else {

		Float_t linf1 =0 , lsup1 =0, linf2 =0 , lsup2 =0 , linf3 =0 , lsup3=0;

		if(a>0) { // ie Y case
			linf1 = mean[0]-15; lsup1 = mean[0]+10;
			linf2 = mean[1]-15; lsup2 = mean[1]+10;
			linf3 = mean[2]-15; lsup3 = mean[2]+10;
		}

		else { // ie X case 
			lsup1 = mean[0]+15; linf1 = mean[0]-10;
			lsup2 = mean[1]+15; linf2 = mean[1]-10;
			lsup3 = mean[2]+15; linf3 = mean[2]-10;
		}

		Double_t keVtoMeV = 1./1000. ;

		TF1 *Pu = new TF1("fit_sat_Pu", source_Pu, linf1, lsup1, 6);
		Pu->SetParameters(150,mean[0],mean[0]-12.4*keVtoMeV/a,mean[0]-51.6*keVtoMeV/a,sigma[0]);
		Pu->SetParLimits(2,mean[0]-12.4*keVtoMeV/a-10,mean[0]-12.6*keVtoMeV/a+10);
		Pu->SetParLimits(3,mean[0]-51.6*keVtoMeV/a-10,mean[0]-51.6*keVtoMeV/a+10);
		Pu->SetParNames("Constant","Mean_value1","Mean_value2","Mean_value3","SigmaPu");
		h->Fit("fit_sat_Pu", "RQ");

		TF1 *Am = new TF1("fit_sat_Am", source_Am, linf2, lsup2, 6);
		Am->SetParameters(150,mean[1],mean[1]-43.2*keVtoMeV/a,mean[1]-98.4*keVtoMeV/a,sigma[1]);
		Am->SetParLimits(2,mean[1]-43.2*keVtoMeV/a-10,mean[1]-43.2*keVtoMeV/a+10);
		Am->SetParLimits(3,mean[1]-98.4*keVtoMeV/a-10,mean[1]-98.4*keVtoMeV/a+10);
		Am->SetParNames("Constant","Mean_value1","Mean_value2","Mean_value3","SigmaAm");
		h->Fit("fit_sat_Am", "RQ+");


		TF1 *Cm = new TF1("fit_sat_Cm", source_Cm, linf3, lsup3, 6);
		Cm->SetParameters(150,mean[2],mean[2]-43.1*keVtoMeV/a,sigma[2]);
		Cm->SetParLimits(2,mean[2]-43.1*keVtoMeV/a-10,mean[0]-43.1*keVtoMeV/a-10);
		Cm->SetParNames("Constant","Mean_value1","Mean_value2","SigmaCm");
		h->Fit("fit_sat_Cm", "RQ+");

		mean[0]=Pu->GetParameter(1);  // Position of the 1st principal peak
		sigma[0]=Pu->GetParameter(4); // Sigma of the 1st principal peak
		sigma_fit->Fill(sigma[0]) ;
		error_par[0]= Pu->GetParError(1);
		mean[1]=Am->GetParameter(1);
		sigma[1]=Am->GetParameter(4);
		sigma_fit->Fill(sigma[1]) ;
		error_par[1]= Am->GetParError(1);
		mean[2]=Cm->GetParameter(1);
		sigma[2]=Cm->GetParameter(3);
		sigma_fit->Fill(sigma[2]) ;
		error_par[2]= Cm->GetParError(1);
	}

}

///////////////////////////////////////////////
Double_t source_Pu(Double_t *x, Double_t *par)
{
	// [0] : constant
	// [1] : position peak1
	// [2] : position peak2
	// [3] : position peak3
	// [4] : sigma

	Double_t arg1 = 0;
	Double_t arg2 = 0;
	Double_t arg3 = 0;

	if(par[4]!=0) { 
		arg1 = (x[0]-par[1])/par[4];
		arg2 = (x[0]-par[2])/par[4];
		arg3 = (x[0]-par[3])/par[4];
	}

	else cout << " Attention, sigma est nul !" << endl;

	Double_t gaus1 =           par[0]*exp(-0.5*arg1*arg1);
	Double_t gaus2 = 15.1/73.8*par[0]*exp(-0.5*arg2*arg2);
	Double_t gaus3 = 11.5/73.8*par[0]*exp(-0.5*arg3*arg3);
	Double_t fitval = gaus1+gaus2+gaus3;

	return fitval;
}

///////////////////////////////////////////////
Double_t source_Am(Double_t *x, Double_t *par)
{
	// [0] : constant
	// [1] : position peak1
	// [2] : position peak2
	// [3] : position peak3
	// [4] : sigma

	Double_t arg1 = 0;
	Double_t arg2 = 0;
	Double_t arg3 = 0;

	if(par[4]!=0) { 
		arg1 = (x[0]-par[1])/par[4];
		arg2 = (x[0]-par[2])/par[4];
		arg3 = (x[0]-par[3])/par[4];
	}

	else cout << " Attention, sigma est nul !" << endl;

	Double_t gaus1 =           par[0]*exp(-0.5*arg1*arg1);
	Double_t gaus2 = 13.0/84.5*par[0]*exp(-0.5*arg2*arg2);
	Double_t gaus3 = 1.6/84.5 *par[0]*exp(-0.5*arg3*arg3);
	Double_t fitval= gaus1+gaus2+gaus3;

	return fitval;
}

///////////////////////////////////////////////
Double_t source_Cm(Double_t *x, Double_t *par)
{
	// [0] : constante
	// [1] : position peak1
	// [2] : position peak2
	// [3] : sigma

	Double_t arg1 = 0;
	Double_t arg2 = 0;

	if(par[3]!=0) { 
		arg1 = (x[0]-par[1])/par[3];
		arg2 = (x[0]-par[2])/par[3];
	}

	else cout << " Attention, sigma est nul !" << endl;

	Double_t gaus1 =           par[0]*exp(-0.5*arg1*arg1);
	Double_t gaus2 = 23.6/76.4*par[0]*exp(-0.5*arg2*arg2);
	Double_t fitval= gaus1+gaus2; 

	return fitval;
}  
