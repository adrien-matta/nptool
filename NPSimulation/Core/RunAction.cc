/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : April 2015                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  A quite Standard Geant4 RunAction class.                                 *
 *  Use to register the run number in the tree                               *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// NPS
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "ParticleStack.hh"
#include "MaterialManager.hh"
// G4
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4VTrajectory.hh"

// NPL
#include "RootOutput.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
RunAction::RunAction(): G4UserRunAction(){
  RootOutput::getInstance()->GetTree()->Branch("Run",&m_RunNumber);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
RunAction::~RunAction(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::BeginOfRunAction(const G4Run* aRun){

    cout << "Starting run " << aRun->GetRunID()+1;
    // Replug the branch in case it no longer exist (e.g. reloading geometry) 
    RootOutput::getInstance()->GetTree()->SetBranchAddress("Run",&m_RunNumber);

    // Increment the run number to be stored in the tree
    m_RunNumber = aRun->GetRunID()+1;
    
    //initialize event cumulative quantities
    EventAction::GetInstance()->SetRunLength(aRun->GetNumberOfEventToBeProcessed());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::EndOfRunAction(const G4Run* aRun){
    // Pass a line for nicer presentation when chainning event generator
    cout << endl;
    // Force the tree to be saved at the end of the run
    RootOutput::getInstance()->GetTree()->AutoSave("Overwrite SaveSelf");
    // Write DEDX Tables
    std::set<string> Particles;
    const std::vector<const G4Event*>* events = aRun->GetEventVector();
    unsigned int sizeE = events->size();
    for(unsigned int e = 0 ; e < sizeE ; e++){
      TrajectoryVector* traj = (*events)[e]->GetTrajectoryContainer()->GetVector();
      unsigned int size = traj->size();
      for(unsigned int i = 0 ; i < size ; i++)
        Particles.insert( (*traj)[i]->GetParticleName());
    }
    
    MaterialManager::getInstance()->WriteDEDXTable(Particles,0,10*GeV); 
}
