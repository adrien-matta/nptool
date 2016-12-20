#ifndef EventGeneratorBeam_h
#define EventGeneratorBeam_h
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
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

// NPSimulation header
#include "VEventGenerator.hh"
#include "Target.hh"
#include "ParticleStack.hh"

// NPL Header
#include "NPBeam.h"
#include "NPInputParser.h"
using namespace std  ;

class EventGeneratorBeam : public NPS::VEventGenerator{
public:     // Constructor and destructor
  EventGeneratorBeam();
  virtual ~EventGeneratorBeam();
  
public:     // Inherit from VEventGenerator Class
  void ReadConfiguration(NPL::InputParser);
  void GenerateEvent(G4Event*);
  void InitializeRootOutput();
  void SetTarget(Target* Target) ;
  
private: // Source parameter
  G4ParticleDefinition*   m_particle;  // Kind of particle to shoot
  NPL::Beam* m_Beam;
  
private: // Target Value
  Target* m_Target;
  
private: // Particle Stack
  ParticleStack* m_ParticleStack;

};
#endif
