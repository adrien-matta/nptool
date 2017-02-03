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
//
//---------------------------------------------------------------------------
//
// ClassName:   G4DriftElectronPhysics
//
// Author:      Adrien Matta 2016-12-19 
//
// Modified:    
//              
//
//---------------------------------------------------------------------------
//
// This class provides construction of default optical physics
//

#ifndef G4DriftElectronPhysics_h
#define G4DriftElectronPhysics_h 1

#include "globals.hh"

#include "G4VPhysicsConstructor.hh"

#include "G4DriftElectronProcessIndex.hh"

#include <vector>

class G4VProcess;
class G4EmSaturation;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4DriftElectronPhysics : public G4VPhysicsConstructor
{
  public:

    G4DriftElectronPhysics(G4int verbose = 0, const G4String& name = "DriftElectron");
    virtual ~G4DriftElectronPhysics();

  protected:

    // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:

    /// Not implemented
    G4DriftElectronPhysics(const G4DriftElectronPhysics& right);
    /// Not implemented
    G4DriftElectronPhysics& operator=(const G4DriftElectronPhysics& right);

  public:

    // configure G4DriftElectronPhysics builder
    void Configure(G4DriftElectronProcessIndex, G4bool );

    void SetMaxNumDriftElectronPerStep(G4int );
    void SetMaxBetaChangePerStep(G4double );
    void SetElectronYieldFactor(G4double );
    void SetTrackSecondariesFirst(G4DriftElectronProcessIndex, G4bool );

  private:

    // methods
    void PrintStatistics() const;

    // The vector of process configuration
    std::vector<G4bool>         fProcessUse;

    // The vector of track secondaries options;
    // the option to track secondaries before finishing their parent track
    std::vector<G4bool>         fProcessTrackSecondariesFirst;

    /// max number of Cerenkov photons per step
    G4int                       fMaxNumDriftElectron;

    /// max change of beta per step
    G4double                    fMaxBetaChange;

    /// scintillation yield factor
    G4double                    fYieldFactor;

    /// option to allow for G4ElectronTrackInformation
    /// to be attached to a drift electron's track
    G4bool                      fElectronTrackInfo;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif // G4DriftElectronPhysics_h
