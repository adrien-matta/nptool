/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche  contact address: marc.labiche@stfc.ac.uk     *
 *                                                                           *
 * Creation Date  : December 2009                                            *
 * Last update    : December 2014                                            *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe the Paris scintillator array                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <sstream>
#include <cmath>
#include <limits>
using namespace std;

//Geant4
#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4SDManager.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4Colour.hh"

// NPS
#include "Paris.hh"
using namespace PARIS;

#include "CalorimeterScorers.hh"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
// NPL
#include "NPOptionManager.h"
#include "RootOutput.h"

// CLHEP header
#include "CLHEP/Random/RandGauss.h"
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Paris Specific Method
Paris::Paris(){
  m_Event = new TParisData();

  // Orange 
  m_NaIVisAtt = new G4VisAttributes(G4Colour(1, 0.5, 0))  ;

  // Blue
  m_LaBr3VisAtt = new G4VisAttributes(G4Colour(0, 0.5, 1));

  // Dark Grey
  m_PMTVisAtt = new G4VisAttributes(G4Colour(0.1, 0.1, 0.1));

  // Grey wireframe
  m_PhoswichCasingVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5,0.2));

  // White wireframe
  m_ClusterCasingVisAtt = new G4VisAttributes(G4Colour(0.7, 0.7, 0.7)); 

  m_LogicalPhoswich = 0;
  m_LogicalCluster = 0;  
  m_NaIScorer = 0 ;
  m_LaBr3Scorer = 0 ;
}

