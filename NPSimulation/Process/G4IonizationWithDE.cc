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
#include "G4ElectroMagneticField.hh"
#include "G4FieldManager.hh"
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
  // Get the material table
  G4MaterialPropertiesTable* aMaterialPropertiesTable =
    aMaterial->GetMaterialPropertiesTable();
  if (!aMaterialPropertiesTable)
    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
  

  if(!aMaterialPropertiesTable->ConstPropertyExists("DE_PAIRENERGY") ||
     !aMaterialPropertiesTable->ConstPropertyExists("DE_TRANSVERSALSPREAD") ||
     !aMaterialPropertiesTable->ConstPropertyExists("DE_LONGITUDINALSPREAD") ||
     !aMaterialPropertiesTable->ConstPropertyExists("DE_DRIFTSPEED") )
    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
 

  G4StepPoint* pPreStepPoint  = aStep.GetPreStepPoint();
  const G4TouchableHandle& handle = pPreStepPoint->GetTouchableHandle();

  G4ThreeVector x0 = pPreStepPoint->GetPosition();
  G4ThreeVector p0 = aStep.GetDeltaPosition().unit();
  G4double      t0 = pPreStepPoint->GetGlobalTime();

  G4double TotalEnergyDeposit = aStep.GetTotalEnergyDeposit();

  
  G4double v_drift=
    aMaterialPropertiesTable->GetConstProperty("DE_DRIFTSPEED");
  G4double pair_energy=
    aMaterialPropertiesTable->GetConstProperty("DE_PAIRENERGY");

  // Physical number of electron produced
  G4int number_electron = TotalEnergyDeposit/pair_energy;
  number_electron = G4Poisson(number_electron);

  // Tracked electron produced
  // 100 per mm per step to have a good statistical accuracy
  
  G4int tracked_electron = (aStep.GetStepLength()/mm)*5;

  //if no electron leave
  if(tracked_electron<1){
    aParticleChange.SetNumberOfSecondaries(0);
    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
  }

  aParticleChange.SetNumberOfSecondaries(tracked_electron);
  aParticleChange.SetSecondaryWeightByProcess(true);

  // Electron follow the field direction
  // The field direction is taken from the field manager
  //Everything common to all created DE:
  static G4double  fieldArr[6];
  static G4double  Point[4];
  Point[0] = x0.x();
  Point[1] = x0.y();
  Point[2] = x0.z();
  Point[3] = t0;

  G4FieldManager* fMng = handle->GetVolume()->GetLogicalVolume()->GetFieldManager();

  G4ElectroMagneticField* field = (G4ElectroMagneticField*)fMng->GetDetectorField();
  field->GetFieldValue(Point,&fieldArr[0]) ;

  // Electron move opposite to the field direction, hance the minus sign
  G4ThreeVector p(-fieldArr[3],-fieldArr[4],-fieldArr[5]);
  // Normalised the drift direction
  p = p.unit();

  G4ThreeVector delta = aStep.GetDeltaPosition();
  G4double     deltaT = aStep.GetDeltaTime();
  static G4ParticleDefinition* DEDefinition = G4DriftElectron::DriftElectron();
  G4double velocity = v_drift/c_light;
  G4int parentID = aTrack.GetTrackID();
  
  // Create the secondary tracks
  for(G4int i = 0 ; i < tracked_electron ; i++){ // always create 100 electron, but with weight
      // Random Position along the step with matching time
    G4double rand = G4UniformRand();
    G4ThreeVector pos = x0 + rand * delta;
    G4double time = t0 + rand* deltaT; 

    G4DynamicParticle* particle = new G4DynamicParticle(DEDefinition,p, pair_energy);
    G4Track* aSecondaryTrack = new G4Track(particle,time,pos);
    aSecondaryTrack->SetVelocity(velocity);
    aSecondaryTrack->SetParentID(parentID);
    aSecondaryTrack->SetTouchableHandle(handle);
    // Set the weight, ie, how many electron the track represents 
    aSecondaryTrack->SetWeight(number_electron/tracked_electron);
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

