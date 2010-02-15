#ifndef PhysicsList_h
#define PhysicsList_h 1
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville contact address:  deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 15/02/2010                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  A quite standard, non-modulable Geant4 PPhysicis list.                   *
 *  Well suited for low energy ions physics.                                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *   This class is taken from the G4 tutorial advanced/hadrontherapy         *
 *                                                                           *
 *****************************************************************************/
#include "G4VModularPhysicsList.hh"
#include "G4EmConfigurator.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
//class StepMax;
class PhysicsListMessenger;

class PhysicsList: public G4VModularPhysicsList
{
public:

  PhysicsList();
  virtual ~PhysicsList();

  void ConstructParticle();
    
  void SetCuts();
  void SetCutForGamma(G4double);
  void SetCutForElectron(G4double);
  void SetCutForPositron(G4double);
        
  void AddPhysicsList(const G4String& name);
  void ConstructProcess();
    
//  void AddStepMax();       
//  StepMax* GetStepMaxProcess() {return stepMaxProcess;};
  void AddPackage(const G4String& name);

private:

  G4EmConfigurator em_config; 

  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForPositron;

  G4bool helIsRegisted;
  G4bool bicIsRegisted;
  G4bool biciIsRegisted;
  G4bool locIonIonInelasticIsRegistered;
    
  G4String                             emName;
  G4VPhysicsConstructor*               emPhysicsList;
  G4VPhysicsConstructor*               decPhysicsList;
  std::vector<G4VPhysicsConstructor*>  hadronPhys;
    
//  StepMax* stepMaxProcess;
    
  PhysicsListMessenger* pMessenger;
};

#endif

