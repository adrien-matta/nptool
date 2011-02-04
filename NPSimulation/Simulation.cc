//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "G4RunManager.hh"

// UI
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

// G4 local source
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"

// G4 General Source
#include "SteppingVerbose.hh"
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

// NPS headers
#include "EventAction.hh"
#include "VDetector.hh"

//NPL headers
#include "NPOptionManager.h"
#include "RootOutput.h"

int main(int argc, char** argv)
{
   // Initialize NPOptionManager object
   NPOptionManager* OptionManager  = NPOptionManager::getInstance(argc, argv);
   // Test if input files are found. If not, exit
   if (OptionManager->IsDefault("EventGenerator"))        OptionManager->SendErrorAndExit("EventGenerator");
   if (OptionManager->IsDefault("DetectorConfiguration")) OptionManager->SendErrorAndExit("DetectorConfiguration");
   // case when input files are here
   G4String EventGeneratorFileName = OptionManager->GetReactionFile();
   G4String DetectorFileName       = OptionManager->GetDetectorFile();

   // my Verbose output class
   G4VSteppingVerbose::SetInstance(new SteppingVerbose);

   // Construct the default run manager
   G4RunManager* runManager = new G4RunManager;

   // set mandatory initialization classes
   DetectorConstruction* detector  = new DetectorConstruction();
   runManager->SetUserInitialization(detector);

   PhysicsList* physics   = new PhysicsList();
   runManager->SetUserInitialization(physics);
   PrimaryGeneratorAction* primary = new PrimaryGeneratorAction(detector);

   // Initialize Geant4 kernel
   runManager->Initialize();
   physics->MyOwnConstruction();

   ///////////////////////////////////////////////////////////////
   ///////////////// Initializing the Root Output ////////////////
   ///////////////////////////////////////////////////////////////
   RootOutput::getInstance("Simulation/" + OptionManager->GetOutputFile());

   ///////////////////////////////////////////////////////////////
   ////////////// Reading Detector Configuration /////////////////
   ///////////////////////////////////////////////////////////////
   detector->ReadConfigurationFile(DetectorFileName);

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
   ///////  Get the pointer to the User Interface manager ////////
   ///////////////////////////////////////////////////////////////
   G4UImanager* UI = G4UImanager::GetUIpointer();

   ///////////////////////////////////////////////////////////////
   /////////// Define UI terminal for interactive mode ///////////
   ///////////////////////////////////////////////////////////////
#ifdef G4VIS_USE
   G4VisManager* visManager = new G4VisExecutive;
   visManager->Initialize();
#endif

   G4UIsession* session = 0;

#ifdef G4UI_USE_TCSH
   session = new G4UIterminal(new G4UItcsh);
#else
   session = new G4UIterminal();
#endif

   UI->ApplyCommand("/control/execute vis.mac");
   session->SessionStart();
   delete session;

#ifdef G4VIS_USE
   delete visManager;
#endif

   ///////////////////////////////////////////////////////////////
   ////////////////////// Job termination ////////////////////////
   ///////////////////////////////////////////////////////////////
   RootOutput::getInstance()->Destroy();
   NPOptionManager::getInstance()->Destroy();

   delete runManager;
   return 0;
}
