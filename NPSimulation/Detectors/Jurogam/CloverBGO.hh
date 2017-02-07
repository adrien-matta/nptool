#ifndef CloverBGO_H
#define CloverBGO_H 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

class G4UnionSolid;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class MyDetectorMessenger;
class Materials;


class CloverBGO {

public:
  CloverBGO();
  ~CloverBGO();

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

  G4ThreeVector        position;
  G4RotationMatrix     rotation;

  G4double BGOHevimetThickness;
  G4double BGOHevimetDist;
  G4double BGOShieldDist;
  G4double BGOCrystalDist;
  G4double BGOCrystalH;
  G4double BGOCrystalAngle;
  G4double BGOCrystalBoxD;
  G4double BGOCrystalBoxD2;
  G4double BGOCrystalTrapL;
  G4double BGOCrystalW;
  G4double BGOCrystalW2;
  G4double BGOCrystalW3;
  G4double abs1_thickness;
  G4double abs2_thickness;
  G4double AbsorberWidth;

  G4bool   useAbsorber;

  G4UnionSolid*       solidBGOCrystal; // 1 side
  G4VSolid*           solidBGOCrystal2; // upper crystal, sliced
  G4VSolid*           solidBGOCrystal3; // lower crystal, sliced (mirrored)
  G4VSolid*           solidBGOHevimet;
  G4VSolid*           solidBGOShield;
  G4VSolid*           CloverAbsorber1;
  G4VSolid*           CloverAbsorber2;

  G4LogicalVolume*     logicBGOCrystal[4];
  G4VPhysicalVolume*   physiBGOCrystal[4];
  G4LogicalVolume*     logicBGOShield;
  G4VPhysicalVolume*   physiBGOShield;
  G4LogicalVolume*     logicBGOHevimet;
  G4VPhysicalVolume*   physiBGOHevimet;
  G4LogicalVolume*     CloverAbsorber1_log;
  G4LogicalVolume*     CloverAbsorber2_log;
  G4VPhysicalVolume*   CloverAbsorber1_phys;
  G4VPhysicalVolume*   CloverAbsorber2_phys;

private:
  void CreateSolids();
};

#endif
