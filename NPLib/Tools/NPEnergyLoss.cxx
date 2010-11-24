/*****************************************************************************
 * Copyright (C) 2009 	this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  Adrien MATTA    contact address: matta@ipno.in2p3.fr   *
 *                                                                           *
 * Creation Date   : April 2009                                              *
 * Last update     :                                                         *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  Deal with energy loss on basis of a dEdX input file, from SRIM or LISE++ *
 *   The class can be used to evaluate energy loss in material or to Evaluate* 
 *   initial energy before crossing the material.                            *
 *                                                                           *
 *   The use of Interpolator derived form the GSL insure a very good speed of*
 *   execution.                                                              *
 *                                                                           *
 *   Table Should come in the following unit:                                *
 *   Particle Energy: MeV/u                                                  *
 *   dEdX:            MeV/micrometer                                         *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
 

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
EnergyLoss::EnergyLoss(string Path , string Source, int NumberOfSlice=100 ,  int LiseColumn , int NumberOfMass) 
	{ 
	
	fNumberOfSlice = NumberOfSlice ; 
	fNumberOfMass  = NumberOfMass  ;
	
	string globalPath = getenv("NPTOOL");
	string StandardPath = globalPath + "/Inputs/EnergyLoss/" + Path;
	
	cout << "///////////////////////////////// " << endl ;
	cout << "Initialising an EnergyLoss object " << endl ;
		
		ifstream TableFile	;
		TableFile.open(StandardPath.c_str())	;

 		// Opening dE/dX file
	  if(TableFile.is_open()) 	cout << "Reading Energy Loss File: " << Path << endl ; 
  	// In case the file is not found in the standard path, the programm try to interpret the file name as an absolute or relative file path.
       else 
        {
          TableFile.open( Path.c_str() );
          if(TableFile.is_open()) { cout << "Reading Energy Loss File: " << Path << endl ;}
          
          else { cout << "ERROR: TABLE FILE NOT FOUND" << endl; return; }
        }
	
	  
	 if (Source == "G4Table")
	 	{
			// Reading Data
			double energy, total;
			string dummy;
			//skipped first line
			getline(TableFile,dummy);
			while ( TableFile >> energy)
	     {
				fEnergy.push_back ( energy*MeV )				;
				TableFile >> total;
				fdEdX_Total.push_back ( total*MeV/micrometer )	;
				}
	   
	  		// Close File
	   		TableFile.close();
				   	
			}
			
		else if (Source == "SRIM")	
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

		else if(Source == "LISE")
			{
		   	cout << "Reading Energy Loss File: " << Path << endl ;
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
			
		else
			{
				cout << "ERROR : Wrong Source Type" << endl ;
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
	
	   double val = fInter->Eval(Energy)									;
	    
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
//    //	Lise file are given in MeV/u
//		//	For SRIM and geant4 file fNumberOfMass = 1 whatever is the nucleus, file are given in MeV
//		Energy = Energy / (double) fNumberOfMass ;
//	
//		if (Angle > halfpi) Angle = pi-Angle								;	
//	
//		TargetThickness = TargetThickness / cos(Angle)  					;
//		double SliceThickness = TargetThickness / (double)fNumberOfSlice 	;
//		
//		//Interpolator* s = new Interpolator( fEnergy , fdEdX_Total	)		;
//		
////		double InitialEnergy = Energy										;
////		//double slow = 0.													;
//		   
//		for (int i = 0; i < fNumberOfSlice ; i++) 
//			{
//			   // double de = s->Eval(Energy) * SliceThickness;
//			    double de = fInter->Eval(Energy) * SliceThickness	;
//			   // slow 	+= de	;
//			    Energy	-= de/fNumberOfMass	;
//			    // If ion do not cross the target
////			    if (Energy < 0) 	{slow = InitialEnergy; break;}
//        if (Energy < 0) 	{Energy=0; break;}
//			}
//		   
//	//	delete s		;
//		return slow	;

    //	Lise file are given in MeV/u
		//	For SRIM and geant4 file fNumberOfMass = 1 whatever is the nucleus, file are given in MeV
		Energy = Energy / (double) fNumberOfMass ;
	
		if (Angle > halfpi) Angle = pi-Angle								;
		TargetThickness = TargetThickness / ( cos(Angle) ) 					;
		
		double SliceThickness = TargetThickness / (double)fNumberOfSlice 	;

		for (int i = 0; i < fNumberOfSlice ; i++) 
			{
			    double de = fInter->Eval(Energy) * SliceThickness	;
			    Energy	 -= de/fNumberOfMass											;
			    
			    if(Energy<0) {Energy=0;break;}
			}
			
		return (Energy*fNumberOfMass)	;

	}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
double EnergyLoss::EvaluateInitialEnergy(	double Energy 					, // Energy of the detected particle
		   																		double TargetThickness	, // Target Thickness at 0 degree
		   																		double Angle						) // Particle Angle
		   																		const
	{
	
		//	Lise file are given in MeV/u
		//	For SRIM and geant4 file fNumberOfMass = 1 whatever is the nucleus, file are given in MeV
		Energy = Energy / (double) fNumberOfMass ;
	
		if (Angle > halfpi) Angle = pi-Angle								;
		TargetThickness = TargetThickness / ( cos(Angle) ) 					;
		
		double SliceThickness = TargetThickness / (double)fNumberOfSlice 	;

		for (int i = 0; i < fNumberOfSlice ; i++) 
			{
			    double de = fInter->Eval(Energy) * SliceThickness	;
			    Energy	+= de/fNumberOfMass												;
			}
			
		return (Energy*fNumberOfMass)	;
	}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
