/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Dustin Scriven  contact address: scrivend@tamu.edu       *
 *                                                                           *
 * Creation Date  : July 2018                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  TNT simulation                                      *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <sstream>
#include <cmath>
#include <limits>
#include <map>
#include <vector>
#include <stdlib.h>

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
#include "TNT.hh"
#include "NeutronDetectorScorers.hh"
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
namespace TNT_NS{
  // Energy and time Resolution
  const double EnergyThreshold = 0.1*MeV;
  const double ResoTime = 4.5*ns ;
  const double ResoEnergy = 1.0*MeV ;
  const double Radius = 50*mm ; 
  // const string Material = "para-Terphenyl_Scintillator";
  //const string Material = "BC400";  
  const string Material = "para-Terphenyl";
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// TNT Specific Method
TNT::TNT(){
  m_Event = new TTNTData() ;
  m_TNTScorer = 0;
  m_SquareDetector = 0;
  m_CylindricalDetector = 0;


  // RGB Color + Transparency
  m_VisSquare = new G4VisAttributes(G4Colour(0, 1, 1, 0.5));   
  m_VisCylinder = new G4VisAttributes(G4Colour(0, 0, 1, 0.5));   

}

TNT::~TNT(){
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TNT::AddDetector(G4ThreeVector POS, string  Shape){
  // Convert the POS value to R theta Phi as Spherical coordinate is easier in G4 

  m_R.push_back(POS.mag());
  m_Theta.push_back(POS.theta());
  m_Phi.push_back(POS.phi());
  m_Shape.push_back(Shape);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TNT::AddDetector(double  R, double  Theta, double  Phi, string  Shape){
  m_R.push_back(R);
  m_Theta.push_back(Theta);
  m_Phi.push_back(Phi);
  m_Shape.push_back(Shape);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* TNT::BuildSquareDetector(){
  if(!m_SquareDetector){
    G4Box* box = new G4Box("TNT_Box",Widthx*0.5,
        Widthy*0.5,Widthz*0.5);

    G4Material* DetectorMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(TNT_NS::Material);
    // DPS m_SquareDetector = new G4LogicalVolume(box,DetectorMaterial,"logic_TNT_Box",0,0,0);
    m_SquareDetector = new G4LogicalVolume(box,DetectorMaterial,"logic_TNT_Box",0,0,0);
    m_SquareDetector->SetVisAttributes(m_VisSquare);
    m_SquareDetector->SetSensitiveDetector(m_TNTScorer);
  }
  return m_SquareDetector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* TNT::BuildCylindricalDetector(){
  if(!m_CylindricalDetector){
    G4Tubs* tub = new G4Tubs("TNT_Cyl",0,TNT_NS::Radius,Widthz*0.5,0,360*deg);

    G4Material* DetectorMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(TNT_NS::Material);
    m_CylindricalDetector = new G4LogicalVolume(tub,DetectorMaterial,"logic_TNT_tub",0,0,0);
    m_CylindricalDetector->SetVisAttributes(m_VisSquare);
    m_CylindricalDetector->SetSensitiveDetector(m_TNTScorer);

  }
  return m_CylindricalDetector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class
// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void TNT::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("TNT");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> cart = {"POS","VOXNUM","VOXSIZE","Shape"};
  vector<string> sphe = {"R","Theta","Phi","Shape"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  TNT " << i+1 <<  endl;
    
      G4ThreeVector Pos = NPS::ConvertVector(blocks[i]->GetTVector3("POS","mm"));
      vector<int> vnumber = blocks[i]->GetVectorInt("VOXNUM");
      if((vnumber.size() != 3) || (vnumber[0]+vnumber[1]+vnumber[2] < 3)) {
        cout << "Check TNT.detector. Insure VOXNUM has three values >= 1" << endl;
        exit(1); // this exit method is not favored among C++ programmers!
        }
      G4ThreeVector VoxNum(vnumber.at(0),vnumber.at(1),vnumber.at(2));
      G4ThreeVector VoxWidth = NPS::ConvertVector(blocks[i]->GetTVector3("VOXSIZE","mm"));
      string Shape = blocks[i]->GetString("Shape");
      vector<int> WLS = blocks[i]->GetVectorInt("WLS");
      int WLSToggle = WLS[0];
      unsigned short wlsSize = WLS[1];
      
      // No WLS slabs or fibers
      Widthx = VoxWidth[0]; // x-width of individual cell
      Widthy = VoxWidth[1]; // y-width
      Widthz = VoxWidth[2]; // z-width
     
      int VoxNumx = VoxNum[0];     // number of voxels placed in x-dim
      int VoxNumy = VoxNum[1];     // y-dim
      int VoxNumz = VoxNum[2];     // z-dim

      // Shift array off center to build geometry
      Pos[0] -= Widthx*VoxNumx/2 - Widthx/2;
      Pos[1] -= Widthy*VoxNumy/2 - Widthy/2;
      Pos[2] -= Widthz*VoxNumz/2 - Widthz/2;

      // Store original offset of detector in dummy vars
      int dummyy = Pos[1];
      int dummyz = Pos[2];      
      int detnumber = 0;
      
      // Add WLS spacing
      if(WLSToggle == false) {
        wlsSize = 0;
        }

      // Build detector array
      for (int l=1; l<=VoxNumx; l++) {
        detnumber += 1;
        detcoord = {Pos[0],Pos[1],Pos[2]};
        DetectorInfo[detnumber] = detcoord;
        AddDetector(Pos,Shape);
//        cout << detnumber << " " << Pos[0] << " " << Pos[1] << " " << Pos[2] << endl;
	for (int m=1; m<=VoxNumy; m++) {
          if (m != 1) {            
            detnumber += 1;
            Pos[1] += Widthy;
            detcoord = {Pos[0],Pos[1],Pos[2]};
            DetectorInfo[detnumber] = detcoord;
	    AddDetector(Pos,Shape);
//            cout << detnumber << " " << Pos[0] << " " << Pos[1] << " " << Pos[2] << endl;
            }
          for (int n=1; n<=VoxNumz; n++) {
            if (n !=1) {
	      detnumber += 1;
              Pos[2] += Widthz + wlsSize;
              detcoord = {Pos[0],Pos[1],Pos[2]};
              DetectorInfo[detnumber] = detcoord;
              AddDetector(Pos,Shape);
//              cout << detnumber << " " << Pos[0] << " " << Pos[1] << " " << Pos[2] << endl;
              }
            }
            Pos[2] = dummyz;
          } 
        Pos[1] = dummyy; 
	Pos[0] += Widthx;
        }
    }

    else if(blocks[i]->HasTokenList(sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  TNT " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("Theta","deg");
      double Phi = blocks[i]->GetDouble("Phi","deg");
      string Shape = blocks[i]->GetString("Shape");
      AddDetector(R,Theta,Phi,Shape);
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
void TNT::ConstructDetector(G4LogicalVolume* world){
  for (size_t i = 0 ; i < m_R.size() ; i++) {

    G4double wX = m_R[i] * sin(m_Theta[i] ) * cos(m_Phi[i] ) ;
    G4double wY = m_R[i] * sin(m_Theta[i] ) * sin(m_Phi[i] ) ;
    G4double wZ = m_R[i] * cos(m_Theta[i] ) ;
    G4ThreeVector Det_pos = G4ThreeVector(wX, wY, wZ) ;
    // So the face of the detector is at R instead of the middle
    Det_pos+=Det_pos.unit()*Widthz*0.5;
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

    G4RotationMatrix* Rot = new G4RotationMatrix(0,0,0); // (u,v,w);

    if(m_Shape[i] == "Cylindrical"){
      new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
          BuildCylindricalDetector(),
          "TNT",world,false,i+1);
    }

    else if(m_Shape[i] == "Square"){
      new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
          BuildSquareDetector(),
          "TNT",world,false,i+1);
    }
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void TNT::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("TNT")){
    pTree->Branch("TNT", "TTNTData", &m_Event) ;
  }
  pTree->SetBranchAddress("TNT", &m_Event) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void TNT::ReadSensitive(const G4Event* event){

	auto ConvertToLight = [](double edep, int A, int Z){
		double light = edep;
		if(A == 0 && abs(Z) == 1) { // electron or positron
			light = edep;
		}
		else if(A >= 1 && Z == 1) { // p,d,t (NOTE - only correct for p!)
			light = 0.83*edep-2.82*(1-exp(-0.25*pow(edep,0.93)));
		}
		else if(A >= 3 && Z == 2) {  // 3He, alpha
			light = 0.41*edep-5.9*(1-exp(-0.065*pow(edep,1.01)));
		}
		else if(Z == 3)  { // Li
			light = 0.1795*( edep );   // Obtained from EXP fit of measured leading coeffs
		}
		else if(Z == 4) { // Be
			light = 0.0821*( edep );   // Obtained from EXP fit of measured leading coeffs
		}
		else if(Z == 5) { // B
			light = 0.0375*( edep );  // Obtained from EXP fit of measured leading coeffs
		}
		else if(Z == 6) { // Carbon
			light = 0.017*( edep );
		}

		return light > 0 ? light : 0;
	};
	
  m_Event->Clear();

  ///////////
  // Calorimeter scorer
  NPS::HitsMap<NEUTRONDETECTORSCORERS::StepInfo>* CaloHitMap;
  std::map<G4int, NEUTRONDETECTORSCORERS::StepInfo*>::iterator Calo_itr;

  G4int CaloCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("TNTScorer/Calorimeter");
  CaloHitMap = (NPS::HitsMap<NEUTRONDETECTORSCORERS::StepInfo>*)(event->GetHCofThisEvent()->GetHC(CaloCollectionID));

  // Loop on the Calo map
  for (Calo_itr = CaloHitMap->GetMap()->begin() ; Calo_itr != CaloHitMap->GetMap()->end() ; Calo_itr++){

    vector<double> Info = Calo_itr->second->Infos;
    double Energy = Info[0]; //RandGauss::shoot(Info[0],TNT_NS::ResoEnergy);

		if(Calo_itr->second->ProcessName == "menateR_neutron") {
			//
			// Convert Energy -> Light (Quenching)
			double light = ConvertToLight(Energy, Calo_itr->second->Particle_A,
																		 Calo_itr->second->Particle_Z);

			Energy = RandGauss::shoot(light, light*sqrt(light*27000)/27000);
		}
		
    if(Energy>TNT_NS::EnergyThreshold){
      double Time = Info[1];  //RandGauss::shoot(Info[1],TNT_NS::ResoTime);
      int DetectorNbr = (int) Info[7];
      m_Event->SetEnergy(DetectorNbr,Energy);
      m_Event->SetTime(DetectorNbr,Time); 
      // cout << DetectorNbr << " " << Energy << " " << Time << " " << endl;
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////   
void TNT::InitializeScorers() { 
  // This check is necessary in case the geometry is reloaded
  bool already_exist = false; 
  m_TNTScorer = CheckScorer("TNTScorer",already_exist) ;

  if(already_exist) 
    return ;

  // Otherwise the scorer is initialised
  vector<int> level; level.push_back(0);
  G4VPrimitiveScorer* Calorimeter= new NEUTRONDETECTORSCORERS::PS_NeutronDetector("Calorimeter",level, 0) ;
  //and register it to the multifunctionnal detector
  m_TNTScorer->RegisterPrimitive(Calorimeter);
  G4SDManager::GetSDMpointer()->AddNewDetector(m_TNTScorer) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* TNT::Construct(){
  return  (NPS::VDetector*) new TNT();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
  class proxy_nps_TNT{
    public:
      proxy_nps_TNT(){
        NPS::DetectorFactory::getInstance()->AddToken("TNT","TNT");
        NPS::DetectorFactory::getInstance()->AddDetector("TNT",TNT::Construct);
      }
  };

  proxy_nps_TNT p_nps_TNT;
}
