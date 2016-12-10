/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre MORFOUACE  contact address:                       *
 *                                                                           *
 * Creation Date  : February 2015                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe the HiRA telescope                                   *
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
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4UnionSolid.hh"
#include "G4ExtrudedSolid.hh"
#include "G4TwoVector.hh"
//G4 sensitive
#include "G4SDManager.hh"

//G4 various object
#include "G4Material.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4Colour.hh"
#include "G4PVDivision.hh"
#include "G4SubtractionSolid.hh"

// NPS
#include "Lassa.hh"
#include "MaterialManager.hh"
#include "SiliconScorers.hh"
#include "CalorimeterScorers.hh"

// NPL
#include "NPOptionManager.h"
#include "RootOutput.h"
#include "NPSDetectorFactory.hh"
//using namespace OBSOLETEGENERALSCORERS ;

using namespace LASSA;

// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Lassa::Lassa(){
    InitializeMaterial();
    m_EventLassa = new TLassaData();
    
    // Dark Grey
    m_SiliconVisAtt = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3)) ;
    
    m_CsIVisAtt = new G4VisAttributes(G4Colour(0.529412, 0.807843, 0.980392, 0.95)) ;
    //m_CsIVisAtt->SetForceWireframe(true);
    m_LogicThickSi = 0;
    m_LogicCsICrystal = 0;
    m_LogicCluster = 0;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Lassa::~Lassa(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of VDetector class
// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Lassa::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("LassaTelescope");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " Telescope found " << endl; 

  // Cartesian Case
  vector<string> cart = {"A","B","C","D"};
  for(unsigned int i  = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Lassa Telescope " << i+1 <<  endl;
      G4ThreeVector A = NPS::ConvertVector(blocks[i]->GetTVector3("A","mm"));
      G4ThreeVector B = NPS::ConvertVector(blocks[i]->GetTVector3("B","mm"));
      G4ThreeVector C = NPS::ConvertVector(blocks[i]->GetTVector3("C","mm"));
      G4ThreeVector D = NPS::ConvertVector(blocks[i]->GetTVector3("D","mm"));
      AddTelescope(A,B,C,D) ;
    }

    else{
      cout << "ERROR: Missing token for M2Telescope blocks, check your input file" << endl;
      exit(1);
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Lassa::AddTelescope(G4ThreeVector Pos1, G4ThreeVector Pos2, G4ThreeVector Pos3, G4ThreeVector Pos4){
    G4ThreeVector Pos=(Pos1+Pos2+Pos3+Pos4)/4.;
    G4ThreeVector u = Pos4-Pos2;
    G4ThreeVector w = Pos.unit();
    G4ThreeVector v = w.cross(u);
    u = u.unit(); v = v.unit(); w = w.unit();
    Pos = Pos + w*Length*0.5;
    
    m_Type.push_back(1);
    m_Pos.push_back(Pos);
    m_Rot.push_back(new G4RotationMatrix(u,v,w));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Lassa::ConstructDetector(G4LogicalVolume* world){
    unsigned int mysize = m_Pos.size();
    
    for(unsigned int i = 0 ; i < mysize ; i++){
        VolumeMaker(i+1, m_Pos[i], m_Rot[i], world);
    }
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Lassa::ReadSensitive(const G4Event* event){
    m_EventLassa->Clear();
    
    // ThickSi //
    G4THitsMap<G4double*>* ThickSiHitMap;
    std::map<G4int, G4double**>::iterator ThickSi_itr;
    G4int ThickSiCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Lassa_ThickSiScorer/ThickSi");
    ThickSiHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(ThickSiCollectionID));
    
    // Loop on the ThickSi map
    for (ThickSi_itr = ThickSiHitMap->GetMap()->begin() ; ThickSi_itr != ThickSiHitMap->GetMap()->end() ; ThickSi_itr++){
        G4double* Info = *(ThickSi_itr->second);
        double E_ThickSi = RandGauss::shoot(Info[0],ResoThickSi);
        if(E_ThickSi>EnergyThreshold){
            m_EventLassa->SetLassaThickSiStripXEEnergy(E_ThickSi);
            m_EventLassa->SetLassaThickSiStripXEDetectorNbr(Info[7]-1);
            m_EventLassa->SetLassaThickSiStripXEStripNbr(Info[8]-1);
            
            m_EventLassa->SetLassaThickSiStripXTTime(Info[1]);
            m_EventLassa->SetLassaThickSiStripXTDetectorNbr(Info[7]-1);
            m_EventLassa->SetLassaThickSiStripXTStripNbr(Info[8]-1);
            
            m_EventLassa->SetLassaThickSiStripYEEnergy(E_ThickSi);
            m_EventLassa->SetLassaThickSiStripYEDetectorNbr(Info[7]-1);
            m_EventLassa->SetLassaThickSiStripYEStripNbr(Info[9]-1);
            
            m_EventLassa->SetLassaThickSiStripYTTime(Info[1]);
            m_EventLassa->SetLassaThickSiStripYTDetectorNbr(Info[7]-1);
            m_EventLassa->SetLassaThickSiStripYTStripNbr(Info[9]-1);
            
            // Interraction Coordinates
            ms_InterCoord->SetDetectedPositionX(Info[2]) ;
            ms_InterCoord->SetDetectedPositionY(Info[3]) ;
            ms_InterCoord->SetDetectedPositionZ(Info[4]) ;
            ms_InterCoord->SetDetectedAngleTheta(Info[5]/deg) ;
            ms_InterCoord->SetDetectedAnglePhi(Info[6]/deg) ;
        }
    }
    // Clear Map for next event
    ThickSiHitMap->clear();
    
    // CsI //
    G4THitsMap<G4double*>* CsIHitMap;
    std::map<G4int, G4double**>::iterator CsI_itr;
    G4int CsICollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Lassa_CsIScorer/CsI");
    CsIHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(CsICollectionID));
    
    // Loop on the CsI map
    for (CsI_itr = CsIHitMap->GetMap()->begin() ; CsI_itr != CsIHitMap->GetMap()->end() ; CsI_itr++){
        G4double* Info = *(CsI_itr->second);
        double E_CsI = RandGauss::shoot(Info[0],ResoCsI);
        if(E_CsI>EnergyThreshold){
            m_EventLassa->SetLassaCsIEEnergy(E_CsI);
            m_EventLassa->SetLassaCsIEDetectorNbr((int)Info[3]-1);
            m_EventLassa->SetLassaCsIECristalNbr((int)Info[2]-1);
        }
    }
    // Clear Map for next event
    CsIHitMap->clear();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Lassa::InitializeScorers(){
    //Look for previous definition of the scorer (geometry reload)
    bool already_exist = false;
    vector<G4int> NestingLevel;
    NestingLevel.push_back(0);
    NestingLevel.push_back(2);
    
    m_ThickSiStripScorer = CheckScorer("Lassa_ThickSiScorer",already_exist);
    m_CsIScorer = CheckScorer("Lassa_CsIScorer",already_exist);
    
    // if the scorer were created previously nothing else need to be made
    if(already_exist) return;
    
    G4VPrimitiveScorer* ThickSiScorer = new SILICONSCORERS::PS_Silicon_Rectangle("ThickSi",0,
                                                                                 SiliconFace,
                                                                                 SiliconFace,
                                                                                 NumberOfStrip,
                                                                                 NumberOfStrip);
    
    m_ThickSiStripScorer->RegisterPrimitive(ThickSiScorer);
    
    G4VPrimitiveScorer* CsIScorer= new CALORIMETERSCORERS::PS_Calorimeter("CsI",NestingLevel);
    
    m_CsIScorer->RegisterPrimitive(CsIScorer);
    
    //   Add All Scorer to the Global Scorer Manager
    G4SDManager::GetSDMpointer()->AddNewDetector(m_ThickSiStripScorer) ;
    G4SDManager::GetSDMpointer()->AddNewDetector(m_CsIScorer) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Lassa::InitializeRootOutput(){
    TTree *pTree = RootOutput::getInstance()->GetTree();
    if(!pTree->FindBranch("Lassa")){
      pTree->Branch("Lassa", "TLassaData", &m_EventLassa) ;
    } 
    // This insure that the object are correctly bind in case of
    // a redifinition of the geometry in the simulation
    pTree->SetBranchAddress("Lassa", &m_EventLassa) ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Lassa::VolumeMaker(G4int DetectorNumber,
                        G4ThreeVector MMpos,
                        G4RotationMatrix* MMrot,
                        G4LogicalVolume* world)
{
    G4double NbrTelescopes = DetectorNumber  ;
    G4String DetNumber                   ;
   	ostringstream Number                      ;
    Number << NbrTelescopes                   ;
   	DetNumber = Number.str()             ;
    
    
    ////////////////////////////////////////////////////////////////
   	////////////// Starting Volume Definition //////////////////////
   	////////////////////////////////////////////////////////////////
   	G4String Name = "Lassa" + DetNumber ;
    
   	// Definition of the volume containing the sensitive detector
   	G4Box*           solidMotherVolume = new G4Box(Name, 0.5*FaceFront, 0.5*FaceBack, 0.5*Length);
    m_logicMotherVolume = new G4LogicalVolume(solidMotherVolume, m_MaterialVacuum, Name, 0, 0, 0);
    
   	new G4PVPlacement(G4Transform3D(*MMrot, MMpos), m_logicMotherVolume, Name, world, false, DetectorNumber);
    
   	G4VisAttributes* MotherVolumeVisAtt = new G4VisAttributes(G4Colour(0.90, 0.90, 0.90));
  	 MotherVolumeVisAtt->SetForceWireframe(true);
   	m_logicMotherVolume->SetVisAttributes(G4VisAttributes::Invisible);
    
    ///////////////////////////////////////////////////
    ///////////////////// Thick Si ////////////////////
    ///////////////////////////////////////////////////
    G4String NameThickSi = "ThickSi"+DetNumber;
    //if(!m_LogicThickSi){
    if(0==0){
        G4Box* solidThickSi = new G4Box(NameThickSi, 0.5*SiliconFace, 0.5*SiliconFace, 0.5*ThickSiThickness);
        m_LogicThickSi = new G4LogicalVolume(solidThickSi, m_MaterialSilicon, "logicThickSi", 0, 0, 0);
        
        G4ThreeVector positionThickSi = G4ThreeVector(0, 0, ThickSi_PosZ);
        
        new G4PVPlacement(new G4RotationMatrix(0,0,0),
                          positionThickSi,
                          m_LogicThickSi,"ThickSi",
                          m_logicMotherVolume,false,DetectorNumber);
        
        // Set ThickSi sensible
        m_LogicThickSi->SetSensitiveDetector(m_ThickSiStripScorer);
        
        // Visualisation of ThickSi
        m_LogicThickSi->SetVisAttributes(m_SiliconVisAtt) ;
    }
    
    ///////////////////////////////////////////////////
    /////////////////////// CsI ///////////////////////
    ///////////////////////////////////////////////////
    G4String NameCsI = "CsI"+DetNumber;
    
    double X1 = (CsIXFront-CsIXBack)/2.;
    double Y1 = (CsIYFront-CsIYBack)/2.;
    double l = sqrt(pow(X1,2) + pow(Y1,2));
    
    double pDz = 0.5*CsIThickness;
    double pTheta = -atan( (l)/(CsIThickness) );
    double pPhi = atan( X1/Y1 );
    double pDy1 = 0.5*CsIYFront;
    double pDx1 = 0.5*CsIXFront;
    double pDx2 = 0.5*CsIXFront;
    double pAlp1 = 0;
    double pDy2 = 0.5*CsIYBack;
    double pDx3 = 0.5*CsIXBack;
    double pDx4 = 0.5*CsIXBack;
    double pAlp2 = 0;
    
    G4Trap* solidCsIStage = new G4Trap(NameCsI, pDz, pTheta, pPhi, pDy1, pDx1, pDx2, pAlp1, pDy2, pDx3, pDx4, pAlp2);
    
    
    m_LogicCsICrystal = new G4LogicalVolume(solidCsIStage, m_MaterialCsI, "logicCsICrystal", 0, 0, 0);
    
    // Set CsI sensible
    m_LogicCsICrystal->SetSensitiveDetector(m_CsIScorer);
    
    // Visualisation of CsI
   	m_LogicCsICrystal->SetVisAttributes(m_CsIVisAtt);
    
    //if(!m_LogicCluster){
    if(0==0){
        // Sub Mother Volume
        G4Trd* solidCluster = new G4Trd("SolidCluster", 0.5*ClusterFaceFront,0.5*ClusterFaceBack,0.5*ClusterFaceFront,0.5*ClusterFaceBack, 0.5*CsIThickness);
        m_LogicCluster = new G4LogicalVolume(solidCluster, m_MaterialVacuum, "LogicSolidCluster", 0, 0, 0);
        m_LogicCluster->SetVisAttributes(G4VisAttributes::Invisible);
        
        G4ThreeVector FramePos(0,0,CsI_PosZ);
        
        new G4PVPlacement(new G4RotationMatrix(0,0,0),
                          FramePos,
                          m_LogicCluster,"Cluster",
                          m_logicMotherVolume,false,0);
        
        //const G4double CsIXMiddle = CsIXFront + (CsIThickness/2)*tan(-pTheta)*sin(pPhi);
        //const G4double CsIYMiddle = CsIYFront + (CsIThickness/2)*tan(-pTheta)*cos(pPhi);
        //double alpha = atan((CsIXBack-CsIXFront)/CsIThickness);
        //double dl = (CsIThickness/2)*tan(alpha);
        double dl = (CsIXBack-CsIXFront)/4;
        const G4double CsIXMiddle = 0.5*CsIXFront + dl;
        const G4double CsIYMiddle = 0.5*CsIYFront + dl;
        const G4double DistInterCsIX = CsIXMiddle+0.5*DistInterCsI;
        const G4double DistInterCsIY = CsIYMiddle+0.5*DistInterCsI;
        
        G4ThreeVector Origin(-DistInterCsIX,-DistInterCsIY,0);
        G4ThreeVector Pos;
        const G4double dangle = 90.*deg;
        // A cluster is a 2 by 2 aggregat of CsI crystal
        unsigned int CsINbr = 1;
        for(unsigned int i = 0 ; i < 2 ; i++){
            for(unsigned int j = 0 ; j < 2 ; j++){
                G4RotationMatrix* rotM = new G4RotationMatrix;
                unsigned int CrystalNbr = CsINbr++;
                if(i==0 && j==0)rotM->rotateZ(0);
                if(i==1 && j==0)rotM->rotateZ(dangle);
                if(i==0 && j==1)rotM->rotateZ(-dangle);
                if(i==1 && j==1)rotM->rotateZ(2*dangle);
                
                if(i==0 && j==0) Pos = Origin;
                if(i==1 && j==0) Pos = G4ThreeVector((DistInterCsIX),-(DistInterCsIY),0);
                if(i==0 && j==1) Pos = G4ThreeVector(-(DistInterCsIX),(DistInterCsIY),0);
                if(i==1 && j==1) Pos = G4ThreeVector((DistInterCsIX),(DistInterCsIY),0);
                //Pos = Origin + G4ThreeVector(i*DistInterCsIX,j*DistInterCsIY,0);
                
                new G4PVPlacement(G4Transform3D(*rotM,Pos),
                                  m_LogicCsICrystal,
                                  "CsI_Cristal",
                                  m_LogicCluster,
                                  false,
                                  CrystalNbr, true);
                delete rotM;
            }
        }
        
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Lassa::InitializeMaterial(){
    m_MaterialSilicon = MaterialManager::getInstance()->GetMaterialFromLibrary("Si");
    m_MaterialAluminium = MaterialManager::getInstance()->GetMaterialFromLibrary("Al");
    m_MaterialCsI = MaterialManager::getInstance()->GetMaterialFromLibrary("CsI");
    m_MaterialMylar = MaterialManager::getInstance()->GetMaterialFromLibrary("Mylar");
    m_MaterialPCB = MaterialManager::getInstance()->GetMaterialFromLibrary("PCB");
    m_MaterialVacuum = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* Lassa::Construct(){
    return  (NPS::VDetector*) new Lassa();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
    class proxy_nps_lassa{
    public:
        proxy_nps_lassa(){
            NPS::DetectorFactory::getInstance()->AddToken("LASSAArray","Lassa");
            NPS::DetectorFactory::getInstance()->AddDetector("LASSAArray",Lassa::Construct);
        }
    };
    
    proxy_nps_lassa p_nps_lassa;
}



