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

EventAction* EventAction::m_EventAction=0;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventAction::EventAction(){
  m_EventAction=this;
  begin=clock();
  treated=0;
  inter=0;
  total=0;
  mean_rate=0;
  displayed=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventAction::~EventAction(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::BeginOfEventAction(const G4Event* event){
  treated= event->GetEventID()+1;
  ProgressDisplay();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::EndOfEventAction(const G4Event* event){
  m_detector->ReadAllSensitive(event) ;
  static TTree* tree =  RootOutput::getInstance()->GetTree();
  tree->Fill();
  if(treated%10000==0){
    tree->AutoSave();
     RootOutput::getInstance()->GetFile()->SaveSelf(kTRUE);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::SetDetector(DetectorConstruction* detector){
  m_detector = detector   ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::ProgressDisplay(){
  if(treated==0){
    begin = clock() ;
    return;
  }
  
  end = clock();
  if((end-begin)>CLOCKS_PER_SEC||treated>=total ){
    displayed++;
    long double elapsed =(long double) (end-begin)/CLOCKS_PER_SEC;
    double event_rate = inter/elapsed;
    mean_rate += (event_rate-mean_rate)/(displayed);
    double percent = 100*treated/total;
    double remain = (total-treated)/mean_rate;

    char* timer;
    double check ;
    check = 0;
    if(remain>60)
      check = asprintf(&timer,"%dmin",(int)(remain/60.));
    else
      check = asprintf(&timer,"%ds",(int)(remain));
    
    check++;
    
    static char star[]="*******";
    if(displayed%2==0)
      sprintf(star,"*******");
    else
      sprintf(star,"-------"); 

    if(treated!=total && mean_rate >=0 && remain>=0 && displayed>1)
      printf("\r \033[1;31m %s Progress: %.1f%% | Rate: %.1fk evt/s | Remain: %s %s\033[0m        ", star,percent,mean_rate/1000.,timer,star);

    else if(mean_rate >=0 && remain>=0 && displayed>1){
      printf("\r \033[1;32m %s Progress: %.1f%% | Rate: %.1fk evt/s | Remain: %s %s\033[0m        ", star,percent,mean_rate/1000.,timer,star);
    }
    fflush(stdout);
    inter=0;
    begin = clock() ;
  }
  //  treated++;
  inter++;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::SetRunLength(int length){
  total = length;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventAction* EventAction::GetInstance(){
  return m_EventAction;
}
