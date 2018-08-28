/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : Octobre 2017                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * Use to kill the beam track and replace it with the reaction product       *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
#include <string>
#include "NPFunction.h"
#include "RootOutput.h"
#include "BeamReaction.hh"
#include "NPOptionManager.h"
#include "NPInputParser.h"
#include "G4VPhysicalVolume.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
////////////////////////////////////////////////////////////////////////////////
NPS::BeamReaction::BeamReaction(G4String modelName,G4Region* envelope) :
  G4VFastSimulationModel(modelName, envelope) {
    ReadConfiguration();
    m_PreviousEnergy=0 ;
    m_PreviousLength=0 ;
    m_active = true;
    m_ReactionConditions = new TReactionConditions();
    AttachReactionConditions();
    if(!RootOutput::getInstance()->GetTree()->FindBranch("ReactionConditions"))
      RootOutput::getInstance()->GetTree()->Branch("ReactionConditions","TReactionConditions",&m_ReactionConditions);
  }


////////////////////////////////////////////////////////////////////////////////
NPS::BeamReaction::BeamReaction(G4String modelName) :
  G4VFastSimulationModel(modelName) {
  }

////////////////////////////////////////////////////////////////////////////////
NPS::BeamReaction::~BeamReaction() {
}

////////////////////////////////////////////////////////////////////////////////
void NPS::BeamReaction::AttachReactionConditions(){
    // Reasssigned the branch address
    if(RootOutput::getInstance()->GetTree()->FindBranch("ReactionConditions"))
        RootOutput::getInstance()->GetTree()->SetBranchAddress("ReactionConditions",&m_ReactionConditions);
}


////////////////////////////////////////////////////////////////////////////////
void NPS::BeamReaction::ReadConfiguration(){
  NPL::InputParser input(NPOptionManager::getInstance()->GetReactionFile());
  m_Reaction.ReadConfigurationFile(input);
  m_BeamName=NPL::ChangeNameToG4Standard(m_Reaction.GetNucleus1().GetName());
  if(m_Reaction.GetNucleus3().GetName()!=""){
    m_active = true;
  }
  else{
    m_active = false;

  }
}

