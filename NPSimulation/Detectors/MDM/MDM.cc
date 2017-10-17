/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Greg Christian  contact address: gchristian@tamu.edu     *
 *                                                                           *
 * Creation Date  : October 2017                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  MDM simulation                                      *
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
#include "MDM.hh"
#include "MDMScorer.hh"
#include "RootOutput.h"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "NPOptionManager.h"
#include "NPSHitsMap.hh"

// ROOT
#include "TSystem.h"

// CLHEP header
#include "CLHEP/Random/RandGauss.h"

#include "Rayin.h"
#include "MDMTrace.h"

using namespace std;
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace MDM_NS{
  // Energy and time Resolution
  const double Width = 200*mm ;
  const double Thickness = Width;
  const string Material = "BC400";
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// MDM Specific Method
MDM::MDM(){
  m_Event = new TMDMData() ;
  m_MDMScorer = 0;
  m_SquareDetector = 0;
  m_Angle = 0;
  m_Field = 0;
  m_Rayin = 0;
}

MDM::~MDM(){
  if(m_Rayin) { delete m_Rayin; m_Rayin = 0; }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MDM::AddDetector(double angle, double field, double xaccept, double yaccept, const string& rayin){
  m_Angle = angle;
  m_Field = field;
	m_Xaccept = xaccept;
	m_Yaccept = yaccept;
  m_Rayin_file = rayin;
  
  m_Rayin = new Rayin(m_Rayin_file, false);
  m_Trace = MDMTrace::Instance();

  m_Trace->SetMDMAngle(angle/mrad);  // mrad
  m_Trace->SetMDMDipoleField(field/gauss); // gauss

  cout << "MDM::AddDetector :: Angle [mrad], Angle [deg], Field [G], Rayin File :: " 
       << angle/mrad << ", " << angle/deg << ", " << field/gauss << ", " << m_Rayin_file << "\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* MDM::BuildSquareDetector(){
  if(!m_SquareDetector){
    G4Box* box = new G4Box("MDM_Box",MDM_NS::Width*0.5,
			   MDM_NS::Width*0.5,MDM_NS::Thickness*0.5);

    G4Material* DetectorMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(MDM_NS::Material);
    m_SquareDetector = new G4LogicalVolume(box,DetectorMaterial,"logic_MDM_Box",0,0,0);
    m_SquareDetector->SetVisAttributes(m_VisSquare);
    m_SquareDetector->SetSensitiveDetector(m_MDMScorer);
  }
  return m_SquareDetector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void MDM::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("MDM");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> sphe = {"Angle","Field","Rayin"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  MDM " << i+1 <<  endl;
      double Angle = blocks[i]->GetDouble("Angle","deg");
      double Field = blocks[i]->GetDouble("Field","gauss");
			double XA    = blocks[i]->GetDouble("XAccept","deg");
			double YA    = blocks[i]->GetDouble("YAccept","deg");
      string Rayin = blocks[i]->GetString("Rayin");
      AddDetector(Angle, Field, XA, YA, Rayin);
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
void MDM::ConstructDetector(G4LogicalVolume* world){
  G4double wX = 0;
  G4double wY = 0;
  G4double wZ = 1e-6*m;
  G4ThreeVector Det_pos = G4ThreeVector(wX, wY, wZ) ;

  new G4PVPlacement(0, Det_pos, BuildSquareDetector(), 
		    "MDM0_Spectrometer", world, false, 0);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void MDM::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("MDM")){
    pTree->Branch("MDM", "TMDMData", &m_Event) ;
  }
  pTree->SetBranchAddress("MDM", &m_Event) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void MDM::ReadSensitive(const G4Event* event){
  m_Event->Clear();

  G4int ID = G4SDManager::GetSDMpointer()->GetCollectionID("MDMScorer/ScorerMDM");
  NPS::HitsMap<MDMScorer::Infos>* Hits =
    static_cast<NPS::HitsMap<MDMScorer::Infos>*> (event->GetHCofThisEvent()->GetHC(ID));

  size_t indx = 0;
  for(auto& Iter : *(Hits->GetMap())) {
    // Read energy, position, momentum
    double Ekin              = Iter.second->Edep;  // MeV
    double Mass              = Iter.second->Mass;  // MeV/c^2
    double Charge            = Iter.second->Charge;// e
    const G4ThreeVector& Pos = Iter.second->Pos;   // mm
    const G4ThreeVector& Mom = Iter.second->Mom;   // rad
		
    // Calculate dispersive & non-dispersive angles
    double thetaX = atan(Mom.x() / Mom.z());
    double thetaY = atan(Mom.y() / Mom.z());

		double x[4] = {1e10,1e10,1e10,1e10};
		double y[4] = {1e10,1e10,1e10,1e10};
		double a = 1e10;
		double b = 1e10;

		// check if within acceptance
		// saves lots of time not tracking events outside of the
		// acceptance
		if(fabs(thetaX) < m_Xaccept && fabs(thetaY) < m_Yaccept)
		{
			// Send Through MDM
			m_Trace->SetScatteredMass(Mass/amu_c2);
			m_Trace->SetScatteredCharge(Charge);
			m_Trace->SetScatteredAngle(thetaX/deg, thetaY/deg);
			m_Trace->SetScatteredEnergy(Ekin/MeV);
			m_Trace->SetBeamPosition(Pos.x()/cm, Pos.y()/cm, Pos.z()/cm);
			m_Trace->SendRay();

			// Read wire1 position, angle
			m_Trace->GetOxfordWirePositions(a,x[0],x[1],x[2],x[3],b,y[0],y[1],y[2],y[3]);
		}
		
    // Set X, Y positions in TMDMData class
    for(int i=0; i< 4; ++i) {
      m_Event->SetXpos(i, x[i]);
      m_Event->SetYpos(i, y[i]);
    }
  
    ++indx;
  }

  Hits->clear() ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////   
void MDM::InitializeScorers() { 
  // This check is necessary in case the geometry is reloaded
  bool already_exist = false; 
  m_MDMScorer = CheckScorer("MDMScorer",already_exist) ;

  if(already_exist) { return ; }

  G4VPrimitiveScorer* ScorerMDM =
    new MDMScorer("ScorerMDM", "MDM", 0);

  //and register it to the multifunctionnal detector
  m_MDMScorer->RegisterPrimitive(ScorerMDM);

  G4SDManager::GetSDMpointer()->AddNewDetector(m_MDMScorer) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* MDM::Construct(){
  return  (NPS::VDetector*) new MDM();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
  class proxy_nps_MDM{
  public:
    proxy_nps_MDM(){
      NPS::DetectorFactory::getInstance()->AddToken("MDM","MDM");
      NPS::DetectorFactory::getInstance()->AddDetector("MDM",MDM::Construct);
    }
  };

  proxy_nps_MDM p_nps_MDM;
}