Paris::~Paris(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Paris::AddCluster(G4ThreeVector Pos1, G4ThreeVector Pos2, G4ThreeVector Pos3, G4ThreeVector Pos4){
  G4ThreeVector Pos=(Pos1+Pos2+Pos3+Pos4)/4.;
  G4ThreeVector u = Pos1-Pos2;
  G4ThreeVector v = Pos1-Pos4;
  u = u.unit(); v = v.unit();
  G4ThreeVector w = u.cross(v);
  Pos = Pos + w*Length*0.5;

  m_Type.push_back(1); 
  m_Pos.push_back(Pos);
  m_Rot.push_back(new G4RotationMatrix(u,v,w));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Paris::AddCluster(G4ThreeVector Pos, double beta_u, double beta_v, double beta_w){
  double Theta = Pos.theta();
  double Phi = Pos.phi();

  // vector parallel to one axis of silicon plane
  G4double ii = cos(Theta / rad) * cos(Phi / rad);
  G4double jj = cos(Theta / rad) * sin(Phi / rad);
  G4double kk = -sin(Theta / rad);
  G4ThreeVector Y = G4ThreeVector(ii, jj, kk);

  G4ThreeVector w = Pos.unit();
  G4ThreeVector u = w.cross(Y);
  G4ThreeVector v = w.cross(u);
  v = v.unit();
  u = u.unit();

  G4RotationMatrix* r = new G4RotationMatrix(u,v,w);
  r->rotate(beta_u,u);
  r->rotate(beta_v,v);
  r->rotate(beta_w,w);

  m_Type.push_back(1);
  m_Pos.push_back(Pos);
  m_Rot.push_back(r);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Paris::AddPhoswich(G4ThreeVector Pos1, G4ThreeVector Pos2, G4ThreeVector Pos3, G4ThreeVector Pos4){
  G4ThreeVector Pos=(Pos1+Pos2+Pos3+Pos4)/4.;
  G4ThreeVector u = Pos1-Pos2;
  G4ThreeVector v = Pos1-Pos4;
  u = u.unit(); v = v.unit();
  G4ThreeVector w = Pos.unit();
  Pos = Pos + w*Length*0.5;

  m_Type.push_back(0); 
  m_Pos.push_back(Pos);
  m_Rot.push_back(new G4RotationMatrix(u,v,w));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Paris::AddPhoswich(G4ThreeVector Pos, double beta_u, double beta_v, double beta_w){
  double Theta = Pos.theta();
  double Phi = Pos.phi();

  // vector parallel to one axis of silicon plane
  G4double ii = cos(Theta / rad) * cos(Phi / rad);
  G4double jj = cos(Theta / rad) * sin(Phi / rad);
  G4double kk = -sin(Theta / rad);
  G4ThreeVector Y = G4ThreeVector(ii, jj, kk);

  G4ThreeVector w = Pos.unit();
  G4ThreeVector u = w.cross(Y);
  G4ThreeVector v = w.cross(u);
  v = v.unit();
  u = u.unit();

  G4RotationMatrix* r = new G4RotationMatrix(u,v,w);
  r->rotate(beta_u,u);
  r->rotate(beta_v,v);
  r->rotate(beta_w,w);

  m_Type.push_back(0);
  m_Pos.push_back(Pos);
  m_Rot.push_back(r);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class
// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Paris::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Paris");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> cart = {"A","B","C","D"};
  vector<string> sphe = {"R","THETA","PHI","BETA"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart) && blocks[i]->GetMainValue()=="Cluster"){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Cluster " << i+1 <<  endl;
      G4ThreeVector A = NPS::ConvertVector(blocks[i]->GetTVector3("A","mm"));
      G4ThreeVector B = NPS::ConvertVector(blocks[i]->GetTVector3("B","mm"));
      G4ThreeVector C = NPS::ConvertVector(blocks[i]->GetTVector3("C","mm"));
      G4ThreeVector D = NPS::ConvertVector(blocks[i]->GetTVector3("D","mm"));

      AddCluster(A,B,C,D);
    }
    else if(blocks[i]->HasTokenList(sphe)&& blocks[i]->GetMainValue()=="Cluster"){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Cluster " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("THETA","deg");
      double Phi = blocks[i]->GetDouble("PHI","deg");
      vector<double> Beta = blocks[i]->GetVectorDouble("BETA","deg");
      R = R +  0.5*Length;
      G4ThreeVector Pos(R*sin(Theta)*cos(Phi),R*sin(Theta)*sin(Phi),R*cos(Theta));
      AddCluster(Pos,Beta[0],Beta[1],Beta[2]);
    }
    else if(blocks[i]->HasTokenList(cart)&& blocks[i]->GetMainValue()=="Phoswich"){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Phoswich" << i+1 <<  endl;
      G4ThreeVector A = NPS::ConvertVector(blocks[i]->GetTVector3("A","mm"));
      G4ThreeVector B = NPS::ConvertVector(blocks[i]->GetTVector3("B","mm"));
      G4ThreeVector C = NPS::ConvertVector(blocks[i]->GetTVector3("C","mm"));
      G4ThreeVector D = NPS::ConvertVector(blocks[i]->GetTVector3("D","mm"));

      AddPhoswich(A,B,C,D);
    }

    else if(blocks[i]->HasTokenList(sphe)&& blocks[i]->GetMainValue()=="Phoswich"){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Phoswich " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("THETA","deg");
      double Phi = blocks[i]->GetDouble("PHI","deg");
      vector<double> Beta = blocks[i]->GetVectorDouble("BETA","deg");
      R = R +  0.5*Length;
      G4ThreeVector Pos(R*sin(Theta)*cos(Phi),R*sin(Theta)*sin(Phi),R*cos(Theta));
      AddPhoswich(Pos,Beta[0],Beta[1],Beta[2]);
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
void Paris::ConstructDetector(G4LogicalVolume* world){
  unsigned int mysize = m_Pos.size();
  for(unsigned int i = 0 ; i < mysize ; i++){
    if(m_Type[i])
      new G4PVPlacement(G4Transform3D(*m_Rot[i], m_Pos[i]), ConstructCluster(),  "ParisCluster", world, false, i+1);
    else
      new G4PVPlacement(G4Transform3D(*m_Rot[i], m_Pos[i]), ConstructPhoswich(),  "ParisPhoswich", world, false, i+1); 
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Paris::ConstructPhoswich(){

  if(!m_LogicalPhoswich){
    G4Material* Vacuum = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
    G4Material* Alu = MaterialManager::getInstance()->GetMaterialFromLibrary("Al");
    G4Material* NaI = MaterialManager::getInstance()->GetMaterialFromLibrary("NaI");
    G4Material* LaBr3 = MaterialManager::getInstance()->GetMaterialFromLibrary("LaBr3");

    G4Box* solidParisPhoswich = new G4Box("SolidParisPhoswich", 0.5*PhoswichCasingWidth, 0.5*PhoswichCasingWidth, 0.5*PhoswichCasingLenth+0.5*PMTLength);
    m_LogicalPhoswich = new G4LogicalVolume(solidParisPhoswich, Vacuum, "LogicParisPhoswich", 0, 0, 0);
    m_LogicalPhoswich->SetVisAttributes(G4VisAttributes::Invisible);

    // Phoswich construction
    // Casing
    G4Box* solidCasing = new G4Box("SolidParisPhoswichCasing", 0.5*PhoswichCasingWidth, 0.5*PhoswichCasingWidth, 0.5*PhoswichCasingLenth);
    G4LogicalVolume* LogicalCasing = new G4LogicalVolume(solidCasing, Alu, "LogicParisPhoswichCasing", 0, 0, 0);
    LogicalCasing->SetVisAttributes(m_PhoswichCasingVisAtt);

    new G4PVPlacement(0, 
        G4ThreeVector(0,0,-PMTLength), 
        LogicalCasing, 
        "ParisPhoswich_Casing", 
        m_LogicalPhoswich, 
        false, 
        0);

    // PMT
    G4Tubs* solidPMT = new G4Tubs("SolidPMT",0,PMTRadius,0.5*PMTLength,0,360*deg);
    G4LogicalVolume* LogicalPMT = new G4LogicalVolume(solidPMT, Alu, "LogicPMT" ,0,0,0);
    new G4PVPlacement(0, 
        G4ThreeVector(0,0,0.5*Length-PMTLength), 
        LogicalPMT, 
        "ParisPhoswich_PMT", 
        m_LogicalPhoswich, 
        false, 
        0);
    LogicalPMT->SetVisAttributes(m_PMTVisAtt);

    // LaBr3
    G4Box*           solidLaBr3Stage = new G4Box("solidLaBr3Stage", 0.5*LaBr3Face, 0.5*LaBr3Face, 0.5*LaBr3Thickness);
    G4LogicalVolume* logicLaBr3Stage = new G4LogicalVolume(solidLaBr3Stage, LaBr3, "logicLaBr3Stage", 0, 0, 0);
    G4ThreeVector  positionLaBr3Stage = G4ThreeVector(0, 0, LaBr3Stage_PosZ);

    new G4PVPlacement(0, 
        positionLaBr3Stage, 
        logicLaBr3Stage, 
        "ParisPhoswich_LaBr3Stage", 
        LogicalCasing, 
        false,
        0);

    // Set LaBr3 sensible
    logicLaBr3Stage->SetSensitiveDetector(m_LaBr3Scorer);

    // Visualisation of LaBr3 stage 
    logicLaBr3Stage->SetVisAttributes(m_LaBr3VisAtt);

    // NaI
    G4ThreeVector  positionNaIStage = G4ThreeVector(0, 0, NaIStage_PosZ);

    G4Box*           solidNaIStage = new G4Box("solidNaIStage", 0.5*NaIFace, 0.5*NaIFace, 0.5*NaIThickness);
    G4LogicalVolume* logicNaIStage = new G4LogicalVolume(solidNaIStage, NaI, "logicNaIStage", 0, 0, 0);

    new G4PVPlacement(0, 
        positionNaIStage, 
        logicNaIStage, 
        "ParisPhoswich_NaIStage", 
        LogicalCasing, 
        false, 
        0);

    // Set NaI sensible
    logicNaIStage->SetSensitiveDetector(m_NaIScorer);

    // Visualisation of the NaI stage 
    logicNaIStage->SetVisAttributes(m_NaIVisAtt);
  }

  return m_LogicalPhoswich;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Paris::ConstructCluster(){

  if(!m_LogicalCluster){
    G4Material* Vacuum =  MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
    G4Material* Alu =  MaterialManager::getInstance()->GetMaterialFromLibrary("Al");

    // Mother Volume
    G4Box* solidParisCluster1 = new G4Box("SolidParisCluster1", 0.5*FaceFront, 0.5*FaceFront, 0.5*Length);
    G4Box* solidParisCluster2 = new G4Box("SolidParisCluster2", 0.5*ClusterFrameWidth, 0.5*ClusterFrameWidth, 0.5*ClusterFrameLength);     

    // The cluster is made of a metal frame that hold the phoswich via the pmt
    G4ThreeVector FramePos(0,0,Length*0.5-PMTLength+ClusterFrameLength*0.5+ClusterOffset);
    G4UnionSolid* solidParisCluster = new G4UnionSolid("solidParisCluster",solidParisCluster1,solidParisCluster2,0,FramePos);
    m_LogicalCluster = new G4LogicalVolume(solidParisCluster, Vacuum, "LogicSolidParisCluster", 0, 0, 0);
    m_LogicalCluster->SetVisAttributes(G4VisAttributes::Invisible);

    if(!m_LogicalPhoswich) ConstructPhoswich();

    // The frame is a plate with hole for the PMT
    G4VSolid* solidClusterFrame = solidParisCluster2;
    G4ThreeVector Origin(-PhoswichCasingWidth,-PhoswichCasingWidth,0.5*PMTLength);
    G4Tubs* solidPMT = new G4Tubs("SolidPMT",0,PMTRadius,10*PMTLength,0,360*deg);

    // A cluster is a 3 by 3 aggregat of phoswich
    unsigned int PhoswichNbr = 1;
    for(unsigned int i = 0 ; i < 3 ; i++){
      for(unsigned int j = 0 ; j <3 ; j++){
        G4ThreeVector Pos = Origin + G4ThreeVector(i*PhoswichCasingWidth,j*PhoswichCasingWidth,0);
        new G4PVPlacement(0, 
            Pos, 
            m_LogicalPhoswich, 
            "Paris_Phoswich", 
            m_LogicalCluster, 
            false, 
            PhoswichNbr++);

        // make room for the PMT in the cluster frame
        solidClusterFrame = new G4SubtractionSolid("ClusterFrame",solidClusterFrame,solidPMT,0,Pos); 
      }
    }

    G4LogicalVolume* LogicalClusterFrame = new G4LogicalVolume(solidClusterFrame, Alu, "LogicSolidClusterFrame", 0, 0, 0);
    LogicalClusterFrame->SetVisAttributes(m_ClusterCasingVisAtt);

    new G4PVPlacement(0, 
        FramePos, 
        LogicalClusterFrame, 
        "Paris_ClusterFrame", 
        m_LogicalCluster, 
        false, 
        0);

  }
  return m_LogicalCluster;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Paris::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("Paris")){
    pTree->Branch("Paris", "TParisData", &m_Event) ;
  }
  pTree->SetBranchAddress("Paris", &m_Event) ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Paris::ReadSensitive(const G4Event* event){
  m_Event->Clear();

  ///////////
  // LaBr3
  G4THitsMap<G4double*>* LaBr3HitMap;
  std::map<G4int, G4double**>::iterator LaBr3_itr;

  G4int LaBr3CollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Paris_LaBr3Scorer/ParisLaBr3");
  LaBr3HitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(LaBr3CollectionID));

  // Loop on the LaBr3 map
  for (LaBr3_itr = LaBr3HitMap->GetMap()->begin() ; LaBr3_itr != LaBr3HitMap->GetMap()->end() ; LaBr3_itr++){

    G4double* Info = *(LaBr3_itr->second);

    double Energy = RandGauss::shoot(Info[0],ResoFirstStage);

    if(Energy>EnergyThreshold){
      double Time = Info[1];
      int PhoswichNbr = (int) Info[2];
      int ClusterNbr = (int) Info[3];
      
      m_Event->SetParisLaBr3E(ClusterNbr,PhoswichNbr,Energy);
      m_Event->SetParisLaBr3T(ClusterNbr,PhoswichNbr,Time);
    }
  }
  // clear map for next event
  LaBr3HitMap->clear();

  ///////////
  // NaI
  G4THitsMap<G4double*>* NaIHitMap;
  std::map<G4int, G4double**>::iterator NaI_itr;

  G4int NaICollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Paris_NaIScorer/ParisNaI");
  NaIHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(NaICollectionID));

  // Loop on the NaI map
  for (NaI_itr = NaIHitMap->GetMap()->begin() ; NaI_itr != NaIHitMap->GetMap()->end() ; NaI_itr++){

    G4double* Info = *(NaI_itr->second);

    double Energy = RandGauss::shoot(Info[0],ResoSecondStage);

    if(Energy>EnergyThreshold){
      double Time = Info[1];
      int PhoswichNbr = (int) Info[2];
      int ClusterNbr = (int) Info[3];
     
       m_Event->SetParisNaIE(ClusterNbr,PhoswichNbr,Energy);
      m_Event->SetParisNaIT(ClusterNbr,PhoswichNbr,Time);
    }
  }

  // clear map for next event
  NaIHitMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Paris::InitializeScorers(){
  vector<G4int> NestingLevel;
  NestingLevel.push_back(2);
  NestingLevel.push_back(3);

  //   LaBr3 Associate Scorer
  bool already_exist = false;
  m_LaBr3Scorer = CheckScorer("Paris_LaBr3Scorer",already_exist);

  // if the scorer were created previously nothing else need to be made
  if(already_exist) return;

  G4VPrimitiveScorer* LaBr3Scorer =
    new  CALORIMETERSCORERS::PS_Calorimeter("ParisLaBr3",NestingLevel);
  //and register it to the multifunctionnal detector
  m_LaBr3Scorer->RegisterPrimitive(LaBr3Scorer);

  //   Add All Scorer to the Global Scorer Manager
  G4SDManager::GetSDMpointer()->AddNewDetector(m_LaBr3Scorer) ;


  //   NaI Associate Scorer
  already_exist = false;
  m_NaIScorer = CheckScorer("Paris_NaIScorer",already_exist);

  // if the scorer were created previously nothing else need to be made
  if(already_exist) return;

  G4VPrimitiveScorer* NaIScorer =
    new  CALORIMETERSCORERS::PS_Calorimeter("ParisNaI",NestingLevel);
  //and register it to the multifunctionnal detector
  m_NaIScorer->RegisterPrimitive(NaIScorer);

  //   Add All Scorer to the Global Scorer Manager
  G4SDManager::GetSDMpointer()->AddNewDetector(m_NaIScorer) ;

}
 ////////////////////////////////////////////////////////////////////////////////
 //            Construct Method to be pass to the DetectorFactory              //
 ////////////////////////////////////////////////////////////////////////////////
 NPS::VDetector* Paris::Construct(){
  return  (NPS::VDetector*) new Paris();
 }

 ////////////////////////////////////////////////////////////////////////////////
 //            Registering the construct method to the factory                 //
 ////////////////////////////////////////////////////////////////////////////////
 extern"C" {
 class proxy_nps_paris{
   public:
    proxy_nps_paris(){
      NPS::DetectorFactory::getInstance()->AddToken("Paris","Paris");
      NPS::DetectorFactory::getInstance()->AddDetector("Paris",Paris::Construct);
    }
};

 proxy_nps_paris p_nps_proxy;
 }
