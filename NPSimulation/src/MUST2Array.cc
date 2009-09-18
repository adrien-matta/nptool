/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This file describe the MUST2 charge particle Detector                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * MUST2 is a modular array made of Telescope (1 to 8 telescope). Each       *
 *  Telescope is made of Three Stage:                                        *
 *  - A 300um Silicium, double-sided strip                                   *
 *  - 16 Si(Li) pad                                                          *
 *  - 16 CsI scintillator Crystal                                            *
 *****************************************************************************/
#include"MUST2Array.hh"

//G4 Geometry object
#include "G4Trd.hh"
#include "G4Box.hh"
#include "G4Trap.hh"

//G4 various object

#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PVDivision.hh"

//G4 sensitive
#include "GeneralScorers.hh"
#include "Must2Scorers.hh"

//Not G4
#include "RootOutput.h"
#include "CLHEP/Random/RandGauss.h"
#include "sstream"
#include "string"
#include <cmath>

using namespace std  ;
using namespace CLHEP   ;
using namespace MUST2   ;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// MUST2Array Specific Method
MUST2Array::MUST2Array()
{
   m_Event = new TMust2Data() ;
   InitializeMaterial()	;
}

MUST2Array::~MUST2Array()
{
   delete m_MaterialAluminium;
   delete m_MaterialIron;
   delete m_MaterialCsI;
   delete m_MaterialVacuum ;
   delete m_MaterialMyl;
   delete m_MaterialHarvar;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MUST2Array::AddTelescope(G4ThreeVector X1_Y1        ,
      G4ThreeVector X128_Y1      ,
      G4ThreeVector X1_Y128      ,
      G4ThreeVector X128_Y128    ,
      bool wSi                ,
      bool wSiLi              ,
      bool wCsI)
{
   m_DefinitionType.push_back(true) ;

   m_X1_Y1     .push_back(X1_Y1)   ;
   m_X128_Y1   .push_back(X128_Y1)   ;
   m_X1_Y128   .push_back(X1_Y128)   ;
   m_X128_Y128 .push_back(X128_Y128)   ;
   m_wSi.push_back(wSi)          ;
   m_wSiLi.push_back(wSiLi)         ;
   m_wCsI.push_back(wCsI)           ;

   m_R.push_back(0)              ;
   m_Theta.push_back(0)          ;
   m_Phi.push_back(0)               ;
   m_beta_u.push_back(0)            ;
   m_beta_v.push_back(0)            ;
   m_beta_w.push_back(0)            ;
}

void MUST2Array::AddTelescope(G4double R              ,
      G4double Theta          ,
      G4double Phi            ,
      G4double beta_u               ,
      G4double beta_v               ,
      G4double beta_w               ,
      bool wSi                ,
      bool wSiLi              ,
      bool wCsI)
{
   G4ThreeVector empty = G4ThreeVector(0, 0, 0)   ;

   m_DefinitionType.push_back(false)   ;

   m_R.push_back(R)              ;
   m_Theta.push_back(Theta)         ;
   m_Phi.push_back(Phi)          ;
   m_beta_u.push_back(beta_u)       ;
   m_beta_v.push_back(beta_v)       ;
   m_beta_w.push_back(beta_w)       ;
   m_wSi.push_back(wSi)          ;
   m_wSiLi.push_back(wSiLi)         ;
   m_wCsI.push_back(wCsI)           ;

   m_X1_Y1     .push_back(empty)    ;
   m_X128_Y1   .push_back(empty)    ;
   m_X1_Y128   .push_back(empty)    ;
   m_X128_Y128 .push_back(empty)    ;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MUST2Array::VolumeMaker(G4int TelescopeNumber ,
      G4ThreeVector MMpos     ,
      G4RotationMatrix* MMrot ,
      bool wSi          ,
      bool wSiLi           ,
      bool wCsI            ,
      G4LogicalVolume* world)
{
   G4double NbrTelescopes = TelescopeNumber        ;
   G4String DetectorNumber                   ;
   std::ostringstream Number                 ;
   Number << NbrTelescopes                   ;
   DetectorNumber = Number.str()             ;

////////////////////////////////////////////////////////////////
////////////// Starting Volume Definition //////////////////////
////////////////////////////////////////////////////////////////

// Little trick to avoid warning in compilation: Use a PVPlacement "buffer".
// If don't you will have a Warning unused variable 'myPVP'
   G4PVPlacement* PVPBuffer ;


   G4Trd*           solidMM = new G4Trd("MM" + DetectorNumber, 0.5*FaceFront, 0.5*FaceBack, 0.5*FaceFront, 0.5*FaceBack, 0.5*Length);
   G4LogicalVolume* logicMM = new G4LogicalVolume(solidMM, m_MaterialIron, "MM" + DetectorNumber, 0, 0, 0)                                ;

   G4String Name = "MM" + DetectorNumber ;


   PVPBuffer =    new G4PVPlacement(G4Transform3D(*MMrot, MMpos)      ,
         logicMM                    ,
         Name                    ,
         world                   ,
         false                   ,
         0);



   if (m_non_sensitive_part_visiualisation) logicMM->SetVisAttributes(G4VisAttributes(G4Colour(0.90, 0.90, 0.90)))  ;
   else                          logicMM->SetVisAttributes(G4VisAttributes::Invisible)               ;

   G4ThreeVector positionVacBox = G4ThreeVector(0, 0, VacBox_PosZ);

   G4Trd*           solidVacBox = new G4Trd("solidVacBox", 0.5*SiliconFace, 0.5*CsIFaceFront, 0.5*SiliconFace, 0.5*CsIFaceFront, 0.5*VacBoxThickness);
   G4LogicalVolume* logicVacBox = new G4LogicalVolume(solidVacBox, m_MaterialVacuum, "logicVacBox", 0, 0, 0);

   PVPBuffer = new G4PVPlacement(0, positionVacBox, logicVacBox, "M" + DetectorNumber + "VacBox", logicMM, false, 0);

   logicVacBox->SetVisAttributes(G4VisAttributes::Invisible);

// Add a degrader plate between Si and CsI:

   /*
   G4Box*                   Degrader = new G4Box("Degrader" , 50*mm , 50*mm , 0.1*mm );
   G4LogicalVolume*     logicDegrader = new G4LogicalVolume( Degrader , Harvar, "logicDegrader",0,0,0)         ;
     PVPBuffer = new G4PVPlacement(0,G4ThreeVector(0,0,0),logicDegrader,"Degrader",logicVacBox,false,0) ;


      //Place two marker to identify the u and v axis on silicon face:
      //marker are placed a bit before the silicon itself so they don't perturbate simulation
      //Uncomment to help debugging or if you want to understand the way the code work.
      //I should recommand to Comment it during simulation to avoid perturbation of simulation
      //Remember G4 is limitationg step on geometry constraints.

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
      logicMarkerV->SetVisAttributes(MarkerVVisAtt); */

////////////////////////////////////////////////////////////////
/////////////////Si Strip Construction//////////////////////////
////////////////////////////////////////////////////////////////

   if (wSi) {
      G4ThreeVector positionAluStripFront = G4ThreeVector(0, 0, AluStripFront_PosZ);
      G4ThreeVector positionAluStripBack  = G4ThreeVector(0, 0, AluStripBack_PosZ);

      G4Box*           solidAluStrip = new G4Box("AluBox", 0.5*SiliconFace, 0.5*SiliconFace, 0.5*AluStripThickness);
      G4LogicalVolume* logicAluStrip = new G4LogicalVolume(solidAluStrip, m_MaterialAluminium, "logicAluStrip", 0, 0, 0);

      PVPBuffer = new G4PVPlacement(0, positionAluStripFront, logicAluStrip, "M" + DetectorNumber + "AluStripFront", logicMM, false, 0);
      PVPBuffer = new G4PVPlacement(0, positionAluStripBack, logicAluStrip, "M" + DetectorNumber + "AluStripBack", logicMM, false, 0);

      logicAluStrip->SetVisAttributes(G4VisAttributes::Invisible);

      G4ThreeVector  positionSilicon = G4ThreeVector(0, 0, Silicon_PosZ);

      G4Box*           solidSilicon = new G4Box("solidSilicon", 0.5*SiliconFace, 0.5*SiliconFace, 0.5*SiliconThickness);
      G4LogicalVolume* logicSilicon = new G4LogicalVolume(solidSilicon, m_MaterialSilicon, "logicSilicon", 0, 0, 0);

      PVPBuffer = new G4PVPlacement(0, positionSilicon, logicSilicon, "M" + DetectorNumber + "Silicon", logicMM, false, 0);


      ///Set Silicon strip sensible
      logicSilicon->SetSensitiveDetector(m_StripScorer);

      ///Visualisation of Silicon Strip
      G4VisAttributes* SiliconVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5)) ;
      logicSilicon->SetVisAttributes(SiliconVisAtt)                        ;
   }

////////////////////////////////////////////////////////////////
//////////////////// SiLi  Construction ////////////////////////
////////////////////////////////////////////////////////////////

   if (wSiLi) {
      G4double SiLiSpace = 8 * mm;

      G4RotationMatrix* rotSiLi = Rotation(0., 0., 0.);

      G4ThreeVector positionSiLi = G4ThreeVector(-0.25 * SiliconFace - 0.5 * SiLiSpace, 0, 0);
      G4ThreeVector positionSiLi2 = G4ThreeVector(0.25 * SiliconFace + 0.5 * SiLiSpace, 0, 0);

      G4Box* solidSiLi = new G4Box("SiLi", 0.5*SiLiFaceX, 0.5*SiLiFaceY, 0.5*SiLiThickness);

      G4LogicalVolume* logicSiLi = new G4LogicalVolume(solidSiLi, m_MaterialAluminium, "SiLi" + DetectorNumber, 0, 0, 0);

      // First Si(Li) 2 time 4 detectore
      PVPBuffer =  new G4PVPlacement(G4Transform3D(*rotSiLi, positionSiLi)  ,
            logicSiLi                        ,
            "SiLi" + DetectorNumber               ,
            logicVacBox                      ,
            false                         ,
            0);

      // Second Si(Li) 2 time 4 detectore
      PVPBuffer =  new G4PVPlacement(G4Transform3D(*rotSiLi, positionSiLi2) ,
            logicSiLi                        ,
            "SiLi" + DetectorNumber               ,
            logicVacBox                      ,
            false                         ,
            1);

      // SiLi are placed inside of the VacBox...
      // Left/Right define when looking to detector from Si to CsI
      G4double SiLi_HighY_Upper = 19.86 * mm;
      G4double SiLi_HighY_Center = 25.39 * mm ;
      G4double SiLi_WidthX_Left  = 22.85 * mm;
      G4double SiLi_WidthX_Right = 24.9 * mm  ;
      G4double SiLi_ShiftX    = 0.775 * mm ;

      // SiLi : left side of SiLi detector
      G4Box* solidSiLi_LT  = new G4Box("SiLi_LT"  , 0.5*SiLi_WidthX_Left  , 0.5*SiLi_HighY_Upper   , 0.5*SiLiThickness);
      G4Box* solidSiLi_RT  = new G4Box("SiLi_RT"  , 0.5*SiLi_WidthX_Right , 0.5*SiLi_HighY_Upper   , 0.5*SiLiThickness);
      G4Box* solidSiLi_LC1 = new G4Box("SiLi_LC1" , 0.5*SiLi_WidthX_Left  , 0.5*SiLi_HighY_Center  , 0.5*SiLiThickness);
      G4Box* solidSiLi_RC1 = new G4Box("SiLi_RC1" , 0.5*SiLi_WidthX_Right , 0.5*SiLi_HighY_Center  , 0.5*SiLiThickness);
      G4Box* solidSiLi_LB  = new G4Box("SiLi_LB"  , 0.5*SiLi_WidthX_Left  , 0.5*SiLi_HighY_Upper   , 0.5*SiLiThickness);
      G4Box* solidSiLi_RB  = new G4Box("SiLi_RB"  , 0.5*SiLi_WidthX_Right , 0.5*SiLi_HighY_Upper   , 0.5*SiLiThickness);
      G4Box* solidSiLi_LC2 = new G4Box("SiLi_LC2" , 0.5*SiLi_WidthX_Left  , 0.5*SiLi_HighY_Center  , 0.5*SiLiThickness);
      G4Box* solidSiLi_RC2 = new G4Box("SiLi_RC2" , 0.5*SiLi_WidthX_Right , 0.5*SiLi_HighY_Center  , 0.5*SiLiThickness);

      G4LogicalVolume* logicSiLi_LT    = new G4LogicalVolume(solidSiLi_LT   , m_MaterialSilicon , "SiLi_LT"  , 0 , 0 , 0);
      G4LogicalVolume* logicSiLi_RT    = new G4LogicalVolume(solidSiLi_RT   , m_MaterialSilicon , "SiLi_RT"  , 0 , 0 , 0);
      G4LogicalVolume* logicSiLi_LC1   = new G4LogicalVolume(solidSiLi_LC1 , m_MaterialSilicon , "SiLi_LC1"  , 0 , 0 , 0);
      G4LogicalVolume* logicSiLi_RC1   = new G4LogicalVolume(solidSiLi_RC1 , m_MaterialSilicon , "SiLi_RC1"  , 0 , 0 , 0);
      G4LogicalVolume* logicSiLi_LB    = new G4LogicalVolume(solidSiLi_LB   , m_MaterialSilicon , "SiLi_LB"  , 0 , 0 , 0);
      G4LogicalVolume* logicSiLi_RB    = new G4LogicalVolume(solidSiLi_RB   , m_MaterialSilicon , "SiLi_RB"  , 0 , 0 , 0);
      G4LogicalVolume* logicSiLi_LC2   = new G4LogicalVolume(solidSiLi_LC2 , m_MaterialSilicon , "SiLi_LC2"  , 0 , 0 , 0);
      G4LogicalVolume* logicSiLi_RC2   = new G4LogicalVolume(solidSiLi_RC2 , m_MaterialSilicon , "SiLi_RC2"  , 0 , 0 , 0);

      G4double interSiLi = 0.5 * mm;

      // Top
      G4ThreeVector positionSiLi_LT = G4ThreeVector(-0.5 * SiLi_WidthX_Left - interSiLi - SiLi_ShiftX            ,
            0.5 * SiLi_HighY_Upper  + SiLi_HighY_Center + 1.5 * interSiLi   ,
            0);

      G4ThreeVector positionSiLi_RT = G4ThreeVector(0.5 * SiLi_WidthX_Right - SiLi_ShiftX                     ,
            0.5 * SiLi_HighY_Upper  + SiLi_HighY_Center + 1.5 * interSiLi   ,
            0);

      G4ThreeVector positionSiLi_LC1 = G4ThreeVector(-0.5 * SiLi_WidthX_Left - interSiLi - SiLi_ShiftX           ,
            0.5 * SiLi_HighY_Center + 0.5 * interSiLi                 ,
            0);

      G4ThreeVector positionSiLi_RC1 = G4ThreeVector(0.5 * SiLi_WidthX_Right - SiLi_ShiftX                     ,
            0.5 * SiLi_HighY_Center + 0.5 * interSiLi                 ,
            0);

      // Bottom
      G4ThreeVector positionSiLi_LB = G4ThreeVector(-0.5 * SiLi_WidthX_Left - interSiLi - SiLi_ShiftX            ,
            -0.5 * SiLi_HighY_Upper  - SiLi_HighY_Center - 1.5 * interSiLi  ,
            0);

      G4ThreeVector positionSiLi_RB = G4ThreeVector(0.5 * SiLi_WidthX_Right - SiLi_ShiftX                     ,
            -0.5 * SiLi_HighY_Upper  - SiLi_HighY_Center - 1.5 * interSiLi  ,
            0);

      G4ThreeVector positionSiLi_LC2 = G4ThreeVector(-0.5 * SiLi_WidthX_Left - interSiLi - SiLi_ShiftX           ,
            -0.5 * SiLi_HighY_Center - 0.5 * interSiLi                ,
            0);

      G4ThreeVector positionSiLi_RC2 = G4ThreeVector(0.5 * SiLi_WidthX_Right - SiLi_ShiftX                    ,
            -0.5 * SiLi_HighY_Center - 0.5 * interSiLi                ,
            0);

      PVPBuffer = new G4PVPlacement(0 , positionSiLi_LT  , logicSiLi_LT  , "M" + DetectorNumber + "SiLi_LT"  , logicSiLi , false , 0)  ;
      PVPBuffer = new G4PVPlacement(0 , positionSiLi_RT  , logicSiLi_RT  , "M" + DetectorNumber + "SiLi_RT"  , logicSiLi , false , 0)  ;
      PVPBuffer = new G4PVPlacement(0 , positionSiLi_LC1 , logicSiLi_LC1 , "M" + DetectorNumber + "SiLi_LC1" , logicSiLi , false , 0)   ;
      PVPBuffer = new G4PVPlacement(0 , positionSiLi_RC1 , logicSiLi_RC1 , "M" + DetectorNumber + "SiLi_RC1" , logicSiLi , false , 0)   ;

      PVPBuffer = new G4PVPlacement(0 , positionSiLi_LB  , logicSiLi_LB  , "M" + DetectorNumber + "SiLi_LB"  , logicSiLi , false , 0)   ;
      PVPBuffer = new G4PVPlacement(0 , positionSiLi_RB  , logicSiLi_RB  , "M" + DetectorNumber + "SiLi_RB"  , logicSiLi , false , 0)   ;
      PVPBuffer = new G4PVPlacement(0 , positionSiLi_LC2 , logicSiLi_LC2 , "M" + DetectorNumber + "SiLi_LC2" , logicSiLi , false , 0) ;
      PVPBuffer = new G4PVPlacement(0 , positionSiLi_RC2 , logicSiLi_RC2 , "M" + DetectorNumber + "SiLi_RC2" , logicSiLi , false , 0) ;

      logicSiLi->SetVisAttributes(G4VisAttributes(G4Colour(1, 1., 1.)));

      // Set SiLi sensible
      logicSiLi_LT->SetSensitiveDetector(m_SiLiScorer);
      logicSiLi_RT->SetSensitiveDetector(m_SiLiScorer);
      logicSiLi_LC1->SetSensitiveDetector(m_SiLiScorer);
      logicSiLi_RC1->SetSensitiveDetector(m_SiLiScorer);

      logicSiLi_LB->SetSensitiveDetector(m_SiLiScorer);
      logicSiLi_RB->SetSensitiveDetector(m_SiLiScorer);
      logicSiLi_LC2->SetSensitiveDetector(m_SiLiScorer);
      logicSiLi_RC2->SetSensitiveDetector(m_SiLiScorer);

      // Mark blue a SiLi to see telescope orientation
      logicSiLi_LT->SetVisAttributes(G4VisAttributes(G4Colour(0, 0., 1.)));
      logicSiLi_RT->SetVisAttributes(G4VisAttributes(G4Colour(0, 1., 0)));
      logicSiLi_LC1->SetVisAttributes(G4VisAttributes(G4Colour(0, 1., 0)));
      logicSiLi_RC1->SetVisAttributes(G4VisAttributes(G4Colour(0, 1., 0)));

      logicSiLi_LB->SetVisAttributes(G4VisAttributes(G4Colour(0, 1., 0)));
      logicSiLi_RB->SetVisAttributes(G4VisAttributes(G4Colour(0, 1., 0)));
      logicSiLi_LC2->SetVisAttributes(G4VisAttributes(G4Colour(0, 1., 0)));
      logicSiLi_RC2->SetVisAttributes(G4VisAttributes(G4Colour(0, 1., 0)));
   }

////////////////////////////////////////////////////////////////
//////////////////// CsI  Construction//////////////////////////
////////////////////////////////////////////////////////////////

   if (wCsI) {
      //G4double DistInterCsI= 1.*mm;//<-Set the distance between CsI cristal. Exact value yet unknown
      G4double DistInterCsI = 0.2 * mm;

      G4ThreeVector positionCsI = G4ThreeVector(0, 0, CsI_PosZ);
      G4Trd* solidCsI = new G4Trd("csI", 0.5*CsIFaceFront, 0.5*CsIFaceBack, 0.5*CsIFaceFront, 0.5*CsIFaceBack, 0.5*CsIThickness);

      G4LogicalVolume* logicCsI = new G4LogicalVolume(solidCsI, m_MaterialAluminium, "CsI" + DetectorNumber, 0, 0, 0);
      PVPBuffer = new G4PVPlacement(0, positionCsI, logicCsI, "CsI" + DetectorNumber, logicMM, false, 0);

      G4ThreeVector   positionMylarCsI = G4ThreeVector(0, 0, MylarCsIThickness * 0.5 - CsIThickness * 0.5);

      G4Box*           solidMylarCsI = new G4Box("MylarCsIBox", 0.5*CsIFaceFront, 0.5*CsIFaceFront, 0.5*MylarCsIThickness);
      G4LogicalVolume* logicMylarCsI = new G4LogicalVolume(solidMylarCsI, m_MaterialMyl, "logicMylarCsI", 0, 0, 0);

      PVPBuffer = new G4PVPlacement(0, positionMylarCsI, logicMylarCsI, "M" + DetectorNumber + "MylarCsI", logicCsI, false, 0);


      logicCsI->SetVisAttributes(G4VisAttributes::Invisible);
      logicMylarCsI->SetVisAttributes(G4VisAttributes::Invisible);

      // Cristal1
      G4Trap* solidCristal1 = new G4Trap("Cristal1", 40.*mm / 2., 6.693896*deg, 41.97814*deg, 33.1*mm / 2., 37.39*mm / 2., 37.39*mm / 2., 0.*deg, 26.9*mm / 2., 30.41*mm / 2., 30.41*mm / 2., 0.*deg);
      G4LogicalVolume* logicCristal1 = new G4LogicalVolume(solidCristal1, m_MaterialCsI, "Cristal1", 0, 0, 0);

      // Cristal2
      G4Trap* solidCristal2 = new G4Trap("Cristal2", 40.*mm / 2., 17.8836*deg, (74.3122 + 180)*deg, 43.49*mm / 2., 37.39*mm / 2., 37.39*mm / 2., 0.*deg, 31.0377*mm / 2., 30.41*mm / 2., 30.41*mm / 2., 0.*deg);
      G4LogicalVolume* logicCristal2 = new G4LogicalVolume(solidCristal2, m_MaterialCsI, "Cristal2", 0, 0, 0);

      // Cristal3
      G4Trap* solidCristal3 = new G4Trap("Cristal3", 40.*mm / 2., 18.243*deg, 13.5988*deg, 33.11*mm / 2., 39.25*mm / 2., 39.25*mm / 2., 0.*deg, 26.91*mm / 2., 27.58*mm / 2., 27.58*mm / 2., 0.*deg);
      G4LogicalVolume* logicCristal3 = new G4LogicalVolume(solidCristal3, m_MaterialCsI, "Cristal3", 0, 0, 0);

      // Cristal4

      G4Trap* solidCristal4 = new G4Trap("Cristal4", 40.*mm / 2., 24.0482*deg, 44.1148*deg, 43.49*mm / 2., 39.19*mm / 2., 39.19*mm / 2., 0.*deg, 31.04*mm / 2., 27.52*mm / 2., 27.52*mm / 2., 0.*deg);
      G4LogicalVolume* logicCristal4 = new G4LogicalVolume(solidCristal4, m_MaterialCsI, "Cristal4", 0, 0, 0);


      // Cristal1s

      G4Trap* solidCristal1s = new G4Trap("Cristal1s", 40.*mm / 2., 6.693896*deg, -41.97814*deg, 33.1*mm / 2., 37.39*mm / 2., 37.39*mm / 2., 0.*deg, 26.9*mm / 2., 30.41*mm / 2., 30.41*mm / 2., 0.*deg);
      G4LogicalVolume* logicCristal1s = new G4LogicalVolume(solidCristal1s, m_MaterialCsI, "Cristal1s", 0, 0, 0);

      // Cristal2s

      G4Trap* solidCristal2s = new G4Trap("Cristal2s", 40.*mm / 2., 17.8836*deg, -(74.3122 + 180)*deg, 43.49*mm / 2., 37.39*mm / 2., 37.39*mm / 2., 0.*deg, 31.0377*mm / 2., 30.41*mm / 2., 30.41*mm / 2., 0.*deg);
      G4LogicalVolume* logicCristal2s = new G4LogicalVolume(solidCristal2s, m_MaterialCsI, "Cristal2s", 0, 0, 0);

      // Cristal3s

      G4Trap* solidCristal3s = new G4Trap("Cristal3s", 40.*mm / 2., 18.243*deg, -13.5988*deg, 33.11*mm / 2., 39.25*mm / 2., 39.25*mm / 2., 0.*deg, 26.91*mm / 2., 27.58*mm / 2., 27.58*mm / 2., 0.*deg);
      G4LogicalVolume* logicCristal3s = new G4LogicalVolume(solidCristal3s, m_MaterialCsI, "Cristal3s", 0, 0, 0);

      // Cristal4s

      G4Trap* solidCristal4s = new G4Trap("Cristal4s", 40.*mm / 2., 24.0482*deg, -44.1148*deg, 43.49*mm / 2., 39.19*mm / 2., 39.19*mm / 2., 0.*deg, 31.04*mm / 2., 27.52*mm / 2., 27.52*mm / 2., 0.*deg);
      G4LogicalVolume* logicCristal4s = new G4LogicalVolume(solidCristal4s, m_MaterialCsI, "Cristal4s", 0, 0, 0);
      //to see telescope orientation
      G4LogicalVolume* logicCristal4sbis = new G4LogicalVolume(solidCristal4s, m_MaterialCsI, "Cristal4s", 0, 0, 0);


      G4double XEdge1 = 16.96 * mm + DistInterCsI * 0.5;
      G4double YEdge1 = 15.01 * mm + DistInterCsI * 0.5;
      G4double XEdge2 = 50.63 * mm + DistInterCsI * 1.5;
      G4double YEdge2 = 48.67 * mm + DistInterCsI * 1.5;

      G4ThreeVector positionCristal1 = G4ThreeVector(-XEdge1, YEdge1, 0 * mm);
      G4ThreeVector positionCristal2 = G4ThreeVector(-XEdge1, YEdge2, 0);
      G4ThreeVector positionCristal3 = G4ThreeVector(-XEdge2, YEdge1, 0);
      G4ThreeVector positionCristal4 = G4ThreeVector(-XEdge2, YEdge2, 0);

      PVPBuffer = new G4PVPlacement(Rotation(180., 0., 0.), positionCristal1, logicCristal1, "M" + DetectorNumber + "Cristal1", logicCsI, false, 1);
      PVPBuffer = new G4PVPlacement(Rotation(180., 0., 180.), positionCristal2, logicCristal2, "M" + DetectorNumber + "Cristal2", logicCsI, false, 2);
      PVPBuffer = new G4PVPlacement(Rotation(180., 0., 0.), positionCristal3, logicCristal3, "M" + DetectorNumber + "Cristal3", logicCsI, false, 3);
      PVPBuffer = new G4PVPlacement(Rotation(180., 0., 0.), positionCristal4, logicCristal4, "M" + DetectorNumber + "Cristal4", logicCsI, false, 4);


      G4ThreeVector positionCristal1b = G4ThreeVector(XEdge1, -YEdge1, 0 * mm);
      G4ThreeVector positionCristal2b = G4ThreeVector(XEdge1, -YEdge2, 0);
      G4ThreeVector positionCristal3b = G4ThreeVector(XEdge2, -YEdge1, 0);
      G4ThreeVector positionCristal4b = G4ThreeVector(XEdge2, -YEdge2, 0);

      PVPBuffer = new G4PVPlacement(Rotation(180., 0., 180.), positionCristal1b, logicCristal1, "M" + DetectorNumber + "Cristal5", logicCsI, false, 5);
      PVPBuffer = new G4PVPlacement(Rotation(180., 0., 0.), positionCristal2b, logicCristal2, "M" + DetectorNumber + "Cristal6", logicCsI, false, 6);
      PVPBuffer = new G4PVPlacement(Rotation(180., 0., 180.), positionCristal3b, logicCristal3, "M" + DetectorNumber + "Cristal7", logicCsI, false, 7);
      PVPBuffer = new G4PVPlacement(Rotation(180., 0., 180.), positionCristal4b, logicCristal4, "M" + DetectorNumber + "Cristal8", logicCsI, false, 8);

      G4ThreeVector positionCristal1s = G4ThreeVector(-XEdge1, -YEdge1, 0 * mm);
      G4ThreeVector positionCristal2s = G4ThreeVector(-XEdge1, -YEdge2, 0);
      G4ThreeVector positionCristal3s = G4ThreeVector(-XEdge2, -YEdge1, 0);
      G4ThreeVector positionCristal4s = G4ThreeVector(-XEdge2, -YEdge2, 0);

      PVPBuffer = new G4PVPlacement(Rotation(180., 0., 0.), positionCristal1s, logicCristal1s, "M" + DetectorNumber + "Cristal9", logicCsI, false, 9);
      PVPBuffer = new G4PVPlacement(Rotation(180., 0., 180.), positionCristal2s, logicCristal2s, "M" + DetectorNumber + "Cristal10", logicCsI, false, 10);
      PVPBuffer = new G4PVPlacement(Rotation(180., 0., 0.), positionCristal3s, logicCristal3s, "M" + DetectorNumber + "Cristal11", logicCsI, false, 11);
      PVPBuffer = new G4PVPlacement(Rotation(180., 0., 0.), positionCristal4s, logicCristal4sbis, "M" + DetectorNumber + "Cristal12", logicCsI, false, 12);

      G4ThreeVector positionCristal1sb = G4ThreeVector(XEdge1, YEdge1, 0 * mm);
      G4ThreeVector positionCristal2sb = G4ThreeVector(XEdge1, YEdge2, 0);
      G4ThreeVector positionCristal3sb = G4ThreeVector(XEdge2, YEdge1, 0);
      G4ThreeVector positionCristal4sb = G4ThreeVector(XEdge2, YEdge2, 0);

      PVPBuffer = new G4PVPlacement(Rotation(180., 0., 180.), positionCristal1sb, logicCristal1s, "M" + DetectorNumber + "Cristal13", logicCsI, false, 13);
      PVPBuffer = new G4PVPlacement(Rotation(180, 0, 0), positionCristal2sb, logicCristal2s, "M" + DetectorNumber + "Cristal14", logicCsI, false, 14);
      PVPBuffer = new G4PVPlacement(Rotation(180., 0., 180.), positionCristal3sb, logicCristal3s, "M" + DetectorNumber + "Cristal15", logicCsI, false, 15);
      PVPBuffer = new G4PVPlacement(Rotation(180., 0., 180.), positionCristal4sb, logicCristal4s, "M" + DetectorNumber + "Cristal16", logicCsI, false, 16);

      ///Set CsI sensible
      logicCristal1->SetSensitiveDetector(m_CsIScorer);
      logicCristal2->SetSensitiveDetector(m_CsIScorer);
      logicCristal3->SetSensitiveDetector(m_CsIScorer);
      logicCristal4->SetSensitiveDetector(m_CsIScorer);

      logicCristal1s->SetSensitiveDetector(m_CsIScorer);
      logicCristal2s->SetSensitiveDetector(m_CsIScorer);
      logicCristal3s->SetSensitiveDetector(m_CsIScorer);
      logicCristal4s->SetSensitiveDetector(m_CsIScorer);
      logicCristal4sbis->SetSensitiveDetector(m_CsIScorer);

      //Mark blue a CsI corner crystal to see telescope orientation
      logicCristal4sbis->SetVisAttributes(G4VisAttributes(G4Colour(0, 0, 1.)));
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void MUST2Array::ReadConfiguration(string Path)
{
   ifstream ConfigFile           ;
   ConfigFile.open(Path.c_str()) ;
   string LineBuffer          ;
   string DataBuffer          ;

   // A:X1_Y1     --> X:1    Y:1
   // B:X128_Y1   --> X:128  Y:1
   // C:X1_Y128   --> X:1    Y:128
   // D:X128_Y128 --> X:128  Y:128

   G4double Ax , Bx , Cx , Dx , Ay , By , Cy , Dy , Az , Bz , Cz , Dz           	;
   G4ThreeVector A , B , C , D                                          			;
   G4double Theta = 0 , Phi = 0 , R = 0 , beta_u = 0 , beta_v = 0 , beta_w = 0      ;
   int SI = 0 , SILI = 0 , CSI = 0                                         			;	

   bool check_A = false ;
   bool check_C = false  ;
   bool check_B = false ;
   bool check_D = false  ;

   bool check_Theta = false   ;
   bool check_Phi  = false  ;
   bool check_R     = false   ;
   bool check_beta = false  ;
   
   bool check_SI = false   ;
   bool check_SILI  = false  ;
   bool check_CSI     = false   ;
   bool check_VIS = false  ;
   
   bool ReadingStatus = false ;
	

   while (!ConfigFile.eof()) {
      
      	getline(ConfigFile, LineBuffer);

		//	If line is a Start Up MUST2 bloc, Reading toggle to true      
      	if (LineBuffer.compare(0, 11, "M2Telescope") == 0) 
	      	{
	        	 G4cout << "///" << G4endl           ;
	       		  G4cout << "Telescope found: " << G4endl   ;        
	        	 ReadingStatus = true ;
	        	
		   	}
		
		//	Else don't toggle to Reading Block Status
		else ReadingStatus = false ;
		
		//	Reading Block
		while(ReadingStatus)
			{
				 
				ConfigFile >> DataBuffer ;
				//	Comment Line 
					if(DataBuffer.compare(0, 1, "%") == 0) {
						 	ConfigFile.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' );
							
						}
			
					//	Finding another telescope (safety), toggle out
				else if (DataBuffer.compare(0, 11, "M2Telescope") == 0) {
						cout << "WARNING: Another Telescope is find before standard sequence of Token, Error may occured in Telecope definition" << endl ;
						ReadingStatus = false ;
						}
			
					//	Position method
					
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
			
				//	End Position Method

		         //	Angle method
		         else if (DataBuffer.compare(0, 6, "THETA=") == 0) {
		            check_Theta = true;
		            ConfigFile >> DataBuffer ;
		            Theta = atof(DataBuffer.c_str()) ;
		            Theta = Theta * deg;
		            cout << "Theta:  " << Theta / deg << endl;
		            
		         }

		         //Angle method
		         else if (DataBuffer.compare(0, 4, "PHI=") == 0) {
		            check_Phi = true;
		            ConfigFile >> DataBuffer ;
		            Phi = atof(DataBuffer.c_str()) ;
		            Phi = Phi * deg;
		            cout << "Phi:  " << Phi / deg << endl;
		          
		         }

		         //Angle method
		         else if (DataBuffer.compare(0, 2, "R=") == 0) {
		            check_R = true;
		            ConfigFile >> DataBuffer ;
		            R = atof(DataBuffer.c_str()) ;
		            R = R * mm;
		            cout << "R:  " << R / mm << endl;
		          
		         }

		         //Angle method
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

		         //	End Angle Method
		         
		         
		         //	Common part
		         else if (DataBuffer.compare(0, 3, "SI=") == 0) {
		         	check_SI = true ;
		            ConfigFile >> DataBuffer;
		            SI = atof(DataBuffer.c_str()) ;
		            G4cout << " SI= " << SI << G4endl ;
		            
		         }

		        
		         else if (DataBuffer.compare(0, 5, "SILI=") == 0) {
		        	 check_SILI = true ;
		            ConfigFile >> DataBuffer;
		            SILI = atof(DataBuffer.c_str()) ;
		             G4cout << " SILI= " << SILI << G4endl ;
		            
		         }

		         
		         else if (DataBuffer.compare(0, 4, "CSI=") == 0) {
		         	check_CSI = true ;
		            ConfigFile >> DataBuffer;
		            CSI = atof(DataBuffer.c_str()) ;
		             G4cout << " CSI= " << CSI << G4endl ;
		            
		         }

		         
		         else if (DataBuffer.compare(0, 4, "VIS=") == 0) {
		         	check_VIS = true ;
		            ConfigFile >> DataBuffer;
		            if (DataBuffer.compare(0, 3, "all") == 0) 
		           		{
		           			m_non_sensitive_part_visiualisation = true;
		           			G4cout << " VIS= all" << G4endl ;
		           		}
		             
		             else 
		           			G4cout << " VIS= Sensible Only" << G4endl ;	             
		             
		            
		         }
		         
		          // If no MUST2 Token and no comment, toggle out
		         else 
		         	{ReadingStatus = false; G4cout << "other token " << DataBuffer << G4endl ;}
		         
		         
		         
		         /////////////////////////////////////////////////
		         //	If All necessary information there, toggle out
		         if ( ( check_SI && check_SILI && check_CSI && check_VIS ) && ( (check_A && check_B && check_C && check_D) || (check_Theta && check_Phi && check_R) ) ) 
		         	{ 
		         	ReadingStatus = false; 
		         	
		         	///Add The previously define telescope
         			//With position method
		         	if ((check_A && check_B && check_C && check_D) || !(check_Theta && check_Phi && check_R)) 
		         		{
				            AddTelescope(A    ,
				                  B     ,
				                  C     ,
				                  D        ,
				                  SI == 1    ,
				                  SILI == 1  ,
				                  CSI == 1);
				         }
		         	
		         	    //with angle method
       				  else if ((check_Theta && check_Phi && check_R) || !(check_A && check_B && check_C && check_D)) 
       				  	{
					            AddTelescope(R       ,
					                  Theta    ,
					                  Phi   ,
					                  beta_u   ,
					                  beta_v   ,
					                  beta_w   ,
					                  SI == 1    ,
					                  SILI == 1 ,
					                  CSI == 1);
      					}
							
								
			        	check_A = false 	;
		  				check_C = false  	;
		  				check_B = false 	;
		  				check_D = false  	;

		   				check_Theta = false   	;
		   				check_Phi  = false  	;
		   				check_R    = false   	;
		   				check_beta = false  	;
		   
		   				check_SI = false   		;
		   				check_SILI  = false  	;
		   				check_CSI   = false   	;
		   				check_VIS = false  		;	
					
		         	}
		         	
			}
  
   }
}


// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void MUST2Array::ConstructDetector(G4LogicalVolume* world)
{
   G4RotationMatrix* MMrot  	= NULL              		;
   G4ThreeVector     MMpos  	= G4ThreeVector(0, 0, 0) 	;
   G4ThreeVector     MMu       	= G4ThreeVector(0, 0, 0) 	;
   G4ThreeVector     MMv       	= G4ThreeVector(0, 0, 0) 	;
   G4ThreeVector     MMw       	= G4ThreeVector(0, 0, 0) 	;
   G4ThreeVector     MMCenter 	= G4ThreeVector(0, 0, 0)  	;
   bool           	 Si     	= true           			;
   bool          	 SiLi   	= true           			;
   bool           	 CsI    	= true           			;

   G4int             NumberOfTelescope = m_DefinitionType.size() ;

   for (G4int i = 0 ; i < NumberOfTelescope ; i++) {
      // By Point
      if (m_DefinitionType[i]) {
         // (u,v,w) unitary vector associated to telescope referencial
         // (u,v) // to silicon plan
         // w perpendicular to (u,v) plan and pointing CsI
         MMu = m_X128_Y1[i] - m_X1_Y1[i]     ;
         MMu = -MMu.unit()              ;

         MMv = m_X1_Y128[i] - m_X1_Y1[i]     ;
         MMv = MMv.unit()              ;

         MMw = MMu.cross(MMv)          ;
        // if (MMw.z() > 0)MMw = MMv.cross(MMu)  ;
         MMw = MMw.unit()                ;

         MMCenter = (m_X1_Y1[i] + m_X1_Y128[i] + m_X128_Y1[i] + m_X128_Y128[i]) / 4 ;

         // Passage Matrix from Lab Referential to Telescope Referential
         // MUST2
         MMrot = new G4RotationMatrix(MMu, MMv, MMw);
         // translation to place Telescope
         MMpos = MMw * Length * 0.5 + MMCenter ;
      }

      // By Angle
      else {
         G4double Theta = m_Theta[i]   ;
         G4double Phi   = m_Phi[i]  ;
         
         // (u,v,w) unitary vector associated to telescope referencial
         // (u,v) // to silicon plan
         // w perpendicular to (u,v) plan and pointing ThirdStage
         // Phi is angle between X axis and projection in (X,Y) plan
         // Theta is angle between  position vector and z axis
         G4double wX = m_R[i] * sin(Theta / rad) * cos(Phi / rad)   ;
         G4double wY = m_R[i] * sin(Theta / rad) * sin(Phi / rad)   ;
         G4double wZ = m_R[i] * cos(Theta / rad)             ;
         MMw = G4ThreeVector(wX, wY, wZ)                ;

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

      Si  = m_wSi[i]    ;
      SiLi = m_wSiLi[i] ;
      CsI  = m_wCsI[i]  ;

      VolumeMaker(i + 1 , MMpos , MMrot , Si , SiLi , CsI , world);
   }

   delete MMrot ;
}

// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method

void MUST2Array::InitializeRootOutput()
{
   RootOutput *pAnalysis = RootOutput::getInstance();
   TTree *pTree = pAnalysis->GetTree();
   pTree->Branch("MUST2", "TMust2Data", &m_Event) ;
}

// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void MUST2Array::ReadSensitive(const G4Event* event)
{
   G4String DetectorNumber 	;
   m_Event->Clear()			;
   
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////// Used to Read Event Map of detector //////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

// Si
	std::map<G4int, G4int*>::iterator DetectorNumber_itr    ;
	std::map<G4int, G4double*>::iterator Energy_itr     	;
	std::map<G4int, G4double*>::iterator Time_itr    		;
	std::map<G4int, G4double*>::iterator X_itr       		;
	std::map<G4int, G4double*>::iterator Y_itr       		;
	std::map<G4int, G4double*>::iterator Pos_X_itr       	;
	std::map<G4int, G4double*>::iterator Pos_Y_itr       	;
	std::map<G4int, G4double*>::iterator Pos_Z_itr       	;
	std::map<G4int, G4double*>::iterator Ang_Theta_itr      ;
	std::map<G4int, G4double*>::iterator Ang_Phi_itr       	;
	
	G4THitsMap<G4int>*	  DetectorNumberHitMap				;      
	G4THitsMap<G4double>* EnergyHitMap              		;
	G4THitsMap<G4double>* TimeHitMap             			;
	G4THitsMap<G4double>* XHitMap                			;	
	G4THitsMap<G4double>* YHitMap               			;
	G4THitsMap<G4double>* PosXHitMap                		;
	G4THitsMap<G4double>* PosYHitMap                		;
	G4THitsMap<G4double>* PosZHitMap                		;
	G4THitsMap<G4double>* AngThetaHitMap                	;
	G4THitsMap<G4double>* AngPhiHitMap                		;

// NULL pointer are given to avoid warning at compilation

// Si(Li)
   std::map<G4int, G4double*>::iterator SiLiEnergy_itr 		;
   G4THitsMap<G4double>* 				SiLiEnergyHitMap	;
// CsI
   std::map<G4int, G4double*>::iterator CsIEnergy_itr  		;
   G4THitsMap<G4double>* 				CsIEnergyHitMap 	;
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

    // Read the Scorer associate to the Silicon Strip
    
	    //Detector Number
	    G4int StripDetCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("StripScorer/DetectorNumber")  	;
	    DetectorNumberHitMap = (G4THitsMap<G4int>*)(event->GetHCofThisEvent()->GetHC(StripDetCollectionID))       	;
	    DetectorNumber_itr =  DetectorNumberHitMap->GetMap()->begin()                                               ;
	    
	    //Energy
	    G4int StripEnergyCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("StripScorer/StripEnergy")   ;
	    EnergyHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripEnergyCollectionID))           ;
	    Energy_itr = EnergyHitMap->GetMap()->begin()                                                          		;

	    //Time of Flight
	    G4int StripTimeCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("StripScorer/StripTime")    	;
	    TimeHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripTimeCollectionID))               ;
	    Time_itr = TimeHitMap->GetMap()->begin()                                                              		;

	    //Strip Number X
	    G4int StripXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("StripScorer/StripNumberX")    	;
	    XHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripXCollectionID))                     ;
	    X_itr = XHitMap->GetMap()->begin()                                                                    		;

	    //Strip Number Y
	    G4int StripYCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("StripScorer/StripNumberY")    	;
	    YHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(StripYCollectionID))                     ;
	    Y_itr = YHitMap->GetMap()->begin()                                                                    		;

	    //Interaction Coordinate X
	    G4int InterCoordXCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("StripScorer/InterCoordX")    ;
	    PosXHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordXCollectionID))              ;
	    Pos_X_itr = PosXHitMap->GetMap()->begin()                                                                    ;

	    //Interaction Coordinate Y
	    G4int InterCoordYCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("StripScorer/InterCoordY")    ;
	    PosYHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordYCollectionID))              ;
	    Pos_Y_itr = PosYHitMap->GetMap()->begin()                                                                    ;

	    //Interaction Coordinate Z
	    G4int InterCoordZCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("StripScorer/InterCoordZ")    ;
	    PosZHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordZCollectionID))              ;
	    Pos_Z_itr = PosXHitMap->GetMap()->begin()                                                                    ;

	    //Interaction Coordinate Angle Theta
	    G4int InterCoordAngThetaCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("StripScorer/InterCoordAngTheta")    	;
	    AngThetaHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordAngThetaCollectionID))                 	;
	    Ang_Theta_itr = AngThetaHitMap->GetMap()->begin()                                                                    		;

	    //Interaction Coordinate Angle Phi
	    G4int InterCoordAngPhiCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("StripScorer/InterCoordAngPhi")    		;	
	    AngPhiHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(InterCoordAngPhiCollectionID))						;
	    Ang_Phi_itr = AngPhiHitMap->GetMap()->begin()																				;			

    // Read the Scorer associate to the SiLi
        //Energy
        G4int SiLiEnergyCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("SiLiScorer/SiLiEnergy")   ;
        SiLiEnergyHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(SiLiEnergyCollectionID))     ;
        SiLiEnergy_itr = SiLiEnergyHitMap->GetMap()->begin()                                                     ;


    // Read the Scorer associate to the CsI crystal
        //Energy
        G4int CsIEnergyCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("CsIScorer/CsIEnergy")      ;
        CsIEnergyHitMap = (G4THitsMap<G4double>*)(event->GetHCofThisEvent()->GetHC(CsIEnergyCollectionID))       ;
        CsIEnergy_itr = CsIEnergyHitMap->GetMap()->begin()                                                       ;

  	G4int sizeN = DetectorNumberHitMap->entries() 	;
    G4int sizeE = EnergyHitMap->entries() 			;
    G4int sizeT = TimeHitMap->entries() 			;
    G4int sizeX = XHitMap->entries()      			;
    G4int sizeY = YHitMap->entries()    			;

    // Loop on Strip energy
    for (G4int l = 0 ; l < sizeE ; l++) {
        G4int ETrackID  =   Energy_itr->first     ;
        G4double E     = *(Energy_itr->second)    ;
        G4int N = 0								  ;
        
        if (E > 0) {
        
	        m_Event->SetMMStripXEEnergy(RandGauss::shoot(E, ResoStrip))    ;
	        m_Event->SetMMStripYEEnergy(RandGauss::shoot(E, ResoStrip))    ;

			//  Detector Number
	        DetectorNumber_itr = DetectorNumberHitMap->GetMap()->begin();
	        for (G4int h = 0 ; h < sizeN ; h++) {
	            G4int NTrackID  =   DetectorNumber_itr->first       ;
	            G4double Nl     = *(DetectorNumber_itr->second)      ;

	            if (NTrackID == ETrackID) {
	            	N = Nl ;
	                m_Event->SetMMStripXEDetectorNbr(N) ;
	                m_Event->SetMMStripYEDetectorNbr(N) ;
	                m_Event->SetMMStripXTDetectorNbr(N) ;
	                m_Event->SetMMStripYTDetectorNbr(N) ;
	            }
	            
	            else { N = 0 ;}
	            DetectorNumber_itr++;
	        }


	        //  Time
	        Time_itr = TimeHitMap->GetMap()->begin();
	        for (G4int h = 0 ; h < sizeT ; h++) {
	            G4int TTrackID  =   Time_itr->first       ;
	            G4double T     = *(Time_itr->second)      ;

	            if (TTrackID == ETrackID) {
	                m_Event->SetMMStripXTTime(RandGauss::shoot(T, ResoTimeMust)) ;
	                m_Event->SetMMStripYTTime(RandGauss::shoot(T, ResoTimeMust)) ;
	            }
	            Time_itr++;
	        }

	        // X
	        X_itr = XHitMap->GetMap()->begin();
	        for (G4int h = 0 ; h < sizeX ; h++) {
	            G4int XTrackID  =   X_itr->first     ;
	            G4double X     = *(X_itr->second)      ;
	            if (XTrackID == ETrackID) {
	                m_Event->SetMMStripXEStripNbr(X)   ;
	                m_Event->SetMMStripXTStripNbr(X)   ;
	            }

	            X_itr++;
	        }

	        // Y
	        Y_itr = YHitMap->GetMap()->begin()  ;
	        for (G4int h = 0 ; h < sizeY ; h++) {
	            G4int YTrackID  =   Y_itr->first    ;
	            G4double Y     = *(Y_itr->second)      ;
	            if (YTrackID == ETrackID) {
	                m_Event->SetMMStripYEStripNbr(Y)   ;
	                m_Event->SetMMStripYTStripNbr(Y)   ;
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

	        // Si(Li)

	        SiLiEnergy_itr = SiLiEnergyHitMap->GetMap()->begin() ;
	        for (G4int h = 0 ; h < SiLiEnergyHitMap->entries() ; h++) {
	            G4int SiLiEnergyTrackID =   SiLiEnergy_itr->first  ;
	            G4double SiLiEnergy     = *(SiLiEnergy_itr->second)   ;

	            if (SiLiEnergyTrackID == ETrackID) {
	                m_Event->SetMMSiLiEEnergy(RandGauss::shoot(SiLiEnergy, ResoSiLi)) ;
	                m_Event->SetMMSiLiEPadNbr(1);
	                m_Event->SetMMSiLiTPadNbr(1);
	                m_Event->SetMMSiLiTTime(1);
	                m_Event->SetMMSiLiTDetectorNbr(N);
	                m_Event->SetMMSiLiEDetectorNbr(N);
	            }

	            SiLiEnergy_itr++;
	        }
	        

	        // CsI
	      
	        CsIEnergy_itr = CsIEnergyHitMap->GetMap()->begin()  ;
	        for (G4int h = 0 ; h < CsIEnergyHitMap->entries() ; h++) {
	            G4int CsIEnergyTrackID  =   CsIEnergy_itr->first      ;
	            G4double CsIEnergy      = *(CsIEnergy_itr->second)    ;

	            if (CsIEnergyTrackID == ETrackID) {
	                m_Event->SetMMCsIEEnergy(RandGauss::shoot(CsIEnergy, ResoCsI*sqrt(CsIEnergy)));
	                m_Event->SetMMCsIECristalNbr(1);
	                m_Event->SetMMCsITCristalNbr(1);
	                m_Event->SetMMCsITTime(1);
	                m_Event->SetMMCsITDetectorNbr(N);
	                m_Event->SetMMCsIEDetectorNbr(N);
	            }

	            CsIEnergy_itr++;
	        }
	        

        }

        Energy_itr++;
    }
    
    // clear map for next event
    DetectorNumberHitMap              ->clear();
    EnergyHitMap   		->clear() ;
    TimeHitMap     		->clear() ;
    XHitMap        		->clear() ;
    YHitMap        		->clear() ;
    CsIEnergyHitMap		->clear() ;
    SiLiEnergyHitMap	->clear() ;
    PosXHitMap			->clear() ; 
    PosYHitMap			->clear() ;
    PosZHitMap			->clear() ;
    AngThetaHitMap		->clear() ;
    AngPhiHitMap		->clear() ;
     
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MUST2Array::InitializeScorers() 
	{ 
		//	Silicon Associate Scorer
		m_StripScorer = new G4MultiFunctionalDetector("StripScorer");
		
		G4VPrimitiveScorer* DetNbr 								= new MUST2::PSDetectorNumber("DetectorNumber", 0, "Silicon")             	;
		G4VPrimitiveScorer* Energy 								= new PSStripE("StripEnergy", 0)             						;
		G4VPrimitiveScorer* TOF 								= new PSTOF("StripTime", 0)                  						;          					 
		G4VPrimitiveScorer* StripPositionX						= new PSStripNumberX("StripNumberX", 0, SiliconFace, 128)  			;
		G4VPrimitiveScorer* StripPositionY						= new PSStripNumberY("StripNumberY", 0, SiliconFace, 128)   		;  				 
		G4VPrimitiveScorer* InteractionCoordinatesX 			= new PSInteractionCoordinatesX("InterCoordX", 0) 					;
		G4VPrimitiveScorer* InteractionCoordinatesY				= new PSInteractionCoordinatesY("InterCoordY", 0) 					;
		G4VPrimitiveScorer* InteractionCoordinatesZ  			= new PSInteractionCoordinatesZ("InterCoordZ", 0) 					;	 		 
		G4VPrimitiveScorer* InteractionCoordinatesAngleTheta	= new PSInteractionCoordinatesAngleTheta("InterCoordAngTheta", 0) 	;
		G4VPrimitiveScorer* InteractionCoordinatesAnglePhi    	= new PSInteractionCoordinatesAnglePhi("InterCoordAngPhi", 0) 		;	    
	  	
	  	
		//and register it to the multifunctionnal detector
		m_StripScorer->RegisterPrimitive(DetNbr)             				;
		m_StripScorer->RegisterPrimitive(Energy)             				;
		m_StripScorer->RegisterPrimitive(TOF)                				;
		m_StripScorer->RegisterPrimitive(StripPositionX)       	 			;
		m_StripScorer->RegisterPrimitive(StripPositionY)        			;
		m_StripScorer->RegisterPrimitive(InteractionCoordinatesX) 			;
		m_StripScorer->RegisterPrimitive(InteractionCoordinatesY) 			;
		m_StripScorer->RegisterPrimitive(InteractionCoordinatesZ) 			;
		m_StripScorer->RegisterPrimitive(InteractionCoordinatesAngleTheta) 	;
		m_StripScorer->RegisterPrimitive(InteractionCoordinatesAnglePhi) 	;
	  	
	  	//	SiLi Associate Scorer
	  	m_SiLiScorer	= new G4MultiFunctionalDetector("SiLiScorer")			;
	    G4VPrimitiveScorer* SiLiEnergy 			= new PSStripE("SiLiEnergy", 0) ;
		m_SiLiScorer->RegisterPrimitive(SiLiEnergy) 							;
	   
	    //	CsI Associate Scorer 
	    m_CsIScorer	= new G4MultiFunctionalDetector("CsIScorer")				;
	    G4VPrimitiveScorer* CsIEnergy 			= new PSStripE("CsIEnergy", 0) 	;
	    m_CsIScorer->RegisterPrimitive(CsIEnergy)								;
	    
	    //	Add All Scorer to the Global Scorer Manager
	    G4SDManager::GetSDMpointer()->AddNewDetector(m_StripScorer) ;
	    G4SDManager::GetSDMpointer()->AddNewDetector(m_SiLiScorer)	;
	    G4SDManager::GetSDMpointer()->AddNewDetector(m_CsIScorer)	;
	    
	    	    
	}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MUST2Array::InitializeMaterial()
	{
	
	////////////////////////////////////////////////////////////////
/////////////////Element  Definition ///////////////////////////
////////////////////////////////////////////////////////////////
   G4String symbol               ;
   G4double density = 0. , a = 0, z = 0   ;
   G4int ncomponents = 0, natoms = 0  ;

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
   m_MaterialSilicon = new G4Material("Si", z = 14., a, density);

   // Al
   density = 2.702 * g / cm3;
   a = 26.98 * g / mole;
   m_MaterialAluminium = new G4Material("Aluminium", z = 13., a, density);

   // Iron
   density = 7.874 * g / cm3;
   a = 55.847 * g / mole;
   m_MaterialIron = new G4Material("Iron", z = 26., a, density);

   // CsI
   density = 4.51 * g / cm3;
   m_MaterialCsI = new G4Material("CsI", density, ncomponents = 2);
   m_MaterialCsI->AddElement(Cs , natoms = 1);
   m_MaterialCsI->AddElement(I  , natoms = 1);

   //  Vacuum
   density = 0.000000001 * mg / cm3;
   m_MaterialVacuum = new G4Material("Vacuum", density, ncomponents = 2);
   m_MaterialVacuum->AddElement(N, .7);
   m_MaterialVacuum->AddElement(O, .3);

   //  Mylar
   density = 1.397 * g / cm3;
   m_MaterialMyl = new G4Material("Mylar", density, ncomponents = 3);
   m_MaterialMyl->AddElement(C, natoms = 10);
   m_MaterialMyl->AddElement(H, natoms = 8);
   m_MaterialMyl->AddElement(O, natoms = 4);

   // Havar
   m_MaterialHarvar = new G4Material("Havar", 8.3*g / cm3, 5);
   m_MaterialHarvar->AddElement(Co , 42);
   m_MaterialHarvar->AddElement(Cr , 20);
   m_MaterialHarvar->AddElement(Ni , 13);
   m_MaterialHarvar->AddElement(Fe , 19);
   m_MaterialHarvar->AddElement(W  ,  1);

	
	}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4RotationMatrix* Rotation(double tetaX, double tetaY, double tetaZ)
{
   double PI = 3.141592653589793238;
   double radX = tetaX * PI / 180.;
   double radY = tetaY * PI / 180.;
   double radZ = tetaZ * PI / 180.;

   G4ThreeVector col1 = G4ThreeVector(cos(radZ) * cos(radY),
         -sin(radZ) * cos(radY),
         -sin(radY));
   G4ThreeVector col2 = G4ThreeVector(sin(radZ) * cos(radX) - cos(radZ) * sin(radY) * sin(radX),
         cos(radZ) * cos(radX) + sin(radZ) * sin(radY) * sin(radX),
         -cos(radY) * sin(radX));
   G4ThreeVector col3 = G4ThreeVector(sin(radZ) * sin(radX) + cos(radZ) * sin(radY) * sin(radX),
         cos(radZ) * sin(radX) - sin(radZ) * sin(radY) * cos(radX),
         cos(radY) * cos(radX));

   return (new G4RotationMatrix(col1, col2, col3));
}


