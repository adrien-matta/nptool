/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  A quite standard, non-modulable Geant4 PPhysicis list.                   *
 *  Well suited for low energy ions physics.                                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * A good improvement should be a modular physicis list in order to deal     *
 * accuratly with different physics cases.                                   *
 *****************************************************************************/
#include "PhysicsList.hh"

// I/O
#include "G4ios.hh"
#include <iomanip>

//Particle Definition
#include "G4ParticleTypes.hh"
#include "G4IonConstructor.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"


//Process
#include "G4Transportation.hh"

// Standard Geant4
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuMultipleScattering.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"
#include "G4hMultipleScattering.hh"

#include "G4EmProcessOptions.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"

// Cut
#include "G4ParticleWithCuts.hh"
#include "G4UserSpecialCuts.hh"

// Decay
#include "G4Decay.hh"
#include "G4DecayTable.hh"
#include "G4VDecayChannel.hh"
#include "G4NuclearDecayChannel.hh"
#include "G4BetaMinusDecayChannel.hh"


//#include "G4Gamma.hh"
//#include "G4Electron.hh"
//#include "G4Positron.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PhysicsList::PhysicsList(){
  // ie: no secondaries
  defaultCutValue = 1000 * pc;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PhysicsList::~PhysicsList(){
  ;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PhysicsList::ConstructParticle(){
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program.
  
  //Usefull to test geometry
  G4Geantino::GeantinoDefinition();
  
  //Usefull for Gamma
  ConstructBosons();
  
  //Usefull for betaDecay
  ConstructLeptons();
  
  //Needed by G4 (4.9.2) to run on mac os X ;-)
  ConstructMesons();
  
  //usefull for p and n
  ConstructBaryons();
  
  //Usefull of course :p
  ConstructIons();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PhysicsList::ConstructBosons(){
  G4Geantino::GeantinoDefinition()              ;
  G4ChargedGeantino::ChargedGeantinoDefinition()   ;
  G4Gamma::GammaDefinition()                 ;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PhysicsList::ConstructLeptons(){
  G4Electron::ElectronDefinition()              ;
  G4Positron::PositronDefinition()              ;
  G4NeutrinoE::NeutrinoEDefinition()            ;
  G4AntiNeutrinoE::AntiNeutrinoEDefinition()    ;
  //G4NeutrinoMu::NeutrinoMuDefinition()        ;
  //G4AntiNeutrinoMu::AntiNeutrinoMuDefinition()   ;
  //G4MuonPlus::MuonPlusDefinition()            ;
  //G4MuonMinus::MuonMinusDefinition()          ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PhysicsList::ConstructBaryons(){
  G4Proton::ProtonDefinition()   ;
  G4AntiProton::AntiProtonDefinition()   ;
  G4Neutron::NeutronDefinition();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PhysicsList::ConstructMesons(){
  //  mesons
  G4PionPlus     ::PionPlusDefinition();
  G4PionMinus    ::PionMinusDefinition();
  G4PionZero     ::PionZeroDefinition();
  G4Eta          ::EtaDefinition();
  G4EtaPrime     ::EtaPrimeDefinition();
  //  G4RhoZero      ::RhoZeroDefinition();
  G4KaonPlus     ::KaonPlusDefinition();
  G4KaonMinus    ::KaonMinusDefinition();
  G4KaonZero     ::KaonZeroDefinition();
  G4AntiKaonZero ::AntiKaonZeroDefinition();
  G4KaonZeroLong ::KaonZeroLongDefinition();
  G4KaonZeroShort::KaonZeroShortDefinition();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PhysicsList::ConstructIons(){
  G4He3::He3Definition()         ;
  G4Deuteron::DeuteronDefinition()  ;
  G4Triton::TritonDefinition()      ;
  G4Alpha::AlphaDefinition()     ;
  
  G4IonConstructor iConstructor     ;
  iConstructor.ConstructParticle()  ;
  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PhysicsList::ConstructProcess(){
  AddTransportation()   ;
  ConstructEM()         ;
  
  SetCuts()          ;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PhysicsList::ConstructEM(){
  
  theParticleIterator->reset();
  
  while ((*theParticleIterator)()) {
    G4ParticleDefinition* particle = theParticleIterator->value() ;
    G4ProcessManager* pmanager = particle->GetProcessManager() ;
    G4String particleName = particle->GetParticleName() ;
    
    if (particleName == "gamma") {
      // gamma
      //standard Geant4
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect) ;
      pmanager->AddDiscreteProcess(new G4ComptonScattering) ;
      pmanager->AddDiscreteProcess(new G4GammaConversion) ;
    }
    else if (particleName == "e-") {
      //electron
      pmanager->AddProcess(new G4eMultipleScattering  , -1,  1, 1)     ;
      //standard geant4:
      pmanager->AddProcess(new G4eIonisation         , -1,  2, 2)     ;
      pmanager->AddProcess(new G4eBremsstrahlung     , -1, -1, 3)     ;
    }
    
    else if (particleName == "e+") {
      //positron
      pmanager->AddProcess(new G4eMultipleScattering  , -1,  1, 1 );
      // standard Geant4 and Low energy
      pmanager->AddProcess(new G4eIonisation         , -1,  2, 2 );
      pmanager->AddProcess(new G4eBremsstrahlung     , -1, -1, 3 );
      pmanager->AddProcess(new G4eplusAnnihilation   ,  0, -1, 4 );
    }
    
    else if (particleName == "mu+" ||
               particleName == "mu-") {
    }
    
    else if ( particleName == "GenericIon" 
              || particleName == "He3"
              || particleName == "alpha"
              || particleName == "deuteron"
              || particleName == "triton") {
      pmanager->AddProcess(new G4hMultipleScattering(), -1, 1, 1) ;
      G4ionIonisation* iI = new G4ionIonisation ;
      iI->ActivateStoppingData(true) ;
      pmanager->AddProcess(iI , -1, 2, 2) ;
    }
    
    else if ((!particle->IsShortLived())     &&
               (particle->GetPDGCharge() != 0.0)   &&
               (particleName != "chargedgeantino")) {
      
      G4hIonisation* hI = new G4hIonisation ;
      pmanager->AddProcess(new G4hMultipleScattering , -1, 1, 1) ;
      pmanager->AddProcess(hI , -1, 2, 2) ;
      
      
    }
  }
  
  G4EmProcessOptions opt        ;
  opt.SetSubCutoff(true)        ;
  opt.SetMinEnergy(0.001*eV)    ;
  opt.SetMaxEnergy(50.*GeV)    ;
  opt.SetDEDXBinning(5000)       ;
  opt.SetLambdaBinning(5000)     ;
    
    //energy loss
    opt.SetLinearLossLimit(1.e-9);
    opt.SetStepFunction(0.001, 10.*um);
    
    //Multiple scattering
    opt.SetMscLateralDisplacement(true);
    opt.SetLossFluctuations(true);
    //opt.SetMscStepLimitation(fMinimal);
    //opt.SetMscStepLimitation(fUseDistanceToBoundary);
    opt.SetMscStepLimitation(fUseSafety);
    
    //ionization
    opt.SetSubCutoff(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PhysicsList::SetCuts(){
  // uppress error messages even in case e/gamma/proton do not exist
  G4int temp = GetVerboseLevel();
  SetVerboseLevel(0);
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets
  //   the default cut value for all particle types
  SetCutsWithDefault();
  
  // for gamma-rays
  /*  G4double em_cuts = 0.01*mm; // Use 10cm to avoid generating delta-rays
    //G4double em_cuts = 10.*cm; // Use 10cm to avoid generating delta-rays
    SetCutValue(em_cuts, "gamma");
    SetCutValue(em_cuts, "e-");
    SetCutValue(em_cuts, "e+");*/
  
  // Retrieve verbose level
  SetVerboseLevel(temp);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PhysicsList::ConstructDecay(){
  
  // Add Decay Process
  G4Decay* theDecayProcess = new G4Decay()                     ;
  theParticleIterator->reset()                              ;
  while ((*theParticleIterator)()) {
    G4ParticleDefinition*  particle = theParticleIterator->value()   ;
    G4ProcessManager*      pmanager = particle->GetProcessManager();
    if (theDecayProcess->IsApplicable(*particle)) {
      pmanager ->AddProcess(theDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager->SetProcessOrdering(theDecayProcess, idxAtRest);
    }
  }
  //end Add Decay Process
  
}

void PhysicsList::MyOwnConstruction(){
  ConstructDecay();
}



