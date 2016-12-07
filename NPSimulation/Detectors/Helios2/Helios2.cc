/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : octobre 2016                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  Helios2 simulation                                  *
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
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"

//G4 sensitive
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

//G4 various object
#include "G4Material.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

// G4 Field
#include "G4FieldManager.hh"
#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include "G4TransportationManager.hh"

// NPTool header
#include "Helios2.hh"
#include "SiliconScorers.hh"
#include "RootOutput.h"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "Target.hh"
#include "NPOptionManager.h"
// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace Helios2_NS{
  // Energy and time Resolution
  const double EnergyThreshold = 100*keV;
  const double ResoTime = 1*ns ;
  const double ResoEnergyFront = 50*keV ;
  const double ResoEnergyBack = 24*keV;
  const double MagnetInnerRadius = 46*cm;
  const double MagnetOutterRadius = 1*m; 
  const double MagnetLength = 2.35*m; 

  const double SquareTubeSide = 16*mm;
  const double SquareTubeExcess = 2.4*mm;// Wafer to Wafer distance
  const double SquareTubeRadius = 5*mm;
  const double WaferWidth = 12*mm ;
  const double WaferLength = 56 *mm ;
  const double ActiveWaferWidth = 9*mm ;
  const double ActiveWaferLength = 50.5 *mm ;
  const double AluThicness = 0.3*micrometer;
  const double WaferThickness = 700* micrometer; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Helios2 Specific Method
Helios2::Helios2(){
  m_Event = new THelios2Data() ;
  m_Helios2Scorer = 0;
  m_SquareTube = 0;
  m_SiliconWafer = 0;
  m_ActiveWafer = 0;
  m_Magnet = 0;
  m_B=0;

  // RGB Color + Transparency
  m_VisSquareTube = new G4VisAttributes(G4Colour(0.8, 0.8, 0.8, 0.5));   
  m_VisPassiveSilicon    = new G4VisAttributes(G4Colour(0.1, 0.1, 0.1, 1));   
  m_VisSilicon    = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3, 1));   
  m_VisMagnet     = new G4VisAttributes(G4Colour(0, 0.5, 1, 0.3));  
}

