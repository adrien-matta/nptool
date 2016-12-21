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
// $Id: G4DETransport.hh 69576 2016-12-19 13:48:13Z gcosmo $
//
////////////////////////////////////////////////////////////////////////
// Optical DriftElectron Transport Implementation 
////////////////////////////////////////////////////////////////////////
//
// File:        G4DETransport.cc
// Description: Discrete Process -- Bulk transport of Drift Electron
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
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4DETransport.hh"

/////////////////////////
// Class Implementation
/////////////////////////

//////////////
// Operators
//////////////

// G4DETransport::operator=(const G4DETransport &right)
// {
// }

/////////////////
// Constructors
/////////////////

G4DETransport::G4DETransport(const G4String& processName, G4ProcessType type)
  : G4VDiscreteProcess(processName, type)
{
  if (verboseLevel>0) {
    G4cout << GetProcessName() << " is created " << G4endl;
  }

  SetProcessSubType(fDETransport);
}

// G4DETransport::G4DETransport(const G4OpAbsorpton &right)
// {
// }

////////////////
// Destructors
////////////////

G4DETransport::~G4DETransport(){}

////////////
// Methods
////////////

// PostStepDoIt
// -------------
//
G4VParticleChange*
G4DETransport::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
{
  aParticleChange.Initialize(aTrack);
  G4StepPoint* pPreStepPoint  = aStep.GetPreStepPoint();

  G4ThreeVector x0 = pPreStepPoint->GetPosition();
  G4ThreeVector p0 = aStep.GetDeltaPosition().unit();
  G4double      t0 = pPreStepPoint->GetGlobalTime();

  G4double Energy = pPreStepPoint->GetKineticEnergy();

  // Get the material table
   const G4Material* aMaterial = aTrack.GetMaterial();

  G4MaterialPropertiesTable* aMaterialPropertiesTable =
    aMaterial->GetMaterialPropertiesTable();
  if (!aMaterialPropertiesTable)
    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  

  if(!aMaterialPropertiesTable->ConstPropertyExists("DE_PAIRENERGY") ||
     !aMaterialPropertiesTable->ConstPropertyExists("DE_YIELD") ||
     !aMaterialPropertiesTable->ConstPropertyExists("DE_TRANSVERSALSPREAD") ||
     !aMaterialPropertiesTable->ConstPropertyExists("DE_LONGITUDINALSPREAD") ||
     !aMaterialPropertiesTable->ConstPropertyExists("DE_DRIFTSPEED") )
    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
 

  // Electron follow the field direction
  G4ThreeVector driftDir(0,1,0);
  G4double v_drift = aMaterialPropertiesTable->GetConstProperty("DE_DRIFTSPEED"); 
  // The time scale is imposed by the drift speed
  G4double step = aStep.GetDeltaTime();
  if(!step)
    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);

  // Should be equal to delta pos y
  G4double step_length = step*v_drift;
  G4double sigmaTrans  = sqrt(2*step_length*aMaterialPropertiesTable->GetConstProperty("DE_TRANSVERSALSPREAD")/v_drift);
  G4double sigmaLong   = sqrt(2*step_length*aMaterialPropertiesTable->GetConstProperty("DE_LONGITUDINALSPREAD")/v_drift);

  G4double d_long  = G4RandGauss::shoot(0,sigmaLong);
  G4double d_trans = G4RandGauss::shoot(0,sigmaTrans);
    
  G4ThreeVector trans(G4RandGauss::shoot(0,d_trans),0,0);
  trans.rotateY(twopi*G4UniformRand());
  G4ThreeVector d_Pos = (step_length+d_long)*driftDir+trans;  
  // Random Position along the step with matching time
  G4ThreeVector pos = x0 + d_Pos;
  G4double time = t0 + step; 

  aParticleChange.ProposeMomentumDirection(d_Pos.unit());
  aParticleChange.ProposeEnergy(Energy);
  aParticleChange.ProposePosition(pos);
  aParticleChange.ProposeGlobalTime(time);

  return &aParticleChange;
}


// GetMeanFreePath
// ---------------
//
G4double G4DETransport::GetMeanFreePath(const G4Track& ,
    G4double ,
    G4ForceCondition* )
{
  return 0.5*mm;
}
