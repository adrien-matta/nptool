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
#include "NPSimulationVersion.hh"
//NPL headers
#include "NPOptionManager.h"
#include "RootOutput.h"

int main(int argc, char** argv){
    // Initialize NPOptionManager object
    NPOptionManager* OptionManager  = NPOptionManager::getInstance(argc, argv);
    if(OptionManager->GetVerboseLevel() > 0){
        string line;
        line.resize(80,'*');
        cout << endl << line << endl;
        cout << "********************************  NPSimulation  ********************************"<< endl;
        cout << line << endl;
        cout << "NPSimulation version: npsimulation-"<< NPS::version_major <<"-" << NPS::version_minor << "-" << NPS::version_dets <<endl;
        cout << " Copyright: NPTool Collaboration "<<endl;
        cout << " GitHub: http://github.com/adrien-matta/nptool"<<endl; ;
        cout << line << endl;
    }
    
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
    PrimaryGeneratorAction* primary = new PrimaryGeneratorAction(detector);

    // Initialize Geant4 kernel
    runManager->Initialize();
    
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
    
    
    G4VisManager* visManager=NULL;
    if(!OptionManager->GetG4BatchMode()){
#ifdef G4UI_USE
        string Path_Macro = getenv("NPTOOL");
        Path_Macro+="/NPSimulation/ressources/macro/";
        UImanager->ApplyCommand("/control/execute " +Path_Macro+"verbose.mac");

#ifdef G4VIS_USE
         
        UImanager->ApplyCommand("/control/execute " +Path_Macro+"aliases.mac");
        visManager = new G4VisExecutive("Quiet");
        visManager->Initialize();
        UImanager->ApplyCommand("/control/execute " +Path_Macro+"vis.mac");
#endif
        if (ui->IsGUI()){
            UImanager->ApplyCommand("/control/execute " +Path_Macro+"gui.mac");
        }
#ifdef __APPLE__
        string command= "osascript ";
        command+= getenv("NPTOOL");
        command+="/NPSimulation/ressources/scripts/bringtofront.osa & ";
        int res =system(command.c_str());
        res =0;
        
#endif
    }
    // Execute user macro
    if(!OptionManager->IsDefault("G4MacroPath")){
        UImanager->ApplyCommand("/control/execute "+ OptionManager->GetG4MacroPath());
    }
    
    // Start the session
    if(!OptionManager->GetG4BatchMode())
        ui->SessionStart();
    
    
    delete ui;
#endif
    
#ifdef G4VIS_USE
    if(visManager)
        delete visManager;
#endif
    
    ///////////////////////////////////////////////////////////////
    ////////////////////// Job termination ////////////////////////
    ///////////////////////////////////////////////////////////////
    // delete primary; delete detector;
    
    delete runManager;
    RootOutput::getInstance()->Destroy();
    return 0;
}
