/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    : October 2009                                             *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe a 20um Silicium detector                             *
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
#include "G4Trd.hh"
#include "G4Box.hh"
#include "G4Trap.hh"

//G4 sensitive
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

//G4 various object
#include "G4Material.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PVDivision.hh"
#include "G4SubtractionSolid.hh"

// NPTool header
#include "SSSD.hh"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "SiliconScorers.hh"
#include "RootOutput.h"
#include "NPOptionManager.h"
using namespace SSSD_LOCAL;

// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// SSSD Specific Method
SSSD::SSSD(){
  InitializeMaterial();
  m_Event = new TSSSDData();
  m_StripScorer=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
SSSD::~SSSD(){
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SSSD::AddTelescope( G4ThreeVector TL,
    G4ThreeVector BL,
    G4ThreeVector BR,
    G4ThreeVector TR)
{
  m_DefinitionType.push_back(true);

  m_TL.push_back(TL);
  m_BL.push_back(BL);
  m_BR.push_back(BR);
  m_TR.push_back(TR);

  m_R.push_back(0);
  m_Theta.push_back(0);
  m_Phi.push_back(0);
  m_beta_u.push_back(0);
  m_beta_v.push_back(0);
  m_beta_w.push_back(0);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SSSD::AddTelescope( G4double R,
    G4double Theta,
    G4double Phi,
    G4double beta_u,
    G4double beta_v,
    G4double beta_w)
{
  G4ThreeVector empty = G4ThreeVector(0, 0, 0);

  m_DefinitionType.push_back(false);

  m_R.push_back(R);
  m_Theta.push_back(Theta);
  m_Phi.push_back(Phi);
  m_beta_u.push_back(beta_u);
  m_beta_v.push_back(beta_v);
  m_beta_w.push_back(beta_w);

  m_TL.push_back(empty);
  m_BL.push_back(empty);
  m_BR.push_back(empty);
  m_TR.push_back(empty);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SSSD::VolumeMaker(  G4int             DetNumber ,
    G4ThreeVector     Det_pos ,
    G4RotationMatrix* Det_rot ,
    G4LogicalVolume*  world   )
{
  G4double NbrTelescopes = DetNumber  ;
  G4String DetectorNumber ;
  std::ostringstream Number ;
  Number << NbrTelescopes ;
  DetectorNumber = Number.str() ;

  ////////////////////////////////////////////////////////////////
  /////////General Geometry Parameter Definition /////////////////
  ////////////////////////////////////////////////////////////////
  /////// Starting Volume Definition ///////
  G4String Name = "SSSD" + DetectorNumber;

  G4Box* solidSSSD = new G4Box(Name+"Solid", 0.5*DetectorSize, 0.5*DetectorSize, 0.5*FrameThickness);

  G4LogicalVolume* logicSSSD =
    new G4LogicalVolume(solidSSSD, m_MaterialVacuum, Name+"_logic", 0, 0);

  new G4PVPlacement(   G4Transform3D(*Det_rot, Det_pos) ,
      logicSSSD         ,
      Name                ,
      world               ,
      false               ,
      DetNumber);

  logicSSSD->SetVisAttributes(G4VisAttributes::Invisible);
  // Frame is made of 4 thick box (2 Horizontal and 2 Vertical)
  G4Box* solidFrameHorizontal = new G4Box(Name + "_Frame", 0.5*SiliconSize, 0.5*(DetectorSize - SiliconSize) / 2, 0.5*FrameThickness*mm)   ;
  G4Box* solidFrameVertical  = new G4Box(Name + "_Frame", 0.5*(DetectorSize - SiliconSize) / 2, 0.5*DetectorSize, 0.5*FrameThickness*mm)   ;

  G4LogicalVolume* logicFrameHorizontal = new G4LogicalVolume(solidFrameHorizontal, m_MaterialAl, Name, 0, 0);
  G4VisAttributes* VisAtt1 = new G4VisAttributes(G4Colour(0.0, 0.5, 1));
  logicFrameHorizontal->SetVisAttributes(VisAtt1);


  G4LogicalVolume* logicFrameVertical = new G4LogicalVolume(solidFrameVertical, m_MaterialAl, Name, 0, 0);
  logicFrameVertical->SetVisAttributes(VisAtt1); 

  G4ThreeVector FrameTopPosition    = G4ThreeVector(0 ,  0.5 * SiliconSize + 0.5 * (DetectorSize - SiliconSize) / 2 , 0) ; 
  G4ThreeVector FrameBottomPosition = G4ThreeVector(0 , -0.5 * SiliconSize - 0.5 * (DetectorSize - SiliconSize) / 2 , 0) ;
  G4ThreeVector FrameLeftPosition   = G4ThreeVector(0.5 * SiliconSize + 0.5 * (DetectorSize - SiliconSize) / 2 , 0 , 0) ;
  G4ThreeVector FrameRightPosition  = G4ThreeVector(-0.5 * SiliconSize - 0.5 * (DetectorSize - SiliconSize) / 2 , 0 , 0) ;


  new G4PVPlacement(   0,
      FrameTopPosition,
      logicFrameHorizontal,
      Name + "_Frame",
      logicSSSD,
      false,
      DetNumber);

  new G4PVPlacement(   0,
      FrameBottomPosition,
      logicFrameHorizontal,
      Name + "_Frame",
      logicSSSD,
      false,
      DetNumber);

  new G4PVPlacement(   0,
      FrameLeftPosition,
      logicFrameVertical,
      Name + "_Frame",
      logicSSSD,
      false,
      DetNumber);

  new G4PVPlacement(   0,
      FrameRightPosition,
      logicFrameVertical,
      Name + "_Frame",
      logicSSSD,
      false,
      DetNumber);


  G4ThreeVector posAluFront  = G4ThreeVector(0 ,  0  ,  AluStripFront_PosZ)  ;
  G4ThreeVector posSi        = G4ThreeVector(0 ,  0  ,  0                 )  ;
  G4ThreeVector posAluBack   = G4ThreeVector(0 ,  0  ,  AluStripBack_PosZ )  ;

  G4Box*   solidAlu  =
    new G4Box("SSSDAlu", 0.5*SiliconSize, 0.5*SiliconSize, 0.5*AluThickness) ;

  G4LogicalVolume* logicAlu  =
    new G4LogicalVolume(solidAlu, m_MaterialAl, "logicAlu", 0, 0, 0)    ;


  new G4PVPlacement(0  ,  posAluFront ,  logicAlu ,  Name + "_AluFront"   ,  logicSSSD ,  true, DetNumber)  ;

  new G4PVPlacement(0  ,  posAluBack  ,  logicAlu ,  Name + "_AluBack"    ,  logicSSSD ,  true, DetNumber)  ;

  G4Box*   solidSi  =
    new G4Box("SSSD", 0.5*SiliconSize, 0.5*SiliconSize, 0.5*SiliconThickness);

  G4LogicalVolume* logicSi  =
    new G4LogicalVolume(solidSi, m_MaterialSilicon, "logicSi", 0, 0, 0);

  logicAlu->SetVisAttributes(G4VisAttributes::Invisible);  

  new G4PVPlacement(0, posSi, logicSi, Name + "_Si", logicSSSD, true, DetNumber);

  //attach it to the Silicon plate
  logicSi ->SetSensitiveDetector(m_StripScorer);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void SSSD::ReadConfiguration(NPL::InputParser parser){
vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("SSSD");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> cart = {"A","B","C","D"};
  vector<string> sphe = {"R","THETA","PHI","BETA"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  SSSD " << i+1 <<  endl;
      G4ThreeVector A = NPS::ConvertVector(blocks[i]->GetTVector3("A","mm"));
      G4ThreeVector B = NPS::ConvertVector(blocks[i]->GetTVector3("B","mm"));
      G4ThreeVector C = NPS::ConvertVector(blocks[i]->GetTVector3("C","mm"));
      G4ThreeVector D = NPS::ConvertVector(blocks[i]->GetTVector3("D","mm"));
      AddTelescope(A,B,C,D);
    }
    else if(blocks[i]->HasTokenList(sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  SSSD " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("THETA","deg");
      double Phi = blocks[i]->GetDouble("PHI","deg");
      vector<double> beta = blocks[i]->GetVectorDouble("BETA","deg");
      AddTelescope(R,Theta,Phi,beta[0],beta[1],beta[2]);
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
void SSSD::ConstructDetector(G4LogicalVolume* world){

  G4RotationMatrix* Det_rot  = NULL;
  G4ThreeVector     Det_pos  = G4ThreeVector(0, 0, 0);
  G4ThreeVector     Det_u    = G4ThreeVector(0, 0, 0);
  G4ThreeVector     Det_v    = G4ThreeVector(0, 0, 0);
  G4ThreeVector     Det_w    = G4ThreeVector(0, 0, 0);

  G4int NumberOfDetector = m_DefinitionType.size();

  for (G4int i = 0 ; i < NumberOfDetector ; i++) {
    // By Point
    if (m_DefinitionType[i]) {
      // (u,v,w) unitary vector associated to telescope referencial
      // (u,v) // to silicon plan
      // w perpendicular to (u,v) plan and pointing outside
      Det_u = m_TL[i] - m_BL[i];
      Det_u = Det_u.unit();
      Det_v = m_BR[i] - m_BL[i];
      Det_v = Det_v.unit();
      Det_w = Det_u.cross(Det_v);
      Det_w = Det_w.unit();
      // Passage Matrix from Lab Referential to Telescope Referential
      // MUST2
      Det_rot = new G4RotationMatrix(Det_u, Det_v, Det_w);
      // translation to place Telescope
      Det_pos =  (m_TR[i]+m_TL[i]+m_BL[i]+m_BR[i])/4 ;
    }

    // By Angle
    else {
      G4double Theta = m_Theta[i];
      G4double Phi   = m_Phi[i];
      //This part because if Phi and Theta = 0 equation are false
      if (Theta == 0) Theta   = 0.0001;
      if (Theta == 2*cos(0)) Theta   = 2 * acos(0) - 0.00001;
      if (Phi   == 0) Phi = 0.0001;


      // (u,v,w) unitary vector associated to telescope referencial
      // (u,v) // to silicon plan
      // w perpendicular to (u,v) plan and pointing ThirdStage
      // Phi is angle between X axis and projection in (X,Y) plan
      // Theta is angle between  position vector and z axis
      G4double wX = m_R[i] * sin(Theta / rad) * cos(Phi / rad)   ;
      G4double wY = m_R[i] * sin(Theta / rad) * sin(Phi / rad)   ;
      G4double wZ = m_R[i] * cos(Theta / rad);
      Det_w = G4ThreeVector(wX, wY, wZ);

      // vector corresponding to the center of the module
      G4ThreeVector CT = Det_w;

      // vector parallel to one axis of silicon plane
      G4double ii = cos(Theta / rad) * cos(Phi / rad);
      G4double jj = cos(Theta / rad) * sin(Phi / rad);
      G4double kk = -sin(Theta / rad);
      G4ThreeVector Y = G4ThreeVector(ii, jj, kk);

      Det_w = Det_w.unit();
      Det_u = Det_w.cross(Y);
      Det_v = Det_w.cross(Det_u);
      Det_v = Det_v.unit();
      Det_u = Det_u.unit();

      // Passage Matrix from Lab Referential to Telescope Referential
      // MUST2
      Det_rot = new G4RotationMatrix(Det_u, Det_v, Det_w);
      // Telescope is rotate of Beta angle around Det_v axis.
      Det_rot->rotate(m_beta_u[i], Det_u);
      Det_rot->rotate(m_beta_v[i], Det_v);
      Det_rot->rotate(m_beta_w[i], Det_w);
      // translation to place Telescope
      Det_pos = Det_w + CT ;
    }

    VolumeMaker(i + 1 , Det_pos , Det_rot , world);
  }

  delete Det_rot ;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void SSSD::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("SSSD")){
    pTree->Branch("SSSD", "TSSSDData", &m_Event) ;
  }
  pTree->SetBranchAddress("SSSD", &m_Event) ;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void SSSD::ReadSensitive(const G4Event* event){
  m_Event->Clear();

  ///////////
  G4THitsMap<G4double*>* StripHitMap;
  std::map<G4int, G4double**>::iterator Strip_itr;
  
  G4int StripCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("SSSD_Scorer/StripScorer");
  StripHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(StripCollectionID));
  
  // Loop on the Strip map
  for (Strip_itr = StripHitMap->GetMap()->begin() ; Strip_itr != StripHitMap->GetMap()->end() ; Strip_itr++){
    
    G4double* Info = *(Strip_itr->second);
    
    double Energy = Info[0];
    
    if(Energy>EnergyThreshold){
      double Time       = Info[1];
      int DetNbr        = (int) Info[7];
      int StripFront    = (int) Info[8];

      m_Event->SetEnergyDetectorNbr(DetNbr);
      m_Event->SetEnergyStripNbr(StripFront);
      m_Event->SetEnergy(RandGauss::shoot(Energy, ResoEnergy));
    
      m_Event->SetTimeDetectorNbr(DetNbr);
      m_Event->SetTimeStripNbr(StripFront);
      m_Event->SetTime(RandGauss::shoot(Time, ResoTime));
      
    }
  }
  // clear map for next event
  StripHitMap->clear();
 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SSSD::InitializeScorers(){
  bool already_exist = false;
  //   Silicon Associate Scorer
  m_StripScorer = CheckScorer("SSSD_Scorer",already_exist);
  if(already_exist) return;

  G4VPrimitiveScorer* StripScorer =
    new  SILICONSCORERS::PS_Silicon_Rectangle("StripScorer",0,
        DetectorSize,
        DetectorSize,
        NumberOfStrip,
        1);

  m_StripScorer->RegisterPrimitive(StripScorer);
  G4SDManager::GetSDMpointer()->AddNewDetector(m_StripScorer);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////
/////////////////Material Definition ///////////////////////////
////////////////////////////////////////////////////////////////
void SSSD::InitializeMaterial(){
  m_MaterialSilicon = MaterialManager::getInstance()->GetMaterialFromLibrary("Si");
  m_MaterialAl = MaterialManager::getInstance()->GetMaterialFromLibrary("Al");
  m_MaterialVacuum = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* SSSD::Construct(){
  return  (NPS::VDetector*) new SSSD();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
  class proxy_nps_sssd{
    public:
      proxy_nps_sssd(){
        NPS::DetectorFactory::getInstance()->AddToken("SSSD","SSSD");
        NPS::DetectorFactory::getInstance()->AddDetector("SSSD",SSSD::Construct);
      }
  };

  proxy_nps_sssd p_nps_sssd;
}
