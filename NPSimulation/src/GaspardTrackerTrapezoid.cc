/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 15/07/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: Define a module of trapezoidal shape for the Gaspard tracker  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <sstream>
#include <string>
#include <cmath>

// G4 Geometry headers
#include "G4Box.hh"
#include "G4Trap.hh"

// G4 various headers
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4PVDivision.hh"

// G4 sensitive
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

// NPTool headers
#include "GaspardTrackerTrapezoid.hh"
#include "GeneralScorers.hh"
#include "GaspardScorers.hh"
#include "RootOutput.h"

// CLHEP
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;
using namespace GPDTRAP ;



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GaspardTrackerTrapezoid::GaspardTrackerTrapezoid()
{
   ms_InterCoord = 0;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GaspardTrackerTrapezoid::~GaspardTrackerTrapezoid()
{
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GaspardTrackerTrapezoid::AddModule(G4ThreeVector X1_Y1     ,
      G4ThreeVector X128_Y1   ,
      G4ThreeVector X1_Y128   ,
      G4ThreeVector X128_Y128 ,
      bool wFirstStage        ,
      bool wSecondStage       ,
      bool wThirdStage)
{
   m_DefinitionType.push_back(true) ;

   m_X1_Y1.push_back(X1_Y1)               ;
   m_X128_Y1.push_back(X128_Y1)           ;
   m_X1_Y128.push_back(X1_Y128)           ;
   m_X128_Y128.push_back(X128_Y128)       ;
   m_wFirstStage.push_back(wFirstStage)   ;
   m_wSecondStage.push_back(wSecondStage) ;
   m_wThirdStage.push_back(wThirdStage)   ;

   m_R.push_back(0)      ;
   m_Theta.push_back(0)  ;
   m_Phi.push_back(0)    ;
   m_beta_u.push_back(0) ;
   m_beta_v.push_back(0) ;
   m_beta_w.push_back(0) ;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GaspardTrackerTrapezoid::AddModule(G4double R        ,
      G4double Theta    ,
      G4double Phi      ,
      G4double beta_u   ,
      G4double beta_v   ,
      G4double beta_w   ,
      bool wFirstStage  ,
      bool wSecondStage ,
      bool wThirdStage)
{
   G4ThreeVector empty = G4ThreeVector(0, 0, 0);

   m_DefinitionType.push_back(false);

   m_R.push_back(R)                       ;
   m_Theta.push_back(Theta)               ;
   m_Phi.push_back(Phi)                   ;
   m_beta_u.push_back(beta_u)             ;
   m_beta_v.push_back(beta_v)             ;
   m_beta_w.push_back(beta_w)             ;
   m_wFirstStage.push_back(wFirstStage)   ;
   m_wSecondStage.push_back(wSecondStage) ;
   m_wThirdStage.push_back(wThirdStage)   ;

   m_X1_Y1.push_back(empty)     ;
   m_X128_Y1.push_back(empty)   ;
   m_X1_Y128.push_back(empty)   ;
   m_X128_Y128.push_back(empty) ;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GaspardTrackerTrapezoid::VolumeMaker(G4int TelescopeNumber   ,
                                          G4ThreeVector MMpos     ,
                                          G4RotationMatrix* MMrot ,
                                          bool wFirstStage                ,
                                          bool wSecondStage              ,
                                          bool wThirdStage               ,
                                          G4LogicalVolume* world)
{
   G4double NbrTelescopes = TelescopeNumber  ;
   G4String DetectorNumber                   ;
   ostringstream Number                      ;
   Number << NbrTelescopes                   ;
   DetectorNumber = Number.str()             ;

   ////////////////////////////////////////////////////////////////
   /////////////////Element  Definition ///////////////////////////
   ////////////////////////////////////////////////////////////////
   G4String symbol                      ;
   G4double density = 0. , a = 0, z = 0 ;
   G4int ncomponents = 0, natoms = 0    ;

   G4Element* H   = new G4Element("Hydrogen" , symbol = "H"  , z = 1  , a = 1.01   * g / mole);
   G4Element* C   = new G4Element("Carbon"   , symbol = "C"  , z = 6  , a = 12.011 * g / mole);
   G4Element* N   = new G4Element("Nitrogen" , symbol = "N"  , z = 7  , a = 14.01  * g / mole);
   G4Element* O   = new G4Element("Oxigen"   , symbol = "O"  , z = 8  , a = 16.00  * g / mole);
   G4Element* I   = new G4Element("Iode"     , symbol = "I"  , z = 53 , a = 126.9  * g / mole);
   G4Element* Cs  = new G4Element("Cesium"   , symbol = "Cs" , z = 55 , a = 132.9  * g / mole);

   G4Element* Co  = new G4Element("Cobalt"  , symbol = "Co" , z = 27 , a = 58.933 * g / mole);
   G4Element* Cr  = new G4Element("Cromium"  , symbol = "Cr" , z = 24 , a = 51.996 * g / mole);
   G4Element* Ni  = new G4Element("Nickel"   , symbol = "Ni" , z = 28 , a = 58.69  * g / mole);
   G4Element* Fe  = new G4Element("Iron"     , symbol = "Fe" , z = 26 , a = 55.847 * g / mole);
   G4Element* W   = new G4Element("Tungsten" , symbol = "W"  , z = 74 , a = 183.5  * g / mole);

   ////////////////////////////////////////////////////////////////
   /////////////////Material Definition ///////////////////////////
   ////////////////////////////////////////////////////////////////
   // Si
   a = 28.0855 * g / mole;
   density = 2.321 * g / cm3;
   G4Material* Silicon = new G4Material("Si", z = 14., a, density);

   // Al
//   density = 2.702 * g / cm3;
//   a = 26.98 * g / mole;
//   G4Material* Aluminium = new G4Material("Aluminium", z = 13., a, density);

   // Iron
//   density = 7.874 * g / cm3;
//   a = 55.847 * g / mole;
//   G4Material* Iron = new G4Material("Iron", z = 26., a, density);

   // CsI
   density = 4.51 * g / cm3;
   G4Material* CsI = new G4Material("CsI", density, ncomponents = 2);
   CsI->AddElement(Cs , natoms = 1);
   CsI->AddElement(I  , natoms = 1);

   //  Vacuum
   density = 0.000000001 * mg / cm3;
   G4Material* Vacuum = new G4Material("Vacuum", density, ncomponents = 2);
   Vacuum->AddElement(N, .7);
   Vacuum->AddElement(O, .3);

   //  Mylar
   density = 1.397 * g / cm3;
   G4Material* Myl = new G4Material("Mylar", density, ncomponents = 3);
   Myl->AddElement(C, natoms = 10);
   Myl->AddElement(H, natoms = 8);
   Myl->AddElement(O, natoms = 4);

   // Havar
   G4Material* Harvar = new G4Material("Havar", 8.3*g / cm3, 5);
   Harvar->AddElement(Co , 42);
   Harvar->AddElement(Cr , 20);
   Harvar->AddElement(Ni , 13);
   Harvar->AddElement(Fe , 19);
   Harvar->AddElement(W  ,  1);

   ////////////////////////////////////////////////////////////////
   ////////////// Starting Volume Definition //////////////////////
   ////////////////////////////////////////////////////////////////
   // Little trick to avoid warning in compilation: Use a PVPlacement "buffer".
   // If don't you will have a Warning unused variable 'myPVP'
   G4PVPlacement* PVPBuffer ;

   // Definition of the volume containing the sensitive detector
   G4Trap* solidMM = new G4Trap("GPDTrapezoid" + DetectorNumber, 
                                Length/2, 0*deg, 0*deg, 
                                Height/2, BaseSmall/2, BaseLarge/2, 0*deg, 
                                Height/2, BaseSmall/2, BaseLarge/2, 0*deg);

//   G4LogicalVolume* logicMM = new G4LogicalVolume(solidMM, Iron, "GPDTrapezoid" + DetectorNumber, 0, 0, 0)                                ;
   G4LogicalVolume* logicMM = new G4LogicalVolume(solidMM, Vacuum, "GPDTrapezoid" + DetectorNumber, 0, 0, 0)                                ;

   G4String Name = "GPDTrapezoid" + DetectorNumber ;
   PVPBuffer     = new G4PVPlacement(G4Transform3D(*MMrot, MMpos) ,
                                     logicMM                      ,
                                     Name                         ,
                                     world                        ,
                                     false                        ,
                                     0);

   logicMM->SetVisAttributes(G4VisAttributes::Invisible);
   if (m_non_sensitive_part_visiualisation) logicMM->SetVisAttributes(G4VisAttributes(G4Colour(0.90, 0.90, 0.90)));

   // Definition of a vaccuum volume
   G4ThreeVector positionVacBox = G4ThreeVector(0, 0, VacBox_PosZ);

   G4Trap* solidVacBox = new G4Trap("solidVacBox", 
                                    VacBoxThickness/2, 0*deg, 0*deg, 
                                    FirstStageHeight/2, FirstStageBaseSmall/2, FirstStageBaseLarge/2, 0*deg, 
                                    FirstStageHeight/2, FirstStageBaseSmall/2, FirstStageBaseLarge/2, 0*deg);

   G4LogicalVolume* logicVacBox = new G4LogicalVolume(solidVacBox, Vacuum, "logicVacBox", 0, 0, 0);

   PVPBuffer = new G4PVPlacement(0, positionVacBox, logicVacBox, "G" + DetectorNumber + "VacBox", logicMM, false, 0);

   logicVacBox->SetVisAttributes(G4VisAttributes::Invisible);

   // Add a degrader plate between Si and CsI:
   /*
      G4Box* Degrader = new G4Box("Degrader" , 50*mm , 50*mm , 0.1*mm );
      G4LogicalVolume* logicDegrader = new G4LogicalVolume( Degrader , Harvar, "logicDegrader",0,0,0);
      PVPBuffer = new G4PVPlacement(0,G4ThreeVector(0,0,0),logicDegrader,"Degrader",logicVacBox,false,0) ;
   */

   //Place two marker to identify the u and v axis on silicon face:
   //marker are placed a bit before the silicon itself so they don't perturbate simulation
   //Uncomment to help debugging or if you want to understand the way the code work.
   //I should recommand to Comment it during simulation to avoid perturbation of simulation
   //Remember G4 is limitationg step on geometry constraints.
  /* 
         G4ThreeVector positionMarkerU = CT*0.98 + MMu*SiliconFace/4;
         G4Box*          solidMarkerU = new G4Box( "solidMarkerU" , SiliconFace/4 , 1*mm , 1*mm )              ;
         G4LogicalVolume* logicMarkerU = new G4LogicalVolume( solidMarkerU , Vacuum , "logicMarkerU",0,0,0)       ;
         PVPBuffer = new G4PVPlacement(G4Transform3D(*MMrot,positionMarkerU),logicMarkerU,"MarkerU",world,false,0) ;

         G4VisAttributes* MarkerUVisAtt= new G4VisAttributes(G4Colour(0.,0.,0.5));//blue
         logicMarkerU->SetVisAttributes(MarkerUVisAtt);

         G4ThreeVector positionMarkerV = CT*0.98 + MMv*SiliconFace/4;
         G4Box*          solidMarkerV = new G4Box( "solidMarkerU" , 1*mm , SiliconFace/4 , 1*mm )              ;
         G4LogicalVolume* logicMarkerV = new G4LogicalVolume( solidMarkerV , Vacuum , "logicMarkerV",0,0,0)       ;
         PVPBuffer = new G4PVPlacement(G4Transform3D(*MMrot,positionMarkerV),logicMarkerV,"MarkerV",world,false,0) ;

         G4VisAttributes* MarkerVVisAtt= new G4VisAttributes(G4Colour(0.,0.5,0.5));//green
         logicMarkerV->SetVisAttributes(MarkerVVisAtt);
   */

   ////////////////////////////////////////////////////////////////
   /////////////////// First Stage Construction////////////////////
   ////////////////////////////////////////////////////////////////
   if (wFirstStage) {
      // Aluminium dead layers
      G4ThreeVector positionAluStripFront = G4ThreeVector(0, 0, AluStripFront_PosZ);
      G4ThreeVector positionAluStripBack  = G4ThreeVector(0, 0, AluStripBack_PosZ);

      G4Trap* solidAluStrip = new G4Trap("AluBox", 
                                         AluStripThickness/2, 0*deg, 0*deg, 
                                         FirstStageHeight/2, FirstStageBaseSmall/2, FirstStageBaseLarge/2, 0*deg, 
                                         FirstStageHeight/2, FirstStageBaseSmall/2, FirstStageBaseLarge/2, 0*deg);

//      G4LogicalVolume* logicAluStrip = new G4LogicalVolume(solidAluStrip, Aluminium, "logicAluStrip", 0, 0, 0);
      G4LogicalVolume* logicAluStrip = new G4LogicalVolume(solidAluStrip, Vacuum, "logicAluStrip", 0, 0, 0);

      PVPBuffer = new G4PVPlacement(0, positionAluStripFront, logicAluStrip, "G" + DetectorNumber + "AluStripFront", logicMM, false, 0);
      PVPBuffer = new G4PVPlacement(0, positionAluStripBack,  logicAluStrip, "G" + DetectorNumber + "AluStripBack",  logicMM, false, 0);

      logicAluStrip->SetVisAttributes(G4VisAttributes::Invisible);

      // Silicon detector itself
      G4ThreeVector  positionSilicon = G4ThreeVector(0, 0, Silicon_PosZ);

      G4Trap* solidSilicon = new G4Trap("solidSilicon", 
                                         FirstStageThickness/2, 0*deg, 0*deg, 
                                         FirstStageHeight/2, FirstStageBaseSmall/2, FirstStageBaseLarge/2, 0*deg, 
                                         FirstStageHeight/2, FirstStageBaseSmall/2, FirstStageBaseLarge/2, 0*deg);
      G4LogicalVolume* logicSilicon = new G4LogicalVolume(solidSilicon, Silicon, "logicSilicon", 0, 0, 0);

      PVPBuffer = new G4PVPlacement(0, positionSilicon, logicSilicon, "G" + DetectorNumber + "Silicon", logicMM, false, 0);

      // Set First Stage sensible
      logicSilicon->SetSensitiveDetector(m_FirstStageScorer);

      // Visualisation of Silicon Strip
      G4VisAttributes* SiliconVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)) ;
      logicSilicon->SetVisAttributes(SiliconVisAtt)                        ;
   }

   ////////////////////////////////////////////////////////////////
   //////////////// Second Stage  Construction ////////////////////
   ////////////////////////////////////////////////////////////////
   if (wSecondStage) {
   }

   ////////////////////////////////////////////////////////////////
   ///////////////// Third Stage Construction /////////////////////
   ////////////////////////////////////////////////////////////////
   if (wThirdStage) {
      // Third stage silicon detector
      G4ThreeVector  positionThirdStage = G4ThreeVector(0, 0, ThirdStage_PosZ);

      G4Trap* solidThirdStage = new G4Trap("solidThirdStage", 
                                           ThirdStageThickness/2, 0*deg, 0*deg, 
                                           Height/2, BaseSmall/2, BaseLarge/2, 0*deg, 
                                           Height/2, BaseSmall/2, BaseLarge/2, 0*deg);

      G4LogicalVolume* logicThirdStage = new G4LogicalVolume(solidThirdStage, Silicon, "logicThirdStage", 0, 0, 0);

      PVPBuffer = new G4PVPlacement(0, positionThirdStage, logicThirdStage, "G" + DetectorNumber + "ThirdStage", logicMM, false, 0);

      // Visualisation of Third Stage
      G4VisAttributes* ThirdStageVisAtt = new G4VisAttributes(G4Colour(0.7, 0.7, 0.7)) ;
      logicThirdStage->SetVisAttributes(ThirdStageVisAtt)                        ;
//      logicThirdStage->SetVisAttributes(G4VisAttributes::Invisible);

      // Set Third Stage sensible
      logicThirdStage->SetSensitiveDetector(m_ThirdStageScorer);
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void GaspardTrackerTrapezoid::ReadConfiguration(string Path)
{
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
   G4double Theta = 0 , Phi = 0 , R = 0 , beta_u = 0 , beta_v = 0 , beta_w = 0 ;
   int FIRSTSTAGE = 0 , SECONDSTAGE = 0 , THIRDSTAGE = 0                       ;

	bool ReadingStatus = false ;

   bool check_A = false ;
   bool check_C = false ;
   bool check_B = false ;
   bool check_D = false ;

   bool check_Theta = false ;
   bool check_Phi   = false ;
   bool check_R     = false ;
   bool check_beta  = false ;
   
   bool check_FirstStage = false ;
   bool check_SecondStage = false ;
   bool check_ThirdStage = false ;
   bool checkVis = false ;

   while (!ConfigFile.eof()) {
      getline(ConfigFile, LineBuffer);
      if (LineBuffer.compare(0, 12, "GPDTrapezoid") == 0) {
         G4cout << "///" << G4endl           ;
         G4cout << "Trapezoid element found: " << G4endl   ;
         ReadingStatus = true ;
         }
         
   while(ReadingStatus){      

         ConfigFile >> DataBuffer;
         //	Comment Line 
		if (DataBuffer.compare(0, 1, "%") == 0) {/*do nothing */;}
		
         // Position method
         else if (DataBuffer.compare(0, 6, "X1_Y1=") == 0) {
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
            cout << "X1 Y1 corner position : " << A << endl;
         }
        
         else if (DataBuffer.compare(0, 8, "X128_Y1=") == 0) {
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
            cout << "X128 Y1 corner position : " << B << endl;
         }
         
         else if (DataBuffer.compare(0, 8, "X1_Y128=") == 0) {
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
            cout << "X1 Y128 corner position : " << C << endl;
         }
        
         else if (DataBuffer.compare(0, 10, "X128_Y128=") == 0) {
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
            cout << "X128 Y128 corner position : " << D << endl;
         }
         

		 // Angle method
         else if (DataBuffer.compare(0, 6, "THETA=") == 0) {
            check_Theta = true;
            ConfigFile >> DataBuffer ;
            Theta = atof(DataBuffer.c_str()) ;
            Theta = Theta * deg;
            cout << "Theta:  " << Theta / deg << endl;
         }

         else if (DataBuffer.compare(0, 4, "PHI=") == 0) {
            check_Phi = true;
            ConfigFile >> DataBuffer ;
            Phi = atof(DataBuffer.c_str()) ;
            Phi = Phi * deg;
            cout << "Phi:  " << Phi / deg << endl;
         }

         else if (DataBuffer.compare(0, 2, "R=") == 0) {
            check_R = true;
            ConfigFile >> DataBuffer ;
            R = atof(DataBuffer.c_str()) ;
            R = R * mm;
            cout << "R:  " << R / mm << endl;
         }

         else if (DataBuffer.compare(0, 5, "BETA=") == 0) {
            check_beta = true;
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
         }

         else if (DataBuffer.compare(0, 11, "FIRSTSTAGE=") == 0) {
            check_FirstStage = true ;
            ConfigFile >> DataBuffer;
            FIRSTSTAGE = atof(DataBuffer.c_str()) ;
         }

         else if (DataBuffer.compare(0, 12, "SECONDSTAGE=") == 0) {
         	check_SecondStage = true ;
            ConfigFile >> DataBuffer;
            SECONDSTAGE = atof(DataBuffer.c_str()) ;
         }

         else if (DataBuffer.compare(0, 11, "THIRDSTAGE=") == 0) {
            check_ThirdStage = true ;
            ConfigFile >> DataBuffer;
            THIRDSTAGE = atof(DataBuffer.c_str()) ;
         }

         else if (DataBuffer.compare(0, 4, "VIS=") == 0) {
         	checkVis = true ;
            ConfigFile >> DataBuffer;
            if (DataBuffer.compare(0, 3, "all") == 0) m_non_sensitive_part_visiualisation = true;
         }
         
         else G4cout << "WARNING: Wrong Token, GaspardTrackerTrapezoid: Trapezoid Element not added" << G4endl;

         //Add The previously define telescope
         //With position method
         if ((check_A && check_B && check_C && check_D && check_FirstStage && check_SecondStage && check_ThirdStage && checkVis) && !(check_Theta && check_Phi && check_R)) {
         
            ReadingStatus = false ;
		    check_A = false ;
		    check_C = false ;
		    check_B = false ;
		    check_D = false ;
		    check_FirstStage = false ;
		    check_SecondStage = false ;
		    check_ThirdStage = false ;
		    checkVis = false ;
         
            AddModule(A                ,
                      B                ,
                      C                ,
                      D                ,
                      FIRSTSTAGE  == 1 ,
                      SECONDSTAGE == 1 ,
                      THIRDSTAGE  == 1);
         }

         //with angle method
        if ((check_Theta && check_Phi && check_R && check_FirstStage && check_SecondStage && check_ThirdStage && checkVis) && !(check_A && check_B && check_C && check_D)) {
         	ReadingStatus = false ;
         	 check_Theta = false ;
   			 check_Phi   = false ;
   			 check_R     = false ;
   			 check_beta  = false ;
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
         }

         
      }
   }
}

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void GaspardTrackerTrapezoid::ConstructDetector(G4LogicalVolume* world)
{
   G4RotationMatrix* MMrot    = NULL                   ;
/*   G4ThreeVector     MMpos    = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     MMu      = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     MMv      = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     MMw      = G4ThreeVector(0, 0, 0) ;*/
   MMpos    = G4ThreeVector(0, 0, 0) ;
   MMu      = G4ThreeVector(0, 0, 0) ;
   MMv      = G4ThreeVector(0, 0, 0) ;
   MMw      = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     MMCenter = G4ThreeVector(0, 0, 0) ;
   bool FirstStage  = true ;
   bool SecondStage = true ;
   bool ThirdStage  = true ;

   G4int NumberOfModule = m_DefinitionType.size() ;

   for (G4int i = 0; i < NumberOfModule; i++) {
      // By Point
      if (m_DefinitionType[i]) {
         // (u,v,w) unitary vector associated to telescope referencial
         // (u,v) // to silicon plan
         // w perpendicular to (u,v) plan and pointing ThirdStage
         G4cout << "############ Gaspard Trapezoid " << i << " #############" << G4endl;
         MMu = m_X128_Y1[i] - m_X1_Y128[i] ;
         G4cout << "MMu: " << MMu << G4endl;
         MMu = MMu.unit()                ;
         G4cout << "Norm MMu: " << MMu << G4endl;

         MMv = -0.5 * (m_X1_Y1[i] + m_X128_Y128[i] - m_X1_Y128[i] - m_X128_Y1[i]);
         G4cout << "MMv: " << MMv << G4endl;
         MMv = MMv.unit()                ;
         G4cout << "Norm MMv: " << MMv << G4endl;

         G4ThreeVector MMscal = MMu.dot(MMv);
         G4cout << "Norm MMu.MMv: " << MMscal << G4endl;

         MMw = MMu.cross(MMv)                  ;
//         if (MMw.z() > 0) MMw = MMv.cross(MMu) ;
         MMw = MMw.unit()                      ;
         G4cout << "Norm MMw: " << MMw << G4endl;

         // Center of the module
         MMCenter = (m_X1_Y1[i] + m_X1_Y128[i] + m_X128_Y1[i] + m_X128_Y128[i]) / 4 ;

         // Passage Matrix from Lab Referential to Module Referential
         MMrot = new G4RotationMatrix(MMu, MMv, MMw) ;
         // translation to place Module
         MMpos = MMw * Length * 0.5 + MMCenter ;
      }

      // By Angle
      else {
         G4double Theta = m_Theta[i] ;
         G4double Phi   = m_Phi[i]   ;
         //This part because if Phi and Theta = 0 equation are false
         if (Theta == 0)        Theta = 0.0001 ;
         if (Theta == 2*cos(0)) Theta = 2 * acos(0) - 0.00001 ;
         if (Phi   == 0)        Phi   = 0.0001 ;

         // (u,v,w) unitary vector associated to telescope referencial
         // (u,v) // to silicon plan
         // w perpendicular to (u,v) plan and pointing ThirdStage
         // Phi is angle between X axis and projection in (X,Y) plan
         // Theta is angle between  position vector and z axis
         G4double wX = m_R[i] * sin(Theta / rad) * cos(Phi / rad)   ;
         G4double wY = m_R[i] * sin(Theta / rad) * sin(Phi / rad)   ;
         G4double wZ = m_R[i] * cos(Theta / rad)             ;

         MMw = G4ThreeVector(wX, wY, wZ)                ;
//         G4ThreeVector CT = MMw                       ;
         CT = MMw                       ;
         MMw = MMw.unit()                          ;

         G4ThreeVector Y = G4ThreeVector(0 , 1 , 0)         ;

         MMu = MMw.cross(Y)      ;
         MMv = MMw.cross(MMu) ;

         MMv = MMv.unit();
         MMu = MMu.unit();
         // Passage Matrix from Lab Referential to Telescope Referential
         // MUST2
         MMrot = new G4RotationMatrix(MMu, MMv, MMw);
         // Telescope is rotate of Beta angle around MMv axis.
         MMrot->rotate(m_beta_u[i], MMu);
         MMrot->rotate(m_beta_v[i], MMv);
         MMrot->rotate(m_beta_w[i], MMw);
         // translation to place Telescope
         MMpos = MMw * Length * 0.5 + CT ;
      }

      FirstStage  = m_wFirstStage[i]  ;
      SecondStage = m_wSecondStage[i] ;
      ThirdStage  = m_wThirdStage[i]  ;

      VolumeMaker(i + 1, MMpos, MMrot, FirstStage, SecondStage, ThirdStage , world);
   }

   delete MMrot ;
}



// Connect the GaspardTrackingData class to the output TTree
// of the simulation
void GaspardTrackerTrapezoid::InitializeRootOutput()
{
}



// Set the TinteractionCoordinates object from VDetector to the present class
void GaspardTrackerTrapezoid::SetInterCoordPointer(TInteractionCoordinates* interCoord)
{
   ms_InterCoord = interCoord;
}



// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void GaspardTrackerTrapezoid::ReadSensitive(const G4Event* event)
{
   bool checkSi = false      ;
   G4String DetectorNumber ;

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////// Used to Read Event Map of detector //////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

   // First Stage
   std::map<G4int, G4int*>::iterator    DetectorNumber_itr;
   std::map<G4int, G4double*>::iterator Energy_itr;
   std::map<G4int, G4double*>::iterator Time_itr;
   std::map<G4int, G4double*>::iterator X_itr;
   std::map<G4int, G4double*>::iterator Y_itr;
   std::map<G4int, G4double*>::iterator Pos_X_itr;
   std::map<G4int, G4double*>::iterator Pos_Y_itr;
   std::map<G4int, G4double*>::iterator Pos_Z_itr;
   std::map<G4int, G4double*>::iterator Ang_Theta_itr;
   std::map<G4int, G4double*>::iterator Ang_Phi_itr;

   G4THitsMap<G4int>*    DetectorNumberHitMap;
   G4THitsMap<G4double>* EnergyHitMap;
   G4THitsMap<G4double>* TimeHitMap;
   G4THitsMap<G4double>* XHitMap;
   G4THitsMap<G4double>* YHitMap;
   G4THitsMap<G4double>* PosXHitMap;
   G4THitsMap<G4double>* PosYHitMap;
   G4THitsMap<G4double>* PosZHitMap;
   G4THitsMap<G4double>* AngThetaHitMap;
   G4THitsMap<G4double>* AngPhiHitMap;

   // NULL pointer are given to avoid warning at compilation

   // Third Stage
   std::map<G4int, G4double*>::iterator ThirdStageEnergy_itr;
   G4THitsMap<G4double>* ThirdStageEnergyHitMap = NULL;

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
   G4int HitNumber = 0;
   checkSi = false;

   // Read the Scorer associated to the first Stage
   //Detector Number
   G4int StripDetCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDTrapezoid/DetectorNumber")    ;
   DetectorNumberHitMap = (G4THitsMap<G4int>*)(event->GetHCofThisEvent()->GetHC(StripDetCollectionID))         ;
   DetectorNumber_itr =  DetectorNumberHitMap->GetMap()->begin()                                               ;

   //Energy
   G4int StripEnergyCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDTrapezoid/StripEnergy")   ;
   EnergyHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripEnergyCollectionID))                    ;
   Energy_itr = EnergyHitMap->GetMap()->begin()                                                          ;

   //Time of Flight
   G4int StripTimeCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDTrapezoid/StripTime")    ;
   TimeHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripTimeCollectionID))                        ;
   Time_itr = TimeHitMap->GetMap()->begin()                                                              ;

   //Strip Number X
   G4int StripXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDTrapezoid/StripNumberX")    ;
   XHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripXCollectionID))                              ;
   X_itr = XHitMap->GetMap()->begin()                                                                    ;

   //Strip Number Y
   G4int StripYCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDTrapezoid/StripNumberY")    ;
   YHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripYCollectionID))                              ;
   Y_itr = YHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate X
   G4int InterCoordXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDTrapezoid/InterCoordX")    ;
   PosXHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordXCollectionID))                              ;
   Pos_X_itr = PosXHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Y
   G4int InterCoordYCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDTrapezoid/InterCoordY")    ;
   PosYHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordYCollectionID))                              ;
   Pos_Y_itr = PosYHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Z
   G4int InterCoordZCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDTrapezoid/InterCoordZ")    ;
   PosZHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordZCollectionID))                              ;
   Pos_Z_itr = PosXHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Angle Theta
   G4int InterCoordAngThetaCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDTrapezoid/InterCoordAngTheta")    ;
   AngThetaHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordAngThetaCollectionID))                              ;
   Ang_Theta_itr = AngThetaHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Angle Phi
   G4int InterCoordAngPhiCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDTrapezoid/InterCoordAngPhi")    ;
   AngPhiHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordAngPhiCollectionID))                              ;
   Ang_Phi_itr = AngPhiHitMap->GetMap()->begin()                                                                    ;

   // Read the Scorer associated to the Third Stage 
   //Energy
   G4int ThirdStageEnergyCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ThirdStageScorerGPDTrapezoid/ThirdStageEnergy")      ;
   ThirdStageEnergyHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(ThirdStageEnergyCollectionID))                      ;
   ThirdStageEnergy_itr = ThirdStageEnergyHitMap->GetMap()->begin()                                                       ;

   // Check the size of different map
   G4int sizeN = DetectorNumberHitMap->entries();
   G4int sizeE = EnergyHitMap->entries();
   G4int sizeT = TimeHitMap->entries();
   G4int sizeX = XHitMap->entries();
   G4int sizeY = YHitMap->entries();

      if (sizeE != sizeT || sizeT != sizeX || sizeX != sizeY) {
         G4cout << "No match size Si Event Map: sE:"
         << sizeE << " sT:" << sizeT << " sX:" << sizeX << " sY:" << sizeY << endl ;
         return;
      }

      // Loop on Strip energy
      for (G4int l = 0 ; l < sizeE ; l++) {
         G4int ETrackID  =   Energy_itr->first     ;
         G4double E     = *(Energy_itr->second)    ;
         G4int N = 0;

         if (E > 0) {
            checkSi = true   ;
            ms_Event->SetGPDTrkFirstStageFrontEEnergy(RandGauss::shoot(E, ResoFirstStage))    ;
            ms_Event->SetGPDTrkFirstStageBackEEnergy(RandGauss::shoot(E, ResoFirstStage))    ;

            //  Detector Number
            DetectorNumber_itr = DetectorNumberHitMap->GetMap()->begin();
            for (G4int h = 0 ; h < sizeN ; h++) {
                G4int NTrackID  =   DetectorNumber_itr->first       ;
                G4double Nl     = *(DetectorNumber_itr->second)      ;

                if (NTrackID == ETrackID) {
                   N = Nl ;
                   ms_Event->SetGPDTrkFirstStageFrontEDetectorNbr(m_index["Trapezoid"] + N);
                   ms_Event->SetGPDTrkFirstStageFrontTDetectorNbr(m_index["Trapezoid"] + N);
                   ms_Event->SetGPDTrkFirstStageBackEDetectorNbr(m_index["Trapezoid"] + N);
                   ms_Event->SetGPDTrkFirstStageBackTDetectorNbr(m_index["Trapezoid"] + N);
                }
                DetectorNumber_itr++;
            }

            //  Time
            Time_itr = TimeHitMap->GetMap()->begin();
            for (G4int h = 0 ; h < sizeT ; h++) {
               G4int TTrackID  =   Time_itr->first       ;
               G4double T     = *(Time_itr->second)      ;

               if (TTrackID == ETrackID) {
                  T = RandGauss::shoot(T, ResoTimePPAC)   ;
                  ms_Event->SetGPDTrkFirstStageFrontTTime(RandGauss::shoot(T, ResoTimeGpd)) ;
                  ms_Event->SetGPDTrkFirstStageBackTTime(RandGauss::shoot(T, ResoTimeGpd)) ;
               }
               Time_itr++;
            }

            // X
            X_itr = XHitMap->GetMap()->begin();
            for (G4int h = 0 ; h < sizeX ; h++) {
               G4int XTrackID  =   X_itr->first     ;
               G4double X     = *(X_itr->second)      ;
               if (XTrackID == ETrackID) {
                  ms_Event->SetGPDTrkFirstStageFrontEStripNbr(X);
                  ms_Event->SetGPDTrkFirstStageFrontTStripNbr(X);
               }

               X_itr++;
            }

            // Y
            Y_itr = YHitMap->GetMap()->begin()  ;
            for (G4int h = 0 ; h < sizeY ; h++) {
               G4int YTrackID  =   Y_itr->first    ;
               G4double Y     = *(Y_itr->second)      ;
               if (YTrackID == ETrackID) {
                  ms_Event->SetGPDTrkFirstStageBackEStripNbr(Y);
                  ms_Event->SetGPDTrkFirstStageBackTStripNbr(Y);
               }

               Y_itr++;
            }

            // Pos X
            Pos_X_itr = PosXHitMap->GetMap()->begin();
            for (G4int h = 0 ; h < sizeX ; h++) {
               G4int PosXTrackID =   Pos_X_itr->first     ;
               G4double PosX     = *(Pos_X_itr->second)      ;
               if (PosXTrackID == ETrackID) {
                  ms_InterCoord->SetDetectedPositionX(PosX) ;
               }
               Pos_X_itr++;
            }

            // Pos Y
            Pos_Y_itr = PosYHitMap->GetMap()->begin();
            for (G4int h = 0 ; h < sizeX ; h++) {
               G4int PosYTrackID =   Pos_Y_itr->first     ;
               G4double PosY     = *(Pos_Y_itr->second)      ;
               if (PosYTrackID == ETrackID) {
                  ms_InterCoord->SetDetectedPositionY(PosY) ;
               }
               Pos_Y_itr++;
            }

            // Pos Z
            Pos_Z_itr = PosZHitMap->GetMap()->begin();
            for (G4int h = 0 ; h < sizeX ; h++) {
               G4int PosZTrackID =   Pos_Z_itr->first     ;
               G4double PosZ     = *(Pos_Z_itr->second)      ;
               if (PosZTrackID == ETrackID) {
                  ms_InterCoord->SetDetectedPositionZ(PosZ) ;
               }
               Pos_Z_itr++;
            }

            // Angle Theta
            Ang_Theta_itr = AngThetaHitMap->GetMap()->begin();
            for (G4int h = 0 ; h < sizeX ; h++) {
               G4int AngThetaTrackID =   Ang_Theta_itr->first     ;
               G4double AngTheta     = *(Ang_Theta_itr->second)      ;
               if (AngThetaTrackID == ETrackID) {
                  ms_InterCoord->SetDetectedAngleTheta(AngTheta) ;
               }
               Ang_Theta_itr++;
            }

            // Angle Phi
            Ang_Phi_itr = AngPhiHitMap->GetMap()->begin();
            for (G4int h = 0 ; h < sizeX ; h++) {
               G4int AngPhiTrackID =   Ang_Phi_itr->first     ;
               G4double AngPhi     = *(Ang_Phi_itr->second)      ;
               if (AngPhiTrackID == ETrackID) {
                  ms_InterCoord->SetDetectedAnglePhi(AngPhi) ;
               }
               Ang_Phi_itr++;
            }

            // Second Stage

            // Third Stage
               ThirdStageEnergy_itr = ThirdStageEnergyHitMap->GetMap()->begin()  ;
               for (G4int h = 0 ; h < ThirdStageEnergyHitMap->entries() ; h++) {
                  G4int ThirdStageEnergyTrackID  =   ThirdStageEnergy_itr->first      ;
                  G4double ThirdStageEnergy      = *(ThirdStageEnergy_itr->second)    ;

                  if (ThirdStageEnergyTrackID == ETrackID) {
                     ms_Event->SetGPDTrkThirdStageEEnergy(RandGauss::shoot(ThirdStageEnergy, ResoThirdStage));
                     ms_Event->SetGPDTrkThirdStageEPadNbr(1);
                     ms_Event->SetGPDTrkThirdStageTPadNbr(1);
                     ms_Event->SetGPDTrkThirdStageTTime(1);
                     ms_Event->SetGPDTrkThirdStageTDetectorNbr(m_index["Trapezoid"] + N);
                     ms_Event->SetGPDTrkThirdStageEDetectorNbr(m_index["Trapezoid"] + N);
                  }

                  ThirdStageEnergy_itr++;
               }

         Energy_itr++;
         if (checkSi) HitNumber++ ;
      }

      // clear map for next event
      DetectorNumberHitMap ->clear();
      EnergyHitMap   ->clear();
      TimeHitMap     ->clear();
      XHitMap        ->clear();
      YHitMap        ->clear();
      PosXHitMap     ->clear();
      PosYHitMap     ->clear();
      PosZHitMap     ->clear();
      AngThetaHitMap ->clear();
      AngPhiHitMap   ->clear();
      ThirdStageEnergyHitMap ->clear();
   }
}



