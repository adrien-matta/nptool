/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : November 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe the Tiara Silicon array                              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <sstream>
#include <cmath>
#include <limits>
//G4 Geometry object
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4UnionSolid.hh" 
#include "G4ExtrudedSolid.hh"
#include "G4TwoVector.hh"
//G4 sensitive
#include "G4SDManager.hh"

//G4 various object
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4Colour.hh"
#include "G4PVDivision.hh"
#include "G4SubtractionSolid.hh"

// NPS
#include "Tiara.hh"

// NPL
#include "NPOptionManager.h"

//#include "TiaraScorers.hh"
#include "RootOutput.h"
using namespace TIARA;

// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Tiara::Tiara(){
  InitializeMaterial();
  m_EventBarrel = new TTiaraBarrelData();
  m_EventHyball = new TTiaraHyballData();

  // Dark Grey
  SiliconVisAtt = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3)) ;
  // Green
  PCBVisAtt = new G4VisAttributes(G4Colour(0.2, 0.5, 0.2)) ;
  // Gold Yellow
  PADVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.2)) ;
  // Light Grey
  FrameVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)) ;

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Tiara::~Tiara(){
  delete m_MaterialSilicon;
  delete m_MaterialAl;
  delete m_MaterialVacuum;
  delete m_MaterialPCB;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of VDetector class
// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Tiara::ReadConfiguration(string Path){
  /*  ifstream ConfigFile           ;
      ConfigFile.open(Path.c_str()) ;
      string LineBuffer             ;
      string DataBuffer             ;



      while (!ConfigFile.eof()){
      int VerboseLevel = NPOptionManager::getInstance()->GetVerboseLevel();

      getline(ConfigFile, LineBuffer);
  // cout << LineBuffer << endl;
  if (LineBuffer.compare(0, 5, "Tiara") == 0)
  ReadingStatus = true;

  while (ReadingStatus && !ConfigFile.eof()) {
  ConfigFile >> DataBuffer ;
  //   Comment Line
  if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

  // Tiara Chamber
  if (DataBuffer=="TiaraChambe="){
  if(VerboseLevel==1) G4cout << "///" << G4endl           ;
  if(VerboseLevel==1) G4cout << "Chamber Found:: " << G4endl   ;
  bool bool_Chamber;
  ConfigFile >> bool_Chamber;
  }

  //  Barrel case
  else if (DataBuffer=="TiaraBarrel"){
  if(VerboseLevel==1) G4cout << "///" << G4endl           ;
  if(VerboseLevel==1) G4cout << "Barrel found: " << G4endl   ;
  // ReadingStatusBOX = true ;
  }

  // Hyball case
  else if (DataBuffer=="TiaraHyball")  
  if(VerboseLevel==1) G4cout << "///" << G4endl           ;
  if(VerboseLevel==1) G4cout << "Hyball  found: " << G4endl   ;

  }
  }


*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Tiara::ConstructDetector(G4LogicalVolume* world){
  ConstructChamber(world);
  ConstructBarrel(world);

}
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Tiara::ReadSensitive(const G4Event* event){

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Tiara::InitializeScorers(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Tiara::InitializeRootOutput(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Tiara::ConstructChamber(G4LogicalVolume* world){
  // Vaccum Chamber of Tiara
  // The chamber is made of a central cylinder surrounding the barrel Si
  // Two Cone that expeand out of the central cylinder to let room for Exogam
  // Two outer cylinder surrounding Hyball
  // Hyball is hold on a back plate that close the Diabolo Shaped Chamber


  // Material to be moved in a Material Function //
  // Al
  G4double density = 2.702*g/cm3;
  G4double a = 26.98*g/mole;
  G4Material* Aluminium = new G4Material("Aluminium", 13., a, density);

  // Making the Chamber //
  // We make the individual pieces, starting from the inside to the outside 
  // Then we merge them together using the a G4AdditionSolid
  // The whole chamber is then placed 

  //  Central Tube
  G4Tubs* solidCentralTube = 
    new G4Tubs("TiaraChamberCentralTube",CHAMBER_CentralTube_Inner_Radius,
        CHAMBER_CentralTube_Outer_Radius,CHAMBER_CentralTube_Length/2.,
        0*deg,360*deg);

  // Forward-Backward Cones
  G4Cons* solidOuterCone = 
    new G4Cons("TiaraChamberOuterCone",CHAMBER_CentralTube_Inner_Radius,
        CHAMBER_CentralTube_Outer_Radius,CHAMBER_OuterCylinder_Inner_Radius,
        CHAMBER_OuterCylinder_Outer_Radius,CHAMBER_OuterCone_Length/2.,
        0*deg,360*deg);

  // Outer Cylinder
  G4Tubs* solidOuterCylinder = 
    new G4Tubs("TiaraChamberOuterCylinder",CHAMBER_OuterCylinder_Inner_Radius,
        CHAMBER_OuterCylinder_Outer_Radius,CHAMBER_OuterCylinder_Length/2.,
        0*deg,360*deg);

  // Add the volume together
  G4UnionSolid* solidTiaraChamberStep1 =
    new G4UnionSolid("TiaraChamber", solidCentralTube, solidOuterCone,
        new G4RotationMatrix,
        G4ThreeVector(0,0,CHAMBER_OuterCone_Z_Pos)); 

  G4UnionSolid* solidTiaraChamberStep2 =
    new G4UnionSolid("TiaraChamber", solidTiaraChamberStep1, solidOuterCone,
        new G4RotationMatrix(0,180*deg,0),
        G4ThreeVector(0,0,-CHAMBER_OuterCone_Z_Pos)); 

  G4UnionSolid* solidTiaraChamberStep3 =
    new G4UnionSolid("TiaraChamber", solidTiaraChamberStep2, solidOuterCylinder,
        new G4RotationMatrix,
        G4ThreeVector(0,0,CHAMBER_OuterCylinder_Z_Pos)); 

  G4UnionSolid* solidTiaraChamberStep4 =
    new G4UnionSolid("TiaraChamber", solidTiaraChamberStep3, solidOuterCylinder,
        new G4RotationMatrix,
        G4ThreeVector(0,0,-CHAMBER_OuterCylinder_Z_Pos)); 

  // Create Logic Volume
  G4LogicalVolume* logicTiaraChamber =
    new G4LogicalVolume(solidTiaraChamberStep4,Aluminium,"logicTiaraChamber", 0, 0, 0);

  // Visual Attribute
  G4VisAttributes* ChamberVisAtt
    = new G4VisAttributes(G4Colour(0.0,0.4,0.5));

  ChamberVisAtt->SetForceWireframe(true);
  ChamberVisAtt->SetForceAuxEdgeVisible (true);
  logicTiaraChamber->SetVisAttributes(ChamberVisAtt);

  // Place the whole chamber
  new G4PVPlacement(new G4RotationMatrix(0,0,0), G4ThreeVector(0,0,0),
      logicTiaraChamber,"TiaraChamber",world,false,0);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Tiara::ConstructBarrel(G4LogicalVolume* world){
  // Tiara Barrel
  // The Barrel is made of 8 identical resistive strip detector
  // The PCB is made from a G4ExtrudeSolid, because it has beveled edge
  // the pcb is a substracted volume
  // the wafer goes into the hole
  // the whole things is design so the local reference is the one of the wafer

  // Start by making a full pcb
  // We start by the definition of the point forming a PCB cross section
  vector<G4TwoVector> PCBCrossSection;
  double l1 = INNERBARREL_PCB_Thickness*0.5/tan(INNERBARREL_PCB_Bevel1_Theta);
  double l2 = INNERBARREL_PCB_Thickness*0.5/tan(INNERBARREL_PCB_Bevel2_Theta);
  
  PCBCrossSection.push_back(G4TwoVector(INNERBARREL_PCB_Width/2.-l2,-INNERBARREL_PCB_Thickness*0.5));
  PCBCrossSection.push_back(G4TwoVector(INNERBARREL_PCB_Width/2.,0));
  PCBCrossSection.push_back(G4TwoVector(INNERBARREL_PCB_Width/2.-l1,INNERBARREL_PCB_Thickness*0.5));

  PCBCrossSection.push_back(G4TwoVector(-INNERBARREL_PCB_Width/2.+l1,INNERBARREL_PCB_Thickness*0.5));
  PCBCrossSection.push_back(G4TwoVector(-INNERBARREL_PCB_Width/2.,0));
  PCBCrossSection.push_back(G4TwoVector(-INNERBARREL_PCB_Width/2.+l2,-INNERBARREL_PCB_Thickness*0.5));

  G4ExtrudedSolid* PCBFull = 
    new G4ExtrudedSolid("PCBFull",
                       PCBCrossSection,
                       INNERBARREL_PCB_Length/2.,
                       G4TwoVector(0,0),1,
                       G4TwoVector(0,0),1);

  // A box having Wafer dimension but thicker than the PCB
  // Will be used to remove material from the PCB to have space for the wafer
  G4Box*  WaferShape = new G4Box("WaferShape",
      INNERBARREL_Wafer_Width/2.,
      INNERBARREL_PCB_Thickness/2.+0.1*mm,
      INNERBARREL_Wafer_Length/2.);

  // The Silicon Wafer itself
  G4Box*  Wafer = new G4Box("Wafer",
      INNERBARREL_Wafer_Width/2.,
      INNERBARREL_Wafer_Thickness/2,
      INNERBARREL_Wafer_Length/2.);

  // Calculate the wafer shift within the PCB
  G4ThreeVector WaferShift = G4ThreeVector(
    0,
    0,
    INNERBARREL_PCB_Offset-(INNERBARREL_PCB_Length/2-INNERBARREL_Wafer_Length/2));

  // Substracting the Wafer Shape from the Stock PCB
  G4SubtractionSolid* PCB = new G4SubtractionSolid("PCB", PCBFull, WaferShape,
    new G4RotationMatrix,WaferShift);

  // Master Volume that encompass everything else
  G4LogicalVolume* logicBarrelDetector =
    new G4LogicalVolume(PCBFull,m_MaterialVacuum,"logicBoxDetector", 0, 0, 0);
  logicBarrelDetector->SetVisAttributes(G4VisAttributes::Invisible);
  
  // Sub Volume PCB
  G4LogicalVolume* logicPCB =
    new G4LogicalVolume(PCB,m_MaterialPCB,"logicPCB", 0, 0, 0);
  logicPCB->SetVisAttributes(PCBVisAtt);

  // Sub Volume Wafer
  G4LogicalVolume* logicWafer =
    new G4LogicalVolume(Wafer,m_MaterialSilicon,"logicWafer", 0, 0, 0);
  logicWafer->SetVisAttributes(SiliconVisAtt);
  
  // The Distance from target is given by half the lenght of a detector
  // plus the length of a detector inclined by 45 deg.
  G4double DistanceFromTarget = INNERBARREL_PCB_Width*(0.5+sin(45*deg)) ; 

  for( unsigned int i = 0; i < 8; i ++){
    // Place the sub volumes in the master volume
    // Last argument is the detector number, used in the scorer to get the
    // revelant information
    new G4PVPlacement(new G4RotationMatrix(0,0,0),
        G4ThreeVector(0,0,0),
        logicPCB,"Tiara_Barrel_PCB",logicBarrelDetector,
        false,i+1);

    new G4PVPlacement(new G4RotationMatrix(0,0,0),
        WaferShift,
        logicWafer,"Barrel_Wafer",
        logicBarrelDetector,false,i+1);

    // The following build the barrel, with detector one at the top
    // and going clowise looking upstrea
    
    // Detector are rotate by 45deg with each other 
    G4RotationMatrix* DetectorRotation = 
      new G4RotationMatrix(0*deg,0*deg,i*45*deg);
    
    // There center is also rotated by 45deg
    G4ThreeVector DetectorPosition(0,DistanceFromTarget,-WaferShift.z());
    DetectorPosition.rotate(i*45*deg,G4ThreeVector(0,0,-1));   
  
    // Place the Master volume with its two daugther volume at the final place 
    new G4PVPlacement(G4Transform3D(*DetectorRotation,DetectorPosition),
        logicBarrelDetector,"Tiara_Barrel_Detector",
        world,false,i+1);
  }

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Tiara::InitializeMaterial(){
  G4Element* H   = new G4Element("Hydrogen" , "H"  , 1  , 1.015  * g / mole);
  G4Element* C   = new G4Element("Carbon"   , "C"  , 6  , 12.011 * g / mole);
  G4Element* N   = new G4Element("Nitrogen" , "N"  , 7  , 14.01  * g / mole);
  G4Element* O   = new G4Element("Oxygen"   , "O"  , 8  , 15.99  * g / mole);
  
  G4double a, z, density;
  // Si
  a = 28.0855 * g / mole;
  density = 2.321 * g / cm3;
  m_MaterialSilicon = new G4Material("Si", z = 14., a, density);
  
  // Al
  density = 2.702 * g / cm3;
  a = 26.98 * g / mole;
  m_MaterialAl = new G4Material("Al", z = 13., a, density);
  
  // PCB (should be FR-4, I took Epoxy Molded from LISE++)
  density = 1.85 * g / cm3;
  m_MaterialPCB = new G4Material("PCB", density, 3);
  m_MaterialPCB->AddElement(H, .475);
  m_MaterialPCB->AddElement(C, .45);
  m_MaterialPCB->AddElement(O, .075);
  //  Vacuum
  density = 0.000000001 * mg / cm3;
  m_MaterialVacuum = new G4Material("Vacuum", density, 2);
  m_MaterialVacuum->AddElement(N, .7);
  m_MaterialVacuum->AddElement(O, .3);
}





