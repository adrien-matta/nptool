/*****************************************************************************
 * Copyright (C) 2009-2015   this file is part of the NPTool Project         *
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
// G4
#include "G4Run.hh"
#include "G4RunManager.hh"
// NPL
#include "RootOutput.h"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
RunAction::RunAction(): G4UserRunAction(){
    RootOutput::getInstance()->GetTree()->Branch("Run",&m_RunNumber,"Run/I");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
RunAction::~RunAction(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::BeginOfRunAction(const G4Run* aRun){
    m_RunNumber = aRun->GetRunID()+1;
    
    //initialize event cumulative quantities
    EventAction::GetInstance()->SetRunLength(aRun->GetNumberOfEventToBeProcessed());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::EndOfRunAction(const G4Run*){
    // Pass a line for nicer presentation when chainning event generator
    cout << endl;
}

