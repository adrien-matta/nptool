/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : December 2016                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 * Class to simulate electron drift in a gas detector such as Ionisation     *
 * chamber and TPC                                                           *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
#include "FastDriftElectron.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include <iostream>
#include "G4SystemOfUnits.hh"
////////////////////////////////////////////////////////////////////////////////
DriftElectron::DriftElectron(G4String modelName,G4Region* envelope) :
  G4VFastSimulationModel(modelName, envelope) {
  }

////////////////////////////////////////////////////////////////////////////////
DriftElectron::DriftElectron(G4String modelName) :
  G4VFastSimulationModel(modelName) {
  }

////////////////////////////////////////////////////////////////////////////////
DriftElectron::~DriftElectron() {
}

////////////////////////////////////////////////////////////////////////////////
G4bool DriftElectron::IsApplicable( const G4ParticleDefinition& particleType) {
  G4String particleName = particleType.GetParticleName();
  if (particleName=="e-") {
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////
G4bool DriftElectron::ModelTrigger(const G4FastTrack& fastTrack) {
  G4String particleName = 
    fastTrack.GetPrimaryTrack()->GetParticleDefinition()->GetParticleName();
  if (particleName=="e-") {
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////
void DriftElectron::DoIt(const G4FastTrack& fastTrack,G4FastStep& fastStep) {
  double v_drift = 8e-3*mm/ns;
  double d_trans = 6e-5*mm2/ns;
  double d_long  = 4e-5*mm2/ns;
  double step = 100*ns;
  double pair_energy=30*eV;
  // fraction of electron drift to be produced
  // 1 means all, 0 means none
  // directly affect the simulation speed
  double production_bias = 0.01; 
  G4ThreeVector driftDir = G4ThreeVector(1,0,0);
  G4ThreeVector TransDir = G4ThreeVector(0,1,0);
  G4ThreeVector LongDir = G4ThreeVector(0,0,1);


  G4ThreeVector pdirection = fastTrack.GetPrimaryTrack()->GetMomentum().unit();
  G4ThreeVector localdir = fastTrack.GetPrimaryTrackLocalDirection();
  
  G4ThreeVector worldPosition = fastTrack.GetPrimaryTrack()->GetPosition();
  G4ThreeVector localPosition = fastTrack.GetPrimaryTrackLocalPosition();

  double energy = fastTrack.GetPrimaryTrack()->GetKineticEnergy();
  double time = fastTrack.GetPrimaryTrack()->GetGlobalTime();

  // If the electron is below the pair energy
  // then propagate
  if(energy<pair_energy*1.1){
  // Vertical Drift
  double step_length = v_drift*step;
  double sigmaTrans  = d_trans*step/step_length;
  double sigmaLong   = d_long*step/step_length;
  G4ThreeVector newPos = localPosition +step_length*driftDir;
  // trans drift
  newPos += G4RandGauss::shoot(0,sigmaTrans)*TransDir;
  newPos += G4RandGauss::shoot(0,sigmaLong)*LongDir;
  G4ThreeVector Dir = localPosition-newPos;

  // Set the end of the step conditions
  fastStep.SetPrimaryTrackFinalKineticEnergyAndDirection(energy,Dir.unit());
  fastStep.SetPrimaryTrackFinalPosition(newPos);  
  fastStep.SetPrimaryTrackFinalTime (time+step);
  }

  // Else the energy is splitted into pairs
  else{
    int number_of_electron = production_bias*energy/pair_energy;
	  fastStep.KillPrimaryTrack();
	  fastStep.SetPrimaryTrackPathLength(0.0);
    for(int i = 0 ; i < number_of_electron ; i++){
      G4ThreeVector p(G4RandFlat::shoot(),G4RandFlat::shoot(),G4RandFlat::shoot()); 
      G4DynamicParticle particle(G4Electron::ElectronDefinition(),p.unit(), pair_energy);
		  fastStep.CreateSecondaryTrack(particle, localPosition, time);
    }
  }
}
