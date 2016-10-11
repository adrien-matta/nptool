/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : November 2014                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe the PrimaryGeneratorAction Messenger                 * 
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
///....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorActionMessenger.hh"

#include "PrimaryGeneratorAction.hh"
#include "G4UIparameter.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"

#include "NPOptionManager.h"
#include <dirent.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorActionMessenger::PrimaryGeneratorActionMessenger(PrimaryGeneratorAction* Gen):PGA(Gen){ 
  GenDir = new G4UIdirectory("/gen/");
  GenDir->SetGuidance("event generator control");

  UpdateCmd = new G4UIcmdWithoutParameter("/gen/update",this);
  UpdateCmd->SetGuidance("Update the event generator");
  UpdateCmd->SetGuidance("Apply this command after editing your event generator file ");
  UpdateCmd->AvailableForStates(G4State_Idle);

  DIR *dir;
  struct dirent *ent;
  string path = getenv("NPTOOL");
  path += "/Inputs/EventGenerator/";
  string choices;
  if ((dir = opendir (path.c_str())) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
      choices += ent->d_name ;
      choices += " " ;
    }
    closedir (dir);
  }

  OpenCmd = new G4UIcmdWithAString("/gen/open",this);
  OpenCmd->SetGuidance("Open a new event generator");
//  OpenCmd->SetCandidates(choices.c_str()); 
  OpenCmd->AvailableForStates(G4State_Idle);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorActionMessenger::~PrimaryGeneratorActionMessenger(){
  delete UpdateCmd;
  delete OpenCmd;
  delete GenDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue){ 
  if( command == UpdateCmd ){ 
    PGA->ClearEventGenerator();
    PGA->ReadEventGeneratorFile(NPOptionManager::getInstance()->GetReactionFile()); 
  }

  else if( command == OpenCmd ){ 
    PGA->ClearEventGenerator();
    NPOptionManager::getInstance()->SetReactionFile(newValue);
    PGA->ReadEventGeneratorFile(NPOptionManager::getInstance()->GetReactionFile()); 
  }

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
