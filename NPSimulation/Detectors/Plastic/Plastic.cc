/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : September 2009                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe a Modular cylindrical Plastic Scintillator           *
 *   Few Material are instantiate and user can choose position and dimension    *
 *  but also the adding of a lead plate on the rear side of the detector     *
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
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

// NPTool header
#include "Plastic.hh"
#include "ObsoleteGeneralScorers.hh"
#include "RootOutput.h"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "NPOptionManager.h"
using namespace OBSOLETEGENERALSCORERS ;
// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace PLASTIC{
  // Energy and time Resolution
  const G4double ResoTime    = 1.         ;// Resolution in ns  //
  const G4double ResoEnergy  = 0.1         ;// Resolution in %
}

using namespace PLASTIC ;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Plastic Specific Method
Plastic::Plastic(){
  m_Event = new TPlasticData() ;
  m_PlasticScorer = 0;
}

Plastic::~Plastic(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Plastic::AddPlastic(   G4double  R                       ,
    G4double  Theta                   ,
    G4double  Phi                  ,
    G4double   PlasticThickness   ,
    G4double   PlasticRadius         ,
    G4String    Scintillator         ,
    G4double    LeadThickness         )
{

  m_R.push_back(R)                                         ;
  m_Theta.push_back(Theta)                                ;
  m_Phi.push_back(Phi)                                     ;
  m_PlasticThickness.push_back(PlasticThickness)   ;
  m_LeadThickness.push_back(LeadThickness)            ;
  m_Scintillator.push_back(Scintillator)               ;
  m_PlasticRadius.push_back(PlasticRadius)            ; // cylindrical shape
  m_PlasticHeight.push_back(-1)                              ; // squared shape
  m_PlasticWidth.push_back(-1)                              ; // squared shape
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Plastic::AddPlastic(   G4double R                      ,
    G4double Theta                ,
    G4double Phi                     ,
    G4double Height                  ,
    G4double Width                  ,
    G4double PlasticThickness   ,
    G4String Scintillator         ,
    G4double LeadThickness      )
{
  m_R.push_back(R)                                         ;
  m_Theta.push_back(Theta)                                ;
  m_Phi.push_back(Phi)                                     ;
  m_PlasticThickness.push_back(PlasticThickness)   ;
  m_LeadThickness.push_back(LeadThickness)            ;
  m_Scintillator.push_back(Scintillator)               ;
  m_PlasticRadius.push_back(-1)            ; // cylindrical shape
  m_PlasticHeight.push_back(Height)                        ; // squared shape
  m_PlasticWidth.push_back(Width)                           ; // squared shape

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class


// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Plastic::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Plastic");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> cart = {"X","Y","Z"};
  vector<string> sphe = {"R","Theta","Phi"};
  vector<string> square= {"Shape","Height","Width","Thickness","Scintillator","LeadThickness"};
  vector<string> cylind= {"Shape","Radius","Thickness","Scintillator","LeadThickness"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Plastic " << i+1 <<  endl;
      double X = blocks[i]->GetDouble("X","mm");
      double Y = blocks[i]->GetDouble("Y","mm");
      double Z = blocks[i]->GetDouble("Z","mm");
      double R = sqrt (X*X+Y*Y+Z*Z);
      double Theta = acos(Z / (R) );
      double Phi = atan2(Y,X);

      if(blocks[i]->HasTokenList(square)){
        string Shape = blocks[i]->GetString("Shape");
        double H = blocks[i]->GetDouble("Height","mm");
        double W = blocks[i]->GetDouble("Width","mm");
        double T = blocks[i]->GetDouble("Thickness","mm");
        string Mat = blocks[i]->GetString("Scintillator");
        double Lead = blocks[i]->GetDouble("LeadThickness","mm");
        AddPlastic(R,Theta,Phi,H,W,T,Mat,Lead);
      }

      else if(blocks[i]->HasTokenList(cylind)){
        string Shape = blocks[i]->GetString("Shape");
        double Rd = blocks[i]->GetDouble("Radius","mm");
        double T = blocks[i]->GetDouble("Thickness","mm");
        string Mat = blocks[i]->GetString("Scintillator");
        double Lead = blocks[i]->GetDouble("LeadThickness","mm");
        AddPlastic(R,Theta,Phi,T,Rd,Mat,Lead);
      }

    }
    else if(blocks[i]->HasTokenList(sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Plastic " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("Theta","deg");
      double Phi = blocks[i]->GetDouble("Phi","deg");

      if(blocks[i]->HasTokenList(square)){
        string Shape = blocks[i]->GetString("Shape");
        double H = blocks[i]->GetDouble("Height","mm");
        double W = blocks[i]->GetDouble("Width","mm");
        double T = blocks[i]->GetDouble("Thickness","mm");
        string Mat = blocks[i]->GetString("Scintillator");
        double Lead = blocks[i]->GetDouble("LeadThickness","mm");
        AddPlastic(R,Theta,Phi,H,W,T,Mat,Lead);
      }

      else if(blocks[i]->HasTokenList(cylind)){
        string Shape = blocks[i]->GetString("Shape");
        double Rd = blocks[i]->GetDouble("Radius","mm");
        double T = blocks[i]->GetDouble("Thickness","mm");
        string Mat = blocks[i]->GetString("Scintillator");
        double Lead = blocks[i]->GetDouble("LeadThickness","mm");
        AddPlastic(R,Theta,Phi,T,Rd,Mat,Lead);
      }

    }


    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
}

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Plastic::ConstructDetector(G4LogicalVolume* world){
  G4ThreeVector Det_pos = G4ThreeVector(0, 0, 0)  ;

  for (unsigned short i = 0 ; i < m_R.size() ; i++) {
    G4double wX = m_R[i] * sin(m_Theta[i] ) * cos(m_Phi[i] ) ;
    G4double wY = m_R[i] * sin(m_Theta[i] ) * sin(m_Phi[i] ) ;
    G4double wZ = m_R[i] * cos(m_Theta[i] ) ;
    Det_pos = G4ThreeVector(wX, wY, wZ) ;
    VolumeMaker(Det_pos , i+1, world) ;
  }

}

void Plastic::VolumeMaker(G4ThreeVector Det_pos, int DetNumber, G4LogicalVolume* world){
  ////////////////////////////////////////////////////////////////
  ////////////// Starting Volume Definition //////////////////////
  ////////////////////////////////////////////////////////////////
  // Name of the module
  std::ostringstream DetectorNumber ;
  DetectorNumber << DetNumber ;
  G4String Name = "Plastic" + DetectorNumber.str() ;

  int i = DetNumber-1;

  G4Material* PlasticMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(m_Scintillator[i]) ;

  // Definition of the volume containing the sensitive detector

  // Cylindrical Case
  if(m_PlasticRadius[i]!=-1){
    if(m_PlasticThickness[i]>0 && m_PlasticRadius[i]>0){
      G4Tubs* solidPlastic = new G4Tubs( Name ,
          0 ,
          m_PlasticRadius[i] ,
          m_PlasticThickness[i]/2 ,
          0*deg ,
          360*deg);

      G4LogicalVolume* logicPlastic = new G4LogicalVolume(solidPlastic, PlasticMaterial, Name+ "_Scintillator", 0, 0, 0);
      logicPlastic->SetSensitiveDetector(m_PlasticScorer);

      G4VisAttributes* PlastVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 0.9)) ;
      logicPlastic->SetVisAttributes(PlastVisAtt) ;



      new G4PVPlacement(0 ,
          Det_pos ,
          logicPlastic ,
          Name  + "_Scintillator" ,
          world ,
          false ,
          0 );
    }


    if(m_LeadThickness[i]>0&& m_PlasticRadius[i]>0){
      G4Tubs* solidLead = new G4Tubs(Name+"_Lead",
          0,
          m_PlasticRadius[i],
          m_LeadThickness[i]/2,
          0*deg,
          360*deg);

      G4Material* MaterialLead = MaterialManager::getInstance()->GetMaterialFromLibrary("Al");
      G4LogicalVolume* logicLead = new G4LogicalVolume(solidLead, MaterialLead, Name+"_Al", 0, 0, 0);//AC changed lead to Al
      G4VisAttributes* LeadVisAtt = new G4VisAttributes(G4Colour(0.1, 0.1, 0.1)) ;
      logicLead->SetVisAttributes(LeadVisAtt) ;

      G4PVPlacement( 0,
          Det_pos+(m_PlasticThickness[i]/2+m_LeadThickness[i]/2)*Det_pos.unit(),
          logicLead,
          Name+"_Al",
          world,
          false,
          0);
    }
  }

  // Squared case
  if(m_PlasticHeight[i]!=-1){
    if(m_PlasticThickness[i]>0 && m_PlasticHeight[i]>0 && m_PlasticWidth[i]>0){
      G4Box* solidPlastic = new G4Box(Name, 0.5*m_PlasticWidth[i], 0.5*m_PlasticHeight[i], 0.5*m_PlasticThickness[i]);
      G4LogicalVolume* logicPlastic = new G4LogicalVolume(solidPlastic, PlasticMaterial, Name+ "_Scintillator", 0, 0, 0);
      logicPlastic->SetSensitiveDetector(m_PlasticScorer);

      G4VisAttributes* PlastVisAtt = new G4VisAttributes(G4Colour(0, 0, 1, 0.5)) ;
      logicPlastic->SetVisAttributes(PlastVisAtt) ;

      G4RotationMatrix Rot3D;
      Rot3D.set(0, 0, 0);
      new G4PVPlacement(  G4Transform3D(Rot3D,Det_pos),
          logicPlastic,
          Name  + "_Scintillator" ,
          world,
          false,
          0);
    }

    if(m_LeadThickness[i]>0&& m_PlasticHeight[i]>0 && m_PlasticWidth[i]>0){
      G4Box* solidLead = new G4Box(Name+"_Al", 1*m_PlasticWidth[i], 1*m_PlasticHeight[i], 0.5*m_LeadThickness[i]);

      G4Material* MaterialLead = MaterialManager::getInstance()->GetMaterialFromLibrary("Al");
      G4LogicalVolume* logicLead = new G4LogicalVolume(solidLead, MaterialLead, Name+"_Al", 0, 0, 0);
      G4VisAttributes* LeadVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)) ;
      logicLead->SetVisAttributes(LeadVisAtt) ;

      new G4PVPlacement(0,
          Det_pos-(m_PlasticThickness[i]/2+m_LeadThickness[i]/2)*Det_pos.unit() -G4ThreeVector(0,0,1*cm)  ,
          logicLead,
          Name+"_Al",
          world,
          false,
          0);
    }
  }
}

// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Plastic::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("Plastic")){
    pTree->Branch("Plastic", "TPlasticData", &m_Event) ;
  }
  pTree->SetBranchAddress("Plastic", &m_Event) ;
}

// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Plastic::ReadSensitive(const G4Event* event){
  G4String DetectorNumber;
  m_Event->Clear();

  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////// Used to Read Event Map of detector //////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////

  std::map<G4int, G4int*>::iterator DetectorNumber_itr;
  std::map<G4int, G4double*>::iterator Energy_itr;
  std::map<G4int, G4double*>::iterator Time_itr;
  std::map<G4int, G4double*>::iterator Pos_X_itr;
  std::map<G4int, G4double*>::iterator Pos_Y_itr;
  std::map<G4int, G4double*>::iterator Pos_Z_itr;
  std::map<G4int, G4double*>::iterator Angle_Theta_itr;

  G4THitsMap<G4int>* DetectorNumberHitMap;
  G4THitsMap<G4double>* EnergyHitMap;
  G4THitsMap<G4double>* TimeHitMap;
  G4THitsMap<G4double>* PosXHitMap;
  G4THitsMap<G4double>* PosYHitMap;
  G4THitsMap<G4double>* PosZHitMap;
  G4THitsMap<G4double>* AngleThetaHitMap;

  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////

  // Read the Scorer associate to the Silicon Strip

  //Detector Number
  G4int StripDetCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("PlasticScorer/PlasticNumber")     ;
  DetectorNumberHitMap = (G4THitsMap<G4int>*)(event->GetHCofThisEvent()->GetHC(StripDetCollectionID))          ;
  DetectorNumber_itr =  DetectorNumberHitMap->GetMap()->begin()                                               ;

  //Energy
  G4int StripEnergyCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("PlasticScorer/Energy")      ;
  EnergyHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripEnergyCollectionID))           ;
  Energy_itr = EnergyHitMap->GetMap()->begin()                                                                ;

  //Time of Flight
  G4int StripTimeCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("PlasticScorer/Time")          ;
  TimeHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripTimeCollectionID))               ;
  Time_itr = TimeHitMap->GetMap()->begin()                                                                    ;

  //Interaction Coordinate X
  G4int InterCoordXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("PlasticScorer/InterCoordX");
  PosXHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordXCollectionID));
  Pos_X_itr = PosXHitMap->GetMap()->begin();

  //Interaction Coordinate Y
  G4int InterCoordYCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("PlasticScorer/InterCoordY");
  PosYHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordYCollectionID));
  Pos_Y_itr = PosYHitMap->GetMap()->begin();

  //Interaction Coordinate Z
  G4int InterCoordZCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("PlasticScorer/InterCoordZ");
  PosZHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordZCollectionID));
  Pos_Z_itr = PosZHitMap->GetMap()->begin();

  //Interaction Coordinate Theta
  G4int InterCoordThetaCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("PlasticScorer/InterCoordTheta");
  AngleThetaHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordThetaCollectionID));
  Angle_Theta_itr = AngleThetaHitMap->GetMap()->begin();

  G4int sizeN = DetectorNumberHitMap->entries()    ;
  G4int sizeE = EnergyHitMap->entries()          ;
  G4int sizeT = TimeHitMap->entries()          ;

  // Loop on Plastic Number
  for (G4int l = 0 ; l < sizeN ; l++) {
    G4int N     =      *(DetectorNumber_itr->second)    ;
    G4int NTrackID  =   DetectorNumber_itr->first - N  ;


    if (N > 0) {
      m_Event->SetPlasticNumber(N) ;
      //  Energy
      Energy_itr = EnergyHitMap->GetMap()->begin();
      for (G4int h = 0 ; h < sizeE ; h++) {
        G4int ETrackID  =   Energy_itr->first  - N      ;
        G4double E     = *(Energy_itr->second)         ;
        if (ETrackID == NTrackID) {
          m_Event->SetEnergy(RandGauss::shoot(E, E*ResoEnergy/100./2.35))    ;
        }
        Energy_itr++;
      }


      //  Time
      Time_itr = TimeHitMap->GetMap()->begin();
      for (G4int h = 0 ; h < sizeT ; h++) {
        G4int TTrackID  =   Time_itr->first   - N    ;
        G4double T     = *(Time_itr->second)      ;
        if (TTrackID == NTrackID) {
          m_Event->SetTime(RandGauss::shoot(T, ResoTime)) ;
        }
        Time_itr++;
      }

      // Pos X
      Pos_X_itr = PosXHitMap->GetMap()->begin();
      for (G4int h = 0 ; h < PosXHitMap->entries() ; h++) {
        G4int PosXTrackID =   Pos_X_itr->first   - N  ;
        G4double PosX     = *(Pos_X_itr->second)      ;
        if (PosXTrackID == NTrackID) {
          ms_InterCoord->SetDetectedPositionX(PosX) ;
        }
        Pos_X_itr++;
      }

      // Pos Y
      Pos_Y_itr = PosYHitMap->GetMap()->begin();
      for (G4int h = 0 ; h < PosYHitMap->entries() ; h++) {
        G4int PosYTrackID =   Pos_Y_itr->first   - N  ;
        G4double PosY     = *(Pos_Y_itr->second)      ;
        if (PosYTrackID == NTrackID) {
          ms_InterCoord->SetDetectedPositionY(PosY) ;
        }
        Pos_Y_itr++;
      }

      // Pos Z
      Pos_Z_itr = PosZHitMap->GetMap()->begin();
      for (G4int h = 0 ; h < PosZHitMap->entries() ; h++) {
        G4int PosZTrackID =   Pos_Z_itr->first   - N  ;
        G4double PosZ     = *(Pos_Z_itr->second)      ;
        if (PosZTrackID == NTrackID) {
          ms_InterCoord->SetDetectedPositionZ(PosZ) ;
        }
        Pos_Z_itr++;
      }

      // Angle Theta
      Angle_Theta_itr = AngleThetaHitMap->GetMap()->begin();
      for (G4int h = 0 ; h < AngleThetaHitMap->entries() ; h++) {
        G4int AngleThetaTrackID =   Angle_Theta_itr->first   - N  ;
        G4double Theta     = *(Angle_Theta_itr->second)      ;
        if (AngleThetaTrackID == NTrackID) {
          ms_InterCoord->SetDetectedAngleTheta(Theta) ;
        }
        Angle_Theta_itr++;
      }

    }

    DetectorNumber_itr++;
  }

  // clear map for next event
  TimeHitMap->clear()   ;
  DetectorNumberHitMap->clear()   ;
  EnergyHitMap->clear()    ;
  PosXHitMap->clear() ;
  PosYHitMap->clear() ;
  PosZHitMap->clear() ;
  AngleThetaHitMap->clear();

}


