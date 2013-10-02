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

using namespace std;


#include "G4THitsMap.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
      : m_printModulo(10000)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* event)
{
   if ((event->GetEventID() + 1) % m_printModulo == 0)
      G4cout << "Event: " << event->GetEventID() + 1 << G4endl ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
   m_detector->ReadAllSensitive(event) ;

   RootOutput *pAnalysis = RootOutput::getInstance();
   pAnalysis->GetTree()->Fill();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::SetDetector(DetectorConstruction* detector)
{
   m_detector = detector   ;
}

