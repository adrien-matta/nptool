#ifndef __EnergyLoss__
#define __EnergyLoss__
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
 *  or Geant4                                                                *
 *  The class can be used to evaluate energy loss in material or to Evaluate * 
 *  initial energy before crossing the material.                             *
 *                                                                           *
 *  The use of Interpolator derived form the GSL insure a very good speed of *
 *  execution.                                                               *
 *                                                                           *
 *   Table Should come in the following unit:                                *
 *   Particle Energy: MeV/u                                                  *
 *   dEdX:            MeV/micrometer                                         *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *        Based on N.Desereville previous classes                            *
 *                                                                           *
 *****************************************************************************/
 
#include <string>
#include <vector>
using namespace std ;

//	ROOT
//#include "TObject.h"

// Use CLHEP System of unit and Physical Constant
#include "CLHEP/Units/GlobalSystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"
using namespace CLHEP ;

// ROOT
#include "Math/InterpolationTypes.h"
#include "Math/Interpolator.h"
using namespace ROOT::Math;

namespace NPL 
	{
		 class EnergyLoss {
		 
		 public :	//	Constructor
		   EnergyLoss();
		   EnergyLoss(   	string Path 				,	//	Path of dE/dX table file
		   								string Source  			, // Type of file : Geant4,Lise,SRIM
		   				 				int NumberOfSlice		, //	Low number = Faster, High Number = more accurate / typical: 100 to 1000
		   								int LiseColumns=0		,	//	Indicate which model to read in a lise File, set to 0 (Default value) for a SRIM / Geant4 file
		   				 				int NumberOfMass=1	);//	Number of mass A of the nucleus (used only for Lise file)	
		   ~EnergyLoss();
		 
		 private :	//	dE/dX, slice parameter
		   int				      fNumberOfSlice    ;  	 // Number Of Slice used to evaluate energy loss
		   int				      fNumberOfMass     ;	 // Number of mass A of the nucleus (used only for Lise file)	
		   vector<double> 	fEnergy           ;    // Energy
		   vector<double> 	fdEdX_Nuclear		  ;    // Nuclear Stopping Power
		   vector<double> 	fdEdX_Electronic	;    // Electronic Stopping Power
		   vector<double> 	fdEdX_Total       ;    // Total Stopping Power
		   Interpolator*    fInter            ;	 // Interpolator Used to evaluate Energy loss at given energy
		   
		public : 	//	General Function on dE/dX table		
		   double	EvaluateNuclearLoss     (double ener) 	const;
		   double	EvaluateElectronicLoss	(double ener) 	const;
		   double	EvaluateTotalLoss		    (double ener) 	const;
		   
		public :	//	Function to Slow down particle or reconstruct their initial energy
		
		   //	Calculate Energy of a particle after crossing material
		   double	Slow(	double Energy           , // Energy of the detected particle
	         					double TargetThickness  , // Target Thickness at 0 degree
	         					double Angle            ) // Particle Angle
	         					const;
		   					
      //	Calculate Energy Loss of a particle inside a material		   					
		   double EnergyLossCalulation(	double Energy           , // Energy of the detected particle
                           					double TargetThickness	, // Target Thickness at 0 degree
                           					double Angle            ) // Particle Angle
                           					const;					
		                             					
		   //	Evaluate Initial Energy of particle before crossing material knowing Angle, final Energy 
		   //   and Target Thickness.
		   double	EvaluateInitialEnergy(	double energy            , // Energy of the detected particle
		                 									double TargetThickness	, // Target Thickness at 0 degree
		                 									double Angle            ) // Particle Angle
		                 									const ;
		   	
		   // Evaluate the thickness the particle has been through using the energy loss and initial energy
		   // usefull for thickness measurement using particle sources
		   double	EvaluateMaterialThickness(	double InitialEnergy 	, // Energy of the detected particle
		   									                  double FinalEnergy	  ,
                                          double ThicknessLimit ,
                                          double ThicknessStep = 0.1*micrometer) // Target Thickness at 0 degree
		   									                  const ;
		   									
		public	:	//	Usefull function
			//	Display parameter	
		   void Print() const;
		   	//	Draw (CERN ROOT)
		   void Draw() const;

		};
	}

#endif
