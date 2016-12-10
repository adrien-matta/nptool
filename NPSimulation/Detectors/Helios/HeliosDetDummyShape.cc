/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche    contact address: marc.labiche@stfc.ac.uk *
 *                                                                           *
 * Creation Date  : 31/01/12                                                 *
 * Last update    : 31/08/15                                                 *
 *---------------------------------------------------------------------------*
 * Decription: Define a dummy module for the Helios detector                 *
 *             The goal of this class is to be a starting point to create a  *
 *             new shape to be added to the Helios detector.                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * For Helios, the shape is a rectangular box (w~1cm, l~5cm, d~700um)        *
 * For Helisol, the shape is a rectangular box (w~2.2cm, l~12.5cm, d~1000um) *
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
#include "G4Tubs.hh"
#include "G4Trap.hh"
#include "G4Polyhedra.hh"
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

// For the magnetic field
#include "MyMagneticField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"
#include "G4MagIntegratorStepper.hh"

// G4 sensitive
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "NPSVDetector.hh"

// NPTool headers
#include "HeliosDetDummyShape.hh"
#include "ObsoleteGeneralScorers.hh"
#include "HeliosScorers.hh"
#include "RootOutput.h"
#include "NPOptionManager.h"
#include "NPSDetectorFactory.hh"
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

   // Al
   a = 26.98 * g / mole;
   density = 2.7 * g / cm3;
   G4Material* Aluminium = new G4Material("Al", z = 13., a, density);

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

   //
   //  Detector support structure (rods)
   //
     // Add the Aluminium rod

      /* For helios: */ 
      G4double Al_rod_x = 1. * cm; // half width
      G4double Al_rod_y = 1. * cm; // half height
      G4double Al_rod_z = 20.0 * cm; // half length
      G4Box* Al_rod_box
      = new G4Box("Al_rod_box", Al_rod_x, Al_rod_y, Al_rod_z);
      G4Tubs* Al_rod_tub
      = new G4Tubs("Al_rod_tub", 0, 0.5*cm, Al_rod_z+1.*mm, 0.*deg, 360*deg);
      G4SubtractionSolid* Al_rod=new G4SubtractionSolid("Rod",Al_rod_box, Al_rod_tub, 0, G4ThreeVector(0.,0.,0.));

      G4LogicalVolume* Al_rod_log = new G4LogicalVolume(Al_rod, Aluminium, "Al_rod", 0, 0, 0);
         
      //new G4PVPlacement(0, G4ThreeVector(0.,0., Al_rod_z+1.*mm + 12.5*cm), Al_rod_log, "Al_rod", world, false, 0);  // 150
      //new G4PVPlacement(0, G4ThreeVector(0.,0., -(Al_rod_z+1.*mm + 12.5*cm)), Al_rod_log, "Al_rod", world, false, 1); // -150
      //new G4PVPlacement(0, G4ThreeVector(0.,0., Al_rod_z+1.*mm + 8.5*cm), Al_rod_log, "Al_rod", world, false, 0);  // 100
      new G4PVPlacement(0, G4ThreeVector(0.,0., -(Al_rod_z+1.*mm + 8.5*cm)), Al_rod_log, "Al_rod", world, false, 1); // -100
      
      

	  /* For helisol: */ /*
       const G4double Zplan[2]={140*mm, 670*mm}; // if first silicons are at 150mm to target (helisol.detector)
      //const G4double Zplan[2]={90*mm, 620*mm}; // if first silicon at 100mm to target (helisol_2.detector)
      //const G4double Zplan[2]={40*mm, 570*mm}; // if first silicon at 50mm to target (helisol_3.detector)
       const G4double Rinner[2]={15*mm, 15*mm};
       const G4double Router[2]={24*mm, 24*mm};
       G4Polyhedra* Al_rod= new G4Polyhedra("Al_rod", 0., 360*deg, 6, 2, Zplan, Rinner, Router);      
      
       G4LogicalVolume* Al_rod_log = new G4LogicalVolume(Al_rod, Aluminium, "Al_rod", 0, 0, 0);
         
       G4RotationMatrix* RotZ30Y180 = new G4RotationMatrix();
       RotZ30Y180->rotateY(180*deg);
       RotZ30Y180->rotateZ(30*deg);
       G4RotationMatrix* RotZ30 = new G4RotationMatrix();
       RotZ30->rotateZ(30*deg);

       new G4PVPlacement(RotZ30, G4ThreeVector(0.,0., 0.), Al_rod_log, "Al_rod", world, false, 0);
       new G4PVPlacement(RotZ30Y180, G4ThreeVector(0.,0., 0.), Al_rod_log, "Al_rod", world, false, 1);
	   */      

      //
      // Add the Aluminium chamber
      //
      G4double Al_chamber_rmin = 50. * cm;
      G4double Al_chamber_rmax = 50.5 * cm;
      G4double Al_chamber_z = 100.0 * cm;
      
      //G4Tubs* Al_chamber_tub
      //  = new G4Tubs("Al_chamber_tub", Al_chamber_rmin, Al_chamber_rmax, Al_chamber_z, 0.*deg, 180*deg);
      G4Tubs* Al_chamber_tub
      = new G4Tubs("Al_chamber_tub", Al_chamber_rmin, Al_chamber_rmax, Al_chamber_z, 0.*deg, 360*deg);
      
      G4LogicalVolume* Al_chamber_log = new G4LogicalVolume(Al_chamber_tub, Aluminium, "Al_chamber", 0, 0, 0);
      
      G4RotationMatrix* RotZ = new G4RotationMatrix();
      RotZ->rotateZ(-90*deg);
      
      new G4PVPlacement(RotZ, G4ThreeVector(0.,0.,0.), Al_chamber_log, "Al_chamber", world, false, 0);
      
      
      //G4VisAttributes* VisAtt1 = new G4VisAttributes(G4Colour(0.2, 0.5, 0.8));
      //Al_rod_log->SetVisAttributes(VisAtt1);
      //G4VisAttributes* VisAtt2 = new G4VisAttributes(G4Colour(0., 0.5, 0.3));
      //Al_chamber_log->SetVisAttributes(VisAtt2);

   //
   // Silicon mother volume (vacuum)
   //
   G4String Name = "HeliosDummyShape" + DetectorNumber ;

   G4Box*           solidHeliosDummyShape = new G4Box(Name, 0.5*FaceFrontWidth, 0.5*FaceFrontLength, 0.5*Thickness);
   G4LogicalVolume* logicHeliosDummyShape = new G4LogicalVolume(solidHeliosDummyShape, Vacuum, Name, 0, 0, 0);

   new G4PVPlacement(G4Transform3D(*MMrot, MMpos) ,
                                     logicHeliosDummyShape           ,
                                     Name                         ,
                                     world                        ,
                                     false                        ,
                                     0);

   logicHeliosDummyShape->SetVisAttributes(G4VisAttributes::Invisible);
   //logicHeliosDummyShape->SetVisAttributes(G4VisAttributes(G4Colour(1, 0., 0.0)));  // red
   //if (m_non_sensitive_part_visiualisation) logicHeliosDummyShape->SetVisAttributes(G4VisAttributes(G4Colour(0.90, 0.90, 0.90)));


   // Adding front Al layer for helisol
   /*
      G4Box*           solidAlLayer = new G4Box("AlLayer", 0.5*FaceFrontWidth, 0.5*FaceFrontLength, 0.5*AlThickness);
   G4LogicalVolume* logicAlLayer = new G4LogicalVolume(solidAlLayer, Aluminium, "AlLayer", 0, 0, 0);

   new G4PVPlacement(0,
		     G4ThreeVector(0.,0., +0.5*FirstStageThickness+0.5*AlThickness),
		     logicAlLayer,
		     "AlLayer",
		     logicHeliosDummyShape,
		     false,
		     0);

   logicAlLayer->SetVisAttributes(G4VisAttributes(G4Colour(0, 200, 200)));  // green
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
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void HeliosDetDummyShape::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("HeliosDummyShape");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  // Cartesian Case
  vector<string> cart = {"X1_Y1","X1_Y128","X128_Y1","X128_Y128"};
  // Spherical Case
  vector<string> sphe= {"R","THETA","PHI","BETA"};

  for(unsigned int i  = 0 ; i < blocks.size() ; i++){

    if(blocks[i]->HasToken("MField")){
      double Bz = blocks[i]->GetDouble("MField","T");
      MyMagneticField* myField = new MyMagneticField(G4ThreeVector(0.,0.,Bz));
	    
	   G4FieldManager* fieldMgr
		 = G4TransportationManager::GetTransportationManager()
		 ->GetFieldManager();
	       fieldMgr->SetDetectorField(myField);
	       fieldMgr->CreateChordFinder(myField);
    } 
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Helios  " << i+1 <<  endl;

      G4ThreeVector A = NPS::ConvertVector(blocks[i]->GetTVector3("X1_Y1","mm"));
      G4ThreeVector B = NPS::ConvertVector(blocks[i]->GetTVector3("X128_Y1","mm"));
      G4ThreeVector C = NPS::ConvertVector(blocks[i]->GetTVector3("X1_Y128","mm"));
      G4ThreeVector D = NPS::ConvertVector(blocks[i]->GetTVector3("X128_Y128","mm"));
      AddModule(A,B,C,D,true) ;
    }

    else if(blocks[i]->HasTokenList(sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Helios " << i+1 <<  endl;

      double Theta = blocks[i]->GetDouble("THETA","deg");
      double Phi= blocks[i]->GetDouble("PHI","deg");
      double R = blocks[i]->GetDouble("R","mm");
      vector<double> beta = blocks[i]->GetVectorDouble("BETA","deg");
      AddModule(Theta,Phi,R,beta[0],beta[1],beta[2],true);
    }

    else{
      cout << "ERROR: Missing token for M2Telescope blocks, check your input file" << endl;
      exit(1);
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


	 //cout << "MMposX= " << MMpos.getX() << " MMposY= " << MMpos.getY()  << " MMposZ= " << MMpos.getZ()  << endl; 
	 //cout << "MMposX= " << MMpos.getX() << " MMposY= " << MMpos.getY()  << " MMposZ= " << MMpos.getZ()  << endl; 



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



// Set the TinteractionCoordinates object from NPS::VDetector to the present class
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
   G4THitsMap<G4double>* XHitMap;  // X strip id
   G4THitsMap<G4double>* YHitMap;  // Y strip id
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
     // << sizeE << " sT:" << sizeT << " sX:" << sizeX << " sY:" << sizeY << G4endl ;
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
        Energy_itr = EnergyHitMap->GetMap()->begin();
         for (G4int ll = 0 ; ll < sizeE ; ll++) {
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
	         //ms_Event->SetHeliosFirstStageEStripTNbr(X);
	         //ms_Event->SetHeliosFirstStageTStripTNbr(X);
            }
            X_itr++;
         }

          // Strip Y
          /*
           Y_itr = YHitMap->GetMap()->begin();
           for (G4int h = 0 ; h < sizeY ; h++) {
             G4int YTrackID  =   Y_itr->first - N;
             G4double Y     = *(Y_itr->second);
             if (YTrackID == NTrackID) {
	         //ms_Event->SetHeliosFirstStageEStripNbr(int(X));
	         //ms_Event->SetHeliosFirstStageTStripNbr(int(X));
	          ms_Event->SetHeliosFirstStageEStripLNbr(Y);
	          ms_Event->SetHeliosFirstStageTStripLNbr(Y);
             }
             Y_itr++;
          }
          */

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



