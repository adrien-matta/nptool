/*****************************************************************************
 * Copyright (C) 2009-2015   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    : October 2015                                             *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  Modular Physics list calling Geant4 reference list                       *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "G4EmConfigurator.hh"
#include "globals.hh"
#include <string>
class G4VPhysicsConstructor;

class PhysicsList: public G4VModularPhysicsList{
  public:

    PhysicsList();
    virtual ~PhysicsList();

    void ReadConfiguration(std::string filename);
    void ConstructParticle();
    void SetCuts();
    void SetCutForGamma(G4double);
    void SetCutForElectron(G4double);
    void SetCutForPositron(G4double);
    void SetDetectorCut(G4double cut);
    void ConstructProcess();
    void AddStepMax();
    void AddPackage(const G4String& name);

  private:
    std::vector<G4VPhysicsConstructor*>  m_PhysList;
    G4EmConfigurator em_config;

  private: // Cuts
    G4double cutForGamma;
    G4double cutForElectron;
    G4double cutForPositron;
    G4String      emName;
    G4VPhysicsConstructor* emPhysicsList;
    G4VPhysicsConstructor* decay_List;
    G4VPhysicsConstructor* radioactiveDecay_List;

  private: // Physics option
    bool m_IonBinaryCascadePhysics;
    bool m_EmExtraPhysics;
    bool m_HadronElasticPhysics;
    bool m_StoppingPhysics;
    bool m_OpticalPhysics; 
    bool m_HadronPhysicsQGSP_BIC_HP; 
    bool m_Decay; 
    
    
};

#endif
