/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
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
#include "NPInputParser.h"
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
#include "EventGeneratorpBUU.hh"
#include "EventGeneratorBeam.hh"
#include "EventGeneratorGammaDecay.hh"
#include "EventGeneratorParticleDecay.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* det): m_detector(det){
  m_Messenger = new PrimaryGeneratorActionMessenger(this);
  m_GenerateEvent = &NPS::VEventGenerator::GenerateEvent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction::~PrimaryGeneratorAction(){
  unsigned int mysize = m_EventGenerator.size();
  for (unsigned int i = 0 ; i < mysize; i++) {
    delete m_EventGenerator[i];
  }
  m_EventGenerator.clear();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
  // In case the target has changed
  SetTarget();
  unsigned int mysize = m_EventGenerator.size();
  for (unsigned int i = 0 ; i < mysize; i++) {
    //m_EventGenerator[i]->GenerateEvent(anEvent);
    (m_EventGenerator[i]->*m_GenerateEvent)(anEvent);
  }

  ParticleStack::getInstance()->ShootAllParticle(anEvent);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::ReadEventGeneratorFile(string Path){
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << endl << "\033[1;35m//// Reading event generator file  "<< Path  << endl; 

  NPL::InputParser parser(Path);

  vector<NPL::InputBlock*> blocks;

  blocks.clear();
  blocks = parser.GetAllBlocksWithToken("Isotropic");
  if (blocks.size()>0) {
    NPS::VEventGenerator* myEventGenerator = new EventGeneratorIsotropic();
    myEventGenerator->ReadConfiguration(parser);
    myEventGenerator->InitializeRootOutput();
    m_EventGenerator.push_back(myEventGenerator);
  }
  blocks.clear();
  blocks = parser.GetAllBlocksWithToken("pBUU");
  if (blocks.size()>0) {
    NPS::VEventGenerator* myEventGenerator = new EventGeneratorpBUU();
    myEventGenerator->ReadConfiguration(parser);
    myEventGenerator->InitializeRootOutput();
    m_EventGenerator.push_back(myEventGenerator);
  }
  blocks.clear();
  blocks = parser.GetAllBlocksWithToken("Beam");
  if (blocks.size()>0) {
    NPS::VEventGenerator* myEventGenerator = new EventGeneratorBeam();
    myEventGenerator->ReadConfiguration(parser);
    myEventGenerator->InitializeRootOutput();
    myEventGenerator->SetTarget(m_detector->GetTarget());
    m_EventGenerator.push_back(myEventGenerator);
  }
  blocks.clear();
  blocks = parser.GetAllBlocksWithToken("TwoBodyReaction");
  if (blocks.size()>0) {
    NPS::VEventGenerator* myEventGenerator = new EventGeneratorTwoBodyReaction();
    myEventGenerator->ReadConfiguration(parser);
    myEventGenerator->InitializeRootOutput();
    myEventGenerator->SetTarget(m_detector->GetTarget());
    m_EventGenerator.push_back(myEventGenerator);
  }
  blocks.clear();
  blocks = parser.GetAllBlocksWithToken("GammaCascade");
  if (blocks.size()>0) {
    NPS::VEventGenerator* myEventGenerator = new EventGeneratorGammaDecay();
    myEventGenerator->ReadConfiguration(parser);
    myEventGenerator->InitializeRootOutput();
    myEventGenerator->SetTarget(m_detector->GetTarget());
    m_EventGenerator.push_back(myEventGenerator);
  }
  blocks.clear();
  blocks = parser.GetAllBlocksWithToken("ParticleDecay");
  if (blocks.size()>0) {
    NPS::VEventGenerator* myEventGenerator = new EventGeneratorParticleDecay();
    myEventGenerator->ReadConfiguration(parser);
    myEventGenerator->InitializeRootOutput();
    myEventGenerator->SetTarget(m_detector->GetTarget());
    m_EventGenerator.push_back(myEventGenerator);
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::ClearEventGenerator(){
  unsigned int mysize = m_EventGenerator.size();
  for (unsigned int i = 0 ; i < mysize; i++) {
    delete m_EventGenerator[i];
  }

  m_EventGenerator.clear();

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::SetTarget(){
  for (unsigned int i = 0 ; i < m_EventGenerator.size(); i++) {
    m_EventGenerator[i]->SetTarget(m_detector->GetTarget());
  }
}

