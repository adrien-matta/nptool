#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>

using namespace std;

#include "NPEnergyLoss.h"
#include "TGraph.h"
#include "TSpline.h"
#include "TAxis.h"

//	NPL
using namespace NPL;

//ClassImp(EnergyLoss)

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
EnergyLoss::EnergyLoss() 
	{fInter = NULL	;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
EnergyLoss::~EnergyLoss() 
	{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
EnergyLoss::EnergyLoss(string Path , int NumberOfSlice=100 , int LiseColumn=0 , int NumberOfMass=1) 
	{ 
	
	fNumberOfSlice = NumberOfSlice ; 
	fNumberOfMass  = NumberOfMass  ;
	
	string globalPath = getenv("NPTOOL");
	Path = globalPath + "/Inputs/dEdX/" + Path;
	
	cout << "///////////////////////////////// " << endl ;
	cout << "Initialising an EnergyLoss object " << endl ;
	
	 //If LiseColumn is set to 0 File type is expected to be from SRIM
	 if (LiseColumn == 0)
	 	{
			// Opening dE/dX file

		ifstream TableFile	;
		TableFile.open(Path.c_str())	;

		if ( !TableFile )
		    {
				cout << "Failed to open file " << Path << endl;
				return;
		    }	
		 
		else
		   	{
				// Reading Data
				double energy, nuclear, electronic;
				string unit, dummy;
				
				while ( TableFile >> energy >> unit 
						>> electronic >> nuclear 
						>> dummy >> dummy >> dummy 
				    	>> dummy >> dummy >> dummy )
				     {
						if ( unit == "keV" ) energy = energy*keV				;
						if ( unit == "MeV" ) energy = energy*MeV				;
						if ( unit == "GeV" ) energy = energy*GeV				;
						fEnergy				.push_back ( energy )				;
						fdEdX_Nuclear		.push_back ( nuclear ) 				;
						fdEdX_Electronic	.push_back ( electronic ) 			;
						fdEdX_Total			.push_back ( nuclear + electronic )	;
					}
		   
		  		// Close File
		   		TableFile.close();
		   	}
			
		}
		
		//Else File is expected to be from Lise, and LiseColumn gives which model to take
		else
			{
				// Opening dE/dX file

				ifstream TableFile	;
				TableFile.open(Path.c_str())	;

				if ( !TableFile )
				    {
						cout << "Failed to open file " << Path << endl;
						return;
				    }	
				   
				else
				   	{ cout << "Reading Energy Loss File: " << Path << endl ;
						// Reading Data
						double energy=0, energyloss=0;
						string dummy;				
						// skipping comment first line		
						getline(TableFile,dummy);
						
						while ( TableFile >> energy )
							{ 
								for (int k = 0 ; k < 11 ; k++ )
									{
										TableFile >> dummy ;
										if (k+1==LiseColumn) energyloss = atof(dummy.c_str()) ;
									}
								fEnergy.push_back (energy*MeV) ;
								fdEdX_Total.push_back(energyloss*MeV/micrometer);
							}
				   
				  		// Close File
				   		TableFile.close();
				   	}
					
			
			}
			fInter = new Interpolator( fEnergy , fdEdX_Total	)		;
			cout << "///////////////////////////////// " << endl ;
	}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void EnergyLoss::Draw() const
	{
	   /*TGraph *gr = new TGraph(fDim, fEnergy, fDedx_Tot);
	   gr->Draw("A*");
	   gr->GetXaxis()->SetTitle("E (MeV)");
	   gr->GetYaxis()->SetTitle("dE/dx   (MeV / (mg/cm^{2})");
	   gr->Draw("A");
	   
	   // use a cubic spline to smooth the graph
	   TSpline3 *s = new TSpline3("grs",gr)	;
	   s->SetLineColor(kRed)				;
	   s->Draw("same")						;*/
	}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
double EnergyLoss::EvaluateNuclearLoss(double Energy) const
	{
		if (fEnergy.size()==0 || fdEdX_Nuclear.size()==0)
			{
				cout << "No Nuclear table for this Energy loss" ;
				return -1000;			
			}
		
	   Interpolator* s = new Interpolator( fEnergy , fdEdX_Nuclear );   
	   double val = s->Eval(Energy);
	   
	   delete s		;   
	   return val	;
	}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
double EnergyLoss::EvaluateElectronicLoss(double Energy) const
	{
		if (fEnergy.size()==0 || fdEdX_Electronic.size()==0)
			{
				cout << "No Electronic table for this Energy loss" ;
				return -1000;
			}
			
	
	   Interpolator* s = new Interpolator( fEnergy , fdEdX_Electronic ); 
	   double val = s->Eval(Energy);
	   
	   delete s		;   
	   return val	;
	}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
double EnergyLoss::EvaluateTotalLoss(double Energy) const
	{
		if (fEnergy.size()==0 || fdEdX_Total.size()==0)
			{
				cout << "No Total table for this Energy loss" ;
				return -1000;
			}
	
	   Interpolator* s = new Interpolator( fEnergy , fdEdX_Total )	;   
	   double val = s->Eval(Energy)									;
	   
	   delete s		;   
	   return val	;
	}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void EnergyLoss::Print() const
	{
	   cout << "Total Energy Loss : " << endl;
	   int size = fdEdX_Total.size() ;
	   for( int i = 0 ; i < size ; i++)
	   			cout << fEnergy[i]/MeV << " MeV " << fdEdX_Total[i]/MeV*micrometer << " MeV/um " << endl;
	   		
	}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
double EnergyLoss::Slow(	double Energy 			, // Energy of the detected particle
		   					double TargetThickness	, // Target Thickness at 0 degree
		   					double Angle			) // Particle Angle
		   					const
	{
		TargetThickness = TargetThickness / cos(Angle)  					;
		double SliceThickness = TargetThickness / (double)fNumberOfSlice 	;
		
		Interpolator* s = new Interpolator( fEnergy , fdEdX_Total	)		;
		
		double InitialEnergy = Energy										;
		double slow = 0.													;
		   
		for (int i = 0; i < fNumberOfSlice ; i++) 
			{
			    double de = s->Eval(Energy) * SliceThickness;
			    slow 	+= de	;
			    Energy	-= de	;
			    // If ion do not cross the target
			    if (Energy < 0) 	{slow = InitialEnergy; break;}
			}
		   
		delete s		;
		return slow		;
	}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double EnergyLoss::EvaluateInitialEnergy(	double Energy 			, // Energy of the detected particle
		   									double TargetThickness	, // Target Thickness at 0 degree
		   									double Angle			) // Particle Angle
		   									const
	{
	
		//	Lise file are given in MeV/u
		//	For SRIM file fNumberOfMass = 1 whatever is the nucleus, file are given in MeV
		Energy = Energy / (double) fNumberOfMass ;
	
		if (Angle > halfpi) Angle = pi-Angle								;
		TargetThickness = TargetThickness / ( cos(Angle) ) 					;
		
		double SliceThickness = TargetThickness / (double)fNumberOfSlice 	;

		for (int i = 0; i < fNumberOfSlice ; i++) 
			{
			    double de = fInter->Eval(Energy) * SliceThickness	;
			    Energy	+= de/fNumberOfMass							;
			}
			
		return (Energy*fNumberOfMass)	;
	}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
