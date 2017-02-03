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
 *  This class describe the Detector Messenger                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
///....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIparameter.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"

#include <dirent.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction* Det):Detector(Det){ 
  detDir = new G4UIdirectory("/det/");
  detDir->SetGuidance("detector geometry control");
  
  UpdateCmd = new G4UIcmdWithoutParameter("/det/update",this);
  UpdateCmd->SetGuidance("Update detector geometry.");
  UpdateCmd->SetGuidance("Apply this command after editing your geometry file ");
  UpdateCmd->AvailableForStates(G4State_Idle);

  OpenCmd = new G4UIcmdWithAString("/det/open",this);
  OpenCmd->SetGuidance("Open detector geometry.");
  OpenCmd->SetGuidance("Apply this command to open a new geometry file");
  OpenCmd->SetGuidance("NB: The new detector file should contain the same detectors as the original one");
  OpenCmd->AvailableForStates(G4State_Idle);

  ExportCmd = new G4UIcmdWithAString("/det/export_gdml",this);
  ExportCmd->SetGuidance("export current geometry to gdml");
  ExportCmd->AvailableForStates(G4State_Idle);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger(){
  delete UpdateCmd;
  delete detDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String value){ 
  if( command == UpdateCmd ){ 
    Detector->RedefineGeometry(""); 
  }
  else if( command == OpenCmd ){ 
    Detector->RedefineGeometry(value); 
  }
  else if(command ==ExportCmd ){
    Detector->ExportGeometry(value);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
