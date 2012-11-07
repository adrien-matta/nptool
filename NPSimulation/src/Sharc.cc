/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
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
 *  This class describe the SHarc Silicon array                              *
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
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PVDivision.hh"
#include "G4SubtractionSolid.hh"

// NPTool header
#include "Sharc.hh"
#include "GeneralScorers.hh"
//#include "SharcScorers.hh"
#include "RootOutput.h"
using namespace SHARC;

// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Sharc Specific Method
Sharc::Sharc()
{
  InitializeMaterial();
  // m_Event = new TSSSDData();
}

Sharc::~Sharc()
{
  delete m_MaterialSilicon;
  delete m_MaterialAl;
  delete m_MaterialVacuum;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Sharc::AddBoxDetector(G4ThreeVector Pos)
{
  m_Type.push_back(true);
  m_Pos.push_back(Pos);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Sharc::AddQQQDetector(G4ThreeVector Pos)
{
  m_Type.push_back(false);
  m_Pos.push_back(Pos);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Sharc::VolumeMaker(G4int             DetNumber ,
                        G4ThreeVector     Det_pos ,
                        G4RotationMatrix* Det_rot ,
                        G4LogicalVolume*  world   )
{
  // create the DSSD
  G4Box*  PCBFull = new G4Box("PCBFull"  ,90*mm,60*mm,1*mm);
  G4Box*  Wafer = new G4Box("Wafer",72*mm,48*mm,100*um);
  
  G4SubtractionSolid* PCB = new G4SubtractionSolid("PCB", PCBFull, Wafer);
  
  G4LogicalVolume* logicPCB =
  new G4LogicalVolume(PCB, m_MaterialSilicon, "logicPCB", 0, 0, 0);
  
  new G4PVPlacement(0, G4ThreeVector(0,0,0), logicPCB, "TEST", world, false, 0);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of VDetector class


// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Sharc::ReadConfiguration(string Path)
{
  ifstream ConfigFile           ;
  ConfigFile.open(Path.c_str()) ;
  string LineBuffer             ;
  string DataBuffer             ;
  
  G4double R,Phi,X,Y,Z;
  G4ThreeVector Pos;
  bool check_R   = false ;
  bool check_Phi = false ;
  bool check_X   = false ;
  bool check_Y   = false ;
  bool check_Z   = false ;
  
  bool ReadingStatusQQQ = false ;
  bool ReadingStatusBOX = false ;
  bool ReadingStatus    = false ;
  while (!ConfigFile.eof()){
    
    getline(ConfigFile, LineBuffer);
    // cout << LineBuffer << endl;
    if (LineBuffer.compare(0, 5, "Sharc") == 0)
      ReadingStatus = true;
    
    while (ReadingStatus && !ConfigFile.eof()) {
      ConfigFile >> DataBuffer ;
      //   Comment Line
      if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
      
      //   CD case
      if (DataBuffer=="SharcQQQ"){
        G4cout << "///" << G4endl           ;
        G4cout << "QQQ Quadrant found: " << G4endl   ;
        ReadingStatusQQQ = true ;
      }
      
      //  Box case
      else if (DataBuffer=="SharcBOX"){
        G4cout << "///" << G4endl           ;
        G4cout << "Box Detector found: " << G4endl   ;
        ReadingStatusBOX = true ;
      }
      
      //   Reading Block
      while(ReadingStatusQQQ){
        // Pickup Next Word
        ConfigFile >> DataBuffer ;
        
        //   Comment Line
        if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
        
        //Position method
        else if (DataBuffer == "Z=") {
          check_Z = true;
          ConfigFile >> DataBuffer ;
          Z= atof(DataBuffer.c_str())*mm;
          cout << "  Z= " << Z/mm << "mm" << endl;
        }
        
        else if (DataBuffer == "R=") {
          check_R = true;
          ConfigFile >> DataBuffer ;
          R= atof(DataBuffer.c_str())*mm;
          cout << "  R= " << R/mm << "mm" << endl;
        }
        
        else if (DataBuffer == "Phi=") {
          check_Phi = true;
          ConfigFile >> DataBuffer ;
          Phi= atof(DataBuffer.c_str())*deg;
          cout << "  Phi= " << Phi/deg << "deg" << endl;
        }
        
        ///////////////////////////////////////////////////
        //   If no Detector Token and no comment, toggle out
        else{
          ReadingStatusQQQ = false;
          G4cout << "Error: Wrong Token Sequence: Getting out " << DataBuffer << G4endl ;
          exit(1);
        }
        
        /////////////////////////////////////////////////
        //   If All necessary information there, toggle out
        
        if (check_R && check_Phi && check_Z){
          
          ReadingStatusQQQ = false;
          AddQQQDetector(G4ThreeVector(Z,R,Phi));
          //   Reinitialisation of Check Boolean
          check_R   = false ;
          check_Phi = false ;
          check_X   = false;
          check_Y   = false;
          check_Z   = false ;
          
        }
        
      }
      
      while(ReadingStatusBOX){
        // Pickup Next Word
        ConfigFile >> DataBuffer ;
        
        //   Comment Line
        if (DataBuffer.compare(0, 1, "%") == 0) {   ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );}
        
        //Position method
        else if (DataBuffer == "X=") {
          check_X = true;
          ConfigFile >> DataBuffer ;
          X= atof(DataBuffer.c_str())*mm;
          cout << "  X= " << Y/mm << "mm" << endl;
        }
        
        else if (DataBuffer == "Y=") {
          check_Y = true;
          ConfigFile >> DataBuffer ;
          Y= atof(DataBuffer.c_str())*mm;
          cout << "  Y= " << Y/mm << "mm" << endl;
        }
        
        else if (DataBuffer == "Z=") {
          check_Z = true;
          ConfigFile >> DataBuffer ;
          Z= atof(DataBuffer.c_str())*mm;
          cout << "  Z= " << Z/mm << "mm" << endl;
        }
        
        ///////////////////////////////////////////////////
        //   If no Detector Token and no comment, toggle out
        else{
          ReadingStatusBOX = false;
          G4cout << "Error: Wrong Token Sequence: Getting out " << DataBuffer << G4endl ;
          exit(1);
        }
        
        /////////////////////////////////////////////////
        //   If All necessary information there, toggle out
        
        if (check_X && check_Y && check_Z){
          ReadingStatusBOX = false;
          AddBoxDetector(G4ThreeVector(X,Y,Z));
          //   Reinitialisation of Check Boolean
          check_R   = false ;
          check_Phi = false ;
          check_X   = false;
          check_Y   = false;
          check_Z   = false ;
          
        }

      }
    }
  }
}

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Sharc::ConstructDetector(G4LogicalVolume* world)
{
  /*  G4RotationMatrix* Det_rot  = NULL;
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
   
   delete Det_rot ;*/
  
  // create the DSSD
  G4Box*  PCBFull = new G4Box("PCBFull"  ,90*mm,60*mm,1*mm);
  G4Box*  Wafer = new G4Box("Wafer",72*mm,48*mm,1.01*mm);
  G4ThreeVector zTrans(0, 0, 50);
  G4SubtractionSolid* PCB = new G4SubtractionSolid("PCB", PCBFull, Wafer,new G4RotationMatrix,G4ThreeVector(10*mm, 10*mm,0 ));
  
  G4LogicalVolume* logicPCB =
  new G4LogicalVolume(PCB, m_MaterialSilicon, "logicPCB", 0, 0, 0);
  
  new G4PVPlacement(new G4RotationMatrix(0,90*deg,0), G4ThreeVector(0,30*mm,-70), logicPCB, "TEST", world, false, 0);
}

// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Sharc::InitializeRootOutput()
{
  /* RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
   pTree->Branch("SSSD", "TSSSDData", &m_Event) ;*/
}

// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Sharc::ReadSensitive(const G4Event* event)
{
  /* m_Event->Clear();
   //////////////////////////////////////////////////////////////////////////////////////
   //////////////////////// Used to Read Event Map of detector //////////////////////////
   //////////////////////////////////////////////////////////////////////////////////////
   
   // Si
   G4THitsMap<G4int>*     DetNbrHitMap;
   G4THitsMap<G4int>*     StripNbrHitMap;
   G4THitsMap<G4double>*  EnergyHitMap;
   G4THitsMap<G4double>*  TimeHitMap;
   
   std::map<G4int, G4int*>::iterator DetNbr_itr     ;
   std::map<G4int, G4int*>::iterator StripNbr_itr  ;
   std::map<G4int, G4double*>::iterator Energy_itr ;
   std::map<G4int, G4double*>::iterator Time_itr   ;
   //////////////////////////////////////////////////////////////////////////////////////
   //////////////////////////////////////////////////////////////////////////////////////
   // Read the Scorer associate to the Silicon Strip
   
   //DetectorNumber
   G4int DetNbrCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Sharc_StripScorer/DetectorNumber");
   DetNbrHitMap = (G4THitsMap<G4int>*)(event->GetHCofThisEvent()->GetHC(DetNbrCollectionID));
   DetNbr_itr = DetNbrHitMap->GetMap()->begin();
   
   //StripNumber
   G4int StripNbrCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Sharc_StripScorer/StripNumber");
   StripNbrHitMap = (G4THitsMap<G4int>*)(event->GetHCofThisEvent()->GetHC(StripNbrCollectionID));
   
   //Energy
   G4int StripEnergyCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Sharc_StripScorer/StripEnergy");
   EnergyHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripEnergyCollectionID));
   
   //Time
   G4int StripTimeCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Sharc_StripScorer/StripTime");
   TimeHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripTimeCollectionID));
   
   G4int sizeN = DetNbrHitMap      ->entries();
   G4int sizeS = StripNbrHitMap    ->entries();
   G4int sizeE = EnergyHitMap       ->entries();
   G4int sizeT = TimeHitMap         ->entries();
   
   // Loop on Det Number
   for (G4int l = 0 ; l < sizeN ; l++)
   {
   G4int N     =      *(DetNbr_itr->second);
   G4int NTrackID  =   DetNbr_itr->first - N;
   
   if (N > 0)
   {
   m_Event->SetEnergyDetectorNbr(N);
   m_Event->SetTimeDetectorNbr(N);
   
   //  Strip Number
   StripNbr_itr = StripNbrHitMap->GetMap()->begin();
   for (G4int h = 0 ; h < sizeS ; h++) {
   G4int STrackID  =   StripNbr_itr->first  - N ;
   G4int S         = *(StripNbr_itr->second);
   
   if (STrackID == NTrackID) {
   m_Event->SetEnergyStripNbr(S);
   m_Event->SetTimeStripNbr(S);
   }
   
   StripNbr_itr++;
   }
   
   //  Energy
   Energy_itr = EnergyHitMap->GetMap()->begin();
   for (G4int h = 0 ; h < sizeE ; h++) {
   G4int ETrackID  =   Energy_itr->first  - N;
   G4double E      = *(Energy_itr->second);
   
   if (ETrackID == NTrackID) {
   m_Event->SetEnergy( RandGauss::shoot(E, ResoEnergy ) );
   }
   
   Energy_itr++;
   }
   
   
   //  Time
   Time_itr = TimeHitMap->GetMap()->begin();
   for (G4int h = 0 ; h < sizeT ; h++) {
   G4int TTrackID  =   Time_itr->first   - N ;
   G4double T     = *(Time_itr->second);
   
   if (TTrackID == NTrackID) {
   m_Event->SetTime( RandGauss::shoot(T, ResoTime ) );
   }
   
   Time_itr++;
   }
   
   }
   
   DetNbr_itr++;
   }
   
   // clear map for next event
   
   DetNbrHitMap   ->clear();
   StripNbrHitMap ->clear();
   EnergyHitMap   ->clear();
   TimeHitMap     ->clear();*/
}


void Sharc::InitializeScorers()
{
  /*
   //   Silicon Associate Scorer
   m_StripScorer = new G4MultiFunctionalDetector("Sharc_StripScorer");
   
   G4VPrimitiveScorer* DetNbr   = new GENERALSCORERS::PSDetectorNumber("DetectorNumber","Sharc_", 0);
   G4VPrimitiveScorer* StripNbr = new PSStripNumber("StripNumber",0,SiliconSize, NumberOfStrip);
   G4VPrimitiveScorer* Energy   = new GENERALSCORERS::PSEnergy("StripEnergy","Sharc_", 0);
   G4VPrimitiveScorer* TOF      = new GENERALSCORERS::PSTOF("StripTime","Sharc_", 0);
   
   
   //and register it to the multifunctionnal detector
   m_StripScorer->RegisterPrimitive(DetNbr);
   m_StripScorer->RegisterPrimitive(StripNbr);
   m_StripScorer->RegisterPrimitive(Energy);
   m_StripScorer->RegisterPrimitive(TOF);
   
   //   Add All Scorer to the Global Scorer Manager
   G4SDManager::GetSDMpointer()->AddNewDetector(m_StripScorer) ;*/
}

////////////////////////////////////////////////////////////////
/////////////////Material Definition ///////////////////////////
////////////////////////////////////////////////////////////////
void Sharc::InitializeMaterial()
{
  
  G4Element* N   = new G4Element("Nitrogen" , "N"  , 7  , 14.01  * g / mole);
  G4Element* O   = new G4Element("Oxigen"   , "O"  , 8  , 16.00  * g / mole);
  
  G4double a, z, density;
  // Si
  a = 28.0855 * g / mole;
  density = 2.321 * g / cm3;
  m_MaterialSilicon = new G4Material("Si", z = 14., a, density);
  
  // Al
  density = 2.702 * g / cm3;
  a = 26.98 * g / mole;
  m_MaterialAl = new G4Material("Al", z = 13., a, density);
  
  //  Vacuum
  density = 0.000000001 * mg / cm3;
  m_MaterialVacuum = new G4Material("Vacuum", density, 2);
  m_MaterialVacuum->AddElement(N, .7);
  m_MaterialVacuum->AddElement(O, .3);
}