Helios2::~Helios2(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Helios2::AddHelios2(double  Z, string  Face){
  m_Z.push_back(Z);
  m_Face.push_back(Face);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Helios2::BuildSquareTube(){
  if(!m_SquareTube){
    G4Box* box = new G4Box("Helios2_Box",Helios2_NS::SquareTubeSide*0.5,
        Helios2_NS::SquareTubeSide*0.5,0.5*(Helios2_NS::SquareTubeExcess+Helios2_NS::WaferLength));

    G4Tubs* tubs = new G4Tubs("Helios2_Box",0,Helios2_NS::SquareTubeRadius,
                  (Helios2_NS::SquareTubeExcess+Helios2_NS::WaferLength),0,360*deg);
 
 
    G4RotationMatrix* R = new G4RotationMatrix();
    G4ThreeVector P(0,0,0);  
    G4SubtractionSolid* sub = new G4SubtractionSolid("Helios2_Sub",box,tubs,R,P);

    G4Material* Al= MaterialManager::getInstance()->GetMaterialFromLibrary("Al");
    m_SquareTube= new G4LogicalVolume(sub,Al,"logic_Helios2_Box",0,0,0);
    m_SquareTube->SetVisAttributes(m_VisSquareTube);
  }
  return m_SquareTube;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Helios2::BuildSiliconWafer(){
  if(!m_SiliconWafer){
    G4Box* box1 = new G4Box("Helios2_Box1",Helios2_NS::WaferWidth*0.5,
        Helios2_NS::WaferThickness*0.5+Helios2_NS::AluThicness,Helios2_NS::WaferLength*0.5);

    G4Box* box2 = new G4Box("Helios2_Box2",Helios2_NS::ActiveWaferWidth*0.5,
        Helios2_NS::WaferThickness*0.5,Helios2_NS::ActiveWaferLength*0.5);


    G4Material* Si= MaterialManager::getInstance()->GetMaterialFromLibrary("Si");
    G4Material* Al= MaterialManager::getInstance()->GetMaterialFromLibrary("Al");

    m_SiliconWafer= new G4LogicalVolume(box1,Al,"logic_Helios2_Wafer",0,0,0);
    m_ActiveWafer= new G4LogicalVolume(box2,Si,"logic_Helios2_ActiveWafer",0,0,0);

    G4ThreeVector AWPos(0,0,0);
    G4RotationMatrix* AWRot = new G4RotationMatrix();
    new G4PVPlacement(G4Transform3D(*AWRot,AWPos),m_ActiveWafer,
                      "Helios2_ActiveWafer",m_SiliconWafer, true, 0);
    m_ActiveWafer->SetSensitiveDetector(m_Helios2Scorer);

    m_SiliconWafer->SetVisAttributes(m_VisPassiveSilicon);
    m_ActiveWafer->SetVisAttributes(m_VisSilicon);
 
    }

  
  return m_SiliconWafer;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Helios2::BuildMagnet(){
  if(!m_Magnet){
    G4Tubs* tubs1 = new G4Tubs("Helios2_MainFull",0,
        Helios2_NS::MagnetOutterRadius,Helios2_NS::MagnetLength*0.5,0,360*deg);
  
    // Inner part of the Soleinoid minus the Target it self (placed in the world)
    G4SubtractionSolid* tubs = new G4SubtractionSolid("Helios_Main",
                                tubs1, Target::GetTarget()->GetTargetSolid(), new G4RotationMatrix() ,Target::GetTarget()->GetTargetPosition());

    G4Tubs* tubs2 = new G4Tubs("Helios2_Mag",Helios2_NS::MagnetInnerRadius,
        Helios2_NS::MagnetOutterRadius,Helios2_NS::MagnetLength*0.5,0,360*deg);

    G4Material* Fe= MaterialManager::getInstance()->GetMaterialFromLibrary("Fe");
    G4Material* Vc= MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
  
    m_Magnet= new G4LogicalVolume(tubs,Vc,"logic_Helios2_Main",0,0,0);
    G4LogicalVolume* Mag = new G4LogicalVolume(tubs2,Fe,"logic_Helios2_Magnet",0,0,0);
    
    Mag->SetVisAttributes(m_VisMagnet);
    m_Magnet->SetVisAttributes(G4VisAttributes::Invisible);
    // Place the Solenoid
    G4ThreeVector MagPos(0,0,0);
    G4RotationMatrix* MagRot = new G4RotationMatrix();

    new G4PVPlacement(G4Transform3D(*MagRot,MagPos),
        Mag,
        "Helios2_Magnet",m_Magnet,false,0);
  
  }
  return m_Magnet;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Helios2::ReadConfiguration(NPL::InputParser parser ){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Helios2");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> token = {"Z","Face"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasToken("MagneticField"))
      m_B=blocks[i]->GetDouble("MagneticField","T");

    if(blocks[i]->HasTokenList(token)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Helios2 " << i+1 <<  endl;
      double Z = blocks[i]->GetDouble("Z","mm");
      string Face = blocks[i]->GetString("Face");
      AddHelios2(Z,Face);
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
void Helios2::ConstructDetector(G4LogicalVolume* world){
  // Place the Solenoid
  G4ThreeVector MagPos(0,0,0);
  G4RotationMatrix* MagRot = new G4RotationMatrix();

  new G4PVPlacement(G4Transform3D(*MagRot,MagPos),
      BuildMagnet(),
      "Helios2",world,false,0);

  // Add the Magnetic field
  static G4FieldManager* fieldMgr = new G4FieldManager();
  G4MagneticField* magField = new G4UniformMagField(G4ThreeVector(0.,0.,m_B));
  fieldMgr->SetDetectorField(magField);
  if(!fieldMgr->GetChordFinder())
    fieldMgr->CreateChordFinder(magField); 
  BuildMagnet()->SetFieldManager(fieldMgr,true);

  fieldMgr->SetMinimumEpsilonStep( 1*mm);
  fieldMgr->SetMaximumEpsilonStep( 10*m );
  fieldMgr->SetDeltaOneStep( 1 * mm ); 
  
  // Place detectors and support inside it
  for (unsigned short i = 0 ; i < m_Z.size() ; i++) {
    G4ThreeVector DetPos;
    G4RotationMatrix* DetRot = NULL;

    if(m_Face[i]=="Up"){
      DetPos = G4ThreeVector(0,Helios2_NS::SquareTubeSide*0.5+1*mm,m_Z[i]);
      DetRot = new G4RotationMatrix();
    }
    else if(m_Face[i]=="Right"){
      DetPos = G4ThreeVector(Helios2_NS::SquareTubeSide*0.5+1*mm,0,m_Z[i]);
      DetRot = new G4RotationMatrix();
      DetRot->rotateZ(90*deg);
    }
    else if(m_Face[i]=="Down"){
      DetPos = G4ThreeVector(0,-Helios2_NS::SquareTubeSide*0.5-1*mm,m_Z[i]);
      DetRot = new G4RotationMatrix();
      DetRot->rotateZ(180*deg);
    }
    else if(m_Face[i]=="Left"){
      DetPos = G4ThreeVector(-Helios2_NS::SquareTubeSide*0.5-1*mm,0,m_Z[i]);
      DetRot = new G4RotationMatrix();
      DetRot->rotateZ(270*deg);
    }
    else{
      cout << "Face orientation for Helios 2 detector "<< m_Face[i] << " Not Valid!" << endl;
      exit(1);
    }

    // Place the Silicon Wafer
    new G4PVPlacement(G4Transform3D(*DetRot,DetPos),
        BuildSiliconWafer(),
        "Helios2_SiliconWafer",BuildMagnet(),false,i+1);

    // Place the Square Tub
    if(m_UsedZ.find(m_Z[i])==m_UsedZ.end()){
      G4ThreeVector TubePos(0,0,m_Z[i]);
      G4RotationMatrix* TubeRot = new G4RotationMatrix();
      new G4PVPlacement(G4Transform3D(*TubeRot,TubePos),
          BuildSquareTube(),
          "Helios2_SquareTube",BuildMagnet(),false,i+1);
      m_UsedZ.insert(m_Z[i]);
    }
  }

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Helios2::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("Helios2")){
    pTree->Branch("Helios2", "THelios2Data", &m_Event) ;
  }

  pTree->SetBranchAddress("Helios2", &m_Event) ;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Helios2::ReadSensitive(const G4Event* event){
    m_Event->Clear();

  ///////////
  // Resistiverimeter scorer
  G4THitsMap<G4double*>* ResistiveHitMap;
  std::map<G4int, G4double**>::iterator Resistive_itr;

  G4int ResistiveCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Helios2Scorer/Resistive");
  ResistiveHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(ResistiveCollectionID));

  // Loop on the Resistive map
  for (Resistive_itr = ResistiveHitMap->GetMap()->begin() ; Resistive_itr != ResistiveHitMap->GetMap()->end() ; Resistive_itr++){
  G4double* Info = *(Resistive_itr->second);
  double EBack = RandGauss::shoot(Info[0]+Info[1],Helios2_NS::ResoEnergyBack);
  double TBack = RandGauss::shoot(Info[2],Helios2_NS::ResoTime);
  double EUp = RandGauss::shoot(Info[1],Helios2_NS::ResoEnergyFront);
  double TUp = RandGauss::shoot(Info[2],Helios2_NS::ResoTime);
  double EDw = RandGauss::shoot(Info[0],Helios2_NS::ResoEnergyFront);
  double TDw = RandGauss::shoot(Info[2],Helios2_NS::ResoTime);

  if(EBack>Helios2_NS::EnergyThreshold){
    m_Event->SetEBack(Info[3],EBack);
    m_Event->SetTBack(Info[3],TBack);  
  }

  if(EUp>Helios2_NS::EnergyThreshold){
    m_Event->SetEUp(Info[3],EUp);
    m_Event->SetTUp(Info[3],TUp);  
  }

  if(EUp>Helios2_NS::EnergyThreshold){
    m_Event->SetEUp(Info[3],EUp);
    m_Event->SetTUp(Info[3],TUp);  
  }

  if(EDw>Helios2_NS::EnergyThreshold){
    m_Event->SetEDw(Info[3],EDw);
    m_Event->SetTDw(Info[3],TDw);  
  }

  }
  // clear map for next event
  ResistiveHitMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////   
void Helios2::InitializeScorers() { 
  // This check is necessary in case the geometry is reloaded
  bool already_exist = false; 
  m_Helios2Scorer = CheckScorer("Helios2Scorer",already_exist) ;

  if(already_exist) 
    return ;

  // Otherwise the scorer is initialised
  G4VPrimitiveScorer* Resistive= new SILICONSCORERS::PS_Silicon_Resistive("Resistive",1,Helios2_NS::WaferLength,Helios2_NS::WaferWidth,1) ;
  //and register it to the multifunctionnal detector
  m_Helios2Scorer->RegisterPrimitive(Resistive);
  G4SDManager::GetSDMpointer()->AddNewDetector(m_Helios2Scorer) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* Helios2::Construct(){
  return  (NPS::VDetector*) new Helios2();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
  class proxy_nps_Helios2{
    public:
      proxy_nps_Helios2(){
        NPS::DetectorFactory::getInstance()->AddToken("Helios2","Helios2");
        NPS::DetectorFactory::getInstance()->AddDetector("Helios2",Helios2::Construct);
      }
  };

  proxy_nps_Helios2 p_nps_Helios2;
}
