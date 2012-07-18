/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
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

#include "Particle.hh"

Particle::Particle(){
    m_ParticleDefinition = NULL; 
    m_T = 1*GeV;
    m_Direction = G4ThreeVector(0,0,1);
}

Particle::~Particle(){
}

Particle::Particle(G4ParticleDefinition* particle,double T,G4ThreeVector Direction, G4ThreeVector Position,bool ShootStatus){
    m_ParticleDefinition = particle;
    m_T = T;
    m_Direction = Direction;
    m_Position = Position;
    m_ShootStatus = ShootStatus;
}

G4ParticleDefinition* Particle::GetParticleDefinition(){
    return m_ParticleDefinition;
}

double Particle::GetParticleKineticEnergy(){
    return m_T;
}

G4ThreeVector Particle::GetParticleMomentumDirection(){
    return m_Direction;
}

G4ThreeVector Particle::GetParticlePosition(){
    return m_Position;
}

bool Particle::GetShootStatus(){
    return m_ShootStatus;
}

void Particle::SetParticleDefinition(G4ParticleDefinition* particle){
    m_ParticleDefinition = particle;
}

void Particle::SetParticleKineticEnergy(double T){
    m_T = T ;
}

void Particle::SetParticleMomentumDirection(G4ThreeVector Direction){
    m_Direction = Direction;
}

void Particle::SetParticlePosition(G4ThreeVector Position){
    m_Position = Position;
}

void Particle::SetShootStatus(bool ShootStatus){
    m_ShootStatus = ShootStatus;
}

