#ifndef Clover_H
#define Clover_H 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"

#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"

//class G4Box;
//class G4Tubs;
//class G4Polycone;
class G4UnionSolid;
//class G4SubtractionSolid;
//class G4IntersectionSolid;
//class G4Polyhedra;
class G4LogicalVolume;
class G4VPhysicalVolume;
class MyDetectorMessenger;
class Materials;


class Clover {

public:
  Clover();
  ~Clover();

public:
  void SetPosition( G4ThreeVector );
  void SetRotation( G4RotationMatrix );
  void Placement(G4int, G4VPhysicalVolume*, G4bool);

  inline G4double GetTaperedEndCapL() {return fEndCapTaperL/mm;};

private:
  //General materials....
  Materials*   fMat;

  G4Material* endCapMaterial;
  G4Material* vacuumMaterial;
  G4Material* geMaterial;
  G4Material* contactMaterial;

  G4ThreeVector        position;
  G4RotationMatrix     rotation;

  G4double             lappingSize;

  G4double             fCrystalR;
  G4double             fTotalGeL;
  G4double             fHoleR;
  G4double             fContactThick;
  G4double             fPassiveThick;
  G4double             fEndCapTaperL;
  G4double             fEndCapThickness;
  G4double             fEndCap2Ge;
  G4double             fFudge;
  G4double             fVacuumPosZ;
  G4double             fContact_dZ;
  G4double             fGeLeafPosZ;
  G4double             fGapBetweenLeaves;
  G4double             fGeLeaf_dX;
  G4double             fHole_dX;
  G4double             fHole_dY;

  G4UnionSolid*        solidEndCap;
  G4UnionSolid*        solidVacuum;
  G4UnionSolid*        solidGeLeaf;
  G4UnionSolid*        solidPassivated;
  G4UnionSolid*        solidContact;
  G4UnionSolid*        solidBoreHole;

  G4LogicalVolume*     logicEndCap;
  G4VPhysicalVolume*   physiEndCap;
  G4LogicalVolume*     logicVacuum;
  G4VPhysicalVolume*   physiVacuum;
  G4LogicalVolume*     logicGeLeaf[4];
  G4VPhysicalVolume*   physiGeLeaf[4];
  G4LogicalVolume*     logicPassivated[4];
  G4VPhysicalVolume*   physiPassivated[4];
  G4LogicalVolume*     logicContact[4];
  G4VPhysicalVolume*   physiContact[4];
  G4LogicalVolume*     logicBoreHole[4];
  G4VPhysicalVolume*   physiBoreHole[4];


private:
  void CreateSolids();
  void MakeMaterials();

};

#endif
