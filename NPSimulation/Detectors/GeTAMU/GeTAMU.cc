/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : November 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe the GeTAMU Germanium array                          *
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
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4Cons.hh"

//G4 sensitive
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

//G4 various object
#include "G4Material.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4RunManager.hh"
#include "G4ios.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4ThreeVector.hh"

// NPS
#include "GeTAMU.hh"
//#include "GeTAMUScorers.hh"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"

// NPL
#include "NPOptionManager.h"
#include "RootOutput.h"
using namespace GETAMU;

// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace {

  // Ge crystal
  // Cylindrical part
  const G4double CrystalOuterRadius   = 30.0*mm; // outer radius for crystal
  const G4double CrystalInnerRadius   =  5.0*mm; // inner radius for hole in crystal
  const G4double CrystalLength        = 90.0*mm; // crystal length
  const G4double CrystalHoleDepth     = 15.0*mm; // depth at which starts the hole
  //const G4double CrystaHoleRadius 		= 0*cm;
  //const G4double CrystalInterDistance =  0.6*mm; // Distance between two crystal

  // Squared part
  const G4double CrystalWidth         = 56.5*mm;  	// Width of one crystal

  // Exogam Stuff
  const G4double CrystalEdgeOffset1  = 26.0*mm; // distance of the edge from the center of the crystal
  const G4double CrystalEdgeOffset2  = 28.5*mm; // distance of the edge from the center of the crystal

  const G4double CapsuleWidth        = 1.5*mm;   // capsule width
  const G4double CapsuleLength       = 110.*mm;   // capsule length
  const G4double CapsuleEdgeDepth    = 3.3*cm;   // same as crystal !
  const G4double CrystalToCapsule    = 7*mm;   // to be adjusted ..

  //const G4double BGOLength           = 120.0*mm;
  //const G4double BGOWidth            = 25.0*mm;

  //const G4double CsILength           = 20.0*mm;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// GeTAMU Specific Method
GeTAMU::GeTAMU(){
  InitializeMaterial();
  m_Event = new TGeTAMUData();

  BlueVisAtt   = new G4VisAttributes(G4Colour(0, 0, 1)) ;
  GreenVisAtt  = new G4VisAttributes(G4Colour(0, 1, 0)) ;
  RedVisAtt    = new G4VisAttributes(G4Colour(1, 0, 0)) ;
  WhiteVisAtt  = new G4VisAttributes(G4Colour(1, 1, 1)) ;
  TrGreyVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.5)) ;

  m_LogicClover = 0;

}

