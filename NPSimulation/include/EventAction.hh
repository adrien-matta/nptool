//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef EventAction_h
#define EventAction_h 1

// G4 header defining G4 types
#include "globals.hh"

// G4 header
#include "G4UserEventAction.hh"

// NPTool header
#include "DetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
public:
   EventAction();
   virtual ~EventAction();

public:
   void BeginOfEventAction(const G4Event*)                 ;
   void EndOfEventAction(const G4Event*)                 ;
   void SetDetector(DetectorConstruction* detector) ;



private:
   DetectorConstruction*  m_detector         ;
   G4double               m_edepPerEvent    ;
   G4int                  m_printModulo     ;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
