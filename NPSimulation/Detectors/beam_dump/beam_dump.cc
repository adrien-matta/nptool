/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: valerian  contact address: girardalcindor@ganil.fr                        *
 *                                                                           *
 * Creation Date  : November 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  beam_dump simulation                             *
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
#include "beam_dump.hh"
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
namespace beam_dump_NS{
    // Energy and time Resolution
    const double EnergyThreshold = 0.0*MeV;
    const double ResoTime = 0.0*ns ;
    const double ResoEnergy = 0.0*MeV ;
    const double Radius = 20*cm ; 
    const double Width = 10*cm ;
    // const double Thickness = 34*um ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// beam_dump Specific Method
beam_dump::beam_dump(){
    m_Event = new Tbeam_dumpData() ;
    m_beam_dumpScorer = 0;
    m_SquareDetector = 0;

    // RGB Color + Transparency
    m_VisSquare = new G4VisAttributes(G4Colour(1, 0, 0, 0.5));   
}

beam_dump::~beam_dump(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void beam_dump::AddDetector(double  R, double  Theta, double  Phi, double Thickness){
    m_R.push_back(R);
    m_Theta.push_back(Theta);
    m_Phi.push_back(Phi);
    m_Thickness.push_back(Thickness);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* beam_dump::BuildSquareDetector(){
    if(!m_SquareDetector){
        G4Box* box = new G4Box("beam_dump_Box",
                20*cm*0.5,
                20*cm*0.5, 
                m_Thickness[0]*0.5);

        G4Material* DetectorMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary("Ta");
        m_SquareDetector = new G4LogicalVolume(box,DetectorMaterial,"logic_beam_dump_Box",0,0,0);
        m_SquareDetector->SetVisAttributes(m_VisSquare);
        m_SquareDetector->SetSensitiveDetector(m_beam_dumpScorer);
    }
    return m_SquareDetector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void beam_dump::ReadConfiguration(NPL::InputParser parser){
    vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("beam_dump");
    if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << "//// " << blocks.size() << " detectors found " << endl; 

    vector<string> sphe = {"R","Theta","Phi","Thickness"};

    for(unsigned int i = 0 ; i < blocks.size() ; i++){
        if(blocks[i]->HasTokenList(sphe)){
            if(NPOptionManager::getInstance()->GetVerboseLevel())
                cout << endl << "////  beam_dump " << i+1 <<  endl;
            double R = blocks[i]->GetDouble("R","mm");
            double Theta = blocks[i]->GetDouble("Theta","deg");
            double Phi = blocks[i]->GetDouble("Phi","deg");
            double Thickness = blocks[i]->GetDouble("Thickness", "micrometer");
            AddDetector(R,Theta,Phi,Thickness);
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
void beam_dump::ConstructDetector(G4LogicalVolume* world){
    for (unsigned short i = 0 ; i < m_R.size() ; i++) {

        G4double wX = m_R[i] * sin(m_Theta[i] ) * cos(m_Phi[i] ) ;
        G4double wY = m_R[i] * sin(m_Theta[i] ) * sin(m_Phi[i] ) ;
        G4double wZ = m_R[i] * cos(m_Theta[i] ) ;
        G4ThreeVector Det_pos = G4ThreeVector(wX, wY, wZ) ;
        // So the face of the detector is at R instead of the middle
        Det_pos+=Det_pos.unit()*m_Thickness[0];
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

        new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
                BuildSquareDetector(),
                "beam_dump",world,false,i+1);
    }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void beam_dump::InitializeRootOutput(){
    RootOutput *pAnalysis = RootOutput::getInstance();
    TTree *pTree = pAnalysis->GetTree();
    if(!pTree->FindBranch("beam_dump")){
        pTree->Branch("beam_dump", "Tbeam_dumpData", &m_Event) ;
    }
    pTree->SetBranchAddress("beam_dump", &m_Event) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void beam_dump::ReadSensitive(const G4Event* event){
    m_Event->Clear();

    ///////////
    // Calorimeter scorer
    NPS::HitsMap<G4double*>* CaloHitMap;
    std::map<G4int, G4double**>::iterator Calo_itr;

    G4int CaloCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("beam_dumpScorer/Calorimeter");
    CaloHitMap = (NPS::HitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(CaloCollectionID));

    // Loop on the Calo map
    for (Calo_itr = CaloHitMap->GetMap()->begin() ; Calo_itr != CaloHitMap->GetMap()->end() ; Calo_itr++){

        G4double* Info = *(Calo_itr->second);
        double Energy = RandGauss::shoot(Info[0],beam_dump_NS::ResoEnergy);
        if(Energy>beam_dump_NS::EnergyThreshold){
            double Time = RandGauss::shoot(Info[1],beam_dump_NS::ResoTime);
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
void beam_dump::InitializeScorers() { 
    // This check is necessary in case the geometry is reloaded
    bool already_exist = false; 
    m_beam_dumpScorer = CheckScorer("beam_dumpScorer",already_exist) ;

    if(already_exist) 
        return ;

    // Otherwise the scorer is initialised
    vector<int> level; level.push_back(0);
    G4VPrimitiveScorer* Calorimeter= new CalorimeterScorers::PS_Calorimeter("Calorimeter",level, 0) ;
    //and register it to the multifunctionnal detector
    m_beam_dumpScorer->RegisterPrimitive(Calorimeter);
    G4SDManager::GetSDMpointer()->AddNewDetector(m_beam_dumpScorer) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* beam_dump::Construct(){
    return  (NPS::VDetector*) new beam_dump();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
    class proxy_nps_beam_dump{
        public:
            proxy_nps_beam_dump(){
                NPS::DetectorFactory::getInstance()->AddToken("beam_dump","beam_dump");
                NPS::DetectorFactory::getInstance()->AddDetector("beam_dump",beam_dump::Construct);
            }
    };

    proxy_nps_beam_dump p_nps_beam_dump;
}
