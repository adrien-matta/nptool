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
		X = m_RandomEngine.Gaus( MeanX , SigmaX) ;
		
		double NumberOfSigma ;
			
		NumberOfSigma = ( 2*X / SigmaX ) ;
		NumberOfSigma =  TMath::Floor( sqrt(NumberOfSigma*NumberOfSigma) + 1) ;
		
		double SigmaYPrim = sqrt( NumberOfSigma*SigmaY/2 *NumberOfSigma*SigmaY/2 * ( 1 - 2*X*X / (SigmaX*NumberOfSigma*SigmaX*NumberOfSigma)) ) ;
			SigmaYPrim = SigmaYPrim / NumberOfSigma ; 
			
		Y = m_RandomEngine.Gaus( MeanY , SigmaYPrim) ;

	}
