/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche    contact address: marc.labiche@stfc.ac.uk *
 *                                                                           *
 * Creation Date  : 31/01/12                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: Define a dummy module for the Helios detector                 *
 *             The goal of this class is to be a starting point to create a  *
 *             new shape to be added to the Helios detector.                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment: Here tyhe shape is a rectangular box (w~1cm, l~5cm, d~700um)     *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <sstream>
#include <string>
#include <cmath>

// G4 Geometry headers
#include "G4Trd.hh"
#include "G4Box.hh"
#include "G4Trap.hh"
#include "G4SubtractionSolid.hh"

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
#include "HeliosDetDummyShape.hh"
#include "GeneralScorers.hh"
#include "HeliosScorers.hh"
#include "RootOutput.h"

// CLHEP
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;
using namespace HELIOSDUMMYSHAPE;



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HeliosDetDummyShape::HeliosDetDummyShape()
{
   ms_InterCoord = 0;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HeliosDetDummyShape::~HeliosDetDummyShape()
{
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HeliosDetDummyShape::AddModule(G4ThreeVector X1_Y1     ,
                                         G4ThreeVector X128_Y1   ,
                                         G4ThreeVector X1_Y128   ,
                                         G4ThreeVector X128_Y128 ,
                                         bool wFirstStage        )
{
   m_DefinitionType.push_back(true) ;

   m_X1_Y1.push_back(X1_Y1)               ;
   m_X128_Y1.push_back(X128_Y1)           ;
   m_X1_Y128.push_back(X1_Y128)           ;
   m_X128_Y128.push_back(X128_Y128)       ;
   m_wFirstStage.push_back(wFirstStage)   ;

   m_R.push_back(0)      ;
   m_Theta.push_back(0)  ;
   m_Phi.push_back(0)    ;
   m_beta_u.push_back(0) ;
   m_beta_v.push_back(0) ;
   m_beta_w.push_back(0) ;


//   m_wNumberStrip.push_back(wNumberStrip);
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HeliosDetDummyShape::AddModule(G4double R        ,
                                         G4double Theta    ,
                                         G4double Phi      ,
                                         G4double beta_u   ,
                                         G4double beta_v   ,
                                         G4double beta_w   ,
                                         bool wFirstStage )
{
   G4ThreeVector empty = G4ThreeVector(0, 0, 0);

   m_DefinitionType.push_back(false);

   m_X1_Y1.push_back(empty)     ;
   m_X128_Y1.push_back(empty)   ;
   m_X1_Y128.push_back(empty)   ;
   m_X128_Y128.push_back(empty) ;

   m_R.push_back(R)                       ;
   m_Theta.push_back(Theta)               ;
   m_Phi.push_back(Phi)                   ;
   m_beta_u.push_back(beta_u)             ;
   m_beta_v.push_back(beta_v)             ;
   m_beta_w.push_back(beta_w)             ;

   m_wFirstStage.push_back(wFirstStage)   ;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void HeliosDetDummyShape::VolumeMaker(G4int TelescopeNumber,
                                           G4ThreeVector MMpos,
                                           G4RotationMatrix* MMrot,
                                           bool wFirstStage,
                                           G4LogicalVolume* world)
{
   G4double NbrTelescopes = TelescopeNumber  ;
   G4String DetectorNumber                   ;
   ostringstream Number                      ;
   Number << NbrTelescopes                   ;
   DetectorNumber = Number.str()             ;

   /////////////////////////////////////////////////////////////////
   /////////////////Element  Definition ///////////////////////////
   ////////////////////////////////////////////////////////////////
   G4String symbol;
   G4double density = 0. , a = 0, z = 0;
   G4int ncomponents = 0;

   ////////////////////////////////////////////////////////////////
   /////////////////Material Definition ///////////////////////////
   ////////////////////////////////////////////////////////////////

   // Si
   a = 28.0855 * g / mole;
   density = 2.321 * g / cm3;
   G4Material* Silicon = new G4Material("Si", z = 14., a, density);

   // Copper
   a = 63.546 * g / mole;
   density = 8.96 * g / cm3;
//   G4Material* Copper;
//   Copper = new G4Material("Cu", z = 29., a, density);

   //  Vacuum
   G4Element* N   = new G4Element("Nitrogen" , symbol = "N"  , z = 7  , a = 14.01  * g / mole);
   G4Element* O   = new G4Element("Oxigen"   , symbol = "O"  , z = 8  , a = 16.00  * g / mole);

   density = 0.000000001 * mg / cm3;
   G4Material* Vacuum = new G4Material("Vacuum", density, ncomponents = 2);
   Vacuum->AddElement(N, .7);
   Vacuum->AddElement(O, .3);

   //  PCB
   G4Element* Si   = new G4Element("Silicon" , symbol = "Si"  , z = 14  , a = 28.0855  * g / mole);
   G4Element* C   = new G4Element("Carbon"   , symbol = "C"  , z = 6  , a = 12.011  * g / mole);
   G4Element* H   = new G4Element("Hydrogen"   , symbol = "H"  , z = 1  , a = 1.0079  * g / mole);
   G4Element* Br   = new G4Element("Bromine"   , symbol = "Br"  , z = 35  , a = 79.904 * g / mole);

   density = 1.7 * g / cm3;
   G4Material* PCB = new G4Material("PCB", density, ncomponents = 5);
   PCB->AddElement(Si, .181);
   PCB->AddElement(O, .406);
   PCB->AddElement(C, .278);
   PCB->AddElement(H, .068);
   PCB->AddElement(Br, .067);


   ////////////////////////////////////////////////////////////////
   ////////////// Starting Volume Definition //////////////////////
   ////////////////////////////////////////////////////////////////
   G4String Name = "HeliosDummyShape" + DetectorNumber ;

   G4Box*           solidHeliosDummyShape = new G4Box(Name, 0.5*FaceFrontWidth, 0.5*FaceFrontLength, 0.5*Thickness);
   G4LogicalVolume* logicHeliosDummyShape = new G4LogicalVolume(solidHeliosDummyShape, Vacuum, Name, 0, 0, 0);

   new G4PVPlacement(G4Transform3D(*MMrot, MMpos) ,
                                     logicHeliosDummyShape           ,
                                     Name                         ,
                                     world                        ,
                                     false                        ,
                                     0);

   //logicHeliosDummyShape->SetVisAttributes(G4VisAttributes::Invisible);
   logicHeliosDummyShape->SetVisAttributes(G4VisAttributes(G4Colour(1, 0., 0.0)));  // red
   //if (m_non_sensitive_part_visiualisation) logicHeliosDummyShape->SetVisAttributes(G4VisAttributes(G4Colour(0.90, 0.90, 0.90)));

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
   ///////////////// First Stage Construction /////////////////////
   ////////////////////////////////////////////////////////////////
   if (wFirstStage) {
      // Silicon detector itself
      G4ThreeVector  positionFirstStage = G4ThreeVector(0, 0, FirstStage_PosZ);

      G4Box*           solidFirstStage = new G4Box("solidFirstStage", 0.5*FirstStageFaceWidth, 0.5*FirstStageFaceLength, 0.5*FirstStageThickness);
      G4LogicalVolume* logicFirstStage = new G4LogicalVolume(solidFirstStage, Silicon, "logicFirstStage", 0, 0, 0);

      new G4PVPlacement(0, 
                                    positionFirstStage, 
                                    logicFirstStage, 
//                                    "G" + DetectorNumber + "FirstStage", 
                                    Name + "_FirstStage", 
                                    logicHeliosDummyShape, 
                                    false, 
                                    0);

      // Set First Stage sensible
      logicFirstStage->SetSensitiveDetector(m_FirstStageScorer);

      ///Visualisation of FirstStage Strip
      G4VisAttributes* FirstStageVisAtt = new G4VisAttributes(G4Colour(1., 0., 0.));  // red
      logicFirstStage->SetVisAttributes(FirstStageVisAtt);


      /*
      G4Box*           solidFirstStageframeIn = new G4Box("solidFirstStageframeIn", 0.5*FaceFront, 0.5*FaceFront, 0.5*1*mm);
      G4Box*           solidFirstStageframeOut = new G4Box("solidFirstStageframeOut", 0.5*FirstStageFace, 0.5*FirstStageFace, 0.5*1.1*mm);
      G4SubtractionSolid* solidFirstStagePCB = new G4SubtractionSolid("solidFirstStagePCB",solidFirstStageframeIn, solidFirstStageframeOut, 0, G4ThreeVector(0.,0.,0.)); 
      G4LogicalVolume* logicFirstStagePCB = new G4LogicalVolume(solidFirstStagePCB, PCB, "logicFirstStagePCB", 0, 0, 0);

      G4ThreeVector positionFirstStagePCB(0.,0.,-0.5*Length+0.5*1*mm);
      PVPBuffer = new G4PVPlacement(0, 
                                    positionFirstStagePCB, 
                                    logicFirstStagePCB, 
//                                    "G" + DetectorNumber + "FirstStage", 
                                    Name + "_FirstStagePCB", 
                                    logicHeliosDummyShape, 
                                    false, 
                                    0);


      ///Visualisation of FirstStage Strip
      G4VisAttributes* FirstStagePCBVisAtt = new G4VisAttributes(G4Colour(0., 0.8, 0.5));  // green
      logicFirstStagePCB->SetVisAttributes(FirstStagePCBVisAtt);
      */

   }



}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void HeliosDetDummyShape::ReadConfiguration(string Path)
{
   ifstream ConfigFile;
   ConfigFile.open(Path.c_str());
   string LineBuffer, DataBuffer; 

   // A:X1_Y1     --> X:1    Y:1
   // B:X128_Y1   --> X:128  Y:1
   // C:X1_Y128   --> X:1    Y:128
   // D:X128_Y128    --> X:128  Y:128

   G4double Ax , Bx , Cx , Dx , Ay , By , Cy , Dy , Az , Bz , Cz , Dz          ;
   G4ThreeVector A , B , C , D                                                 ;
   G4double Theta = 0 , Phi = 0 , R = 0 , beta_u = 0 , beta_v = 0 , beta_w = 0 ;
   int FIRSTSTAGE = 0;
//   int SECONDSTAGE = 0 , THIRDSTAGE = 0                       ;
 //  int NSTRIP = 128;

   bool ReadingStatus = false;

   bool check_A = false;
   bool check_C = false;
   bool check_B = false;
   bool check_D = false;

   bool check_Theta = false;
   bool check_Phi   = false;
   bool check_R     = false;
//   bool check_beta  = false;
   
   bool check_FirstStage = false;
//   bool check_SecondStage = false;
//   bool check_ThirdStage = false;
//   bool check_NStrip = false;
   bool checkVis = false;

   while (!ConfigFile.eof()) {
      getline(ConfigFile, LineBuffer);
      if (LineBuffer.compare(0, 16, "HeliosDummyShape") == 0) {
         G4cout << "///" << G4endl           ;
         G4cout << "DummyShape element found: " << G4endl   ;
         ReadingStatus = true ;
      }
         
      while (ReadingStatus) {
         ConfigFile >> DataBuffer;
         // Comment Line 
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
//            check_beta = true;
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
            FIRSTSTAGE = atoi(DataBuffer.c_str()) ;
         }
/*         else if (DataBuffer.compare(0, 12, "SECONDSTAGE=") == 0) {
//            check_SecondStage = true ;
            ConfigFile >> DataBuffer;
            SECONDSTAGE = atoi(DataBuffer.c_str()) ;
         }
         else if (DataBuffer.compare(0, 11, "THIRDSTAGE=") == 0) {
//            check_ThirdStage = true ;
            ConfigFile >> DataBuffer;
            THIRDSTAGE = atoi(DataBuffer.c_str()) ;
         }

         else if (DataBuffer.compare(0, 7, "NSTRIP=") == 0) {
//            check_NStrip = true ;
            ConfigFile >> DataBuffer;
            NSTRIP = atoi(DataBuffer.c_str()) ;
         }*/

         else if (DataBuffer.compare(0, 4, "VIS=") == 0) {
            checkVis = true ;
            ConfigFile >> DataBuffer;
            if (DataBuffer.compare(0, 3, "all") == 0) m_non_sensitive_part_visiualisation = true;
         }
         
         else G4cout << "WARNING: Wrong Token, HeliosDummyShape: DummyShape Element not added" << G4endl;

         // Add The previously define telescope
         // With position method
         if ((check_A && check_B && check_C && check_D && check_FirstStage && checkVis) && 
             !(check_Theta && check_Phi && check_R)) {
            ReadingStatus = false;
	    check_A = false;
	    check_C = false;
	    check_B = false;
	    check_D = false;
	    check_FirstStage = false;
	    checkVis = false;

            AddModule(A, B, C, D, FIRSTSTAGE  == 1);
         }

         // With angle method
         if ((check_Theta && check_Phi && check_R && check_FirstStage && checkVis) && 
             !(check_A && check_B && check_C && check_D)) {
            ReadingStatus = false;
            check_Theta = false;
   	    check_Phi   = false;
   	    check_R     = false;
//   	    check_beta  = false;
	    check_FirstStage = false;
	    checkVis = false;
		     
            AddModule(R, Theta, Phi, beta_u, beta_v, beta_w, FIRSTSTAGE  == 1);
         }
      }
   }
}



// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void HeliosDetDummyShape::ConstructDetector(G4LogicalVolume* world)
{
   G4RotationMatrix* MMrot    = NULL                   ;
   G4ThreeVector     MMpos    = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     MMu      = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     MMv      = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     MMw      = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     MMCenter = G4ThreeVector(0, 0, 0) ;
   bool FirstStage  = true ;

   G4int NumberOfTelescope = m_DefinitionType.size() ;

   for (G4int i = 0; i < NumberOfTelescope; i++) {
      // By Point
      if (m_DefinitionType[i]) {
         // (u,v,w) unitary vector associated to telescope referencial
         // (u,v) // to silicon plan
         // w perpendicular to (u,v) plan and pointing ThirdStage
         MMu = m_X128_Y1[i] - m_X1_Y1[i]; 
         MMu = MMu.unit();

         MMv = m_X1_Y128[i] - m_X1_Y1[i];
         MMv = MMv.unit();

	 //        G4double MMscal = MMu.dot(MMv);

         MMw = MMu.cross(MMv);
//         if (MMw.z() > 0) MMw = MMv.cross(MMu) ;
         MMw = MMw.unit();

         MMCenter = (m_X1_Y1[i] + m_X1_Y128[i] + m_X128_Y1[i] + m_X128_Y128[i]) / 4;

         // Passage Matrix from Lab Referential to Telescope Referential
         // MUST2
         MMrot = new G4RotationMatrix(MMu, MMv, MMw);
         // translation to place Telescope
         MMpos = MMw * Thickness * 0.5 + MMCenter;
      }

      // By Angle
      else {
         G4double Theta = m_Theta[i] ;
         G4double Phi   = m_Phi[i]   ;

         // (u,v,w) unitary vector associated to telescope referencial
         // (u,v) // to silicon plan
         // w perpendicular to (u,v) plan and pointing ThirdStage
         // Phi is angle between X axis and projection in (X,Y) plan
         // Theta is angle between  position vector and z axis
         G4double wX = m_R[i] * sin(Theta / rad) * cos(Phi / rad);
         G4double wY = m_R[i] * sin(Theta / rad) * sin(Phi / rad);
         G4double wZ = m_R[i] * cos(Theta / rad);
         MMw = G4ThreeVector(wX, wY, wZ);

         // vector corresponding to the center of the module
         G4ThreeVector CT = MMw;

         // vector parallel to one axis of silicon plane
         G4double ii = cos(Theta / rad) * cos(Phi / rad);
         G4double jj = cos(Theta / rad) * sin(Phi / rad);
         G4double kk = -sin(Theta / rad);
         G4ThreeVector Y = G4ThreeVector(ii, jj, kk);

         MMw = MMw.unit();
         MMu = MMw.cross(Y);
         MMv = MMw.cross(MMu);
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
         MMpos = MMw * Thickness * 0.5 + CT ;
      }

      FirstStage  = m_wFirstStage[i]  ;

      VolumeMaker(i + 1, MMpos, MMrot, FirstStage, world);
   }

   delete MMrot ;
}



// Connect the GaspardTrackingData class to the output TTree
// of the simulation
void HeliosDetDummyShape::InitializeRootOutput()
{
}



// Set the TinteractionCoordinates object from VDetector to the present class
void HeliosDetDummyShape::SetInterCoordPointer(TInteractionCoordinates* interCoord)
{
   ms_InterCoord = interCoord;
}



// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void HeliosDetDummyShape::ReadSensitive(const G4Event* event)
{
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



   // Read the Scorer associate to the Silicon Strip
   //Detector Number
   G4int StripDetCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerHeliosDummyShape/DetectorNumber")    ;
   DetectorNumberHitMap = (G4THitsMap<G4int>*)(event->GetHCofThisEvent()->GetHC(StripDetCollectionID))         ;
   DetectorNumber_itr =  DetectorNumberHitMap->GetMap()->begin()                                               ;

   //Energy
   G4int StripEnergyCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerHeliosDummyShape/StripEnergy")   ;
   EnergyHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripEnergyCollectionID))                    ;
   Energy_itr = EnergyHitMap->GetMap()->begin()                                                          ;

   //Time of Flight
   G4int StripTimeCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerHeliosDummyShape/StripTime")    ;
   TimeHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripTimeCollectionID))                        ;
   Time_itr = TimeHitMap->GetMap()->begin()                                                              ;

   //Strip Number X
   G4int StripXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerHeliosDummyShape/StripIDFront")    ;
   XHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripXCollectionID))                              ;
   X_itr = XHitMap->GetMap()->begin()                                                                    ;

   //Strip Number Y
   G4int StripYCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerHeliosDummyShape/StripIDBack");
   YHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripYCollectionID))                              ;
   Y_itr = YHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate X
   G4int InterCoordXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerHeliosDummyShape/InterCoordX")    ;
   PosXHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordXCollectionID))                              ;
   Pos_X_itr = PosXHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Y
   G4int InterCoordYCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerHeliosDummyShape/InterCoordY")    ;
   PosYHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordYCollectionID))                              ;
   Pos_Y_itr = PosYHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Z
   G4int InterCoordZCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerHeliosDummyShape/InterCoordZ")    ;
   PosZHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordZCollectionID))                              ;
   Pos_Z_itr = PosXHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Angle Theta
   G4int InterCoordAngThetaCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerHeliosDummyShape/InterCoordAngTheta")    ;
   AngThetaHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordAngThetaCollectionID))                              ;
   Ang_Theta_itr = AngThetaHitMap->GetMap()->begin()                                                                    ;

   //Interaction Coordinate Angle Phi
   G4int InterCoordAngPhiCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerHeliosDummyShape/InterCoordAngPhi")    ;
   AngPhiHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordAngPhiCollectionID))                              ;
   Ang_Phi_itr = AngPhiHitMap->GetMap()->begin()                                                                    ;




   // Check the size of different map
   G4int sizeN = DetectorNumberHitMap->entries();
   G4int sizeE = EnergyHitMap->entries();
   G4int sizeT = TimeHitMap->entries();
   G4int sizeX = XHitMap->entries();
   G4int sizeY = YHitMap->entries();


   //G4cout << "sizeN:" << sizeN << G4endl;


   if (sizeE != sizeT || sizeT != sizeX || sizeX != sizeY) {
     //G4cout << "sizeE:" << sizeE << G4endl;
     //G4cout << "sizeT:" << sizeT << G4endl;
     //G4cout << "sizeX:" << sizeX << G4endl;
     //G4cout << "sizeY:" << sizeY << G4endl;
     //G4cout << "No match size Si Event Map: sE:"
     // << sizeE << " sT:" << sizeT << " sX:" << sizeX << " sY:" << sizeY << endl ;
      return;
   }

   // Loop on FirstStage number
   for (G4int l = 0; l < sizeN; l++) {
      G4int N     = *(DetectorNumber_itr->second);
      G4int NTrackID =   DetectorNumber_itr->first - N;

      //G4cout <<"N:" <<N << G4endl;
      //G4cout <<"DetectorNumber_itr->first:" << DetectorNumber_itr->first <<  G4endl;
      //G4cout <<"NTrackID:" <<NTrackID << G4endl;


      if (N > 0) {
         // Fill detector number
         ms_Event->SetHeliosFirstStageEDetectorNbr(m_index["DummyShape"] + N);
         ms_Event->SetHeliosFirstStageTDetectorNbr(m_index["DummyShape"] + N);

         // Energy
         for (G4int l = 0 ; l < sizeE ; l++) {
            G4int ETrackID  =   Energy_itr->first - N;
            G4double E     = *(Energy_itr->second);
            if (ETrackID == NTrackID) {
               ms_Event->SetHeliosFirstStageEEnergy(RandGauss::shoot(E, ResoFirstStage));
              }
            Energy_itr++;
         }

         //  Time
         Time_itr = TimeHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeT ; h++) {
            G4int TTrackID  =   Time_itr->first - N;
            G4double T     = *(Time_itr->second);
            if (TTrackID == NTrackID) {
               T = RandGauss::shoot(T, ResoTimeGpd);
               ms_Event->SetHeliosFirstStageTTime(RandGauss::shoot(T, ResoTimeGpd));
           }
            Time_itr++;
         }

         // Strip X
         X_itr = XHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeX ; h++) {
            G4int XTrackID  =   X_itr->first - N;
            G4double X     = *(X_itr->second);
            if (XTrackID == NTrackID) {
	      ms_Event->SetHeliosFirstStageEStripNbr(int(X));
	      ms_Event->SetHeliosFirstStageTStripNbr(int(X));
            }
            X_itr++;
         }

 
         // Pos X
         Pos_X_itr = PosXHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeX ; h++) {
            G4int PosXTrackID =   Pos_X_itr->first - N    ;
            G4double PosX     = *(Pos_X_itr->second)      ;
            if (PosXTrackID == NTrackID) {
               ms_InterCoord->SetDetectedPositionX(PosX) ;
            }
            Pos_X_itr++;
         }

         // Pos Y
         Pos_Y_itr = PosYHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeX ; h++) {
            G4int PosYTrackID =   Pos_Y_itr->first - N    ;
            G4double PosY     = *(Pos_Y_itr->second)      ;
            if (PosYTrackID == NTrackID) {
               ms_InterCoord->SetDetectedPositionY(PosY) ;
            }
            Pos_Y_itr++;
         }

         // Pos Z
         Pos_Z_itr = PosZHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeX ; h++) {
            G4int PosZTrackID =   Pos_Z_itr->first - N    ;
            G4double PosZ     = *(Pos_Z_itr->second)      ;
            if (PosZTrackID == NTrackID) {
	      ms_InterCoord->SetDetectedPositionZ(RandGauss::shoot(PosZ, ResoFirstStage)) ; // for helios !!!!
            }
            Pos_Z_itr++;
         }

         // Angle Theta
         Ang_Theta_itr = AngThetaHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeX ; h++) {
            G4int AngThetaTrackID =   Ang_Theta_itr->first - N    ;
            G4double AngTheta     = *(Ang_Theta_itr->second)      ;
            if (AngThetaTrackID == NTrackID) {
               ms_InterCoord->SetDetectedAngleTheta(AngTheta) ;
            }
            Ang_Theta_itr++;
         }

         // Angle Phi
         Ang_Phi_itr = AngPhiHitMap->GetMap()->begin();
         for (G4int h = 0 ; h < sizeX ; h++) {
            G4int AngPhiTrackID =   Ang_Phi_itr->first - N    ;
            G4double AngPhi     = *(Ang_Phi_itr->second)      ;
            if (AngPhiTrackID == NTrackID) {
               ms_InterCoord->SetDetectedAnglePhi(AngPhi) ;
            }
            Ang_Phi_itr++;
         }


         DetectorNumber_itr++;
      }

      // clear map for next event
      DetectorNumberHitMap    -> clear();
      EnergyHitMap            -> clear();
      TimeHitMap              -> clear();
      XHitMap                 -> clear();
      YHitMap                 -> clear();
      PosXHitMap              -> clear();
      PosYHitMap              -> clear();
      PosZHitMap              -> clear();
      AngThetaHitMap          -> clear();
      AngPhiHitMap            -> clear();
   }
}



