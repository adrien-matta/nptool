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
#include <map>

#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4IonConstructor.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmExtraPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4DecayPhysics.hh"

class G4VPhysicsConstructor;

class PhysicsList: public G4VModularPhysicsList{
  public:

    PhysicsList();
    virtual ~PhysicsList();

    void ReadConfiguration(std::string filename);
    void ConstructParticle();
    void SetCuts();
    void ConstructProcess();
    void AddStepMax();
    void AddPackage(const G4String& name);
    void BiasCrossSectionByFactor(double factor);
  

  private:
    std::map<std::string,G4VPhysicsConstructor*>  m_PhysList;
    G4EmConfigurator em_config;

  private: // Cuts
    G4OpticalPhysics* opticalPhysicsList;
    G4VPhysicsConstructor* emPhysicsList;
    G4VPhysicsConstructor* decay_List;
    G4VPhysicsConstructor* radioactiveDecay_List;

  private: // Physics option
    double m_IonBinaryCascadePhysics;
    double m_EmExtraPhysics;
    double m_HadronElasticPhysics;
    double m_StoppingPhysics;
    double m_OpticalPhysics; 
    double m_HadronPhysicsQGSP_BIC_HP; 
    double m_Decay; 
    
    
};


#endif
