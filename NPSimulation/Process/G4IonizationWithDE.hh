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

#ifndef G4IonizationWithDE_h
#define G4IonizationWithDE_h 1

/////////////
// Includes
/////////////

#include "globals.hh"
#include "templates.hh"
#include "Randomize.hh"
#include "G4Poisson.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleMomentum.hh"
#include "G4Step.hh"
#include "G4VRestDiscreteProcess.hh"
#include "G4DriftElectron.hh"
#include "G4DynamicParticle.hh"
#include "G4Material.hh" 
#include "G4PhysicsTable.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4PhysicsOrderedFreeVector.hh"

#include "G4EmSaturation.hh"
#include<iostream>
// Class Description:
// RestDiscrete Process - Generation of IonizationWithDE Photons.
// Class inherits publicly from G4VRestDiscreteProcess.
// Class Description - End:

/////////////////////
// Class Definition
/////////////////////

class G4IonizationWithDE : public G4VRestDiscreteProcess
{

public:

	////////////////////////////////
	// Constructors and Destructor
	////////////////////////////////

	explicit G4IonizationWithDE(const G4String& processName = "IonizationWithDE",
                                 G4ProcessType type = fElectromagnetic);
	~G4IonizationWithDE();

private:

        G4IonizationWithDE(const G4IonizationWithDE &right) = delete;

        //////////////
        // Operators
        //////////////

        G4IonizationWithDE& operator=(const G4IonizationWithDE &right) = delete;

public:

        ////////////
        // Methods
        ////////////

        // G4IonizationWithDE Process has both PostStepDoIt (for energy 
        // deposition of particles in flight) and AtRestDoIt (for energy
        // given to the medium by particles at rest)

        G4bool IsApplicable(
          const G4ParticleDefinition& aParticleType) override;
        // Returns true -> 'is applicable', for any particle type except
        // for an 'driftelectron' and for short-lived particles

        void BuildPhysicsTable(
          const G4ParticleDefinition& aParticleType) override;
        // Build table at the right time

        G4double GetMeanFreePath(const G4Track& aTrack,
                                       G4double ,
                                       G4ForceCondition* ) override;
        // Returns infinity; i. e. the process does not limit the step,
        // but sets the 'StronglyForced' condition for the DoIt to be 
        // invoked at every step.

        G4double GetMeanLifeTime(const G4Track& aTrack,
                                 G4ForceCondition* ) override;
        // Returns infinity; i. e. the process does not limit the time,
        // but sets the 'StronglyForced' condition for the DoIt to be
        // invoked at every step.

        G4VParticleChange* PostStepDoIt(const G4Track& aTrack, 
                            const G4Step&  aStep) override;
        G4VParticleChange* AtRestDoIt (const G4Track& aTrack,
                                       const G4Step& aStep) override;

        // These are the methods implementing the drift electron process.

        void SetTrackSecondariesFirst(const G4bool state);
        // If set, the primary particle tracking is interrupted and any
        // produced drift electron are tracked next. When all 
        // have been tracked, the tracking of the primary resumes.

        G4bool GetTrackSecondariesFirst() const;
        // Returns the boolean flag for tracking secondaries first.

        G4PhysicsTable* GetIntegralTable() const;
        // Returns the address of the ionisation integral table.

        void AddSaturation(G4EmSaturation* sat);
        // Adds Birks Saturation to the process.

        void RemoveSaturation();
        // Removes the Birks Saturation from the process.

        G4EmSaturation* GetSaturation() const;
        // Returns the Birks Saturation.

        void SetStackDriftElectron(const G4bool );
        // Call by the user to set the flag for stacking the drift electron

        G4bool GetStackDriftElectron() const;
        // Return the boolean for whether or not the drift electron are stacked

        G4int GetNumDriftElectron() const;
        // Returns the current number of drift electron (after PostStepDoIt)

        void DumpPhysicsTable() const;
        // Prints the drift electron integral tables.

protected:

        void BuildThePhysicsTable();
        // It builds either the drift electronintegral table; 

        ///////////////////////
        // Class Data Members
        ///////////////////////

private:

        G4bool fTrackSecondariesFirst;
        G4bool fStackingFlag;
        G4int fNumDriftElectron;

        G4double single_exp(G4double t, G4double tau2);
        G4double bi_exp(G4double t, G4double tau1, G4double tau2);
        G4EmSaturation* fEmSaturation;

};

////////////////////
// Inline methods
////////////////////

inline 
G4bool G4IonizationWithDE::IsApplicable(const G4ParticleDefinition& aParticleType)
{
       if (aParticleType.GetParticleName() == "driftelectron") return false;
       return true;
}

inline
void G4IonizationWithDE::SetTrackSecondariesFirst(const G4bool state)
{
        fTrackSecondariesFirst = state;
}

inline
G4bool G4IonizationWithDE::GetTrackSecondariesFirst() const
{
        return fTrackSecondariesFirst;
}

inline
void G4IonizationWithDE::AddSaturation(G4EmSaturation* sat)
{
        fEmSaturation = sat;
}

inline
void G4IonizationWithDE::RemoveSaturation()
{
        fEmSaturation = nullptr;
}

inline
G4EmSaturation* G4IonizationWithDE::GetSaturation() const
{
        return fEmSaturation;
}

inline
void G4IonizationWithDE::SetStackDriftElectron(const G4bool stackingFlag)
{
        fStackingFlag = stackingFlag;
}

inline
G4bool G4IonizationWithDE::GetStackDriftElectron() const
{
        return fStackingFlag;
}

inline
G4int G4IonizationWithDE::GetNumDriftElectron() const
{
        return fNumDriftElectron;
}


#endif /* G4IonizationWithDE_h */
