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
 *  This class describe  AGATA simulation                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *   AGATA geometry is based on SToGS GDML file by Olivier Stezowski         *
 *   More info on this package at https://github.com/stezow/stogs            *
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
#include "NPOptionManager.h"
// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace AGATA_NS{
  // Energy and time Resolution
  const double EnergyThreshold = 1*keV;
  const double ResoTime = 4.5*ns ;
  const double ResoEnergy = 2*keV ;
  const double ResoAngle = 5*deg;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// AGATA Specific Method
AGATA::AGATA(){
  m_Event = new TAGATAData() ;
  m_AGATAScorer = 0;
  m_TripleCluster= 0;
}

AGATA::~AGATA(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void AGATA::AddAGATA(double  R, double  Theta, double  Phi){
  m_R.push_back(R);
  m_Theta.push_back(Theta);
  m_Phi.push_back(Phi);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4AssemblyVolume* AGATA::BuildTripleCluster(){
  if(!m_TripleCluster){

    cout << "AGATA geometry is based on SToGS GDML file by Olivier Stezowski"<< endl
         << "More info on this package at https://github.com/stezow/stogs"<<endl;
    
    m_TripleCluster = new G4AssemblyVolume;
    string basepath = getenv("NPTOOL");
    string path=basepath+"/NPSimulation/Detectors/AGATA/gdml/ATC.gdml";
    m_gdmlparser.Read(path);

    G4VisAttributes* RedVisAtt = new G4VisAttributes(G4Colour(1, 0, 0,0.5)) ;
    G4VisAttributes* GreenVisAtt = new G4VisAttributes(G4Colour(0, 1, 0,0.5)) ;
    G4VisAttributes* BlueVisAtt = new G4VisAttributes(G4Colour(0, 0, 1,0.5)) ;

    G4LogicalVolume* RedCrystal= m_gdmlparser.GetVolume("ARedCapsuleLV");
    G4LogicalVolume* GreenCrystal= m_gdmlparser.GetVolume("BGreenCapsuleLV");
    G4LogicalVolume* BlueCrystal= m_gdmlparser.GetVolume("CBlueCapsuleLV");

    RedCrystal->SetVisAttributes(RedVisAtt);
    GreenCrystal->SetVisAttributes(GreenVisAtt);
    BlueCrystal->SetVisAttributes(BlueVisAtt);


    G4LogicalVolume* ATC =  m_gdmlparser.GetVolume("ATC");

    G4ThreeVector Pos_Red= ATC->GetDaughter(0)->GetTranslation();
    G4RotationMatrix* Rot_Red =new G4RotationMatrix( (ATC->GetDaughter(0)->GetObjectRotationValue()));
    ATC->GetDaughter(0)->GetLogicalVolume()->GetDaughter(0)->GetLogicalVolume()->GetDaughter(0)->GetLogicalVolume()->SetSensitiveDetector(m_AGATAScorer);
    m_TripleCluster->AddPlacedVolume(RedCrystal,Pos_Red, Rot_Red);

    G4ThreeVector Pos_Green= ATC->GetDaughter(1)->GetTranslation();
    G4RotationMatrix* Rot_Green =new G4RotationMatrix( (ATC->GetDaughter(1)->GetObjectRotationValue()));
    ATC->GetDaughter(1)->GetLogicalVolume()->GetDaughter(0)->GetLogicalVolume()->GetDaughter(0)->GetLogicalVolume()->SetSensitiveDetector(m_AGATAScorer);
    m_TripleCluster->AddPlacedVolume(GreenCrystal,Pos_Green, Rot_Green);
    
    G4ThreeVector Pos_Blue= ATC->GetDaughter(2)->GetTranslation();
    G4RotationMatrix* Rot_Blue =new G4RotationMatrix( (ATC->GetDaughter(2)->GetObjectRotationValue()));
    ATC->GetDaughter(2)->GetLogicalVolume()->GetDaughter(0)->GetLogicalVolume()->GetDaughter(0)->GetLogicalVolume()->SetSensitiveDetector(m_AGATAScorer);
    m_TripleCluster->AddPlacedVolume(BlueCrystal,Pos_Blue, Rot_Blue);
  }
  return m_TripleCluster;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void AGATA::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("AGATA");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> token = {"R","Theta","Phi"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("Theta","deg");
      double Phi = blocks[i]->GetDouble("Phi","deg");
      
      // To be done
      AddAGATA(R,Theta,Phi);
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
void AGATA::ConstructDetector(G4LogicalVolume* world){
  for (unsigned short i = 0 ; i < m_R.size() ; i++) {

    G4double wX = m_R[i] * sin(m_Theta[i] ) * cos(m_Phi[i] ) ;
    G4double wY = m_R[i] * sin(m_Theta[i] ) * sin(m_Phi[i] ) ;
    G4double wZ = m_R[i] * cos(m_Theta[i] ) ;
    G4ThreeVector Det_pos = G4ThreeVector(wX, wY, wZ+85) ;
    // So the face of the detector is at R instead of the middle
    Det_pos+=Det_pos.unit();
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
    if(m_Theta[i]>90*deg)
      Rot->rotate(-72*deg,w);
    else
      Rot->rotate(72*deg,w);

    BuildTripleCluster();
    G4Transform3D Trans(*Rot,Det_pos);
    m_TripleCluster->MakeImprint(world,Trans,i+1);
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void AGATA::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("AGATA")){
    pTree->Branch("AGATA", "TAGATAData", &m_Event) ;
  }
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

  G4int CaloCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("AGATAScorer/Crystal");
  CaloHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(CaloCollectionID));

  // Loop on the Calo map
  for (Calo_itr = CaloHitMap->GetMap()->begin() ; Calo_itr != CaloHitMap->GetMap()->end() ; Calo_itr++){

    G4double* Info = *(Calo_itr->second);
    //(Info[0]/2.35)*((Info[0]*1.02)*pow((Info[0]*1.8),.5))
    // double Energy = RandGauss::shoot(Info[0],((Info[0]*1000*1.02/2.35)*pow((Info[0]*1000*1.8),.5)) );
    double Energy = RandGauss::shoot(Info[0],AGATA_NS::ResoEnergy);
    if(Energy>AGATA_NS::EnergyThreshold){
      double Time = RandGauss::shoot(Info[1],AGATA_NS::ResoTime);
      int DetectorNbr = (int) Info[7];
      double Angle = RandGauss::shoot(Info[5]/deg,AGATA_NS::ResoAngle);
      m_Event->SetEnergy(DetectorNbr,Energy);
      m_Event->SetAngle(DetectorNbr,Angle);
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
  vector<int> level; level.push_back(1);
  G4VPrimitiveScorer* Calorimeter= new CALORIMETERSCORERS::PS_CalorimeterWithInteraction("Crystal",level, 0) ;
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
