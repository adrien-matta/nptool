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
 * Last update    : 03/11/2009                                               *
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
 *    + 03/11/09: Adding EffectiveThiknessBeforeInteraction in the           *
 *                CalculateBeamInteraction() method (N. de Sereville)        *
 *                                                                           *
 *****************************************************************************/
#include "VEventGenerator.hh"

// C++ headers
#include "cmath"

// G4 headers
#include "G4UnitsTable.hh"

// CLHEP headers
#include "Randomize.hh"


VEventGenerator::VEventGenerator()
{
}



VEventGenerator::~VEventGenerator()
{
}



void VEventGenerator::CalculateBeamInteraction(double MeanPosX, double SigmaPosX, double MeanPosTheta, double SigmaPosTheta,
                                               double MeanPosY, double SigmaPosY, double MeanPosPhi,   double SigmaPosPhi,
                                               Target* target,
                                               G4ThreeVector &InterCoord, double &AngleEmittanceTheta, double &AngleEmittancePhi,
                                               double &AngleIncidentTheta, double &AngleIncidentPhi,
                                               double &EffectiveTargetThicknessBeforeInteraction)
{
   // target parameters
   double TargetThickness = target->GetTargetThickness();
   double TargetRadius    = target->GetTargetRadius();
   double TargetAngle     = target->GetTargetAngle();

   // beam interaction parameters
   double x0 = 1000 * cm;
   double y0 = 1000 * cm;
   double z0 =    0 * cm;
   double dz =    0 * cm;

   // calculate emittance parameters (x,theta) and (y,phi)
   if (TargetRadius != 0) {	// case of finite target dimensions
      while (sqrt(x0*x0 + y0*y0) > TargetRadius) {
         RandomGaussian2D(MeanPosX, MeanPosTheta, SigmaPosX, SigmaPosTheta, x0, AngleEmittanceTheta);
         RandomGaussian2D(MeanPosY, MeanPosPhi,   SigmaPosY, SigmaPosPhi,   y0, AngleEmittancePhi);
      }
      // in case target is tilted, correct the z-position of interaction
      // x is the vertical axis
      dz = x0 * tan(TargetAngle);
   }
   else {			// if no target radius is given consider a point-like target
      RandomGaussian2D(0, 0, 0, SigmaPosTheta, x0, AngleEmittanceTheta);
      RandomGaussian2D(0, 0, 0, SigmaPosPhi,   y0, AngleEmittancePhi);
   }

	if(target!=0)
		{
			 // target parameters
		   double TargetThickness = target->GetTargetThickness();
		   double TargetRadius    = target->GetTargetRadius();
		   double TargetAngle     = target->GetTargetAngle();

		   // beam interaction parameters
		   double x0 = 1000 * cm;
		   double y0 = 1000 * cm;
		   double z0 =    0 * cm;
		   double dz =    0 * cm;

		   // calculate emittance parameters (x,theta) and (y,phi)
		   if (TargetRadius != 0) {	// case of finite target dimensions
		      while (sqrt(x0*x0 + y0*y0) > TargetRadius) {
		         RandomGaussian2D(MeanPosX, MeanPosTheta, SigmaPosX, SigmaPosTheta, x0, AngleEmittanceTheta);
		         RandomGaussian2D(MeanPosY, MeanPosPhi,   SigmaPosY, SigmaPosPhi,   y0, AngleEmittancePhi);
		      }
		      // in case target is tilted, correct the z-position of interaction
		      dz = x0 * tan(TargetAngle);
		   }
		   else {			// if no target radius is given consider a point-like target
		      RandomGaussian2D(0, 0, 0, SigmaPosTheta, x0, AngleEmittanceTheta);
		      RandomGaussian2D(0, 0, 0, SigmaPosPhi,   y0, AngleEmittancePhi);
		   }

		   // correct for the target angle wrt the beam axis
		   // this simple correction is only valid if the beam is parallel to the beam axis
		   // should be improved in a next version
		   TargetThickness /= cos(TargetAngle);
		   z0 = dz + (-TargetThickness / 2 + RandFlat::shoot() * TargetThickness);

		   // Move to the target position
		   x0 += target->GetTargetX();
		   y0 += target->GetTargetY();
		   z0 += target->GetTargetZ();
		   InterCoord = G4ThreeVector(x0, y0, z0);

		   // Calculate incident angle in spherical coordinate, passing by the direction vector dir      
		   double Xdir = sin(AngleEmittanceTheta);
		   double Ydir = sin(AngleEmittancePhi);
		   double Zdir = cos(AngleEmittanceTheta) + cos(AngleEmittancePhi);

		   AngleIncidentTheta = acos(Zdir / sqrt(Xdir*Xdir + Ydir*Ydir + Zdir*Zdir)) * rad;
		   AngleIncidentPhi   = atan2(Ydir, Xdir) * rad;
		   if (AngleIncidentPhi   < 0)    AngleIncidentPhi += 2*pi;
		   if (AngleIncidentTheta < 1e-6) AngleIncidentPhi  = 0;
	
		}

	else{
		InterCoord = G4ThreeVector(0, 0, 0);
		AngleEmittanceTheta=0;
		AngleEmittancePhi=0;
		AngleIncidentTheta=0; 
		AngleIncidentPhi=0;

		}
		
   
}



void VEventGenerator::RandomGaussian2D(double MeanX, double MeanY, double SigmaX, double SigmaY, double &X, double &Y, double NumberOfSigma)
{
   if (SigmaX != 0) {
      X = 2 * NumberOfSigma*SigmaX;
      while (X > NumberOfSigma*SigmaX) X = RandGauss::shoot(MeanX, SigmaX);

      double a = NumberOfSigma * SigmaX/2;
      double b = NumberOfSigma * SigmaY/2;
      double SigmaYPrim = b * sqrt(1 - X*X / (a*a));

      SigmaYPrim = 2*SigmaYPrim / NumberOfSigma;
      Y = RandGauss::shoot(MeanY, SigmaYPrim);
   }
   else {
      X = MeanX;
      Y = RandGauss::shoot(MeanY, SigmaY);
   }
}
