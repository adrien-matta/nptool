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
 *  This class describe the Tigress Germanium array                          *
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
#include "Tigress.hh"
//#include "TigressScorers.hh"
#include "MaterialManager.hh"

// NPL
#include "NPOptionManager.h"
#include "RootOutput.h"
using namespace TIGRESS;

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
  const G4double CrystalInterDistance =  0.6*mm; // Distance between two crystal
  
  // Squared part
  const G4double CrystalWidth         = 56.5*mm;  	// Width of one crystal
//  const G4double CrystalsShift        = 1.05*mm;  	// this can't be more than 2.75mm. It is the amount by which one side is cut closer to the center than the other
  
  
  // Bevel part 
 // const G4double CrystalBentLegth     = 3.62*cm;
  // this->germanium_corner_cone_end_length     = 3.0*cm; 	  // the ending length of the cones
  
  
  
  // Exogam Stuff
  const G4double CrystalEdgeOffset1  = 26.0*mm; // distance of the edge from the center of the crystal
  const G4double CrystalEdgeOffset2  = 28.5*mm; // distance of the edge from the center of the crystal
  
//  const G4double CrystalEdgeDepth    = 30.0*mm;  // depth to which the crystal is shaped
 // const G4double CrystalEdgeAngle    = 22.5*deg; // bevel angle
  
  const G4double CapsuleWidth        = 1.5*mm;   // capsule width
  const G4double CapsuleLength       = 110.*mm;   // capsule length
  const G4double CapsuleEdgeDepth    = 3.3*cm;   // same as crystal !
  const G4double CrystalToCapsule    = 3.5*mm;   // to be adjusted ..
  
  //const G4double BGOLength           = 120.0*mm;
  //const G4double BGOWidth            = 25.0*mm;
  
  //const G4double CsILength           = 20.0*mm;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Tigress Specific Method
Tigress::Tigress(){
  InitializeMaterial();
  m_Event = new TTigressData();
  
 BlueVisAtt   = new G4VisAttributes(G4Colour(0, 0, 1)) ;
 GreenVisAtt  = new G4VisAttributes(G4Colour(0, 1, 0)) ;
 RedVisAtt    = new G4VisAttributes(G4Colour(1, 0, 0)) ;
 WhiteVisAtt  = new G4VisAttributes(G4Colour(1, 1, 1)) ;
 TrGreyVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.5)) ;
  
  m_LogicClover = 0;
  
}

