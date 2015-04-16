//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// STL
#include<cstdlib>

#include "G4RunManager.hh"
#include "G4PhysListFactory.hh"
// UI
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

// G4 local source
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"

// G4 General Source
#include "SteppingVerbose.hh"

// NPS headers
#include "EventAction.hh"
#include "RunAction.hh"

//NPL headers
#include "NPOptionManager.h"
#include "RootOutput.h"

int main(int argc, char** argv){
  // Initialize NPOptionManager object
  NPOptionManager* OptionManager  = NPOptionManager::getInstance(argc, argv);
  // Test if input files are found. If not, exit
  if (OptionManager->IsDefault("EventGenerator"))
    OptionManager->SendErrorAndExit("EventGenerator");
  if (OptionManager->IsDefault("DetectorConfiguration"))
    OptionManager->SendErrorAndExit("DetectorConfiguration");
  // case when input files are here
  G4String EventGeneratorFileName = OptionManager->GetReactionFile();
  G4String DetectorFileName       = OptionManager->GetDetectorFile();
  // my Verbose output class
  G4VSteppingVerbose::SetInstance(new SteppingVerbose);
 
 ///////////////////////////////////////////////////////////////
  ///////////////// Initializing the Root Output ////////////////
  ///////////////////////////////////////////////////////////////
  RootOutput::getInstance("Simulation/" + OptionManager->GetOutputFile());
   
  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;
  
  // set mandatory initialization classes
  DetectorConstruction* detector  = new DetectorConstruction();
  runManager->SetUserInitialization(detector);
  
  PhysicsList* physicsList   = new PhysicsList();
  runManager->SetUserInitialization(physicsList);
  
  // Test for Built in physics list
  // G4PhysListFactory *physListFactory = new G4PhysListFactory();
  //G4VUserPhysicsList *physicsList =
  //physListFactory->GetReferencePhysList("QGSP_BERT");
  
  runManager->SetUserInitialization(physicsList);
  
  
  PrimaryGeneratorAction* primary = new PrimaryGeneratorAction(detector);
  
  // Initialize Geant4 kernel
  runManager->Initialize();
  physicsList->MyOwnConstruction();
  
  ///////////////////////////////////////////////////////////////
  /////////// Define UI terminal for interactive mode ///////////
  ///////////////////////////////////////////////////////////////
  // interactive mode : define UI session
  // Get the pointer to the User Interface manager
  G4cout << "//////////// Starting UI ////////////"<< endl;
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
#ifdef G4UI_USE
  G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#endif 

 
  ///////////////////////////////////////////////////////////////
  ////////////////////// Reading Reaction ///////////////////////
  ///////////////////////////////////////////////////////////////
  primary->ReadEventGeneratorFile(EventGeneratorFileName);
  runManager->SetUserAction(primary);
  
  ///////////////////////////////////////////////////////////////
  ////////////////// Starting the Event Action //////////////////
  ///////////////////////////////////////////////////////////////
  EventAction* event_action = new EventAction() ;
  event_action->SetDetector(detector)           ;
  runManager->SetUserAction(event_action)       ;

  ///////////////////////////////////////////////////////////////
  /////////////////// Starting the Run Action ///////////////////
  ///////////////////////////////////////////////////////////////
  RunAction* run_action = new RunAction() ;
  runManager->SetUserAction(run_action);

#ifdef G4UI_USE
#ifdef G4VIS_USE
  string Path_Macro = getenv("NPTOOL");
  Path_Macro+="/NPSimulation/macro/";

  UImanager->ApplyCommand("/control/execute " +Path_Macro+"aliases.mac");
  G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
  UImanager->ApplyCommand("/control/execute " +Path_Macro+"vis.mac");
#endif
   if (ui->IsGUI()){
            UImanager->ApplyCommand("/control/execute " +Path_Macro+"gui.mac");
    }
#ifdef __APPLE__
  string command= "osascript ";
  command+= getenv("NPTOOL");
  command+="/NPSimulation/scripts/bringtofront.osa & ";
  int res =system(command.c_str());
  res =0;
#endif
 
    ui->SessionStart();
   
    delete ui;
#endif
  
#ifdef G4VIS_USE
  delete visManager;
#endif
  
  ///////////////////////////////////////////////////////////////
  ////////////////////// Job termination ////////////////////////
  ///////////////////////////////////////////////////////////////
  // delete primary; delete detector;
  
  delete runManager;
  NPOptionManager::getInstance()->Destroy();
  RootOutput::getInstance()->Destroy();
  return 0;
}
