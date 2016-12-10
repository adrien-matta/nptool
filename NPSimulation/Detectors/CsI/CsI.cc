    /*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : Avril 2015                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe a Modular cylindrical CsI Scintillator           *
 *   Few Material are instantiate and user can choose position and dimension    * 
 *  but also the adding of a lead plate on the rear side of the detector     *
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
#include "CsI.hh"
#include "ObsoleteGeneralScorers.hh"
#include "RootOutput.h"
#include "MaterialManager.hh"
#include "SiliconScorers.hh"
#include "PhotoDiodeScorers.hh"
#include "CalorimeterScorers.hh"
#include "NPSDetectorFactory.hh"
#include "NPOptionManager.h"
//using namespace OBSOLETEGENERALSCORERS ;
// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// CsI Specific Method
CsI::CsI(){
    m_Event = new TCsIData() ;
    m_CsIScorer = 0;
    ResoCsI = 2.5/2.35;// 2.5% FWHM
    PhotoDiodeFace = 18.;//mm
    PhotoDiodeThickness = 3.;//mm
}

CsI::~CsI(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void CsI::AddCsI(   G4double  R                       ,
    G4double  Theta                   ,
    G4double  Phi                  ,
    G4double   CsIThickness   ,
    G4double   CsIRadius         ,
    G4String    Scintillator         ,
    G4double    LeadThickness         )
{

  m_R.push_back(R)                                         ;
  m_Theta.push_back(Theta)                                ;
  m_Phi.push_back(Phi)                                     ;
  m_CsIThickness.push_back(CsIThickness)   ;
  m_LeadThickness.push_back(LeadThickness)            ;
  m_Scintillator.push_back(Scintillator)               ;
  m_CsIRadius.push_back(CsIRadius)            ; // cylindrical shape
  m_CsIFaceFront.push_back(-1)                              ; // Trapezoidal shape
  m_CsIFaceBack.push_back(-1)                              ; // Trapezoidal shape
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void CsI::AddCsI(   G4double R                      ,
    G4double Theta                ,
    G4double Phi                     ,
    G4double FaceFront                  ,
    G4double FaceBack                  ,
    G4double CsIThickness   ,
    G4String Scintillator         ,
    G4double LeadThickness      )
{
  m_R.push_back(R)                                         ;
  m_Theta.push_back(Theta)                                ;
  m_Phi.push_back(Phi)                                     ;
  m_CsIThickness.push_back(CsIThickness)   ;
  m_LeadThickness.push_back(LeadThickness)            ;
  m_Scintillator.push_back(Scintillator)               ;
  m_CsIRadius.push_back(-1)            ; // cylindrical shape
  m_CsIFaceFront.push_back(FaceFront)                        ; // Trapezoidal shape
  m_CsIFaceBack.push_back(FaceBack)                           ; // Trapezoidal shape

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of VDetector class


// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void CsI::ReadConfiguration(NPL::InputParser parser ){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("CsI");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> cart = {"X","Y","Z"};
  vector<string> sphe = {"R","Theta","Phi"};
  vector<string> trapez= {"Shape","FaceFront","FaceBack","Thickness","Scintillator","LeadThickness"};
  vector<string> cylind= {"Shape","Radius","Thickness","Scintillator","LeadThickness"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Plastic " << i+1 <<  endl;
      double X = blocks[i]->GetDouble("X","mm");
      double Y = blocks[i]->GetDouble("Y","mm");
      double Z = blocks[i]->GetDouble("Z","mm");
      double R = sqrt (X*X+Y*Y+Z*Z);
      double Theta = acos(Z / (R) );
      double Phi = atan2(Y,X);

      if(blocks[i]->HasTokenList(trapez)){
        string Shape = blocks[i]->GetString("Shape");
        double FaceFront = blocks[i]->GetDouble("FaceFront","mm");
        double FaceBack = blocks[i]->GetDouble("FaceBack","mm");
        double Thickness = blocks[i]->GetDouble("Thickness","mm");
        string Scintillator= blocks[i]->GetString("Scintillator");
        double LeadThickness = blocks[i]->GetDouble("LeadThickness","mm");
        AddCsI(R,Theta,Phi,FaceFront,FaceBack,Thickness,Scintillator,LeadThickness);
      }
      
      else if(blocks[i]->HasTokenList(cylind)){
        string Shape = blocks[i]->GetString("Shape");
        double Radius = blocks[i]->GetDouble("Radius","mm");
        double Thickness = blocks[i]->GetDouble("Thickness","mm");
        string Scintillator= blocks[i]->GetString("Scintillator");
        double LeadThickness = blocks[i]->GetDouble("LeadThickness","mm");
        AddCsI(R,Theta,Phi,Thickness,Radius,Scintillator,LeadThickness);
      }

    }
    else if(blocks[i]->HasTokenList(sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Plastic " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("Theta","deg");
      double Phi = blocks[i]->GetDouble("Phi","deg");

      if(blocks[i]->HasTokenList(trapez)){
        string Shape = blocks[i]->GetString("Shape");
        double FaceFront = blocks[i]->GetDouble("FaceFront","mm");
        double FaceBack = blocks[i]->GetDouble("FaceBack","mm");
        double Thickness = blocks[i]->GetDouble("Thickness","mm");
        string Scintillator = blocks[i]->GetString("Scintillator");
        double LeadThickness = blocks[i]->GetDouble("LeadThickness","mm");
        AddCsI(R,Theta,Phi,FaceFront,FaceBack,Thickness,Scintillator,LeadThickness);
      }
      
      else if(blocks[i]->HasTokenList(cylind)){
        string Shape = blocks[i]->GetString("Shape");
        double Radius = blocks[i]->GetDouble("Radius","mm");
        double Thickness = blocks[i]->GetDouble("Thickness","mm");
        string Scintillator = blocks[i]->GetString("Scintillator");
        double LeadThickness = blocks[i]->GetDouble("LeadThickness","mm");
        AddCsI(R,Theta,Phi,Thickness,Radius,Scintillator,LeadThickness);
      }

    }


    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
}

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void CsI::ConstructDetector(G4LogicalVolume* world){
  G4ThreeVector Det_pos = G4ThreeVector(0, 0, 0)  ;

  for (unsigned short i = 0 ; i < m_R.size() ; i++) {
    G4double wX = m_R[i] * sin(m_Theta[i] ) * cos(m_Phi[i] ) ;
    G4double wY = m_R[i] * sin(m_Theta[i] ) * sin(m_Phi[i] ) ;
    G4double wZ = m_R[i] * cos(m_Theta[i] ) ;
    Det_pos = G4ThreeVector(wX, wY, wZ) ;
    VolumeMaker(Det_pos , i+1, world) ;
  }

}

void CsI::VolumeMaker(G4ThreeVector Det_pos, int DetNumber, G4LogicalVolume* world){
  ////////////////////////////////////////////////////////////////
  ////////////// Starting Volume Definition //////////////////////
  ////////////////////////////////////////////////////////////////      
  // Name of the module
  std::ostringstream DetectorNumber ;
  DetectorNumber << DetNumber ;
  G4String Name = "CsI" + DetectorNumber.str() ;

  int i = DetNumber-1;

    G4Material* CsIMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(m_Scintillator[i]) ;
    G4Material* Vacuum = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum") ;
    
    //Create experimental hall
    G4double expHall_x = 1.*m;
    G4double expHall_y = 1.*m;
    G4double expHall_z = 1.*m;
    
    G4Box* fExperimentalHall_box = new G4Box("expHall_box",expHall_x,expHall_y,expHall_z);
    G4LogicalVolume* fExperimentalHall_log = new G4LogicalVolume(fExperimentalHall_box,
                                                Vacuum,"expHall_log",0,0,0);
    G4VPhysicalVolume* fExperimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(),
                                               fExperimentalHall_log,"expHall",0,false,0);
    
    fExperimentalHall_log->SetVisAttributes(G4VisAttributes::Invisible);


  // Definition of the volume containing the sensitive detector

  // Cylindrical Case
  if(m_CsIRadius[i]!=-1){
    if(m_CsIThickness[i]>0 && m_CsIRadius[i]>0){
        
        // CsI crystal
        G4Tubs* solidCsI = new G4Tubs( Name ,
                                      0 ,
                                      m_CsIRadius[i] ,
                                      m_CsIThickness[i]/2 ,
                                      0*deg ,
                                      360*deg);

        G4LogicalVolume* logicCsI = new G4LogicalVolume(solidCsI, CsIMaterial, Name+ "_Scintillator", 0, 0, 0);
        logicCsI->SetSensitiveDetector(m_CsIScorer);

        G4VisAttributes* CsIVisAtt = new G4VisAttributes(G4Colour(1.0, 0.5, 0.0,0.25)) ;
        logicCsI->SetVisAttributes(CsIVisAtt) ;

        G4VPhysicalVolume* physCsI = new G4PVPlacement(0 ,
                                                       Det_pos ,
                                                       logicCsI ,
                                                       Name  + "_Scintillator" ,
                                                       world ,
                                                       false ,
                                                       0 );
        
        G4OpticalSurface* OpticalCrysralSurface = new G4OpticalSurface("CrystalSurface");
        OpticalCrysralSurface->SetType(dielectric_metal);
        //polished: smooth perfectly polished surcface
        //ground: rough surface
        OpticalCrysralSurface->SetFinish(polished);
        //unified
        //glisur
        OpticalCrysralSurface->SetModel(glisur);
        
        G4double pp[] = {0.1*eV, 10*eV};
        const G4int num = sizeof(pp)/sizeof(G4double);
        G4double reflectivity[] = {0., 0.};
        assert(sizeof(reflectivity) == sizeof(pp));
        G4double efficiency[] = {1., 1.};
        assert(sizeof(efficiency) == sizeof(pp));
        
        G4MaterialPropertiesTable* OpticalCrysralSurfaceProperty = new G4MaterialPropertiesTable();
        
        OpticalCrysralSurfaceProperty->AddProperty("REFLECTIVITY",pp,reflectivity,num);
        OpticalCrysralSurfaceProperty->AddProperty("EFFICIENCY",pp,efficiency,num);
        OpticalCrysralSurface->SetMaterialPropertiesTable(OpticalCrysralSurfaceProperty);
        
        new G4LogicalBorderSurface("CrystalSurface", physCsI, fExperimentalHall_phys, OpticalCrysralSurface);
        //new G4LogicalSkinSurface("OpticalCrysralSurface",logicCsI,OpticalCrysralSurface);
        
        // Photodiode
        G4String NamePD = Name+"PhotoDiode";
        
        G4Material* PDMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary("Si");

        G4Box* solidPhotoDiode = new G4Box(NamePD,0.5*PhotoDiodeFace,0.5*PhotoDiodeFace,0.5*PhotoDiodeThickness);
        
        G4LogicalVolume* logicPD = new G4LogicalVolume(solidPhotoDiode, PDMaterial, NamePD,0,0,0);
        logicPD->SetSensitiveDetector(m_PDScorer);
        
        G4VisAttributes* PDVisAtt = new G4VisAttributes(G4Colour(0.1, 0.2, 0.3)) ;
        logicPD->SetVisAttributes(PDVisAtt);
        
        new G4PVPlacement(0 ,
                         Det_pos+(m_CsIThickness[i]*0.5+PhotoDiodeThickness*0.5)*Det_pos.unit() ,
                         logicPD ,
                         NamePD ,
                         world ,
                         false ,
                         0 );
    }

    if(m_LeadThickness[i]>0&& m_CsIRadius[i]>0){
      G4Tubs* solidLead = new G4Tubs(Name+"_Lead",    
          0,
          m_CsIRadius[i],
          m_LeadThickness[i]/2,
          0*deg, 
          360*deg);

      G4Material* MaterialLead = MaterialManager::getInstance()->GetMaterialFromLibrary("Pb");
      G4LogicalVolume* logicLead = new G4LogicalVolume(solidLead, MaterialLead, Name+"_Lead", 0, 0, 0);//AC changed lead to Al
      G4VisAttributes* LeadVisAtt = new G4VisAttributes(G4Colour(0.1, 0.1, 0.1)) ;
      logicLead->SetVisAttributes(LeadVisAtt) ;

      G4PVPlacement( 0,
          Det_pos+(m_CsIThickness[i]/2+m_LeadThickness[i]/2)*Det_pos.unit(),
          logicLead,
          Name+"_Lead",   
          world,
          false,
          0);
    }   
  }

  // Trapezoidal case
  if(m_CsIFaceFront[i]!=-1){
    if(m_CsIThickness[i]>0 && m_CsIFaceFront[i]>0 && m_CsIFaceBack[i]>0){ 
    
	G4Trd* solidCsI = new G4Trd("solidCsICrystal",0.5*m_CsIFaceFront[i],0.5*m_CsIFaceBack[i],0.5*m_CsIFaceFront[i],0.5*m_CsIFaceBack[i],0.5*m_CsIThickness[i]);
	
      //G4Box* solidCsI = new G4Box(Name, 0.5*m_CsIFaceBack[i], 0.5*m_CsIFaceFront[i], 0.5*m_CsIThickness[i]);
      G4LogicalVolume* logicCsI = new G4LogicalVolume(solidCsI, CsIMaterial, Name+ "_Scintillator", 0, 0, 0);
      logicCsI->SetSensitiveDetector(m_CsIScorer);

      G4VisAttributes* PlastVisAtt = new G4VisAttributes(G4Colour(1.0, 0.5, 0.0)) ;
      logicCsI->SetVisAttributes(PlastVisAtt) ;

      G4RotationMatrix Rot3D;
      Rot3D.set(0, 0, 0);
        
      G4VPhysicalVolume* physCsI = new G4PVPlacement(  G4Transform3D(Rot3D,Det_pos),
          logicCsI,
          Name  + "_Scintillator" ,
          world,
          false,
          0);
        
        G4OpticalSurface* OpticalCrysralSurface = new G4OpticalSurface("CrystalSurface");
        OpticalCrysralSurface->SetType(dielectric_metal);
        //polished: smooth perfectly polished surcface
        //ground: rough surface
        OpticalCrysralSurface->SetFinish(polished);
        //unified
        //glisur
        OpticalCrysralSurface->SetModel(glisur);
        
        G4double pp[] = {0.1*eV, 10*eV};
        const G4int num = sizeof(pp)/sizeof(G4double);
        G4double reflectivity[] = {0., 0.};
        assert(sizeof(reflectivity) == sizeof(pp));
        G4double efficiency[] = {1., 1.};
        assert(sizeof(efficiency) == sizeof(pp));
        
        G4MaterialPropertiesTable* OpticalCrysralSurfaceProperty = new G4MaterialPropertiesTable();
        
        OpticalCrysralSurfaceProperty->AddProperty("REFLECTIVITY",pp,reflectivity,num);
        OpticalCrysralSurfaceProperty->AddProperty("EFFICIENCY",pp,efficiency,num);
        OpticalCrysralSurface->SetMaterialPropertiesTable(OpticalCrysralSurfaceProperty);
        
        new G4LogicalBorderSurface("CrystalSurface", physCsI, fExperimentalHall_phys, OpticalCrysralSurface);
        //new G4LogicalSkinSurface("OpticalCrysralSurface",logicCsI,OpticalCrysralSurface);
        
        // Photodiode
        /*G4String NamePD = Name+"PhotoDiode";
        
        G4Material* PDMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary("Si");
        
        G4Box* solidPhotoDiode = new G4Box(NamePD,0.5*PhotoDiodeFace,0.5*PhotoDiodeFace,0.5*PhotoDiodeThickness);
        
        G4LogicalVolume* logicPD = new G4LogicalVolume(solidPhotoDiode, PDMaterial, NamePD,0,0,0);
        logicPD->SetSensitiveDetector(m_PDScorer);
        
        G4VisAttributes* PDVisAtt = new G4VisAttributes(G4Colour(0.1, 0.2, 0.3)) ;
        logicPD->SetVisAttributes(PDVisAtt);
        
        new G4PVPlacement(0 ,
                          Det_pos+(m_CsIThickness[i]*0.5+PhotoDiodeThickness*0.5)*Det_pos.unit() ,
                          logicPD ,
                          NamePD ,
                          world ,
                          false ,
                          0 );*/

    }

    if(m_LeadThickness[i]>0&& m_CsIFaceFront[i]>0 && m_CsIFaceBack[i]>0){
      G4Box* solidLead = new G4Box(Name+"_Lead", 0.5*m_CsIFaceBack[i], 0.5*m_CsIFaceFront[i], 0.5*m_LeadThickness[i]);

      G4Material* MaterialLead = MaterialManager::getInstance()->GetMaterialFromLibrary("Pb");
      G4LogicalVolume* logicLead = new G4LogicalVolume(solidLead, MaterialLead, Name+"_Lead", 0, 0, 0);
      G4VisAttributes* LeadVisAtt = new G4VisAttributes(G4Colour(0.1, 0.1, 0.1)) ;
      logicLead->SetVisAttributes(LeadVisAtt) ;

      new G4PVPlacement(0,
          Det_pos+(m_CsIThickness[i]/2+m_LeadThickness[i]/2)*Det_pos.unit() +G4ThreeVector(0,0,-10*cm)  ,
          logicLead,
          Name+"_Lead",   
          world,
          false,
          0);
    }   
  }
}

// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void CsI::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("CsI")){
    pTree->Branch("CsI", "TCsIData", &m_Event) ;
  }
  pTree->SetBranchAddress("CsI", &m_Event) ;
}

// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void CsI::ReadSensitive(const G4Event* event){
    //G4String DetectorNumber;
    m_Event->Clear();
    
    // CsI //
    G4THitsMap<G4double*>* CsIHitMap;
    std::map<G4int, G4double**>::iterator CsI_itr;
    G4int CsICollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("CsIScorer/CsI");
    CsIHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(CsICollectionID));
    
    // Loop on the CsI map
    for (CsI_itr = CsIHitMap->GetMap()->begin() ; CsI_itr != CsIHitMap->GetMap()->end() ; CsI_itr++){
        G4double* Info = *(CsI_itr->second);
        double E_CsI = RandGauss::shoot(Info[0],Info[0]*ResoCsI/100);
        //cout << "Energy CsI " << endl;
        //cout << E_CsI << endl;
        m_Event->SetCsIEEnergy(E_CsI);
        m_Event->SetENumber(Info[7]);
        
        ms_InterCoord->SetDetectedPositionX(Info[2]) ;
        ms_InterCoord->SetDetectedPositionY(Info[3]) ;
        ms_InterCoord->SetDetectedPositionZ(Info[4]) ;
        ms_InterCoord->SetDetectedAngleTheta(Info[5]/deg) ;
        ms_InterCoord->SetDetectedAnglePhi(Info[6]/deg) ;
    }
    // Clear Map for next event
    CsIHitMap->clear();
    
    // PhotoDiode //
    G4THitsMap<G4double*>* PhotoDiodeHitMap;
    std::map<G4int, G4double**>::iterator PhotoDiode_itr;
    
    G4int PhotoDiodeCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("PDScorer/PhotoDiode");
    PhotoDiodeHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(PhotoDiodeCollectionID));
    
    // Loop on the PhotoDiode map
    vector<double> NumberOfOpticalPhoton;
    NumberOfOpticalPhoton.clear();
    for (PhotoDiode_itr = PhotoDiodeHitMap->GetMap()->begin() ; PhotoDiode_itr != PhotoDiodeHitMap->GetMap()->end() ; PhotoDiode_itr++){
        G4double* Info = *(PhotoDiode_itr->second);
        
        m_Event->SetPhotoDiodeEnergy(Info[0]);
        m_Event->SetPhotoDiodeEDetectorNbr(Info[7]);
            
        m_Event->SetPhotoDiodeTime(Info[1]);
        m_Event->SetPhotoDiodeTDetectorNbr(Info[7]);
        NumberOfOpticalPhoton.push_back(Info[8]);
        //cout << "CsI class Info[8] = " << Info[8] << endl;;
        //m_Event->SetNumberOfOpticalPhoton(Info[8]);
    }
    //cout << "CsI class total optical photon = " << NumberOfOpticalPhoton.size() << endl;
    m_Event->SetNumberOfOpticalPhoton(NumberOfOpticalPhoton.size());
    PhotoDiodeHitMap->clear();

}


