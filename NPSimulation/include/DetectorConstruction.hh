#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;

#include "G4VUserDetectorConstruction.hh"
#include "VDetector.hh"
#include "TTree.h"
#include "G4Event.hh"
#include <vector>

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:

   DetectorConstruction();
   virtual ~DetectorConstruction();

   G4VPhysicalVolume* Construct();
   G4LogicalVolume* GetWorldLogic();

public:

   void ReadConfigurationFile(string);
   void AddDetector(VDetector*);
   void ReadAllSensitive(const G4Event*);

private:

   // Logical volumes
   G4LogicalVolume* world_log      ;

   // Physical volumes
   G4VPhysicalVolume* world_phys   ;

   // Virtual Detector Vector
   vector<VDetector*> m_Detectors   ;


private: // Data about the target
   G4double m_TargetThickness ;
   G4double m_TargetRadius    ;
   G4double m_TargetX         ;
   G4double m_TargetY         ;
   G4double m_TargetZ         ;

public:  // Getter and Setter
   G4double GetTargetThickness() {
      return m_TargetThickness   ;
   }
   G4double GetTargetRadius()    {
      return m_TargetRadius      ;
   }
   G4double GetTargetX()         {
      return m_TargetX        ;
   }
   G4double GetTargetY()         {
      return m_TargetY        ;
   }
   G4double GetTargetZ()         {
      return m_TargetZ        ;
   }
};

#endif

