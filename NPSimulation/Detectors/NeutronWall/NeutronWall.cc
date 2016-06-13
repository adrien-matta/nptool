/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
 *                                                                           *
 * Creation Date  : June 2016                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  NeutronWall simulation                             *
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
#include "NeutronWall.hh"
#include "CalorimeterScorers.hh"
#include "RootOutput.h"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace NeutronWall_NS{
    // Energy and time Resolution
    const double EnergyThreshold = 0.1*MeV;
    const double ResoTime = 4.5*ns ;
    const double ResoEnergy = 5.0*MeV ;
    const double NS_X = 1.1*m;
    const double NS_Y = 1.1*m;
    const double NS_Z = 5*cm;
    const double Al_X = 1.1*m;
    const double Al_Y = 1.1*m;
    const double Al_Z = 0.2*m;
    const double NE213_X = 1.0*m;
    const double NE213_Y = 3.66*cm;
    const double NE213_Z = 3.025*cm;
    const double Py_X = 1.006*m;
    const double Py_Y = 3.81*cm;
    const double Py_Z = 3.175*cm;
    
    const string Scintillator = "NE213";
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// NeutronWall Specific Method
NeutronWall::NeutronWall(){
    m_Event = new TNeutronWallData() ;
    m_NeutronWallScorer = 0;
    m_NeutronWall_log = 0;
    m_AlCase_log = 0;
    m_Quartz_log = 0;
    m_QuartzCap_log = 0;
    m_PMTube_log = 0;
    m_NE213_log = 0;
    m_ShadowBar_log = 0;
    
    
    // RGB Color + Transparency
    m_VisNE213 = new G4VisAttributes(G4Colour(0, 1, 0, 0.5));
    m_VisQuartz = new G4VisAttributes(G4Colour(0, 1, 1, 0.5));
    m_VisAl = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.5));
    m_VisNW = new G4VisAttributes(G4Colour(0.4,0.2,0.8,0.5));
}

