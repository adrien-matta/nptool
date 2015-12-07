/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : Avril 2015                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe a Modular cylindrical CsI Scintillator           *
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
#include "G4Trd.hh"

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
#include "CsI.hh"
#include "ObsoleteGeneralScorers.hh"
#include "RootOutput.h"
#include "MaterialManager.hh"
#include "SiliconScorers.hh"
#include "CalorimeterScorers.hh"
#include "NPSDetectorFactory.hh"
//using namespace OBSOLETEGENERALSCORERS ;
// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// CsI Specific Method
CsI::CsI(){
  m_Event = new TCsIData() ;
  m_CsIScorer = 0;
   ResoCsI = 2.4;//%
    PhotoDiodeFace = 18.;//mm
    PhotoDiodeThickness = 3.;//mm
 


}

CsI::~CsI(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void CsI::AddCsI(   G4double  R                       ,
    G4double  Theta                   ,
    G4double  Phi                  ,
    G4double   CsIThickness   ,
    G4double   CsIRadius         ,
    G4String    Scintillator         ,
    G4double    LeadThickness         )
{

  m_R.push_back(R)                                         ;
  m_Theta.push_back(Theta)                                ;
  m_Phi.push_back(Phi)                                     ;
  m_CsIThickness.push_back(CsIThickness)   ;
  m_LeadThickness.push_back(LeadThickness)            ;
  m_Scintillator.push_back(Scintillator)               ;
  m_CsIRadius.push_back(CsIRadius)            ; // cylindrical shape
  m_CsIFaceFront.push_back(-1)                              ; // Trapezoidal shape
  m_CsIFaceBack.push_back(-1)                              ; // Trapezoidal shape
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void CsI::AddCsI(   G4double R                      ,
    G4double Theta                ,
    G4double Phi                     ,
    G4double FaceFront                  ,
    G4double FaceBack                  ,
    G4double CsIThickness   ,
    G4String Scintillator         ,
    G4double LeadThickness      )
{
  m_R.push_back(R)                                         ;
  m_Theta.push_back(Theta)                                ;
  m_Phi.push_back(Phi)                                     ;
  m_CsIThickness.push_back(CsIThickness)   ;
  m_LeadThickness.push_back(LeadThickness)            ;
  m_Scintillator.push_back(Scintillator)               ;
  m_CsIRadius.push_back(-1)            ; // cylindrical shape
  m_CsIFaceFront.push_back(FaceFront)                        ; // Trapezoidal shape
  m_CsIFaceBack.push_back(FaceBack)                           ; // Trapezoidal shape

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of VDetector class


// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void CsI::ReadConfiguration(string Path){
  ifstream ConfigFile           ;
  ConfigFile.open(Path.c_str()) ;
  string LineBuffer          ;
  string DataBuffer          ;

  G4double Theta = 0 , Phi = 0 , R = 0 , Thickness = 0 , Radius = 0 ;
  G4double  LeadThickness = 0, X = 0 , Y = 0 , Z = 0 , FaceFront = 0 , FaceBack = 0 ;
  G4String Scintillator, Shape ;

  bool check_Theta = false ;
  bool check_Phi = false ;
  bool check_R = false ;
  bool check_Thickness = false ;
  bool check_Radius = false ;
  bool check_LeadThickness = false ;
  bool check_Scintillator = false ;
  bool check_FaceFront = false ;
  bool check_FaceBack = false ;
  bool check_Shape = false ;
  bool check_X = false ;
  bool check_Y = false ;
  bool check_Z = false ;      
  bool ReadingStatus = false ;


  while (!ConfigFile.eof()) {
    getline(ConfigFile, LineBuffer);

    //   If line is a Start Up CsI bloc, Reading toggle to true      
    if (LineBuffer.compare(0, 3, "CsI") == 0) {
      G4cout << "///" << G4endl           ;
      G4cout << "CsI found: " << G4endl   ;
      ReadingStatus = true ;

    }

    //   Else don't toggle to Reading Block Status
    else ReadingStatus = false ;

    //   Reading Block
    while(ReadingStatus){
      // Pickup Next Word 
      ConfigFile >> DataBuffer ;

      //   Comment Line 
      if (DataBuffer.compare(0, 1, "%") == 0) {   
        ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
      }

      //   Finding another telescope (safety), toggle out
      else if (DataBuffer.compare(0, 3, "CsI") == 0) {
        G4cout << "WARNING: Another Telescope is find before standard sequence of Token, Error may occured in Telecope definition" << G4endl ;
        ReadingStatus = false ;
      }

      //Angle method
      else if (DataBuffer.compare(0, 6, "THETA=") == 0) {
        check_Theta = true;
        ConfigFile >> DataBuffer ;
        Theta = atof(DataBuffer.c_str()) ;
        Theta = Theta * deg;
        G4cout << "Theta:  " << Theta / deg << G4endl;
      }

      else if (DataBuffer.compare(0, 4, "PHI=") == 0) {
        check_Phi = true;
        ConfigFile >> DataBuffer ;
        Phi = atof(DataBuffer.c_str()) ;
        Phi = Phi * deg;
        G4cout << "Phi:  " << Phi / deg << G4endl;
      }

      else if (DataBuffer.compare(0, 2, "R=") == 0) {
        check_R = true;
        ConfigFile >> DataBuffer ;
        R = atof(DataBuffer.c_str()) ;
        R = R * mm;
        G4cout << "R:  " << R/mm << G4endl;
      }

      //Position method
      else if (DataBuffer.compare(0, 2, "X=") == 0) {
        check_X = true;
        ConfigFile >> DataBuffer ;
        X = atof(DataBuffer.c_str()) ;
        X = X * mm;
        G4cout << "X:  " << X / mm << G4endl;
      }

      else if (DataBuffer.compare(0, 2, "Y=") == 0) {
        check_Y = true;
        ConfigFile >> DataBuffer ;
        Y = atof(DataBuffer.c_str()) ;
        Y = Y * mm;
        G4cout << "Y:  " << Y / mm << G4endl;
      }

      else if (DataBuffer.compare(0, 2, "Z=") == 0) {
        check_Z = true;
        ConfigFile >> DataBuffer ;
        Z = atof(DataBuffer.c_str()) ;
        Z = Z * mm;
        G4cout << "Z:  " << Z / mm << G4endl;
      }


      //General
      else if (DataBuffer.compare(0, 6, "Shape=") == 0) {
        check_Shape = true;
        ConfigFile >> DataBuffer ;
        Shape = DataBuffer ;
        G4cout << "Shape:  " << Shape << G4endl;
      }

      // Cylindrical shape
      else if (DataBuffer.compare(0, 7, "Radius=") == 0) {
        check_Radius = true;
        ConfigFile >> DataBuffer ;
        Radius = atof(DataBuffer.c_str()) ;
        Radius = Radius * mm;
        G4cout << "CsI Radius:  " << Radius/mm << G4endl;
      }

      // Trapezoidal shape
      else if (DataBuffer.compare(0, 10, "FaceFront=") == 0) {
        check_FaceFront = true;
        ConfigFile >> DataBuffer ;
        FaceFront = atof(DataBuffer.c_str()) ;
        FaceFront = FaceFront * mm;
        G4cout << "CsI FaceFront:  " << FaceFront/mm << G4endl;
      }

      else if (DataBuffer.compare(0, 9, "FaceBack=") == 0) {
        check_FaceBack = true;
        ConfigFile >> DataBuffer ;
        FaceBack = atof(DataBuffer.c_str()) ;
        FaceBack = FaceBack * mm;
        G4cout << "CsI FaceBack:  " << FaceBack/mm << G4endl;
      }

      // Common
      else if (DataBuffer.compare(0, 10, "Thickness=") == 0) {
        check_Thickness = true;
        ConfigFile >> DataBuffer ;
        Thickness = atof(DataBuffer.c_str()) ;
        Thickness = Thickness * mm;
        G4cout << "CsI Thickness:  " << Thickness/mm << G4endl;
      }

      else if (DataBuffer.compare(0, 13, "Scintillator=") == 0) {
        check_Scintillator = true ;
        ConfigFile >> DataBuffer ;
        Scintillator = DataBuffer ;
        G4cout << "CsI Scintillator type:  " << Scintillator << G4endl;
      }

      else if (DataBuffer.compare(0, 14, "LeadThickness=") == 0) {
        check_LeadThickness = true;
        ConfigFile >> DataBuffer ;
        LeadThickness = atof(DataBuffer.c_str()) ;
        LeadThickness = LeadThickness * mm;
        G4cout << "Lead Thickness :  " << LeadThickness/mm << G4endl;
      }

      ///////////////////////////////////////////////////
      //   If no Detector Token and no comment, toggle out
      else{
        ReadingStatus = false; 
        G4cout << "Wrong Token Sequence: Getting out " << DataBuffer << G4endl ;
      }

      /////////////////////////////////////////////////
      //   If All necessary information there, toggle out

      if (( check_Theta && check_Phi && check_R && check_Thickness 
            && check_Radius && check_LeadThickness && check_Scintillator 
            && check_Shape) // Cylindrical case
          ||    
          ( check_X && check_Y && check_Z && check_Thickness && check_Radius 
            && check_LeadThickness && check_Scintillator )
          ||   
          ( check_Theta && check_Phi && check_R && check_Thickness 
            && check_FaceBack && check_FaceFront && check_LeadThickness 
            && check_Scintillator && check_Shape ) // Trapezoidal case
          ||   
          ( check_X && check_Y && check_Z && check_Thickness && check_FaceBack 
            && check_FaceFront && check_LeadThickness && check_Scintillator )) { 

        if (check_X && check_Y && check_Z){
          R = sqrt (X*X+Y*Y+Z*Z);
          Theta = acos(Z / (R) );
          Phi = atan2(Y,X);
        }

        if (Shape == "Cylinder")
          AddCsI(   R                ,
              Theta             ,
              Phi               ,
              Thickness         ,
              Radius            ,
              Scintillator   ,
              LeadThickness   );

        else if (Shape == "Trapezoidal")
          AddCsI(   R                ,
              Theta             ,
              Phi               ,
              FaceFront            ,
              FaceBack               ,
              Thickness         ,
              Scintillator   ,
              LeadThickness   );

        //   Reinitialisation of Check Boolean 

        check_Theta = false ;
        check_Phi = false ;
        check_R = false ;
        check_Thickness = false ;
        check_Radius = false ;
        check_LeadThickness = false ;
        check_Scintillator = false ;
        check_FaceFront = false ;
        check_FaceBack = false ;
        check_Shape = false ;
        check_X = false ;
        check_Y = false ;
        check_Z = false ;
        ReadingStatus = false ;   
        G4cout << "///"<< G4endl ;            
      }
    }
  }
}

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void CsI::ConstructDetector(G4LogicalVolume* world){
  G4ThreeVector Det_pos = G4ThreeVector(0, 0, 0)  ;

  for (unsigned short i = 0 ; i < m_R.size() ; i++) {
    G4double wX = m_R[i] * sin(m_Theta[i] ) * cos(m_Phi[i] ) ;
    G4double wY = m_R[i] * sin(m_Theta[i] ) * sin(m_Phi[i] ) ;
    G4double wZ = m_R[i] * cos(m_Theta[i] ) ;
    Det_pos = G4ThreeVector(wX, wY, wZ) ;
    VolumeMaker(Det_pos , i+1, world) ;
  }

}

void CsI::VolumeMaker(G4ThreeVector Det_pos, int DetNumber, G4LogicalVolume* world){
  ////////////////////////////////////////////////////////////////
  ////////////// Starting Volume Definition //////////////////////
  ////////////////////////////////////////////////////////////////      
  // Name of the module
  std::ostringstream DetectorNumber ;
  DetectorNumber << DetNumber ;
  G4String Name = "CsI" + DetectorNumber.str() ;

  int i = DetNumber-1;

  G4Material* CsIMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(m_Scintillator[i]) ;

  // Definition of the volume containing the sensitive detector

  // Cylindrical Case
  if(m_CsIRadius[i]!=-1){
    if(m_CsIThickness[i]>0 && m_CsIRadius[i]>0){
        
        // CsI crystal
        G4Tubs* solidCsI = new G4Tubs( Name ,
                                      0 ,
                                      m_CsIRadius[i] ,
                                      m_CsIThickness[i]/2 ,
                                      0*deg ,
                                      360*deg);

        G4LogicalVolume* logicCsI = new G4LogicalVolume(solidCsI, CsIMaterial, Name+ "_Scintillator", 0, 0, 0);
        logicCsI->SetSensitiveDetector(m_CsIScorer);

        G4VisAttributes* CsIVisAtt = new G4VisAttributes(G4Colour(1.0, 0.5, 0.0,0.25)) ;
        logicCsI->SetVisAttributes(CsIVisAtt) ;

        new G4PVPlacement(0 ,
                          Det_pos ,
                          logicCsI ,
                          Name  + "_Scintillator" ,
                          world ,
                          false ,
                          0 );
        
        // Photodiode
        G4String NamePD = Name+"PhotoDiode";
        
        G4Material* PDMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary("Si");

        G4Box* solidPhotoDiode = new G4Box(NamePD,0.5*PhotoDiodeFace,0.5*PhotoDiodeFace,0.5*PhotoDiodeThickness);
        
        G4LogicalVolume* logicPD = new G4LogicalVolume(solidPhotoDiode, PDMaterial, NamePD,0,0,0);
        logicPD->SetSensitiveDetector(m_PDScorer);
        
        G4VisAttributes* PDVisAtt = new G4VisAttributes(G4Colour(0.1, 0.2, 0.3)) ;
        logicPD->SetVisAttributes(PDVisAtt);
        
        new G4PVPlacement(0 ,
                         Det_pos+(m_CsIThickness[i]*0.5+PhotoDiodeThickness*0.5)*Det_pos.unit() ,
                         logicPD ,
                         NamePD ,
                         world ,
                         false ,
                         0 );
    }

    if(m_LeadThickness[i]>0&& m_CsIRadius[i]>0){
      G4Tubs* solidLead = new G4Tubs(Name+"_Lead",    
          0,
          m_CsIRadius[i],
          m_LeadThickness[i]/2,
          0*deg, 
          360*deg);

      G4Material* MaterialLead = MaterialManager::getInstance()->GetMaterialFromLibrary("Pb");
      G4LogicalVolume* logicLead = new G4LogicalVolume(solidLead, MaterialLead, Name+"_Lead", 0, 0, 0);//AC changed lead to Al
      G4VisAttributes* LeadVisAtt = new G4VisAttributes(G4Colour(0.1, 0.1, 0.1)) ;
      logicLead->SetVisAttributes(LeadVisAtt) ;

      G4PVPlacement( 0,
          Det_pos+(m_CsIThickness[i]/2+m_LeadThickness[i]/2)*Det_pos.unit(),
          logicLead,
          Name+"_Lead",   
          world,
          false,
          0);
    }   
  }

  // Trapezoidal case
  if(m_CsIFaceFront[i]!=-1){
    if(m_CsIThickness[i]>0 && m_CsIFaceFront[i]>0 && m_CsIFaceBack[i]>0){ 
    
	G4Trd* solidCsI = new G4Trd("solidCsICrystal",0.5*m_CsIFaceFront[i],0.5*m_CsIFaceBack[i],0.5*m_CsIFaceFront[i],0.5*m_CsIFaceBack[i],0.5*m_CsIThickness[i]);
	
      //G4Box* solidCsI = new G4Box(Name, 0.5*m_CsIFaceBack[i], 0.5*m_CsIFaceFront[i], 0.5*m_CsIThickness[i]);
      G4LogicalVolume* logicCsI = new G4LogicalVolume(solidCsI, CsIMaterial, Name+ "_Scintillator", 0, 0, 0);
      logicCsI->SetSensitiveDetector(m_CsIScorer);

      G4VisAttributes* PlastVisAtt = new G4VisAttributes(G4Colour(1.0, 0.5, 0.0)) ;
      logicCsI->SetVisAttributes(PlastVisAtt) ;

      G4RotationMatrix Rot3D;
      Rot3D.set(0, 0, 0);
      new G4PVPlacement(  G4Transform3D(Rot3D,Det_pos),
          logicCsI,
          Name  + "_Scintillator" ,
          world,
          false,
          0);   
    }

    if(m_LeadThickness[i]>0&& m_CsIFaceFront[i]>0 && m_CsIFaceBack[i]>0){
      G4Box* solidLead = new G4Box(Name+"_Lead", 0.5*m_CsIFaceBack[i], 0.5*m_CsIFaceFront[i], 0.5*m_LeadThickness[i]);

      G4Material* MaterialLead = MaterialManager::getInstance()->GetMaterialFromLibrary("Pb");
      G4LogicalVolume* logicLead = new G4LogicalVolume(solidLead, MaterialLead, Name+"_Lead", 0, 0, 0);
      G4VisAttributes* LeadVisAtt = new G4VisAttributes(G4Colour(0.1, 0.1, 0.1)) ;
      logicLead->SetVisAttributes(LeadVisAtt) ;

      new G4PVPlacement(0,
          Det_pos+(m_CsIThickness[i]/2+m_LeadThickness[i]/2)*Det_pos.unit() +G4ThreeVector(0,0,-10*cm)  ,
          logicLead,
          Name+"_Lead",   
          world,
          false,
          0);
    }   
  }
}

// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void CsI::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  pTree->Branch("CsI", "TCsIData", &m_Event) ;
  pTree->SetBranchAddress("CsI", &m_Event) ;
}

// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void CsI::ReadSensitive(const G4Event* event){
    //G4String DetectorNumber;
    m_Event->Clear();
    
    // CsI //
    G4THitsMap<G4double*>* CsIHitMap;
    std::map<G4int, G4double**>::iterator CsI_itr;
    G4int CsICollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("CsIScorer/CsI");
    CsIHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(CsICollectionID));
    
    // Loop on the CsI map
    for (CsI_itr = CsIHitMap->GetMap()->begin() ; CsI_itr != CsIHitMap->GetMap()->end() ; CsI_itr++){
        G4double* Info = *(CsI_itr->second);
        double E_CsI = RandGauss::shoot(Info[0],Info[0]*ResoCsI/100);
        //cout << "Energy CsI " << endl;
        //cout << E_CsI << endl;
        m_Event->SetCsIEEnergy(E_CsI);
        m_Event->SetENumber(Info[2]);
    }
    // Clear Map for next event
    CsIHitMap->clear();
    
    // PhotoDiode //
    G4THitsMap<G4double*>* PhotoDiodeHitMap;
    std::map<G4int, G4double**>::iterator PhotoDiode_itr;
    
    G4int PhotoDiodeCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("PDScorer/PhotoDiode");
    PhotoDiodeHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(PhotoDiodeCollectionID));
    
    // Loop on the PhotoDiode map
    for (PhotoDiode_itr = PhotoDiodeHitMap->GetMap()->begin() ; PhotoDiode_itr != PhotoDiodeHitMap->GetMap()->end() ; PhotoDiode_itr++){
        G4double* Info = *(PhotoDiode_itr->second);
        double E_PhotoDiode = RandGauss::shoot(Info[0],Info[0]*ResoCsI/100);
        
        m_Event->SetPhotoDiodeEnergy(E_PhotoDiode);
        m_Event->SetPhotoDiodeEDetectorNbr(Info[7]);
            
        m_Event->SetPhotoDiodeTime(Info[1]);
        m_Event->SetPhotoDiodeTDetectorNbr(Info[7]);
    }
    PhotoDiodeHitMap->clear();

    /*
  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////// Used to Read Event Map of detector //////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////

  std::map<G4int, G4int*>::iterator DetectorNumber_itr;
  std::map<G4int, G4double*>::iterator Energy_itr;
  std::map<G4int, G4double*>::iterator Time_itr;
  std::map<G4int, G4double*>::iterator Pos_Z_itr;

  G4THitsMap<G4int>* DetectorNumberHitMap;      
  G4THitsMap<G4double>* EnergyHitMap;
  G4THitsMap<G4double>* TimeHitMap;
  G4THitsMap<G4double>* PosZHitMap;

  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////

  // Read the Scorer associate to the Silicon Strip

  //Detector Number
  G4int StripDetCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("CsIScorer/CsINumber")     ;
  DetectorNumberHitMap = (G4THitsMap<G4int>*)(event->GetHCofThisEvent()->GetHC(StripDetCollectionID))          ;
  DetectorNumber_itr =  DetectorNumberHitMap->GetMap()->begin()                                               ;

  //Energy
  G4int StripEnergyCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("CsIScorer/Energy")      ;
  EnergyHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripEnergyCollectionID))           ;
  Energy_itr = EnergyHitMap->GetMap()->begin()                                                                ;

  //Time of Flight
  G4int StripTimeCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("CsIScorer/Time")          ;
  TimeHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripTimeCollectionID))               ;
  Time_itr = TimeHitMap->GetMap()->begin()                                                                    ;

  //Interaction Coordinate Z
  G4int InterCoordZCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("CsIScorer/InterCoordZ");
  PosZHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordZCollectionID));
  Pos_Z_itr = PosZHitMap->GetMap()->begin();

  G4int sizeN = DetectorNumberHitMap->entries()    ;
  G4int sizeE = EnergyHitMap->entries()          ;
  G4int sizeT = TimeHitMap->entries()          ;

  // Loop on CsI Number
  for (G4int l = 0 ; l < sizeN ; l++) {
    G4int N     =      *(DetectorNumber_itr->second)    ;
    G4int NTrackID  =   DetectorNumber_itr->first - N  ;


    if (N > 0) {
      m_Event->SetENumber(N) ;
      //  Energy
      Energy_itr = EnergyHitMap->GetMap()->begin();
      for (G4int h = 0 ; h < sizeE ; h++) {
        G4int ETrackID  =   Energy_itr->first  - N      ;
        G4double E     = *(Energy_itr->second)         ;
        if (ETrackID == NTrackID) {
            m_Event->SetEEnergy(RandGauss::shoot(E, E*ResoEnergy/100./2.35))    ;
          //m_Event->SetEEnergy(RandGauss::shoot(E, ResoEnergy))    ;
        }
        Energy_itr++;
      }


      //  Time
      Time_itr = TimeHitMap->GetMap()->begin();
      for (G4int h = 0 ; h < sizeT ; h++) {
        G4int TTrackID  =   Time_itr->first   - N    ;
        G4double T     = *(Time_itr->second)      ;
        if (TTrackID == NTrackID) {
          m_Event->SetTTime(RandGauss::shoot(T, ResoTime)) ;
        }
        Time_itr++;
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
    }

    DetectorNumber_itr++;
  }

  // clear map for next event
  TimeHitMap            ->clear()   ;    
  DetectorNumberHitMap    ->clear()   ;
  EnergyHitMap            ->clear()    ; 
  PosZHitMap			->clear() ;
     */

}


