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
 *  This class manage the event Generator used in simulation                 *
 *  Every time you had a new kind of event generator, you need to add a few  *
 *  line in the ReadConfiguration method to detect the associate Token       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
// STL
#include "math.h"
#include <cstdlib>

// G4
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

// NPL
#include "NPOptionManager.h"

// CLHEP
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Random/RandGeneral.h"
#include "PrimaryGeneratorAction.hh"
#include "PhysicsList.hh"
#include "Randomize.hh"
#include "ParticleStack.hh"

// Event Generator Class
#include "EventGeneratorTwoBodyReaction.hh"
#include "EventGeneratorIsotropic.hh"
#include "EventGeneratorBeam.hh"
#include "EventGeneratorGammaDecay.hh"
#include "EventGeneratorParticleDecay.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction::~PrimaryGeneratorAction(){
  for (unsigned int i = 0 ; i < m_EventGenerator.size(); i++) {
    delete m_EventGenerator[i];
  }
  m_EventGenerator.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* det): m_detector(det){
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  for (unsigned int i = 0 ; i < m_EventGenerator.size(); i++) {
    m_EventGenerator[i]->GenerateEvent(anEvent);
  }
  ParticleStack::getInstance()->ShootAllParticle(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::ReadEventGeneratorFile(string Path){
  bool check_Isotropic            = false;
  bool check_TwoBodyReaction      = false;
  bool check_Beam                 = false;
  
  // You can have more than one of those
  int   alreadyiInstantiate_GammaDecay = 0;
  int   seenToken_GammaDecay = 0;
  int   alreadyiInstantiate_ParticleDecay = 0;
  int   seenToken_ParticleDecay = 0;
  
  if(NPOptionManager::getInstance()->GetVerboseLevel()==1) G4cout << "/////////////////////////////////////////////////// " << G4endl ;

  string LineBuffer;
  ifstream EventGeneratorFile;
  EventGeneratorFile.open(Path.c_str());
  
  if (EventGeneratorFile.is_open()) { // should always be true
    G4cout << "Event Generator file " << Path << " loading " << G4endl  ;
  }
  else {
    G4cout << "Error, Event Generator file " << Path << " found" << G4endl;
  }
  
  while (!EventGeneratorFile.eof()) {
    //Pick-up next line
    getline(EventGeneratorFile, LineBuffer);
    
    //Search for comment Symbol %
    if (LineBuffer.compare(0, 1, "%") == 0) {   /*do nothing*/
      ;
    }
    
    //Search for Isotropic source
    else if (LineBuffer.compare(0, 9, "Isotropic") == 0  && !check_Isotropic) {
      check_Isotropic = true;
      VEventGenerator* myEventGenerator = new EventGeneratorIsotropic();
      EventGeneratorFile.close();
      myEventGenerator->ReadConfiguration(Path);
      EventGeneratorFile.open(Path.c_str());
      myEventGenerator->InitializeRootOutput();
      m_EventGenerator.push_back(myEventGenerator);
    }
    
    //Search for Beam
    else if (LineBuffer.compare(0, 4, "Beam") == 0  && !check_Beam) {
      check_Beam = true;
      VEventGenerator* myEventGenerator = new EventGeneratorBeam();
      EventGeneratorFile.close();
      myEventGenerator->ReadConfiguration(Path);
      EventGeneratorFile.open(Path.c_str());
      myEventGenerator->InitializeRootOutput();
      myEventGenerator->SetTarget(m_detector->GetTarget());
      m_EventGenerator.push_back(myEventGenerator);
    }
    
    //Search for Two body reaction
    else if (LineBuffer.compare(0, 15, "TwoBodyReaction") == 0 && !check_TwoBodyReaction) {
      check_TwoBodyReaction = true;
      VEventGenerator* myEventGenerator = new EventGeneratorTwoBodyReaction();
      EventGeneratorFile.close();
      myEventGenerator->ReadConfiguration(Path);
      EventGeneratorFile.open(Path.c_str());
      myEventGenerator->InitializeRootOutput();
      myEventGenerator->SetTarget(m_detector->GetTarget());
      m_EventGenerator.push_back(myEventGenerator);
    }
    
    //Search for GammaDecay
    else if ( LineBuffer.compare(0, 10, "GammaDecay") == 0 ) {
      seenToken_GammaDecay++;
      if (seenToken_GammaDecay>alreadyiInstantiate_GammaDecay) {
        alreadyiInstantiate_GammaDecay++;
        VEventGenerator* myEventGenerator = new EventGeneratorGammaDecay();
        EventGeneratorFile.close();
        myEventGenerator->ReadConfiguration(Path,alreadyiInstantiate_GammaDecay);
        EventGeneratorFile.open(Path.c_str());
        myEventGenerator->InitializeRootOutput();
        myEventGenerator->SetTarget(m_detector->GetTarget());
        m_EventGenerator.push_back(myEventGenerator);
        seenToken_GammaDecay=0;
      }
      
    }
    
    //Search for ParticleDecay
    else if ( LineBuffer.compare(0, 13, "ParticleDecay") == 0 ) {
      seenToken_ParticleDecay++;
      if(seenToken_ParticleDecay>alreadyiInstantiate_ParticleDecay){
        alreadyiInstantiate_ParticleDecay++;
        VEventGenerator* myEventGenerator = new EventGeneratorParticleDecay();
        EventGeneratorFile.close();
        myEventGenerator->ReadConfiguration(Path,alreadyiInstantiate_ParticleDecay);
        EventGeneratorFile.open(Path.c_str());
        myEventGenerator->InitializeRootOutput();
        myEventGenerator->SetTarget(m_detector->GetTarget());
        m_EventGenerator.push_back(myEventGenerator);
        seenToken_ParticleDecay=0;
      }
    }
  }
  
  EventGeneratorFile.close();
}

