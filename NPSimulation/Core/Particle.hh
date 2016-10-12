#ifndef __PARTICLE__
#define __PARTICLE__
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : April 2012                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * This class define particle to be shoot at the end of the generated event  *
 * It contain a G4ParticleDefinition, a Kinetic energy and the direction to  *
 * shoot the particle in the laboratory frame.                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// G4
#include"G4ParticleDefinition.hh"
#include"G4ThreeVector.hh"
#include"CLHEP/Units/SystemOfUnits.h"
using namespace CLHEP;
class Particle{
  
public: // Constructor and Destructor
        //  Empty constructor (return geantino at zero degree)
  Particle();
  ~Particle();
  //  Constructor to be used
  Particle(G4ParticleDefinition* particle,double ThetaCM,double T,G4ThreeVector Direction, G4ThreeVector Position,bool ShootStatus=true);
  
private: // Private Member
  G4ParticleDefinition* m_ParticleDefinition;
  double m_ThetaCM;
  double m_T ;
  G4ThreeVector m_Direction;
  G4ThreeVector m_Position;
  bool m_ShootStatus;
  
public: // Setter and Getter
  // Getter
  G4ParticleDefinition*   GetParticleDefinition();
  double                  GetParticleThetaCM();
  double                  GetParticleKineticEnergy();
  G4ThreeVector           GetParticleMomentumDirection();
  G4ThreeVector           GetParticlePosition();
  bool                    GetShootStatus();
  // Setter
  void SetParticleDefinition(G4ParticleDefinition*);
  void SetParticleThetaCM(double);
  void SetParticleKineticEnergy(double);
  void SetParticlePosition(G4ThreeVector);
  void SetParticleMomentumDirection(G4ThreeVector);
  void SetShootStatus(bool);
};
#endif
