/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : January 2016                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  TRex simulation                                     *
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

//G4 sensitive
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

//G4 various object
#include "G4Material.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4Colour.hh"

// NPTool header
#include "TRex.hh"
#include "SiliconScorers.hh"
#include "RootOutput.h"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "NPOptionManager.h"
// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace TRex_NS{
  // Energy and time Resolution
  const double EnergyThreshold = 0.1*MeV;
  const double ResoTime = 4.5*ns ;
  const double ResoEnergy = 0.1*MeV ;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// TRex Specific Method
TRex::TRex(){
  m_Event = new TTRexData() ;
  m_StripScorer = 0;
  m_PADScorer = 0;

  m_BarrelDetector = 0;
  m_Chamber=0;

  // Dark Grey
  m_SiliconVisAtt = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3)) ;
  // Green
  m_PCBVisAtt = new G4VisAttributes(G4Colour(0.2, 0.5, 0.2)) ;
  // Gold Yellow
  m_PADVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.2)) ;

}

TRex::~TRex(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TRex::AddTRex(double  X, double  Y, double  Z){
  m_X.push_back(X);
  m_Y.push_back(Y);
  m_Z.push_back(Z);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4AssemblyVolume* TRex::BuildBarrelDetector(){
  if(!m_BarrelDetector){
    // Load the reference file
    if(!m_Chamber){
      cout << "TRex geometry is based on Munich Group Simulation exported in GDML"<< endl;
      string basepath = getenv("NPTOOL");
      string path=basepath+"/NPSimulation/Detectors/TRex/TRex_Miniball.gdml";
      m_gdmlparser.Read(path,false);
    }   
    m_BarrelDetector = new G4AssemblyVolume();
    G4LogicalVolume* PCB =  m_gdmlparser.GetVolume("PCBForwardBarrel_log");
    G4LogicalVolume* Det =  m_gdmlparser.GetVolume("FBarrelDeltaESingle0_log");
    G4LogicalVolume* PCBE =  m_gdmlparser.GetVolume("PCBForwardBarrel_log2");
    G4LogicalVolume* DetE =  m_gdmlparser.GetVolume("FBarrelErestSingle0_log");

    PCB->SetVisAttributes(m_PCBVisAtt);
    Det->SetVisAttributes(m_SiliconVisAtt);
    Det->SetSensitiveDetector(m_StripScorer);
    PCBE->SetVisAttributes(m_PADVisAtt);
    DetE->SetVisAttributes(m_SiliconVisAtt); 
    DetE->SetSensitiveDetector(m_PADScorer);

    G4ThreeVector PosPCB(16.5*mm,0,0);
    G4ThreeVector PosDet(0,0,0);
    G4ThreeVector PosPCBE(16.5*mm,1.5*mm,0);
    G4ThreeVector PosDetE(0,1.5*mm,0);

    G4RotationMatrix* Rot = new G4RotationMatrix();
    Rot->rotateZ(90*deg);
    m_BarrelDetector->AddPlacedVolume(PCB, PosPCB, Rot);
    m_BarrelDetector->AddPlacedVolume(Det, PosDet, Rot);
    m_BarrelDetector->AddPlacedVolume(PCBE, PosPCBE, Rot);
    m_BarrelDetector->AddPlacedVolume(DetE, PosDetE, Rot);
  }
  return m_BarrelDetector;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* TRex::BuildChamber(){
  if(!m_Chamber){
    if(!m_BarrelDetector){
      // Load the reference file
      cout << "TRex geometry is based on Munich Group Simulation exported in GDML"<< endl;
      string basepath = getenv("NPTOOL");
      string path=basepath+"/NPSimulation/Detectors/TRex/TRex_Miniball.gdml";
      m_gdmlparser.Read(path);
    }
    m_Chamber= m_gdmlparser.GetVolume("chamber_log");
  }
  return m_Chamber;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void TRex::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("TRex");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " Detector found " << endl; 

  vector<string> token = {"X","Y","Z"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << "//// TRex " << i+1 << endl; 


      double X = blocks[i]->GetDouble("X","mm");
      double Y = blocks[i]->GetDouble("Y","mm");
      double Z = blocks[i]->GetDouble("Z","mm");
      AddTRex(X,Y,Z);
      if(blocks[i]->HasToken("Chamber"))
        if(blocks[i]->GetInt("Chamber"))
          BuildChamber();          
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
void TRex::ConstructDetector(G4LogicalVolume* world){
  BuildBarrelDetector();
  for (unsigned short i = 0 ; i < m_X.size() ; i++) {

    G4double wX = m_X[i];
    G4double wY = m_Y[i];
    G4double wZ = m_Z[i];

    G4ThreeVector Det_pos = G4ThreeVector(wX, wY, wZ) ;

    G4RotationMatrix* Rot= new G4RotationMatrix();

    if(wZ<0)
      Rot->rotateY(180*deg);

    if(wX>0){
      Rot->rotateZ(-90*deg);
    }
    else if(wX<0){
      Rot->rotateZ(90*deg); 
    }

    else if(wY<0)
      Rot->rotateZ(180*deg);

    G4Transform3D Trans(*Rot,Det_pos);
    m_BarrelDetector->MakeImprint(world,Trans,i+1);
    // set a nicer name
    std::vector< G4VPhysicalVolume * >::iterator it = m_BarrelDetector->GetVolumesIterator();
    it+=m_BarrelDetector->GetImprintsCount()*4-1;
    (*it)->SetName("TRex_E_Detector");
    (*it)->SetCopyNo(i+1);
    it--;
    (*it)->SetName("TRex_E_PCB");
    (*it)->SetCopyNo(i+1);
    it--;
    (*it)->SetName("TRex_DE_Detector");
    (*it)->SetCopyNo(i+1);
    it--;
    (*it)->SetName("TRex_DE_PCB");
    (*it)->SetCopyNo(i+1);

  }

  if(m_Chamber){
    // Visual Attribute
    G4VisAttributes* ChamberVisAtt
      = new G4VisAttributes(G4Colour(0.0,0.4,0.5,0.2));
    m_Chamber->SetVisAttributes(ChamberVisAtt);
    new G4PVPlacement(new G4RotationMatrix(),G4ThreeVector(0,0,50),m_Chamber,"TRex_Chamber",world,false,0);
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void TRex::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("TRex")){
    pTree->Branch("TRex", "TTRexData", &m_Event) ;
  }
  pTree->SetBranchAddress("TRex", &m_Event) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void TRex::ReadSensitive(const G4Event* event){
  m_Event->Clear();

  ///////////
  // Striprimeter scorer
  G4THitsMap<G4double*>* StripHitMap;
  std::map<G4int, G4double**>::iterator Strip_itr;

  G4int StripCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("TRexStripScorer/Strip");
  StripHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(StripCollectionID));
  
  G4THitsMap<G4double*>* PADHitMap;
  std::map<G4int, G4double**>::iterator PAD_itr;

  G4int PADCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("TRexPADScorer/PAD");
  PADHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(PADCollectionID));


  // Loop on the Strip map
  for (Strip_itr = StripHitMap->GetMap()->begin() ; Strip_itr != StripHitMap->GetMap()->end() ; Strip_itr++){
    G4double* Info = *(Strip_itr->second);

    double Energy = Info[0];

    if(Energy>TRex_NS::EnergyThreshold){
      double Time       = Info[1];
      int DetNbr        = (int) Info[7];
      int StripFront    = (int) Info[8];
      int StripBack     = (int) Info[9];

      m_Event->SetFrontEnergy(DetNbr,StripFront,RandGauss::shoot(Energy,TRex_NS::ResoEnergy));
      m_Event->SetFrontTime(DetNbr,StripFront,RandGauss::shoot(Time, TRex_NS::ResoTime));

      m_Event->SetBackEnergy(DetNbr,StripBack,RandGauss::shoot(Energy, TRex_NS::ResoEnergy));
      m_Event->SetBackTime(DetNbr,StripBack,RandGauss::shoot(Time, TRex_NS::ResoTime));

      // Interraction Coordinates
      ms_InterCoord->SetDetectedPositionX(Info[2]) ;
      ms_InterCoord->SetDetectedPositionY(Info[3]) ;
      ms_InterCoord->SetDetectedPositionZ(Info[4]) ;
      ms_InterCoord->SetDetectedAngleTheta(Info[5]/deg) ;
      ms_InterCoord->SetDetectedAnglePhi(Info[6]/deg) ;

    }
  }
  // clear map for next event
  StripHitMap->clear();
 
  // Loop on the PAD map
  for (PAD_itr = PADHitMap->GetMap()->begin() ; PAD_itr != PADHitMap->GetMap()->end() ; PAD_itr++){
    G4double* Info = *(PAD_itr->second);

    double Energy = Info[0];

    if(Energy>TRex_NS::EnergyThreshold){
      double Time       = Info[1];
      int DetNbr        = (int) Info[7];

      m_Event->SetPADEnergy(DetNbr,RandGauss::shoot(Energy,TRex_NS::ResoEnergy));
      m_Event->SetPADTime(DetNbr,RandGauss::shoot(Time, TRex_NS::ResoTime));

      // Interraction Coordinates
     /* ms_InterCoord->SetDetectedPositionX(Info[2]) ;
      ms_InterCoord->SetDetectedPositionY(Info[3]) ;
      ms_InterCoord->SetDetectedPositionZ(Info[4]) ;
      ms_InterCoord->SetDetectedAngleTheta(Info[5]/deg) ;
      ms_InterCoord->SetDetectedAnglePhi(Info[6]/deg) ;
*/
    }
  }
  // clear map for next event
  PADHitMap->clear();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////   
void TRex::InitializeScorers() { 
  // This check is necessary in case the geometry is reloaded
  bool already_exist = false; 
  m_StripScorer = CheckScorer("TRexStripScorer",already_exist) ;
   m_PADScorer = CheckScorer("TRexPADScorer",already_exist) ;

  if(already_exist) 
    return ;

  // Otherwise the scorer is initialised
  G4VPrimitiveScorer* Strip= new SILICONSCORERS::PS_Silicon_Rectangle("Strip",0,50*mm,50*mm,16,16,0,"yz") ;
  G4VPrimitiveScorer* PAD= new SILICONSCORERS::PS_Silicon_Rectangle("PAD",0,50*mm,50*mm,1,1,0) ;

  //and register it to the multifunctionnal detector
  m_StripScorer->RegisterPrimitive(Strip);
  m_PADScorer->RegisterPrimitive(PAD);

  G4SDManager::GetSDMpointer()->AddNewDetector(m_StripScorer) ;
  G4SDManager::GetSDMpointer()->AddNewDetector(m_PADScorer) ;


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* TRex::Construct(){
  return  (NPS::VDetector*) new TRex();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
  class proxy_nps_plastic{
    public:
      proxy_nps_plastic(){
        NPS::DetectorFactory::getInstance()->AddToken("TRex","TRex");
        NPS::DetectorFactory::getInstance()->AddDetector("TRex",TRex::Construct);
      }
  };

  proxy_nps_plastic p_nps_plastic;
}
