/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 21/07/09                                                 *
 * Last update    : 16/10/09                                                 *
 *---------------------------------------------------------------------------*
 * Decription: Define the S1 detector from Micron                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *  + 11/10/09: Change scorer philosophy, one scorer for the detector number *
 *              added (N. de Sereville)                                      *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <sstream>
#include <string>
#include <cmath>

// Geant4 
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4PVDivision.hh"
#include "G4ExtrudedSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

// NPTool headers
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "AnnularS1.hh"
#include "DSSDScorers.hh"
#include "InteractionScorers.hh"
#include "TS1Data.h"
#include "RootOutput.h"
#include "NPOptionManager.h"
// CLHEP
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;
using namespace ANNULARS1;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
AnnularS1::AnnularS1(){
  m_Event = new TS1Data();
  m_LogicalDetector = 0 ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
AnnularS1::~AnnularS1(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void AnnularS1::AddModule(G4double PosZ){
  m_PosZ.push_back(PosZ);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* AnnularS1::ConstructVolume(){

  if(!m_LogicalDetector){
    G4Material* Silicon = MaterialManager::getInstance()->GetMaterialFromLibrary("Si");
    G4Material* Vacuum = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
    ////////////////////////////////////////////////////////////////
    ////////////// Starting Volume Definition //////////////////////
    ////////////////////////////////////////////////////////////////
    // Name of the module
    G4String Name = "AnnularS1";

    // Building the PCB
    // The PCB is a simple extruded volume from 8reference point
    vector<G4TwoVector> polygon;
    for(unsigned int i = 0 ; i < 8 ; i++){
      G4TwoVector Point(PCBPointsX[i],PCBPointsY[i]);
      polygon.push_back(Point);
    }

    // Mast volume containing all the detector
    G4ExtrudedSolid* solidAnnularS1 = new G4ExtrudedSolid(Name,
        polygon,
        PCBThickness*0.5,
        G4TwoVector(0,0),1,
        G4TwoVector(0,0),1);

    // Definition of the volume containing the sensitive detector
    m_LogicalDetector = new G4LogicalVolume(solidAnnularS1, Vacuum, Name, 0, 0, 0);
    m_LogicalDetector->SetVisAttributes(G4VisAttributes::Invisible);

    // PCB Base
    G4ExtrudedSolid* solidPCBBase = new G4ExtrudedSolid("PCBBase",
        polygon,
        PCBThickness*0.5,
        G4TwoVector(0,0),1,
        G4TwoVector(0,0),1);   

    // Wafer Shape to be substracted to the PCB
    G4Tubs* solidWaferShapeBase = new G4Tubs("WaferShape", 
        0,
        WaferOutterRadius,
        PCBThickness,
        0*deg, 
        360*deg); 

    // A no rotation matrix is always handy ;)
    G4RotationMatrix* norotation = new  G4RotationMatrix(); 
    // Rotation of the box that make the Si cut                       
    G4RotationMatrix* cutrotation = new  G4RotationMatrix(G4ThreeVector(0,0,1),45*deg);                        
    G4ThreeVector cutposition1(80*mm+WaferRCut,0,0); cutposition1.setPhi(45*deg);
    G4Transform3D transform1(*cutrotation,cutposition1);

    G4Box* solidCutout = new G4Box("cuttout",80*mm,80*mm,80*mm); 

    G4SubtractionSolid* solidWaferShape1 = new G4SubtractionSolid("WaferShape1",
        solidWaferShapeBase,
        solidCutout,
        transform1);


    G4ThreeVector cutposition2(-80*mm-WaferRCut,0,0); cutposition2.setPhi(-135*deg);
    G4Transform3D transform2(*cutrotation,cutposition2);
    G4SubtractionSolid* solidWaferShape = new G4SubtractionSolid("WaferShape",
        solidWaferShape1,
        solidCutout,
        transform2);


    // PCB final
    G4SubtractionSolid* solidPCB = new G4SubtractionSolid("AnnularS1_PCB1",
        solidPCBBase,
        solidWaferShape);

    G4LogicalVolume* logicPCB = new G4LogicalVolume(solidPCB, Vacuum, "AnnularS1_PCB", 0, 0, 0);

    new G4PVPlacement(G4Transform3D(*norotation, G4ThreeVector()),
        logicPCB,
        "AnnularS1_PCB",
        m_LogicalDetector,
        false,
        0);

    G4VisAttributes* PCBVisAtt = new G4VisAttributes(G4Colour(0.2, 0.5, 0.2)) ;
    logicPCB->SetVisAttributes(PCBVisAtt);


    // Wafer itself
    G4Tubs* solidWaferBase = new G4Tubs("Wafer", 
        WaferInnerRadius,
        WaferOutterRadius,
        0.5*WaferThickness,
        0*deg, 
        360*deg); 

    G4SubtractionSolid* solidWafer1 = new G4SubtractionSolid("Wafer1",
        solidWaferBase,
        solidCutout,
        transform1);

    G4SubtractionSolid* solidWafer = new G4SubtractionSolid("Wafer",
        solidWafer1,
        solidCutout,
        transform2);

    G4LogicalVolume* logicWafer = new G4LogicalVolume(solidWafer, Silicon, "AnnularS1_Wafer", 0, 0, 0);
    new G4PVPlacement(G4Transform3D(*norotation, G4ThreeVector()),
        logicWafer,
        "AnnularS1_Wafer",
        m_LogicalDetector,
        false,
        0);

    G4VisAttributes* SiVisAtt = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3)) ;
    logicWafer->SetVisAttributes(SiVisAtt); 

    // Active Wafer
    G4Tubs* solidActiveWaferBase = new G4Tubs("ActiveWafer", 
        ActiveWaferInnerRadius,
        ActiveWaferOutterRadius,
        0.5*WaferThickness,
        0*deg, 
        360*deg); 


    G4ThreeVector activecutposition1(80*mm+ActiveWaferRCut,0,0); activecutposition1.setPhi(45*deg);
    G4Transform3D activetransform1(*cutrotation,activecutposition1);

    G4SubtractionSolid* solidActiveWafer1 = new G4SubtractionSolid("ActiveWafer1",
        solidActiveWaferBase,
        solidCutout,
        activetransform1);

    G4ThreeVector activecutposition2(-80*mm-ActiveWaferRCut,0,0); activecutposition2.setPhi(-135*deg);
    G4Transform3D activetransform2(*cutrotation,activecutposition2);

    G4SubtractionSolid* solidActiveWafer = new G4SubtractionSolid("ActiveWafer",
        solidActiveWafer1,
        solidCutout,
        activetransform2);

    G4LogicalVolume* logicActiveWafer = new G4LogicalVolume(solidActiveWafer, Silicon, "AnnularS1_ActiveWafer", 0, 0, 0);
    new G4PVPlacement(G4Transform3D(*norotation, G4ThreeVector()),
        logicActiveWafer,
        "AnnularS1_ActiveWafer",
        logicWafer,
        false,
        0);

    logicActiveWafer->SetVisAttributes(SiVisAtt);

    // Set Silicon strip sensible
    logicActiveWafer->SetSensitiveDetector(m_Scorer);
  }
  return m_LogicalDetector;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void AnnularS1::ReadConfiguration(NPL::InputParser parser){
 vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("AnnularS1");
  cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> token = {"Z"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      double Z = blocks[i]->GetDouble("Z","mm");
      AddModule(Z);
    }

    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void AnnularS1::ConstructDetector(G4LogicalVolume* world){
  G4RotationMatrix* rotation = NULL;
  G4ThreeVector     position = G4ThreeVector(0, 0, 0);

  G4int NumberOfModule = m_PosZ.size() ;

  for (G4int i = 0; i < NumberOfModule; i++) {
    // translation to position the module
    position = G4ThreeVector(0, 0, m_PosZ[i]);

    // Passage Matrix from Lab Referential to Module Referential
    // Identity matrix by default
    rotation = new G4RotationMatrix();
    if (position.z() < 0) rotation->rotateX(180*deg);

    new G4PVPlacement(G4Transform3D(*rotation, position),
        ConstructVolume(),
        "AnnularS1",
        world,
        false,
        i+1);
  }

  delete rotation ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Connect the GaspardTrackingData class to the output TTree
// of the simulation
void AnnularS1::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("AnnularS1")){
    pTree->Branch("AnnularS1", "TS1Data", &m_Event);
  }
  pTree->SetBranchAddress("AnnularS1",&m_Event);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void AnnularS1::ReadSensitive(const G4Event*){
  // Clear ROOT objects
  m_Event->Clear();

  DSSDScorers::PS_Annular* Scorer= (DSSDScorers::PS_Annular*) m_Scorer->GetPrimitive(0);
 
  // Loop on Silicon Ring Hit
  unsigned int sizeRing = Scorer->GetRingMult();
  for(unsigned int i = 0 ; i < sizeRing ; i++){
    double Energy = Scorer->GetEnergyRing(i);

    if(Energy>EnergyThreshold){
      double Time       = Scorer->GetTimeRing(i);
      unsigned int DetNbr        = Scorer->GetDetectorRing(i);;
      unsigned int StripTheta    = Scorer->GetStripRing(i);
       
       
      // Check for associated Quadrant strip
      int StripQuadrant = 0;
      unsigned int sizeQ = Scorer->GetQuadrantMult();
      for(unsigned int q = 0 ; q < sizeQ ; q++){
        if(Scorer->GetDetectorQuadrant(q)==DetNbr){
          StripQuadrant = Scorer->GetStripQuadrant(q)-1;
          break;
          }
      }

      m_Event->SetS1ThetaEDetectorNbr(DetNbr);
      m_Event->SetS1ThetaEStripNbr(StripTheta+StripQuadrant*NbrRingStrips);
      m_Event->SetS1ThetaEEnergy(RandGauss::shoot(Energy, ResoEnergy));

      m_Event->SetS1ThetaTDetectorNbr(DetNbr);
      m_Event->SetS1ThetaTStripNbr(StripTheta+StripQuadrant*NbrRingStrips);
      m_Event->SetS1ThetaTTime(RandGauss::shoot(Time, ResoTime));

     }

  }
 
 // Loop on Silicon Sector Hit
 unsigned int sizeSector = Scorer->GetSectorMult();
 for(unsigned int i = 0 ; i < sizeSector ; i++){ 
    double Energy = Scorer->GetEnergyRing(i);

    if(Energy>EnergyThreshold){
      double Time       = Scorer->GetTimeRing(i);
      unsigned int DetNbr        = Scorer->GetDetectorRing(i);;
      unsigned int StripPhi      = Scorer->GetStripSector(i);
     
      m_Event->SetS1PhiEDetectorNbr(DetNbr);
      m_Event->SetS1PhiEStripNbr(StripPhi); 
      m_Event->SetS1PhiEEnergy(RandGauss::shoot(Energy, ResoEnergy));

      m_Event->SetS1PhiTDetectorNbr(DetNbr);
      m_Event->SetS1PhiTStripNbr(StripPhi);
      m_Event->SetS1PhiTTime(RandGauss::shoot(Time, ResoTime));
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Initilize the Scorer use to read out the sensitive volume 
void AnnularS1::InitializeScorers(){
  bool already_exist = false;  
  // Associate Scorer
  m_Scorer = CheckScorer("AnnularS1_Scorer",already_exist);
  if(already_exist) return;

  G4VPrimitiveScorer* AnnularScorer =
    new  DSSDScorers::PS_Annular("AnnularS1_Scorer",
        2,
        ActiveWaferInnerRadius,
        ActiveWaferOutterRadius,
        -8*22.5*deg, //MUST2 campaign 2009, See: Phd Sandra Giron
        +8*22.5*deg,
        NbrRingStrips,
        NbrSectorStrips,
        NbQuadrant);

  m_Scorer->RegisterPrimitive(AnnularScorer);
  G4VPrimitiveScorer* InteractionScorer = new InteractionScorers::PS_Interactions("InteractionS1",ms_InterCoord,2);
  m_Scorer->RegisterPrimitive(InteractionScorer);
  //  Add All Scorer to the Global Scorer Manager
  G4SDManager::GetSDMpointer()->AddNewDetector(m_Scorer);
}
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* AnnularS1::Construct(){
  return (NPS::VDetector*) new AnnularS1();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_nps_annulars1{
  public:
    proxy_nps_annulars1(){
      NPS::DetectorFactory::getInstance()->AddToken("AnnularS1","AnnularS1");
      NPS::DetectorFactory::getInstance()->AddDetector("AnnularS1",AnnularS1::Construct);
    }
};

proxy_nps_annulars1 p_nps_annulars1;
}


