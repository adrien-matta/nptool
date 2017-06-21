#ifndef EventGeneratorRadioactiveDecay_h
#define EventGeneratorRadioactiveDecay_h
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Daniel Cox    contact address: daniel.m.cox@jyu.fi       *
 *                                                                           *
 * Creation Date  : June 2017                                                *
 * Last update    : 16/06/2017                                               *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *                                                                           *
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

// NPSimulation
#include "VEventGenerator.hh"
#include "Target.hh"
#include "ParticleStack.hh"

using namespace CLHEP;
using namespace std;


class EventGeneratorRadioactiveDecay: public NPS::VEventGenerator{
    public://constructor and destructor
      EventGeneratorRadioactiveDecay();
      virtual ~EventGeneratorRadioactiveDecay();

    public: // Inherit from VEventGenerator class
      void ReadConfiguration(NPL::InputParser);
      void GenerateEvent(G4Event*);
      void InitializeRootOutput() {};

      void LevelReader(const char* filename);//read in the level scheme file
      void ICCReader(const char* filename);//read in the icc file
      void IntensityReader(const char* filename);//read in the intensity file

      // Used in some case to generate event inside the target
      virtual void SetTarget(Target*) {};

      // Used to simulate beam emmitance effect
      void RandomGaussian2D(double MeanX, double MeanY, double SigmaX, double SigmaY, double &X, double &Y, double NumberOfSigma = 10000);

    private: // Pointer to the Particle stack for faster acces
      ParticleStack* m_ParticleStack;

      G4double               m_EnergyLow        ;  // Lower limit of energy range
      G4double               m_EnergyHigh       ;  // Upper limit of energy range
      G4double               m_x0               ;  // Vertex Position X
      G4double               m_y0               ;  // Vertex Position Y
      G4double               m_z0               ;  // Vertex Position Z
      G4double               m_SigmaX           ;
      G4double               m_SigmaY           ;
      G4double               m_HalfOpenAngleMin ;  // Min Half open angle of the source
      G4double               m_HalfOpenAngleMax ;  // Max Half open angle of the source
      G4int                  m_Z                ;
      G4int                  m_A                ;
  };

#endif