NeutronWall::~NeutronWall(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void NeutronWall::AddNeutronWall(double  R, double  Theta, double  Phi, double X, double Y, double Z, double Rotation){
    m_R.push_back(R);
    m_Theta.push_back(Theta);
    m_Phi.push_back(Phi);
    m_X.push_back(X);
    m_Y.push_back(Y);
    m_Z.push_back(Z);
    m_Rot.push_back(Rotation);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void NeutronWall::BuildDetector(){


}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void NeutronWall::ReadConfiguration(string Path){
    ifstream ConfigFile           ;
    ConfigFile.open(Path.c_str()) ;
    string LineBuffer          ;
    string DataBuffer          ;
    
    double Theta = 0 , Phi = 0 , R = 0 ;
    double X = 0 , Y = 0 , Z = 0 ;
    double Rot =0;
    
    bool check_Theta = false ;
    bool check_Phi = false ;
    bool check_R = false ;
    bool check_rotation = false ;
    bool check_X = false ;
    bool check_Y = false ;
    bool check_Z = false ;
    bool ReadingStatus = false ;
    
    
    while (!ConfigFile.eof()) {
        getline(ConfigFile, LineBuffer);
        
        //   If line is a Start Up NeutronWall bloc, Reading toggle to true
        string name = "NeutronWall";
        
        if (LineBuffer.compare(0, name.length(), name) == 0) {
            G4cout << "///" << G4endl           ;
            G4cout << "NeutronWall found: " << G4endl   ;
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
                R = R * m;
                G4cout << "R:  " << R/m << G4endl;
            }
            
            //Position method
            else if (DataBuffer.compare(0, 2, "X=") == 0) {
                check_X = true;
                ConfigFile >> DataBuffer ;
                X = atof(DataBuffer.c_str()) ;
                X = X * cm;
                G4cout << "X:  " << X / cm << G4endl;
            }
            
            else if (DataBuffer.compare(0, 2, "Y=") == 0) {
                check_Y = true;
                ConfigFile >> DataBuffer ;
                Y = atof(DataBuffer.c_str()) ;
                Y = Y * cm;
                G4cout << "Y:  " << Y / cm << G4endl;
            }
            
            else if (DataBuffer.compare(0, 2, "Z=") == 0) {
                check_Z = true;
                ConfigFile >> DataBuffer ;
                Z = atof(DataBuffer.c_str()) ;
                Z = Z * cm;
                G4cout << "Z:  " << Z / cm << G4endl;
            }
            
            
            //General
            else if (DataBuffer.compare(0, 4, "Rot=") == 0) {
                check_rotation = true;
                ConfigFile >> DataBuffer ;
                Rot = atof(DataBuffer.c_str());
                Rot = Rot*deg ;
                G4cout << "Rotation:  " << Rot/deg << G4endl;
            }
            
            ///////////////////////////////////////////////////
            //   If no Detector Token and no comment, toggle out
            else{
                ReadingStatus = false;
                G4cout << "Wrong Token Sequence: Getting out " << DataBuffer << G4endl ;
            }
            
            /////////////////////////////////////////////////
            //   If All necessary information there, toggle out
            
            if (( check_Theta && check_Phi && check_R)
                ||
                ( check_X && check_Y && check_Z && check_rotation)){
                
                
                // Convert Cartesian to Spherical (detector always face the target)
                if (check_X){
                    R = sqrt (X*X+Y*Y+Z*Z);
                    Theta = acos(Z / (R) );
                    Phi = atan2(Y,X);
                }
                
                AddNeutronWall(R,Theta,Phi,X,Y,Z,Rot);
                
                //   Reinitialisation of Check Boolean
                check_Theta = false ;
                check_Phi = false ;
                check_R = false ;
                check_rotation = false ;
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
void NeutronWall::ConstructDetector(G4LogicalVolume* world){
    
    for (unsigned short i = 0 ; i < m_R.size() ; i++) {
        
        G4double wX = m_R[i] * sin(m_Theta[i] ) * cos(m_Phi[i] ) ;
        G4double wY = m_R[i] * sin(m_Theta[i] ) * sin(m_Phi[i] ) ;
        G4double wZ = m_R[i] * cos(m_Theta[i] ) ;
        G4ThreeVector Det_pos = G4ThreeVector(wX, wY, wZ) ;
        // So the face of the detector is at R instead of the middle
        Det_pos+=Det_pos.unit()*NeutronWall_NS::NS_Z*0.5;
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
        
        G4Material* ScintMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(NeutronWall_NS::Scintillator);
        G4Material* vacuum = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
        G4Material* Aluminum = MaterialManager::getInstance()->GetMaterialFromLibrary("Al");
        G4Material* Pyrex = MaterialManager::getInstance()->GetMaterialFromLibrary("Pyrex");
        
        //Neutron Wall Box
        G4Box* NeutronWall_box = new G4Box("NeutronWall_Box",NeutronWall_NS::NS_X*0.5,
                                           NeutronWall_NS::NS_Y*0.5,NeutronWall_NS::NS_Z*0.5);
        m_NeutronWall_log = new G4LogicalVolume(NeutronWall_box,vacuum,"NeutronWall_Log",0,0,0);
        m_NeutronWall_log->SetVisAttributes(m_VisNW);
        
        //Aluminum case
        G4Box* AlCase_box = new G4Box("AlCase_Box",NeutronWall_NS::Al_X*0.5,NeutronWall_NS::Al_Y*0.5,NeutronWall_NS::Al_Z*0.5);
        m_AlCase_log = new G4LogicalVolume(AlCase_box,Aluminum,"AlCase_Log",0,0,0);
        
        m_AlCase_log->SetVisAttributes(m_VisAl);
        
        //Quartz tube volume
        G4Box* Quartz_box = new G4Box("Quartz_Box",NeutronWall_NS::Py_X*0.5,NeutronWall_NS::Py_Y*0.5,NeutronWall_NS::Py_Z*0.5);
        m_Quartz_log = new G4LogicalVolume(Quartz_box,Pyrex,"Quartz_Log",0,0,0);
        
        m_Quartz_log->SetVisAttributes(m_VisQuartz);
        
        G4Tubs* QuartzCap_tube = new G4Tubs("QuartsCap_Tube",0,7.99*cm,0.3175*cm,0.0*deg,360.0*deg);
        m_QuartzCap_log = new G4LogicalVolume(QuartzCap_tube,Aluminum,"QuartzCap_Log",0,0,0);
        
        //NE213 Volume
        G4Box* NE213_box = new G4Box("NE213_Box",NeutronWall_NS::NE213_X*0.5,NeutronWall_NS::NE213_Y*0.5,NeutronWall_NS::NE213_Z*0.5);
        m_NE213_log = new G4LogicalVolume(NE213_box,ScintMaterial,"NE213_Log",0,0,0);
        
        m_NE213_log->SetVisAttributes(m_VisNE213);
        m_NE213_log->SetSensitiveDetector(m_NeutronWallScorer);
        
        //Shadow bar construction
        G4Trd* ShadowBar_trd = new G4Trd("ShadowBar_Trd",6.51/2*cm, 7.23/2*cm, 6.79/2*cm, 7.66/2*cm, 30./2*cm);
        m_ShadowBar_log = new G4LogicalVolume(ShadowBar_trd, Aluminum, "ShadowBar_Log");
        
        
        //******************* Placement *******************//
        m_NE213Tube_phys = new G4PVPlacement(0,G4ThreeVector(0,0,0),
                                             m_NE213_log,
                                             "NE213Tube_phys",
                                             m_Quartz_log,
                                             false,
                                             0);
        for (int j = 0; j < 12; j++ ) {
            G4double detector_y = (j-6)*8.60*cm;
            m_Quartz_phys = new G4PVPlacement(0,G4ThreeVector(0,detector_y,0),m_Quartz_log,
                                            "Quartz_phys",m_NeutronWall_log,false,j);
        }
        
        //----Aluminum Case----
        m_AlCase_phys = new G4PVPlacement(0,G4ThreeVector(0,0, 4.2*cm),m_AlCase_log,
                                          "AlCase_phys",m_NeutronWall_log,false,0);
        
        
        /****************** Place the walls*************************/
        m_NeutronWall_phys = new G4PVPlacement(G4Transform3D(*Rot, Det_pos),
                                                m_NeutronWall_log,
                                                "NeutronWall_phys",world,false,i);


    }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void NeutronWall::InitializeRootOutput(){
    RootOutput *pAnalysis = RootOutput::getInstance();
    TTree *pTree = pAnalysis->GetTree();
    pTree->Branch("NeutronWall", "TNeutronWallData", &m_Event) ;
    pTree->SetBranchAddress("NeutronWall", &m_Event) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void NeutronWall::ReadSensitive(const G4Event* event){
    m_Event->Clear();
    
    ///////////
    // Calorimeter scorer
    G4THitsMap<G4double*>* CaloHitMap;
    std::map<G4int, G4double**>::iterator Calo_itr;
    
    G4int CaloCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("NeutronWallScorer/Calorimeter");
    CaloHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(CaloCollectionID));
    
    // Loop on the Calo map
    for (Calo_itr = CaloHitMap->GetMap()->begin() ; Calo_itr != CaloHitMap->GetMap()->end() ; Calo_itr++){
        
        G4double* Info = *(Calo_itr->second);
        //(Info[0]/2.35)*((Info[0]*1.02)*pow((Info[0]*1.8),.5))
        // double Energy = RandGauss::shoot(Info[0],((Info[0]*1000*1.02/2.35)*pow((Info[0]*1000*1.8),.5)) );
        double Energy = RandGauss::shoot(Info[0],NeutronWall_NS::ResoEnergy);
        if(Energy>NeutronWall_NS::EnergyThreshold){
            double Time = RandGauss::shoot(Info[1],NeutronWall_NS::ResoTime);
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
void NeutronWall::InitializeScorers() {
    // This check is necessary in case the geometry is reloaded
    bool already_exist = false;
    m_NeutronWallScorer = CheckScorer("NeutronWallScorer",already_exist) ;
    
    if(already_exist)
        return ;
    
    // Otherwise the scorer is initialised
    vector<int> level; level.push_back(0);
    G4VPrimitiveScorer* Calorimeter= new CALORIMETERSCORERS::PS_Calorimeter("Calorimeter",level, 0) ;
    //and register it to the multifunctionnal detector
    m_NeutronWallScorer->RegisterPrimitive(Calorimeter);
    G4SDManager::GetSDMpointer()->AddNewDetector(m_NeutronWallScorer) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* NeutronWall::Construct(){
    return  (NPS::VDetector*) new NeutronWall();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
    class proxy_nps_plastic{
    public:
        proxy_nps_plastic(){
            NPS::DetectorFactory::getInstance()->AddToken("NeutronWall","NeutronWall");
            NPS::DetectorFactory::getInstance()->AddDetector("NeutronWall",NeutronWall::Construct);
        }
    };
    
    proxy_nps_plastic p_nps_plastic;
}
