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
 *  This class describe  Helios simulation                                  *
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
#include "Helios.hh"
#include "DSSDScorers.hh"
#include "InteractionScorers.hh"
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
namespace Helios_NS{
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
// Helios Specific Method
Helios::Helios(){
  m_Event = new THeliosData() ;
  m_HeliosScorer = 0;
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

Helios::~Helios(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Helios::AddHelios(double  Z, string  Face){
  m_Z.push_back(Z);
  m_Face.push_back(Face);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Helios::BuildSquareTube(){
  if(!m_SquareTube){
    G4Box* box = new G4Box("Helios_Box",Helios_NS::SquareTubeSide*0.5,
        Helios_NS::SquareTubeSide*0.5,0.5*(Helios_NS::SquareTubeExcess+Helios_NS::WaferLength));

    G4Tubs* tubs = new G4Tubs("Helios_Box",0,Helios_NS::SquareTubeRadius,
        (Helios_NS::SquareTubeExcess+Helios_NS::WaferLength),0,360*deg);


    G4RotationMatrix* R = new G4RotationMatrix();
    G4ThreeVector P(0,0,0);  
    G4SubtractionSolid* sub = new G4SubtractionSolid("Helios_Sub",box,tubs,R,P);

    G4Material* Al= MaterialManager::getInstance()->GetMaterialFromLibrary("Al");
    m_SquareTube= new G4LogicalVolume(sub,Al,"logic_Helios_Box",0,0,0);
    m_SquareTube->SetVisAttributes(m_VisSquareTube);
  }
  return m_SquareTube;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Helios::BuildSiliconWafer(){
  if(!m_SiliconWafer){
    G4Box* box1 = new G4Box("Helios_Box1",Helios_NS::WaferWidth*0.5,
        Helios_NS::WaferThickness*0.5+Helios_NS::AluThicness,Helios_NS::WaferLength*0.5);

    G4Box* box2 = new G4Box("Helios_Box2",Helios_NS::ActiveWaferWidth*0.5,
        Helios_NS::WaferThickness*0.5,Helios_NS::ActiveWaferLength*0.5);


    G4Material* Si= MaterialManager::getInstance()->GetMaterialFromLibrary("Si");
    G4Material* Al= MaterialManager::getInstance()->GetMaterialFromLibrary("Al");

    m_SiliconWafer= new G4LogicalVolume(box1,Al,"logic_Helios_Wafer",0,0,0);
    m_ActiveWafer= new G4LogicalVolume(box2,Si,"logic_Helios_ActiveWafer",0,0,0);

    G4ThreeVector AWPos(0,0,0);
    G4RotationMatrix* AWRot = new G4RotationMatrix();
    new G4PVPlacement(G4Transform3D(*AWRot,AWPos),m_ActiveWafer,
        "Helios_ActiveWafer",m_SiliconWafer, true, 0);
    m_ActiveWafer->SetSensitiveDetector(m_HeliosScorer);

    m_SiliconWafer->SetVisAttributes(m_VisPassiveSilicon);
    m_ActiveWafer->SetVisAttributes(m_VisSilicon);

  }


  return m_SiliconWafer;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Helios::BuildMagnet(){
  if(!m_Magnet){
    G4Tubs* tubs1 = new G4Tubs("Helios_MainFull",0,
        Helios_NS::MagnetOutterRadius,Helios_NS::MagnetLength*0.5,0,360*deg);

    // Inner part of the Soleinoid minus the Target it self (placed in the world)
    G4SubtractionSolid* tubs = new G4SubtractionSolid("Helios_Main",
        tubs1, Target::GetTarget()->GetTargetSolid(), new G4RotationMatrix() ,Target::GetTarget()->GetTargetPosition());

    G4Tubs* tubs2 = new G4Tubs("Helios_Mag",Helios_NS::MagnetInnerRadius,
        Helios_NS::MagnetOutterRadius,Helios_NS::MagnetLength*0.5,0,360*deg);

    G4Material* Fe= MaterialManager::getInstance()->GetMaterialFromLibrary("Fe");
    G4Material* Vc= MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");

    m_Magnet= new G4LogicalVolume(tubs,Vc,"logic_Helios_Main",0,0,0);
    G4LogicalVolume* Mag = new G4LogicalVolume(tubs2,Fe,"logic_Helios_Magnet",0,0,0);

    Mag->SetVisAttributes(m_VisMagnet);
    m_Magnet->SetVisAttributes(G4VisAttributes::Invisible);
    // Place the Solenoid
    G4ThreeVector MagPos(0,0,0);
    G4RotationMatrix* MagRot = new G4RotationMatrix();

    new G4PVPlacement(G4Transform3D(*MagRot,MagPos),
        Mag,
        "Helios_Magnet",m_Magnet,false,0);

  }
  return m_Magnet;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Helios::ReadConfiguration(NPL::InputParser parser ){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Helios");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> token = {"Z","Face"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasToken("MagneticField"))
      m_B=blocks[i]->GetDouble("MagneticField","T");

    if(blocks[i]->HasTokenList(token)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Helios " << i+1 <<  endl;
      double Z = blocks[i]->GetDouble("Z","mm");
      string Face = blocks[i]->GetString("Face");
      AddHelios(Z,Face);
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
void Helios::ConstructDetector(G4LogicalVolume* world){
  // Place the Solenoid
  G4ThreeVector MagPos(0,0,0);
  G4RotationMatrix* MagRot = new G4RotationMatrix();

  new G4PVPlacement(G4Transform3D(*MagRot,MagPos),
      BuildMagnet(),
      "Helios",world,false,0);

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
      DetPos = G4ThreeVector(0,Helios_NS::SquareTubeSide*0.5+1*mm,m_Z[i]);
      DetRot = new G4RotationMatrix();
    }
    else if(m_Face[i]=="Right"){
      DetPos = G4ThreeVector(Helios_NS::SquareTubeSide*0.5+1*mm,0,m_Z[i]);
      DetRot = new G4RotationMatrix();
      DetRot->rotateZ(90*deg);
    }
    else if(m_Face[i]=="Down"){
      DetPos = G4ThreeVector(0,-Helios_NS::SquareTubeSide*0.5-1*mm,m_Z[i]);
      DetRot = new G4RotationMatrix();
      DetRot->rotateZ(180*deg);
    }
    else if(m_Face[i]=="Left"){
      DetPos = G4ThreeVector(-Helios_NS::SquareTubeSide*0.5-1*mm,0,m_Z[i]);
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
        "Helios_SiliconWafer",BuildMagnet(),false,i+1);

    // Place the Square Tub
    if(m_UsedZ.find(m_Z[i])==m_UsedZ.end()){
      G4ThreeVector TubePos(0,0,m_Z[i]);
      G4RotationMatrix* TubeRot = new G4RotationMatrix();
      new G4PVPlacement(G4Transform3D(*TubeRot,TubePos),
          BuildSquareTube(),
          "Helios_SquareTube",BuildMagnet(),false,i+1);
      m_UsedZ.insert(m_Z[i]);
    }
  }

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Helios::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("Helios")){
    pTree->Branch("Helios", "THeliosData", &m_Event) ;
  }

  pTree->SetBranchAddress("Helios", &m_Event) ;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Helios::ReadSensitive(const G4Event* ){
  m_Event->Clear();

  ///////////
  // Resistiverimeter scorer
  DSSDScorers::PS_Resistive* Scorer= (DSSDScorers::PS_Resistive*) m_HeliosScorer->GetPrimitive(0);
  
  // Loop on the Back
  unsigned int sizeBack = Scorer->GetBackMult();
  for(unsigned int i = 0 ; i < sizeBack ; i++){
    double EBack = RandGauss::shoot(Scorer->GetEnergyBack(i),Helios_NS::ResoEnergyBack);
    double TBack = RandGauss::shoot(Scorer->GetTimeBack(i),Helios_NS::ResoTime);
    if(EBack>Helios_NS::EnergyThreshold){
      m_Event->SetEBack(Scorer->GetDetectorBack(i),EBack);
      m_Event->SetTBack(Scorer->GetDetectorBack(i),TBack);  
    }
  }
  // Loop on the Up 
  unsigned int sizeUp = Scorer->GetUpMult();
  for(unsigned int i = 0 ; i < sizeUp ; i++){
    double EUp = RandGauss::shoot(Scorer->GetEnergyUp(i),Helios_NS::ResoEnergyFront);
    double TUp = RandGauss::shoot(Scorer->GetTimeUp(i),Helios_NS::ResoTime);
    if(EUp>Helios_NS::EnergyThreshold){
      m_Event->SetEUp(Scorer->GetDetectorUp(i),EUp);
      m_Event->SetTUp(Scorer->GetDetectorUp(i),TUp);  
    }
  }
  
  // Loop on the Down 
  unsigned int sizeDown = Scorer->GetDownMult();
  for(unsigned int i = 0 ; i < sizeDown ; i++){
    double EDw = RandGauss::shoot(Scorer->GetEnergyDown(i),Helios_NS::ResoEnergyFront);
    double TDw = RandGauss::shoot(Scorer->GetTimeDown(i),Helios_NS::ResoTime);
    if(EDw>Helios_NS::EnergyThreshold){
      m_Event->SetEDw(Scorer->GetDetectorDown(i),EDw);
      m_Event->SetTDw(Scorer->GetDetectorDown(i),TDw);  
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////   
void Helios::InitializeScorers() { 
  // This check is necessary in case the geometry is reloaded
  bool already_exist = false; 
  m_HeliosScorer = CheckScorer("HeliosScorer",already_exist) ;

  if(already_exist) 
    return ;

  // Otherwise the scorer is initialised
  G4VPrimitiveScorer* Resistive= new DSSDScorers::PS_Resistive("Resistive",1,Helios_NS::WaferLength,Helios_NS::WaferWidth,1) ;
  //and register it to the multifunctionnal detector
  m_HeliosScorer->RegisterPrimitive(Resistive);
  G4VPrimitiveScorer* Inter = new InteractionScorers::PS_Interactions("Resistive",ms_InterCoord,1) ; 
  m_HeliosScorer->RegisterPrimitive(Inter);

  G4SDManager::GetSDMpointer()->AddNewDetector(m_HeliosScorer) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* Helios::Construct(){
  return  (NPS::VDetector*) new Helios();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
  class proxy_nps_Helios{
    public:
      proxy_nps_Helios(){
        NPS::DetectorFactory::getInstance()->AddToken("Helios","Helios");
        NPS::DetectorFactory::getInstance()->AddDetector("Helios",Helios::Construct);
      }
  };

  proxy_nps_Helios p_nps_Helios;
}
