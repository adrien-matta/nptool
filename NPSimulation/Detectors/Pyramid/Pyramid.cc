/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Mohamad Moukaddam                                        *
 * contact address: mohamad.moukaddam@iphc.cnrs.fr                           *
 *                                                                           *
 * Creation Date  : November 2018                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  Pyramid simulation                                  *
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
#include "Pyramid.hh"
#include "NPSDetectorFactory.hh"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "SiliconScorers.hh"

// NPL
#include "NPOptionManager.h"
#include "RootOutput.h"
using namespace PYRAMID;

// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Pyramid::Pyramid(){
  InitializeMaterial();
  m_EventPyramid = new TPyramidData();

  // Dark Grey
  SiliconVisAtt = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3)) ;
  // Green
  PCBVisAtt = new G4VisAttributes(G4Colour(0.2, 0.5, 0.2)) ;
  // Gold Yellow
  PADVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.2)) ;
  // Light Grey
  FrameVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)) ;
  // Light Blue
  GuardRingVisAtt = new G4VisAttributes(G4Colour(0.1, 0.1, 0.1)) ;

  m_boolChamber = false;
  m_boolInner.clear();
  m_boolOuter.clear();
  m_ANGLE.clear();
  m_Z.clear();
    
  m_InnerScorer = 0 ;
  m_OuterScorer = 0 ;

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Pyramid::~Pyramid(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class
// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Pyramid::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Pyramid");

  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl;

  vector<string> token = {"Inner","Outer","Z","ANGLE"};
      
  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Pyramid " << i+1 <<  endl;
      m_boolInner.push_back(blocks[i]->GetInt("Inner"));
      m_boolOuter.push_back(blocks[i]->GetInt("Outer"));
      //m_boolChamber   = blocks[i]->GetInt("Chamber");
      m_Z.push_back(blocks[i]->GetDouble("Z","mm"));
      m_ANGLE.push_back(blocks[i]->GetDouble("ANGLE","deg"));
    }

    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }

  blocks.clear();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Pyramid::ConstructDetector(G4LogicalVolume* world){

  if(m_boolChamber)
    ConstructChamber(world);

  for (unsigned i = 0 ; i < m_boolInner.size() ; i++)
    if(m_boolInner[i])
      ConstructInner(world, m_ANGLE[i], m_Z[i]);

  for (unsigned i = 0 ; i < m_boolOuter.size() ; i++)
    if(m_boolOuter[i])
      ConstructOuter(world, m_ANGLE[i], m_Z[i]);

}
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Pyramid::ReadSensitive(const G4Event* event){
  m_EventPyramid->Clear();


  // Inner //
  NPS::HitsMap<G4double*>* InnerHitMap;
  std::map<G4int, G4double**>::iterator Inner_itr;
  G4int InnerCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Pyramid_InnerScorer/Inner");
  InnerHitMap = (NPS::HitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(InnerCollectionID));

  // Loop on the Inner map
  for (Inner_itr = InnerHitMap->GetMap()->begin() ; Inner_itr != InnerHitMap->GetMap()->end() ; Inner_itr++){
    G4double* Info = *(Inner_itr->second);

    // Downstream Energy
    double ED = RandGauss::shoot(Info[0],ResoEnergyInner);
    if(ED>EnergyThreshold){
      m_EventPyramid->SetFrontDownstreamE(Info[3],Info[4],ED);
      m_EventPyramid->SetFrontDownstreamT(Info[3],Info[4],Info[2]);
    }

    // Upstream Energy
    double EU = RandGauss::shoot(Info[1],ResoEnergyInner);
    if(EU>EnergyThreshold){
      m_EventPyramid->SetFrontUpstreamE(Info[3],Info[4],EU);
      m_EventPyramid->SetFrontUpstreamT(Info[3],Info[4],Info[2]);
	//cout << "Pyramid " << Info[3] << " STRIP " << Info[4] << " " << Info[9]/deg << " " << Info[5] << " " << Info[6] << " " << Info[7] << endl;
    }

    // Back Energy
    double EB = RandGauss::shoot(Info[1]+Info[0],ResoEnergyInner);
    if(EB>EnergyThreshold){
      m_EventPyramid->SetBackE(Info[3],EB);
      m_EventPyramid->SetBackT(Info[3],Info[2]);
    }
        // Interaction Coordinates
    ms_InterCoord->SetDetectedPositionX(Info[5]) ;
    ms_InterCoord->SetDetectedPositionY(Info[6]) ;
    ms_InterCoord->SetDetectedPositionZ(Info[7]) ;
    ms_InterCoord->SetDetectedAngleTheta(Info[8]/deg) ;
    ms_InterCoord->SetDetectedAnglePhi(Info[9]/deg) ;
  }
  // Clear Map for next event
  InnerHitMap->clear();

  // Outer //
  NPS::HitsMap<G4double*>* OuterHitMap;
  std::map<G4int, G4double**>::iterator Outer_itr;
  G4int OuterCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Pyramid_OuterScorer/Outer");
  OuterHitMap = (NPS::HitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(OuterCollectionID));

  // Loop on the Outer map
  for (Outer_itr = OuterHitMap->GetMap()->begin() ; Outer_itr != OuterHitMap->GetMap()->end() ; Outer_itr++){
    G4double* Info = *(Outer_itr->second);

    double E = RandGauss::shoot(Info[0]/keV,ResoEnergyOuter);
    if(E>EnergyThreshold){
      m_EventPyramid->SetOuterE(Info[7],Info[9],E);
      m_EventPyramid->SetOuterT(Info[7],Info[9],Info[1]);
    }
        // Interaction Coordinates
    ms_InterCoord->SetDetectedPositionX(Info[5]) ;
    ms_InterCoord->SetDetectedPositionY(Info[6]) ;
    ms_InterCoord->SetDetectedPositionZ(Info[7]) ;
    ms_InterCoord->SetDetectedAngleTheta(Info[8]/deg) ;
    ms_InterCoord->SetDetectedAnglePhi(Info[9]/deg) ;
  }
  // Clear Map for next event
  OuterHitMap->clear();


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Pyramid::InitializeScorers(){
  //Look for previous definition of the scorer (geometry reload)
  bool already_exist = false;
  m_InnerScorer = CheckScorer("Pyramid_InnerScorer",already_exist);
  m_OuterScorer = CheckScorer("Pyramid_OuterScorer",already_exist);


  // if the scorer were created previously nothing else need to be made
  if(already_exist) return;

  G4VPrimitiveScorer* Inner = new SILICONSCORERS::PS_Silicon_Resistive("Inner",1,
      Inner_ActiveWafer_Length,
      Inner_ActiveWafer_Width,
      Inner_NumberOfStrip);

  m_InnerScorer->RegisterPrimitive(Inner);

  G4VPrimitiveScorer* Outer = new SILICONSCORERS::PS_Silicon_Rectangle("Outer",1,
      Inner_ActiveWafer_Length,
      Inner_ActiveWafer_Width,
      1,
      Outer_NumberOfStrip);

  m_OuterScorer->RegisterPrimitive(Outer);


  //   Add All Scorer to the Global Scorer Manager
  G4SDManager::GetSDMpointer()->AddNewDetector(m_InnerScorer) ;
  G4SDManager::GetSDMpointer()->AddNewDetector(m_OuterScorer) ;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Pyramid::InitializeRootOutput(){
  TTree *pTree = RootOutput::getInstance()->GetTree();
  if(!pTree->FindBranch("Pyramid")){
    pTree->Branch("Pyramid", "TPyramidData", &m_EventPyramid) ;
  }


  // This insure that the object are correctly bind in case of
  // a redifinition of the geometry in the simulation
  pTree->SetBranchAddress("Pyramid", &m_EventPyramid) ;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Pyramid::ConstructInner(G4LogicalVolume* world, double angle, double z){
  // Pyramid Pyramid
  // The Pyramid is made of 4 identical resistive strip detector
  // The PCB is made from a G4ExtrudeSolid, because it has beveled edge
  // the pcb is a substracted volume
  // the wafer goes into the hole, but a 15mm part is still covered by some PCB
  // the whole things is design so the local reference is the one of the wafer

  // Start by making a full pcb
  // We start by the definition of the point forming a PCB cross section
  vector<G4TwoVector> PCBCrossSection;
  double l1 = Inner_PCB_Thickness*0.5/tan(Inner_PCB_Bevel1_Theta);
  double l2 = Inner_PCB_Thickness*0.5/tan(Inner_PCB_Bevel2_Theta);

  PCBCrossSection.push_back(G4TwoVector(Inner_PCB_Width/2.-l2,-Inner_PCB_Thickness*0.5));
  PCBCrossSection.push_back(G4TwoVector(Inner_PCB_Width/2.,0));
  PCBCrossSection.push_back(G4TwoVector(Inner_PCB_Width/2.-l1,Inner_PCB_Thickness*0.5));

  PCBCrossSection.push_back(G4TwoVector(-Inner_PCB_Width/2.+l1,Inner_PCB_Thickness*0.5));
  PCBCrossSection.push_back(G4TwoVector(-Inner_PCB_Width/2.,0));
  PCBCrossSection.push_back(G4TwoVector(-Inner_PCB_Width/2.+l2,-Inner_PCB_Thickness*0.5));

  G4ExtrudedSolid* PCBFull =
    new G4ExtrudedSolid("PCBFull",
        PCBCrossSection,
        Inner_PCB_Length/2.,
        G4TwoVector(0,0),1,
        G4TwoVector(0,0),1);

  // A box having Wafer dimension but thicker than the PCB
  // Will be used to remove material from the PCB to have space for the wafer

  // Calculate the hole shift within the PCB
  G4ThreeVector HoleShift = G4ThreeVector(
      0,
      0,
      Inner_PCB_Offset-(Inner_PCB_Length/2-Inner_PCB_HoleLength/2));

  G4Box*  HoleShape = new G4Box("HoleShape",
      Inner_ActiveWafer_Width/2.,
      Inner_PCB_Thickness/2.+0.1*mm,
      Inner_PCB_HoleLength/2.);

  G4Box*  WaferShape = new G4Box("WaferShape",
      Inner_InertWafer_Width/2.,
      Inner_PCB_Thickness/2.,
      Inner_InertWafer_Length/2.);

  // The Silicon Wafer itself
  G4Box*  InertWaferFull = new G4Box("InertWaferFull",
      Inner_InertWafer_Width/2.,
      Inner_InertWafer_Thickness/2.,
      Inner_InertWafer_Length/2.);

  G4Box*  ActiveWafer = new G4Box("ActiveWafer",
      Inner_ActiveWafer_Width/2.,
      Inner_ActiveWafer_Thickness/2.,
      Inner_ActiveWafer_Length/2.);

  G4Box*  DeadLayer = new G4Box("DeadLayer",
      Inner_ActiveWafer_Width/2.,
      Inner_ActiveWafer_DeadLayerThickness/2.,
      Inner_ActiveWafer_Length/2.);

  G4Box*  ActiveWaferShape = new G4Box("ActiveWaferShape",
      Inner_ActiveWafer_Width/2.,
      Inner_PCB_Thickness/2.,
      Inner_ActiveWafer_Length/2.);


  // Substracting the hole Shape from the Stock PCB
  G4SubtractionSolid* PCB_1 = new G4SubtractionSolid("PCB_1", PCBFull, HoleShape,
      new G4RotationMatrix,HoleShift);

  // Substracting the wafer space from the Stock PCB
  G4SubtractionSolid* PCB = new G4SubtractionSolid("PCB", PCB_1, WaferShape,
      new G4RotationMatrix,
      G4ThreeVector(0,Inner_PCB_Thickness/2.-Inner_PCB_WaferDepth,0));

  // Substract active part from inert part of the Wafer
  G4SubtractionSolid* InertWafer = new G4SubtractionSolid("InertWafer", InertWaferFull, ActiveWaferShape,
      new G4RotationMatrix,
      G4ThreeVector(0,0,0));

  // Master Volume that encompass everything else
  G4LogicalVolume* logicPyramidDetector =
    new G4LogicalVolume(PCBFull,m_MaterialVacuum,"logicBoxDetector", 0, 0, 0);
  logicPyramidDetector->SetVisAttributes(G4VisAttributes::Invisible);

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

  G4LogicalVolume* logicDeadLayer =
    new G4LogicalVolume(DeadLayer,m_MaterialSilicon,"logicActiveWaferDeadLayer", 0, 0, 0);
  logicDeadLayer->SetVisAttributes(SiliconVisAtt);

  // Set the sensitive volume
  logicActiveWafer->SetSensitiveDetector(m_InnerScorer);

  // Place the sub volumes in the master volume
  // Last argument is the detector number, used in the scorer to get the
  // revelant information
  new G4PVPlacement(new G4RotationMatrix(0,0,0),
      G4ThreeVector(0,0,0),
      logicPCB,"Pyramid_Pyramid_PCB",logicPyramidDetector,
      false,0);

  G4ThreeVector WaferPosition(0,
      0.5*(Inner_PCB_Thickness+Inner_InertWafer_Thickness)-Inner_PCB_WaferDepth
      ,0);

  G4ThreeVector DeadLayerPositionF = WaferPosition + G4ThreeVector(0,-Inner_ActiveWafer_Thickness*0.5-Inner_ActiveWafer_DeadLayerThickness*0.5,0);

  G4ThreeVector DeadLayerPositionB = WaferPosition + G4ThreeVector(0,Inner_ActiveWafer_Thickness*0.5+Inner_ActiveWafer_DeadLayerThickness*0.5,0);



  new G4PVPlacement(new G4RotationMatrix(0,0,0),
      WaferPosition,
      logicActiveWafer,"Pyramid_Wafer",
      logicPyramidDetector,false,0);

  new G4PVPlacement(new G4RotationMatrix(0,0,0),
      DeadLayerPositionF,
      logicDeadLayer,"Pyramid_WaferDeadLayerFront",
      logicPyramidDetector,false,0);

  new G4PVPlacement(new G4RotationMatrix(0,0,0),
      DeadLayerPositionB,
      logicDeadLayer,"Pyramid_WaferDeadLayerBack",
      logicPyramidDetector,false,0);


  new G4PVPlacement(new G4RotationMatrix(0,0,0),
      WaferPosition,
      logicInertWafer,"Pyramid_Wafer_GuardRing",
      logicPyramidDetector,false,0);

  // The Distance from target is given by half the width of a detector
  G4double DistanceFromTarget = Inner_PCB_Width*(0.5)      
                                   + Inner_PCB_Length*(0.5)*fabs(sin(angle)) ;
  for( unsigned int i = 0; i < 4; i ++){
    // The following builds the Pyramid, with detector one at the top
    // and going anticlockwise looking downstream
    double sign = -z/fabs(z);
    // Detector are rotate by 90deg with each other
    G4RotationMatrix* DetectorRotation =
      new G4RotationMatrix(0,sign*angle,i*90*deg);

    // There center is also rotated by 90deg
    G4ThreeVector DetectorPosition(0,DistanceFromTarget,z-(0.5)*Inner_ActiveWafer_Length);
    DetectorPosition.rotate(i*90*deg,G4ThreeVector(0,0,-1));

    // Place the Master volume with its two daugther volume at the final place
    new G4PVPlacement(G4Transform3D(*DetectorRotation,DetectorPosition),
        logicPyramidDetector,"Pyramid_Inner_Detector",
        world,false,i+1);
  }
    
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Pyramid::ConstructOuter(G4LogicalVolume* world, double angle, double z){
  // Pyramid Outer Pyramid
  // The outer Pyramid is identical to the inner Pyramid but wider in terms of
  // geometry. It feature four non resistive strip on the front face

  // Start by making a full pcb
  // We start by the definition of the point forming a PCB cross section
  vector<G4TwoVector> PCBCrossSection;
  double l1 = Outer_PCB_Thickness*0.5/tan(Outer_PCB_Bevel1_Theta);
  double l2 = Outer_PCB_Thickness*0.5/tan(Outer_PCB_Bevel2_Theta);

  PCBCrossSection.push_back(G4TwoVector(Outer_PCB_Width/2.-l2,-Outer_PCB_Thickness*0.5));
  PCBCrossSection.push_back(G4TwoVector(Outer_PCB_Width/2.,0));
  PCBCrossSection.push_back(G4TwoVector(Outer_PCB_Width/2.-l1,Outer_PCB_Thickness*0.5));

  PCBCrossSection.push_back(G4TwoVector(-Outer_PCB_Width/2.+l1,Outer_PCB_Thickness*0.5));
  PCBCrossSection.push_back(G4TwoVector(-Outer_PCB_Width/2.,0));
  PCBCrossSection.push_back(G4TwoVector(-Outer_PCB_Width/2.+l2,-Outer_PCB_Thickness*0.5));

  G4ExtrudedSolid* PCBFull =
    new G4ExtrudedSolid("PCBFull",
        PCBCrossSection,
        Outer_PCB_Length/2.,
        G4TwoVector(0,0),1,
        G4TwoVector(0,0),1);

  // A box having Wafer dimension but thicker than the PCB
  // Will be used to remove material from the PCB to have space for the wafer

  // Calculate the hole shift within the PCB
  G4ThreeVector HoleShift = G4ThreeVector(
      0,
      0,
      Outer_PCB_Offset-(Outer_PCB_Length/2-Outer_PCB_HoleLength/2));

  G4Box*  HoleShape = new G4Box("HoleShape",
      Outer_ActiveWafer_Width/2.,
      Outer_PCB_Thickness/2.+0.1*mm,
      Outer_PCB_HoleLength/2.);

  G4Box*  WaferShape = new G4Box("WaferShape",
      Outer_InertWafer_Width/2.,
      Outer_PCB_Thickness/2.,
      Outer_InertWafer_Length/2.);

  // The Silicon Wafer itself
  G4Box*  InertWaferFull = new G4Box("InertWaferFull",
      Outer_InertWafer_Width/2.,
      Outer_ActiveWafer_Thickness/2.,
      Outer_InertWafer_Length/2.);

  G4Box*  ActiveWafer = new G4Box("ActiveWafer",
      Outer_ActiveWafer_Width/2.,
      Outer_ActiveWafer_Thickness/2.,
      Outer_ActiveWafer_Length/2.);

  G4Box*  ActiveWaferShape = new G4Box("ActiveWaferShape",
      Outer_ActiveWafer_Width/2.,
      Outer_PCB_Thickness/2.,
      Outer_ActiveWafer_Length/2.);


  // Substracting the hole Shape from the Stock PCB
  G4SubtractionSolid* PCB_1 = new G4SubtractionSolid("PCB_1", PCBFull, HoleShape,
      new G4RotationMatrix,HoleShift);

  // Substracting the wafer space from the Stock PCB
  G4SubtractionSolid* PCB = new G4SubtractionSolid("PCB", PCB_1, WaferShape,
      new G4RotationMatrix,
      G4ThreeVector(0,Outer_PCB_Thickness/2.-Outer_PCB_WaferDepth,0));

  // Substract active part from inert part of the Wafer
  G4SubtractionSolid* InertWafer = new G4SubtractionSolid("InertWafer", InertWaferFull, ActiveWaferShape,
      new G4RotationMatrix,
      G4ThreeVector(0,0,0));

  // Master Volume that encompass everything else
  G4LogicalVolume* logicPyramidDetector =
    new G4LogicalVolume(PCBFull,m_MaterialVacuum,"logicBoxDetector", 0, 0, 0);
  logicPyramidDetector->SetVisAttributes(G4VisAttributes::Invisible);

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
  logicActiveWafer->SetSensitiveDetector(m_OuterScorer);

  // The Distance from target is given by half the width of a detector
  // plus 0.75 cm
  G4double DistanceFromTarget = Inner_PCB_Width*(0.5)+0.5*cm
                                   + Inner_PCB_Length*(0.5)*fabs(sin(angle)) ;
                                   
  // Place the sub volumes in the master volume
  // Last argument is the detector number, used in the scorer to get the
  // revelant information
  new G4PVPlacement(new G4RotationMatrix(0,0,0),
      G4ThreeVector(0,0,0),
      logicPCB,"Pyramid_Outer_PCB",logicPyramidDetector,
      false,0);


  G4ThreeVector WaferPosition(0,
      0.5*(Outer_PCB_Thickness+Outer_ActiveWafer_Thickness)-Outer_PCB_WaferDepth
      ,0);

  new G4PVPlacement(new G4RotationMatrix(0,0,0),
      WaferPosition,
      logicActiveWafer,"Outer_Wafer",
      logicPyramidDetector,false,0);

  new G4PVPlacement(new G4RotationMatrix(0,0,0),
      WaferPosition,
      logicInertWafer,"Outer_Wafer_GuardRing",
      logicPyramidDetector,false,0);

   
  // The following build the Pyramid, with detector one at the top
  // and going clowise looking upstrea
  for( unsigned int i = 0; i < 4; i ++){
    // Detector are rotate by 90 deg with each other
    double sign = -z/fabs(z);
    G4RotationMatrix* DetectorRotation =
      new G4RotationMatrix(0*deg,sign*angle,i*90*deg);

    // There center is also rotated by 90deg
    G4ThreeVector DetectorPosition(0,DistanceFromTarget,z);
    DetectorPosition.rotate(i*90*deg,G4ThreeVector(0,0,-1));

    // Place the Master volume with its daugthers at the final place
    new G4PVPlacement(G4Transform3D(*DetectorRotation,DetectorPosition),
        logicPyramidDetector,"Pyramid_Outer_Detector",
        world,false,i+1);
  }

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Pyramid::ConstructChamber(G4LogicalVolume* world){
  // Vaccum Chamber of Pyramid
  // The chamber is made of a central cylinder surrounding the Pyramid Si
  // Two Cone that expeand out of the central cylinder to let room for Exogam

  // Making the Chamber //
  // We make the individual pieces, starting from the inside to the outside
  // Then we merge them together using the a G4AdditionSolid
  // The whole chamber is then placed

  //  Central Tube
  G4Tubs* solidCentralTube =
    new G4Tubs("PyramidChamberCentralTube",CHAMBER_CentralTube_Inner_Radius,
        CHAMBER_CentralTube_Outer_Radius,CHAMBER_CentralTube_Length/2.,
        0*deg,360*deg);

  // Forward-Backward Cones
  G4Cons* solidOuterCone =
    new G4Cons("PyramidChamberOuterCone",CHAMBER_CentralTube_Inner_Radius,
        CHAMBER_CentralTube_Outer_Radius,CHAMBER_OuterCylinder_Inner_Radius,
        CHAMBER_OuterCylinder_Outer_Radius,CHAMBER_OuterCone_Length/2.,
        0*deg,360*deg);

  // Outer Cylinder
  G4Tubs* solidOuterCylinder =
    new G4Tubs("PyramidChamberOuterCylinder",CHAMBER_OuterCylinder_Inner_Radius,
        CHAMBER_OuterCylinder_Outer_Radius,CHAMBER_OuterCylinder_Length/2.,
        0*deg,360*deg);

  // Add the volume together
  G4UnionSolid* solidPyramidChamberStep1 =
    new G4UnionSolid("PyramidChamber", solidCentralTube, solidOuterCone,
        new G4RotationMatrix,
        G4ThreeVector(0,0,CHAMBER_OuterCone_Z_Pos));

  G4UnionSolid* solidPyramidChamberStep2 =
    new G4UnionSolid("PyramidChamber", solidPyramidChamberStep1, solidOuterCone,
        new G4RotationMatrix(0,180*deg,0),
        G4ThreeVector(0,0,-CHAMBER_OuterCone_Z_Pos));

  G4UnionSolid* solidPyramidChamberStep3 =
    new G4UnionSolid("PyramidChamber", solidPyramidChamberStep2, solidOuterCylinder,
        new G4RotationMatrix,
        G4ThreeVector(0,0,CHAMBER_OuterCylinder_Z_Pos));

  G4UnionSolid* solidPyramidChamberStep4 =
    new G4UnionSolid("PyramidChamber", solidPyramidChamberStep3, solidOuterCylinder,
        new G4RotationMatrix,
        G4ThreeVector(0,0,-CHAMBER_OuterCylinder_Z_Pos));

  // Create Logic Volume
  G4LogicalVolume* logicPyramidChamber =
    new G4LogicalVolume(solidPyramidChamberStep4,m_MaterialAl,"logicPyramidChamber", 0, 0, 0);

  // Visual Attribute
  G4VisAttributes* ChamberVisAtt
    = new G4VisAttributes(G4Colour(0.0,0.4,0.5,0.2));

  logicPyramidChamber->SetVisAttributes(ChamberVisAtt);

  // Place the whole chamber
  new G4PVPlacement(new G4RotationMatrix(0,0,0), G4ThreeVector(0,0,0),
      logicPyramidChamber,"PyramidChamber",world,false,0);

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Pyramid::InitializeMaterial(){
  m_MaterialSilicon = MaterialManager::getInstance()->GetMaterialFromLibrary("Si");
  m_MaterialAl = MaterialManager::getInstance()->GetMaterialFromLibrary("Al");
  m_MaterialPCB = MaterialManager::getInstance()->GetMaterialFromLibrary("PCB");
  m_MaterialVacuum = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* Pyramid::Construct(){
  return (NPS::VDetector*) new Pyramid();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
  class proxy_nps_Pyramid{
    public:
      proxy_nps_Pyramid(){
        NPS::DetectorFactory::getInstance()->AddToken("Pyramid","Pyramid");
        NPS::DetectorFactory::getInstance()->AddDetector("Pyramid",Pyramid::Construct);
      }
  };

  proxy_nps_Pyramid p_nps_Pyramid;
}

