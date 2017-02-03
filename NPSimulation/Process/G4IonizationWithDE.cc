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
// $Id: G4IonizationWithDE.hh 98002 2016-06-30 13:03:36Z gcosmo $
//
// 
////////////////////////////////////////////////////////////////////////
// IonizationWithDE Light Class Definition 
////////////////////////////////////////////////////////////////////////
//
// File:        G4IonizationWithDE.hh  
// Description:	Discrete Process - Generation of Drift Electron 
// Version:     1.0
// Created:     2016-12-19
// Author:      Adrien Matta 
// Updated:     
// 
//
// mail:        matta@lpccaen.in2p3.fr
//
////////////////////////////////////////////////////////////////////////

#include "G4ios.hh"
#include "globals.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTypes.hh"
#include "G4EmProcessSubType.hh"

#include "G4IonizationWithDE.hh"
#include <iostream>
using namespace std;
/////////////////////////
// Class Implementation
/////////////////////////

//////////////////////
// static data members
//////////////////////


//G4bool G4IonizationWithDE::fTrackSecondariesFirst = false;
//G4bool G4IonizationWithDE::fIonizationWithDEByParticleType = false;
//G4bool G4IonizationWithDE::fStackingFlag = true;
//G4EmSaturation* G4IonizationWithDE::fEmSaturation = NULL;

//////////////
// Operators
//////////////

// G4IonizationWithDE::operator=(const G4IonizationWithDE &right)
// {
// }

/////////////////
// Constructors
/////////////////

G4IonizationWithDE::G4IonizationWithDE(const G4String& processName,
    G4ProcessType type)
  : G4VRestDiscreteProcess(processName, type) ,
  fTrackSecondariesFirst(false),
  fStackingFlag(true),
  fNumDriftElectron(1e6),
  fEmSaturation(nullptr)
{
  SetProcessSubType(fIonisation); // left as it is

  if (verboseLevel>0) {
    G4cout << GetProcessName() << " is created " << G4endl;
  }
}

////////////////
// Destructors
////////////////

G4IonizationWithDE::~G4IonizationWithDE()
{
}

////////////
// Methods
////////////

void G4IonizationWithDE::BuildPhysicsTable(const G4ParticleDefinition&)
{
}


// AtRestDoIt
// ----------
//
  G4VParticleChange*
G4IonizationWithDE::AtRestDoIt(const G4Track& aTrack, const G4Step& aStep)

  // This routine simply calls the equivalent PostStepDoIt since all the
  // necessary information resides in aStep.GetTotalEnergyDeposit()

{
  return G4IonizationWithDE::PostStepDoIt(aTrack, aStep);
}

// PostStepDoIt
// -------------
//
  G4VParticleChange*
G4IonizationWithDE::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)

  // This routine is called for each tracking step of a charged particle
  // in a drift chamber gas. an electron is created for each pair energy deposited in 
  // the material. 

{
  // Get the primary track
  aParticleChange.Initialize(aTrack);

  const G4Material* aMaterial = aTrack.GetMaterial();

  G4StepPoint* pPreStepPoint  = aStep.GetPreStepPoint();

  G4ThreeVector x0 = pPreStepPoint->GetPosition();
  G4ThreeVector p0 = aStep.GetDeltaPosition().unit();
  G4double      t0 = pPreStepPoint->GetGlobalTime();

  G4double TotalEnergyDeposit = aStep.GetTotalEnergyDeposit();

  // Get the material table
  G4MaterialPropertiesTable* aMaterialPropertiesTable =
    aMaterial->GetMaterialPropertiesTable();
  if (!aMaterialPropertiesTable)
    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
  

  if(!aMaterialPropertiesTable->ConstPropertyExists("DE_PAIRENERGY") ||
     !aMaterialPropertiesTable->ConstPropertyExists("DE_YIELD") ||
     !aMaterialPropertiesTable->ConstPropertyExists("DE_TRANSVERSALSPREAD") ||
     !aMaterialPropertiesTable->ConstPropertyExists("DE_LONGITUDINALSPREAD") ||
     !aMaterialPropertiesTable->ConstPropertyExists("DE_DRIFTSPEED") )
    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
 

  G4double pair_energy=0; 
  pair_energy=
    aMaterialPropertiesTable->GetConstProperty("DE_PAIRENERGY");
  G4double IonizationWithDEYield = 0;
  IonizationWithDEYield=
    aMaterialPropertiesTable->GetConstProperty("DE_YIELD");

  G4int number_electron = IonizationWithDEYield*TotalEnergyDeposit/pair_energy;
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

    // Random Position along the step with matching time
    G4double rand = G4UniformRand();
    G4ThreeVector pos = x0 + rand * aStep.GetDeltaPosition();
    G4double time = t0+ rand* aStep.GetDeltaTime(); 

    G4DynamicParticle* particle = new G4DynamicParticle(G4DriftElectron::DriftElectron(),p, pair_energy);
     G4Track* aSecondaryTrack = new G4Track(particle,time,pos);

    aSecondaryTrack->SetTouchableHandle(
        aStep.GetPreStepPoint()->GetTouchableHandle());

    aSecondaryTrack->SetParentID(aTrack.GetTrackID());
    aSecondaryTrack->SetTouchableHandle(aStep.GetPreStepPoint()->GetTouchableHandle());
    aParticleChange.AddSecondary(aSecondaryTrack);
  }

  if (fTrackSecondariesFirst) {
    if (aTrack.GetTrackStatus() == fAlive )
      aParticleChange.ProposeTrackStatus(fSuspend);
  }
  return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
}

// BuildThePhysicsTable for the scintillation process
// --------------------------------------------------
//

void G4IonizationWithDE::BuildThePhysicsTable()
{
}

// GetMeanFreePath
// ---------------
//

G4double G4IonizationWithDE::GetMeanFreePath(const G4Track&,
    G4double ,
    G4ForceCondition* condition)
{
  *condition = StronglyForced;

  return DBL_MAX;

}

// GetMeanLifeTime
// ---------------
//

G4double G4IonizationWithDE::GetMeanLifeTime(const G4Track&,
    G4ForceCondition* condition)
{
  *condition = StronglyForced;

  return DBL_MAX;

}

