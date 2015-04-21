/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre MORFOUACE  contact address:                       *
 *                                                                           *
 * Creation Date  : February 2015                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe the HiRA telescope                                   *
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
#include "HiRA.hh"
#include "MaterialManager.hh"
#include "SiliconScorers.hh"

// NPL
#include "NPOptionManager.h"
#include "RootOutput.h"
using namespace HIRA;

// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Hira::Hira(){
  InitializeMaterial();
  m_EventHira = new THiraData();
  
  // Dark Grey
  SiliconVisAtt = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3)) ;
  // Green
  m_CsIVisAtt = new G4VisAttributes(G4Colour(0.2, 0.5, 0.2)) ;
  
  m_boolTelescope = false;

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Hira::~Hira(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of VDetector class
// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Hira::ReadConfiguration(string Path){
  ifstream ConfigFile           ;
  ConfigFile.open(Path.c_str()) ;
  string LineBuffer             ;
  string DataBuffer             ;
  bool ReadingStatus = false    ;

  int VerboseLevel = NPOptionManager::getInstance()->GetVerboseLevel();

  while (getline(ConfigFile, LineBuffer)){

    if (LineBuffer.compare(0, 4, "Hira") == 0)
      ReadingStatus = true;

    while (ReadingStatus && ConfigFile >> DataBuffer ) {
      //   Comment Line
      if (DataBuffer.compare(0, 1, "%") == 0) { ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}

      else if (DataBuffer=="HiraTelescope") {
        if(VerboseLevel==1) G4cout << "// \n Hira  found: " << G4endl   ;
        bool ReadingHyball = true;
        double Z,R,Phi;
        bool  boolZ= false;
        bool  boolR= false;
        bool  boolPhi= false;
        while(ReadingHyball && ConfigFile >> DataBuffer){
          if (DataBuffer.compare(0, 1, "%") == 0) { 
            ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
          }

          else if(DataBuffer == "Z="){
            ConfigFile >> Z ;
            boolZ = true;
            if(VerboseLevel==1) G4cout << "\t" << DataBuffer << Z << G4endl;

          }

          else if(DataBuffer == "R="){
            ConfigFile >> R ;
            boolR = true; 
            if(VerboseLevel==1) G4cout <<"\t" << DataBuffer << R << G4endl;
          }

          else if(DataBuffer == "Phi="){
            ConfigFile >> Phi ;
            boolPhi = true;
            if(VerboseLevel==1) G4cout <<"\t" <<  DataBuffer << Phi << G4endl;
          }

          else{
            G4cout << "Error: Wrong Token Sequence for Hira : Getting out " << DataBuffer << G4endl;
            exit(1);
          }

          if(boolPhi && boolR && boolZ){
            ReadingHyball = false;
            m_HiraZ.push_back(Z*mm);
            m_HiraR.push_back(R*mm);
            m_HiraPhi.push_back(Phi*deg);
          }
        }
      }
    }
  }

  ConfigFile.close();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Hira::ConstructDetector(G4LogicalVolume* world){

  if(m_booltelescope)
    ConstructThinSi(world);
    ConstructThickSi(world);
    ConstructCsICrystal(world);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Hira::ReadSensitive(const G4Event* event){
  m_EventHira->Clear();

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
    if(EB>EnergyThreshold){
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


  // Hyball //
  G4THitsMap<G4double*>* HyballHitMap;
  std::map<G4int, G4double**>::iterator Hyball_itr;
  G4int HyballCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Tiara_HyballScorer/Hyball");
  HyballHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(HyballCollectionID));

  // Loop on the Hyball map 
  for (Hyball_itr = HyballHitMap->GetMap()->begin() ; Hyball_itr != HyballHitMap->GetMap()->end() ; Hyball_itr++){
    G4double* Info = *(Hyball_itr->second); 

    // Front Energy
    double EF = RandGauss::shoot(Info[0],ResoEnergy);
    if(EF>EnergyThreshold){
      int RingNumber=Info[8];
      RingNumber=abs(RingNumber-17);                                                                                 //
      Info[8]=RingNumber; 
      m_EventHyball->SetRingE(Info[7],Info[8],EF);
      m_EventHyball->SetRingT(Info[7],Info[8],Info[1]); 
    }

    // Back Energy
    double EB = RandGauss::shoot(Info[1]+Info[0],ResoEnergy);
    if(EB>EnergyThreshold){
      m_EventHyball->SetSectorE(Info[7],Info[9],EF);
      m_EventHyball->SetSectorT(Info[7],Info[9],Info[1]); 
    }

  }
  // Clear Map for next event
  HyballHitMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Hira::InitializeScorers(){
  //Look for previous definition of the scorer (geometry reload)
  bool already_exist = false;
  m_ThinSiStripScorer = CheckScorer("Hira_ThinSiScorer",already_exist);
  m_ThickSiStripScorer = CheckScorer("Hira_ThickSiScorer",already_exist);
  m_CsIScorer = CheckScorer("Hira_CsIScorer",already_exist);

  // if the scorer were created previously nothing else need to be made
  if(already_exist) return;

  G4VPrimitiveScorer* ThinSiScorer = new SILICONSCORERS::PS_Silicon_Rectangle("ThinSi",1,
      SiliconFace,
      SiliconFace,
      NumberOfStrip,
      1);

  m_ThinSiStripScorer->RegisterPrimitive(ThinSiScorer);

  G4VPrimitiveScorer* ThickSiScorer = new SILICONSCORERS::PS_Silicon_Rectangle("ThickSi",1,
      SiliconFace,
      SiliconFace,
      NumberOfStrip,
      NumberOfStrip);

  m_ThickSiStripScorer->RegisterPrimitive(ThickSiScorer);

  G4VPrimitiveScorer* CsIScorer= new CALORIMETERSCORERS::PS_Calorimeter("CsI",1);

  m_CsIScorer->RegisterPrimitive(CsIScorer);

  //   Add All Scorer to the Global Scorer Manager 
  G4SDManager::GetSDMpointer()->AddNewDetector(m_ThinSiStripScorer) ;
  G4SDManager::GetSDMpointer()->AddNewDetector(m_ThickSiStripScorer) ;
  G4SDManager::GetSDMpointer()->AddNewDetector(m_CsIScorer) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Hira::InitializeRootOutput(){
  TTree *pTree = RootOutput::getInstance()->GetTree();   
  pTree->Branch("Hira", "THiraData", &m_EventHira) ;
 
  // This insure that the object are correctly bind in case of 
  // a redifinition of the geometry in the simulation
  pTree->SetBranchAddress("Hira", &m_EventHira) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Hira::ConstructCsICrystal(G4LogicalVolume* world){
    
    m_LogicCsICrystal = new G4LogicalVolume(CsIFull, m_MaterialVacuum, "logicCsI", 0, 0, 0);
    m_LogicCsICrystal->SetVisAttributes(G4VisAttributes::Invisible);
    
    G4Box* solidCsICrystal = new G4Box("solidCsICrystal",0.5*CsIFaceFront,0.5*CsIFaceBack,0.5*CsIThickness);
    G4LogicalVolume* logicCsICrystal = new G4LogicalVolume(solidCsICrystal, m_MaterialCsI, "logicCsICrystal", 0, 0, 0);
    G4ThreeVector positionCsICrystal = G4ThreeVector(0, 0, CsI_PosZ);
    
    new G4PVPlacement(new G4RotationMatrix(0,0,0),
                      G4ThreeVector(0,0,0),
                      logicCsICrystal,"CsICrystal",
                      m_LogicCsICrystal,false,0);
    
    // Set CsI sensible
    logicCsICrystal->SetSensitiveDetector(m_CsIScorer)
    
    // Visualisation of CsI
    logicCsICrystal->SetVisAttibutes(m_CsIVisAtt);
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Hira::ConstructThinSi(G4LogicalVolume* world){
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Hira::ConstructThickSi(G4LogicalVolume* world){
  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Hira::InitializeMaterial(){
    m_MaterialSilicon = MaterialManager::getInstance()->GetMaterialFromLibrary("Si");
    m_MaterialAluminium = MaterialManager::getInstance()->GetMaterialFromLibrary("Al");
    m_MaterialCsI = MaterialManager::getInstance()->GetMaterialFromLibrary("CsI");
    m_MaterialMylar = MaterialManager::getInstance()->GetMaterialFromLibrary("Mylar");
    m_MaterialPCB = MaterialManager::getInstance()->GetMaterialFromLibrary("PCB");
    m_MaterialVacuum = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
}





