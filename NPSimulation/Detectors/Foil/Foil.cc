/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Mhd Moukaddam  contact address: m.moukaddam@surrey.ac.uk *
 *                                                                           *
 * Creation Date  : July 2017                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describes the simulation of a simple generic foil normally    *
 *  used in experiments as a catcher before/after target or a screen of      *
 *  other detectors e.g. the common Aluminum foil placed upstream the        *
 *  TRIFOIL with SHARC experiments                                           * 
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
#include "Foil.hh"
#include "CalorimeterScorers.hh"
#include "RootOutput.h"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "NPOptionManager.h"
#include "NPSHitsMap.hh"
// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace Foil_NS{
  // Energy and time Resolution
  const double ResoTime = 4.5*ns ;
  const double ResoEnergy = 1.0*MeV ;
  const double EnergyThreshold = 1*keV;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Foil Specific Method
Foil::Foil(){
  m_Event = new TFoilData() ;
  m_FoilScorer = 0;
  m_SquareDetector = 0;
  m_CylindricalDetector = 0;


  // RGB Color + Transparency
  m_VisSquare = new G4VisAttributes(G4Colour(0, 1, 0, 0.5));   
  m_VisCylinder = new G4VisAttributes(G4Colour(0, 0, 1, 0.5));   

}

