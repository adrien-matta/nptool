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
#include <set>
#include "NPFunction.h"
#include "Decay.hh"
#include "NPOptionManager.h"
#include "NPInputParser.h"
#include "G4VPhysicalVolume.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"


using namespace NPS;
////////////////////////////////////////////////////////////////////////////////
Decay::Decay(G4String modelName,G4Region* envelope) :
  G4VFastSimulationModel(modelName, envelope) {
    ReadConfiguration();
    m_PreviousEnergy=0 ;
    m_PreviousLength=0 ;
  }


////////////////////////////////////////////////////////////////////////////////
Decay::Decay(G4String modelName) :
  G4VFastSimulationModel(modelName) {
  }

////////////////////////////////////////////////////////////////////////////////
Decay::~Decay() {
}

////////////////////////////////////////////////////////////////////////////////
void Decay::ReadConfiguration(){
  NPL::InputParser input(NPOptionManager::getInstance()->GetReactionFile());
  m_Decay.ReadConfiguration(input);
  std::set<std::string> Mother = m_Decay.GetAllMotherName();
  std::set<std::string>::iterator it ;
  for(it = Mother.begin() ; it != Mother.end() ; it++)
    m_MotherName.insert(NPL::ChangeNameToG4Standard(*it));
}

////////////////////////////////////////////////////////////////////////////////
G4bool Decay::IsApplicable( const G4ParticleDefinition& particleType) {
  m_CurrentName = particleType.GetParticleName();
  // Extract Ex from name
  if(m_CurrentName.find("[")!=std::string::npos)
    m_ExcitationEnergy = atof(m_CurrentName.substr(m_CurrentName.find("[")+1,m_CurrentName.find("]")-1).c_str())*keV;
  else
    m_ExcitationEnergy=0;


  // Strip name from excitation energy
  m_CurrentName = m_CurrentName.substr(0,m_CurrentName.find("["));
  // If the decay exist
   if (m_MotherName.find(m_CurrentName)!=m_MotherName.end()) {
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////
G4bool Decay::ModelTrigger(const G4FastTrack& ) {
  //FIXME: Solve the issue of long lived decay
  // Check that a decay is possible:
  return m_Decay.AnyAboveThreshold(NPL::ChangeNameFromG4Standard(m_CurrentName),m_ExcitationEnergy);
}

////////////////////////////////////////////////////////////////////////////////
void Decay::DoIt(const G4FastTrack& fastTrack,G4FastStep& fastStep){
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
  //////////////////////////////////////////////////
  //////Define the kind of particle to shoot////////
  //////////////////////////////////////////////////
  std::vector<NPL::Nucleus> Daughter;
  std::vector<double> Ex;
  std::vector<double> DEK;
  std::vector<double> DPx;
  std::vector<double> DPy;
  std::vector<double> DPz;

  m_Decay.GenerateEvent(NPL::ChangeNameFromG4Standard(m_CurrentName),m_ExcitationEnergy,energy,
      pdirection.x(),pdirection.y(),pdirection.z(),
      Daughter, Ex,DEK,DPx,DPy,DPz);

 
  G4ParticleDefinition* DaughterDef; 
  unsigned int size = Daughter.size();
  if(size == 0)
    return;
  for(unsigned int i = 0 ; i < size ; i++){
    // Get the decaying particle
    int DaughterZ = Daughter[i].GetZ();
    int DaughterA = Daughter[i].GetA();
    DaughterDef=NULL;

    // neutral particle
    if(DaughterZ==0){
      if(DaughterA==1)
        DaughterDef=G4ParticleTable::GetParticleTable()->FindParticle("neutron");
     
      else if(DaughterA==0){
        DaughterDef=G4ParticleTable::GetParticleTable()->FindParticle("gamma");
        }

    }
    // proton
    else if (DaughterZ==1 && DaughterA==1 )
      DaughterDef=G4ParticleTable::GetParticleTable()->FindParticle("proton");
    // the rest
    else
      DaughterDef=G4ParticleTable::GetParticleTable()->GetIonTable()->GetIon(DaughterZ, DaughterA, Ex[i]);

    // Set the momentum direction
    G4ThreeVector Momentum (DPx[i],DPy[i],DPz[i]);
    Momentum=Momentum.unit();
      
    G4DynamicParticle DynamicDaughter(DaughterDef,Momentum,DEK[i]);
    fastStep.CreateSecondaryTrack(DynamicDaughter, localPosition, time);
  }
  if(size){
    // Set the end of the step conditions
    fastStep.SetPrimaryTrackFinalKineticEnergyAndDirection(0,pdirection);
    fastStep.SetPrimaryTrackFinalPosition(worldPosition);  
    fastStep.SetTotalEnergyDeposited(0);
    fastStep.SetPrimaryTrackFinalTime (time);
    fastStep.KillPrimaryTrack();
    fastStep.SetPrimaryTrackPathLength(0.0);
    }
}
