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
//---------------------------------------------------------------------------
//
// ClassName:   G4DriftElectronPhysics
//
// Author:     Adrie Matta 2016-12-19 
//
// Modified:    
// 
//
//----------------------------------------------------------------------------
//

#include "G4DriftElectronPhysics.hh"

#include "G4DEAbsorption.hh"
#include "G4IonizationWithDE.hh"
#include "G4DEAmplification.hh"
#include "G4DETransport.hh"
#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"
#include<iostream>
// factory
#include "G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(G4DriftElectronPhysics);


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4DriftElectronPhysics::G4DriftElectronPhysics(G4int verbose, const G4String& name)
  : G4VPhysicsConstructor(name),

    fMaxNumDriftElectron(100),
    fMaxBetaChange(10.0),
    fYieldFactor(1.)
{
  verboseLevel = verbose;

  for ( G4int i=0; i<kDENoProcess; i++ ) {
    fProcessUse.push_back(true);
    fProcessTrackSecondariesFirst.push_back(true);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4DriftElectronPhysics::~G4DriftElectronPhysics()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4DriftElectronPhysics::PrintStatistics() const
{
// Print all processes activation and their parameters

  for ( G4int i=0; i<kDENoProcess; i++ ) {
    G4cout << "  " << G4DriftElectronProcessName(i) << " process:  ";
    if ( ! fProcessUse[i] ) {
      G4cout << "not used" << G4endl;
    }
    else {
      G4cout << "used" << G4endl;
      if ( i == kDEIonization ) {
        if ( fProcessTrackSecondariesFirst[kDEIonization] ) {
          G4cout << "    Track secondaries first:  activated" << G4endl;
        }
        else {  
          G4cout << "    Track secondaries first:  inactivated" << G4endl;
        }
      }
      if ( i == kDEAmplification ) {
        if ( fProcessTrackSecondariesFirst[kDEAmplification] ) {
          G4cout << "    Track secondaries first:  activated" << G4endl;
        }
        else {  
          G4cout << "    Track secondaries first:  inactivated" << G4endl;
        }
      }
      if ( i == kDETransport) {
        if ( fProcessTrackSecondariesFirst[kDETransport] ) {
          G4cout << "    Track secondaries first:  activated" << G4endl;
        }
        else {  
          G4cout << "    Track secondaries first:  inactivated" << G4endl;
        }
      }
      if ( i == kDEAbsorption ) {
        if ( fProcessTrackSecondariesFirst[kDEAbsorption] ) {
          G4cout << "    Track secondaries first:  activated" << G4endl;
        }
        else {  
          G4cout << "    Track secondaries first:  inactivated" << G4endl;
        }
      }
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4DriftElectron.hh"

void G4DriftElectronPhysics::ConstructParticle()
{
/// Instantiate particles.

  // Drift Electron 
  G4DriftElectron::DriftElectronDefinition();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Threading.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4AutoDelete.hh"
#include<iostream>
using namespace std;
void G4DriftElectronPhysics::ConstructProcess()
{
// Construct drift electron processes.

  if(verboseLevel>0)
         G4cout <<"G4DriftElectronPhysics:: Add DriftElectron Physics Processes"<< G4endl;
  
// A vector of optical processes
  std::vector<G4VProcess*> DEProcesses;

  for ( G4int i=0; i<kDENoProcess; i++ ) DEProcesses.push_back(NULL);

  // Add DriftElectron Processes
  G4IonizationWithDE* DEIonizationProcess  = new G4IonizationWithDE();
  DEProcesses[kDEIonization] = DEIonizationProcess;

  G4DEAbsorption* DEAbsorptionProcess  = new G4DEAbsorption();
  DEProcesses[kDEAbsorption] = DEAbsorptionProcess;

  G4DETransport * DETransportProcess  = new G4DETransport();
  DEProcesses[kDETransport] = DETransportProcess;

  G4DEAmplification* DEAmplificationProcess  = new G4DEAmplification();
  DEProcesses[kDEAmplification] = DEAmplificationProcess;

  G4ProcessManager * pManager = 0;
  pManager = G4DriftElectron::DriftElectron()->GetProcessManager();

  if (!pManager) {
     std::ostringstream o;
     o << "Drift electron without a Process Manager";
     G4Exception("G4DriftElectronPhysics::ConstructProcess()","",
                  FatalException,o.str().c_str());
     return;
  }

  for ( G4int i=kDEIonization; i<=kDEAbsorption; i++ ) {
      if ( fProcessUse[i] ) {
         pManager->AddDiscreteProcess(DEProcesses[i]);
      }
  }

  G4IonizationWithDE* IonizationProcess = new G4IonizationWithDE();
  IonizationProcess->SetTrackSecondariesFirst(fProcessTrackSecondariesFirst[kDEIonization]);
  G4EmSaturation* emSaturation = G4LossTableManager::Instance()->EmSaturation();
  IonizationProcess->AddSaturation(emSaturation);
  DEProcesses[kDEIonization] = IonizationProcess;

  auto myParticleIterator=GetParticleIterator();
  myParticleIterator->reset();

  while( (*myParticleIterator)() ){

    G4ParticleDefinition* particle = myParticleIterator->value();
    G4String particleName = particle->GetParticleName();

    pManager = particle->GetProcessManager();
    if (!pManager) {
       std::ostringstream o;
       o << "Particle " << particleName << "without a Process Manager";
       G4Exception("G4DriftElectronPhysics::ConstructProcess()","",
                    FatalException,o.str().c_str());
       return;                 // else coverity complains for pManager use below	    
    }

    if( IonizationProcess->IsApplicable(*particle) &&
        fProcessUse[kDEIonization] ){
          pManager->AddProcess(IonizationProcess);
          pManager->SetProcessOrderingToLast(IonizationProcess,idxAtRest);
          pManager->SetProcessOrderingToLast(IonizationProcess,idxPostStep);
    }

  }

  // Add verbose
  for ( G4int i=0; i<kDENoProcess; i++ ) {
    if ( fProcessUse[i] ) DEProcesses[i]->SetVerboseLevel(verboseLevel);
  }

    if (verboseLevel > 1) PrintStatistics();
  if (verboseLevel > 0)
    G4cout << "### " << namePhysics << " physics constructed." << G4endl;
}

void G4DriftElectronPhysics::SetMaxNumDriftElectronPerStep(G4int maxNumDriftElectron)
{
/// Limit step to the specified maximum number of Cherenkov photons

  fMaxNumDriftElectron = maxNumDriftElectron;
}

void G4DriftElectronPhysics::SetMaxBetaChangePerStep(G4double maxBetaChange)
{
/// Limit step to the specified maximum change of beta of the parent particle

  fMaxBetaChange = maxBetaChange;
}

void G4DriftElectronPhysics::SetTrackSecondariesFirst(G4DriftElectronProcessIndex index,
                                                G4bool trackSecondariesFirst)
{
  if ( index >= kDENoProcess ) return;
  if ( fProcessTrackSecondariesFirst[index] == trackSecondariesFirst ) return;
  fProcessTrackSecondariesFirst[index] = trackSecondariesFirst;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
