/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
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
#include "Hira.hh"
#include "MaterialManager.hh"
#include "SiliconScorers.hh"
#include "CalorimeterScorers.hh"

// NPL
#include "NPOptionManager.h"
#include "RootOutput.h"
#include "NPSDetectorFactory.hh"
//using namespace OBSOLETEGENERALSCORERS ;

using namespace HIRA;

// CLHEP header
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Hira::Hira(){
  InitializeMaterial();
  m_EventHira = new THiraData();
  
  // Dark Grey
  m_SiliconVisAtt = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3)) ;
  // Green
  m_CsIVisAtt = new G4VisAttributes(G4Colour(0.2, 0.5, 0.2)) ;
    //m_CsIVisAtt->SetForceWireframe(true);
  m_LogicThinSi = 0;
  m_LogicThickSi = 0;
  m_LogicCsICrystal = 0;
  m_LogicCluster = 0;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Hira::~Hira(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of VDetector class
// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Hira::ReadConfiguration(string Path){
  
   ifstream ConfigFile           ;
   ConfigFile.open(Path.c_str()) ;
   string LineBuffer          ;
   string DataBuffer          ;

   // A:X1_Y1     --> X:1    Y:1
   // B:X128_Y1   --> X:128  Y:1
   // C:X1_Y128   --> X:1    Y:128
   // D:X128_Y128    --> X:128  Y:128

   G4double Ax , Bx , Cx , Dx , Ay , By , Cy , Dy , Az , Bz , Cz , Dz          ;
   G4ThreeVector A , B , C , D                                                 ;
   //G4double Theta = 0 , Phi = 0 , R = 0                     ;

   bool ReadingStatus = false ;

   bool check_A = false ;
   bool check_C = false ;
   bool check_B = false ;
   bool check_D = false ;

   /*bool check_Theta = false ;
   bool check_Phi   = false ;
   bool check_R     = false ;*/
   


   while (!ConfigFile.eof()) {
      getline(ConfigFile, LineBuffer);
      if (LineBuffer.compare(0, 13, "HiraTelescope") == 0) {
         G4cout << "///" << G4endl           ;
         G4cout << "Hira element found: " << G4endl   ;
         ReadingStatus = true ;
         }
         
   while(ReadingStatus){      

         ConfigFile >> DataBuffer;
         //   Comment Line 
      if (DataBuffer.compare(0, 1, "%") == 0) {/*do nothing */;}
      
         // Position method
         else if (DataBuffer.compare(0, 2, "A=") == 0) {
            check_A = true;
            ConfigFile >> DataBuffer ;
            Ax = atof(DataBuffer.c_str()) ;
            Ax = Ax * mm ;
            ConfigFile >> DataBuffer ;
            Ay = atof(DataBuffer.c_str()) ;
            Ay = Ay * mm ;
            ConfigFile >> DataBuffer ;
            Az = atof(DataBuffer.c_str()) ;
            Az = Az * mm ;

            A = G4ThreeVector(Ax, Ay, Az);
            G4cout << "A corner position : " << A << G4endl;
         }
        
         else if (DataBuffer.compare(0, 2, "B=") == 0) {
            check_B = true;
            ConfigFile >> DataBuffer ;
            Bx = atof(DataBuffer.c_str()) ;
            Bx = Bx * mm ;
            ConfigFile >> DataBuffer ;
            By = atof(DataBuffer.c_str()) ;
            By = By * mm ;
            ConfigFile >> DataBuffer ;
            Bz = atof(DataBuffer.c_str()) ;
            Bz = Bz * mm ;

            B = G4ThreeVector(Bx, By, Bz);
            G4cout << "B corner position : " << B << G4endl;
         }
         
         else if (DataBuffer.compare(0, 2, "C=") == 0) {
            check_C = true;
            ConfigFile >> DataBuffer ;
            Cx = atof(DataBuffer.c_str()) ;
            Cx = Cx * mm ;
            ConfigFile >> DataBuffer ;
            Cy = atof(DataBuffer.c_str()) ;
            Cy = Cy * mm ;
            ConfigFile >> DataBuffer ;
            Cz = atof(DataBuffer.c_str()) ;
            Cz = Cz * mm ;

            C = G4ThreeVector(Cx, Cy, Cz);
            G4cout << "C corner position : " << C << G4endl;
         }
        
         else if (DataBuffer.compare(0, 2, "D=") == 0) {
            check_D = true;
            ConfigFile >> DataBuffer ;
            Dx = atof(DataBuffer.c_str()) ;
            Dx = Dx * mm ;
            ConfigFile >> DataBuffer ;
            Dy = atof(DataBuffer.c_str()) ;
            Dy = Dy * mm ;
            ConfigFile >> DataBuffer ;
            Dz = atof(DataBuffer.c_str()) ;
            Dz = Dz * mm ;

            D = G4ThreeVector(Dx, Dy, Dz);
            G4cout << "D corner position : " << D << G4endl;
         }
         

       // Angle method
      /*   else if (DataBuffer.compare(0, 6, "THETA=") == 0) {
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
            R = R * mm;
            G4cout << "R:  " << R / mm << G4endl;
         }

         else if (DataBuffer.compare(0, 5, "BETA=") == 0) {
            ConfigFile >> DataBuffer ;
            beta_u = atof(DataBuffer.c_str()) ;
            beta_u = beta_u * deg   ;
            ConfigFile >> DataBuffer ;
            beta_v = atof(DataBuffer.c_str()) ;
            beta_v = beta_v * deg   ;
            ConfigFile >> DataBuffer ;
            beta_w = atof(DataBuffer.c_str()) ;
            beta_w = beta_w * deg   ;
            G4cout << "Beta:  " << beta_u / deg << " " << beta_v / deg << " " << beta_w / deg << G4endl  ;
         }*/

         
         else G4cout << "WARNING: Wrong Token, GaspardTrackerRectangle: Rectangle Element not added" << G4endl;

         //Add The previously define telescope
         //With position method
         if (check_A && check_B && check_C && check_D) {
         
          ReadingStatus = false ;
          check_A = false ;
          check_C = false ;
          check_B = false ;
          check_D = false ;

            AddTelescope(A                ,
                      	 B                ,
                         C                ,
                         D);
         }

         //with angle method
        /*if ((check_Theta && check_Phi && check_R && check_FirstStage && check_SecondStage && check_ThirdStage && checkVis) && !(check_A && check_B && check_C && check_D)) {
            ReadingStatus = false ;
             check_Theta = false ;
             check_Phi   = false ;
             check_R     = false ;
           check_FirstStage = false ;
          check_SecondStage = false ;
           check_ThirdStage = false ;
           checkVis = false ;
           
            AddModule(R                ,
                      Theta            ,
                      Phi              ,
                      beta_u           ,
                      beta_v           ,
                      beta_w           ,
                      FIRSTSTAGE  == 1 ,
                      SECONDSTAGE == 1 ,
                      THIRDSTAGE  == 1);
         }*/

         
      }
   }
}
       



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Hira::AddTelescope(G4ThreeVector Pos1, G4ThreeVector Pos2, G4ThreeVector Pos3, G4ThreeVector Pos4){
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
void Hira::ConstructDetector(G4LogicalVolume* world){
	unsigned int mysize = m_Pos.size();
	 
	for(unsigned int i = 0 ; i < mysize ; i++){
		VolumeMaker(i + 1, m_Pos[i], m_Rot[i], world);
	}
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Hira::ReadSensitive(const G4Event* event){
  m_EventHira->Clear();
	
  // ThinSi //
  G4THitsMap<G4double*>* ThinSiHitMap;
  std::map<G4int, G4double**>::iterator ThinSi_itr;

  G4int ThinSiCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Hira_ThinSiScorer/ThinSi");
  ThinSiHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(ThinSiCollectionID));

  // Loop on the ThinSi map 
  for (ThinSi_itr = ThinSiHitMap->GetMap()->begin() ; ThinSi_itr != ThinSiHitMap->GetMap()->end() ; ThinSi_itr++){
    G4double* Info = *(ThinSi_itr->second); 
    double E_ThinSi = RandGauss::shoot(Info[0],ResoThinSi);

    if(E_ThinSi>EnergyThreshold){
        m_EventHira->SetHiraThinSiStripEEnergy(E_ThinSi);
      	m_EventHira->SetHiraThinSiStripEDetectorNbr(Info[7]);
        m_EventHira->SetHiraThinSiStripEStripNbr(Info[8]);
	
        m_EventHira->SetHiraThinSiStripTTime(Info[1]);
        m_EventHira->SetHiraThinSiStripTDetectorNbr(Info[7]);
        m_EventHira->SetHiraThinSiStripTStripNbr(Info[8]);
    }
  }

  // Clear Map for next event
  ThinSiHitMap->clear();

  // ThickSi //
  G4THitsMap<G4double*>* ThickSiHitMap;
  std::map<G4int, G4double**>::iterator ThickSi_itr;
  G4int ThickSiCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Hira_ThickSiScorer/ThickSi");
  ThickSiHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(ThickSiCollectionID));

  // Loop on the ThickSi map 
  for (ThickSi_itr = ThickSiHitMap->GetMap()->begin() ; ThickSi_itr != ThickSiHitMap->GetMap()->end() ; ThickSi_itr++){
    G4double* Info = *(ThickSi_itr->second); 
    double E_ThickSi = RandGauss::shoot(Info[0],ResoThickSi);
    if(E_ThickSi>EnergyThreshold){
        m_EventHira->SetHiraThickSiStripXEEnergy(E_ThickSi);
        m_EventHira->SetHiraThickSiStripXEDetectorNbr(Info[7]);
        m_EventHira->SetHiraThickSiStripXEStripNbr(Info[8]);
	
        m_EventHira->SetHiraThickSiStripXTTime(Info[1]);
        m_EventHira->SetHiraThickSiStripXTDetectorNbr(Info[7]);
        m_EventHira->SetHiraThickSiStripXTStripNbr(Info[8]);

        m_EventHira->SetHiraThickSiStripYEEnergy(E_ThickSi);
        m_EventHira->SetHiraThickSiStripYEDetectorNbr(Info[7]);
        m_EventHira->SetHiraThickSiStripYEStripNbr(Info[9]);
	
        m_EventHira->SetHiraThickSiStripYTTime(Info[1]);
        m_EventHira->SetHiraThickSiStripYTDetectorNbr(Info[7]);
        m_EventHira->SetHiraThickSiStripYTStripNbr(Info[9]);
        
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
  G4int CsICollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("Hira_CsIScorer/CsI");
  CsIHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(CsICollectionID));

  // Loop on the CsI map 
  for (CsI_itr = CsIHitMap->GetMap()->begin() ; CsI_itr != CsIHitMap->GetMap()->end() ; CsI_itr++){
    G4double* Info = *(CsI_itr->second); 
    double E_CsI = RandGauss::shoot(Info[0],ResoCsI);
    if(E_CsI>EnergyThreshold){
        m_EventHira->SetHiraCsIEEnergy(E_CsI);
      	m_EventHira->SetHiraCsIEDetectorNbr((int)Info[3]);
        m_EventHira->SetHiraCsIECristalNbr((int)Info[2]);
    }
  }
  // Clear Map for next event
  CsIHitMap->clear();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Hira::InitializeScorers(){
  //Look for previous definition of the scorer (geometry reload)
  bool already_exist = false;
  vector<G4int> NestingLevel;
  NestingLevel.push_back(0);
  NestingLevel.push_back(2);

  m_ThinSiStripScorer = CheckScorer("Hira_ThinSiScorer",already_exist);
  m_ThickSiStripScorer = CheckScorer("Hira_ThickSiScorer",already_exist);
  m_CsIScorer = CheckScorer("Hira_CsIScorer",already_exist);

  // if the scorer were created previously nothing else need to be made
  if(already_exist) return;

  G4VPrimitiveScorer* ThinSiScorer = new SILICONSCORERS::PS_Silicon_Rectangle("ThinSi",0,
      SiliconFace,
      SiliconFace,
      NumberOfStrip,
      1);

  m_ThinSiStripScorer->RegisterPrimitive(ThinSiScorer);

  G4VPrimitiveScorer* ThickSiScorer = new SILICONSCORERS::PS_Silicon_Rectangle("ThickSi",0,
      SiliconFace,
      SiliconFace,
      NumberOfStrip,
      NumberOfStrip);

  m_ThickSiStripScorer->RegisterPrimitive(ThickSiScorer);

  G4VPrimitiveScorer* CsIScorer= new CALORIMETERSCORERS::PS_Calorimeter("CsI",NestingLevel);

  m_CsIScorer->RegisterPrimitive(CsIScorer);

  //   Add All Scorer to the Global Scorer Manager 
  G4SDManager::GetSDMpointer()->AddNewDetector(m_ThinSiStripScorer) ;
  G4SDManager::GetSDMpointer()->AddNewDetector(m_ThickSiStripScorer) ;
  G4SDManager::GetSDMpointer()->AddNewDetector(m_CsIScorer) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Hira::InitializeRootOutput(){
  TTree *pTree = RootOutput::getInstance()->GetTree();   
  pTree->Branch("Hira", "THiraData", &m_EventHira) ;
 
  // This insure that the object are correctly bind in case of 
  // a redifinition of the geometry in the simulation
  pTree->SetBranchAddress("Hira", &m_EventHira) ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Hira::VolumeMaker(G4int DetectorNumber,
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
   	G4String Name = "Hira" + DetNumber ;

   	// Definition of the volume containing the sensitive detector
   	G4Box*           solidMotherVolume = new G4Box(Name, 0.5*FaceFront, 0.5*FaceBack, 0.5*Length);
    m_logicMotherVolume = new G4LogicalVolume(solidMotherVolume, m_MaterialVacuum, Name, 0, 0, 0);

   	new G4PVPlacement(G4Transform3D(*MMrot, MMpos), m_logicMotherVolume, Name, world, false, DetectorNumber);

   	G4VisAttributes* MotherVolumeVisAtt = new G4VisAttributes(G4Colour(0.90, 0.90, 0.90));
  	 MotherVolumeVisAtt->SetForceWireframe(true); 
   	m_logicMotherVolume->SetVisAttributes(G4VisAttributes::Invisible);

	///////////////////////////////////////////////////
	///////////////////// Thin Si /////////////////////
	///////////////////////////////////////////////////
	G4String NameThinSi = "ThinSi"+DetNumber;
	//if(!m_LogicThinSi){
	if(0==0){
		G4Box* solidThinSi = new G4Box(NameThinSi, 0.5*SiliconFace, 0.5*SiliconFace, 0.5*ThinSiThickness);	;
		m_LogicThinSi = new G4LogicalVolume(solidThinSi, m_MaterialSilicon, "logicThinSi", 0, 0, 0);

		G4ThreeVector positionThinSi = G4ThreeVector(0, 0, ThinSi_PosZ);

		new G4PVPlacement(new G4RotationMatrix(0,0,0),
               		      	 positionThinSi,
                    		 m_LogicThinSi,"ThinSi",
                   		 m_logicMotherVolume,false,DetectorNumber);

		// Set ThinSi sensible
        	m_LogicThinSi->SetSensitiveDetector(m_ThinSiStripScorer);
	
		// Visualisation of ThinSi
        	m_LogicThinSi->SetVisAttributes(m_SiliconVisAtt) ;   
	}

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
        //m_LogicCluster->SetVisAttributes(G4VisAttributes::Invisible);
        G4VisAttributes* TempVisAtt = new G4VisAttributes(G4Colour(0.6, 0.6, 0.3)) ;
        TempVisAtt->SetForceWireframe(true);
        m_LogicCluster->SetVisAttributes(TempVisAtt);

        G4ThreeVector FramePos(0,0,CsI_PosZ);

        new G4PVPlacement(new G4RotationMatrix(0,0,0),
                      	FramePos,
                      	m_LogicCluster,"Cluster",
                      	m_logicMotherVolume,false,0);

        const G4double CsIXMiddle = CsIXFront + (CsIThickness/2)*tan(-pTheta)*sin(pPhi);
        const G4double CsIYMiddle = CsIYFront + (CsIThickness/2)*tan(-pTheta)*cos(pPhi);
        const G4double DistInterCsIX = CsIXMiddle+DistInterCsI/2;
        const G4double DistInterCsIY = CsIYMiddle+DistInterCsI/2;
        
        G4ThreeVector Origin(-0.5*DistInterCsIX,-0.5*DistInterCsIY,0);
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
                    Pos = Origin + G4ThreeVector(i*(DistInterCsIX+DistInterCsI/2),j*(DistInterCsIY+DistInterCsI/2),0);
                    
                    new G4PVPlacement(G4Transform3D(*rotM,Pos),
                                      m_LogicCsICrystal,
                                      "CsI_Cristal",
                                      m_LogicCluster,
                                      false,
                                      CrystalNbr);
                    delete rotM;
                }
            }
        }
    }


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Hira::InitializeMaterial(){
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
NPS::VDetector* Hira::Construct(){
    return  (NPS::VDetector*) new Hira();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
    class proxy_nps_hira{
    public:
        proxy_nps_hira(){
            NPS::DetectorFactory::getInstance()->AddToken("HIRAArray","Hira");
            NPS::DetectorFactory::getInstance()->AddDetector("HIRAArray",Hira::Construct);
        }
    };
    
    proxy_nps_hira p_nps_hira;
}



