/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 10/06/09                                                 *
 * Last update    : 12/10/09                                                 *
 *---------------------------------------------------------------------------*
 * Decription: Define a module of square shape for the Gaspard tracker       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    + 07/09/09: Fix bug for placing module with (r,theta,phi) method.      *
 *                (N. de Sereville)                                          *
 *    + 12/10/09: Change scorer scheme (N. de Sereville)                     *
 *    + 01/10/10: Fix bug with TInteractionCoordinate map size in Read       *
 *                Sensitive (N. de Sereville)                                *
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
#include "GaspardTrackerSquare.hh"
#include "ObsoleteGeneralScorers.hh"
//#include "GaspardScorers.hh"
#include "SiliconScorers.hh"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "RootOutput.h"
#include "NPSVDetector.hh"

// CLHEP
#include "CLHEP/Random/RandGauss.h"

using namespace std;
using namespace CLHEP;

using namespace GPDSQUARE;
//using namespace GPDSCORERS;




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GaspardTrackerSquare::GaspardTrackerSquare()
{
   ms_InterCoord = 0;
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GaspardTrackerSquare::~GaspardTrackerSquare()
{
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GaspardTrackerSquare::AddModule(G4ThreeVector X1_Y1     ,
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
void GaspardTrackerSquare::AddModule(G4double R        ,
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
void GaspardTrackerSquare::VolumeMaker(G4int DetectorNumber,
                                       G4ThreeVector MMpos,
                                       G4RotationMatrix* MMrot,
                                       bool wFirstStage,
                                       bool wSecondStage,
                                       bool wThirdStage,
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
   G4String Name = "GPDSquare" + DetNumber;

   G4Box*           solidGPDSquare = new G4Box(Name, 0.5*FaceFront, 0.5*FaceFront, 0.5*Length);
   G4LogicalVolume* logicGPDSquare = new G4LogicalVolume(solidGPDSquare, m_MaterialVacuum, Name, 0, 0, 0);

   new G4PVPlacement(G4Transform3D(*MMrot, MMpos), logicGPDSquare, Name, world, false, DetectorNumber);


   G4VisAttributes* SquareVisAtt = new G4VisAttributes(G4Colour(0.90, 0.90, 0.90)); 
   SquareVisAtt->SetForceWireframe(true); 
   logicGPDSquare->SetVisAttributes(SquareVisAtt);

   //Place two marker to identify the u and v axis on silicon face:
   //marker are placed a bit before the silicon itself so they don't perturbate simulation
   //Uncomment to help debugging or if you want to understand the way the code work.
   //I should recommand to Comment it during simulation to avoid perturbation of simulation
   //Remember G4 is limitationg step on geometry constraints.
  /* 
         G4ThreeVector positionMarkerU = CT*0.98 + MMu*SiliconFace/4;
         G4Box*          solidMarkerU = new G4Box( "solidMarkerU" , SiliconFace/4 , 1*mm , 1*mm )              ;
         G4LogicalVolume* logicMarkerU = new G4LogicalVolume( solidMarkerU , m_MaterialVacuum , "logicMarkerU",0,0,0)       ;
         PVPBuffer = new G4PVPlacement(G4Transform3D(*MMrot,positionMarkerU),logicMarkerU,"MarkerU",world,false,0) ;

         G4VisAttributes* MarkerUVisAtt= new G4VisAttributes(G4Colour(0.,0.,0.5));//blue
         logicMarkerU->SetVisAttributes(MarkerUVisAtt);

         G4ThreeVector positionMarkerV = CT*0.98 + MMv*SiliconFace/4;
         G4Box*          solidMarkerV = new G4Box( "solidMarkerU" , 1*mm , SiliconFace/4 , 1*mm )              ;
         G4LogicalVolume* logicMarkerV = new G4LogicalVolume( solidMarkerV , m_MaterialVacuum , "logicMarkerV",0,0,0)       ;
         PVPBuffer = new G4PVPlacement(G4Transform3D(*MMrot,positionMarkerV),logicMarkerV,"MarkerV",world,false,0) ;

         G4VisAttributes* MarkerVVisAtt= new G4VisAttributes(G4Colour(0.,0.5,0.5));//green
         logicMarkerV->SetVisAttributes(MarkerVVisAtt);
   */

   ////////////////////////////////////////////////////////////////
   //////////////// First Stage Construction //////////////////////
   ////////////////////////////////////////////////////////////////
   if (wFirstStage) {
      // Silicon detector itself
      G4ThreeVector  positionFirstStage = G4ThreeVector(0, 0, FirstStage_PosZ);

      G4Box*           solidFirstStage = new G4Box("solidFirstStage", 0.5*FirstStageFace, 0.5*FirstStageFace, 0.5*FirstStageThickness);
      G4LogicalVolume* logicFirstStage = new G4LogicalVolume(solidFirstStage, m_MaterialSilicon, "logicFirstStage", 0, 0, 0);

      new G4PVPlacement(0, positionFirstStage, logicFirstStage, Name + "_FirstStage", logicGPDSquare, false, DetectorNumber);

      // Set First Stage sensible
      logicFirstStage->SetSensitiveDetector(m_FirstStageScorer);

      ///Visualisation of FirstStage Strip
      G4VisAttributes* FirstStageVisAtt = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3));   // blue
      logicFirstStage->SetVisAttributes(FirstStageVisAtt);
   }

   ////////////////////////////////////////////////////////////////
   //////////////////// Second Stage  Construction ////////////////
   ////////////////////////////////////////////////////////////////
   if (wSecondStage) {
      // Second stage silicon detector
      G4ThreeVector  positionSecondStage = G4ThreeVector(0, 0, SecondStage_PosZ);

      G4Box*           solidSecondStage = new G4Box("solidSecondStage", 0.5*SecondStageFace, 0.5*SecondStageFace, 0.5*SecondStageThickness);
      G4LogicalVolume* logicSecondStage = new G4LogicalVolume(solidSecondStage, m_MaterialSilicon, "logicSecondStage", 0, 0, 0);

      new G4PVPlacement(0, positionSecondStage, logicSecondStage, Name + "_SecondStage", logicGPDSquare, false, DetectorNumber);

      // Set Second Stage sensible
      logicSecondStage->SetSensitiveDetector(m_SecondStageScorer);

      ///Visualisation of SecondStage Strip
      G4VisAttributes* SecondStageVisAtt = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3)) ;
      logicSecondStage->SetVisAttributes(SecondStageVisAtt)                        ;
   }

   ////////////////////////////////////////////////////////////////
   ///////////////// Third Stage Construction /////////////////////
   ////////////////////////////////////////////////////////////////
   if (wThirdStage) {
      // Third stage silicon detector
      G4ThreeVector  positionThirdStage = G4ThreeVector(0, 0, ThirdStage_PosZ);

      G4Box*           solidThirdStage = new G4Box("solidThirdStage", 0.5*ThirdStageFace, 0.5*ThirdStageFace, 0.5*ThirdStageThickness);
      G4LogicalVolume* logicThirdStage = new G4LogicalVolume(solidThirdStage, m_MaterialSilicon, "logicThirdStage", 0, 0, 0);

      new G4PVPlacement(0, positionThirdStage, logicThirdStage, Name + "_ThirdStage", logicGPDSquare, false, DetectorNumber);

      // Set Third Stage sensible
      logicThirdStage->SetSensitiveDetector(m_ThirdStageScorer);

      ///Visualisation of Third Stage
      G4VisAttributes* ThirdStageVisAtt = new G4VisAttributes(G4Colour(0.3, 0.3, 0.3));   // green
      logicThirdStage->SetVisAttributes(ThirdStageVisAtt);
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void GaspardTrackerSquare::ReadConfiguration(string Path)
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
   
   bool check_FirstStage = false ;
   bool check_SecondStage = false ;
   bool check_ThirdStage = false ;
   bool checkVis = false ;

   while (!ConfigFile.eof()) {
      getline(ConfigFile, LineBuffer);
      if (LineBuffer.compare(0, 9, "GPDSquare") == 0) {
         G4cout << "///" << G4endl           ;
         G4cout << "Square element found: " << G4endl   ;
         ReadingStatus = true ;
         }
         
   while(ReadingStatus){      

         ConfigFile >> DataBuffer;
         //   Comment Line 
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
            G4cout << "X1 Y1 corner position : " << A << G4endl;
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
            G4cout << "X128 Y1 corner position : " << B << G4endl;
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
            G4cout << "X1 Y128 corner position : " << C << G4endl;
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
            G4cout << "X128 Y128 corner position : " << D << G4endl;
         }
         

       // Angle method
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
         
         else G4cout << "WARNING: Wrong Token, GaspardTrackerSquare: Square Element not added" << G4endl;

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
void GaspardTrackerSquare::ConstructDetector(G4LogicalVolume* world)
{
   G4RotationMatrix* MMrot    = NULL                   ;
   G4ThreeVector     MMpos    = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     MMu      = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     MMv      = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     MMw      = G4ThreeVector(0, 0, 0) ;
   G4ThreeVector     MMCenter = G4ThreeVector(0, 0, 0) ;
   bool FirstStage  = true;
   bool SecondStage = true;
   bool ThirdStage  = true;

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

         MMw = MMu.cross(MMv);
         MMw = MMw.unit();

         MMCenter = (m_X1_Y1[i] + m_X1_Y128[i] + m_X128_Y1[i] + m_X128_Y128[i]) / 4;

         // Passage Matrix from Lab Referential to Telescope Referential
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
         MMCenter = MMw;

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
         MMpos = MMw * Length * 0.5 + MMCenter;
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
void GaspardTrackerSquare::InitializeRootOutput()
{
}



// Set the TinteractionCoordinates object from NPS::VDetector to the present class
void GaspardTrackerSquare::SetInterCoordPointer(TInteractionCoordinates* interCoord)
{
   ms_InterCoord = interCoord;
}



// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void GaspardTrackerSquare::ReadSensitive(const G4Event* event)
{
  //////////////
  // First stage
  G4THitsMap<G4double*>* GPD1HitMap;
  std::map<G4int, G4double**>::iterator GPD1_itr;

  G4int GPD1CollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("FirstStageScorerGPDSquare/GPDSquareFirstStage");
  GPD1HitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(GPD1CollectionID));

  // Loop on the GPD map
  for (GPD1_itr = GPD1HitMap->GetMap()->begin(); GPD1_itr != GPD1HitMap->GetMap()->end(); GPD1_itr++) {
     G4double* Info = *(GPD1_itr->second);

     double Energy = Info[0];
     if (Energy > EnergyThreshold) {
        double Time       = Info[1];
        int DetNbr        = (int) Info[7];
        int StripFront    = (int) Info[8];
        int StripBack     = (int) Info[9];

        // detector number
        ms_Event->SetGPDTrkFirstStageFrontEDetectorNbr(m_index["Square"] + DetNbr);
        ms_Event->SetGPDTrkFirstStageFrontTDetectorNbr(m_index["Square"] + DetNbr);
        ms_Event->SetGPDTrkFirstStageBackEDetectorNbr(m_index["Square"] + DetNbr);
        ms_Event->SetGPDTrkFirstStageBackTDetectorNbr(m_index["Square"] + DetNbr);
        
        // energy
        ms_Event->SetGPDTrkFirstStageFrontEEnergy(RandGauss::shoot(Energy, ResoFirstStage));
        ms_Event->SetGPDTrkFirstStageBackEEnergy(RandGauss::shoot(Energy, ResoFirstStage));

        // time
        Time = RandGauss::shoot(Time, ResoTimePPAC);
        ms_Event->SetGPDTrkFirstStageFrontTTime(RandGauss::shoot(Time, ResoTimeGpd));
        ms_Event->SetGPDTrkFirstStageBackTTime(RandGauss::shoot(Time, ResoTimeGpd));

        // strips X and Y
        ms_Event->SetGPDTrkFirstStageFrontEStripNbr(StripFront);
        ms_Event->SetGPDTrkFirstStageFrontTStripNbr(StripFront);
        ms_Event->SetGPDTrkFirstStageBackEStripNbr(StripBack);
        ms_Event->SetGPDTrkFirstStageBackTStripNbr(StripBack);

        // Interaction Coordinates
        ms_InterCoord->SetDetectedPositionX(Info[2]);
        ms_InterCoord->SetDetectedPositionY(Info[3]);
        ms_InterCoord->SetDetectedPositionZ(Info[4]);
        ms_InterCoord->SetDetectedAngleTheta(Info[5]/deg);
        ms_InterCoord->SetDetectedAnglePhi(Info[6]/deg);

     }
  }
  // clear map for next event
  GPD1HitMap->clear();


  //////////////
  // Second stage
  G4THitsMap<G4double*>* GPD2HitMap;
  std::map<G4int, G4double**>::iterator GPD2_itr;

  G4int GPD2CollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("SecondStageScorerGPDSquare/GPDSquareSecondStage");
  GPD2HitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(GPD2CollectionID));

  // Loop on the GPD map
  for (GPD2_itr = GPD2HitMap->GetMap()->begin(); GPD2_itr != GPD2HitMap->GetMap()->end(); GPD2_itr++) {
     G4double* Info = *(GPD2_itr->second);

     double Energy = Info[0];
     if (Energy > EnergyThreshold) {
        double Time       = Info[1];
        int DetNbr        = (int) Info[7];
        int StripFront    = (int) Info[8];

        // detector number
        ms_Event->SetGPDTrkSecondStageEDetectorNbr(m_index["Square"] + DetNbr);
        ms_Event->SetGPDTrkSecondStageTDetectorNbr(m_index["Square"] + DetNbr);
        
        // energy
        ms_Event->SetGPDTrkSecondStageEEnergy(RandGauss::shoot(Energy, ResoSecondStage));

        // time
        Time = RandGauss::shoot(Time, ResoTimePPAC);
        ms_Event->SetGPDTrkSecondStageTTime(RandGauss::shoot(Time, ResoTimeGpd));

        // strips X and Y
        ms_Event->SetGPDTrkSecondStageEPadNbr(StripFront);
        ms_Event->SetGPDTrkSecondStageTPadNbr(StripFront);
     }
  }
  // clear map for next event
  GPD2HitMap->clear();


  //////////////
  // Third stage
  G4THitsMap<G4double*>* GPD3HitMap;
  std::map<G4int, G4double**>::iterator GPD3_itr;

  G4int GPD3CollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("ThirdStageScorerGPDSquare/GPDSquareThirdStage");
  GPD3HitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(GPD3CollectionID));

  // Loop on the GPD map
  for (GPD3_itr = GPD3HitMap->GetMap()->begin(); GPD3_itr != GPD3HitMap->GetMap()->end(); GPD3_itr++) {
     G4double* Info = *(GPD3_itr->second);

     double Energy = Info[0];
     if (Energy > EnergyThreshold) {
        double Time       = Info[1];
        int DetNbr        = (int) Info[7];
        int StripFront    = (int) Info[8];

        // detector number
        ms_Event->SetGPDTrkThirdStageEDetectorNbr(m_index["Square"] + DetNbr);
        ms_Event->SetGPDTrkThirdStageTDetectorNbr(m_index["Square"] + DetNbr);
        
        // energy
        ms_Event->SetGPDTrkThirdStageEEnergy(RandGauss::shoot(Energy, ResoThirdStage));

        // time
        Time = RandGauss::shoot(Time, ResoTimePPAC);
        ms_Event->SetGPDTrkThirdStageTTime(RandGauss::shoot(Time, ResoTimeGpd));

        // strips X and Y
        ms_Event->SetGPDTrkThirdStageEPadNbr(StripFront);
        ms_Event->SetGPDTrkThirdStageTPadNbr(StripFront);
     }
  }
  // clear map for next event
  GPD3HitMap->clear();
}



void GaspardTrackerSquare::InitializeScorers()
{
   // check whether scorers are already defined
   bool already_exist  = false;
   m_FirstStageScorer  = NPS::VDetector::CheckScorer("FirstStageScorerGPDSquare",  already_exist);
   m_SecondStageScorer = NPS::VDetector::CheckScorer("SecondStageScorerGPDSquare", already_exist);
   m_ThirdStageScorer  = NPS::VDetector::CheckScorer("ThirdStageScorerGPDSquare",  already_exist);
   if (already_exist) return;

   // First stage scorer
   G4VPrimitiveScorer* GPDScorerFirstStage =
      new SILICONSCORERS::PS_Silicon_Rectangle("GPDSquareFirstStage",
                                               0,
                                               FirstStageFace,
                                               FirstStageFace,
                                               NumberOfStrips,
                                               NumberOfStrips);

   // Second stage scorer
   G4VPrimitiveScorer* GPDScorerSecondStage =
      new SILICONSCORERS::PS_Silicon_Rectangle("GPDSquareSecondStage",
                                               0,
                                               SecondStageFace,
                                               SecondStageFace,
                                               1,
                                               1);

   // Third stage scorer
   G4VPrimitiveScorer* GPDScorerThirdStage =
      new SILICONSCORERS::PS_Silicon_Rectangle("GPDSquareThirdStage",
                                               0,
                                               ThirdStageFace,
                                               ThirdStageFace,
                                               1,
                                               1);

   // register scorers to the multifunctionnal detector
   m_FirstStageScorer  ->RegisterPrimitive(GPDScorerFirstStage);
   m_SecondStageScorer ->RegisterPrimitive(GPDScorerSecondStage);
   m_ThirdStageScorer  ->RegisterPrimitive(GPDScorerThirdStage);

   //  Add All Scorer to the Global Scorer Manager
   G4SDManager::GetSDMpointer()->AddNewDetector(m_FirstStageScorer);
   G4SDManager::GetSDMpointer()->AddNewDetector(m_SecondStageScorer);
   G4SDManager::GetSDMpointer()->AddNewDetector(m_ThirdStageScorer);
}
