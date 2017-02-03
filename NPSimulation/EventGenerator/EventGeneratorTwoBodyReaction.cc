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
 * Last update    : January 2013                                             *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This event Generator is used to simulated two body two body reaction.    *
 *  A Relativistic computation is performed to determine angle and energy of *
 *   the different particle, knowing the ThetaCM angle given by a cross      *
 *   section shoot. Eleastic scattering can also be simulated.               *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + 20/01/2011: Add support for excitation energy for light ejectile     *
 *                  (N. de Sereville)                                        *
 *    + 23/01/2013: Class change name (ild name EventGeneratorTransfert)     *
 *                  (A. MATTA)                                               *
 *                                                                           *
 *****************************************************************************/
// C++ headers
#include <iostream>
#include <fstream>
#include <limits>

// G4 header defining G4 types
#include "globals.hh"

// G4 headers
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4RotationMatrix.hh"

// G4 headers including CLHEP headers
// for generating random numbers
#include "Randomize.hh"

// NPTool headers
#include "EventGeneratorTwoBodyReaction.hh"
#include "RootOutput.h"

using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorTwoBodyReaction::EventGeneratorTwoBodyReaction()
:  m_ShootLight(0),
m_ShootHeavy(0),
m_Target(0),
m_Reaction(new Reaction)
{
  //------------- Default Constructor -------------
  m_ParticleStack= ParticleStack::getInstance();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorTwoBodyReaction::SetTarget(Target* Target) {
  if (Target != 0) {
    m_Target = Target;
  }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventGeneratorTwoBodyReaction::~EventGeneratorTwoBodyReaction(){
  //------------- Default Destructor ------------
  delete m_Reaction;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorTwoBodyReaction::InitializeRootOutput(){
  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorTwoBodyReaction::Print() const{
  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//    Inherit from VEventGenerator

void EventGeneratorTwoBodyReaction::ReadConfiguration(NPL::InputParser parser){
  m_Reaction->ReadConfigurationFile(parser);
  m_ShootLight = m_Reaction->GetShoot3();
  m_ShootHeavy = m_Reaction->GetShoot4();;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventGeneratorTwoBodyReaction::GenerateEvent(G4Event*){
  //////////////////////////////////////////////////
  //////Define the kind of particle to shoot////////
  //////////////////////////////////////////////////
  // Nucleus 3
  G4int LightZ = m_Reaction->GetNucleus3().GetZ() ;
  G4int LightA = m_Reaction->GetNucleus3().GetA() ;
  
  G4ParticleDefinition* LightName
  = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(LightZ, LightA, m_Reaction->GetExcitation3()*MeV);
  
  // Nucleus 4
  G4int HeavyZ = m_Reaction->GetNucleus4().GetZ() ;
  G4int HeavyA = m_Reaction->GetNucleus4().GetA() ;
  
  // Generate the excitation energy if a distribution is given
  m_Reaction->ShootRandomExcitationEnergy();

  G4ParticleDefinition* HeavyName
  = G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(HeavyZ, HeavyA, m_Reaction->GetExcitation4()*MeV);
  
  // Get the beam particle form the Particle Stack
  Particle BeamParticle = m_ParticleStack->SearchAndRemoveParticle(m_BeamName);
  m_Reaction->SetBeamEnergy(BeamParticle.GetParticleKineticEnergy());
  
  G4double Beam_theta = BeamParticle.GetParticleMomentumDirection().theta();
  G4double Beam_phi = BeamParticle.GetParticleMomentumDirection().phi();
  
  //////////////////////////////////////////////////////////
  ///// Build rotation matrix to go from the incident //////
  ///// beam frame to the "world" frame               //////
  //////////////////////////////////////////////////////////
  G4ThreeVector col1(cos(Beam_theta) * cos(Beam_phi),
                     cos(Beam_theta) * sin(Beam_phi),
                     -sin(Beam_theta));
  G4ThreeVector col2(-sin(Beam_phi),
                     cos(Beam_phi),
                     0);
  G4ThreeVector col3(sin(Beam_theta) * cos(Beam_phi),
                     sin(Beam_theta) * sin(Beam_phi),
                     cos(Beam_theta));
  G4RotationMatrix BeamToWorld(col1, col2, col3);
  
  /////////////////////////////////////////////////////////////////
  ///// Angles for emitted particles following Cross Section //////
  ///// Angles are in the beam frame                         //////
  /////////////////////////////////////////////////////////////////
  
  // Angles
  // Shoot and Set a Random ThetaCM
  G4double ThetaCM = m_Reaction->ShootRandomThetaCM();
  G4double phi     = RandFlat::shoot() * 2. * pi;
 
  //////////////////////////////////////////////////
  /////  Momentum and angles from  kinematics  /////
  /////  Angles are in the beam frame          /////
  //////////////////////////////////////////////////
  // Variable where to store results
  G4double ThetaLight, EnergyLight, ThetaHeavy, EnergyHeavy;
  
  // Compute Kinematic using previously defined ThetaCM
  m_Reaction->KineRelativistic(ThetaLight, EnergyLight, ThetaHeavy, EnergyHeavy);
  
  // Momentum in beam frame for light particle
  G4ThreeVector momentum_kineLight_beam(sin(ThetaLight) * cos(phi),
                                        sin(ThetaLight) * sin(phi),
                                        cos(ThetaLight));
  
  // Momentum in beam frame for heavy particle
  G4ThreeVector momentum_kineHeavy_beam(sin(ThetaHeavy) * cos(phi+pi),
                                        sin(ThetaHeavy) * sin(phi+pi),
                                        cos(ThetaHeavy));
  
  //////////////////////////////////////////////////
  ///////// Set up everything for shooting /////////
  //////////////////////////////////////////////////
  // Case of light particle
  // Particle direction
  // Kinematical angles in the beam frame are transformed
  // to the "world" frame
  G4ThreeVector momentum_kine_world = BeamToWorld * momentum_kineLight_beam;
  //Add the particle to the particle stack
  Particle LightParticle(LightName, ThetaCM , EnergyLight,momentum_kine_world, BeamParticle.GetParticlePosition(), m_ShootLight);
  m_ParticleStack->AddParticleToStack(LightParticle);
  
  // Case of heavy particle
  // Particle direction
  // Kinematical angles in the beam frame are transformed
  // to the "world" frame
  momentum_kine_world = BeamToWorld * momentum_kineHeavy_beam;
  Particle HeavyParticle(HeavyName, ThetaCM + M_PI, EnergyHeavy,momentum_kine_world, BeamParticle.GetParticlePosition(), m_ShootHeavy);
  //Add the particle to the particle stack
  m_ParticleStack->AddParticleToStack(HeavyParticle);
}
