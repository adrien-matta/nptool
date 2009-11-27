#ifndef EventGeneratorBeam_h
#define EventGeneratorBeam_h
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
 *  This event Generator is used to simulated Radioactive Ion beam           *
 *  Emmitance, Energy spread and dispersion are taken into account           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// C++ header
#include <string>

// G4 header
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleDefinition.hh"

// NPTool header
#include "VEventGenerator.hh"
#include "TInitialConditions.h"
#include "Target.hh"

using namespace std  ;

class EventGeneratorBeam : public VEventGenerator
{
public:     // Constructor and destructor
   EventGeneratorBeam();
   virtual ~EventGeneratorBeam();


public:     // Inherit from VEventGenerator Class
   void ReadConfiguration(string);
   void GenerateEvent(G4Event*, G4ParticleGun*);
   void InitializeRootOutput();

   void SetTarget(Target* Target) ;


private: // TTree to store initial value of beam and reaction
   TInitialConditions*	m_InitConditions;

		   
private: // Source parameter
   G4ParticleDefinition*	m_particle;  // Kind of particle to shoot
   G4double			m_BeamEnergy;
   G4double			m_BeamEnergySpread;
   G4double			m_SigmaX;
   G4double			m_SigmaY;
   G4double			m_SigmaThetaX;
   G4double			m_SigmaPhiY;

private: // Target Value
   Target* m_Target;
};
#endif
