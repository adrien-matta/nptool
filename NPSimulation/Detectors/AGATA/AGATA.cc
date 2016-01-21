/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************.

/*****************************************************************************
 * Original Author: Adrien Matta  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : January 2016                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  AGATA simulation                                    *
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
#include "AGATA.hh"
#include "CalorimeterScorers.hh"
#include "RootOutput.h"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace AGATA_NS{
  // Energy and time Resolution
  const double EnergyThreshold = 0.1*MeV;
  const double ResoTime = 4.5*ns ;
  const double ResoEnergy = 5.0*MeV ;
  const double Radius = 50*mm ; 
  const double Width = 500*mm ;
  const double Thickness = 300*mm ;
  const string Scintillator = "BC400";
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// AGATA Specific Method
AGATA::AGATA(){
  m_Event = new TAGATAData() ;
  m_AGATAScorer = 0;
  m_SquareDetector = 0;
  m_CylindricalDetector = 0;


  // RGB Color + Transparency
  m_VisSquare = new G4VisAttributes(G4Colour(0, 1, 0, 0.5));   
  m_VisCylinder = new G4VisAttributes(G4Colour(0, 0, 1, 0.5));   

}

AGATA::~AGATA(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void AGATA::AddAGATA(double  R, double  Theta, double  Phi, string  Shape){
  m_R.push_back(R);
  m_Theta.push_back(Theta);
  m_Phi.push_back(Phi);
  m_Shape.push_back(Shape);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* AGATA::BuildSquareDetector(){
  if(!m_SquareDetector){
    G4Box* box = new G4Box("AGATA_Box",AGATA_NS::Width*0.5,
        AGATA_NS::Width*0.5,AGATA_NS::Thickness*0.5);
    G4Material* Vac= MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
    m_SquareDetector = new G4LogicalVolume(box,Vac,"logic_AGATA_Box",0,0,0);

    string path = getenv("NPTOOL");
    path+="/NPSimulation/Detectors/AGATA/gdml/AGATA1clust.gdml";
    m_gdmlparser.Read(path);
    G4RotationMatrix* Rot = new G4RotationMatrix;
    G4ThreeVector Det_pos(0,0,0);
    new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
        m_gdmlparser.GetVolume("Element_Step_0_200195712_Volume"),
        "AGATA0",m_SquareDetector,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
        m_gdmlparser.GetVolume("Element_Step_1_200195936_Volume"),
        "AGATA1",m_SquareDetector,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
        m_gdmlparser.GetVolume("Element_Step_2_200196160_Volume"),
        "AGATA2",m_SquareDetector,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
        m_gdmlparser.GetVolume("Element_Step_3_200196384_Volume"),
        "AGATA3",m_SquareDetector,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
        m_gdmlparser.GetVolume("Element_Step_4_200196608_Volume"),
        "AGATA4",m_SquareDetector,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
        m_gdmlparser.GetVolume("Element_Step_5_200196832_Volume"),
        "AGATA5",m_SquareDetector,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
        m_gdmlparser.GetVolume("Element_Step_6_200197056_Volume"),
        "AGATA6",m_SquareDetector,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
        m_gdmlparser.GetVolume("Element_Step_7_200197280_Volume"),
        "AGATA7",m_SquareDetector,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
        m_gdmlparser.GetVolume("Element_Step_8_200197504_Volume"),
        "AGATA8",m_SquareDetector,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
        m_gdmlparser.GetVolume("Element_Step_9_200197728_Volume"),
        "AGATA9",m_SquareDetector,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
        m_gdmlparser.GetVolume("Element_Step_10_200197952_Volume"),
        "AGATA10",m_SquareDetector,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
        m_gdmlparser.GetVolume("Element_Step_11_200198176_Volume"),
        "AGATA11",m_SquareDetector,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
        m_gdmlparser.GetVolume("Element_Step_12_200198400_Volume"),
        "AGATA12",m_SquareDetector,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
        m_gdmlparser.GetVolume("Element_Step_13_200198624_Volume"),
        "AGATA13",m_SquareDetector,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
        m_gdmlparser.GetVolume("Element_Step_14_200198848_Volume"),
        "AGATA14",m_SquareDetector,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
        m_gdmlparser.GetVolume("Element_Step_15_200199072_Volume"),
        "AGATA15",m_SquareDetector,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
        m_gdmlparser.GetVolume("Element_Step_16_200199296_Volume"),
        "AGATA16",m_SquareDetector,false,0);

    new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
        m_gdmlparser.GetVolume("Element_Step_17_200199520_Volume"),
        "AGATA17",m_SquareDetector,false,0);
  }
  return m_SquareDetector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* AGATA::BuildCylindricalDetector(){
  if(!m_CylindricalDetector){
    G4Tubs* tub = new G4Tubs("AGATA_Cyl",0,AGATA_NS::Radius,AGATA_NS::Thickness*0.5,0,360*deg);

    G4Material* ScintMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(AGATA_NS::Scintillator);
    m_CylindricalDetector = new G4LogicalVolume(tub,ScintMaterial,"logic_AGATA_tub",0,0,0);
    m_CylindricalDetector->SetVisAttributes(m_VisSquare);
    m_CylindricalDetector->SetSensitiveDetector(m_AGATAScorer);

  }
  return m_CylindricalDetector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void AGATA::ReadConfiguration(string Path){
  ifstream ConfigFile           ;
  ConfigFile.open(Path.c_str()) ;
  string LineBuffer          ;
  string DataBuffer          ;

  double Theta = 0 , Phi = 0 , R = 0 ;
  double X = 0 , Y = 0 , Z = 0 ;
  string Shape ;

  bool check_Theta = false ;
  bool check_Phi = false ;
  bool check_R = false ;
  bool check_Shape = false ;
  bool check_X = false ;
  bool check_Y = false ;
  bool check_Z = false ;      
  bool ReadingStatus = false ;


  while (!ConfigFile.eof()) {
    getline(ConfigFile, LineBuffer);

    //   If line is a Start Up AGATA bloc, Reading toggle to true      
    string name = "AGATA";

    if (LineBuffer.compare(0, name.length(), name) == 0) {
      G4cout << "///" << G4endl           ;
      G4cout << "AGATA found: " << G4endl   ;        
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
      else if (DataBuffer.compare(0, name.length(),name) == 0) {
        G4cout << "WARNING: Another Detector is find before standard sequence of Token, Error may occured in Telecope definition" << G4endl ;
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

      ///////////////////////////////////////////////////
      //   If no Detector Token and no comment, toggle out
      else{
        ReadingStatus = false; 
        G4cout << "Wrong Token Sequence: Getting out " << DataBuffer << G4endl ;
      }

      /////////////////////////////////////////////////
      //   If All necessary information there, toggle out

      if (( check_Theta && check_Phi && check_R && check_Shape)
          ||    
          ( check_X && check_Y && check_Z && check_Shape)){


        // Convert Cartesian to Spherical (detector always face the target)
        if (check_X){
          R = sqrt (X*X+Y*Y+Z*Z);
          Theta = acos(Z / (R) );
          Phi = atan2(Y,X);
        }

        AddAGATA(R,Theta,Phi,Shape);

        //   Reinitialisation of Check Boolean 
        check_Theta = false ;
        check_Phi = false ;
        check_R = false ;
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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void AGATA::ConstructDetector(G4LogicalVolume* world){
  for (unsigned short i = 0 ; i < m_R.size() ; i++) {

    G4double wX = m_R[i] * sin(m_Theta[i] ) * cos(m_Phi[i] ) ;
    G4double wY = m_R[i] * sin(m_Theta[i] ) * sin(m_Phi[i] ) ;
    G4double wZ = m_R[i] * cos(m_Theta[i] ) ;
    G4ThreeVector Det_pos = G4ThreeVector(wX, wY, wZ) ;
    // So the face of the detector is at R instead of the middle
    Det_pos+=Det_pos.unit()*AGATA_NS::Thickness*0.5;
    // Building Detector reference frame
    G4double ii = cos(m_Theta[i]) * cos(m_Phi[i]);
    G4double jj = cos(m_Theta[i]) * sin(m_Phi[i]);
    G4double kk = -sin(m_Theta[i]);
    G4ThreeVector Y(ii,jj,kk);
    G4ThreeVector w = Det_pos.unit();
    G4ThreeVector u = w.cross(Y);
    G4ThreeVector v = w.cross(u);
    v = v.unit();
    u = u.unit();

    G4RotationMatrix* Rot = new G4RotationMatrix(u,v,w);

    if(m_Shape[i] == "Cylindrical"){
      new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
          BuildCylindricalDetector(),
          "AGATA",world,false,i+1);
    }

    else if(m_Shape[i] == "Square"){
      new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
          BuildSquareDetector(),
          "AGATA",world,false,i+1);
    }
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void AGATA::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  pTree->Branch("AGATA", "TAGATAData", &m_Event) ;
  pTree->SetBranchAddress("AGATA", &m_Event) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void AGATA::ReadSensitive(const G4Event* event){
  m_Event->Clear();

  ///////////
  // Calorimeter scorer
  G4THitsMap<G4double*>* CaloHitMap;
  std::map<G4int, G4double**>::iterator Calo_itr;

  G4int CaloCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("AGATAScorer/Calorimeter");
  CaloHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(CaloCollectionID));

  // Loop on the Calo map
  for (Calo_itr = CaloHitMap->GetMap()->begin() ; Calo_itr != CaloHitMap->GetMap()->end() ; Calo_itr++){

    G4double* Info = *(Calo_itr->second);
    //(Info[0]/2.35)*((Info[0]*1.02)*pow((Info[0]*1.8),.5))
    // double Energy = RandGauss::shoot(Info[0],((Info[0]*1000*1.02/2.35)*pow((Info[0]*1000*1.8),.5)) );
    double Energy = RandGauss::shoot(Info[0],AGATA_NS::ResoEnergy);
    if(Energy>AGATA_NS::EnergyThreshold){
      double Time = RandGauss::shoot(Info[1],AGATA_NS::ResoTime);
      int DetectorNbr = (int) Info[2];
      m_Event->SetEnergy(DetectorNbr,Energy);
      m_Event->SetTime(DetectorNbr,Time); 
    }
  }
  // clear map for next event
  CaloHitMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////   
void AGATA::InitializeScorers() { 
  // This check is necessary in case the geometry is reloaded
  bool already_exist = false; 
  m_AGATAScorer = CheckScorer("AGATAScorer",already_exist) ;

  if(already_exist) 
    return ;

  // Otherwise the scorer is initialised
  vector<int> level; level.push_back(0);
  G4VPrimitiveScorer* Calorimeter= new CALORIMETERSCORERS::PS_Calorimeter("Calorimeter",level, 0) ;
  //and register it to the multifunctionnal detector
  m_AGATAScorer->RegisterPrimitive(Calorimeter);
  G4SDManager::GetSDMpointer()->AddNewDetector(m_AGATAScorer) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* AGATA::Construct(){
  return  (NPS::VDetector*) new AGATA();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
  class proxy_nps_plastic{
    public:
      proxy_nps_plastic(){
        NPS::DetectorFactory::getInstance()->AddToken("AGATA","AGATA");
        NPS::DetectorFactory::getInstance()->AddDetector("AGATA",AGATA::Construct);
      }
  };

  proxy_nps_plastic p_nps_plastic;
}
