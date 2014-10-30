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
#include "G4Material.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4Colour.hh"
#include "G4PVDivision.hh"
#include "G4SubtractionSolid.hh"

// NPS
#include "Tiara.hh"
#include "MaterialManager.hh"
#include "ResistiveStripScorers.hh"
#include "SiliconScorers.hh"

// NPL
#include "NPOptionManager.h"
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
  // Light Blue
  GuardRingVisAtt = new G4VisAttributes(G4Colour(0.0, 0.8, 0.9)) ;  

  m_boolChamber = false;
  m_boolInner = false;
  m_boolOuter = false;

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Tiara::~Tiara(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of VDetector class
// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Tiara::ReadConfiguration(string Path){
  ifstream ConfigFile           ;
  ConfigFile.open(Path.c_str()) ;
  string LineBuffer             ;
  string DataBuffer             ;
  bool ReadingStatus = false    ;

  int VerboseLevel = NPOptionManager::getInstance()->GetVerboseLevel();

  while (getline(ConfigFile, LineBuffer)){
    
    if (LineBuffer.compare(0, 5, "Tiara") == 0)
      ReadingStatus = true;

    while (ReadingStatus && ConfigFile >> DataBuffer ) {
      //   Comment Line
      if (DataBuffer.compare(0, 1, "%") == 0) { ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

      // Tiara Chamber
      else if (DataBuffer=="TiaraChamber="){
        if(VerboseLevel==1) G4cout << "Chamber Found " << G4endl   ;
        ConfigFile >> m_boolChamber;
      }

      // Inner Barrel case
      else if (DataBuffer=="TiaraOuterBarrel="){
        if(VerboseLevel==1) G4cout << "Inner Barrel found " << G4endl   ;
        ConfigFile >> m_boolInner;
      }

      // Outter Barrel case
      else if (DataBuffer=="TiaraInnerBarrel="){
        if(VerboseLevel==1) G4cout << "Outer Barrel found " << G4endl   ;
        ConfigFile >> m_boolOuter;
      }

      // Hyball case
      else if (DataBuffer=="TiaraHyball") { 
        if(VerboseLevel==1) G4cout << "Hyball  found: " << G4endl   ;
      }
    }
  }

  ConfigFile.close();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Tiara::ConstructDetector(G4LogicalVolume* world){

  if(m_boolChamber)
    ConstructChamber(world);
  
  if(m_boolInner)
    ConstructInnerBarrel(world);
  
  if(m_boolOuter)
    ConstructOuterBarrel(world);
  
  ConstructHyball(world);
}
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Tiara::ReadSensitive(const G4Event* event){
  m_EventBarrel->Clear();
  m_EventHyball->Clear();

  // InnerBarrel //
  G4THitsMap<G4double*>* InnerBarrelHitMap;
  std::map<G4int, G4double**>::iterator InnerBarrel_itr;
  G4int InnerBarrelCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Tiara_InnerBarrelScorer/InnerBarrel");
  InnerBarrelHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(InnerBarrelCollectionID));

  // Loop on the InnerBarrel map 
  for (InnerBarrel_itr = InnerBarrelHitMap->GetMap()->begin() ; InnerBarrel_itr != InnerBarrelHitMap->GetMap()->end() ; InnerBarrel_itr++){
    G4double* Info = *(InnerBarrel_itr->second); 

    // Upstream Energy
    double EU = RandGauss::shoot(Info[0],ResoEnergy);
    if(EU>EnergyThreshold){
      m_EventBarrel->SetFrontUpstreamE(Info[3],Info[4],EU);
      m_EventBarrel->SetFrontUpstreamT(Info[3],Info[4],Info[2]); 
    }

    // Downstream Energy
    double ED = RandGauss::shoot(Info[1],ResoEnergy); 
    if(ED>EnergyThreshold){
      m_EventBarrel->SetFrontDownstreamE(Info[3],Info[4],ED);
      m_EventBarrel->SetFrontDownstreamT(Info[3],Info[4],Info[2]); 
    }

    // Back Energy
    double EB = RandGauss::shoot(Info[1]+Info[0],ResoEnergy);
    if(ED>EnergyThreshold){
      m_EventBarrel->SetBackE(Info[3],EB);
      m_EventBarrel->SetBackT(Info[3],Info[2]); 
    }

  }
  // Clear Map for next event
  InnerBarrelHitMap->clear();

  // OuterBarrel //
  G4THitsMap<G4double*>* OuterBarrelHitMap;
  std::map<G4int, G4double**>::iterator OuterBarrel_itr;
  G4int OuterBarrelCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Tiara_OuterBarrelScorer/OuterBarrel");
  OuterBarrelHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(OuterBarrelCollectionID));

  // Loop on the OuterBarrel map 
  for (OuterBarrel_itr = OuterBarrelHitMap->GetMap()->begin() ; OuterBarrel_itr != OuterBarrelHitMap->GetMap()->end() ; OuterBarrel_itr++){
    G4double* Info = *(OuterBarrel_itr->second); 

    double E = RandGauss::shoot(Info[0],ResoEnergy);
    if(E>EnergyThreshold){
      m_EventBarrel->SetOuterE(Info[7],Info[9],E);
      m_EventBarrel->SetOuterT(Info[7],Info[9],Info[1]); 
    }
  }
  // Clear Map for next event
  OuterBarrelHitMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Tiara::InitializeScorers(){

  m_InnerBarrelScorer = new G4MultiFunctionalDetector("Tiara_InnerBarrelScorer");
  m_OuterBarrelScorer = new G4MultiFunctionalDetector("Tiara_OuterBarrelScorer");

  G4VPrimitiveScorer* InnerBarrel = new SILICONSCORERS::PS_Silicon_Resistive("InnerBarrel",
      INNERBARREL_ActiveWafer_Length,
      INNERBARREL_ActiveWafer_Width,
      INNERBARREL_NumberOfStrip);

  m_InnerBarrelScorer->RegisterPrimitive(InnerBarrel);

  G4VPrimitiveScorer* OuterBarrel = new SILICONSCORERS::PS_Silicon_Rectangle("OuterBarrel",
      INNERBARREL_ActiveWafer_Length,
      INNERBARREL_ActiveWafer_Width,
      1,
      OUTERBARREL_NumberOfStrip);

  m_OuterBarrelScorer->RegisterPrimitive(OuterBarrel);


  //   Add All Scorer to the Global Scorer Manager 
  G4SDManager::GetSDMpointer()->AddNewDetector(m_InnerBarrelScorer) ;
  G4SDManager::GetSDMpointer()->AddNewDetector(m_OuterBarrelScorer) ;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Tiara::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance(); 
  TTree *pTree = pAnalysis->GetTree();   
  pTree->Branch("TiaraBarrel", "TTiaraBarrelData", &m_EventBarrel) ;
  pTree->Branch("TiaraHyball", "TTiaraHyballData", &m_EventHyball) ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Tiara::ConstructInnerBarrel(G4LogicalVolume* world){
  // Tiara Barrel
  // The Barrel is made of 8 identical resistive strip detector
  // The PCB is made from a G4ExtrudeSolid, because it has beveled edge
  // the pcb is a substracted volume
  // the wafer goes into the hole, but a 15mm part is still covered by some PCB
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

  // Calculate the hole shift within the PCB
  G4ThreeVector HoleShift = G4ThreeVector(
      0,
      0,
      INNERBARREL_PCB_Offset-(INNERBARREL_PCB_Length/2-INNERBARREL_PCB_HoleLength/2));

  G4Box*  HoleShape = new G4Box("HoleShape",
      INNERBARREL_ActiveWafer_Width/2.,
      INNERBARREL_PCB_Thickness/2.+0.1*mm,
      INNERBARREL_PCB_HoleLength/2.);

  G4Box*  WaferShape = new G4Box("WaferShape",
      INNERBARREL_InertWafer_Width/2.,
      INNERBARREL_PCB_Thickness/2.,
      INNERBARREL_InertWafer_Length/2.);

  // The Silicon Wafer itself
  G4Box*  InertWaferFull = new G4Box("InertWaferFull",
      INNERBARREL_InertWafer_Width/2.,
      INNERBARREL_ActiveWafer_Thickness/2.,
      INNERBARREL_InertWafer_Length/2.);

  G4Box*  ActiveWafer = new G4Box("ActiveWafer",
      INNERBARREL_ActiveWafer_Width/2.,
      INNERBARREL_ActiveWafer_Thickness/2.,
      INNERBARREL_ActiveWafer_Length/2.);

  G4Box*  ActiveWaferShape = new G4Box("ActiveWaferShape",
      INNERBARREL_ActiveWafer_Width/2.,
      INNERBARREL_PCB_Thickness/2.,
      INNERBARREL_ActiveWafer_Length/2.);


  // Substracting the hole Shape from the Stock PCB
  G4SubtractionSolid* PCB_1 = new G4SubtractionSolid("PCB_1", PCBFull, HoleShape,
      new G4RotationMatrix,HoleShift);

  // Substracting the wafer space from the Stock PCB
  G4SubtractionSolid* PCB = new G4SubtractionSolid("PCB", PCB_1, WaferShape,
      new G4RotationMatrix,
      G4ThreeVector(0,INNERBARREL_PCB_Thickness/2.-INNERBARREL_PCB_WaferDepth,0));

  // Substract active part from inert part of the Wafer
  G4SubtractionSolid* InertWafer = new G4SubtractionSolid("InertWafer", InertWaferFull, ActiveWaferShape,
      new G4RotationMatrix,
      G4ThreeVector(0,0,0));

  // Master Volume that encompass everything else
  G4LogicalVolume* logicBarrelDetector =
    new G4LogicalVolume(PCBFull,m_MaterialVacuum,"logicBoxDetector", 0, 0, 0);
  logicBarrelDetector->SetVisAttributes(G4VisAttributes::Invisible);

  // Sub Volume PCB
  G4LogicalVolume* logicPCB =
    new G4LogicalVolume(PCB,m_MaterialPCB,"logicPCB", 0, 0, 0);
  logicPCB->SetVisAttributes(PCBVisAtt);

  // Sub Volume Wafer
  G4LogicalVolume* logicInertWafer =
    new G4LogicalVolume(InertWafer,m_MaterialSilicon,"logicInertWafer", 0, 0, 0);
  logicInertWafer->SetVisAttributes(GuardRingVisAtt);

  G4LogicalVolume* logicActiveWafer =
    new G4LogicalVolume(ActiveWafer,m_MaterialSilicon,"logicActiveWafer", 0, 0, 0);
  logicActiveWafer->SetVisAttributes(SiliconVisAtt);

  // Set the sensitive volume
  logicActiveWafer->SetSensitiveDetector(m_InnerBarrelScorer);

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


    G4ThreeVector WaferPosition(0,0.5*(INNERBARREL_PCB_Thickness-INNERBARREL_PCB_WaferDepth+INNERBARREL_ActiveWafer_Thickness),0); 

    new G4PVPlacement(new G4RotationMatrix(0,0,0),
        WaferPosition,
        logicActiveWafer,"Barrel_Wafer",
        logicBarrelDetector,false,i+1);

    new G4PVPlacement(new G4RotationMatrix(0,0,0),
        WaferPosition,
        logicInertWafer,"Barrel_Wafer_GuardRing",
        logicBarrelDetector,false,i+1);


    // The following build the barrel, with detector one at the top
    // and going clowise looking upstrea

    // Detector are rotate by 45deg with each other 
    G4RotationMatrix* DetectorRotation = 
      new G4RotationMatrix(0*deg,0*deg,i*45*deg);

    // There center is also rotated by 45deg
    G4ThreeVector DetectorPosition(0,DistanceFromTarget,0);
    DetectorPosition.rotate(i*45*deg,G4ThreeVector(0,0,-1));   

    // Place the Master volume with its two daugther volume at the final place 
    new G4PVPlacement(G4Transform3D(*DetectorRotation,DetectorPosition),
        logicBarrelDetector,"Tiara_Barrel_Detector",
        world,false,i+1);
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Tiara::ConstructOuterBarrel(G4LogicalVolume* world){
  // Tiara Outer Barrel
  // The outer Barrel is identical to the inner barrel but wider in terms of
  // geometry. It feature four non resistive strip on the front face

  // Start by making a full pcb
  // We start by the definition of the point forming a PCB cross section
  vector<G4TwoVector> PCBCrossSection;
  double l1 = OUTERBARREL_PCB_Thickness*0.5/tan(OUTERBARREL_PCB_Bevel1_Theta);
  double l2 = OUTERBARREL_PCB_Thickness*0.5/tan(OUTERBARREL_PCB_Bevel2_Theta);

  PCBCrossSection.push_back(G4TwoVector(OUTERBARREL_PCB_Width/2.-l2,-OUTERBARREL_PCB_Thickness*0.5));
  PCBCrossSection.push_back(G4TwoVector(OUTERBARREL_PCB_Width/2.,0));
  PCBCrossSection.push_back(G4TwoVector(OUTERBARREL_PCB_Width/2.-l1,OUTERBARREL_PCB_Thickness*0.5));

  PCBCrossSection.push_back(G4TwoVector(-OUTERBARREL_PCB_Width/2.+l1,OUTERBARREL_PCB_Thickness*0.5));
  PCBCrossSection.push_back(G4TwoVector(-OUTERBARREL_PCB_Width/2.,0));
  PCBCrossSection.push_back(G4TwoVector(-OUTERBARREL_PCB_Width/2.+l2,-OUTERBARREL_PCB_Thickness*0.5));

  G4ExtrudedSolid* PCBFull = 
    new G4ExtrudedSolid("PCBFull",
        PCBCrossSection,
        OUTERBARREL_PCB_Length/2.,
        G4TwoVector(0,0),1,
        G4TwoVector(0,0),1);

  // A box having Wafer dimension but thicker than the PCB
  // Will be used to remove material from the PCB to have space for the wafer

  // Calculate the hole shift within the PCB
  G4ThreeVector HoleShift = G4ThreeVector(
      0,
      0,
      OUTERBARREL_PCB_Offset-(OUTERBARREL_PCB_Length/2-OUTERBARREL_PCB_HoleLength/2));

  G4Box*  HoleShape = new G4Box("HoleShape",
      OUTERBARREL_ActiveWafer_Width/2.,
      OUTERBARREL_PCB_Thickness/2.+0.1*mm,
      OUTERBARREL_PCB_HoleLength/2.);

  G4Box*  WaferShape = new G4Box("WaferShape",
      OUTERBARREL_InertWafer_Width/2.,
      OUTERBARREL_PCB_Thickness/2.,
      OUTERBARREL_InertWafer_Length/2.);

  // The Silicon Wafer itself
  G4Box*  InertWaferFull = new G4Box("InertWaferFull",
      OUTERBARREL_InertWafer_Width/2.,
      OUTERBARREL_ActiveWafer_Thickness/2.,
      OUTERBARREL_InertWafer_Length/2.);

  G4Box*  ActiveWafer = new G4Box("ActiveWafer",
      OUTERBARREL_ActiveWafer_Width/2.,
      OUTERBARREL_ActiveWafer_Thickness/2.,
      OUTERBARREL_ActiveWafer_Length/2.);

  G4Box*  ActiveWaferShape = new G4Box("ActiveWaferShape",
      OUTERBARREL_ActiveWafer_Width/2.,
      OUTERBARREL_PCB_Thickness/2.,
      OUTERBARREL_ActiveWafer_Length/2.);


  // Substracting the hole Shape from the Stock PCB
  G4SubtractionSolid* PCB_1 = new G4SubtractionSolid("PCB_1", PCBFull, HoleShape,
      new G4RotationMatrix,HoleShift);

  // Substracting the wafer space from the Stock PCB
  G4SubtractionSolid* PCB = new G4SubtractionSolid("PCB", PCB_1, WaferShape,
      new G4RotationMatrix,
      G4ThreeVector(0,OUTERBARREL_PCB_Thickness/2.-OUTERBARREL_PCB_WaferDepth,0));

  // Substract active part from inert part of the Wafer
  G4SubtractionSolid* InertWafer = new G4SubtractionSolid("InertWafer", InertWaferFull, ActiveWaferShape,
      new G4RotationMatrix,
      G4ThreeVector(0,0,0));

  // Master Volume that encompass everything else
  G4LogicalVolume* logicBarrelDetector =
    new G4LogicalVolume(PCBFull,m_MaterialVacuum,"logicBoxDetector", 0, 0, 0);
  logicBarrelDetector->SetVisAttributes(G4VisAttributes::Invisible);

  // Sub Volume PCB
  G4LogicalVolume* logicPCB =
    new G4LogicalVolume(PCB,m_MaterialPCB,"logicPCB", 0, 0, 0);
  logicPCB->SetVisAttributes(PCBVisAtt);

  // Sub Volume Wafer
  G4LogicalVolume* logicInertWafer =
    new G4LogicalVolume(InertWafer,m_MaterialSilicon,"logicInertWafer", 0, 0, 0);
  logicInertWafer->SetVisAttributes(GuardRingVisAtt);

  G4LogicalVolume* logicActiveWafer =
    new G4LogicalVolume(ActiveWafer,m_MaterialSilicon,"logicActiveWafer", 0, 0, 0);
  logicActiveWafer->SetVisAttributes(SiliconVisAtt);

  // Set the sensitive detector
  logicActiveWafer->SetSensitiveDetector(m_OuterBarrelScorer);

  // The Distance from target is given by half the lenght of a detector
  // plus the length of a detector inclined by 45 deg.
  G4double DistanceFromTarget = OUTERBARREL_PCB_Width*(0.5+sin(45*deg)) ; 

  for( unsigned int i = 0; i < 8; i ++){
    // Place the sub volumes in the master volume
    // Last argument is the detector number, used in the scorer to get the
    // revelant information
    new G4PVPlacement(new G4RotationMatrix(0,0,0),
        G4ThreeVector(0,0,0),
        logicPCB,"Tiara_Barrel_PCB",logicBarrelDetector,
        false,i+1);


    G4ThreeVector WaferPosition(0,0.5*(OUTERBARREL_PCB_Thickness-OUTERBARREL_PCB_WaferDepth+OUTERBARREL_ActiveWafer_Thickness),0); 

    new G4PVPlacement(new G4RotationMatrix(0,0,0),
        WaferPosition,
        logicActiveWafer,"Barrel_Wafer",
        logicBarrelDetector,false,i+1);

    new G4PVPlacement(new G4RotationMatrix(0,0,0),
        WaferPosition,
        logicInertWafer,"Barrel_Wafer_GuardRing",
        logicBarrelDetector,false,i+1);


    // The following build the barrel, with detector one at the top
    // and going clowise looking upstrea

    // Detector are rotate by 45deg with each other 
    G4RotationMatrix* DetectorRotation = 
      new G4RotationMatrix(0*deg,0*deg,i*45*deg);

    // There center is also rotated by 45deg
    G4ThreeVector DetectorPosition(0,DistanceFromTarget,0);
    DetectorPosition.rotate(i*45*deg,G4ThreeVector(0,0,-1));   

    // Place the Master volume with its two daugther volume at the final place 
    new G4PVPlacement(G4Transform3D(*DetectorRotation,DetectorPosition),
        logicBarrelDetector,"Tiara_Barrel_Detector",
        world,false,i+1);
  }

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Tiara::ConstructHyball(G4LogicalVolume* world){

  // TO BE DONE //

  // Put the needed geometry parameter definition here instead of the namespace
  // to facilitate the merge
  // Respect Naming convention: example HYBALL_PCB_Radius / HYBALL_ActiveWafer_Radius
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Tiara::ConstructChamber(G4LogicalVolume* world){
  // Vaccum Chamber of Tiara
  // The chamber is made of a central cylinder surrounding the barrel Si
  // Two Cone that expeand out of the central cylinder to let room for Exogam
  // Two outer cylinder surrounding Hyball
  // Hyball is hold on a back plate that close the Diabolo Shaped Chamber

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
    new G4LogicalVolume(solidTiaraChamberStep4,m_MaterialAl,"logicTiaraChamber", 0, 0, 0);

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
void Tiara::InitializeMaterial(){
  m_MaterialSilicon = MaterialManager::getInstance()->GetMaterialFromLibrary("Si");
  m_MaterialAl = MaterialManager::getInstance()->GetMaterialFromLibrary("Al"); 
  m_MaterialPCB = MaterialManager::getInstance()->GetMaterialFromLibrary("PCB");
  m_MaterialVacuum = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
}





