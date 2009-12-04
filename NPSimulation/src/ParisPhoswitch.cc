/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 04/12/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: Define a phoswitch module for the Paris detector.             *
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
#include "G4Trd.hh"
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
#include "ParisPhoswitch.hh"
#include "GeneralScorers.hh"
#include "RootOutput.h"

// CLHEP
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;
using namespace PARISPHOSWITCH;



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ParisPhoswitch::ParisPhoswitch()
{
   ms_InterCoord = 0;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ParisPhoswitch::~ParisPhoswitch()
{
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ParisPhoswitch::AddModule(G4ThreeVector X1_Y1,
                               G4ThreeVector X128_Y1,
                               G4ThreeVector X1_Y128,
                               G4ThreeVector X128_Y128)
{
   m_DefinitionType.push_back(true);

   m_X1_Y1.push_back(X1_Y1);
   m_X128_Y1.push_back(X128_Y1);
   m_X1_Y128.push_back(X1_Y128);
   m_X128_Y128.push_back(X128_Y128);

   m_R.push_back(0);
   m_Theta.push_back(0);
   m_Phi.push_back(0);
   m_beta_u.push_back(0);
   m_beta_v.push_back(0);
   m_beta_w.push_back(0);
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ParisPhoswitch::AddModule(G4double R,
                               G4double Theta,
                               G4double Phi,
                               G4double beta_u,
                               G4double beta_v,
                               G4double beta_w) 
{
   G4ThreeVector empty = G4ThreeVector(0, 0, 0);

   m_DefinitionType.push_back(false);

   m_X1_Y1.push_back(empty);
   m_X128_Y1.push_back(empty);
   m_X1_Y128.push_back(empty);
   m_X128_Y128.push_back(empty);

   m_R.push_back(R);
   m_Theta.push_back(Theta);
   m_Phi.push_back(Phi);
   m_beta_u.push_back(beta_u);
   m_beta_v.push_back(beta_v);
   m_beta_w.push_back(beta_w);
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ParisPhoswitch::VolumeMaker(G4int             DetecNumber,
                                 G4ThreeVector     MMpos,
                                 G4RotationMatrix* MMrot,
                                 G4LogicalVolume*  world)
{
   G4double NbrTelescopes = DetecNumber;
   G4String DetectorNumber;
   ostringstream Number;
   Number << NbrTelescopes;
   DetectorNumber = Number.str();

   /////////////////////////////////////////////////////////////////
   /////////////////Element  Definition ///////////////////////////
   ////////////////////////////////////////////////////////////////
   G4String symbol, name;
   G4double density = 0. , a = 0, z = 0;
   G4int ncomponents = 0;
   G4int nel = 0, natoms = 0;

   ////////////////////////////////////////////////////////////////
   /////////////////Material Definition ///////////////////////////
   ////////////////////////////////////////////////////////////////
   a=137.327*g/mole;
   G4Element* Ba = new G4Element(name="Barium",symbol="Ba",z=56.,a);
   a=18.9984032*g/mole;
   G4Element* F = new G4Element(name="Fluorine",symbol="F",z=9.,a);
   a=22.99*g/mole;
   G4Element* Na = new G4Element(name="Sodium",symbol="Na",z=11.,a);
   a=79.904*g/mole;
   G4Element* Br = new G4Element(name="Bromine",symbol="Br",z=35.,a);
   a=126.90477*g/mole;
   G4Element* I  = new G4Element(name="Iodine",symbol="I",z=53.,a);
   a=132.90545*g/mole;
   G4Element* Cs = new G4Element(name="Cesium",symbol="Cs",z=55.,a);
   a=138.9055*g/mole;
   G4Element* La = new G4Element(name="Lanthanum",symbol="La",z=57.,a);

   //  Vacuum
   G4Element* N   = new G4Element("Nitrogen" , symbol = "N"  , z = 7  , a = 14.01  * g / mole);
   G4Element* O   = new G4Element("Oxigen"   , symbol = "O"  , z = 8  , a = 16.00  * g / mole);

   density = 0.000000001 * mg / cm3;
   G4Material* Vacuum = new G4Material("Vacuum", density, ncomponents = 2);
   Vacuum->AddElement(N, .7);
   Vacuum->AddElement(O, .3);

   // NaI
   density = 3.67*g/cm3, nel = 2; 
   G4Material* NaI = new G4Material(name="NaI",density,nel);
   NaI->AddElement(Na, natoms = 1);
   NaI->AddElement(I,  natoms = 1);

   // CsI
   density  = 4.51*g/cm3, nel = 2; 
   G4Material* CsI = new G4Material(name="CsI", density, nel);
   CsI->AddElement(Cs, natoms = 1);
   CsI->AddElement(I,  natoms = 1);

   // LaBr3
   density = 5.29*g/cm3, nel = 2; 
   G4Material* LaBr3 = new G4Material(name="LaBr3",density,nel);
   LaBr3->AddElement(La, natoms = 1);
   LaBr3->AddElement(Br, natoms = 3);

   // BaF2
   density = 4.89*g/cm3, nel = 2;
   G4Material* BaF2 = new G4Material(name="BaF2", density, nel);
   BaF2->AddElement(Ba, natoms = 1);
   BaF2->AddElement(F,  natoms = 2);

   ////////////////////////////////////////////////////////////////
   ////////////// Starting Volume Definition //////////////////////
   ////////////////////////////////////////////////////////////////
   // Little trick to avoid warning in compilation: Use a PVPlacement "buffer".
   // If don't you will have a Warning unused variable 'myPVP'
   G4PVPlacement* PVPBuffer;
   G4String Name = "ParisPhoswitch" + DetectorNumber ;

   // Mother Volume
   G4Box*           solidParisPhoswitch = new G4Box(Name, 0.5*FaceFront, 0.5*FaceFront, 0.5*Length);
   G4LogicalVolume* logicParisPhoswitch = new G4LogicalVolume(solidParisPhoswitch, Vacuum, Name, 0, 0, 0);

   PVPBuffer     = new G4PVPlacement(G4Transform3D(*MMrot, MMpos) ,
                                     logicParisPhoswitch           ,
                                     Name                         ,
                                     world                        ,
                                     false                        ,
                                     0);

   logicParisPhoswitch->SetVisAttributes(G4VisAttributes::Invisible);
   if (m_non_sensitive_part_visiualisation) logicParisPhoswitch->SetVisAttributes(G4VisAttributes(G4Colour(0.90, 0.90, 0.90)));

   // Phoswitch construction
   // LaBr3
   G4ThreeVector  positionLaBr3Stage = G4ThreeVector(0, 0, LaBr3Stage_PosZ);

   G4Box*           solidLaBr3Stage = new G4Box("solidLaBr3Stage", 0.5*LaBr3Face, 0.5*LaBr3Face, 0.5*LaBr3Thickness);
   G4LogicalVolume* logicLaBr3Stage = new G4LogicalVolume(solidLaBr3Stage, LaBr3, "logicLaBr3Stage", 0, 0, 0);

   PVPBuffer = new G4PVPlacement(0, 
                                 positionLaBr3Stage, 
                                 logicLaBr3Stage, 
                                 Name + "_LaBr3Stage", 
                                 logicParisPhoswitch, 
                                 false, 
                                 0);

   // Set LaBr3 sensible
   logicLaBr3Stage->SetSensitiveDetector(m_LaBr3StageScorer);

   // Visualisation of LaBr3Stage Strip
   G4VisAttributes* LaBr3VisAtt = new G4VisAttributes(G4Colour(0., 0., 1.));
   logicLaBr3Stage->SetVisAttributes(LaBr3VisAtt);

   // CsI
   G4ThreeVector  positionCsIStage = G4ThreeVector(0, 0, CsIStage_PosZ);

   G4Box*           solidCsIStage = new G4Box("solidCsIStage", 0.5*CsIFace, 0.5*CsIFace, 0.5*CsIThickness);
   G4LogicalVolume* logicCsIStage = new G4LogicalVolume(solidCsIStage, CsI, "logicCsIStage", 0, 0, 0);

   PVPBuffer = new G4PVPlacement(0, 
                                 positionCsIStage, 
                                 logicCsIStage, 
                                 Name + "_CsIStage", 
                                 logicParisPhoswitch, 
                                 false, 
                                 0);

   // Set CsI sensible
   logicCsIStage->SetSensitiveDetector(m_CsIStageScorer);

   // Visualisation of CsIStage Strip
   G4VisAttributes* CsIVisAtt = new G4VisAttributes(G4Colour(1., 0., 0.));
   logicCsIStage->SetVisAttributes(CsIVisAtt);
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void ParisPhoswitch::ReadConfiguration(string Path)
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

   bool ReadingStatus = false;

   bool check_A = false;
   bool check_C = false;
   bool check_B = false;
   bool check_D = false;

   bool check_Theta = false;
   bool check_Phi   = false;
   bool check_R     = false;
   bool check_beta  = false;
   
   bool checkVis = false;

   while (!ConfigFile.eof()) {
      getline(ConfigFile, LineBuffer);
      if (LineBuffer.compare(0, 14, "ParisPhoswitch") == 0) {
         G4cout << "///" << G4endl           ;
         G4cout << "Phoswitch element found: " << G4endl   ;
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

         else if (DataBuffer.compare(0, 4, "VIS=") == 0) {
            checkVis = true ;
            ConfigFile >> DataBuffer;
            if (DataBuffer.compare(0, 3, "all") == 0) m_non_sensitive_part_visiualisation = true;
         }
         
         else G4cout << "WARNING: Wrong Token, ParisPhoswitch: Phoswitch Element not added" << G4endl;

         // Add The previously define telescope
         // With position method
         if ((check_A && check_B && check_C && check_D && checkVis) && 
             !(check_Theta && check_Phi && check_R)) {
            ReadingStatus = false;
	    check_A = false;
	    check_C = false;
	    check_B = false;
	    check_D = false;
	    checkVis = false;

            AddModule(A, B, C, D);
         }

         // With angle method
         if ((check_Theta && check_Phi && check_R && checkVis) && 
             !(check_A && check_B && check_C && check_D)) {
            ReadingStatus = false;
            check_Theta = false;
   	    check_Phi   = false;
   	    check_R     = false;
   	    check_beta  = false;
	    checkVis = false;
		     
            AddModule(R, Theta, Phi, beta_u, beta_v, beta_w);
         }
      }
   }
}



// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void ParisPhoswitch::ConstructDetector(G4LogicalVolume* world)
{
   G4RotationMatrix* MMrot    = NULL                   ;
   G4ThreeVector     MMpos    = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     MMu      = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     MMv      = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     MMw      = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     MMCenter = G4ThreeVector(0, 0, 0) ;

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

         G4ThreeVector MMscal = MMu.dot(MMv);

         MMw = MMu.cross(MMv);
//         if (MMw.z() > 0) MMw = MMv.cross(MMu) ;
         MMw = MMw.unit();

         MMCenter = (m_X1_Y1[i] + m_X1_Y128[i] + m_X128_Y1[i] + m_X128_Y128[i]) / 4;

         // Passage Matrix from Lab Referential to Telescope Referential
         // MUST2
         MMrot = new G4RotationMatrix(MMu, MMv, MMw);
         // translation to place Telescope
         MMpos = MMw * Length * 0.5 + MMCenter;
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
         MMpos = MMw * Length * 0.5 + CT ;
      }

      VolumeMaker(i + 1, MMpos, MMrot, world);
   }

   delete MMrot ;
}



// Connect the ParisData class to the output TTree
// of the simulation
void ParisPhoswitch::InitializeRootOutput()
{
}



// Set the TinteractionCoordinates object from VDetector to the present class
void ParisPhoswitch::SetInterCoordPointer(TInteractionCoordinates* interCoord)
{
   ms_InterCoord = interCoord;
}



// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void ParisPhoswitch::ReadSensitive(const G4Event* event)
{
}



void ParisPhoswitch::InitializeScorers()
{
   // LaBr3 Associate Scorer
   m_LaBr3StageScorer = new G4MultiFunctionalDetector("LaBr3StageScorerParisPhoswitch");

   // Second stage Associate Scorer
   m_CsIStageScorer = new G4MultiFunctionalDetector("CsIStageScorerParisPhoswitch");

   //  Add All Scorer to the Global Scorer Manager
   G4SDManager::GetSDMpointer()->AddNewDetector(m_LaBr3StageScorer);
   G4SDManager::GetSDMpointer()->AddNewDetector(m_CsIStageScorer);
}