////////////////////////////////////////////////////////////////   
void CsI::InitializeScorers() { 
    bool already_exist = false;
    vector<G4int> NestingLevel;
    NestingLevel.push_back(0);
    NestingLevel.push_back(1);
    m_CsIScorer = CheckScorer("CsIScorer",already_exist) ;
    m_PDScorer = CheckScorer("PDScorer",already_exist) ;
  
    if(already_exist) return ;
    
    G4VPrimitiveScorer* CsIScorer= new CALORIMETERSCORERS::PS_Calorimeter("CsI",NestingLevel);
    m_CsIScorer->RegisterPrimitive(CsIScorer);
    
    G4VPrimitiveScorer* PDScorer = new SILICONSCORERS::PS_Silicon_Rectangle("PhotoDiode",0,
                                                                                PhotoDiodeFace,
                                                                                PhotoDiodeFace,
                                                                                1,
                                                                                1);
    m_PDScorer->RegisterPrimitive(PDScorer);
    
    G4SDManager::GetSDMpointer()->AddNewDetector(m_PDScorer) ;
    G4SDManager::GetSDMpointer()->AddNewDetector(m_CsIScorer) ;


    /*G4VPrimitiveScorer* DetNbr = new PSDetectorNumber("CsINumber","CsI", 0) ;
    G4VPrimitiveScorer* Energy = new PSEnergy("Energy","CsI", 0)                   ;
    G4VPrimitiveScorer* Time   = new PSTOF("Time","CsI", 0)                         ;
    G4VPrimitiveScorer* InteractionCoordinatesZ  			= new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesZ("InterCoordZ","CsI", 0);
    //and register it to the multifunctionnal detector
    m_CsIScorer->RegisterPrimitive(DetNbr)                         ;
    m_CsIScorer->RegisterPrimitive(Energy)                         ;
    m_CsIScorer->RegisterPrimitive(Time)                            ;
    m_CsIScorer->RegisterPrimitive(InteractionCoordinatesZ);
    G4SDManager::GetSDMpointer()->AddNewDetector(m_CsIScorer) ;
    G4SDManager::GetSDMpointer()->AddNewDetector(m_PDScorer) ;*/

}
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* CsI::Construct(){
    return  (NPS::VDetector*) new CsI();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
    class proxy_nps_csi{
    public:
        proxy_nps_csi(){
            NPS::DetectorFactory::getInstance()->AddToken("CsI","CsI");
            NPS::DetectorFactory::getInstance()->AddDetector("CsI",CsI::Construct);
        }
    };
    
    proxy_nps_csi p_nps_csi;
}
