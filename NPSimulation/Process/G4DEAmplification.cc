//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: G4DEAmplification.hh 69576 2016-12-19 13:48:13Z gcosmo $
//
////////////////////////////////////////////////////////////////////////
// Optical DriftElectron Amplification Implementation 
////////////////////////////////////////////////////////////////////////
//
// File:        G4DEAmplification.hh
// Description: Discrete Process -- Bulk amplification of Drift Electron
// Version:     1.0
// Created:     2016-12-19 
// Author:      Adrien Matta
// Updated:     
//             
//             
//              
//              
// mail:        matta@lpccaen.in2p3.fr 
//
////////////////////////////////////////////////////////////////////////

#include "G4ios.hh"
#include "G4DEProcessSubType.hh"

#include "G4DEAmplification.hh"
#include "G4PhysicalConstants.hh"
#include <iostream>
using namespace std;
/////////////////////////
// Class Implementation
/////////////////////////

//////////////
// Operators
//////////////

// G4DEAmplification::operator=(const G4DEAmplification &right)
// {
// }

/////////////////
// Constructors
/////////////////

G4DEAmplification::G4DEAmplification(const G4String& processName, G4ProcessType type)
  : G4VRestDiscreteProcess(processName, type)
{
  if (verboseLevel>0) {
    G4cout << GetProcessName() << " is created " << G4endl;
  }

  SetProcessSubType(fDEAmplification);
}

// G4DEAmplification::G4DEAmplification(const G4OpAbsorpton &right)
// {
// }

////////////////
// Destructors
////////////////

G4DEAmplification::~G4DEAmplification(){}

////////////
// Methods
////////////

// PostStepDoIt
// -------------
//
  G4VParticleChange*
G4DEAmplification::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
{
  // Get the primary track
  aParticleChange.Initialize(aTrack);

  // Check that the parent process is not amplification
  if(aTrack.GetCreatorProcess()->GetProcessName()=="DEAmplification" )
    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);

  const G4Material* aMaterial = aTrack.GetMaterial();

  G4StepPoint* pPreStepPoint  = aStep.GetPreStepPoint();

  G4ThreeVector x0 = pPreStepPoint->GetPosition();
  G4ThreeVector p0 = aStep.GetDeltaPosition().unit();
  G4double      t0 = pPreStepPoint->GetGlobalTime();

  // Get the material table
  G4MaterialPropertiesTable* aMaterialPropertiesTable =
    aMaterial->GetMaterialPropertiesTable();
  if (!aMaterialPropertiesTable){ // Does the table exist
    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }
  
  if(!aMaterialPropertiesTable->ConstPropertyExists("DE_PAIRENERGY") ||
     !aMaterialPropertiesTable->ConstPropertyExists("DE_AMPLIFICATION") ||
     !aMaterialPropertiesTable->ConstPropertyExists("DE_YIELD") ||
     !aMaterialPropertiesTable->ConstPropertyExists("DE_DRIFTSPEED") ||
     !aMaterialPropertiesTable->ConstPropertyExists("DE_TRANSVERSALSPREAD") ||
     !aMaterialPropertiesTable->ConstPropertyExists("DE_LONGITUDINALSPREAD"))
    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
  
  G4double pair_energy = 0;
  pair_energy =
    aMaterialPropertiesTable->GetConstProperty("DE_PAIRENERGY");

  G4double amplification=0; 
  amplification=
    aMaterialPropertiesTable->GetConstProperty("DE_AMPLIFICATION");
  G4double Yield = 0;
  Yield=
    aMaterialPropertiesTable->GetConstProperty("DE_YIELD");

  G4int number_electron = amplification*Yield;
    //if no electron leave
  if(number_electron<1){
    aParticleChange.SetNumberOfSecondaries(0);
    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }

  aParticleChange.SetNumberOfSecondaries(number_electron);

  // Create the secondary tracks
  for(G4int i = 0 ; i < number_electron ; i++){
    // Random direction at creation
    G4double cost = 1-2*G4UniformRand();
    G4double theta = acos(cost);
    G4double phi = twopi*G4UniformRand();
    G4ThreeVector p;
    p.setRThetaPhi(1,theta,phi); 

    G4DynamicParticle* particle = new G4DynamicParticle(G4DriftElectron::DriftElectron(),p, pair_energy);
     G4Track* aSecondaryTrack = new G4Track(particle,t0,x0);

    aSecondaryTrack->SetTouchableHandle(
        aStep.GetPreStepPoint()->GetTouchableHandle());

    aSecondaryTrack->SetParentID(aTrack.GetTrackID());
    aSecondaryTrack->SetTouchableHandle(aStep.GetPreStepPoint()->GetTouchableHandle());
    aParticleChange.AddSecondary(aSecondaryTrack);
  }

  // The original track is left intact
  return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
}


// GetMeanFreePath
// ---------------
//
G4double G4DEAmplification::GetMeanFreePath(const G4Track& ,
    G4double ,
    G4ForceCondition* condition)
{
  *condition = StronglyForced;
  return DBL_MAX;
}
// GetMeanLifeTime
// ---------------
//
G4double G4DEAmplification::GetMeanLifeTime(const G4Track& ,
    G4ForceCondition* condition)
{
  *condition = StronglyForced;
  return DBL_MAX;
}
