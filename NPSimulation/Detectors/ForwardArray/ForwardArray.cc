/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu *
 *                                                                           *
 * Creation Date  : May 2017                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  ForwardArray simulation                             *
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
#include "ForwardArray.hh"
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
namespace ForwardArray_NS{
    // Energy and time Resolution
    const double EnergyThreshold = 0.1*MeV;
    const double ResoTime = 0.3*ns ;
    const double ResoEnergy = 1.0*MeV ;
    //const double Width = 100*mm ;
    
    const string Material = "BC400";
    const double Rmin=12.70*mm;
    const double Rmax=64.26*mm;
    const double Rmax_new=50.8*mm;
    const double Thickness=4.33*mm;
    const double Phimin=0;
    const double Phimax=18.0;
    const double R_PM=8.25*mm;
    const double PMLength=33*mm;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// ForwardArray Specific Method
ForwardArray::ForwardArray(){
    m_Event = new TForwardArrayData() ;
    m_ForwardArrayScorer = 0;
    m_PieDetector = 0;
    m_PhotoTube = 0;
    
    
    // RGB Color + Transparency
    m_VisSquare = new G4VisAttributes(G4Colour(192, 192, 192, 0.8));
    m_VisPM = new G4VisAttributes(G4Colour(0, 0, 0, 1));
    
}

ForwardArray::~ForwardArray(){
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ForwardArray::AddDetector(G4ThreeVector POS, double Rot){
    // Convert the POS value to R theta Phi as Spherical coordinate is easier in G4
    m_R.push_back(POS.mag());
    m_Theta.push_back(POS.theta());
    m_Phi.push_back(POS.phi());
    m_Rot.push_back(Rot);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ForwardArray::AddDetector(double  R, double  Theta, double  Phi, double Rot){
    m_R.push_back(R);
    m_Theta.push_back(Theta);
    m_Phi.push_back(Phi);
    m_Rot.push_back(Rot);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* ForwardArray::BuildPieVolume(){
    G4Tubs* pie = new G4Tubs("ForwardArray_Cyl",ForwardArray_NS::Rmin,ForwardArray_NS::Rmax,ForwardArray_NS::Thickness*0.5,ForwardArray_NS::Phimin*deg,ForwardArray_NS::Phimax*deg);
        
    G4Material* DetectorMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(ForwardArray_NS::Material);
    m_PieDetector = new G4LogicalVolume(pie,DetectorMaterial,"logic_ForwardArray_pie",0,0,0);
    m_PieDetector->SetVisAttributes(m_VisSquare);
    m_PieDetector->SetSensitiveDetector(m_ForwardArrayScorer);
        
    return m_PieDetector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* ForwardArray::BuildPhotoTube(){
    G4Tubs* PM = new G4Tubs("ForwardArray_PM",0,ForwardArray_NS::R_PM,ForwardArray_NS::PMLength*0.5,0,360*deg);
    
    G4Material* DetectorMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(ForwardArray_NS::Material);
    m_PhotoTube = new G4LogicalVolume(PM,DetectorMaterial,"logic_ForwardArray_PM",0,0,0);
    m_PhotoTube->SetVisAttributes(m_VisPM);
    //m_PhotoTube->SetSensitiveDetector(m_ForwardArrayScorer);
    
    return m_PhotoTube;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* ForwardArray::BuildSmallVolume(){
    G4Tubs* pie = new G4Tubs("ForwardArray_Cyl",ForwardArray_NS::Rmin,ForwardArray_NS::Rmax_new,ForwardArray_NS::Thickness*0.5,ForwardArray_NS::Phimin*deg,ForwardArray_NS::Phimax*2*deg);
    
    G4Material* DetectorMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(ForwardArray_NS::Material);
    m_PieDetector = new G4LogicalVolume(pie,DetectorMaterial,"logic_ForwardArray_pie",0,0,0);
    m_PieDetector->SetVisAttributes(m_VisSquare);
    m_PieDetector->SetSensitiveDetector(m_ForwardArrayScorer);
    
    return m_PieDetector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void ForwardArray::ReadConfiguration(NPL::InputParser parser){
    vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("ForwardArray");
    if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << "//// " << blocks.size() << " detectors found " << endl;
    
    vector<string> cart = {"POS","ROT"};
    vector<string> sphe = {"R","Theta","Phi","ROT"};
    
    for(unsigned int i = 0 ; i < blocks.size() ; i++){
        if(blocks[i]->HasTokenList(cart)){
            if(NPOptionManager::getInstance()->GetVerboseLevel())
                cout << endl << "////  ForwardArray " << i+1 <<  endl;
            
            G4ThreeVector Pos = NPS::ConvertVector(blocks[i]->GetTVector3("POS","mm"));
            double Rot = blocks[i]->GetDouble("ROT","deg");
            AddDetector(Pos,Rot);
        }
        else if(blocks[i]->HasTokenList(sphe)){
            if(NPOptionManager::getInstance()->GetVerboseLevel())
                cout << endl << "////  ForwardArray " << i+1 <<  endl;
            double R = blocks[i]->GetDouble("R","mm");
            double Theta = blocks[i]->GetDouble("Theta","deg");
            double Phi = blocks[i]->GetDouble("Phi","deg");
            double Rot = blocks[i]->GetDouble("ROT","deg");
            AddDetector(R,Theta,Phi,Rot);
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
void ForwardArray::ConstructDetector(G4LogicalVolume* world){
    for (unsigned short i = 0 ; i < m_R.size() ; i++) {
        
        G4double wX = m_R[i] * sin(m_Theta[i] ) * cos(m_Phi[i] ) ;
        G4double wY = m_R[i] * sin(m_Theta[i] ) * sin(m_Phi[i] ) ;
        G4double wZ = m_R[i] * cos(m_Theta[i] ) ;
        G4ThreeVector Det_pos = G4ThreeVector(wX, wY, wZ) ;
        // So the face of the detector is at R instead of the middle
        Det_pos+=Det_pos.unit()*ForwardArray_NS::Thickness*0.5;
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
        
        
        const G4double dangle = 18.*deg;
        G4RotationMatrix* RotPM = new G4RotationMatrix;
        RotPM->rotateY(90*deg);
        RotPM->rotateZ(m_Rot[i]+dangle/2);
        for(int j=0; j<16; j++){
            G4RotationMatrix* Rotz = new G4RotationMatrix;
            Rotz->rotateZ(j*dangle+m_Rot[i]);
            new G4PVPlacement(G4Transform3D(*Rotz,Det_pos),
                              BuildPieVolume(),
                              "ForwardArray",world,false,j+1,false);
            
            
            G4double wXpm = (ForwardArray_NS::Rmax + ForwardArray_NS::PMLength/2) * cos(j*dangle+m_Rot[i]+dangle/2);
            G4double wYpm = (ForwardArray_NS::Rmax + ForwardArray_NS::PMLength/2) * sin(j*dangle+m_Rot[i]+dangle/2);
            G4double wZpm = m_R[i]+ForwardArray_NS::Thickness*0.5;
            G4ThreeVector Det_PM = G4ThreeVector(wXpm, wYpm, wZpm);
            
            if(j>0)RotPM->rotateZ(dangle);
            new G4PVPlacement(G4Transform3D(*RotPM,Det_PM),
                              BuildPhotoTube(),
                              "PhotoTube",world,false,j+1,false);

          
            
        }
        
        G4RotationMatrix* Rotz = new G4RotationMatrix;
        Rotz->rotateZ(16*dangle+m_Rot[i]);
        new G4PVPlacement(G4Transform3D(*Rotz,Det_pos),
                          BuildSmallVolume(),
                          "ForwardArray",world,false,17,false);
        
        /*G4double wXpm = (ForwardArray_NS::Rmax_new + ForwardArray_NS::PMLength/2) * cos(16*dangle+m_Rot[i]+dangle);
        G4double wYpm = (ForwardArray_NS::Rmax_new + ForwardArray_NS::PMLength/2) * sin(16*dangle+m_Rot[i]+dangle);
        G4double wZpm = m_R[i]+ForwardArray_NS::Thickness*0.5;
        G4ThreeVector Det_PM = G4ThreeVector(wXpm, wYpm, wZpm);
        RotPM->rotateZ(dangle+dangle/2);
        new G4PVPlacement(G4Transform3D(*RotPM,Det_PM),
                          BuildPhotoTube(),
                          "PhotoTube",world,false,17,false);*/
        
        
        
        Rotz = new G4RotationMatrix;
        Rotz->rotateZ(18*dangle+m_Rot[i]);
        new G4PVPlacement(G4Transform3D(*Rotz,Det_pos),
                          BuildSmallVolume(),
                          "ForwardArray",world,false,18,false);
        
        /*wXpm = (ForwardArray_NS::Rmax_new + ForwardArray_NS::PMLength/2) * cos(18*dangle+m_Rot[i]+dangle);
        wYpm = (ForwardArray_NS::Rmax_new + ForwardArray_NS::PMLength/2) * sin(18*dangle+m_Rot[i]+dangle);
        wZpm = m_R[i]+ForwardArray_NS::Thickness*0.5;
        Det_PM = G4ThreeVector(wXpm, wYpm, wZpm);
        RotPM->rotateZ(2*dangle);
        new G4PVPlacement(G4Transform3D(*RotPM,Det_PM),
                          BuildPhotoTube(),
                          "PhotoTube",world,false,18,false);*/

        
    }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void ForwardArray::InitializeRootOutput(){
    RootOutput *pAnalysis = RootOutput::getInstance();
    TTree *pTree = pAnalysis->GetTree();
    if(!pTree->FindBranch("ForwardArray")){
        pTree->Branch("ForwardArray", "TForwardArrayData", &m_Event) ;
    }
    pTree->SetBranchAddress("ForwardArray", &m_Event) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void ForwardArray::ReadSensitive(const G4Event* event){
    m_Event->Clear();
    
    ///////////
    // Calorimeter scorer
    NPS::HitsMap<G4double*>* CaloHitMap;
    std::map<G4int, G4double**>::iterator Calo_itr;
    
    G4int CaloCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ForwardArrayScorer/Calorimeter");
    CaloHitMap = (NPS::HitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(CaloCollectionID));
    
    // Loop on the Calo map
    for (Calo_itr = CaloHitMap->GetMap()->begin() ; Calo_itr != CaloHitMap->GetMap()->end() ; Calo_itr++){
        
        G4double* Info = *(Calo_itr->second);
        double Energy = RandGauss::shoot(Info[0],ForwardArray_NS::ResoEnergy);
        if(Energy>ForwardArray_NS::EnergyThreshold){
            double Time = RandGauss::shoot(Info[1],ForwardArray_NS::ResoTime);
            int DetectorNbr = (int) Info[7];
            m_Event->SetEnergy(DetectorNbr,Energy);
            m_Event->SetTime(DetectorNbr,Time);
            
            ms_InterCoord->SetDetectedPositionX(Info[2]) ;
            ms_InterCoord->SetDetectedPositionY(Info[3]) ;
            ms_InterCoord->SetDetectedPositionZ(Info[4]) ;
            ms_InterCoord->SetDetectedAngleTheta(Info[5]/deg) ;
            ms_InterCoord->SetDetectedAnglePhi(Info[6]/deg) ;
        }
    }
    // clear map for next event
    CaloHitMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////
void ForwardArray::InitializeScorers() {
    // This check is necessary in case the geometry is reloaded
    bool already_exist = false;
    m_ForwardArrayScorer = CheckScorer("ForwardArrayScorer",already_exist) ;
    
    if(already_exist)
        return ;
    
    // Otherwise the scorer is initialised
    vector<int> level; level.push_back(0);
    G4VPrimitiveScorer* Calorimeter= new CALORIMETERSCORERS::PS_CalorimeterWithInteraction("Calorimeter",level, 0) ;
    //and register it to the multifunctionnal detector
    m_ForwardArrayScorer->RegisterPrimitive(Calorimeter);
    G4SDManager::GetSDMpointer()->AddNewDetector(m_ForwardArrayScorer) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* ForwardArray::Construct(){
    return  (NPS::VDetector*) new ForwardArray();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
    class proxy_nps_ForwardArray{
    public:
        proxy_nps_ForwardArray(){
            NPS::DetectorFactory::getInstance()->AddToken("ForwardArray","ForwardArray");
            NPS::DetectorFactory::getInstance()->AddDetector("ForwardArray",ForwardArray::Construct);
        }
    };
    
    proxy_nps_ForwardArray p_nps_ForwardArray;
}
