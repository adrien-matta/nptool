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

  const G4Material* aMaterial = aTrack.GetMaterial();


  // Get the material table
  G4MaterialPropertiesTable* aMaterialPropertiesTable =
    aMaterial->GetMaterialPropertiesTable();
  if (!aMaterialPropertiesTable){ // Does the table exist
    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }

  if(!aMaterialPropertiesTable->ConstPropertyExists("DE_AMPLIFICATION"))
    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);


  G4StepPoint* pPreStepPoint  = aStep.GetPreStepPoint();

  G4ThreeVector x0 = pPreStepPoint->GetPosition();
  G4ThreeVector p0 = aStep.GetDeltaPosition().unit();
  G4double      t0 = pPreStepPoint->GetGlobalTime();

  G4double pair_energy = pPreStepPoint->GetKineticEnergy();

  G4double amplification = aMaterialPropertiesTable->GetConstProperty("DE_AMPLIFICATION");


  double OriginalW = aTrack.GetWeight();
  // Number of Physical electron to be created
  G4int number_electron = amplification*OriginalW;
  //Number of tracked electron effectively created instead
  G4int tracked_electron = 10;

  //if no electron leave
  if(tracked_electron<1){
    aParticleChange.SetNumberOfSecondaries(0);
    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }

  static G4ParticleDefinition* DEDef = G4DriftElectron::DriftElectron();
  G4TouchableHandle handle = pPreStepPoint->GetTouchableHandle();
  G4int ParentID = aTrack.GetTrackID();

   aParticleChange.SetNumberOfSecondaries(tracked_electron);
  aParticleChange.SetSecondaryWeightByProcess(true);
  // Create the secondary tracks
  for(G4int i = 0 ; i < tracked_electron ; i++){
     G4DynamicParticle* particle = new G4DynamicParticle(DEDef,p0, pair_energy);
     // The secondary track follow the original one
     G4Track* aSecondaryTrack = new G4Track(particle,t0,x0);

    aSecondaryTrack->SetCreatorProcess(this);
    aSecondaryTrack->SetParentID(ParentID);
    aSecondaryTrack->SetTouchableHandle(handle);
    aSecondaryTrack->SetWeight(number_electron/tracked_electron);
    aParticleChange.AddSecondary(aSecondaryTrack);
  }
  // to kill the primary track to avoid it being re-amplified
  aParticleChange.ProposeEnergy(0);
  // The original track is left intact
  return &aParticleChange;
}


// GetMeanFreePath
// ---------------
//
G4double G4DEAmplification::GetMeanFreePath(const G4Track& aTrack,
    G4double ,
    G4ForceCondition* condition)
{
  if(aTrack.GetCreatorProcess()->GetProcessName()=="DEAmplification" ){
    *condition = NotForced;
    return DBL_MAX;
  }
  *condition = StronglyForced;
  return DBL_MAX;
}
// GetMeanLifeTime
// ---------------
//
G4double G4DEAmplification::GetMeanLifeTime(const G4Track& aTrack,
    G4ForceCondition* condition)
{
  if(aTrack.GetCreatorProcess()->GetProcessName()=="DEAmplification" ){
    *condition = NotForced;
    return DBL_MAX;
  }

  *condition = StronglyForced;
  return DBL_MAX;
}