Tigress::~Tigress(){
  delete m_MaterialVacuum;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of VDetector class
// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Tigress::ReadConfiguration(string Path){
  ifstream ConfigFile           ;
  ConfigFile.open(Path.c_str()) ;
  string LineBuffer             ;
  string DataBuffer             ;
  istringstream LineStream      ;
  // Standard Case:
  bool check_CloverId = false;
  
  vector<int> CloverId;
  int    CloverId_Free = 0;
  double R     = 0;
  double Theta = 0;
  double Phi   = 0;
  double BetaX;
  double BetaY;
  double BetaZ;
  
  // Free postion case:
  bool check_R   = false ;
  bool check_Theta = false ;
  bool check_Phi = false ;
  bool check_Beta = false ;
  
  // Frame Case
  bool check_RightFrame = false ;
  bool check_LeftFrame = false ;
  
  bool ReadingStatusStandard = false ;
  bool ReadingStatusFree = false ;
  bool ReadingStatusFrame = false ;
  bool ReadingStatus = false ;
  
  while (!ConfigFile.eof()){
    int VerboseLevel = NPOptionManager::getInstance()->GetVerboseLevel();
    
    getline(ConfigFile, LineBuffer);
    
    if (LineBuffer.compare(0, 7, "Tigress") == 0)
      ReadingStatus = true;
    
    while (ReadingStatus && !ConfigFile.eof()) {
      getline(ConfigFile, LineBuffer);
      
      //   Comment Line
      while (LineBuffer.compare(0, 1, "%") == 0) {
        // Take the next line
        getline(ConfigFile, LineBuffer);
      }
      
      //   Standard case
      if (LineBuffer.compare(0, 15, "TigressStandard") == 0){
        if(VerboseLevel==1)
          G4cout << "/// Clovers in Standard Configuration : ///" << G4endl   ;
        ReadingStatusStandard = true ;
      }
      
      //  Free placing case
      else if (LineBuffer.compare(0, 13, "TigressClover") == 0){
        if(VerboseLevel==1)
          G4cout << "/// Free placed clover : ///" << G4endl   ;
        ReadingStatusFree = true ;
      }
      
      //  Frame case
      else if (LineBuffer.compare(0, 12, "TigressFrame") == 0){
        if(VerboseLevel==1)
          G4cout << "/// Support Frame : ///" << G4endl   ;
        ReadingStatusFrame = true ;
      }
      
      //   Reading Block
      while(ReadingStatusStandard){
        // Pickup Next Line
        getline(ConfigFile, LineBuffer);
        //   Comment Line
        while (LineBuffer.compare(0, 1, "%") == 0) {
          // Take the next line
          getline(ConfigFile, LineBuffer);
        }
        
        LineStream.clear();
        LineStream.str(LineBuffer);
        LineStream >> DataBuffer;

        if ( DataBuffer == "CloverId=" ) {
          check_CloverId = true;
                    
          if(VerboseLevel==1) G4cout << "CloverId: " ;
          while(LineStream >> DataBuffer){
            CloverId.push_back(atoi(DataBuffer.c_str()));
            if(VerboseLevel==1) G4cout << atoi(DataBuffer.c_str()) << " ";
          }
          if(VerboseLevel==1) G4cout << G4endl << G4endl;
        }
      
        ///////////////////////////////////////////////////
        //   If no Detector Token and no comment, toggle out
        else{
          ReadingStatusStandard = false;
          G4cout << "Error: Wrong Token Sequence: Getting out " << DataBuffer << G4endl ;
          exit(1);
        }
        
        /////////////////////////////////////////////////
        //   If All necessary information there, toggle out
        
        if (check_CloverId){
          ReadingStatusStandard = false;
          AddCloverStandard(CloverId);
          CloverId.clear();
          check_CloverId   = false ;
        }
      }
      
      //   Reading Block
      while(ReadingStatusFree){
        // Pickup Next Line
        getline(ConfigFile, LineBuffer);
        //   Comment Line
        while (LineBuffer.compare(0, 1, "%") == 0) {
          // Take the next line
          getline(ConfigFile, LineBuffer);
        }
        
        LineStream.clear();
        LineStream.str(LineBuffer);
        LineStream >> DataBuffer;
        
        if ( DataBuffer == "CloverId=" ) {
          check_CloverId = true;
          LineStream >> DataBuffer;
          CloverId_Free = atoi(DataBuffer.c_str());
          if(VerboseLevel==1)
            G4cout << "CloverId: " << atoi(DataBuffer.c_str()) << " " << G4endl ;
        }
        
        else if ( DataBuffer == "R=" ) {
          check_R = true;
          LineStream >> DataBuffer;
          R = atof(DataBuffer.c_str())*mm;
          if(VerboseLevel==1)
            G4cout << "R: " << R/mm << " " << G4endl ;
        }
        
        else if ( DataBuffer == "Theta=" ) {
          check_Theta = true;
          LineStream >> DataBuffer;
          Theta = atof(DataBuffer.c_str())*deg;
          if(VerboseLevel==1)
            G4cout << "Theta: " << Theta/deg << " " << G4endl ;
        }
        
        else if ( DataBuffer == "Phi=" ) {
          check_Phi = true;
          LineStream >> DataBuffer;
          Phi = atof(DataBuffer.c_str())*deg;
          if(VerboseLevel==1)
            G4cout << "Phi: " << Phi/deg << " " << G4endl ;
        }
        
        else if ( DataBuffer == "Beta=" ) {
          check_Beta = true;
          LineStream >> DataBuffer;
          BetaX = atof(DataBuffer.c_str())*deg;
          if(VerboseLevel==1)
            G4cout << "BetaX: " << BetaX/deg << " " << G4endl ;
          LineStream >> DataBuffer;
          BetaY = atof(DataBuffer.c_str())*deg;
          if(VerboseLevel==1)
            G4cout << "BetaY: " << BetaY/deg << " " << G4endl ;
          LineStream >> DataBuffer;
          BetaZ = atof(DataBuffer.c_str())*deg;
          if(VerboseLevel==1)
            G4cout << "BetaZ: " << BetaZ/deg << " " << G4endl ;
        }
        
        
        ///////////////////////////////////////////////////
        //   If no Detector Token and no comment, toggle out
        else{
          ReadingStatusStandard = false;
          G4cout << "Error: Wrong Token Sequence: Getting out " << DataBuffer << G4endl ;
          exit(1);
        }
        
        /////////////////////////////////////////////////
        //   If All necessary information there, toggle out
        
        if (check_CloverId && check_R && check_Theta && check_Phi && check_Beta){
          ReadingStatusFree = false;
          AddCloverFreePosition(CloverId_Free,R,Theta,Phi,BetaX,BetaY,BetaZ);
          check_CloverId = false ;
          check_R = false ;
          check_Theta = false ;
          check_Phi = false ;
          check_Beta = false ;
        }
      }
      
      //   Reading Block
      while(ReadingStatusFrame){
        // Pickup Next Line
        getline(ConfigFile, LineBuffer);
        //   Comment Line
        while (LineBuffer.compare(0, 1, "%") == 0) {
          // Take the next line
          getline(ConfigFile, LineBuffer);
        }
        
        LineStream.clear();
        LineStream.str(LineBuffer);
        LineStream >> DataBuffer;
        
        if ( DataBuffer == "RightFrame=" ) {
          check_RightFrame = true;
          
          LineStream >> DataBuffer;
          m_RightFrame=atoi(DataBuffer.c_str());
          
          if (VerboseLevel==1) {
            if(m_RightFrame)
              G4cout << "Right frame: yes" << G4endl;
            else
              G4cout << "Right frame: no" << G4endl;
	  }
        }
        
        else if ( DataBuffer == "LeftFrame=" ) {
          check_LeftFrame = true;
          
          LineStream >> DataBuffer;
          m_LeftFrame=atoi(DataBuffer.c_str());
          
          if (VerboseLevel==1) {
            if(m_LeftFrame)
              G4cout << "Left frame: yes" << G4endl;
            else
              G4cout << "Left frame: no" << G4endl;
	  }
        }
        
        ///////////////////////////////////////////////////
        //   If no Detector Token and no comment, toggle out
        else{
          ReadingStatusStandard = false;
          G4cout << "Error: Wrong Token Sequence: Getting out " << DataBuffer << G4endl ;
          exit(1);
        }
        
        /////////////////////////////////////////////////
        //   If All necessary information there, toggle out
        
        if (check_RightFrame && check_LeftFrame){
          ReadingStatusFrame = false;
          AddCloverStandard(CloverId);
          CloverId.clear();
          check_RightFrame = false;
          check_LeftFrame = false;
        }
      }
      
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Return a G4VSolid modeling the Crystal
G4VSolid* Tigress::ConstructCrystal(){
  
  
  G4Tubs* Crystal_Cylinder = new G4Tubs("Crystal_Cylinder", 0, CrystalOuterRadius, CrystalLength*0.5, 0, 2*M_PI);
  G4Tubs* Crystal_Hole = new G4Tubs("Crystal_Hole", 0, CrystalInnerRadius, (CrystalLength-CrystalHoleDepth)*0.5, 0, 2*M_PI);
  
  G4SubtractionSolid* Crystal_Stage1 = new G4SubtractionSolid("Crystal_Stage1",Crystal_Cylinder,Crystal_Hole,new G4RotationMatrix,G4ThreeVector(0,0,CrystalHoleDepth));
  
  
  G4Box* Crystal_Box1 = new G4Box("Crystal_Box1", CrystalWidth*0.5, CrystalWidth*0.5,CrystalLength*0.51);

  G4SubtractionSolid* Crystal_Stage2 = new G4SubtractionSolid("Crystal_Stage2",Crystal_Stage1,Crystal_Box1,new G4RotationMatrix,G4ThreeVector(CrystalWidth,0,0));
  G4SubtractionSolid* Crystal_Stage3 = new G4SubtractionSolid("Crystal_Stage3",Crystal_Stage2,Crystal_Box1,new G4RotationMatrix,G4ThreeVector(-CrystalWidth,0,0));
  G4SubtractionSolid* Crystal_Stage4 = new G4SubtractionSolid("Crystal_Stage4",Crystal_Stage3,Crystal_Box1,new G4RotationMatrix,G4ThreeVector(0,CrystalWidth,0));
  G4SubtractionSolid* Crystal_Stage5 = new G4SubtractionSolid("Crystal_Stage5",Crystal_Stage4,Crystal_Box1,new G4RotationMatrix,G4ThreeVector(0,-CrystalWidth,0));
  
    /*const G4double CrystalOuterRadius   = 30.0*mm; // outer radius for crystal
  const G4double CrystalInnerRadius   =  5.0*mm; // inner radius for hole in crystal
  const G4double CrystalLength        = 90.0*mm; // crystal length
  const G4double CrystalHoleDepth     = 15.0*mm; // depth at which starts the hole
  const G4double CrystalInterDistance =  0.6*mm; // Distance between two quarter crystal
     const G4double CrystalWidth         = 56.5*mm;  	// Width of one crystal
     const G4double CrystalsShift        = 1.05*mm;  	// this can't be more than 2.75mm. It is the amount by which one side is cut closer to the
*/
   return  Crystal_Stage5;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Return a G4VSolid modeling the Capsule
G4VSolid* Tigress::ConstructCapsule(){
  
  G4int nbslice = 7;
  const G4double widthface = 45.5*mm;
  G4double zSlice[7] = {  0.0*mm,
    CapsuleWidth-0.1*mm,
    CapsuleWidth,
    CapsuleEdgeDepth,
    CapsuleLength-CapsuleWidth,
    CapsuleLength-CapsuleWidth-0.1*mm,
    CapsuleLength  };
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
  
    G4Polyhedra *caps = new G4Polyhedra(G4String("Capsule"), 45.*deg, 360.*deg, 4, nbslice, zSlice, InnRad, OutRad);
  return caps;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Return a G4VSolid modeling the BGO
G4VSolid* Tigress::ConstructBGO(){
  
  return NULL;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Return a clover in the configuration given by option (not use a the moment)
void Tigress::ConstructClover(string){
  
  if(m_LogicClover==0){
    
    // Construct the clover itself
    G4VSolid*   Capsule   = ConstructCapsule();
    // Place the cristal in the clover
    
    /*const G4double CrystalOuterRadius   = 30.0*mm; // outer radius for crystal
     const G4double CrystalInnerRadius   =  5.0*mm; // inner radius for hole in crystal
     const G4double CrystalLength        = 90.0*mm; // crystal length
     const G4double CrystalHoleDepth     = 15.0*mm; // depth at which starts the hole
     const G4double CrystalInterDistance =  0.6*mm; // Distance between two quarter crystal*/
    
    G4ThreeVector CrystalPosition ;
    double CrystalOffset = (CrystalWidth+CrystalInterDistance)*0.5;
    
    G4VSolid* CrystalB = ConstructCrystal();
    
    m_LogicClover =
    new G4LogicalVolume(Capsule,m_MaterialVacuum,"LogicCloverCase", 0, 0, 0);
    
    G4LogicalVolume* logicCrystalB =
    new G4LogicalVolume(CrystalB,m_MaterialVacuum,"LogicCrystalB", 0, 0, 0);

    G4LogicalVolume* logicCrystalG =
    new G4LogicalVolume(CrystalB,m_MaterialVacuum,"LogicCrystalG", 0, 0, 0);
    
    G4LogicalVolume* logicCrystalR =
    new G4LogicalVolume(CrystalB,m_MaterialVacuum,"LogicCrystalR", 0, 0, 0);
    
    G4LogicalVolume* logicCrystalW =
    new G4LogicalVolume(CrystalB,m_MaterialVacuum,"LogicCrystalW", 0, 0, 0);
    
    G4RotationMatrix* CrystalRotation = new G4RotationMatrix(0,0,0);
    CrystalPosition = G4ThreeVector(CrystalOffset,CrystalOffset,-0.5*CapsuleWidth);
    new G4PVPlacement(G4Transform3D(*CrystalRotation, CrystalPosition),
                      logicCrystalB,"LogicCrystalB",m_LogicClover,false,m_CloverId[0]);
    logicCrystalB->SetVisAttributes(BlueVisAtt);
    
    CrystalRotation->rotate(-180*deg, G4ThreeVector(0,0,1));
    CrystalPosition = G4ThreeVector(+CrystalOffset,-CrystalOffset,0);
    new G4PVPlacement(G4Transform3D(*CrystalRotation, CrystalPosition),
                      logicCrystalG,"LogicCrystalG",m_LogicClover,false,m_CloverId[0]);
    logicCrystalG->SetVisAttributes(GreenVisAtt);
    
    CrystalRotation->rotate(-180*deg, G4ThreeVector(0,0,1));
    CrystalPosition = G4ThreeVector(-CrystalOffset,-CrystalOffset,0);
    new G4PVPlacement(G4Transform3D(*CrystalRotation, CrystalPosition),
                      logicCrystalR,"LogicCrystalR",m_LogicClover,false,m_CloverId[0]);
    logicCrystalR->SetVisAttributes(RedVisAtt);
  
    CrystalRotation->rotate(-180*deg, G4ThreeVector(0,0,1));
    CrystalPosition = G4ThreeVector(-CrystalOffset,CrystalOffset,0);
    new G4PVPlacement(G4Transform3D(*CrystalRotation, CrystalPosition),
                      logicCrystalW,"LogicCrystalW",m_LogicClover,false,m_CloverId[0]);
    logicCrystalW->SetVisAttributes(WhiteVisAtt);
    
    m_LogicClover->SetVisAttributes(TrGreyVisAtt);
  }
  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Tigress::ConstructDetector(G4LogicalVolume* world){
  ConstructClover("");
  
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
                      m_LogicClover,"Clover",world,false,m_CloverId[0]);
  }
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add clover at the standard position of the array
// Take as argument the standard clover Id.
void Tigress::AddCloverStandard(vector<int> CloverId){
  
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
void Tigress::AddCloverFreePosition(int CloverId,double R,double Theta,double Phi,double BetaX,double BetaY,double BetaZ){
  
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
void Tigress::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  pTree->Branch("Tigress", "TTigressData", &m_Event) ;
  pTree->SetBranchAddress("Tigress", &m_Event) ;    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Tigress::ReadSensitive(const G4Event* event){
   event->GetHCofThisEvent(); // event should be used to remove compilation warning
  /*m_Event->Clear();
  
  ///////////
  // BOX
  G4THitsMap<G4double*>*     BOXHitMap;
  std::map<G4int, G4double**>::iterator    BOX_itr;
  
  G4int BOXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Tigress_BOXScorer/TigressBOX");
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
  
  G4int PADCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Tigress_PADScorer/TigressPAD");
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
  
  G4int QQQCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Tigress_QQQScorer/TigressQQQ");
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
void Tigress::InitializeScorers(){
  /*
  //   Silicon Associate Scorer
  m_BOXScorer = new G4MultiFunctionalDetector("Tigress_BOXScorer");
  m_PADScorer = new G4MultiFunctionalDetector("Tigress_PADScorer");
  m_QQQScorer = new G4MultiFunctionalDetector("Tigress_QQQScorer");
  
  G4VPrimitiveScorer* BOXScorer =
  new  Tigress::PS_Silicon_Rectangle("TigressBOX",
                                   BOX_Wafer_Length,
                                   BOX_Wafer_Width,
                                   BOX_Wafer_Back_NumberOfStrip ,
                                   BOX_Wafer_Front_NumberOfStrip,
                                   EnergyThreshold);
  
  G4VPrimitiveScorer* PADScorer =
  new  Tigress::PS_Silicon_Rectangle("TigressPAD",
                                   PAD_Wafer_Length,
                                   PAD_Wafer_Width,
                                   1 ,
                                   1,
                                   EnergyThreshold);
  
  G4VPrimitiveScorer* QQQScorer =
  new  Tigress::PS_Silicon_Annular("TigressQQQ",
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
void Tigress::InitializeMaterial(){
  m_MaterialVacuum = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
}


