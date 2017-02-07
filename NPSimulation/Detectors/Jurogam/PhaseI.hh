#ifndef PhaseI_H
#define PhaseI_H 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"

#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"

//class G4Box;
class G4Tubs;
class G4Polycone;
class G4UnionSolid;
//class G4SubtractionSolid;
//class G4IntersectionSolid;
//class G4Polyhedra;
class G4LogicalVolume;
class G4VPhysicalVolume;
class MyDetectorMessenger;
//class MySensitiveDet;
class Materials;

class PhaseI { //: public G4VUserDetectorConstruction {

public:
  PhaseI();
  ~PhaseI();

public:
  void SetPosition( G4ThreeVector );
  void SetRotation( G4RotationMatrix );
  void Placement(G4int, G4LogicalVolume*, G4bool);
  G4LogicalVolume* CreatePhaseILogicalVolumes();

  //G4VPhysicalVolume* Construct();
  //void SetDetectorType (G4String);
  void SetGeRingZPos   (G4double);

private:
  //General materials....
  Materials*   fMat;

  //G4Material* eDetMat;
  G4Material* endCapMaterial;
  G4Material* vacuumMaterial;
  G4Material* geMaterial;
  G4Material* beMaterial;
  G4Material* contactMaterial;

  G4ThreeVector        position;
  G4RotationMatrix     rotation;

  //Ortec or Intertechnique
  G4int    fDetMan;

  //Keep track of which detector is used
  G4String  theDetector;
  G4int     nDetectors;

  //-------------------------------------------
  // A Phase-I tapered Ge
  G4double            fGeTaperL_PhaseI;
  G4double            fTotalGeL_PhaseI;
  G4double            fGeOuterD_PhaseI;
  G4double            fGeInnerD_PhaseI;
  G4double            fAlCap2Ge_PhaseI;
  G4double            fEndCapThickness_PhaseI;
  G4double            fEndCapTaperL_PhaseI;
  G4double            fEndCapTubeL_PhaseI;
  G4double            fEndCapFrontR_PhaseI;
  G4double            fEndCapBackR_PhaseI;
  //
public:
  G4Polycone*         solidAlCap_PhaseI;  //Al end-cap
  G4LogicalVolume*    logicAlCap_PhaseI;
  G4VPhysicalVolume*  physiAlCap_PhaseI;
  G4Polycone*         solidVacuum_PhaseI; //vacuum
  G4LogicalVolume*    logicVacuum_PhaseI;
  G4VPhysicalVolume*  physiVacuum_PhaseI;
  G4Polycone*         solidGe_PhaseI; //Ge
  G4LogicalVolume*    logicGe_PhaseI;
  G4VPhysicalVolume*  physiGe_PhaseI;
  //G4String            sensitiveGeName;
  G4double           fContact_dZ_PhaseI;  //dZ to position Passivated Ge
  G4UnionSolid*      solidPassivated_PhaseI;
  G4LogicalVolume*   logicPassivated_PhaseI;
  G4VPhysicalVolume* physiPassivated_PhaseI;
  G4UnionSolid*      solidContact_PhaseI; //inner Li contact
  G4LogicalVolume*   logicContact_PhaseI;
  G4VPhysicalVolume* physiContact_PhaseI;
  G4UnionSolid*      solidBoreHole_PhaseI; //inner Li contact
  G4LogicalVolume*   logicBoreHole_PhaseI;
  G4VPhysicalVolume* physiBoreHole_PhaseI;
private:


  //Specific Phase-I's
  G4double fAlCap2Ge_Ortec;
  G4double fGeTaperL_Ortec;
  G4double fTotalGeL_Ortec;
  G4double fGeOuterD_Ortec;
  G4double fGeInnerD_Ortec;
  G4double fEndCapFrontR_Ortec;
  G4double fEndCapBackR_Ortec;
  G4double fEndCapTaperL_Ortec;
  G4double fEndCapTubeL_Ortec;
  G4double fEndCapThickness_Ortec;

  //-------------------------------------------
  G4double            geRingZPos;

private:
  void SetDetectorParameters();

  void debugSolids();

  void Set_PhaseI_Dimensions();

  void CreatePhaseISolids();

};

#endif

