#ifndef VEventGenerator_h
#define VEventGenerator_h
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
// C++ header
#include <string>

// G4 header defining G4 types
#include "globals.hh"

// G4 headers
#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "G4ThreeVector.hh"

// NPTool headers
#include "Target.hh"

using namespace CLHEP;
using namespace std;



class VEventGenerator
{
public:
   VEventGenerator();
   virtual ~VEventGenerator();

public:
   virtual void ReadConfiguration(string) {};
   virtual void GenerateEvent(G4Event*, G4ParticleGun*) {};
   virtual void InitializeRootOutput() {};

   // Used in some case to generate event inside the target
   virtual void SetTarget(Target*) {};

   // Used to calculate the incident beam direction (taking into account
   // the emittance) and the vertex of interaction in target
   void CalculateBeamInteraction(double MeanPosX, double SigmaPosX, double MeanPosTheta, double SigmaPosTheta,
                                 double MeanPosY, double SigmaPosY, double MeanPosPhi,   double SigmaPosPhi,
                                 Target* target,
                                 G4ThreeVector &InterCoord, double &AngleEmittanceTheta, double &AngleEmittancePhi,
                                 double &AngleIncidentTheta, double &AngleIncidentPhi);

   // Used to simulate beam emmitance effect
   void RandomGaussian2D(double MeanX, double MeanY, double SigmaX, double SigmaY, double &X, double &Y, double NumberOfSigma = 10000);
};


#endif