////////////////////////////////////////////////////////////////   
void CsI::InitializeScorers() { 
    bool already_exist = false;
    vector<G4int> NestingLevel;
    NestingLevel.push_back(0);
    NestingLevel.push_back(1);
    m_CsIScorer = CheckScorer("CsIScorer",already_exist) ;
    m_PDScorer = CheckScorer("PDScorer",already_exist) ;
  
    if(already_exist) return ;
    
    G4VPrimitiveScorer* CsIScorer= new CALORIMETERSCORERS::PS_CalorimeterWithInteraction("CsI",NestingLevel);
    m_CsIScorer->RegisterPrimitive(CsIScorer);
    
    G4VPrimitiveScorer* PDScorer = new PHOTODIODESCORERS::PS_PhotoDiode_Rectangle("PhotoDiode",0,
                                                                                PhotoDiodeFace,
                                                                                PhotoDiodeFace,
                                                                                1,
                                                                                1);
    m_PDScorer->RegisterPrimitive(PDScorer);
    
    G4SDManager::GetSDMpointer()->AddNewDetector(m_PDScorer) ;
    G4SDManager::GetSDMpointer()->AddNewDetector(m_CsIScorer) ;


    /*G4VPrimitiveScorer* DetNbr = new PSDetectorNumber("CsINumber","CsI", 0) ;
    G4VPrimitiveScorer* Energy = new PSEnergy("Energy","CsI", 0)                   ;
    G4VPrimitiveScorer* Time   = new PSTOF("Time","CsI", 0)                         ;
    G4VPrimitiveScorer* InteractionCoordinatesZ  			= new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesZ("InterCoordZ","CsI", 0);
    //and register it to the multifunctionnal detector
    m_CsIScorer->RegisterPrimitive(DetNbr)                         ;
    m_CsIScorer->RegisterPrimitive(Energy)                         ;
    m_CsIScorer->RegisterPrimitive(Time)                            ;
    m_CsIScorer->RegisterPrimitive(InteractionCoordinatesZ);
    G4SDManager::GetSDMpointer()->AddNewDetector(m_CsIScorer) ;
    G4SDManager::GetSDMpointer()->AddNewDetector(m_PDScorer) ;*/

}
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* CsI::Construct(){
    return  (NPS::VDetector*) new CsI();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
    class proxy_nps_csi{
    public:
        proxy_nps_csi(){
            NPS::DetectorFactory::getInstance()->AddToken("CsI","CsI");
            NPS::DetectorFactory::getInstance()->AddDetector("CsI",CsI::Construct);
        }
    };
    
    proxy_nps_csi p_nps_csi;
}