////////////////////////////////////////////////////////////////////////////////
G4bool NPS::BeamReaction::IsApplicable( const G4ParticleDefinition& particleType) {
  if(!m_active)
    return false;

  static std::string particleName;
  particleName = particleType.GetParticleName();
  if (particleName.find(m_BeamName)!=std::string::npos) {
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////
G4bool NPS::BeamReaction::ModelTrigger(const G4FastTrack& fastTrack) {
  //static bool shoot = false;
  static double rand = 0;
  const G4Track* PrimaryTrack = fastTrack.GetPrimaryTrack();   
  G4ThreeVector V = PrimaryTrack->GetMomentum().unit();
  G4ThreeVector P = fastTrack.GetPrimaryTrackLocalPosition();
  G4VSolid* solid = 
    fastTrack.GetPrimaryTrack()->GetVolume()->GetLogicalVolume()->GetSolid();
  double in = solid->DistanceToOut(P,V);
  double out = solid->DistanceToOut(P,-V);
  double ratio  = in / (out+in) ; 
  
  if(out == 0){// first step of current event
      rand = G4RandFlat::shoot();
      m_PreviousLength = m_StepSize ;
      m_PreviousEnergy = PrimaryTrack->GetKineticEnergy() ;
    }

  else if(in==0){// last step
    return true;
    }

  // If the condition is met, the event is generated 
  if(ratio<rand){ 
    // Reset the static for next event
    //  shoot = false;
    if(m_Reaction.IsAllowed(PrimaryTrack->GetKineticEnergy())){
      return true;
      
      }
    else{
      return false;
      }
  }

  // Record the situation of the current step
  // so it can be used in the next one
  if(!PrimaryTrack->GetStep()->IsLastStepInVolume()){
    m_PreviousLength = PrimaryTrack->GetStep()->GetStepLength(); 
    m_PreviousEnergy = PrimaryTrack->GetKineticEnergy();
  }

  return false;
}

////////////////////////////////////////////////////////////////////////////////
void NPS::BeamReaction::DoIt(const G4FastTrack& fastTrack,G4FastStep& fastStep) {
    
    // Clear Previous Event
    m_ReactionConditions->Clear();
    
    // Get the track info
    const G4Track* PrimaryTrack = fastTrack.GetPrimaryTrack();
    G4ThreeVector pdirection = PrimaryTrack->GetMomentum().unit();
    G4ThreeVector localdir = fastTrack.GetPrimaryTrackLocalDirection();
    
    G4ThreeVector worldPosition = PrimaryTrack->GetPosition();
    G4ThreeVector localPosition = fastTrack.GetPrimaryTrackLocalPosition();
    
    double energy = PrimaryTrack->GetKineticEnergy();
    double time = PrimaryTrack->GetGlobalTime();
    
    // Randomize within the step
    // Assume energy loss is linear within the step
    // Assume no scattering
    double rand =  G4RandFlat::shoot();
    double length = rand*(m_PreviousLength);
    energy += (1-rand)*(m_PreviousEnergy-energy);
    G4ThreeVector ldir = pdirection;
    ldir*=length;
    localPosition = localPosition - ldir;
    // Set the end of the step conditions
    fastStep.SetPrimaryTrackFinalKineticEnergyAndDirection(0,pdirection);
    fastStep.SetPrimaryTrackFinalPosition(worldPosition);
    fastStep.SetTotalEnergyDeposited(0);
    fastStep.SetPrimaryTrackFinalTime (time);
    fastStep.KillPrimaryTrack();
    fastStep.SetPrimaryTrackPathLength(0.0);
    
    //////////////////////////////////////////////////
    //////Define the kind of particle to shoot////////
    //////////////////////////////////////////////////
    
    // Nucleus 3
    int LightZ = m_Reaction.GetNucleus3().GetZ() ;
    int LightA = m_Reaction.GetNucleus3().GetA() ;
    static G4IonTable* IonTable = G4ParticleTable::GetParticleTable()->GetIonTable();
    
    G4ParticleDefinition* LightName
    = IonTable->GetIon(LightZ, LightA, m_Reaction.GetExcitation3()*MeV);
    
    // Nucleus 4
    G4int HeavyZ = m_Reaction.GetNucleus4().GetZ() ;
    G4int HeavyA = m_Reaction.GetNucleus4().GetA() ;
    
    // Generate the excitation energy if a distribution is given
    m_Reaction.ShootRandomExcitationEnergy();
    
    // Use to clean up the IonTable in case of the Ex changing at every event
    G4ParticleDefinition* HeavyName
    = IonTable->GetIon(HeavyZ, HeavyA, m_Reaction.GetExcitation4()*MeV);
    // Set the Energy of the reaction
    m_Reaction.SetBeamEnergy(energy);
    
    double Beam_theta = pdirection.theta();
    double Beam_phi = pdirection.phi();
    
    ///////////////////////////
    ///// Beam Parameters /////
    ///////////////////////////
    m_ReactionConditions->SetBeamParticleName(PrimaryTrack->GetParticleDefinition()->GetParticleName());

    m_ReactionConditions->SetBeamReactionEnergy(energy);
    m_ReactionConditions->SetVertexPositionX(localPosition.x());
    m_ReactionConditions->SetVertexPositionY(localPosition.y());
    m_ReactionConditions->SetVertexPositionZ(localPosition.z());
    
    G4ThreeVector U(1,0,0);
    G4ThreeVector V(0,1,0);
    
    m_ReactionConditions->SetBeamEmittanceTheta(PrimaryTrack->GetMomentumDirection().theta()/deg);
    m_ReactionConditions->SetBeamEmittancePhi(PrimaryTrack->GetMomentumDirection().phi()/deg);
    m_ReactionConditions->SetBeamEmittanceThetaX(PrimaryTrack->GetMomentumDirection().angle(U)/deg);
    m_ReactionConditions->SetBeamEmittancePhiY(PrimaryTrack->GetMomentumDirection().angle(V)/deg);
    
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
    m_Reaction.ShootRandomThetaCM();
    double phi     = RandFlat::shoot() * 2. * pi;
    
    //////////////////////////////////////////////////
    /////  Momentum and angles from  kinematics  /////
    /////  Angles are in the beam frame          /////
    //////////////////////////////////////////////////
    // Variable where to store results
    double Theta3, Energy3, Theta4, Energy4;
    
    // Compute Kinematic using previously defined ThetaCM
    m_Reaction.KineRelativistic(Theta3, Energy3, Theta4, Energy4);
    // Momentum in beam frame for light particle
    G4ThreeVector momentum_kine3_beam(sin(Theta3) * cos(phi),
                                      sin(Theta3) * sin(phi),
                                      cos(Theta3));
    // Momentum in World frame
    G4ThreeVector momentum_kine3_world = BeamToWorld * momentum_kine3_beam;
    
    
    // Momentum in beam frame for heavy particle
    G4ThreeVector momentum_kine4_beam(sin(Theta4) * cos(phi+pi),
                                      sin(Theta4) * sin(phi+pi),
                                      cos(Theta4));
    // Momentum in World frame
    G4ThreeVector momentum_kine4_world = BeamToWorld * momentum_kine4_beam;
    
    
    // Emitt secondary
    G4DynamicParticle particle3(LightName,momentum_kine3_world,Energy3);
    fastStep.CreateSecondaryTrack(particle3, localPosition, time);
    
    G4DynamicParticle particle4(HeavyName,momentum_kine4_world,Energy4);
    fastStep.CreateSecondaryTrack(particle4, localPosition, time);
    
    // Reinit for next event
    m_PreviousEnergy=0 ;
    m_PreviousLength=0 ;
    
    ///////////////////////////////////////
    ///// Emitted particle Parameters /////
    ///////////////////////////////////////
    // Names 3 and 4//
    m_ReactionConditions->SetParticleName(LightName->GetParticleName());
    m_ReactionConditions->SetParticleName(HeavyName->GetParticleName());
    // Angle 3 and 4 //
    m_ReactionConditions->SetTheta(Theta3/deg);
    m_ReactionConditions->SetTheta(Theta4/deg);
    // Energy 3 and 4 //
    m_ReactionConditions->SetKineticEnergy(Energy3);
    m_ReactionConditions->SetKineticEnergy(Energy4);
    // ThetaCM //
    m_ReactionConditions->SetThetaCM(m_Reaction.GetThetaCM()/deg);
    // Momuntum X 3 and 4 //
    m_ReactionConditions->SetMomentumDirectionX(momentum_kine3_world.x());
    m_ReactionConditions->SetMomentumDirectionX(momentum_kine4_world.x());
    // Momuntum Y 3 and 4 //
    m_ReactionConditions->SetMomentumDirectionY(momentum_kine3_world.y());
    m_ReactionConditions->SetMomentumDirectionY(momentum_kine4_world.y());
    // Momuntum Z 3 and 4 //
    m_ReactionConditions->SetMomentumDirectionZ(momentum_kine3_world.z());
    m_ReactionConditions->SetMomentumDirectionZ(momentum_kine4_world.z());
}