GeTAMU::~GeTAMU(){
  delete m_MaterialVacuum;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class
// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void GeTAMU::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("GeTAMU");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " clovers found " << endl; 

  vector<string> token = {"CloverID","R","Theta","Phi","Beta"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("Theta","deg");
      double Phi = blocks[i]->GetDouble("Phi","deg");
      vector<double> beta = blocks[i]->GetVectorDouble("Beta","deg");
      int     id = blocks[i]->GetInt("CloverID");
      AddCloverFreePosition(id,R,Theta,Phi,beta[0],beta[1],beta[2]);
    }

    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Return a G4VSolid modeling the Crystal
G4LogicalVolume* GeTAMU::ConstructCrystal(){
  G4Tubs* Crystal_Cylinder = new G4Tubs("Crystal_Cylinder", 0, CrystalOuterRadius, CrystalLength*0.5, 0, 2*M_PI);

  // Central Hole for cold finger
  G4RotationMatrix* BoxRotation = new G4RotationMatrix(0,0,0);
  G4Tubs* Crystal_Hole = new G4Tubs("Crystal_Hole", 0, CrystalInnerRadius, (CrystalLength-CrystalHoleDepth)*0.5, 0, 2*M_PI);
  G4SubtractionSolid* Crystal_Stage1 = new G4SubtractionSolid("Crystal_Stage1",Crystal_Cylinder,Crystal_Hole,BoxRotation,G4ThreeVector(0,0,CrystalHoleDepth));

  // Flat surface on the side
  G4Box* Crystal_Box1 = new G4Box("Crystal_Box1", CrystalWidth*0.6, CrystalWidth*0.6,CrystalLength*0.6);
  G4SubtractionSolid* Crystal_Stage2 = new G4SubtractionSolid("Crystal_Stage2",Crystal_Stage1,Crystal_Box1,BoxRotation,G4ThreeVector(24.5+CrystalWidth*0.6,0,0));
  G4SubtractionSolid* Crystal_Stage3 = new G4SubtractionSolid("Crystal_Stage3",Crystal_Stage2,Crystal_Box1,BoxRotation,G4ThreeVector(-29-CrystalWidth*0.6,0,0));
  G4SubtractionSolid* Crystal_Stage4 = new G4SubtractionSolid("Crystal_Stage4",Crystal_Stage3,Crystal_Box1,BoxRotation,G4ThreeVector(0,29+CrystalWidth*0.6,0));
  G4SubtractionSolid* Crystal_Stage5 = new G4SubtractionSolid("Crystal_Stage5",Crystal_Stage4,Crystal_Box1,BoxRotation,G4ThreeVector(0,-24.5-CrystalWidth*0.6,0));

  // Bezel
  G4RotationMatrix* BoxRotation1 = new G4RotationMatrix(0,0,0);
  BoxRotation1->rotate(22.5*deg,G4ThreeVector(1,0,0)); 
  G4SubtractionSolid* Crystal_Stage6= new G4SubtractionSolid("Crystal_Stage6",Crystal_Stage5,Crystal_Box1,BoxRotation1,G4ThreeVector(0,20.54*mm+CrystalWidth*0.6,-45*mm));
  
  G4RotationMatrix* BoxRotation2 = new G4RotationMatrix(0,0,0);
  BoxRotation2->rotate(22.5*deg,G4ThreeVector(0,1,0)); 
  G4SubtractionSolid* Crystal_Stage7= new G4SubtractionSolid("Crystal_Stage7",Crystal_Stage6,Crystal_Box1,BoxRotation2,G4ThreeVector(-20.54*mm-CrystalWidth*0.6,0,-45*mm));

    G4LogicalVolume* logicCrystal =
      new G4LogicalVolume(Crystal_Stage7,m_MaterialGe,"LogicCrystal", 0, 0, 0);

  return  logicCrystal;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Return a G4VSolid modeling the Capsule
G4LogicalVolume* GeTAMU::ConstructCapsule(){

  G4int nbslice = 7;
  const G4double widthface = 45.5*mm;
  G4double zSlice[7] = {  0.0*mm,
    CapsuleWidth-0.1*mm,
    CapsuleWidth,
    CapsuleEdgeDepth,
    CapsuleLength-CapsuleWidth,
    CapsuleLength-CapsuleWidth-0.1*mm,
    CapsuleLength  };
   
  G4double InnNullRad[7] = {0,0,0,0,0,0,0};

  G4double InnRad[7] = {  0.0*mm,
    0.0*mm,
    widthface-CapsuleWidth,
    CrystalEdgeOffset1 + CrystalEdgeOffset2 + CrystalToCapsule - CapsuleWidth,
    CrystalEdgeOffset1 + CrystalEdgeOffset2 + CrystalToCapsule - CapsuleWidth,
    0.0*mm,
    0.0*mm};
 
  G4double OutRad[7] = {  widthface-1.5*mm,
    widthface,
    widthface,
    CrystalEdgeOffset1 + CrystalEdgeOffset2 + CrystalToCapsule,
    CrystalEdgeOffset1 + CrystalEdgeOffset2 + CrystalToCapsule,
    CrystalEdgeOffset1 + CrystalEdgeOffset2 + CrystalToCapsule,
    CrystalEdgeOffset1 + CrystalEdgeOffset2 + CrystalToCapsule};

  // The whole volume of the Capsule, made of N2
  G4Polyhedra* caps = new G4Polyhedra(G4String("Capsule"), 45.*deg, 360.*deg, 4, nbslice, zSlice, InnNullRad, OutRad);
  G4LogicalVolume* LogicCapsule=
  new G4LogicalVolume(caps,m_MaterialN2,"LogicCapsule", 0, 0, 0);
  LogicCapsule->SetVisAttributes(G4VisAttributes::Invisible);

  // The wall of the Capsule made of Al
  G4Polyhedra* capsWall = new G4Polyhedra(G4String("CapsuleWall"), 45.*deg, 360.*deg, 4, nbslice, zSlice, InnRad, OutRad);
  G4LogicalVolume* logicCapsuleWall =
  new G4LogicalVolume(capsWall,m_MaterialAl,"LogicCapsuleWall", 0, 0, 0);

  new G4PVPlacement(G4Transform3D(*(new G4RotationMatrix()), G4ThreeVector(0,0,0)),
  logicCapsuleWall,"CapsuleWall",LogicCapsule,false,1);
  logicCapsuleWall->SetVisAttributes(TrGreyVisAtt);

  return LogicCapsule;

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* GeTAMU::ConstructDewar(){
  G4Tubs* DewarSolid = new G4Tubs("DewarSolid",0,90*mm*0.5,90*mm*0.5,0,2*M_PI);
  G4Tubs* DewarCFSolid = new G4Tubs("DewarCFSolid",0,45*mm*0.5,145*mm*0.5,0,2*M_PI);

  G4UnionSolid* DewarFull =
    new G4UnionSolid("Dewarfull", DewarSolid, DewarCFSolid, new G4RotationMatrix(),G4ThreeVector(0,0,-90*mm-(145-90)*0.5*mm));

  G4LogicalVolume* LogicDewar = new G4LogicalVolume(DewarFull,m_MaterialAl,"LogicDewar",0,0,0);

  G4Tubs* N2Solid = new G4Tubs("N2Solid",0,90*mm*0.5-1*mm,90*mm*0.5-1*mm,0,2*M_PI);
  G4Tubs* N2CFSolid = new G4Tubs("N2CFSolid",0,45*mm*0.5-1*mm,145*mm*0.5-1*mm,0,2*M_PI);

  G4LogicalVolume* LogicN2 = new G4LogicalVolume(N2Solid,m_MaterialN2,"LogicN2",0,0,0);
  G4LogicalVolume* LogicN2CF = new G4LogicalVolume(N2CFSolid,m_MaterialN2,"LogicN2CF",0,0,0);
 
  LogicN2->SetVisAttributes(GreenVisAtt);
  LogicN2CF->SetVisAttributes(GreenVisAtt);
  new G4PVPlacement(G4Transform3D(*(new G4RotationMatrix()), G4ThreeVector(0,0,0)),
  LogicN2,"N2 Deware",LogicDewar,false,1);
 
  //new G4PVPlacement(G4Transform3D(*(new G4RotationMatrix()), G4ThreeVector(0,0,-90*mm)),
 // LogicN2CF,"N2 Deware",LogicDewar,false,1);

  LogicDewar->SetVisAttributes(TrGreyVisAtt);

  return LogicDewar;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Return a G4VSolid modeling the BGO
G4LogicalVolume* GeTAMU::ConstructBGO(){

  return NULL;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Return a clover in the configuration given by option (not use a the moment)
void GeTAMU::ConstructClover(){

  if(m_LogicClover==0){
    // Construct the clover itself
    m_LogicClover = ConstructCapsule();
    
    // Place the cristal in the clover
    double CrystalOffset = (24.5*mm+0.5*mm);

    G4LogicalVolume* logicCrystal = ConstructCrystal();

    G4RotationMatrix* CrystalRotation = new G4RotationMatrix(0,0,0);
    G4ThreeVector CrystalPositionB = G4ThreeVector(-CrystalOffset,+CrystalOffset,0.5*CrystalLength+7*mm);
    new G4PVPlacement(G4Transform3D(*CrystalRotation, CrystalPositionB),
        logicCrystal,"LogicCrystalB",m_LogicClover,false,1);
    logicCrystal->SetVisAttributes(BlueVisAtt);

    CrystalRotation->rotate(-90*deg, G4ThreeVector(0,0,1));
    G4ThreeVector CrystalPositionG = G4ThreeVector(+CrystalOffset,+CrystalOffset,0.5*CrystalLength+7*mm);
    new G4PVPlacement(G4Transform3D(*CrystalRotation, CrystalPositionG),
        logicCrystal,"LogicCrystalG",m_LogicClover,false,2);

    CrystalRotation->rotate(-90*deg, G4ThreeVector(0,0,1));
    G4ThreeVector CrystalPositionR = G4ThreeVector(+CrystalOffset,-CrystalOffset,0.5*CrystalLength+7*mm);
    new G4PVPlacement(G4Transform3D(*CrystalRotation, CrystalPositionR),
        logicCrystal,"LogicCrystalR",m_LogicClover,false,3);

    CrystalRotation->rotate(-90*deg, G4ThreeVector(0,0,1));
    G4ThreeVector CrystalPositionW = G4ThreeVector(-CrystalOffset,-CrystalOffset,0.5*CrystalLength+7*mm);
    new G4PVPlacement(G4Transform3D(*CrystalRotation, CrystalPositionW),
        logicCrystal,"LogicCrystalW",m_LogicClover,false,4);

  }

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void GeTAMU::ConstructDetector(G4LogicalVolume* world){
  ConstructClover();

  G4RotationMatrix* DetectorRotation = new G4RotationMatrix(0,0,0);
  for (unsigned int i = 0 ;  i < m_CloverId.size(); i++) {

    // Constructing the Detector referential and the transition matrix
    G4ThreeVector U,V,W;
    G4double wX = sin(m_Theta[i]) * cos(m_Phi[i]) ;
    G4double wY = sin(m_Theta[i]) * sin(m_Phi[i]) ;
    G4double wZ = cos(m_Theta[i]);
    W = G4ThreeVector(wX, wY, wZ) ;

    // vector parallel to one axis of the entrance plane
    G4double vX = cos(m_Theta[i]) * cos(m_Phi[i]);
    G4double vY = cos(m_Theta[i]) * sin(m_Phi[i]);
    G4double vZ = -sin(m_Theta[i]);
    V = G4ThreeVector(vX, vY, vZ);

    W = W.unit();
    U = V.cross(W);
    U = U.unit();
    V = W.cross(U);
    V = V.unit();
    // Passage Matrix from Lab Referential to Clover Referential
    delete DetectorRotation;
    DetectorRotation = new G4RotationMatrix(U, V, W);

    DetectorRotation->rotate(m_BetaX[i], U);
    DetectorRotation->rotate(m_BetaY[i], V);
    DetectorRotation->rotate(m_BetaZ[i], W);
    G4ThreeVector DetectorPosition = m_R[i]*W;
  
    new G4PVPlacement(G4Transform3D(*DetectorRotation, DetectorPosition),
        m_LogicClover,"Clover",world,false,m_CloverId[i]);
  
    G4LogicalVolume* LogicDewar = ConstructDewar();
  
    new G4PVPlacement(G4Transform3D(*DetectorRotation, DetectorPosition+W*((90*mm+(145)*mm)+CapsuleLength*0.5+90*0.5*mm)),
        LogicDewar,"Dewar",world,false,m_CloverId[i]);


  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add clover at the standard position of the array
// Take as argument the standard clover Id.
void GeTAMU::AddCloverStandard(vector<int> CloverId){

  for (unsigned int i = 0 ;  i < CloverId.size(); i++) {
    if(CloverId[i] == 1 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(45*deg);
      m_Phi.push_back(22.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(0);
    }

    else if(CloverId[i] == 2 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(45*deg);
      m_Phi.push_back(112.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(0);
    }

    else if(CloverId[i] == 3 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(45*deg);
      m_Phi.push_back(202.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(0);
    }

    else if(CloverId[i] == 4 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(45*deg);
      m_Phi.push_back(292.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(0);
    }

    else if(CloverId[i] == 5 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(90*deg);
      m_Phi.push_back(22.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }

    else if(CloverId[i] == 6 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(90*deg);
      m_Phi.push_back(67.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }

    else if(CloverId[i] == 7 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(90*deg);
      m_Phi.push_back(112.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }

    else if(CloverId[i] == 8 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(90*deg);
      m_Phi.push_back(157.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }

    else if(CloverId[i] == 9 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(90*deg);
      m_Phi.push_back(202.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }

    else if(CloverId[i] == 10 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(90*deg);
      m_Phi.push_back(247.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }

    else if(CloverId[i] == 11 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(90*deg);
      m_Phi.push_back(292.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }

    else if(CloverId[i] == 12 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(90*deg);
      m_Phi.push_back(337.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }

    else if(CloverId[i] == 13 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(135*deg);
      m_Phi.push_back(22.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }

    else if(CloverId[i] == 14 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(135*deg);
      m_Phi.push_back(112.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }

    else if(CloverId[i] == 15 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(135*deg);
      m_Phi.push_back(202.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }

    else if(CloverId[i] == 16 ){
      m_CloverId.push_back(CloverId[i]);
      m_R.push_back(145*mm);
      m_Theta.push_back(135*deg);
      m_Phi.push_back(292.5*deg);
      m_BetaX.push_back(0);
      m_BetaY.push_back(0);
      m_BetaZ.push_back(180*deg);
    }
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add clover at a free position in space with coordinate
// in spherical coordinate
// Beta are the three angles of rotation in the Clover frame
void GeTAMU::AddCloverFreePosition(int CloverId,double R,double Theta,double Phi,double BetaX,double BetaY,double BetaZ){

  m_CloverId.push_back(CloverId);
  m_R.push_back(R);
  m_Theta.push_back(Theta);
  m_Phi.push_back(Phi);
  m_BetaX.push_back(BetaX);
  m_BetaY.push_back(BetaY);
  m_BetaZ.push_back(BetaZ);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void GeTAMU::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("GeTAMU")){
   pTree->Branch("GeTAMU", "TGeTAMUData", &m_Event) ;
  }  
  pTree->SetBranchAddress("GeTAMU", &m_Event) ;    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void GeTAMU::ReadSensitive(const G4Event* event){
  event->GetHCofThisEvent(); // event should be used to remove compilation warning
  /*m_Event->Clear();

  ///////////
  // BOX
  G4THitsMap<G4double*>*     BOXHitMap;
  std::map<G4int, G4double**>::iterator    BOX_itr;

  G4int BOXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("GeTAMU_BOXScorer/GeTAMUBOX");
  BOXHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(BOXCollectionID));

  // Loop on the BOX map
  for (BOX_itr = BOXHitMap->GetMap()->begin() ; BOX_itr != BOXHitMap->GetMap()->end() ; BOX_itr++){

  G4double* Info = *(BOX_itr->second);

  double Energy =  Info[0];
  double Time  = Info[1];
  int DetNbr =     (int) Info[2];
  int StripFront = (int) Info[3];
  int StripBack =  (int) Info[4];

  m_Event->SetFront_DetectorNbr(DetNbr);
  m_Event->SetFront_StripNbr(StripFront);
  m_Event->SetFront_Energy(RandGauss::shoot(Energy, ResoEnergy));
  m_Event->SetFront_TimeCFD(RandGauss::shoot(Time, ResoTime));
  m_Event->SetFront_TimeLED(RandGauss::shoot(Time, ResoTime));

  m_Event->SetBack_DetectorNbr(DetNbr);
  m_Event->SetBack_StripNbr(StripBack);
  m_Event->SetBack_Energy(RandGauss::shoot(Energy, ResoEnergy));
  m_Event->SetBack_TimeCFD(RandGauss::shoot(Time, ResoTime));
  m_Event->SetBack_TimeLED(RandGauss::shoot(Time, ResoTime));


  // Interraction Coordinates
  ms_InterCoord->SetDetectedPositionX(Info[5]) ;
  ms_InterCoord->SetDetectedPositionY(Info[6]) ;
  ms_InterCoord->SetDetectedPositionZ(Info[7]) ;
  ms_InterCoord->SetDetectedAngleTheta(Info[8]/deg) ;
  ms_InterCoord->SetDetectedAnglePhi(Info[9]/deg) ;

  }

  // clear map for next event
  BOXHitMap->clear();

  ///////////
  // PAD
  G4THitsMap<G4double*>*     PADHitMap;
  std::map<G4int, G4double**>::iterator    PAD_itr;

  G4int PADCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("GeTAMU_PADScorer/GeTAMUPAD");
  PADHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(PADCollectionID));

  // Loop on the BOX map
  for (PAD_itr = PADHitMap->GetMap()->begin() ; PAD_itr != PADHitMap->GetMap()->end() ; PAD_itr++){

  G4double* Info = *(PAD_itr->second);

  double Energy =  Info[0];
  double Time  = Info[1];
  int DetNbr =     (int) Info[2];

  m_Event->SetPAD_DetectorNbr(DetNbr);
  m_Event->SetPAD_Energy(RandGauss::shoot(Energy, ResoEnergy));
  m_Event->SetPAD_TimeCFD(RandGauss::shoot(Time, ResoTime));
  m_Event->SetPAD_TimeLED(RandGauss::shoot(Time, ResoTime));

  }

  // clear map for next event
  PADHitMap->clear();

  ///////////
  // QQQ
  G4THitsMap<G4double*>*     QQQHitMap;
  std::map<G4int, G4double**>::iterator    QQQ_itr;

  G4int QQQCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("GeTAMU_QQQScorer/GeTAMUQQQ");
  QQQHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(QQQCollectionID));

  // Loop on the BOX map
  for (QQQ_itr = QQQHitMap->GetMap()->begin() ; QQQ_itr != QQQHitMap->GetMap()->end() ; QQQ_itr++){

    G4double* Info = *(QQQ_itr->second);

    double Energy =  Info[0];
    double Time  = Info[1];
    int DetNbr =     (int) Info[2];
    int StripFront = (int) Info[3];
    int StripBack =  (int) Info[4];

    m_Event->SetFront_DetectorNbr(DetNbr);
    m_Event->SetFront_StripNbr(StripFront);
    m_Event->SetFront_Energy(RandGauss::shoot(Energy, ResoEnergy));
    m_Event->SetFront_TimeCFD(RandGauss::shoot(Time, ResoTime));
    m_Event->SetFront_TimeLED(RandGauss::shoot(Time, ResoTime));

    m_Event->SetBack_DetectorNbr(DetNbr);
    m_Event->SetBack_StripNbr(StripBack);
    m_Event->SetBack_Energy(RandGauss::shoot(Energy, ResoEnergy));
    m_Event->SetBack_TimeCFD(RandGauss::shoot(Time, ResoTime));
    m_Event->SetBack_TimeLED(RandGauss::shoot(Time, ResoTime));

    // Interraction Coordinates
    ms_InterCoord->SetDetectedPositionX(Info[5]) ;
    ms_InterCoord->SetDetectedPositionY(Info[6]) ;
    ms_InterCoord->SetDetectedPositionZ(Info[7]) ;
    ms_InterCoord->SetDetectedAngleTheta(Info[8]/deg) ;
    ms_InterCoord->SetDetectedAnglePhi(Info[9]/deg) ;

  }

  // clear map for next event
  QQQHitMap->clear();
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GeTAMU::InitializeScorers(){
  /*
  //   Silicon Associate Scorer
  m_BOXScorer = new G4MultiFunctionalDetector("GeTAMU_BOXScorer");
  m_PADScorer = new G4MultiFunctionalDetector("GeTAMU_PADScorer");
  m_QQQScorer = new G4MultiFunctionalDetector("GeTAMU_QQQScorer");

  G4VPrimitiveScorer* BOXScorer =
  new  GeTAMU::PS_Silicon_Rectangle("GeTAMUBOX",
  BOX_Wafer_Length,
  BOX_Wafer_Width,
  BOX_Wafer_Back_NumberOfStrip ,
  BOX_Wafer_Front_NumberOfStrip,
  EnergyThreshold);

  G4VPrimitiveScorer* PADScorer =
  new  GeTAMU::PS_Silicon_Rectangle("GeTAMUPAD",
  PAD_Wafer_Length,
  PAD_Wafer_Width,
  1 ,
  1,
  EnergyThreshold);

  G4VPrimitiveScorer* QQQScorer =
  new  GeTAMU::PS_Silicon_Annular("GeTAMUQQQ",
  QQQ_Wafer_Inner_Radius,
  QQQ_Wafer_Outer_Radius,
  QQQ_Wafer_Stopping_Phi-QQQ_Wafer_Starting_Phi,
  QQQ_Wafer_NumberOf_RadialStrip,
  QQQ_Wafer_NumberOf_AnnularStrip,
  EnergyThreshold);



  //and register it to the multifunctionnal detector
  m_BOXScorer->RegisterPrimitive(BOXScorer);
  m_PADScorer->RegisterPrimitive(PADScorer);
  m_QQQScorer->RegisterPrimitive(QQQScorer);

  //   Add All Scorer to the Global Scorer Manager
  G4SDManager::GetSDMpointer()->AddNewDetector(m_BOXScorer) ;
  G4SDManager::GetSDMpointer()->AddNewDetector(m_PADScorer) ;
  G4SDManager::GetSDMpointer()->AddNewDetector(m_QQQScorer) ;*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////
/////////////////Material Definition ///////////////////////////
////////////////////////////////////////////////////////////////
void GeTAMU::InitializeMaterial(){
  m_MaterialVacuum = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
  m_MaterialGe= MaterialManager::getInstance()->GetMaterialFromLibrary("Ge");
  m_MaterialAl= MaterialManager::getInstance()->GetMaterialFromLibrary("Al"); 
  m_MaterialN2= MaterialManager::getInstance()->GetMaterialFromLibrary("N2_liquid"); 
}


////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* GeTAMU::Construct(){
  return  (NPS::VDetector*) new GeTAMU();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
  class proxy_nps_getamu{
    public:
      proxy_nps_getamu(){
        NPS::DetectorFactory::getInstance()->AddToken("GeTAMU","GeTAMU");
        NPS::DetectorFactory::getInstance()->AddDetector("GeTAMU",GeTAMU::Construct);
      }
  };

  proxy_nps_getamu p_nps_getamu;
}
