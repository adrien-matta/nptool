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
// NPS Source
#include "EventAction.hh"
#include "VDetector.hh"
#include "RootOutput.h"
// ROOT Source
#include "TTree.h"
#include "TFile.h"
// STL
#include <vector>

int main(int argc, char** argv)
{
   if (argc != 3) {
      cout << "you need to specify both a Reaction file and a Detector file such as : Simulation myReaction.reaction myDetector.detector" << endl ;
      return 0;
   }

   // Getting arguments from command line
   G4String EventGeneratorFileName = argv[1];
   G4String DetectorFileName       = argv[2];

   //my Verbose output class
   G4VSteppingVerbose::SetInstance(new SteppingVerbose);

   // Construct the default run manager
   G4RunManager* runManager = new G4RunManager;

   // Initialize the ROOT output
   RootOutput::getInstance("Simulation/mySimul");

   // Initialize the geometry
   DetectorConstruction* detector = new DetectorConstruction();
   runManager->SetUserInitialization(detector);

   // Initialize the physics
   PhysicsList* physics   = new PhysicsList();
   runManager->SetUserInitialization(physics);

   // Initialize the primary particles
   PrimaryGeneratorAction* primary = new PrimaryGeneratorAction(detector);
   runManager->SetUserAction(primary);

   // Optional UserActions: run, event, stepping
   EventAction* event_action = new EventAction();
   event_action->SetDetector(detector);
   runManager->SetUserAction(event_action);

#ifdef G4VIS_USE
   // Visualization manager
   G4VisManager* visManager = new G4VisExecutive;
   visManager->Initialize();
#endif

   // Get the pointer to the User Interface manager 
   G4UImanager* UI = G4UImanager::GetUIpointer();
   G4UIsession* session = 0;

#ifdef G4UI_USE_TCSH
   session = new G4UIterminal(new G4UItcsh);
#else
   session = new G4UIterminal();
#endif

   UI->ApplyCommand("/control/execute vis.mac");

   // should be done here since the runManager->Initialize()
   // command is done in the vis.mac macro
   detector->ReadConfigurationFile(DetectorFileName);
   primary->ReadEventGeneratorFile(EventGeneratorFileName);

   session->SessionStart();
   delete session;

#ifdef G4VIS_USE
   delete visManager;
#endif

   ///////////////////////////////////////////////////////////////
   ////////////////////// Job termination ////////////////////////
   ///////////////////////////////////////////////////////////////
   RootOutput::getInstance()->Destroy();

   delete runManager;
   return 0;
}