void HeliosDetDummyShape::InitializeScorers()
{
   bool already_exist = false; 
   // First stage Associate Scorer
   m_FirstStageScorer = NPS::VDetector::CheckScorer("FirstStageScorerHeliosDummyShape",already_exist);
   if(already_exist) return;


   G4VPrimitiveScorer* DetNbr                           = new OBSOLETEGENERALSCORERS::PSDetectorNumber("DetectorNumber", "HeliosDummyShape", 0);
   G4VPrimitiveScorer* TOF                              = new OBSOLETEGENERALSCORERS::PSTOF("StripTime","HeliosDummyShape", 0);
   G4VPrimitiveScorer* InteractionCoordinatesX          = new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesX("InterCoordX","HeliosDummyShape", 0);
   G4VPrimitiveScorer* InteractionCoordinatesY          = new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesY("InterCoordY","HeliosDummyShape", 0);
   G4VPrimitiveScorer* InteractionCoordinatesZ          = new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesZ("InterCoordZ","HeliosDummyShape", 0);
   G4VPrimitiveScorer* InteractionCoordinatesAngleTheta = new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesAngleTheta("InterCoordAngTheta","HeliosDummyShape", 0);
   G4VPrimitiveScorer* InteractionCoordinatesAnglePhi   = new OBSOLETEGENERALSCORERS::PSInteractionCoordinatesAnglePhi("InterCoordAngPhi","HeliosDummyShape", 0);
   G4VPrimitiveScorer* Energy                           = new HeliosScorerFirstStageEnergy("StripEnergy", "HeliosDummyShape", 0);
//   G4VPrimitiveScorer* StripPositionX                   = new HeliosScorerFirstStageFrontStripDummyShape("StripIDFront", 0, NumberOfTStrips);  // X <=> transversal (ie: _|_ to z)
//   G4VPrimitiveScorer* StripPositionY                   = new HeliosScorerFirstStageBackStripDummyShape("StripIDBack", 0, NumberOfLStrips);    // Y <=> longitudinal (ie: // to z)
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