Foil::~Foil(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Foil::AddDetector(G4ThreeVector POS, string Shape, double Radius, double Thick, string Material){
  // Convert the POS value to R theta Phi as Spherical coordinate is easier in G4 
  m_R.push_back(POS.mag());
  m_Theta.push_back(POS.theta());
  m_Phi.push_back(POS.phi());
  m_Shape.push_back(Shape);

  m_FoilThickness.push_back(Thick) ;
  m_FoilRadius.push_back(Radius)    ; // cylindrical shape
  m_FoilHeight.push_back(-1)     ; // squared shape
  m_FoilWidth.push_back(-1)      ; // squared shape
  m_FoilMaterial.push_back(Material)  ; 

}

void Foil::AddDetector(G4ThreeVector POS, string Shape, double Height, double Width, double Thick, string Material){
  // Convert the POS value to R theta Phi as Spherical coordinate is easier in G4 
  m_R.push_back(POS.mag());
  m_Theta.push_back(POS.theta());
  m_Phi.push_back(POS.phi());
  m_Shape.push_back(Shape);

  m_FoilThickness.push_back(Thick) ;
  m_FoilRadius.push_back(-1)    ; // cylindrical shape
  m_FoilHeight.push_back(Height)     ; // squared shape
  m_FoilWidth.push_back(Width)      ; // squared shape
  m_FoilMaterial.push_back(Material)  ; 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Foil::BuildSquareDetector(double Height, double Width, double Thickness, string Material){
  if(!m_SquareDetector){
    G4Box* box = new G4Box("Foil_Box",Height*0.5, Width*0.5,Thickness*0.5);

    G4Material* DetectorMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(Material);
    m_SquareDetector = new G4LogicalVolume(box,DetectorMaterial,"logic_Foil_Box",0,0,0);
    m_SquareDetector->SetVisAttributes(m_VisSquare);
    m_SquareDetector->SetSensitiveDetector(m_FoilScorer);
  }
  return m_SquareDetector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Foil::BuildCylindricalDetector(double Radius, double Thickness, string Material){
  if(!m_CylindricalDetector){
    G4Tubs* tub = new G4Tubs("Foil_Cyl",0,Radius,Thickness*0.5,0,360*deg);

    G4Material* DetectorMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(Material);
    m_CylindricalDetector = new G4LogicalVolume(tub,DetectorMaterial,"logic_Foil_tub",0,0,0);
    m_CylindricalDetector->SetVisAttributes(m_VisSquare);
    m_CylindricalDetector->SetSensitiveDetector(m_FoilScorer);

  }
  return m_CylindricalDetector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Foil::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Foil");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> cart = {"POS",};
  vector<string> sphe = {"R","Theta","Phi"};
  vector<string> square= {"Shape","Height","Width","Thickness","Material"};
  vector<string> cylind= {"Shape","Radius","Thickness","Material"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel()){
        cout << endl << "////  Foil " << i+1 <<  endl;
        G4ThreeVector Pos = NPS::ConvertVector(blocks[i]->GetTVector3("POS","mm"));
        if(blocks[i]->HasTokenList(square)){
          string Shape = blocks[i]->GetString("Shape");
          double H = blocks[i]->GetDouble("Height","mm");
          double W = blocks[i]->GetDouble("Width","mm");
          double T = blocks[i]->GetDouble("Thickness","mm");
          string Mat = blocks[i]->GetString("Material");
          AddDetector(Pos,Shape,H,W,T,Mat);
        }

        else if(blocks[i]->HasTokenList(cylind)){
          string Shape = blocks[i]->GetString("Shape");
          double Rd = blocks[i]->GetDouble("Radius","mm");
          double T = blocks[i]->GetDouble("Thickness","mm");
          string Mat = blocks[i]->GetString("Material");
          AddDetector(Pos,Shape,Rd,T,Mat);
        }
      }
    }
    else if(blocks[i]->HasTokenList(sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel()){
        cout << endl << "////  Foil " << i+1 <<  endl;
      
        double R = blocks[i]->GetDouble("R","mm");
        double Theta = blocks[i]->GetDouble("Theta","deg");
        double Phi = blocks[i]->GetDouble("Phi","deg");
        G4ThreeVector Pos;
        Pos.setRThetaPhi(R,Theta,Phi);

        if(blocks[i]->HasTokenList(square)){
          string Shape = blocks[i]->GetString("Shape");
          double H = blocks[i]->GetDouble("Height","mm");
          double W = blocks[i]->GetDouble("Width","mm");
          double T = blocks[i]->GetDouble("Thickness","mm");
          string Mat = blocks[i]->GetString("Material");
          AddDetector(Pos,Shape,H,W,T,Mat);
        }
        else if(blocks[i]->HasTokenList(cylind)){
          string Shape = blocks[i]->GetString("Shape");
          double Rd = blocks[i]->GetDouble("Radius","mm");
          double T = blocks[i]->GetDouble("Thickness","mm");
          string Mat = blocks[i]->GetString("Material");
          AddDetector(Pos,Shape,Rd,T,Mat);
        }
      }
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
void Foil::ConstructDetector(G4LogicalVolume* world){

  for (unsigned short i = 0 ; i < m_R.size() ; i++) {

    double wX = m_R[i] * sin(m_Theta[i] ) * cos(m_Phi[i] ) ;
    double wY = m_R[i] * sin(m_Theta[i] ) * sin(m_Phi[i] ) ;
    double wZ = m_R[i] * cos(m_Theta[i] ) ;
    G4ThreeVector Det_pos = G4ThreeVector(wX, wY, wZ) ;
    // So the face of the detector is at R instead of the middle
    Det_pos+=Det_pos.unit()*(m_FoilThickness[i]*0.5);
    // Building Detector reference frame
    double ii = cos(m_Theta[i]) * cos(m_Phi[i]);
    double jj = cos(m_Theta[i]) * sin(m_Phi[i]);
    double kk = -sin(m_Theta[i]);
    G4ThreeVector Y(ii,jj,kk);
    G4ThreeVector w = Det_pos.unit();
    G4ThreeVector u = w.cross(Y);
    G4ThreeVector v = w.cross(u);
    v = v.unit();
    u = u.unit();

    G4RotationMatrix* Rot = new G4RotationMatrix(u,v,w);

    if(m_Shape[i] == "Cylindrical"){
      new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
          BuildCylindricalDetector(m_FoilRadius[i], m_FoilThickness[i], m_FoilMaterial[i]),"Foil",world,false,i+1);
    }

    else if(m_Shape[i] == "Square"){
      new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
          BuildSquareDetector(m_FoilHeight[i], m_FoilWidth[i], m_FoilThickness[i], m_FoilMaterial[i]),"Foil",world,false,i+1);
    }
  }
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Foil::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("Foil")){
    pTree->Branch("Foil", "TFoilData", &m_Event) ;
  }
  pTree->SetBranchAddress("Foil", &m_Event) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Foil::ReadSensitive(const G4Event* ){
  m_Event->Clear();

  ///////////
  // Calorimeter scorer
  CalorimeterScorers::PS_Calorimeter* Scorer= (CalorimeterScorers::PS_Calorimeter*) m_FoilScorer->GetPrimitive(0);
  unsigned int size = Scorer->GetMult(); 
  for(unsigned int i = 0 ; i < size ; i++){
    double Energy = RandGauss::shoot(Scorer->GetEnergy(i),Foil_NS::ResoEnergy);
    if(Energy>Foil_NS::EnergyThreshold){
      double Time = RandGauss::shoot(Scorer->GetTime(i),Foil_NS::ResoTime);
      int DetectorNbr = Scorer->GetLevel(i)[0];
      m_Event->SetEnergy(DetectorNbr,Energy);
      m_Event->SetTime(DetectorNbr,Time); 
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////   
void Foil::InitializeScorers() { 
  // This check is necessary in case the geometry is reloaded
  bool already_exist = false; 
  m_FoilScorer = CheckScorer("FoilScorer",already_exist) ;

  if(already_exist) 
    return ;

  // Otherwise the scorer is initialised
  vector<int> level; level.push_back(0);
  G4VPrimitiveScorer* Calorimeter= new CalorimeterScorers::PS_Calorimeter("Calorimeter",level, 0) ;
  //and register it to the multifunctionnal detector
  m_FoilScorer->RegisterPrimitive(Calorimeter);
  G4SDManager::GetSDMpointer()->AddNewDetector(m_FoilScorer) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* Foil::Construct(){
  return  (NPS::VDetector*) new Foil();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
  class proxy_nps_Foil{
    public:
      proxy_nps_Foil(){
        NPS::DetectorFactory::getInstance()->AddToken("Foil","Foil");
        NPS::DetectorFactory::getInstance()->AddDetector("Foil",Foil::Construct);
      }
  };

  proxy_nps_Foil p_nps_Foil;
}