////////////////////////////////////////////////////////////////   
void Plastic::InitializeScorers() { 
  bool already_exist = false; 
  m_PlasticScorer = CheckScorer("PlasticScorer",already_exist) ;

  if(already_exist) return ;

  G4VPrimitiveScorer* DetNbr = new PSDetectorNumber("PlasticNumber","Plastic", 0) ;
  G4VPrimitiveScorer* Energy = new PSEnergy("Energy","Plastic", 0)                   ;
  G4VPrimitiveScorer* Time   = new PSTOF("Time","Plastic", 0)                         ;
  G4VPrimitiveScorer* InteractionCoordinatesX  			= new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesX("InterCoordX","Plastic", 0);
  G4VPrimitiveScorer* InteractionCoordinatesY  			= new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesY("InterCoordY","Plastic", 0);
  G4VPrimitiveScorer* InteractionCoordinatesZ  			= new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesZ("InterCoordZ","Plastic", 0);
  G4VPrimitiveScorer* InteractionCoordinatesTheta  		= new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesAngleTheta("InterCoordTheta","Plastic", 0);
  //and register it to the multifunctionnal detector
  m_PlasticScorer->RegisterPrimitive(DetNbr)                         ;
  m_PlasticScorer->RegisterPrimitive(Energy)                         ;
  m_PlasticScorer->RegisterPrimitive(Time)                            ;
  m_PlasticScorer->RegisterPrimitive(InteractionCoordinatesX);
  m_PlasticScorer->RegisterPrimitive(InteractionCoordinatesY);
  m_PlasticScorer->RegisterPrimitive(InteractionCoordinatesZ);
  m_PlasticScorer->RegisterPrimitive(InteractionCoordinatesTheta);
  G4SDManager::GetSDMpointer()->AddNewDetector(m_PlasticScorer) ;

}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* Plastic::Construct(){
  return  (NPS::VDetector*) new Plastic();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
  class proxy_nps_plastic{
    public:
      proxy_nps_plastic(){
        NPS::DetectorFactory::getInstance()->AddToken("Plastic","Plastic");
        NPS::DetectorFactory::getInstance()->AddDetector("Plastic",Plastic::Construct);
      }
  };

  proxy_nps_plastic p_nps_plastic;
}
