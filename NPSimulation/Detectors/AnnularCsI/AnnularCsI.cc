/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Greg Christian  contact address: gchristian@tamu.edu     *
 *                                                                           *
 * Creation Date  : March 2018                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  AnnularCsI simulation                               *
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
#include "AnnularCsI.hh"
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
using namespace AnnularCsI_Utils;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace AnnularCsI_NS{
// Energy and time Resolution
const double EnergyThreshold = 0.*MeV; // for now
const double ResoTime = 10*ns ; // ??? (doesn't really matter)
const double ResoEnergy = 0.01; // percent, from Wilton
// const double Radius = 50*mm ; // not used 
// const double Width = 100*mm ; // not used
const double Thickness = 10*mm ; // from Wilton
const string Material = "CsI";
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// AnnularCsI Specific Method
AnnularCsI::AnnularCsI(){
  m_Event = new TAnnularCsIData() ;
  m_AnnularCsIScorer = 0;
  m_CylindricalDetector = 0;

  // RGB Color + Transparency
  m_VisSquare = new G4VisAttributes(G4Colour(0, 1, 0, 0.5));   
  m_VisCylinder = new G4VisAttributes(G4Colour(0, 0, 1, 0.5));   
}

AnnularCsI::~AnnularCsI(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void AnnularCsI::AddDetector(double R_min,   double R_max,
														 double Phi_min, double Phi_max,
														 double Z){
	// Wedge Geometry
	Geometry g;
	g.R_min     = R_min;
	g.R_max     = R_max;
	g.R_delta   = R_max - R_min;
	g.Phi_min   = Phi_min;
	g.Phi_max   = Phi_max;
	g.Phi_delta = Phi_max - Phi_min;
	g.Z         = Z;
	m_Geo.push_back(g);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* AnnularCsI::BuildCylindricalDetector(unsigned short i){
  if(!m_CylindricalDetector){
		if(!(i<m_Geo.size())){
			G4cerr << "ERROR (AnnularCsI::BuildCylindricalDetector):"
						 << "Invalid index: " << i << endl;
			exit(1);
		}
		stringstream name; name << "AnnularCsI_Wedge" << i+1;
    G4Tubs* tub = new G4Tubs(name.str().c_str(),
														 m_Geo[i].R_min, m_Geo[i].R_max,
														 AnnularCsI_NS::Thickness*0.5,
														 m_Geo[i].Phi_min, m_Geo[i].Phi_delta);

    G4Material* DetectorMaterial = 
			MaterialManager::getInstance()->GetMaterialFromLibrary(AnnularCsI_NS::Material);
    m_CylindricalDetector =
			new G4LogicalVolume(tub,DetectorMaterial,"logic_AnnularCsI_tub",0,0,0);
    m_CylindricalDetector->SetVisAttributes(m_VisSquare);
    m_CylindricalDetector->SetSensitiveDetector(m_AnnularCsIScorer);
  }
  return m_CylindricalDetector;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void AnnularCsI::ReadConfiguration(NPL::InputParser parser){
	vector<Geometry> detectors =
		AnnularCsI_Utils::ReadConfiguration(parser);
	for(const auto& d : detectors){
		AddDetector(d.R_min,d.R_max,d.Phi_min,d.Phi_max,d.Z);
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void AnnularCsI::ConstructDetector(G4LogicalVolume* world){
  for (unsigned short i = 0 ; i < m_Geo.size() ; i++) {

		// each wedge is part of a circle centered at {0,0,Z}
    G4double wX = 0;
    G4double wY = 0;
    G4double wZ = m_Geo[i].Z;
    G4ThreeVector Det_pos = G4ThreeVector(wX, wY, wZ) ;
    // So the face of the detector is at R instead of the middle
    Det_pos+=Det_pos.unit()*AnnularCsI_NS::Thickness*0.5;
    // // Building Detector reference frame
    // G4double ii = cos(m_Theta[i]) * cos(m_Phi[i]);
    // G4double jj = cos(m_Theta[i]) * sin(m_Phi[i]);
    // G4double kk = -sin(m_Theta[i]);
    // G4ThreeVector Y(ii,jj,kk);
    // G4ThreeVector w = Det_pos.unit();
    // G4ThreeVector u = w.cross(Y);
    // G4ThreeVector v = w.cross(u);
    // v = v.unit();
    // u = u.unit();

    // G4RotationMatrix* Rot = new G4RotationMatrix(u,v,w);

		// Cylindrical detector
		new G4PVPlacement(0, Det_pos, // G4Transform3D(*Rot,Det_pos),
											BuildCylindricalDetector(i),
											"AnnularCsI",world,false,i+1);
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void AnnularCsI::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("AnnularCsI")){
    pTree->Branch("AnnularCsI", "TAnnularCsIData", &m_Event) ;
  }
  pTree->SetBranchAddress("AnnularCsI", &m_Event) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void AnnularCsI::ReadSensitive(const G4Event* event){
  m_Event->Clear();

  ///////////
  // Calorimeter scorer
  NPS::HitsMap<G4double*>* CaloHitMap;
  std::map<G4int, G4double**>::iterator Calo_itr;

  G4int CaloCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("AnnularCsIScorer/Calorimeter");
  CaloHitMap = (NPS::HitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(CaloCollectionID));

  // Loop on the Calo map
  for (Calo_itr = CaloHitMap->GetMap()->begin() ; Calo_itr != CaloHitMap->GetMap()->end() ; Calo_itr++){

    G4double* Info = *(Calo_itr->second);
		double Eres = AnnularCsI_NS::ResoEnergy*Info[0];
    double Energy = RandGauss::shoot(Info[0],Eres);
    if(Energy>AnnularCsI_NS::EnergyThreshold){
      double Time = RandGauss::shoot(Info[1],AnnularCsI_NS::ResoTime);
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
void AnnularCsI::InitializeScorers() { 
  // This check is necessary in case the geometry is reloaded
  bool already_exist = false; 
  m_AnnularCsIScorer = CheckScorer("AnnularCsIScorer",already_exist) ;

  if(already_exist) 
    return ;

  // Otherwise the scorer is initialised
  vector<int> level; level.push_back(0);
  G4VPrimitiveScorer* Calorimeter= new CALORIMETERSCORERS::PS_Calorimeter("Calorimeter",level, 0) ;
  //and register it to the multifunctionnal detector
  m_AnnularCsIScorer->RegisterPrimitive(Calorimeter);
  G4SDManager::GetSDMpointer()->AddNewDetector(m_AnnularCsIScorer) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* AnnularCsI::Construct(){
  return  (NPS::VDetector*) new AnnularCsI();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
class proxy_nps_AnnularCsI{
public:
	proxy_nps_AnnularCsI(){
		NPS::DetectorFactory::getInstance()->AddToken("AnnularCsI","AnnularCsI");
		NPS::DetectorFactory::getInstance()->AddDetector("AnnularCsI",AnnularCsI::Construct);
	}
};

proxy_nps_AnnularCsI p_nps_AnnularCsI;
}
