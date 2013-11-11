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
  //InitializeMaterial();
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
Tiara::~Tiara(){
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


  // Geometry Information to be moved in Tiara Name Space //
  // Central Tube
  G4double CHAMBER_CentralTube_Inner_Radius = 4.86*cm;	//4.05->Original Value for the Single stage barrel
  G4double CHAMBER_CentralTube_Outer_Radius = 5.05*cm;	//4.25->Original Value for the Single stage barrel
  G4double CHAMBER_CentralTube_Length = 4.12*cm;
  
  // Outer Cone
  G4double CHAMBER_OuterCone_Length = 4.94*cm;
  G4double CHAMBER_OuterCone_Z_Pos = 9.06*cm;

  // Outer Cylinder
  G4double CHAMBER_OuterCylinder_Inner_Radius = 24.6*cm;
  G4double CHAMBER_OuterCylinder_Outer_Radius = 25.0*cm;
  G4double CHAMBER_OuterCylinder_Length = 6.04*cm;
  G4double CHAMBER_OuterCylinder_Z_Pos = 19.96*cm;

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
               CHAMBER_CentralTube_Outer_Radius,CHAMBER_CentralTube_Length,
               0*deg,360*deg);
 
  // Forward-Backward Cones
  G4Cons* solidOuterCone = 
    new G4Cons("TiaraChamberOuterCone",CHAMBER_CentralTube_Inner_Radius,
                CHAMBER_CentralTube_Outer_Radius,CHAMBER_OuterCylinder_Inner_Radius,
                CHAMBER_OuterCylinder_Outer_Radius,CHAMBER_OuterCone_Length,
                0*deg,360*deg);
 
  // Outer Cylinder
  G4Tubs* solidOuterCylinder = 
    new G4Tubs("TiaraChamberOuterCylinder",CHAMBER_OuterCylinder_Inner_Radius,
                CHAMBER_OuterCylinder_Outer_Radius,CHAMBER_OuterCylinder_Length,
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
    = new G4VisAttributes(G4Colour(0.6,0.6,0.6));

    ChamberVisAtt->SetForceWireframe(true);
    ChamberVisAtt->SetForceAuxEdgeVisible (true);
    logicTiaraChamber->SetVisAttributes(ChamberVisAtt);

  // Place the whole chamber
  new G4PVPlacement(new G4RotationMatrix(0,0,0), G4ThreeVector(0,0,0),
                      logicTiaraChamber,"TiaraChamber",world,false,0);

}

