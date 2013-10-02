/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  All event generator added in the project should derive from this virtual * 
 *  class.                                                                   *
 *  A VEventGenerator object is manage by the PrimaryGeneratorAction class   *
 *  and call the virtual method of this class implemented in the daughter    *
 *  class object.                                                            *
 *  This inheritance insure homogeneity and modularity of the code           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include "VEventGenerator.hh"

// ROOT headers
#include"TMath.h"

// CLHEP headers
#include "Randomize.hh"

VEventGenerator::VEventGenerator()
	{
	}

VEventGenerator::~VEventGenerator()
	{
	}

void VEventGenerator::RandomGaussian2D(double MeanX,double MeanY,double SigmaX,double SigmaY,double &X,double &Y,double NumberOfSigma)
	{
	
		if(SigmaX!=0)
			{
				X=2* NumberOfSigma *SigmaX;
				while(X>NumberOfSigma*SigmaX)
				X = RandGauss::shoot( MeanX , SigmaX);
				double a = NumberOfSigma * SigmaX/2  ;
				double b = NumberOfSigma * SigmaY/2  ;
				 
				double SigmaYPrim = b * sqrt(  1 - X*X / (a*a) ) ;
				SigmaYPrim = 2*SigmaYPrim / NumberOfSigma ; 
			
				Y = RandGauss::shoot( MeanY , SigmaYPrim) ;
			}
	
		 else
		 	{
		 		X = MeanX								;
		 		Y = RandGauss::shoot( MeanY , SigmaY)	;
		 	}
	
	}