void HeliosDetDummyShape::InitializeScorers()
{
   // First stage Associate Scorer
   m_FirstStageScorer                                   = new G4MultiFunctionalDetector("FirstStageScorerHeliosDummyShape");
   G4VPrimitiveScorer* DetNbr                           = new GENERALSCORERS::PSDetectorNumber("DetectorNumber", "HeliosDummyShape", 0);
   G4VPrimitiveScorer* TOF                              = new GENERALSCORERS::PSTOF("StripTime","HeliosDummyShape", 0);
   G4VPrimitiveScorer* InteractionCoordinatesX          = new GENERALSCORERS::PSInteractionCoordinatesX("InterCoordX","HeliosDummyShape", 0);
   G4VPrimitiveScorer* InteractionCoordinatesY          = new GENERALSCORERS::PSInteractionCoordinatesY("InterCoordY","HeliosDummyShape", 0);
   G4VPrimitiveScorer* InteractionCoordinatesZ          = new GENERALSCORERS::PSInteractionCoordinatesZ("InterCoordZ","HeliosDummyShape", 0);
   G4VPrimitiveScorer* InteractionCoordinatesAngleTheta = new GENERALSCORERS::PSInteractionCoordinatesAngleTheta("InterCoordAngTheta","HeliosDummyShape", 0);
   G4VPrimitiveScorer* InteractionCoordinatesAnglePhi   = new GENERALSCORERS::PSInteractionCoordinatesAnglePhi("InterCoordAngPhi","HeliosDummyShape", 0);
   G4VPrimitiveScorer* Energy                           = new HeliosScorerFirstStageEnergy("StripEnergy", "HeliosDummyShape", 0);
   G4VPrimitiveScorer* StripPositionX                   = new HeliosScorerFirstStageFrontStripDummyShape("StripIDFront", 0, NumberOfStrips);
   G4VPrimitiveScorer* StripPositionY                   = new HeliosScorerFirstStageBackStripDummyShape("StripIDBack", 0, NumberOfStrips);

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




   //  Add All Scorer to the Global Scorer Manager
   G4SDManager::GetSDMpointer()->AddNewDetector(m_FirstStageScorer);
}
