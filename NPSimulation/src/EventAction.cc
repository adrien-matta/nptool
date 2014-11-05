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
 *  A quite Standard Geant4 EventAction class.                               *
 *  Call the Fill method of the output tree.                                 *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// G4 headers
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Trajectory.hh"
#include "G4TrajectoryContainer.hh"

// NPTool headers
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "RootOutput.h"

#include<iostream>
using namespace std;


#include "G4THitsMap.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(): m_printModulo(10000){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::BeginOfEventAction(const G4Event* event){

   if ((event->GetEventID() + 1) % m_printModulo == 0)
//      G4cout << "Event: " << event->GetEventID() + 1 << G4endl;
      G4cout << "\rEvent: " << event->GetEventID() + 1 << flush;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event){
   m_detector->ReadAllSensitive(event) ;
   RootOutput *pAnalysis = RootOutput::getInstance();
   pAnalysis->GetTree()->Fill();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::SetDetector(DetectorConstruction* detector){
   m_detector = detector   ;
}

