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

//Root
#include"TMath.h"


VEventGenerator::VEventGenerator()
	{
	}



VEventGenerator::~VEventGenerator()
	{
	}

void VEventGenerator::RandomGaussian2D(double MeanX,double MeanY,double SigmaX,double SigmaY,double &X,double &Y)
	{
		if(SigmaX!=0)
			{
				X = m_RandomEngine.Gaus( MeanX , SigmaX) ;
		
				double NumberOfSigma ;
			
				NumberOfSigma = ( 2*X / SigmaX ) ;
				NumberOfSigma =  TMath::Floor( sqrt(NumberOfSigma*NumberOfSigma) + 1) ;
		
				double SigmaYPrim = sqrt( NumberOfSigma*SigmaY/2 *NumberOfSigma*SigmaY/2 * ( 1 - 2*X*X / (SigmaX*NumberOfSigma*SigmaX*NumberOfSigma)) ) ;
				SigmaYPrim = SigmaYPrim / NumberOfSigma ; 
			
				Y = m_RandomEngine.Gaus( MeanY , SigmaYPrim) ;
			
			}
	
		 else
		 	{
		 		X= MeanX;
		 		Y = m_RandomEngine.Gaus( MeanY , SigmaY) ;
		 	}
		

	}
