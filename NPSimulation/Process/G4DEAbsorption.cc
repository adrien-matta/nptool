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
// $Id: G4DEAbsorption.hh 69576 2016-12-19 13:48:13Z gcosmo $
//
////////////////////////////////////////////////////////////////////////
// Optical DriftElectron Absorption Implementation 
////////////////////////////////////////////////////////////////////////
//
// File:        G4DEAbsorption.hh
// Description: Discrete Process -- Bulk absorption of Drift Electron
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

#include "G4DEAbsorption.hh"

/////////////////////////
// Class Implementation
/////////////////////////

//////////////
// Operators
//////////////

// G4DEAbsorption::operator=(const G4DEAbsorption &right)
// {
// }

/////////////////
// Constructors
/////////////////

G4DEAbsorption::G4DEAbsorption(const G4String& processName, G4ProcessType type)
  : G4VDiscreteProcess(processName, type)
{
  if (verboseLevel>0) {
    G4cout << GetProcessName() << " is created " << G4endl;
  }

  SetProcessSubType(fDEAbsorption);
}

// G4DEAbsorption::G4DEAbsorption(const G4OpAbsorpton &right)
// {
// }

////////////////
// Destructors
////////////////

G4DEAbsorption::~G4DEAbsorption(){}

////////////
// Methods
////////////

// PostStepDoIt
// -------------
//
  G4VParticleChange*
G4DEAbsorption::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
{
  aParticleChange.Initialize(aTrack);

  const G4DynamicParticle* aParticle = aTrack.GetDynamicParticle();
  G4double theDriftElectronMomentum = aParticle->GetTotalMomentum();

  aParticleChange.ProposeLocalEnergyDeposit(theDriftElectronMomentum);
  aParticleChange.ProposeTrackStatus(fStopAndKill);
  if (verboseLevel>0) {
    G4cout << "\n** Drift electron absorbed! **" << G4endl;
  }
  return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
}


// GetMeanFreePath
// ---------------
//
G4double G4DEAbsorption::GetMeanFreePath(const G4Track& aTrack,
    G4double ,
    G4ForceCondition* )
{
  const G4Material* aMaterial = aTrack.GetMaterial();

  G4MaterialPropertiesTable* aMaterialPropertyTable;

  // If none provided, the absorbtion is maximum
  G4double AttenuationLength = 0;

  aMaterialPropertyTable = aMaterial->GetMaterialPropertiesTable();

  if ( aMaterialPropertyTable ){
    if( aMaterialPropertyTable->ConstPropertyExists("DE_ABSLENGTH"))
      AttenuationLength = aMaterialPropertyTable->GetConstProperty("DE_ABSLENGTH");
  }
  return AttenuationLength;
}
