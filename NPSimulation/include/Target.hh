#ifndef Target2Array_h
#define Target2Array_h 1

// C++ headers
#include <string>

// G4 header defining G4 types
#include "globals.hh"

// G4 headers
#include "G4Event.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"

// NPTool headers
#include "VDetector.hh"

using namespace std;



class Target : public VDetector
{
public:
   Target()               ;

public:
   // Read stream at Configfile to pick-up parameters of detector (Position,...)
   // Called in DetecorConstruction::ReadDetextorConfiguration Method
   void ReadConfiguration(string Path)          ;

   // Construct detector and inialise sensitive part.
   // Called After DetecorConstruction::AddDetector Method
   void ConstructDetector(G4LogicalVolume* world)  ;

   // Add Detector branch to the EventTree.
   // Called After DetecorConstruction::AddDetector Method
   void InitializeRootOutput()            ;

   // Read sensitive part and fill the Root tree.
   // Called at in the EventAction::EndOfEventAvtion
   void ReadSensitive(const G4Event* event)     ;


public:
   G4Material* GetMaterialFromLibrary(G4String MaterialName, G4double Temperature = 0, G4double Pressure = 0);

private:
   // Target type : true = normal ; false = cryo
   bool     m_TargetType         ;

   // Standard parameter
   G4double m_TargetThickness    ;
   G4double m_TargetRadius       ;
   G4Material* m_TargetMaterial     ;

   // For Cryo Target
   G4double m_WindowsThickness      ;
   G4Material* m_WindowsMaterial    ;
   G4double m_TargetTemperature     ;
   G4double m_TargetPressure     ;

   // Positioning
   G4double    m_TargetX            ;
   G4double    m_TargetY            ;
   G4double    m_TargetZ            ;

public:
   G4double GetTargetThickness() {
      return m_TargetThickness  ;
   }
   G4double GetTargetRadius()    {
      return m_TargetRadius     ;
   }
   G4double GetTargetX()         {
      return m_TargetX       ;
   }
   G4double GetTargetY()         {
      return m_TargetY       ;
   }
   G4double GetTargetZ()         {
      return m_TargetZ       ;
   }

};

#endif
