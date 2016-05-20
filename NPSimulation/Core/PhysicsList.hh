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
#include "G4EmProcessOptions.hh"

#include "globals.hh"
#include <string>
#include <map>

// Particle definition
#include "G4IonConstructor.hh"

// Decay
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

// EM
#include "G4EmProcessOptions.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmExtraPhysics.hh"
#include "G4StoppingPhysics.hh"

//Optical
#include "G4OpticalPhysics.hh"

//Hadronique
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4IonBinaryCascadePhysics.hh"
#if NPS_GEANT4_VERSION_MAJOR > 9
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#endif
#include "G4HadronPhysicsQGSP_BIC.hh"
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
    G4EmProcessOptions em_option;

  private: // Physics List
    G4OpticalPhysics* opticalPhysicsList;
    G4VPhysicsConstructor* emPhysicsList;
    G4VPhysicsConstructor* decay_List;
    G4VPhysicsConstructor* radioactiveDecay_List;
    
  private: // Physics option
    std::string m_EmList;
    double m_IonBinaryCascadePhysics;
    double m_EmExtraPhysics;
    double m_HadronElasticPhysics;
    double m_StoppingPhysics;
    double m_OpticalPhysics; 
    double m_HadronPhysicsQGSP_BIC_HP; 
    double m_Decay; 
};


#endif
