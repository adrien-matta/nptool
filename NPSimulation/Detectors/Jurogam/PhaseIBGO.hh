#ifndef PhaseIBGO_H
#define PhaseIBGO_H 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include <string>

//class G4Box;
//class G4Tubs;
//class G4Polycone;
class G4UnionSolid;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class MyDetectorMessenger;
class Materials;


class PhaseIBGO {

public:
  PhaseIBGO();
  ~PhaseIBGO();

public:
  void SetPosition( G4ThreeVector );
  void SetRotation( G4RotationMatrix );
  void Placement(G4int, G4VPhysicalVolume*, G4bool);
  void SetAbsorber( G4bool );

  //inline G4double GetShieldDistance {return BGOShieldDist;};

private:
  //General materials....
  Materials*   fMat;

  G4Material* vacuumMaterial;
  G4Material* BGOMaterial;
  G4Material* HevimetMaterial;
  G4Material* ShieldMaterial;
  G4Material* Absorber1Material;
  G4Material* Absorber2Material;

  G4bool   useAbsorber;

  G4ThreeVector        position;
  G4RotationMatrix     rotation;

  G4double BGOShieldLength;
  G4double BGOHevimetThickness;
  G4double BGOHevimetDist;
  G4double BGOShieldDist;
  G4double BGOCrystalDist;
  G4double BGOShieldOuterRadius;
  G4double BGOShieldInnerRadius;
  G4double subInnerConeL;
  G4double abs1_thickness;
  G4double abs2_thickness;

  G4VSolid*           solidBGOCrystal;
  G4VSolid*           solidBGOHevimet; //G4SubtractionSolid*
  G4VSolid*           solidBGOShield;
  G4VSolid*           Phase1Absorber1;
  G4VSolid*           Phase1Absorber2;
  //G4UnionSolid*           solidBGOShield;

  G4LogicalVolume*     logicBGOCrystal;
  G4VPhysicalVolume*   physiBGOCrystal;
  G4LogicalVolume*     logicBGOShield;
  G4VPhysicalVolume*   physiBGOShield;
  G4LogicalVolume*     logicBGOHevimet;
  G4VPhysicalVolume*   physiBGOHevimet;
  G4LogicalVolume*     Phase1Absorber1_log;
  G4LogicalVolume*     Phase1Absorber2_log;
  G4VPhysicalVolume*   Phase1Absorber1_phys;
  G4VPhysicalVolume*   Phase1Absorber2_phys;

private:
  void CreateSolids();
};

#endif