void GaspardTrackerTrapezoid::InitializeScorers()
{
   // First stage Associate Scorer
   m_FirstStageScorer = new G4MultiFunctionalDetector("FirstStageScorerGPDTrapezoid");
   G4VPrimitiveScorer* DetNbr                           = new GPDScorerDetectorNumber("DetectorNumber", 0, "Silicon");
   G4VPrimitiveScorer* Energy                           = new GPDScorerFirstStageEnergy("StripEnergy", 0);
   G4VPrimitiveScorer* TOF                              = new PSTOF("StripTime", 0);
   G4VPrimitiveScorer* StripPositionX                   = new GPDScorerFirstStageFrontStripTrapezoid("StripNumberX", 0, NumberOfStripsX);
   G4VPrimitiveScorer* StripPositionY                   = new GPDScorerFirstStageBackStripTrapezoid("StripNumberY",  0, NumberOfStripsY);
   G4VPrimitiveScorer* InteractionCoordinatesX          = new PSInteractionCoordinatesX("InterCoordX", 0);
   G4VPrimitiveScorer* InteractionCoordinatesY          = new PSInteractionCoordinatesY("InterCoordY", 0);
   G4VPrimitiveScorer* InteractionCoordinatesZ          = new PSInteractionCoordinatesZ("InterCoordZ", 0);
   G4VPrimitiveScorer* InteractionCoordinatesAngleTheta = new PSInteractionCoordinatesAngleTheta("InterCoordAngTheta", 0);
   G4VPrimitiveScorer* InteractionCoordinatesAnglePhi   = new PSInteractionCoordinatesAnglePhi("InterCoordAngPhi", 0);

   //and register it to the multifunctionnal detector
   m_FirstStageScorer->RegisterPrimitive(DetNbr);
   m_FirstStageScorer->RegisterPrimitive(Energy);
   m_FirstStageScorer->RegisterPrimitive(TOF);
   m_FirstStageScorer->RegisterPrimitive(StripPositionX);
   m_FirstStageScorer->RegisterPrimitive(StripPositionY);
   m_FirstStageScorer->RegisterPrimitive(InteractionCoordinatesX);
   m_FirstStageScorer->RegisterPrimitive(InteractionCoordinatesY);
   m_FirstStageScorer->RegisterPrimitive(InteractionCoordinatesZ);
   m_FirstStageScorer->RegisterPrimitive(InteractionCoordinatesAngleTheta);
   m_FirstStageScorer->RegisterPrimitive(InteractionCoordinatesAnglePhi);

   // Second stage Associate Scorer
   m_SecondStageScorer = new G4MultiFunctionalDetector("SecondStageScorerGPDTrapezoid");
   G4VPrimitiveScorer* SecondStageEnergy = new GPDScorerSecondStageEnergy("SecondStageEnergy", 0);
   m_SecondStageScorer->RegisterPrimitive(SecondStageEnergy);

   //  Third stage Associate Scorer 
   m_ThirdStageScorer = new G4MultiFunctionalDetector("ThirdStageScorerGPDTrapezoid");
   G4VPrimitiveScorer* ThirdStageEnergy = new GPDScorerThirdStageEnergy("ThirdStageEnergy", 0);
   m_ThirdStageScorer->RegisterPrimitive(ThirdStageEnergy);

   //  Add All Scorer to the Global Scorer Manager
   G4SDManager::GetSDMpointer()->AddNewDetector(m_FirstStageScorer);
   G4SDManager::GetSDMpointer()->AddNewDetector(m_SecondStageScorer);
   G4SDManager::GetSDMpointer()->AddNewDetector(m_ThirdStageScorer);
}
