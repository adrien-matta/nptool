/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu *
 *                                                                           *
 * Creation Date  : June 2016                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  Microball simulation                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <sstream>
#include <cmath>
#include <limits>
#include <vector>
//G4 Geometry object
#include "G4Tubs.hh"
#include "G4Box.hh"
//below this line, the geometry objects have been added by John Bromell
#include "G4Trap.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Ellipsoid.hh"
#include "G4Trd.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"

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
#include "Microball.hh"
#include "CalorimeterScorers.hh"
#include "RootOutput.h"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "NPOptionManager.h"
// CLHEP header
#include "CLHEP/Random/RandGauss.h"

//Materials database, added by John Bromell
#include "G4NistManager.hh"

using namespace std;
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace Microball_NS{
  // Energy and time Resolution
  const double EnergyThreshold = 0.3*MeV; //0.1*MeV ;
  const double ResoTime = 8.49*ns ;
  const double ResoEnergy = 0*MeV; //0.102*MeV ;
  const double Radius = 50*mm ; 
  const double Width = 100*mm ;
  const double Thickness = 300*mm ;
  const string Scintillator = "BC400";
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Microball Specific Method
Microball::Microball(){
  m_Event = new TMicroballData() ;
  m_MicroballScorer = 0;
  m_SquareDetector = 0;
  m_CylindricalDetector = 0;


  // RGB Color + Transparency
  m_VisSquare = new G4VisAttributes(G4Colour(0, 1, 0, 0.5));   
  m_VisCylinder = new G4VisAttributes(G4Colour(0, 0, 1, 0.5));   

}

Microball::~Microball(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Microball::AddMicroball(bool  bR1, bool  bR2, bool  bR3, 
				bool  bR4, bool  bR5, bool  bR6, 
				bool  bR7, bool  bR8, bool  bR9,
				vector<int> copyNumArray, bool bFlip, bool bChamber){
  m_Ring1 = bR1;
  m_Ring2 = bR2;
  m_Ring3 = bR3;
  m_Ring4 = bR4;
  m_Ring5 = bR5;
  m_Ring6 = bR6;
  m_Ring7 = bR7;
  m_Ring8 = bR8;
  m_Ring9 = bR9;
  m_Array = copyNumArray;
  m_Flip = bFlip;
  m_Chamber = bChamber;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Microball::BuildSquareDetector(){
  if(!m_SquareDetector){
    G4Box* box = new G4Box("Microball_Box",Microball_NS::Width*0.5,
        Microball_NS::Width*0.5,Microball_NS::Thickness*0.5);

    G4Material* ScintMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(Microball_NS::Scintillator);
    m_SquareDetector = new G4LogicalVolume(box,ScintMaterial,"logic_Microball_Box",0,0,0);
    m_SquareDetector->SetVisAttributes(m_VisSquare);
    m_SquareDetector->SetSensitiveDetector(m_MicroballScorer);
  }
  return m_SquareDetector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Microball::BuildCylindricalDetector(){
  if(!m_CylindricalDetector){
    G4Tubs* tub = new G4Tubs("Microball_Cyl",0,Microball_NS::Radius,Microball_NS::Thickness*0.5,0,360*deg);

    G4Material* ScintMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary(Microball_NS::Scintillator);
    m_CylindricalDetector = new G4LogicalVolume(tub,ScintMaterial,"logic_Microball_tub",0,0,0);
    m_CylindricalDetector->SetVisAttributes(m_VisSquare);
    m_CylindricalDetector->SetSensitiveDetector(m_MicroballScorer);

  }
  return m_CylindricalDetector;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Microball::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Microball");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> token = {"RING1","RING2","RING3","RING4","RING5","RING6","RING7","RING8","RING9"};

  vector<int> copyNumArray;
  bool bFlip = false;
  bool bChamber = false;

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
       cout << endl << "////  Microball " << i+1 <<  endl;
      
      bool bR1 = blocks[i]->GetInt("RING1");
      bool bR2 = blocks[i]->GetInt("RING2");
      bool bR3 = blocks[i]->GetInt("RING3");
      bool bR4 = blocks[i]->GetInt("RING4");
      bool bR5 = blocks[i]->GetInt("RING5");
      bool bR6 = blocks[i]->GetInt("RING6");
      bool bR7 = blocks[i]->GetInt("RING7");
      bool bR8 = blocks[i]->GetInt("RING8");
      bool bR9 = blocks[i]->GetInt("RING9");
      if(blocks[i]->HasToken("DISABLE_CRYSTAL"))
        copyNumArray.push_back( blocks[i]->GetInt("DISABLE_CRYSTAL"));
      if(blocks[i]->HasToken("DETECTOR_FLIP"))
       bFlip =  blocks[i]->GetInt("DETECTOR_FLIP");
      if(blocks[i]->HasToken("INCLUDE_CHAMBER"))
       bChamber =  blocks[i]->GetInt("INCLUDE_CHAMBER=");

      AddMicroball(bR1, bR2, bR3,bR4, bR5, bR6, bR7, bR8, bR9, copyNumArray, bFlip, bChamber);
    }

    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Microball::ConstructDetector(G4LogicalVolume* world){
  //the below for-loop is commented out, and to be replaced with our detector geometry.

  G4bool checkOverlaps = false;
  G4double delrinBool = 1;
  G4double crystalBool = 1;
  G4double lgBool = 1;

 G4NistManager* nist = G4NistManager::Instance();

G4Material* Al = nist->FindOrBuildMaterial("G4_Al");
G4VisAttributes* chamberVisAtt = new G4VisAttributes(G4Colour(.7,.7,.7, .3));
G4Material* CsI = nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");
G4VisAttributes* crystalVisAtt = new G4VisAttributes(G4Colour(0.,1.,0., .5));
G4Material* glass = nist->FindOrBuildMaterial("G4_Pyrex_Glass");
G4VisAttributes* lightGuideVisAtt = new G4VisAttributes(G4Colour(0.,0.,1.));
G4Material* delrin = nist->FindOrBuildMaterial("G4_POLYOXYMETHYLENE");
G4VisAttributes* delrinVisAtt = new G4VisAttributes(G4Colour(1.,0.,0.));

if(m_Chamber){
 //----------------------------------------------------
 //the specifications for the dimensions of the chamber
 //----------------------------------------------------

 G4double chamberThickness = 7.9375*mm;
 G4double beamlineThickness = 10*mm;

 //the empty space in the chamber
 G4double subchamber_rmin = 0*mm;
 G4double subchamber_rmax = 673.1*mm;
 G4double subchamber_hz = (406.4*mm +251.46*mm)/2;  //673.1*mm/2;  
 G4double subchamber_phimin = 0*deg;
 G4double subchamber_phimax = 360*deg;
 G4RotationMatrix subchamber_rotm1 = G4RotationMatrix();
 G4ThreeVector subchamber_pos1 = G4ThreeVector(0,0,0);
 G4Transform3D subchamber_trans1 = G4Transform3D(subchamber_rotm1, subchamber_pos1);

 //the walls of the chamber
 G4double chamber_rmin = 0*mm;
 G4double chamber_rmax = subchamber_rmax +chamberThickness;
 G4double chamber_hz = subchamber_hz -0.1*mm; //fudge factor to make sure the top doesn't show up in the visualizer
 G4double chamber_phimin = 0*deg;
 G4double chamber_phimax = 360*deg;
 G4RotationMatrix chamber_rotm = G4RotationMatrix();
 chamber_rotm.rotateX(90*deg);
 G4ThreeVector chamber_pos = G4ThreeVector(0,-(chamber_hz -406.4*mm +76.2*mm),0);
 G4Transform3D chamber_trans = G4Transform3D(chamber_rotm, chamber_pos);

 //the top and bottom of the chamber
 G4double chambertop_height = 274.32*mm;
 G4double chambertop_semix = chamber_rmax;
 G4double chambertop_semiy = chamber_rmax;
 G4double chambertop_semiz = chambertop_height;
 G4double chambertop_bottomcut = 0;
 G4double chambertop_topcut = chambertop_semiz;
 G4RotationMatrix chambertop_rotm = G4RotationMatrix();
 chambertop_rotm.rotateX(-90*deg);
 G4ThreeVector chambertop_pos = G4ThreeVector(0,chamber_pos.getY() +chamber_hz,0);
 G4Transform3D chambertop_trans = G4Transform3D(chambertop_rotm, chambertop_pos);
 G4RotationMatrix chamberbottom_rotm = G4RotationMatrix();
 chamberbottom_rotm.rotateX(90*deg);
 G4ThreeVector chamberbottom_pos = G4ThreeVector(0,chamber_pos.getY() -chamber_hz,0);
 G4Transform3D chamberbottom_trans = G4Transform3D(chamberbottom_rotm, chamberbottom_pos);

 //the empty space in the top and bottom of the chamber
 G4double subtop_semix = chambertop_semix -chamberThickness;
 G4double subtop_semiy = chambertop_semiy -chamberThickness;
 G4double subtop_semiz = chambertop_semiz -chamberThickness;
 G4double subtop_bottomcut = 0 -0.1*mm; //fudge factor to make sure the bottom doesn't show up in the visualizer
 G4double subtop_topcut = subtop_semiz;
 G4RotationMatrix subtop_rotm = G4RotationMatrix();
 G4ThreeVector subtop_pos = G4ThreeVector(0,0,0);
 G4Transform3D subtop_trans = G4Transform3D(subtop_rotm, subtop_pos);

 //the hole for the beamline
 G4double subhole_rmin = 0*mm;
 G4double subhole_rmax = 152.4*mm/2;
 G4double subhole_hz = 10*(chamber_rmax -subchamber_rmax);
 G4double subhole_phimin = 0*deg;
 G4double subhole_phimax = 360*deg;
 G4RotationMatrix subhole_rotm = G4RotationMatrix();
 subhole_rotm.rotateX(90*deg);
 G4ThreeVector subhole_pos = G4ThreeVector(0,-(chamber_rmax +subchamber_rmax)/2,chamber_pos.getY());
 G4Transform3D subhole_trans = G4Transform3D(subhole_rotm, subhole_pos);

 //the beamline
 G4double beamline_rmin = subhole_rmax -beamlineThickness;
 G4double beamline_rmax = subhole_rmax;
 G4double beamline_hz = 500*mm; //the real value is 8760.46*mm; but this makes visualization difficult.
 G4double beamline_phimin = 0*deg;
 G4double beamline_phimax = 360*deg;
 G4RotationMatrix beamline_rotm = G4RotationMatrix();
 G4ThreeVector beamline_pos = G4ThreeVector(0,0,-(subchamber_rmax +beamline_hz -subhole_hz));
 G4Transform3D beamline_trans = G4Transform3D(beamline_rotm, beamline_pos);
 G4RotationMatrix subchamber_rotm2 = G4RotationMatrix();
 subchamber_rotm2.rotateX(90*deg);
 G4ThreeVector subchamber_pos2 = G4ThreeVector(0,0,-beamline_pos.getZ());
 G4Transform3D subchamber_trans2 = G4Transform3D(subchamber_rotm2, subchamber_pos2);

 //----------------------------------------------------
 //constructing the chamber
 //----------------------------------------------------
 //the walls of the chamber
 G4Tubs* solidSubchamber =    
    new G4Tubs("Subchamber",                       //its name
	       subchamber_rmin, subchamber_rmax, 
	       subchamber_hz,
	       subchamber_phimin, subchamber_phimax); //its size

 G4Tubs* solidSubhole =    
    new G4Tubs("Subhole",                       //its name
	       subhole_rmin, subhole_rmax, 
	       subhole_hz,
	       subhole_phimin, subhole_phimax); //its size

 G4Tubs* solidStartchamber =    
    new G4Tubs("Startchamber",                       //its name
	       chamber_rmin, chamber_rmax, 
	       chamber_hz,
	       chamber_phimin, chamber_phimax); //its size

   G4SubtractionSolid* solidMidchamber =
    new G4SubtractionSolid("Midchamber",
			   solidStartchamber,
			   solidSubchamber,
			   subchamber_trans1);

   G4SubtractionSolid* solidChamber =
    new G4SubtractionSolid("Chamber",
			   solidMidchamber,
			   solidSubhole,
			   subhole_trans);
      
  G4LogicalVolume* logicChamber =                         
    new G4LogicalVolume(solidChamber,           //its solid
                        Al,                 //its material
                        "Chamber");             //its name
  logicChamber->SetVisAttributes(chamberVisAtt);

new G4PVPlacement(chamber_trans,       //rotation, position
                    logicChamber,           //its logical volume
                    "Chamber",         //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
                    1,                     //copy number
                    checkOverlaps);          //overlaps checking

//the top and bottom of the chamber
 G4Ellipsoid* solidStartchambertop =    
    new G4Ellipsoid("Startchambertop",                       //its name
	       chambertop_semix, chambertop_semiy, chambertop_semiz, 
	       chambertop_bottomcut, chambertop_topcut); //its size

 G4Ellipsoid* solidSubtop =    
    new G4Ellipsoid("Subtop",                       //its name
	       subtop_semix, subtop_semiy, subtop_semiz, 
	       subtop_bottomcut, subtop_topcut); //its size

 G4SubtractionSolid* solidChambertop =
    new G4SubtractionSolid("Chambertop",
			   solidStartchambertop,
			   solidSubtop,
			   subtop_trans);

  G4LogicalVolume* logicChambertop =                         
    new G4LogicalVolume(solidChambertop,           //its solid
                        Al,                 //its material
                        "Chambertop");             //its name
  logicChambertop->SetVisAttributes(chamberVisAtt);

new G4PVPlacement(chambertop_trans,       //rotation, position
                    logicChambertop,           //its logical volume
                    "Chambertop",         //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
                    1,                     //copy number
                    checkOverlaps);          //overlaps checking
new G4PVPlacement(chamberbottom_trans,       //rotation, position
                    logicChambertop,           //its logical volume
                    "Chamberbottom",         //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
                    1,                     //copy number
                    checkOverlaps);          //overlaps checking

 //the beamline
 G4Tubs* solidStartbeamline =    
    new G4Tubs("Startbeamline",                       //its name
	       beamline_rmin, beamline_rmax, 
	       beamline_hz,
	       beamline_phimin, beamline_phimax); //its size

   G4SubtractionSolid* solidBeamline =
    new G4SubtractionSolid("Beamline",
			   solidStartbeamline,
			   solidSubchamber,
			   subchamber_trans2);

  G4LogicalVolume* logicBeamline =                         
    new G4LogicalVolume(solidBeamline,           //its solid
                        Al,                 //its material
                        "Beamline");             //its name
  logicBeamline->SetVisAttributes(chamberVisAtt);

new G4PVPlacement(beamline_trans,       //rotation, position
                    logicBeamline,           //its logical volume
                    "Beamline",         //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
                    1,                     //copy number
                    checkOverlaps);          //overlaps checking


}


if(m_Ring1){
  // Ring 1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ lg pseudofixed
  G4int detNum1 = 6;
  G4double distanceFromTargetBack1 = 110.*mm;
  G4double theta1 = 9.0*deg;
  G4double halfTheta1 = 5.0*deg;
  G4double lgThickness1 = 8.0*mm;
  G4double detThickness1 = 9.2*mm;
  G4double distanceFromTarget1 = distanceFromTargetBack1 - detThickness1;

  //G4double distance1 = distanceFromTarget1*sin(theta1); //this gives the distance from the beam axis.
  
  //
  // Delrin Ring
  //
  G4double delrinA1 = 5*mm;
  G4double delrinB1 = 10*mm;
  G4double fdelrinHalfAngle1 = atan(delrinB1/2/(distanceFromTarget1+detThickness1+lgThickness1));
  G4double firs1 = pow(pow(distanceFromTarget1+detThickness1+lgThickness1,2)+pow(delrinB1/2,2),0.5)*sin(theta1-fdelrinHalfAngle1);
  G4double firb1 = pow(pow(distanceFromTarget1+detThickness1+lgThickness1,2)+pow(delrinB1/2,2),0.5)*sin(theta1+fdelrinHalfAngle1);
  G4double bdelrinHalfAngle1 = atan(delrinB1/2/(distanceFromTarget1+detThickness1+lgThickness1+delrinA1));
  G4double birs1 = pow(pow(distanceFromTarget1+detThickness1+lgThickness1+delrinA1,2)+pow(delrinB1/2,2),0.5)*sin(theta1-bdelrinHalfAngle1);
  G4double birb1 = pow(pow(distanceFromTarget1+detThickness1+lgThickness1+delrinA1,2)+pow(delrinB1/2,2),0.5)*sin(theta1+bdelrinHalfAngle1);

  G4double delrin_dz1 = delrinB1/2;
  G4double delrin_theta1 = 0;
  G4double delrin_phi1 = 0;
  G4double delrin_dyab1 = delrinA1/2;
  G4double delrin_dxa1 = firs1*tan(180*deg/detNum1);
  G4double delrin_dxb1 = birs1*tan(180*deg/detNum1);
  G4double delrin_dycd1 = delrinA1/2;
  G4double delrin_dxc1 = firb1*tan(180*deg/detNum1);
  G4double delrin_dxd1 = birb1*tan(180*deg/detNum1);
  G4double delrin_alpab1 = 0;
  G4double delrin_alpcd1 = delrin_alpab1;

              
  G4ThreeVector delrin_pos1 = G4ThreeVector(0,0,distanceFromTarget1*cos(theta1));

  G4Trap* solidDelrin1 =    
    new G4Trap("Delrin1",                       //its name
	      delrin_dz1, 
	      delrin_theta1, delrin_phi1,
	      delrin_dyab1,
	      delrin_dxa1, delrin_dxb1,
	      delrin_alpab1,
	      delrin_dycd1,
	      delrin_dxc1, delrin_dxd1,
	      delrin_alpcd1);                   //its size
      
  G4LogicalVolume* logicDelrin1 =                         
    new G4LogicalVolume(solidDelrin1,           //its solid
                        delrin,                 //its material
                        "Delrin1");             //its name
  logicDelrin1->SetVisAttributes(delrinVisAtt);
               
  //
  // CsI Detector
  //
  G4double coneheight1 = 2*distanceFromTarget1;
  G4double flatheight1 = coneheight1-0.01*mm;
  G4double coneangle1 = theta1;
  G4double crystal_rmina1 = (coneheight1-flatheight1)*tan(coneangle1-halfTheta1);
  G4double crystal_rmaxa1 = (coneheight1-flatheight1)*tan(coneangle1+halfTheta1);
  G4double crystal_rminb1 = coneheight1*tan(coneangle1-halfTheta1);
  G4double crystal_rmaxb1 = coneheight1*tan(coneangle1+halfTheta1);
  G4double crystal_hz1 = 0.5*flatheight1;//(crystal_rmaxb1-crystal_rmaxa1)/crystal_rmaxb1*coneheight1;
  G4double crystal_phimin1 = 0.*deg, crystal_phimax1 = 360.*deg/detNum1;
  G4RotationMatrix tip_rotm1 = G4RotationMatrix();
  G4ThreeVector tip_pos1 = G4ThreeVector(0,0,0.5*flatheight1+(coneheight1-flatheight1));
  G4Transform3D tip_trans1 = G4Transform3D(tip_rotm1, tip_pos1);
  G4ThreeVector z1 = G4ThreeVector(0,0,1);
  G4ThreeVector d01 = G4ThreeVector(distanceFromTarget1*sin(coneangle1)*cos(0.5*360*deg/detNum1), 
				    distanceFromTarget1*sin(coneangle1)*sin(0.5*360*deg/detNum1), 
				    distanceFromTarget1*cos(coneangle1));
  G4ThreeVector unitAxis1 = z1.cross(d01);
  unitAxis1 = unitAxis1/unitAxis1.mag();
  G4double ux1 = unitAxis1.x();
  G4double uy1 = unitAxis1.y();
  G4double uz1 = unitAxis1.z();
  G4ThreeVector top1 = G4ThreeVector(cos(coneangle1)+pow(ux1,2)*(1-cos(coneangle1)), ux1*uy1*(1-cos(coneangle1))-uz1*sin(coneangle1), ux1*uz1*(1-cos(coneangle1))+uy1*sin(coneangle1));
  G4ThreeVector mid1 = G4ThreeVector(uy1*ux1*(1-cos(coneangle1))+uz1*sin(coneangle1), cos(coneangle1)+pow(uy1,2)*(1-cos(coneangle1)), uy1*uz1*(1-cos(coneangle1))-ux1*sin(coneangle1));
  G4ThreeVector bot1 = G4ThreeVector(uz1*ux1*(1-cos(coneangle1))-uy1*sin(coneangle1), uz1*uy1*(1-cos(coneangle1))+ux1*sin(coneangle1), cos(coneangle1)+pow(uz1,2)*(1-cos(coneangle1)));
  G4RotationMatrix firstBox_rotm1 = G4RotationMatrix();
  firstBox_rotm1.setRows(top1,mid1,bot1);
  G4ThreeVector firstBox_pos1 = G4ThreeVector(0,0,0);
  G4Transform3D firstBox_trans1 = G4Transform3D(firstBox_rotm1, firstBox_pos1);
  G4RotationMatrix secondBox_rotm1 = G4RotationMatrix();
  secondBox_rotm1.setRows(top1,mid1,bot1);
  G4ThreeVector secondBox_pos1 = (2*distanceFromTarget1+detThickness1)/distanceFromTarget1*d01;
  G4Transform3D secondBox_trans1 = G4Transform3D(secondBox_rotm1, secondBox_pos1);
  
  G4Orb* solidTip1 =
    new G4Orb("tip1",
	      .0001*mm);
  G4Box* solidSubtractionBox1 = 
    new G4Box("SubtractionBox1",                  //its name
	      distanceFromTarget1,
	      distanceFromTarget1,
	      distanceFromTarget1);               //its size
  G4Cons* solidCone1 =    
    new G4Cons("Cone1",                           //its name
	       crystal_rmina1, crystal_rmaxa1, 
	       crystal_rminb1, crystal_rmaxb1, 
	       crystal_hz1,
	       crystal_phimin1, crystal_phimax1); //its size
  G4UnionSolid* solidTippedCone1 =
    new G4UnionSolid("TippedCone1",
		     solidTip1,
		     solidCone1,
		     tip_trans1);
  G4SubtractionSolid* solidIntermediate1 =
    new G4SubtractionSolid("Intermediate1",
			   solidTippedCone1,
			   solidSubtractionBox1,
			   firstBox_trans1);
  G4SubtractionSolid* solidCrystal1 =
    new G4SubtractionSolid("Crystal1",
			   solidIntermediate1,
			   solidSubtractionBox1,
			   secondBox_trans1);
  
  G4LogicalVolume* logicCrystal1 =                         
    new G4LogicalVolume(solidCrystal1,            //its solid
                        CsI,                      //its material
                        "CsI Detector1");         //its name
  logicCrystal1->SetVisAttributes(crystalVisAtt);
  logicCrystal1->SetSensitiveDetector(m_MicroballScorer);
  //fScoringVolume = logicCrystal1;
  
  //
  // Light Guide
  //       
  G4double lightGuide_px1 = 10*mm;
  G4double lightGuide_py1 = 10*mm;
  G4double lightGuide_pz1  = lgThickness1;      
  G4Box* solidLightGuide1 =    
    new G4Box("LightGuide1",                 //its name
              0.5*lightGuide_px1, 
	      0.5*lightGuide_py1,
	      0.5*lightGuide_pz1);           //its size
                
  G4LogicalVolume* logicLightGuide1 =                         
    new G4LogicalVolume(solidLightGuide1,    //its solid
                        glass,               //its material
                        "LightGuide1");      //its name
  logicLightGuide1->SetVisAttributes(lightGuideVisAtt);               
  
  for(int i = 1; i <= detNum1; i++)
    {
      G4RotationMatrix crystal_rotm = G4RotationMatrix();
      G4ThreeVector d0 = G4ThreeVector(distanceFromTarget1*sin(coneangle1)*cos(0.5*360*deg/detNum1), 
				       distanceFromTarget1*sin(coneangle1)*sin(0.5*360*deg/detNum1), 
				       distanceFromTarget1*cos(coneangle1));
      G4ThreeVector df = G4ThreeVector(distanceFromTarget1*sin(theta1)*sin((i-1)*360*deg/detNum1+.001*deg/*+0.5*360*deg/detNum1*/), 
				       distanceFromTarget1*sin(theta1)*cos((i-1)*360*deg/detNum1+.001*deg/*+0.5*360*deg/detNum1*/), 
				       -distanceFromTarget1*cos(theta1));
      G4ThreeVector axis = d0.cross(df);
      G4double ax = axis.x();
      G4double ay = axis.y();
      G4double az = axis.z();
      G4double ux = ax/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double uy = ay/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double uz = az/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double j = acos(d0.dot(df)/(d0.mag()*df.mag()));
      G4ThreeVector a = G4ThreeVector(cos(j)+pow(ux,2)*(1-cos(j)), ux*uy*(1-cos(j))-uz*sin(j), ux*uz*(1-cos(j))+uy*sin(j));
      G4ThreeVector b = G4ThreeVector(uy*ux*(1-cos(j))+uz*sin(j), cos(j)+pow(uy,2)*(1-cos(j)), uy*uz*(1-cos(j))-ux*sin(j));
      G4ThreeVector c = G4ThreeVector(uz*ux*(1-cos(j))-uy*sin(j), uz*uy*(1-cos(j))+ux*sin(j), cos(j)+pow(uz,2)*(1-cos(j)));
      G4RotationMatrix abc = G4RotationMatrix();
      abc.setRows(a,b,c);
      G4ThreeVector unitdf = df/df.mag();
      G4double udfx = unitdf.x();
      G4double udfy = unitdf.y();
      G4double udfz = unitdf.z();
      G4double k = 180.*deg;
      G4ThreeVector d = G4ThreeVector(cos(k)+pow(udfx,2)*(1-cos(k)), udfx*udfy*(1-cos(k))-udfz*sin(k), udfx*udfz*(1-cos(k))+udfy*sin(k));
      G4ThreeVector e = G4ThreeVector(udfy*udfx*(1-cos(k))+udfz*sin(k), cos(k)+pow(udfy,2)*(1-cos(k)), udfy*udfz*(1-cos(k))-udfx*sin(k));
      G4ThreeVector f = G4ThreeVector(udfz*udfx*(1-cos(k))-udfy*sin(k), udfz*udfy*(1-cos(k))+udfx*sin(k), cos(k)+pow(udfz,2)*(1-cos(k)));
      G4RotationMatrix def = G4RotationMatrix();
      def.setRows(d,e,f);
      crystal_rotm=def*abc;
      if (m_Flip == 0){
	crystal_rotm.rotateY(180*deg);
      }
      crystal_rotm.rotateZ(180*deg);
      G4ThreeVector crystal_pos = G4ThreeVector(0,0,0);
      G4Transform3D crystal_trans = G4Transform3D(crystal_rotm, crystal_pos);
      G4RotationMatrix lg_rotm = G4RotationMatrix();
      G4ThreeVector lg_Axis = df.cross(z1);
      lg_Axis = lg_Axis/lg_Axis.mag();
      lg_Axis.rotateZ(180*deg);
      G4double ulgx = lg_Axis.x();
      G4double ulgy = lg_Axis.y();
      G4double ulgz = lg_Axis.z();
      G4ThreeVector p = G4ThreeVector(cos(theta1)+pow(ulgx,2)*(1-cos(theta1)), ulgx*ulgy*(1-cos(theta1))-ulgz*sin(theta1), ulgx*ulgz*(1-cos(theta1))+ulgy*sin(theta1));
      G4ThreeVector q = G4ThreeVector(ulgy*ulgx*(1-cos(theta1))+ulgz*sin(theta1), cos(theta1)+pow(ulgy,2)*(1-cos(theta1)), ulgy*ulgz*(1-cos(theta1))-ulgx*sin(theta1));
      G4ThreeVector r = G4ThreeVector(ulgz*ulgx*(1-cos(theta1))-ulgy*sin(theta1), ulgz*ulgy*(1-cos(theta1))+ulgx*sin(theta1), cos(theta1)+pow(ulgz,2)*(1-cos(theta1)));
      G4RotationMatrix pqr = G4RotationMatrix();
      pqr.setRows(p,q,r);
      lg_rotm.rotateZ((i-1)*360*deg/detNum1);
      lg_rotm.rotateX(180*deg);
      lg_rotm=pqr*lg_rotm;
      G4ThreeVector lg_pos = (distanceFromTarget1+detThickness1+0.5*lgThickness1)/df.mag()*df;
      lg_pos.rotateZ(180*deg);
      if (m_Flip == 0){
	lg_pos.rotateY(180*deg);
	lg_rotm.rotateY(180*deg);
      }
      G4Transform3D lg_trans = G4Transform3D(lg_rotm, lg_pos);
      G4RotationMatrix delrin_rotm = G4RotationMatrix();
      G4ThreeVector w = G4ThreeVector(sin(theta1)*sin((i-1)*360*deg/detNum1+.001*deg), sin(theta1)*cos((i-1)*360*deg/detNum1+.001*deg), cos(theta1));
      G4ThreeVector delrin_Axis = w.cross(z1);
      delrin_Axis = delrin_Axis/delrin_Axis.mag();
      delrin_Axis.rotateX(180*deg);
      G4double udx = delrin_Axis.x();
      G4double udy = delrin_Axis.y();
      G4double udz = delrin_Axis.z();
      G4double delAng = 90*deg+theta1;
      G4ThreeVector pdel = G4ThreeVector(cos(delAng)+pow(udx,2)*(1-cos(delAng)), udx*udy*(1-cos(delAng))-udz*sin(delAng), udx*udz*(1-cos(delAng))+udy*sin(delAng));
      G4ThreeVector qdel = G4ThreeVector(udy*udx*(1-cos(delAng))+udz*sin(delAng), cos(delAng)+pow(udy,2)*(1-cos(delAng)), udy*udz*(1-cos(delAng))-udx*sin(delAng));
      G4ThreeVector rdel = G4ThreeVector(udz*udx*(1-cos(delAng))-udy*sin(delAng), udz*udy*(1-cos(delAng))+udx*sin(delAng), cos(delAng)+pow(udz,2)*(1-cos(delAng)));
      G4RotationMatrix pqrdel = G4RotationMatrix();
      pqrdel.setRows(pdel,qdel,rdel);
      delrin_rotm.rotateZ((i-1)*360*deg/detNum1+.001*deg);
      delrin_rotm=pqrdel*delrin_rotm;
      G4ThreeVector delrin_pos = (distanceFromTarget1+detThickness1+lgThickness1+delrinA1/2+.0003*mm)/df.mag()*df;  //the .0003 mm is a fudge factor to prevent overlapping
      delrin_pos.rotateZ(180*deg);
      delrin_pos.rotateY(180*deg);
      if (m_Flip == 1){
	delrin_pos.rotateY(180*deg);
	delrin_rotm.rotateY(180*deg);
      }
      G4Transform3D delrin_trans = G4Transform3D(delrin_rotm, delrin_pos);
      bool placeCrystal = true;
      for (unsigned int i2=0; i2 < m_Array.size(); i2++){
	if (0 < m_Array[i2] && m_Array[i2] < 7){
	  /*if (1+6-m_Array[i2] == 0+i){
	    placeCrystal = false;
	  }*/
	  /*if (m_Array[i2] == 0+i){
	    placeCrystal = false;
	  }*/
	  if (m_Array[i2] == 0 +(i -detNum1/2)%detNum1){
	    placeCrystal = false;
	  }else if (m_Array[i2] == 0 +(i -detNum1/2)%detNum1 +detNum1){
	    placeCrystal = false;
	  }
	}
      }
      if (placeCrystal){
      if(crystalBool) {new G4PVPlacement(crystal_trans,       //rotation, position
                    logicCrystal1,           //its logical volume
                    "CsI Detector1",         //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
                    0+i,                     //copy number
                    checkOverlaps);          //overlaps checking
      }
      if(lgBool) {new G4PVPlacement(lg_trans,            //rotation, position
                    logicLightGuide1,        //its logical volume
                    "LightGuide1",           //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
		    0+i,                     //copy number
                    checkOverlaps);          //overlaps checking
      }
      if(delrinBool) {new G4PVPlacement(delrin_trans,          //rotation, position
                    logicDelrin1,               //its logical volume
                    "Delrin1",                  //its name
                    world,                 //its mother  volume
                    false,                      //no boolean operation
                    0+i,                        //copy number
                    checkOverlaps);             //overlaps checking
      }
      }
    }}
  if(m_Ring2){
  // Ring 2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ lg pseudofixed
  G4int detNum2 = 10;
  G4double distanceFromTargetBack2 = 80.*mm;
  G4double theta2 = 21.0*deg;
  G4double halfTheta2 = 7.0*deg;  //+0.1*deg; //0.1 is a fudge factor to make it cover solid angle 4pi
  G4double lgThickness2 = 7.5*mm;
  G4double detThickness2 = 7.2*mm;
  G4double distanceFromTarget2 = distanceFromTargetBack2 - detThickness2;

  //G4double distance2 = distanceFromTarget2*sin(theta2); //this gives the distance from the beam axis.

  //
  // Delrin Ring
  //
  G4double delrinA2 = 5*mm;
  G4double delrinB2 = 10*mm;
  G4double fdelrinHalfAngle2 = atan(delrinB2/2/(distanceFromTarget2+detThickness2+lgThickness2));
  G4double firs2 = pow(pow(distanceFromTarget2+detThickness2+lgThickness2,2)+pow(delrinB2/2,2),0.5)*sin(theta2-fdelrinHalfAngle2);
  G4double firb2 = pow(pow(distanceFromTarget2+detThickness2+lgThickness2,2)+pow(delrinB2/2,2),0.5)*sin(theta2+fdelrinHalfAngle2);
  G4double bdelrinHalfAngle2 = atan(delrinB2/2/(distanceFromTarget2+detThickness2+lgThickness2+delrinA2));
  G4double birs2 = pow(pow(distanceFromTarget2+detThickness2+lgThickness2+delrinA2,2)+pow(delrinB2/2,2),0.5)*sin(theta2-bdelrinHalfAngle2);
  G4double birb2 = pow(pow(distanceFromTarget2+detThickness2+lgThickness2+delrinA2,2)+pow(delrinB2/2,2),0.5)*sin(theta2+bdelrinHalfAngle2);

  G4double delrin_dz2 = delrinB2/2;
  G4double delrin_theta2 = 0;
  G4double delrin_phi2 = 0;
  G4double delrin_dyab2 = delrinA2/2;
  G4double delrin_dxa2 = firs2*tan(180*deg/detNum2);
  G4double delrin_dxb2 = birs2*tan(180*deg/detNum2);
  G4double delrin_dycd2 = delrinA2/2;
  G4double delrin_dxc2 = firb2*tan(180*deg/detNum2);
  G4double delrin_dxd2 = birb2*tan(180*deg/detNum2);
  G4double delrin_alpab2 = 0;
  G4double delrin_alpcd2 = delrin_alpab2;

            
  G4ThreeVector delrin_pos2 = G4ThreeVector(0,0,distanceFromTarget2*cos(theta2));

  G4Trap* solidDelrin2 =    
    new G4Trap("Delrin2",                       //its name
	      delrin_dz2, 
	      delrin_theta2, delrin_phi2,
	      delrin_dyab2,
	      delrin_dxa2, delrin_dxb2,
	      delrin_alpab2,
	      delrin_dycd2,
	      delrin_dxc2, delrin_dxd2,
	      delrin_alpcd2);                   //its size
      
  G4LogicalVolume* logicDelrin2 =                         
    new G4LogicalVolume(solidDelrin2,           //its solid
                        delrin,                 //its material
                        "Delrin2");             //its name
  logicDelrin2->SetVisAttributes(delrinVisAtt);
  
  
  //
  // CsI Detector
  //
  G4double coneheight2 = 2*distanceFromTarget2;
  G4double flatheight2 = coneheight2-0.01*mm;
  G4double coneangle2 = theta2;
  G4double crystal_rmina2 = (coneheight2-flatheight2)*tan(coneangle2-halfTheta2);
  G4double crystal_rmaxa2 = (coneheight2-flatheight2)*tan(coneangle2+halfTheta2);
  G4double crystal_rminb2 = coneheight2*tan(coneangle2-halfTheta2);
  G4double crystal_rmaxb2 = coneheight2*tan(coneangle2+halfTheta2);
  G4double crystal_hz2 = 0.5*flatheight2;//(crystal_rmaxb2-crystal_rmaxa2)/crystal_rmaxb2*coneheight2;
  G4double crystal_phimin2 = 0.*deg, crystal_phimax2 = 360.*deg/detNum2;
  G4RotationMatrix tip_rotm2 = G4RotationMatrix();
  G4ThreeVector tip_pos2 = G4ThreeVector(0,0,0.5*flatheight2+(coneheight2-flatheight2));
  G4Transform3D tip_trans2 = G4Transform3D(tip_rotm2, tip_pos2);
  G4ThreeVector z2 = G4ThreeVector(0,0,1);
  G4ThreeVector d02 = G4ThreeVector(distanceFromTarget2*sin(coneangle2)*cos(0.5*360*deg/detNum2), 
				    distanceFromTarget2*sin(coneangle2)*sin(0.5*360*deg/detNum2), 
				    distanceFromTarget2*cos(coneangle2));
  G4ThreeVector unitAxis2 = z2.cross(d02);
  unitAxis2 = unitAxis2/unitAxis2.mag();
  G4double ux2 = unitAxis2.x();
  G4double uy2 = unitAxis2.y();
  G4double uz2 = unitAxis2.z();
  G4ThreeVector top2 = G4ThreeVector(cos(coneangle2)+pow(ux2,2)*(1-cos(coneangle2)), ux2*uy2*(1-cos(coneangle2))-uz2*sin(coneangle2), ux2*uz2*(1-cos(coneangle2))+uy2*sin(coneangle2));
  G4ThreeVector mid2 = G4ThreeVector(uy2*ux2*(1-cos(coneangle2))+uz2*sin(coneangle2), cos(coneangle2)+pow(uy2,2)*(1-cos(coneangle2)), uy2*uz2*(1-cos(coneangle2))-ux2*sin(coneangle2));
  G4ThreeVector bot2 = G4ThreeVector(uz2*ux2*(1-cos(coneangle2))-uy2*sin(coneangle2), uz2*uy2*(1-cos(coneangle2))+ux2*sin(coneangle2), cos(coneangle2)+pow(uz2,2)*(1-cos(coneangle2)));
  G4RotationMatrix firstBox_rotm2 = G4RotationMatrix();
  firstBox_rotm2.setRows(top2,mid2,bot2);
  G4ThreeVector firstBox_pos2 = G4ThreeVector(0,0,0);
  G4Transform3D firstBox_trans2 = G4Transform3D(firstBox_rotm2, firstBox_pos2);
  G4RotationMatrix secondBox_rotm2 = G4RotationMatrix();
  secondBox_rotm2.setRows(top2,mid2,bot2);
  G4ThreeVector secondBox_pos2 = (2*distanceFromTarget2+detThickness2)/distanceFromTarget2*d02;
  G4Transform3D secondBox_trans2 = G4Transform3D(secondBox_rotm2, secondBox_pos2);
  
  G4Orb* solidTip2 =
    new G4Orb("tip2",
	      .0001*mm);
  G4Box* solidSubtractionBox2 = 
    new G4Box("SubtractionBox2",                  //its name
	      distanceFromTarget2,
	      distanceFromTarget2,
	      distanceFromTarget2);               //its size
  G4Cons* solidCone2 =    
    new G4Cons("Cone2",                           //its name
	       crystal_rmina2, crystal_rmaxa2, 
	       crystal_rminb2, crystal_rmaxb2, 
	       crystal_hz2,
	       crystal_phimin2, crystal_phimax2); //its size
  G4UnionSolid* solidTippedCone2 =
    new G4UnionSolid("TippedCone2",
		     solidTip2,
		     solidCone2,
		     tip_trans2);
  G4SubtractionSolid* solidIntermediate2 =
    new G4SubtractionSolid("Intermediate2",
			   solidTippedCone2,
			   solidSubtractionBox2,
			   firstBox_trans2);
  G4SubtractionSolid* solidCrystal2 =
    new G4SubtractionSolid("Crystal2",
			   solidIntermediate2,
			   solidSubtractionBox2,
			   secondBox_trans2);
  
  G4LogicalVolume* logicCrystal2 =                         
    new G4LogicalVolume(solidCrystal2,            //its solid
                        CsI,                      //its material
                        "CsI Detector2");         //its name
  logicCrystal2->SetVisAttributes(crystalVisAtt);
  logicCrystal2->SetSensitiveDetector(m_MicroballScorer);		     
  //
  // Light Guide
  //       
  G4double lightGuide_px2 = 10*mm;
  G4double lightGuide_py2 = 10*mm;
  G4double lightGuide_pz2  = lgThickness2;      
  G4Box* solidLightGuide2 =    
    new G4Box("LightGuide2",                 //its name
              0.5*lightGuide_px2, 
	      0.5*lightGuide_py2,
	      0.5*lightGuide_pz2);           //its size
                
  G4LogicalVolume* logicLightGuide2 =                         
    new G4LogicalVolume(solidLightGuide2,    //its solid
                        glass,               //its material
                        "LightGuide2");      //its name
  logicLightGuide2->SetVisAttributes(lightGuideVisAtt);               

  for(int i = 1; i <= detNum2; i++)
    {
      G4RotationMatrix crystal_rotm = G4RotationMatrix();
      G4ThreeVector d0 = G4ThreeVector(distanceFromTarget2*sin(coneangle2)*cos(0.5*360*deg/detNum2), 
				       distanceFromTarget2*sin(coneangle2)*sin(0.5*360*deg/detNum2), 
				       distanceFromTarget2*cos(coneangle2));
      G4ThreeVector df = G4ThreeVector(distanceFromTarget2*sin(theta2)*sin((i-1)*360*deg/detNum2+.001*deg/*+0.5*360*deg/detNum2*/), 
				       distanceFromTarget2*sin(theta2)*cos((i-1)*360*deg/detNum2+.001*deg/*+0.5*360*deg/detNum2*/), 
				       -distanceFromTarget2*cos(theta2));
      G4ThreeVector axis = d0.cross(df);
      G4double ax = axis.x();
      G4double ay = axis.y();
      G4double az = axis.z();
      G4double ux = ax/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double uy = ay/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double uz = az/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double j = acos(d0.dot(df)/(d0.mag()*df.mag()));
      G4ThreeVector a = G4ThreeVector(cos(j)+pow(ux,2)*(1-cos(j)), ux*uy*(1-cos(j))-uz*sin(j), ux*uz*(1-cos(j))+uy*sin(j));
      G4ThreeVector b = G4ThreeVector(uy*ux*(1-cos(j))+uz*sin(j), cos(j)+pow(uy,2)*(1-cos(j)), uy*uz*(1-cos(j))-ux*sin(j));
      G4ThreeVector c = G4ThreeVector(uz*ux*(1-cos(j))-uy*sin(j), uz*uy*(1-cos(j))+ux*sin(j), cos(j)+pow(uz,2)*(1-cos(j)));
      G4RotationMatrix abc = G4RotationMatrix();
      abc.setRows(a,b,c);
      G4ThreeVector unitdf = df/df.mag();
      G4double udfx = unitdf.x();
      G4double udfy = unitdf.y();
      G4double udfz = unitdf.z();
      G4double k = 180.*deg;
      G4ThreeVector d = G4ThreeVector(cos(k)+pow(udfx,2)*(1-cos(k)), udfx*udfy*(1-cos(k))-udfz*sin(k), udfx*udfz*(1-cos(k))+udfy*sin(k));
      G4ThreeVector e = G4ThreeVector(udfy*udfx*(1-cos(k))+udfz*sin(k), cos(k)+pow(udfy,2)*(1-cos(k)), udfy*udfz*(1-cos(k))-udfx*sin(k));
      G4ThreeVector f = G4ThreeVector(udfz*udfx*(1-cos(k))-udfy*sin(k), udfz*udfy*(1-cos(k))+udfx*sin(k), cos(k)+pow(udfz,2)*(1-cos(k)));
      G4RotationMatrix def = G4RotationMatrix();
      def.setRows(d,e,f);
      crystal_rotm=def*abc;
      if (m_Flip == 0){
	crystal_rotm.rotateY(180*deg);
      }
      crystal_rotm.rotateZ(180*deg);
      G4ThreeVector crystal_pos = G4ThreeVector(0,0,0);
      G4Transform3D crystal_trans = G4Transform3D(crystal_rotm, crystal_pos);
      G4RotationMatrix lg_rotm = G4RotationMatrix();
      G4ThreeVector lg_Axis = df.cross(z2);
      lg_Axis = lg_Axis/lg_Axis.mag();
      lg_Axis.rotateZ(180*deg);
      G4double ulgx = lg_Axis.x();
      G4double ulgy = lg_Axis.y();
      G4double ulgz = lg_Axis.z();
      G4ThreeVector p = G4ThreeVector(cos(theta2)+pow(ulgx,2)*(1-cos(theta2)), ulgx*ulgy*(1-cos(theta2))-ulgz*sin(theta2), ulgx*ulgz*(1-cos(theta2))+ulgy*sin(theta2));
      G4ThreeVector q = G4ThreeVector(ulgy*ulgx*(1-cos(theta2))+ulgz*sin(theta2), cos(theta2)+pow(ulgy,2)*(1-cos(theta2)), ulgy*ulgz*(1-cos(theta2))-ulgx*sin(theta2));
      G4ThreeVector r = G4ThreeVector(ulgz*ulgx*(1-cos(theta2))-ulgy*sin(theta2), ulgz*ulgy*(1-cos(theta2))+ulgx*sin(theta2), cos(theta2)+pow(ulgz,2)*(1-cos(theta2)));
      G4RotationMatrix pqr = G4RotationMatrix();
      pqr.setRows(p,q,r);
      lg_rotm.rotateZ((i-1)*360*deg/detNum2);
      lg_rotm.rotateX(180*deg);
      lg_rotm=pqr*lg_rotm;
      G4ThreeVector lg_pos = (distanceFromTarget2+detThickness2+0.5*lgThickness2)/df.mag()*df;
      lg_pos.rotateZ(180*deg);
      if (m_Flip == 0){
	lg_pos.rotateY(180*deg);
	lg_rotm.rotateY(180*deg);
      }
      G4Transform3D lg_trans = G4Transform3D(lg_rotm, lg_pos);
      G4Transform3D d0_trans = G4Transform3D(G4RotationMatrix(), d0);
      G4Transform3D df_trans = G4Transform3D(G4RotationMatrix(), df);
      G4RotationMatrix delrin_rotm = G4RotationMatrix();
      G4ThreeVector w = G4ThreeVector(sin(theta2)*sin((i-1)*360*deg/detNum2+.001*deg), sin(theta2)*cos((i-1)*360*deg/detNum2+.001*deg), cos(theta2));
      G4ThreeVector delrin_Axis = w.cross(z2);
      delrin_Axis.rotateX(180*deg);
      delrin_Axis = delrin_Axis/delrin_Axis.mag();
      G4double udx = delrin_Axis.x();
      G4double udy = delrin_Axis.y();
      G4double udz = delrin_Axis.z();
      G4double delAng = 90*deg+theta2;
      G4ThreeVector pdel = G4ThreeVector(cos(delAng)+pow(udx,2)*(1-cos(delAng)), udx*udy*(1-cos(delAng))-udz*sin(delAng), udx*udz*(1-cos(delAng))+udy*sin(delAng));
      G4ThreeVector qdel = G4ThreeVector(udy*udx*(1-cos(delAng))+udz*sin(delAng), cos(delAng)+pow(udy,2)*(1-cos(delAng)), udy*udz*(1-cos(delAng))-udx*sin(delAng));
      G4ThreeVector rdel = G4ThreeVector(udz*udx*(1-cos(delAng))-udy*sin(delAng), udz*udy*(1-cos(delAng))+udx*sin(delAng), cos(delAng)+pow(udz,2)*(1-cos(delAng)));
      G4RotationMatrix pqrdel = G4RotationMatrix();
      pqrdel.setRows(pdel,qdel,rdel);
      delrin_rotm.rotateZ((i-1)*360*deg/detNum2+.001*deg);
      delrin_rotm=pqrdel*delrin_rotm;
      delrin_rotm.rotateZ(0*deg);
      G4ThreeVector delrin_pos = (distanceFromTarget2+detThickness2+lgThickness2+delrinA2/2+.0003*mm)/df.mag()*df;  //the .0003 mm is a fudge factor to prevent overlapping
      delrin_pos.rotateZ(180*deg);
      delrin_pos.rotateY(180*deg);
      if (m_Flip == 1){
	delrin_pos.rotateY(180*deg);
	delrin_rotm.rotateY(180*deg);
      }
      G4Transform3D delrin_trans = G4Transform3D(delrin_rotm, delrin_pos);  
      bool placeCrystal = true;
      for (unsigned int i2=0; i2 < m_Array.size(); i2++){
	if (6 < m_Array[i2] && m_Array[i2] < 17){
	  if (m_Array[i2] == 6 +(i -detNum2/2)%detNum2){
	    placeCrystal = false;
	  }else if (m_Array[i2] == 6 +(i -detNum2/2)%detNum2 +detNum2){
	    placeCrystal = false;
	  }
	}
      }
      if (placeCrystal){    
      if(crystalBool) {new G4PVPlacement(crystal_trans,       //rotation, position
                    logicCrystal2,           //its logical volume
                    "CsI Detector2",         //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
                    6+i,                     //copy number
                    checkOverlaps);          //overlaps checking
      }
      if(lgBool) {new G4PVPlacement(lg_trans,            //rotation, position
                    logicLightGuide2,        //its logical volume
                    "LightGuide2",           //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
		    6+i,                     //copy number
                    checkOverlaps);          //overlaps checking
      }
      if(delrinBool) {new G4PVPlacement(delrin_trans,          //rotation, position
                    logicDelrin2,               //its logical volume
                    "Delrin2",                  //its name
                    world,                 //its mother  volume
                    false,                      //no boolean operation
                    6+i,                        //copy number
                    checkOverlaps);             //overlaps checking
      }
      }
    }}
 
  if(m_Ring3){
  // Ring 3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ lg pseudofixed
  G4int detNum3 = 12;
  G4double distanceFromTargetBack3 = 60.*mm;
  G4double theta3 = 36.0*deg;
  G4double halfTheta3 = 8.0*deg;
  G4double lgThickness3 = 6.0*mm;
  G4double detThickness3 = 6.4*mm;
  G4double distanceFromTarget3 = distanceFromTargetBack3 - detThickness3;

  //G4double distance3 = distanceFromTarget3*sin(theta3); //this gives the distance from the beam axis.

  //
  // Delrin Ring
  //
  G4double delrinA3 = 5*mm;
  G4double delrinB3 = 10*mm;
  G4double fdelrinHalfAngle3 = atan(delrinB3/2/(distanceFromTarget3+detThickness3+lgThickness3));
  G4double firs3 = pow(pow(distanceFromTarget3+detThickness3+lgThickness3,2)+pow(delrinB3/2,2),0.5)*sin(theta3-fdelrinHalfAngle3);
  G4double firb3 = pow(pow(distanceFromTarget3+detThickness3+lgThickness3,2)+pow(delrinB3/2,2),0.5)*sin(theta3+fdelrinHalfAngle3);
  G4double bdelrinHalfAngle3 = atan(delrinB3/2/(distanceFromTarget3+detThickness3+lgThickness3+delrinA3));
  G4double birs3 = pow(pow(distanceFromTarget3+detThickness3+lgThickness3+delrinA3,2)+pow(delrinB3/2,2),0.5)*sin(theta3-bdelrinHalfAngle3);
  G4double birb3 = pow(pow(distanceFromTarget3+detThickness3+lgThickness3+delrinA3,2)+pow(delrinB3/2,2),0.5)*sin(theta3+bdelrinHalfAngle3);

  G4double delrin_dz3 = delrinB3/2;
  G4double delrin_theta3 = 0;
  G4double delrin_phi3 = 0;
  G4double delrin_dyab3 = delrinA3/2;
  G4double delrin_dxa3 = firs3*tan(180*deg/detNum3);
  G4double delrin_dxb3 = birs3*tan(180*deg/detNum3);
  G4double delrin_dycd3 = delrinA3/2;
  G4double delrin_dxc3 = firb3*tan(180*deg/detNum3);
  G4double delrin_dxd3 = birb3*tan(180*deg/detNum3);
  G4double delrin_alpab3 = 0;
  G4double delrin_alpcd3 = delrin_alpab3;

            
  G4ThreeVector delrin_pos3 = G4ThreeVector(0,0,distanceFromTarget3*cos(theta3));

  G4Trap* solidDelrin3 =    
    new G4Trap("Delrin3",                       //its name
	      delrin_dz3, 
	      delrin_theta3, delrin_phi3,
	      delrin_dyab3,
	      delrin_dxa3, delrin_dxb3,
	      delrin_alpab3,
	      delrin_dycd3,
	      delrin_dxc3, delrin_dxd3,
	      delrin_alpcd3);                   //its size
      
  G4LogicalVolume* logicDelrin3 =                         
    new G4LogicalVolume(solidDelrin3,           //its solid
                        delrin,                 //its material
                        "Delrin3");             //its name
  logicDelrin3->SetVisAttributes(delrinVisAtt);
  
  //
  // CsI Detector
  //
  G4double coneheight3 = distanceFromTarget3;
  G4double flatheight3 = coneheight3-0.01*mm;
  G4double coneangle3 = theta3;
  G4double crystal_rmina3 = (coneheight3-flatheight3)*tan(coneangle3-halfTheta3);
  G4double crystal_rmaxa3 = (coneheight3-flatheight3)*tan(coneangle3+halfTheta3);
  G4double crystal_rminb3 = coneheight3*tan(coneangle3-halfTheta3);
  G4double crystal_rmaxb3 = coneheight3*tan(coneangle3+halfTheta3);
  G4double crystal_hz3 = 0.5*flatheight3;//(crystal_rmaxb3-crystal_rmaxa3)/crystal_rmaxb3*coneheight3;
  G4double crystal_phimin3 = 0.*deg, crystal_phimax3 = 360.*deg/detNum3;
  G4RotationMatrix tip_rotm3 = G4RotationMatrix();
  G4ThreeVector tip_pos3 = G4ThreeVector(0,0,0.5*flatheight3+(coneheight3-flatheight3));
  G4Transform3D tip_trans3 = G4Transform3D(tip_rotm3, tip_pos3);
  G4ThreeVector z3 = G4ThreeVector(0,0,1);
  G4ThreeVector d03 = G4ThreeVector(distanceFromTarget3*sin(coneangle3)*cos(0.5*360*deg/detNum3), 
				    distanceFromTarget3*sin(coneangle3)*sin(0.5*360*deg/detNum3), 
				    distanceFromTarget3*cos(coneangle3));
  G4ThreeVector unitAxis3 = z3.cross(d03);
  unitAxis3 = unitAxis3/unitAxis3.mag();
  G4double ux3 = unitAxis3.x();
  G4double uy3 = unitAxis3.y();
  G4double uz3 = unitAxis3.z();
  G4ThreeVector top3 = G4ThreeVector(cos(coneangle3)+pow(ux3,2)*(1-cos(coneangle3)), ux3*uy3*(1-cos(coneangle3))-uz3*sin(coneangle3), ux3*uz3*(1-cos(coneangle3))+uy3*sin(coneangle3));
  G4ThreeVector mid3 = G4ThreeVector(uy3*ux3*(1-cos(coneangle3))+uz3*sin(coneangle3), cos(coneangle3)+pow(uy3,2)*(1-cos(coneangle3)), uy3*uz3*(1-cos(coneangle3))-ux3*sin(coneangle3));
  G4ThreeVector bot3 = G4ThreeVector(uz3*ux3*(1-cos(coneangle3))-uy3*sin(coneangle3), uz3*uy3*(1-cos(coneangle3))+ux3*sin(coneangle3), cos(coneangle3)+pow(uz3,2)*(1-cos(coneangle3)));
  G4RotationMatrix firstBox_rotm3 = G4RotationMatrix();
  firstBox_rotm3.setRows(top3,mid3,bot3);
  G4ThreeVector firstBox_pos3 = G4ThreeVector(0,0,0);
  G4Transform3D firstBox_trans3 = G4Transform3D(firstBox_rotm3, firstBox_pos3);
  G4RotationMatrix secondBox_rotm3 = G4RotationMatrix();
  secondBox_rotm3.setRows(top3,mid3,bot3);
  G4ThreeVector secondBox_pos3 = (2*distanceFromTarget3+detThickness3)/distanceFromTarget3*d03;
  G4Transform3D secondBox_trans3 = G4Transform3D(secondBox_rotm3, secondBox_pos3);
  
  G4Orb* solidTip3 =
    new G4Orb("tip3",
	      .0001*mm);
  G4Box* solidSubtractionBox3 = 
    new G4Box("SubtractionBox3",                  //its name
	      distanceFromTarget3,
	      distanceFromTarget3,
	      distanceFromTarget3);               //its size
  G4Cons* solidCone3 =    
    new G4Cons("Cone3",                           //its name
	       crystal_rmina3, crystal_rmaxa3, 
	       crystal_rminb3, crystal_rmaxb3, 
	       crystal_hz3,
	       crystal_phimin3, crystal_phimax3); //its size
  G4UnionSolid* solidTippedCone3 =
    new G4UnionSolid("TippedCone3",
		     solidTip3,
		     solidCone3,
		     tip_trans3);
  G4SubtractionSolid* solidIntermediate3 =
    new G4SubtractionSolid("Intermediate3",
			   solidTippedCone3,
			   solidSubtractionBox3,
			   firstBox_trans3);
  G4SubtractionSolid* solidCrystal3 =
    new G4SubtractionSolid("Crystal3",
			   solidIntermediate3,
			   solidSubtractionBox3,
			   secondBox_trans3);
  
  G4LogicalVolume* logicCrystal3 =                         
    new G4LogicalVolume(solidCrystal3,            //its solid
                        CsI,                      //its material
                        "CsI Detector3");         //its name
  logicCrystal3->SetVisAttributes(crystalVisAtt);
  logicCrystal3->SetSensitiveDetector(m_MicroballScorer);		                  
  
  
  //
  // Light Guide
  //       
  G4double lightGuide_px3 = 10*mm;
  G4double lightGuide_py3 = 10*mm;
  G4double lightGuide_pz3  = lgThickness3;      
  G4Box* solidLightGuide3 =    
    new G4Box("LightGuide3",                 //its name
              0.5*lightGuide_px3, 
	      0.5*lightGuide_py3,
	      0.5*lightGuide_pz3);           //its size
                
  G4LogicalVolume* logicLightGuide3 =                         
    new G4LogicalVolume(solidLightGuide3,    //its solid
                        glass,               //its material
                        "LightGuide3");      //its name
  logicLightGuide3->SetVisAttributes(lightGuideVisAtt);               
  
  
  for(int i = 1; i <= detNum3; i++)
    {
      G4RotationMatrix crystal_rotm = G4RotationMatrix();
      G4ThreeVector d0 = G4ThreeVector(distanceFromTarget3*sin(coneangle3)*cos(0.5*360*deg/detNum3), 
				       distanceFromTarget3*sin(coneangle3)*sin(0.5*360*deg/detNum3), 
				       distanceFromTarget3*cos(coneangle3));
      G4ThreeVector df = G4ThreeVector(distanceFromTarget3*sin(theta3)*sin((i-1)*360*deg/detNum3+.001*deg/*+0.5*360*deg/detNum3*/), 
				       distanceFromTarget3*sin(theta3)*cos((i-1)*360*deg/detNum3+.001*deg/*+0.5*360*deg/detNum3*/), 
				       -distanceFromTarget3*cos(theta3));
      G4ThreeVector axis = d0.cross(df);
      G4double ax = axis.x();
      G4double ay = axis.y();
      G4double az = axis.z();
      G4double ux = ax/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double uy = ay/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double uz = az/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double j = acos(d0.dot(df)/(d0.mag()*df.mag()));
      G4ThreeVector a = G4ThreeVector(cos(j)+pow(ux,2)*(1-cos(j)), ux*uy*(1-cos(j))-uz*sin(j), ux*uz*(1-cos(j))+uy*sin(j));
      G4ThreeVector b = G4ThreeVector(uy*ux*(1-cos(j))+uz*sin(j), cos(j)+pow(uy,2)*(1-cos(j)), uy*uz*(1-cos(j))-ux*sin(j));
      G4ThreeVector c = G4ThreeVector(uz*ux*(1-cos(j))-uy*sin(j), uz*uy*(1-cos(j))+ux*sin(j), cos(j)+pow(uz,2)*(1-cos(j)));
      G4RotationMatrix abc = G4RotationMatrix();
      abc.setRows(a,b,c);
      G4ThreeVector unitdf = df/df.mag();
      G4double udfx = unitdf.x();
      G4double udfy = unitdf.y();
      G4double udfz = unitdf.z();
      G4double k = 180.*deg;
      G4ThreeVector d = G4ThreeVector(cos(k)+pow(udfx,2)*(1-cos(k)), udfx*udfy*(1-cos(k))-udfz*sin(k), udfx*udfz*(1-cos(k))+udfy*sin(k));
      G4ThreeVector e = G4ThreeVector(udfy*udfx*(1-cos(k))+udfz*sin(k), cos(k)+pow(udfy,2)*(1-cos(k)), udfy*udfz*(1-cos(k))-udfx*sin(k));
      G4ThreeVector f = G4ThreeVector(udfz*udfx*(1-cos(k))-udfy*sin(k), udfz*udfy*(1-cos(k))+udfx*sin(k), cos(k)+pow(udfz,2)*(1-cos(k)));
      G4RotationMatrix def = G4RotationMatrix();
      def.setRows(d,e,f);
      crystal_rotm=def*abc;
      if (m_Flip == 0){
	crystal_rotm.rotateY(180*deg);
      }
      crystal_rotm.rotateZ(180*deg);
      G4ThreeVector crystal_pos = G4ThreeVector(0,0,0);
      G4Transform3D crystal_trans = G4Transform3D(crystal_rotm, crystal_pos);
      G4RotationMatrix lg_rotm = G4RotationMatrix();
      G4ThreeVector lg_Axis = df.cross(z3);
      lg_Axis = lg_Axis/lg_Axis.mag();
      lg_Axis.rotateZ(180*deg);
      G4double ulgx = lg_Axis.x();
      G4double ulgy = lg_Axis.y();
      G4double ulgz = lg_Axis.z();
      G4ThreeVector p = G4ThreeVector(cos(theta3)+pow(ulgx,2)*(1-cos(theta3)), ulgx*ulgy*(1-cos(theta3))-ulgz*sin(theta3), ulgx*ulgz*(1-cos(theta3))+ulgy*sin(theta3));
      G4ThreeVector q = G4ThreeVector(ulgy*ulgx*(1-cos(theta3))+ulgz*sin(theta3), cos(theta3)+pow(ulgy,2)*(1-cos(theta3)), ulgy*ulgz*(1-cos(theta3))-ulgx*sin(theta3));
      G4ThreeVector r = G4ThreeVector(ulgz*ulgx*(1-cos(theta3))-ulgy*sin(theta3), ulgz*ulgy*(1-cos(theta3))+ulgx*sin(theta3), cos(theta3)+pow(ulgz,2)*(1-cos(theta3)));
      G4RotationMatrix pqr = G4RotationMatrix();
      pqr.setRows(p,q,r);
      lg_rotm.rotateZ((i-1)*360*deg/detNum3);
      lg_rotm.rotateX(180*deg);
      lg_rotm=pqr*lg_rotm;
      G4ThreeVector lg_pos = (distanceFromTarget3+detThickness3+0.5*lgThickness3)/df.mag()*df;
      lg_pos.rotateZ(180*deg);
      if (m_Flip == 0){
	lg_pos.rotateY(180*deg);
	lg_rotm.rotateY(180*deg);
      }
      G4Transform3D lg_trans = G4Transform3D(lg_rotm, lg_pos);
      G4RotationMatrix delrin_rotm = G4RotationMatrix();
      G4ThreeVector w = G4ThreeVector(sin(theta3)*sin((i-1)*360*deg/detNum3+.001*deg), sin(theta3)*cos((i-1)*360*deg/detNum3+.001*deg), cos(theta3));
      G4ThreeVector delrin_Axis = w.cross(z3);
      delrin_Axis.rotateX(180*deg);
      delrin_Axis = delrin_Axis/delrin_Axis.mag();
      G4double udx = delrin_Axis.x();
      G4double udy = delrin_Axis.y();
      G4double udz = delrin_Axis.z();
      G4double delAng = 90*deg+theta3;
      G4ThreeVector pdel = G4ThreeVector(cos(delAng)+pow(udx,2)*(1-cos(delAng)), udx*udy*(1-cos(delAng))-udz*sin(delAng), udx*udz*(1-cos(delAng))+udy*sin(delAng));
      G4ThreeVector qdel = G4ThreeVector(udy*udx*(1-cos(delAng))+udz*sin(delAng), cos(delAng)+pow(udy,2)*(1-cos(delAng)), udy*udz*(1-cos(delAng))-udx*sin(delAng));
      G4ThreeVector rdel = G4ThreeVector(udz*udx*(1-cos(delAng))-udy*sin(delAng), udz*udy*(1-cos(delAng))+udx*sin(delAng), cos(delAng)+pow(udz,2)*(1-cos(delAng)));
      G4RotationMatrix pqrdel = G4RotationMatrix();
      pqrdel.setRows(pdel,qdel,rdel);
      delrin_rotm.rotateZ((i-1)*360*deg/detNum3+.001*deg);
      delrin_rotm=pqrdel*delrin_rotm;
      delrin_rotm.rotateZ(0*deg);
      G4ThreeVector delrin_pos = (distanceFromTarget3+detThickness3+lgThickness3+delrinA3/2+.0003*mm)/df.mag()*df;  //the .0003 mm is a fudge factor to prevent overlapping
      delrin_pos.rotateZ(180*deg);
      delrin_pos.rotateY(180*deg);
      if (m_Flip == 1){
	delrin_pos.rotateY(180*deg);
	delrin_rotm.rotateY(180*deg);
      }
      G4Transform3D delrin_trans = G4Transform3D(delrin_rotm, delrin_pos);
      //if(i != 1){
      bool placeCrystal = true;
      for (unsigned int i2=0; i2 < m_Array.size(); i2++){
	if (16 < m_Array[i2] && m_Array[i2] < 29){
	  if (m_Array[i2] == 16 +(i -detNum3/2)%detNum3){
	    placeCrystal = false;
	  }else if (m_Array[i2] == 16 +(i -detNum3/2)%detNum3 +detNum3){
	    placeCrystal = false;
	  }
	}
      }
      if (placeCrystal){
      if(crystalBool) {new G4PVPlacement(crystal_trans,       //rotation, position
                    logicCrystal3,           //its logical volume
                    "CsI Detector3",         //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
                    16+i,                     //copy number
                    checkOverlaps);          //overlaps checking
      }
      if(lgBool) {new G4PVPlacement(lg_trans,            //rotation, position
                    logicLightGuide3,        //its logical volume
                    "LightGuide3",           //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
		    16+i,                     //copy number
                    checkOverlaps);          //overlaps checking
      }
      if(delrinBool) {new G4PVPlacement(delrin_trans,          //rotation, position
                    logicDelrin3,               //its logical volume
                    "Delrin3",                  //its name
                    world,                 //its mother  volume
                    false,                      //no boolean operation
                    16+i,                        //copy number
                    checkOverlaps);             //overlaps checking
      }
      }
      //}
    }}

  if(m_Ring4){
  // Ring 4 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  G4int detNum4 = 12;
  G4double distanceFromTargetBack4 = 50.*mm;
  G4double theta4 = 52.0*deg;
  G4double halfTheta4 = 8.0*deg; // fudge factor
  G4double lgThickness4 = 6.0*mm;
  G4double detThickness4 = 6.0*mm;
  G4double distanceFromTarget4 = distanceFromTargetBack4 - detThickness4;

  //G4double distance4 = distanceFromTarget4*sin(theta4); //this gives the distance from the beam axis.

  //
  // Delrin Ring
  //
  G4double delrinA4 = 5*mm;
  G4double delrinB4 = 10*mm;
  G4double fdelrinHalfAngle4 = atan(delrinB4/2/(distanceFromTarget4+detThickness4+lgThickness4));
  G4double firs4 = pow(pow(distanceFromTarget4+detThickness4+lgThickness4,2)+pow(delrinB4/2,2),0.5)*sin(theta4-fdelrinHalfAngle4);
  G4double firb4 = pow(pow(distanceFromTarget4+detThickness4+lgThickness4,2)+pow(delrinB4/2,2),0.5)*sin(theta4+fdelrinHalfAngle4);
  G4double bdelrinHalfAngle4 = atan(delrinB4/2/(distanceFromTarget4+detThickness4+lgThickness4+delrinA4));
  G4double birs4 = pow(pow(distanceFromTarget4+detThickness4+lgThickness4+delrinA4,2)+pow(delrinB4/2,2),0.5)*sin(theta4-bdelrinHalfAngle4);
  G4double birb4 = pow(pow(distanceFromTarget4+detThickness4+lgThickness4+delrinA4,2)+pow(delrinB4/2,2),0.5)*sin(theta4+bdelrinHalfAngle4);

  G4double delrin_dz4 = delrinB4/2;
  G4double delrin_theta4 = 0;
  G4double delrin_phi4 = 0;
  G4double delrin_dyab4 = delrinA4/2;
  G4double delrin_dxa4 = firs4*tan(180*deg/detNum4);
  G4double delrin_dxb4 = birs4*tan(180*deg/detNum4);
  G4double delrin_dycd4 = delrinA4/2;
  G4double delrin_dxc4 = firb4*tan(180*deg/detNum4);
  G4double delrin_dxd4 = birb4*tan(180*deg/detNum4);
  G4double delrin_alpab4 = 0;
  G4double delrin_alpcd4 = delrin_alpab4;

            
  G4ThreeVector delrin_pos4 = G4ThreeVector(0,0,distanceFromTarget4*cos(theta4));

  G4Trap* solidDelrin4 =    
    new G4Trap("Delrin4",                       //its name
	      delrin_dz4, 
	      delrin_theta4, delrin_phi4,
	      delrin_dyab4,
	      delrin_dxa4, delrin_dxb4,
	      delrin_alpab4,
	      delrin_dycd4,
	      delrin_dxc4, delrin_dxd4,
	      delrin_alpcd4);                   //its size
      
  G4LogicalVolume* logicDelrin4 =                         
    new G4LogicalVolume(solidDelrin4,           //its solid
                        delrin,                 //its material
                        "Delrin4");             //its name
  logicDelrin4->SetVisAttributes(delrinVisAtt);
  
  
  //
  // CsI Detector
  //
  G4double coneheight4 = distanceFromTarget4;
  G4double flatheight4 = coneheight4 - 0.01*mm;
  G4double coneangle4 = theta4;
  G4double crystal_rmina4 = (coneheight4-flatheight4)*tan(coneangle4-halfTheta4);
  G4double crystal_rmaxa4 = (coneheight4-flatheight4)*tan(coneangle4+halfTheta4);
  G4double crystal_rminb4 = coneheight4*tan(coneangle4-halfTheta4);
  G4double crystal_rmaxb4 = coneheight4*tan(coneangle4+halfTheta4);
  G4double crystal_hz4 = 0.5*flatheight4;//(crystal_rmaxb4-crystal_rmaxa4)/crystal_rmaxb4*coneheight4;
  G4double crystal_phimin4 = 0.*deg, crystal_phimax4 = 360.*deg/detNum4;
  G4RotationMatrix tip_rotm4 = G4RotationMatrix();
  G4ThreeVector tip_pos4 = G4ThreeVector(0,0,0.5*flatheight4+(coneheight4-flatheight4));
  G4Transform3D tip_trans4 = G4Transform3D(tip_rotm4, tip_pos4);
  G4ThreeVector z4 = G4ThreeVector(0,0,1);
  G4ThreeVector d04 = G4ThreeVector(distanceFromTarget4*sin(coneangle4)*cos(0.5*360*deg/detNum4), 
				    distanceFromTarget4*sin(coneangle4)*sin(0.5*360*deg/detNum4), 
				    distanceFromTarget4*cos(coneangle4));
  G4ThreeVector unitAxis4 = z4.cross(d04);
  unitAxis4 = unitAxis4/unitAxis4.mag();
  G4double ux4 = unitAxis4.x();
  G4double uy4 = unitAxis4.y();
  G4double uz4 = unitAxis4.z();
  G4ThreeVector top4 = G4ThreeVector(cos(coneangle4)+pow(ux4,2)*(1-cos(coneangle4)), ux4*uy4*(1-cos(coneangle4))-uz4*sin(coneangle4), ux4*uz4*(1-cos(coneangle4))+uy4*sin(coneangle4));
  G4ThreeVector mid4 = G4ThreeVector(uy4*ux4*(1-cos(coneangle4))+uz4*sin(coneangle4), cos(coneangle4)+pow(uy4,2)*(1-cos(coneangle4)), uy4*uz4*(1-cos(coneangle4))-ux4*sin(coneangle4));
  G4ThreeVector bot4 = G4ThreeVector(uz4*ux4*(1-cos(coneangle4))-uy4*sin(coneangle4), uz4*uy4*(1-cos(coneangle4))+ux4*sin(coneangle4), cos(coneangle4)+pow(uz4,2)*(1-cos(coneangle4)));
  G4RotationMatrix firstBox_rotm4 = G4RotationMatrix();
  firstBox_rotm4.setRows(top4,mid4,bot4);
  G4ThreeVector firstBox_pos4 = G4ThreeVector(0,0,0);
  G4Transform3D firstBox_trans4 = G4Transform3D(firstBox_rotm4, firstBox_pos4);
  G4RotationMatrix secondBox_rotm4 = G4RotationMatrix();
  secondBox_rotm4.setRows(top4,mid4,bot4);
  G4ThreeVector secondBox_pos4 = (2*distanceFromTarget4+detThickness4)/distanceFromTarget4*d04;
  G4Transform3D secondBox_trans4 = G4Transform3D(secondBox_rotm4, secondBox_pos4);
  
  G4Orb* solidTip4 =
    new G4Orb("tip4",
	      .0001*mm);
  G4Box* solidSubtractionBox4 = 
    new G4Box("SubtractionBox4",                  //its name
	      distanceFromTarget4,
	      distanceFromTarget4,
	      distanceFromTarget4);               //its size
  G4Cons* solidCone4 =    
    new G4Cons("Cone4",                           //its name
	       crystal_rmina4, crystal_rmaxa4, 
	       crystal_rminb4, crystal_rmaxb4, 
	       crystal_hz4,
	       crystal_phimin4, crystal_phimax4); //its size
  G4UnionSolid* solidTippedCone4 =
    new G4UnionSolid("TippedCone4",
		     solidTip4,
		     solidCone4,
		     tip_trans4);
  G4SubtractionSolid* solidIntermediate4 =
    new G4SubtractionSolid("Intermediate4",
			   solidTippedCone4,
			   solidSubtractionBox4,
			   firstBox_trans4);
  G4SubtractionSolid* solidCrystal4 =
    new G4SubtractionSolid("Crystal4",
			   solidIntermediate4,
			   solidSubtractionBox4,
			   secondBox_trans4);
  
  G4LogicalVolume* logicCrystal4 =                         
    new G4LogicalVolume(solidCrystal4,            //its solid
                        CsI,                      //its material
                        "CsI Detector4");         //its name
  logicCrystal4->SetVisAttributes(crystalVisAtt);
  logicCrystal4->SetSensitiveDetector(m_MicroballScorer);
  
  //
  // Light Guide
  //       
  G4double lightGuide_px4 = 10*mm;
  G4double lightGuide_py4 = 10*mm;
  G4double lightGuide_pz4  = lgThickness4;      
  G4Box* solidLightGuide4 =    
    new G4Box("LightGuide4",                 //its name
              0.5*lightGuide_px4, 
	      0.5*lightGuide_py4,
	      0.5*lightGuide_pz4);           //its size
                
  G4LogicalVolume* logicLightGuide4 =                         
    new G4LogicalVolume(solidLightGuide4,    //its solid
                        glass,               //its material
                        "LightGuide4");      //its name
  logicLightGuide4->SetVisAttributes(lightGuideVisAtt);               
  
  
  for(int i = 1; i <= detNum4; i++)
    {
      G4RotationMatrix crystal_rotm = G4RotationMatrix();
      G4ThreeVector d0 = G4ThreeVector(distanceFromTarget4*sin(coneangle4)*cos(0.5*360*deg/detNum4), 
				       distanceFromTarget4*sin(coneangle4)*sin(0.5*360*deg/detNum4), 
				       distanceFromTarget4*cos(coneangle4));
      G4ThreeVector df = G4ThreeVector(distanceFromTarget4*sin(theta4)*sin((i-1)*360*deg/detNum4+.001*deg/*+0.5*360*deg/detNum4*/), 
				       distanceFromTarget4*sin(theta4)*cos((i-1)*360*deg/detNum4+.001*deg/*+0.5*360*deg/detNum4*/), 
				       -distanceFromTarget4*cos(theta4));
      G4ThreeVector axis = d0.cross(df);
      G4double ax = axis.x();
      G4double ay = axis.y();
      G4double az = axis.z();
      G4double ux = ax/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double uy = ay/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double uz = az/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double j = acos(d0.dot(df)/(d0.mag()*df.mag()));
      G4ThreeVector a = G4ThreeVector(cos(j)+pow(ux,2)*(1-cos(j)), ux*uy*(1-cos(j))-uz*sin(j), ux*uz*(1-cos(j))+uy*sin(j));
      G4ThreeVector b = G4ThreeVector(uy*ux*(1-cos(j))+uz*sin(j), cos(j)+pow(uy,2)*(1-cos(j)), uy*uz*(1-cos(j))-ux*sin(j));
      G4ThreeVector c = G4ThreeVector(uz*ux*(1-cos(j))-uy*sin(j), uz*uy*(1-cos(j))+ux*sin(j), cos(j)+pow(uz,2)*(1-cos(j)));
      G4RotationMatrix abc = G4RotationMatrix();
      abc.setRows(a,b,c);
      G4ThreeVector unitdf = df/df.mag();
      G4double udfx = unitdf.x();
      G4double udfy = unitdf.y();
      G4double udfz = unitdf.z();
      G4double k = 180.*deg;
      G4ThreeVector d = G4ThreeVector(cos(k)+pow(udfx,2)*(1-cos(k)), udfx*udfy*(1-cos(k))-udfz*sin(k), udfx*udfz*(1-cos(k))+udfy*sin(k));
      G4ThreeVector e = G4ThreeVector(udfy*udfx*(1-cos(k))+udfz*sin(k), cos(k)+pow(udfy,2)*(1-cos(k)), udfy*udfz*(1-cos(k))-udfx*sin(k));
      G4ThreeVector f = G4ThreeVector(udfz*udfx*(1-cos(k))-udfy*sin(k), udfz*udfy*(1-cos(k))+udfx*sin(k), cos(k)+pow(udfz,2)*(1-cos(k)));
      G4RotationMatrix def = G4RotationMatrix();
      def.setRows(d,e,f);
      crystal_rotm=def*abc;
      if (m_Flip == 0){
	crystal_rotm.rotateY(180*deg);
      }
      crystal_rotm.rotateZ(180*deg);
      G4ThreeVector crystal_pos = G4ThreeVector(0,0,0);
      G4Transform3D crystal_trans = G4Transform3D(crystal_rotm, crystal_pos);
      G4RotationMatrix lg_rotm = G4RotationMatrix();
      G4ThreeVector lg_Axis = df.cross(z4);
      lg_Axis = lg_Axis/lg_Axis.mag();
      lg_Axis.rotateZ(180*deg);
      G4double ulgx = lg_Axis.x();
      G4double ulgy = lg_Axis.y();
      G4double ulgz = lg_Axis.z();
      G4ThreeVector p = G4ThreeVector(cos(theta4)+pow(ulgx,2)*(1-cos(theta4)), ulgx*ulgy*(1-cos(theta4))-ulgz*sin(theta4), ulgx*ulgz*(1-cos(theta4))+ulgy*sin(theta4));
      G4ThreeVector q = G4ThreeVector(ulgy*ulgx*(1-cos(theta4))+ulgz*sin(theta4), cos(theta4)+pow(ulgy,2)*(1-cos(theta4)), ulgy*ulgz*(1-cos(theta4))-ulgx*sin(theta4));
      G4ThreeVector r = G4ThreeVector(ulgz*ulgx*(1-cos(theta4))-ulgy*sin(theta4), ulgz*ulgy*(1-cos(theta4))+ulgx*sin(theta4), cos(theta4)+pow(ulgz,2)*(1-cos(theta4)));
      G4RotationMatrix pqr = G4RotationMatrix();
      pqr.setRows(p,q,r);
      lg_rotm.rotateZ((i-1)*360*deg/detNum4);
      lg_rotm.rotateX(180*deg);
      lg_rotm=pqr*lg_rotm;
      G4ThreeVector lg_pos = (distanceFromTarget4+detThickness4+0.5*lgThickness4)/df.mag()*df;
      lg_pos.rotateZ(180*deg);
      if (m_Flip == 0){
	lg_pos.rotateY(180*deg);
	lg_rotm.rotateY(180*deg);
      }
      G4Transform3D lg_trans = G4Transform3D(lg_rotm, lg_pos);
      G4RotationMatrix delrin_rotm = G4RotationMatrix();
      G4ThreeVector w = G4ThreeVector(sin(theta4)*sin((i-1)*360*deg/detNum4+.001*deg), sin(theta4)*cos((i-1)*360*deg/detNum4+.001*deg), cos(theta4));
      G4ThreeVector delrin_Axis = w.cross(z4);
      delrin_Axis.rotateX(180*deg);
      delrin_Axis = delrin_Axis/delrin_Axis.mag();
      G4double udx = delrin_Axis.x();
      G4double udy = delrin_Axis.y();
      G4double udz = delrin_Axis.z();
      G4double delAng = 90*deg+theta4;
      G4ThreeVector pdel = G4ThreeVector(cos(delAng)+pow(udx,2)*(1-cos(delAng)), udx*udy*(1-cos(delAng))-udz*sin(delAng), udx*udz*(1-cos(delAng))+udy*sin(delAng));
      G4ThreeVector qdel = G4ThreeVector(udy*udx*(1-cos(delAng))+udz*sin(delAng), cos(delAng)+pow(udy,2)*(1-cos(delAng)), udy*udz*(1-cos(delAng))-udx*sin(delAng));
      G4ThreeVector rdel = G4ThreeVector(udz*udx*(1-cos(delAng))-udy*sin(delAng), udz*udy*(1-cos(delAng))+udx*sin(delAng), cos(delAng)+pow(udz,2)*(1-cos(delAng)));
      G4RotationMatrix pqrdel = G4RotationMatrix();
      pqrdel.setRows(pdel,qdel,rdel);
      delrin_rotm.rotateZ((i-1)*360*deg/detNum4+.001*deg);
      delrin_rotm=pqrdel*delrin_rotm;
      delrin_rotm.rotateZ(0*deg);
      G4ThreeVector delrin_pos = (distanceFromTarget4+detThickness4+lgThickness4+delrinA4/2+.0001*mm)/df.mag()*df;  //the .0003 mm is a fudge factor to prevent overlapping
      delrin_pos.rotateZ(180*deg);
      delrin_pos.rotateY(180*deg);
      if (m_Flip == 1){
	delrin_pos.rotateY(180*deg);
	delrin_rotm.rotateY(180*deg);
      }
      G4Transform3D delrin_trans = G4Transform3D(delrin_rotm, delrin_pos);      
      //if(i != 1){
      bool placeCrystal = true;
      for (unsigned int i2=0; i2 < m_Array.size(); i2++){
	if (28 < m_Array[i2] && m_Array[i2] < 41){
	  if (m_Array[i2] == 28 +(i -detNum4/2)%detNum4){
	    placeCrystal = false;
	  }else if (m_Array[i2] == 28 +(i -detNum4/2)%detNum4 +detNum4){
	    placeCrystal = false;
	  }
	}
      }
      if (placeCrystal){
      if(crystalBool) {new G4PVPlacement(crystal_trans,       //rotation, position
                    logicCrystal4,           //its logical volume
                    "CsI Detector4",         //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
                    28+i,                     //copy number
                    checkOverlaps);          //overlaps checking
      }
      if(lgBool) {new G4PVPlacement(lg_trans,            //rotation, position
                    logicLightGuide4,        //its logical volume
                    "LightGuide4",           //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
		    28+i,                     //copy number
                    checkOverlaps);          //overlaps checking
      }
      if(delrinBool) {new G4PVPlacement(delrin_trans,          //rotation, position
                    logicDelrin4,               //its logical volume
                    "Delrin4",                  //its name
                    world,                 //its mother  volume
                    false,                      //no boolean operation
                    28+i,                        //copy number
                    checkOverlaps);             //overlaps checking
      }
      }
      //}
    }}

  if(m_Ring5){
  // Ring 5 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  G4int detNum5 = 14;
  G4double distanceFromTargetBack5 = 50.*mm;
  G4double theta5 = 70.0*deg;
  G4double halfTheta5 = 10.0*deg;// -0.1*deg; //fudge factor
  G4double lgThickness5 = 6.0*mm;
  G4double detThickness5 = 5.6*mm;
  G4double distanceFromTarget5 = distanceFromTargetBack5 - detThickness5;

  //G4double distance5 = distanceFromTarget5*sin(theta5); //this gives the distance from the beam axis.

  //
  // Delrin Ring
  //
  G4double delrinA5 = 5*mm;
  G4double delrinB5 = 10*mm;
  G4double fdelrinHalfAngle5 = atan(delrinB5/2/(distanceFromTarget5+detThickness5+lgThickness5));
  G4double firs5 = pow(pow(distanceFromTarget5+detThickness5+lgThickness5,2)+pow(delrinB5/2,2),0.5)*sin(theta5-fdelrinHalfAngle5);
  G4double firb5 = pow(pow(distanceFromTarget5+detThickness5+lgThickness5,2)+pow(delrinB5/2,2),0.5)*sin(theta5+fdelrinHalfAngle5);
  G4double bdelrinHalfAngle5 = atan(delrinB5/2/(distanceFromTarget5+detThickness5+lgThickness5+delrinA5));
  G4double birs5 = pow(pow(distanceFromTarget5+detThickness5+lgThickness5+delrinA5,2)+pow(delrinB5/2,2),0.5)*sin(theta5-bdelrinHalfAngle5);
  G4double birb5 = pow(pow(distanceFromTarget5+detThickness5+lgThickness5+delrinA5,2)+pow(delrinB5/2,2),0.5)*sin(theta5+bdelrinHalfAngle5);

  G4double delrin_dz5 = delrinB5/2;
  G4double delrin_theta5 = 0;
  G4double delrin_phi5 = 0;
  G4double delrin_dyab5 = delrinA5/2;
  G4double delrin_dxa5 = firs5*tan(180*deg/detNum5);
  G4double delrin_dxb5 = birs5*tan(180*deg/detNum5);
  G4double delrin_dycd5 = delrinA5/2;
  G4double delrin_dxc5 = firb5*tan(180*deg/detNum5);
  G4double delrin_dxd5 = birb5*tan(180*deg/detNum5);
  G4double delrin_alpab5 = 0;
  G4double delrin_alpcd5 = delrin_alpab5;

            
  G4ThreeVector delrin_pos5 = G4ThreeVector(0,0,distanceFromTarget5*cos(theta5));

  G4Trap* solidDelrin5 =    
    new G4Trap("Delrin5",                       //its name
	      delrin_dz5, 
	      delrin_theta5, delrin_phi5,
	      delrin_dyab5,
	      delrin_dxa5, delrin_dxb5,
	      delrin_alpab5,
	      delrin_dycd5,
	      delrin_dxc5, delrin_dxd5,
	      delrin_alpcd5);                   //its size
      
  G4LogicalVolume* logicDelrin5 =                         
    new G4LogicalVolume(solidDelrin5,           //its solid
                        delrin,                 //its material
                        "Delrin5");             //its name
  logicDelrin5->SetVisAttributes(delrinVisAtt);
  
    
  //
  // CsI Detector
  //
  G4double coneheight5 = 0.6*distanceFromTarget5;
  G4double flatheight5 = coneheight5 -0.01*mm;
  G4double coneangle5 = theta5;
  G4double crystal_rmina5 = (coneheight5-flatheight5)*tan(coneangle5-halfTheta5);
  G4double crystal_rmaxa5 = (coneheight5-flatheight5)*tan(coneangle5+halfTheta5);
  G4double crystal_rminb5 = coneheight5*tan(coneangle5-halfTheta5);
  G4double crystal_rmaxb5 = coneheight5*tan(coneangle5+halfTheta5);
  G4double crystal_hz5 = 0.5*flatheight5;//(crystal_rmaxb5-crystal_rmaxa5)/crystal_rmaxb5*coneheight5;
  G4double crystal_phimin5 = 0.*deg, crystal_phimax5 = 360.*deg/detNum5;
  G4RotationMatrix tip_rotm5 = G4RotationMatrix();
  G4ThreeVector tip_pos5 = G4ThreeVector(0,0,0.5*flatheight5+(coneheight5-flatheight5)); //last z term is a fudge
  G4Transform3D tip_trans5 = G4Transform3D(tip_rotm5, tip_pos5);
  G4ThreeVector z5 = G4ThreeVector(0,0,1);
  G4ThreeVector d05 = G4ThreeVector(distanceFromTarget5*sin(coneangle5)*cos(0.5*360*deg/detNum5), 
				    distanceFromTarget5*sin(coneangle5)*sin(0.5*360*deg/detNum5), 
				    distanceFromTarget5*cos(coneangle5));
  G4ThreeVector unitAxis5 = z5.cross(d05);
  unitAxis5 = unitAxis5/unitAxis5.mag();
  G4double ux5 = unitAxis5.x();
  G4double uy5 = unitAxis5.y();
  G4double uz5 = unitAxis5.z();
  G4ThreeVector top5 = G4ThreeVector(cos(coneangle5)+pow(ux5,2)*(1-cos(coneangle5)), ux5*uy5*(1-cos(coneangle5))-uz5*sin(coneangle5), ux5*uz5*(1-cos(coneangle5))+uy5*sin(coneangle5));
  G4ThreeVector mid5 = G4ThreeVector(uy5*ux5*(1-cos(coneangle5))+uz5*sin(coneangle5), cos(coneangle5)+pow(uy5,2)*(1-cos(coneangle5)), uy5*uz5*(1-cos(coneangle5))-ux5*sin(coneangle5));
  G4ThreeVector bot5 = G4ThreeVector(uz5*ux5*(1-cos(coneangle5))-uy5*sin(coneangle5), uz5*uy5*(1-cos(coneangle5))+ux5*sin(coneangle5), cos(coneangle5)+pow(uz5,2)*(1-cos(coneangle5)));
  G4RotationMatrix firstBox_rotm5 = G4RotationMatrix();
  firstBox_rotm5.setRows(top5,mid5,bot5);
  G4ThreeVector firstBox_pos5 = G4ThreeVector(0,0,0);
  G4Transform3D firstBox_trans5 = G4Transform3D(firstBox_rotm5, firstBox_pos5);
  G4RotationMatrix secondBox_rotm5 = G4RotationMatrix();
  secondBox_rotm5.setRows(top5,mid5,bot5);
  G4ThreeVector secondBox_pos5 = (3*distanceFromTarget5+detThickness5)/distanceFromTarget5*d05;
  G4Transform3D secondBox_trans5 = G4Transform3D(secondBox_rotm5, secondBox_pos5);
  
  G4Orb* solidTip5 =
    new G4Orb("tip5",
	      .0001*mm);
  G4Box* solidSubtractionBox5 = 
    new G4Box("SubtractionBox5",                  //its name
	      distanceFromTarget5,
	      distanceFromTarget5,
	      distanceFromTarget5);               //its size
  G4Box* solidSecondSubtractionBox5 = 
    new G4Box("SecondSubtractionBox5",            //its name
	      2*distanceFromTarget5,
	      2*distanceFromTarget5,
	      2*distanceFromTarget5);             //its size
  G4Cons* solidCone5 =    
    new G4Cons("Cone5",                           //its name
	       crystal_rmina5, crystal_rmaxa5, 
	       crystal_rminb5, crystal_rmaxb5, 
	       crystal_hz5,
	       crystal_phimin5, crystal_phimax5); //its size
  G4UnionSolid* solidTippedCone5 =
    new G4UnionSolid("TippedCone5",
		     solidTip5,
		     solidCone5,
		     tip_trans5);
  G4SubtractionSolid* solidIntermediate5 =
    new G4SubtractionSolid("Intermediate5",
			   solidTippedCone5,
			   solidSubtractionBox5,
			   firstBox_trans5);
  G4SubtractionSolid* solidCrystal5 =
    new G4SubtractionSolid("Crystal5",
			   solidIntermediate5,
			   solidSecondSubtractionBox5,
			   secondBox_trans5);
  
  G4LogicalVolume* logicCrystal5 =                         
    new G4LogicalVolume(solidCrystal5,            //its solid
                        CsI,                      //its material
                        "CsI Detector5");         //its name
  logicCrystal5->SetVisAttributes(crystalVisAtt);
  logicCrystal5->SetSensitiveDetector(m_MicroballScorer);
  
  
  //
  // Light Guide
  //       
  G4double lightGuide_px5 = 10*mm;
  G4double lightGuide_py5 = 10*mm;
  G4double lightGuide_pz5  = lgThickness5;      
  G4Box* solidLightGuide5 =    
    new G4Box("LightGuide5",                 //its name
              0.5*lightGuide_px5, 
	      0.5*lightGuide_py5,
	      0.5*lightGuide_pz5);           //its size
                
  G4LogicalVolume* logicLightGuide5 =                         
    new G4LogicalVolume(solidLightGuide5,    //its solid
                        glass,               //its material
                        "LightGuide5");      //its name
  logicLightGuide5->SetVisAttributes(lightGuideVisAtt);               
  
  
  for(int i = 1; i <= detNum5; i++)
    {
      G4RotationMatrix crystal_rotm = G4RotationMatrix();
      G4ThreeVector d0 = G4ThreeVector(distanceFromTarget5*sin(coneangle5)*cos(0.5*360*deg/detNum5), 
				       distanceFromTarget5*sin(coneangle5)*sin(0.5*360*deg/detNum5), 
				       distanceFromTarget5*cos(coneangle5));
      G4ThreeVector df = G4ThreeVector(distanceFromTarget5*sin(theta5)*sin((i-1)*360*deg/detNum5+.001*deg/*+0.5*360*deg/detNum5*/), 
				       distanceFromTarget5*sin(theta5)*cos((i-1)*360*deg/detNum5+.001*deg/*+0.5*360*deg/detNum5*/), 
				       -distanceFromTarget5*cos(theta5));
      G4ThreeVector axis = d0.cross(df);
      G4double ax = axis.x();
      G4double ay = axis.y();
      G4double az = axis.z();
      G4double ux = ax/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double uy = ay/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double uz = az/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double j = acos(d0.dot(df)/(d0.mag()*df.mag()));
      G4ThreeVector a = G4ThreeVector(cos(j)+pow(ux,2)*(1-cos(j)), ux*uy*(1-cos(j))-uz*sin(j), ux*uz*(1-cos(j))+uy*sin(j));
      G4ThreeVector b = G4ThreeVector(uy*ux*(1-cos(j))+uz*sin(j), cos(j)+pow(uy,2)*(1-cos(j)), uy*uz*(1-cos(j))-ux*sin(j));
      G4ThreeVector c = G4ThreeVector(uz*ux*(1-cos(j))-uy*sin(j), uz*uy*(1-cos(j))+ux*sin(j), cos(j)+pow(uz,2)*(1-cos(j)));
      G4RotationMatrix abc = G4RotationMatrix();
      abc.setRows(a,b,c);
      G4ThreeVector unitdf = df/df.mag();
      G4double udfx = unitdf.x();
      G4double udfy = unitdf.y();
      G4double udfz = unitdf.z();
      G4double k = 180.*deg;
      G4ThreeVector d = G4ThreeVector(cos(k)+pow(udfx,2)*(1-cos(k)), udfx*udfy*(1-cos(k))-udfz*sin(k), udfx*udfz*(1-cos(k))+udfy*sin(k));
      G4ThreeVector e = G4ThreeVector(udfy*udfx*(1-cos(k))+udfz*sin(k), cos(k)+pow(udfy,2)*(1-cos(k)), udfy*udfz*(1-cos(k))-udfx*sin(k));
      G4ThreeVector f = G4ThreeVector(udfz*udfx*(1-cos(k))-udfy*sin(k), udfz*udfy*(1-cos(k))+udfx*sin(k), cos(k)+pow(udfz,2)*(1-cos(k)));
      G4RotationMatrix def = G4RotationMatrix();
      def.setRows(d,e,f);
      crystal_rotm=def*abc;
      if (m_Flip == 0){
	crystal_rotm.rotateY(180*deg);
      }
      crystal_rotm.rotateZ(180*deg);
      G4ThreeVector crystal_pos = G4ThreeVector(0,0,0);
      G4Transform3D crystal_trans = G4Transform3D(crystal_rotm, crystal_pos);
      G4RotationMatrix lg_rotm = G4RotationMatrix();
      G4ThreeVector lg_Axis = df.cross(z5);
      lg_Axis = lg_Axis/lg_Axis.mag();
      lg_Axis.rotateZ(180*deg);
      G4double ulgx = lg_Axis.x();
      G4double ulgy = lg_Axis.y();
      G4double ulgz = lg_Axis.z();
      G4ThreeVector p = G4ThreeVector(cos(theta5)+pow(ulgx,2)*(1-cos(theta5)), ulgx*ulgy*(1-cos(theta5))-ulgz*sin(theta5), ulgx*ulgz*(1-cos(theta5))+ulgy*sin(theta5));
      G4ThreeVector q = G4ThreeVector(ulgy*ulgx*(1-cos(theta5))+ulgz*sin(theta5), cos(theta5)+pow(ulgy,2)*(1-cos(theta5)), ulgy*ulgz*(1-cos(theta5))-ulgx*sin(theta5));
      G4ThreeVector r = G4ThreeVector(ulgz*ulgx*(1-cos(theta5))-ulgy*sin(theta5), ulgz*ulgy*(1-cos(theta5))+ulgx*sin(theta5), cos(theta5)+pow(ulgz,2)*(1-cos(theta5)));
      G4RotationMatrix pqr = G4RotationMatrix();
      pqr.setRows(p,q,r);
      lg_rotm.rotateZ((i-1)*360*deg/detNum5);
      lg_rotm.rotateX(180*deg);
      lg_rotm=pqr*lg_rotm;
      G4ThreeVector lg_pos = (distanceFromTarget5+detThickness5+0.5*lgThickness5)/df.mag()*df;
      lg_pos.rotateZ(180*deg);
      if (m_Flip == 0){
	lg_pos.rotateY(180*deg);
	lg_rotm.rotateY(180*deg);
      }
      G4Transform3D lg_trans = G4Transform3D(lg_rotm, lg_pos);
      G4RotationMatrix delrin_rotm = G4RotationMatrix();
      G4ThreeVector w = G4ThreeVector(sin(theta5)*sin((i-1)*360*deg/detNum5+.001*deg), sin(theta5)*cos((i-1)*360*deg/detNum5+.001*deg), cos(theta5));
      G4ThreeVector delrin_Axis = w.cross(z5);
      delrin_Axis.rotateX(180*deg);
      delrin_Axis = delrin_Axis/delrin_Axis.mag();
      G4double udx = delrin_Axis.x();
      G4double udy = delrin_Axis.y();
      G4double udz = delrin_Axis.z();
      G4double delAng = 90*deg+theta5;
      G4ThreeVector pdel = G4ThreeVector(cos(delAng)+pow(udx,2)*(1-cos(delAng)), udx*udy*(1-cos(delAng))-udz*sin(delAng), udx*udz*(1-cos(delAng))+udy*sin(delAng));
      G4ThreeVector qdel = G4ThreeVector(udy*udx*(1-cos(delAng))+udz*sin(delAng), cos(delAng)+pow(udy,2)*(1-cos(delAng)), udy*udz*(1-cos(delAng))-udx*sin(delAng));
      G4ThreeVector rdel = G4ThreeVector(udz*udx*(1-cos(delAng))-udy*sin(delAng), udz*udy*(1-cos(delAng))+udx*sin(delAng), cos(delAng)+pow(udz,2)*(1-cos(delAng)));
      G4RotationMatrix pqrdel = G4RotationMatrix();
      pqrdel.setRows(pdel,qdel,rdel);
      delrin_rotm.rotateZ((i-1)*360*deg/detNum5+.001*deg);
      delrin_rotm=pqrdel*delrin_rotm;
      delrin_rotm.rotateZ(0*deg);
      G4ThreeVector delrin_pos = (distanceFromTarget5+detThickness5+lgThickness5+delrinA5/2+.0004*mm)/df.mag()*df;  //the .0003 mm is a fudge factor to prevent overlapping
      delrin_pos.rotateZ(180*deg);
      delrin_pos.rotateY(180*deg);
      if (m_Flip == 1){
	delrin_pos.rotateY(180*deg);
	delrin_rotm.rotateY(180*deg);
      }
      G4Transform3D delrin_trans = G4Transform3D(delrin_rotm, delrin_pos);      
      //if(i != 1){
      bool placeCrystal = true;
      for (unsigned int i2=0; i2 < m_Array.size(); i2++){
	if (40 < m_Array[i2] && m_Array[i2] < 55){
	  if (m_Array[i2] == 40 +(i -detNum5/2)%detNum5){
	    placeCrystal = false;
	  }else if (m_Array[i2] == 40 +(i -detNum5/2)%detNum5 +detNum5){
	    placeCrystal = false;
	  }
	}
      }
      if (placeCrystal){
      if(crystalBool) {new G4PVPlacement(crystal_trans,       //rotation, position
                    logicCrystal5,           //its logical volume
                    "CsI Detector5",         //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
                    40+i,                     //copy number
                    checkOverlaps);          //overlaps checking
      }
      if(lgBool) {new G4PVPlacement(lg_trans,            //rotation, position
                    logicLightGuide5,        //its logical volume
                    "LightGuide5",           //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
		    40+i,                     //copy number
                    checkOverlaps);          //overlaps checking
      }
      //}
      if(delrinBool) {new G4PVPlacement(delrin_trans,          //rotation, position
                    logicDelrin5,               //its logical volume
                    "Delrin5",                  //its name
                    world,                 //its mother  volume
                    false,                      //no boolean operation
                    40+i,                        //copy number
                    checkOverlaps);             //overlaps checking
      }
      }
    }}
  
  if(m_Ring6){
  // Ring 6 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ this ring is weird.
  G4int detNum6 = 14;
  G4double distanceFromTargetBack6 = 50.*mm;
  G4double theta6 = 90.0*deg;
  G4double halfTheta6 = 10.0*deg;// -0.1*deg;
  G4double lgThickness6 = 7.0*mm;
  G4double detThickness6 = 5.2*mm;
  G4double distanceFromTarget6 = distanceFromTargetBack6 - detThickness6;

  //G4double distance6 = distanceFromTarget6*sin(theta6); //this gives the distance from the beam axis.

  //
  // Delrin Ring
  //
  G4double delrinA6 = 5*mm;
  G4double delrinB6 = 10*mm;
  G4double fdelrinHalfAngle6 = atan(delrinB6/2/(distanceFromTarget6+detThickness6+lgThickness6));
  G4double firs6 = pow(pow(distanceFromTarget6+detThickness6+lgThickness6,2)+pow(delrinB6/2,2),0.5)*sin(theta6-fdelrinHalfAngle6);
  G4double firb6 = pow(pow(distanceFromTarget6+detThickness6+lgThickness6,2)+pow(delrinB6/2,2),0.5)*sin(theta6+fdelrinHalfAngle6);
  G4double bdelrinHalfAngle6 = atan(delrinB6/2/(distanceFromTarget6+detThickness6+lgThickness6+delrinA6));
  G4double birs6 = pow(pow(distanceFromTarget6+detThickness6+lgThickness6+delrinA6,2)+pow(delrinB6/2,2),0.5)*sin(theta6-bdelrinHalfAngle6);
  G4double birb6 = pow(pow(distanceFromTarget6+detThickness6+lgThickness6+delrinA6,2)+pow(delrinB6/2,2),0.5)*sin(theta6+bdelrinHalfAngle6);

  G4double delrin_dz6 = delrinB6/2;
  G4double delrin_theta6 = 0;
  G4double delrin_phi6 = 0;
  G4double delrin_dyab6 = delrinA6/2;
  G4double delrin_dxa6 = firs6*tan(180*deg/detNum6);
  G4double delrin_dxb6 = birs6*tan(180*deg/detNum6);
  G4double delrin_dycd6 = delrinA6/2;
  G4double delrin_dxc6 = firb6*tan(180*deg/detNum6);
  G4double delrin_dxd6 = birb6*tan(180*deg/detNum6);
  G4double delrin_alpab6 = 0;
  G4double delrin_alpcd6 = delrin_alpab6;

            
  G4ThreeVector delrin_pos6 = G4ThreeVector(0,0,distanceFromTarget6*cos(theta6));

  G4Trap* solidDelrin6 =    
    new G4Trap("Delrin6",                       //its name
	      delrin_dz6, 
	      delrin_theta6, delrin_phi6,
	      delrin_dyab6,
	      delrin_dxa6, delrin_dxb6,
	      delrin_alpab6,
	      delrin_dycd6,
	      delrin_dxc6, delrin_dxd6,
	      delrin_alpcd6);                   //its size
      
  G4LogicalVolume* logicDelrin6 =                         
    new G4LogicalVolume(solidDelrin6,           //its solid
                        delrin,                 //its material
                        "Delrin6");             //its name
  logicDelrin6->SetVisAttributes(delrinVisAtt);
  

  /*
  //
  // CsI Detector for deletion
  //
  G4double crystal_rmina6 = distance6/cos(halfTheta6); 
  G4double crystal_rmaxa6 = distance6/cos(halfTheta6)+2*distance6*tan(halfTheta6);
  G4double crystal_rminb6 = (distance6+detThickness6)/cos(halfTheta6);
  G4double crystal_rmaxb6 = (distance6+detThickness6)/cos(halfTheta6)+2*(distance6+detThickness6)*tan(halfTheta6);
  G4double crystal_hz6 = 0.5*pow(pow(detThickness6,2)+pow((crystal_rminb6+crystal_rmaxb6-crystal_rmina6-crystal_rmaxa6)/2,2),0.5);
  //G4double crystal_hz6 = 0.5*detThickness6;
  G4double crystal_phimin6 = 0.*deg, crystal_phimax6 = 360.*deg/detNum6;
  

  G4Cons* solidCrystal6 =    
    new G4Cons("Crystal6",                        //its name
	       crystal_rmina6, crystal_rmaxa6, 
	       crystal_rminb6, crystal_rmaxb6, 
	       crystal_hz6,
	       crystal_phimin6, crystal_phimax6); //its size
      
  G4LogicalVolume* logicCrystal6 =                         
    new G4LogicalVolume(solidCrystal6,            //its solid
                        CsI,                      //its material
                        "CsI Detector6");         //its name
  logicCrystal6->SetVisAttributes(crystalVisAtt);
  */             
  
  G4double crystal_dxa6 = distanceFromTarget6*tan(halfTheta6);
  G4double crystal_dxb6 = (distanceFromTarget6+detThickness6)*tan(halfTheta6);
  G4double crystal_dya6 = distanceFromTarget6*tan(0.5*360*deg/detNum6);
  G4double crystal_dyb6 = (distanceFromTarget6+detThickness6)*tan(0.5*360*deg/detNum6);
  G4double crystal_dz = 0.5*detThickness6;

  G4Trd* solidCrystal6 = 
    new G4Trd("Crystal6",                        //its name
	      crystal_dxa6, crystal_dxb6,
	      crystal_dya6, crystal_dyb6,
	      crystal_dz);                      //its size
  G4LogicalVolume* logicCrystal6 =                         
    new G4LogicalVolume(solidCrystal6,            //its solid
                        CsI,                      //its material
                        "CsI Detector6");         //its name
  logicCrystal6->SetVisAttributes(crystalVisAtt);
  logicCrystal6->SetSensitiveDetector(m_MicroballScorer);
  //
  // Light Guide
  //       
  G4double lightGuide_px6 = 10*mm;
  G4double lightGuide_py6 = 10*mm;
  G4double lightGuide_pz6  = lgThickness6;      
  G4Box* solidLightGuide6 =    
    new G4Box("LightGuide6",                 //its name
              0.5*lightGuide_px6, 
	      0.5*lightGuide_py6,
	      0.5*lightGuide_pz6);           //its size
                
  G4LogicalVolume* logicLightGuide6 =                         
    new G4LogicalVolume(solidLightGuide6,    //its solid
                        glass,               //its material
                        "LightGuide6");      //its name
  logicLightGuide6->SetVisAttributes(lightGuideVisAtt);               
  
  
  for(int i = 1; i <= detNum6; i++)
    {
      G4RotationMatrix crystal_rotm = G4RotationMatrix();
      /*
      G4RotationMatrix abc6 = G4RotationMatrix();
      G4RotationMatrix def6 = G4RotationMatrix();
      G4ThreeVector a6 = G4ThreeVector(pow(sin(i*360*deg/detNum6),2), -sin(i*360*deg/detNum6)*cos(i*360*deg/detNum6), cos(i*360*deg/detNum6));
      G4ThreeVector b6 = G4ThreeVector(-sin(i*360*deg/detNum6)*cos(i*360*deg/detNum6), pow(cos(i*360*deg/detNum6),2), sin(i*360*deg/detNum6));
      G4ThreeVector c6 = G4ThreeVector(-cos(i*360*deg/detNum6), -sin(i*360*deg/detNum6), 0);
      abc6.setRows(a6,b6,c6);
      G4double k= (i-1)*360*deg/detNum6+0.5*360*deg/detNum6;
      G4ThreeVector d6 = G4ThreeVector(cos(k)+pow(cos(i*360*deg/detNum6),2)*(1-cos(k)),
				       cos(i*360*deg/detNum6)*sin(i*360*deg/detNum6)*(1-cos(k)),
				       sin(i*360*deg/detNum6)*sin(k));
      G4ThreeVector e6 = G4ThreeVector(cos(i*360*deg/detNum6)*sin(i*360*deg/detNum6)*(1-cos(k)),
				       cos(k)+pow(sin(i*360*deg/detNum6),2)*(1-cos(k)),
				       -cos(i*360*deg/detNum6)*sin(k));
      G4ThreeVector f6 = G4ThreeVector(-sin(i*360*deg/detNum6)*sin(k),
				       cos(i*360*deg/detNum6)*sin(k),
				       cos(k));
      def6.setRows(d6,e6,f6);
      crystal_rotm = def6*abc6;
      */
      G4ThreeVector z6 = G4ThreeVector(0,0,1);
      G4ThreeVector df6 = G4ThreeVector(cos((i-1)*360*deg/detNum6+0.5*360*deg/detNum6), 
					sin((i-1)*360*deg/detNum6+0.5*360*deg/detNum6), 
					0);
      G4ThreeVector unitAxis6 = z6.cross(df6);
      unitAxis6 = unitAxis6/unitAxis6.mag();
      G4double ux6 = unitAxis6.x();
      G4double uy6 = unitAxis6.y();
      G4double uz6 = unitAxis6.z();
      G4double angle6 = acos(z6.dot(df6)/(z6.mag()*df6.mag()));
      G4ThreeVector a6 = G4ThreeVector(cos(angle6)+pow(ux6,2)*(1-cos(angle6)), ux6*uy6*(1-cos(angle6))-uz6*sin(angle6), ux6*uz6*(1-cos(angle6))+uy6*sin(angle6));
      G4ThreeVector b6 = G4ThreeVector(uy6*ux6*(1-cos(angle6))+uz6*sin(angle6), cos(angle6)+pow(uy6,2)*(1-cos(angle6)), uy6*uz6*(1-cos(angle6))-ux6*sin(angle6));
      G4ThreeVector c6 = G4ThreeVector(uz6*ux6*(1-cos(angle6))-uy6*sin(angle6), uz6*uy6*(1-cos(angle6))+ux6*sin(angle6), cos(angle6)+pow(uz6,2)*(1-cos(angle6)));
      G4RotationMatrix abc6 = G4RotationMatrix();
      abc6.setRows(a6,b6,c6);
      crystal_rotm.rotateZ((i-1)*360*deg/detNum6+0.5*360*deg/detNum6);
      crystal_rotm=abc6*crystal_rotm;
      crystal_rotm.rotateZ(0*deg-4*360/14*deg);
      G4ThreeVector crystal_pos = G4ThreeVector((distanceFromTarget6+0.5*detThickness6)*cos((i-1)*360*deg/detNum6+0.5*360*deg/detNum6),
						(distanceFromTarget6+0.5*detThickness6)*sin((i-1)*360*deg/detNum6+0.5*360*deg/detNum6),
					        0)+delrin_pos6;
      crystal_pos.rotateZ(0*deg-4*360/14*deg);
      if (m_Flip == 1){
	crystal_pos.setX(-crystal_pos.x());
	crystal_rotm.rotateY(180*deg);
      }
      G4Transform3D crystal_trans = G4Transform3D(crystal_rotm, crystal_pos);
      G4RotationMatrix lg_rotm = G4RotationMatrix();
      G4ThreeVector df = G4ThreeVector(distanceFromTarget6*sin(theta6)*sin((i-1)*360*deg/detNum6/*+0.5*360*deg/detNum6*/), 
				       distanceFromTarget6*sin(theta6)*cos((i-1)*360*deg/detNum6/*+0.5*360*deg/detNum6*/), 
				       -distanceFromTarget6*cos(theta6));
      G4ThreeVector lg_Axis = df.cross(z6);
      lg_Axis = lg_Axis/lg_Axis.mag();
      lg_Axis.rotateZ(180*deg);
      G4double ulgx = lg_Axis.x();
      G4double ulgy = lg_Axis.y();
      G4double ulgz = lg_Axis.z();
      G4ThreeVector p = G4ThreeVector(cos(theta6)+pow(ulgx,2)*(1-cos(theta6)), ulgx*ulgy*(1-cos(theta6))-ulgz*sin(theta6), ulgx*ulgz*(1-cos(theta6))+ulgy*sin(theta6));
      G4ThreeVector q = G4ThreeVector(ulgy*ulgx*(1-cos(theta6))+ulgz*sin(theta6), cos(theta6)+pow(ulgy,2)*(1-cos(theta6)), ulgy*ulgz*(1-cos(theta6))-ulgx*sin(theta6));
      G4ThreeVector r = G4ThreeVector(ulgz*ulgx*(1-cos(theta6))-ulgy*sin(theta6), ulgz*ulgy*(1-cos(theta6))+ulgx*sin(theta6), cos(theta6)+pow(ulgz,2)*(1-cos(theta6)));
      G4RotationMatrix pqr = G4RotationMatrix();
      pqr.setRows(p,q,r);
      lg_rotm.rotateZ((i-1)*360*deg/detNum6);
      lg_rotm.rotateX(180*deg);
      lg_rotm=pqr*lg_rotm;
      G4ThreeVector lg_pos = (distanceFromTarget6+detThickness6+0.5*lgThickness6+.09*mm)/df.mag()*df; //the .09 mm is a fudge factor to prevent overlapping.
      lg_pos.rotateZ(180*deg);
      if (m_Flip == 0){
	lg_pos.rotateY(180*deg);
	lg_rotm.rotateY(180*deg);
      }
      G4Transform3D lg_trans = G4Transform3D(lg_rotm, lg_pos);
      G4RotationMatrix delrin_rotm = G4RotationMatrix();
      G4ThreeVector w = G4ThreeVector(sin(theta6)*sin((i-1)*360*deg/detNum6+.001*deg), sin(theta6)*cos((i-1)*360*deg/detNum6+.001*deg), cos(theta6));
      G4ThreeVector delrin_Axis = w.cross(z6);
      delrin_Axis = delrin_Axis/delrin_Axis.mag();
      G4double udx = delrin_Axis.x();
      G4double udy = delrin_Axis.y();
      G4double udz = delrin_Axis.z();
      G4double delAng = 0*deg;//90*deg+theta6;
      G4ThreeVector pdel = G4ThreeVector(cos(delAng)+pow(udx,2)*(1-cos(delAng)), udx*udy*(1-cos(delAng))-udz*sin(delAng), udx*udz*(1-cos(delAng))+udy*sin(delAng));
      G4ThreeVector qdel = G4ThreeVector(udy*udx*(1-cos(delAng))+udz*sin(delAng), cos(delAng)+pow(udy,2)*(1-cos(delAng)), udy*udz*(1-cos(delAng))-udx*sin(delAng));
      G4ThreeVector rdel = G4ThreeVector(udz*udx*(1-cos(delAng))-udy*sin(delAng), udz*udy*(1-cos(delAng))+udx*sin(delAng), cos(delAng)+pow(udz,2)*(1-cos(delAng)));
      G4RotationMatrix pqrdel = G4RotationMatrix();
      pqrdel.setRows(pdel,qdel,rdel);
      delrin_rotm.rotateZ((i-1)*360*deg/detNum6+.001*deg);
      delrin_rotm=pqrdel*delrin_rotm;
      delrin_rotm.rotateZ(180*deg);
      G4ThreeVector delrin_pos = (distanceFromTarget6+detThickness6+lgThickness6+delrinA6/2+.0904*mm)/df.mag()*df;  //the .0003 mm is a fudge factor to prevent overlapping
      delrin_pos.rotateZ(180*deg);
      delrin_pos.rotateY(180*deg);
      if (m_Flip == 1){
	delrin_pos.rotateY(180*deg);
	delrin_rotm.rotateY(180*deg);
      }
      G4Transform3D delrin_trans = G4Transform3D(delrin_rotm, delrin_pos);  
      bool placeCrystal = true;
      for (unsigned int i2=0; i2 < m_Array.size(); i2++){
	/*if(54 < m_Array[i2] && m_Array[i2] < 58){
	  if (55+57-m_Array[i2] == 54+i){
	    placeCrystal = false;
	  }
	}
	if(57 < m_Array[i2] && m_Array[i2] < 69){
	  if (58+68-m_Array[i2] == 54+i){
	    placeCrystal = false;
	  }
	}*/
	if (54 < m_Array[i2] && m_Array[i2] < 69){
	  if (m_Array[i2] == 54 +(i -detNum6/2)%detNum6){
	    placeCrystal = false;
	  }else if (m_Array[i2] == 54 +(i -detNum6/2)%detNum6 +detNum6){
	    placeCrystal = false;
	  }
	}
      }
      if (placeCrystal){
      if(crystalBool) {new G4PVPlacement(crystal_trans,       //rotation, position
                    logicCrystal6,           //its logical volume
                    "CsI Detector6",         //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
                    54+i,                     //copy number
                    checkOverlaps);          //overlaps checking
      }
      if(lgBool) {new G4PVPlacement(lg_trans,            //rotation, position
                    logicLightGuide6,        //its logical volume
                    "LightGuide6",           //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
		    54+i,                     //copy number
                    checkOverlaps);          //overlaps checking
      }
      if(delrinBool) {new G4PVPlacement(delrin_trans,          //rotation, position
                    logicDelrin6,               //its logical volume
                    "Delrin6",                  //its name
                    world,                 //its mother  volume
                    false,                      //no boolean operation
                    54+i,                        //copy number
                    checkOverlaps);             //overlaps checking
      }
      }
    }}
  
  if(m_Ring7){
  // Ring 7 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ lg pseudofixed
  G4int detNum7 = 12;
  G4double distanceFromTargetBack7 = 45.*mm;
  G4double theta7 = 111.5*deg;
  G4double halfTheta7 = 11.5*deg;//-0.1*deg; //fudge factor
  G4double lgThickness7 = 7.0*mm;
  G4double detThickness7 = 4.1*mm;
  G4double distanceFromTarget7 = distanceFromTargetBack7 - detThickness7;

  //G4double distance7 = distanceFromTarget7*sin(theta7); //this gives the distance from the beam axis.
  
  /*
  //
  // Delrin Ring (for deletion)
  //
  G4double delrin_rmina7 = distance7+detThickness7+lgThickness7, delrin_rmaxa7 = delrin_rmina7+5*mm;
  G4double delrin_hz7 = 5*mm; //distance7*tan(halfTheta7);
  G4double delrin_phimin7 = 0.*deg, delrin_phimax7 = 360.*deg;
  G4ThreeVector delrin_pos7 = G4ThreeVector(0,0,distanceFromTarget7*cos(theta7)); //G4ThreeVector(0,0,distance7+lgThickness7+detThickness7*sin(theta7));

  G4Tubs* solidDelrin7 =    
    new G4Tubs("Delrin7",                       //its name
	       delrin_rmina7, delrin_rmaxa7, 
	       delrin_hz7,
	       delrin_phimin7, delrin_phimax7); //its size
      
  G4LogicalVolume* logicDelrin7 =                         
    new G4LogicalVolume(solidDelrin7,           //its solid
                        delrin,                 //its material
                        "Delrin7");             //its name
  logicDelrin7->SetVisAttributes(delrinVisAtt);
  
  if(delrinBool) {new G4PVPlacement(0,          //no rotation
                    delrin_pos7,                //its position
                    logicDelrin7,               //its logical volume
                    "Delrin7",                  //its name
                    world,                 //its mother  volume
                    false,                      //no boolean operation
                    0,                          //copy number
                    checkOverlaps);             //overlaps checking
  }
  */
  //
  // Delrin Ring
  //
  G4double delrinA7 = 5*mm;
  G4double delrinB7 = 10*mm;
  G4double fdelrinHalfAngle7 = atan(delrinB7/2/(distanceFromTarget7+detThickness7+lgThickness7));
  G4double firs7 = pow(pow(distanceFromTarget7+detThickness7+lgThickness7,2)+pow(delrinB7/2,2),0.5)*sin(theta7-fdelrinHalfAngle7);
  G4double firb7 = pow(pow(distanceFromTarget7+detThickness7+lgThickness7,2)+pow(delrinB7/2,2),0.5)*sin(theta7+fdelrinHalfAngle7);
  G4double bdelrinHalfAngle7 = atan(delrinB7/2/(distanceFromTarget7+detThickness7+lgThickness7+delrinA7));
  G4double birs7 = pow(pow(distanceFromTarget7+detThickness7+lgThickness7+delrinA7,2)+pow(delrinB7/2,2),0.5)*sin(theta7-bdelrinHalfAngle7);
  G4double birb7 = pow(pow(distanceFromTarget7+detThickness7+lgThickness7+delrinA7,2)+pow(delrinB7/2,2),0.5)*sin(theta7+bdelrinHalfAngle7);

  G4double delrin_dz7 = delrinB7/2;
  G4double delrin_theta7 = 0;
  G4double delrin_phi7 = 0;
  G4double delrin_dyab7 = delrinA7/2;
  G4double delrin_dxa7 = firs7*tan(180*deg/detNum7);
  G4double delrin_dxb7 = birs7*tan(180*deg/detNum7);
  G4double delrin_dycd7 = delrinA7/2;
  G4double delrin_dxc7 = firb7*tan(180*deg/detNum7);
  G4double delrin_dxd7 = birb7*tan(180*deg/detNum7);
  G4double delrin_alpab7 = 0;
  G4double delrin_alpcd7 = delrin_alpab7;    
  
  G4ThreeVector delrin_pos7 = G4ThreeVector(0,0,distanceFromTarget7*cos(theta7));

  G4Trap* solidDelrin7 =    
    new G4Trap("Delrin7",                       //its name
	      delrin_dz7, 
	      delrin_theta7, delrin_phi7,
	      delrin_dyab7,
	      delrin_dxa7, delrin_dxb7,
	      delrin_alpab7,
	      delrin_dycd7,
	      delrin_dxc7, delrin_dxd7,
	      delrin_alpcd7);                   //its size
      
  G4LogicalVolume* logicDelrin7 =                         
    new G4LogicalVolume(solidDelrin7,           //its solid
                        delrin,                 //its material
                        "Delrin7");             //its name
  logicDelrin7->SetVisAttributes(delrinVisAtt);

  //
  // CsI Detector
  //
  G4double coneheight7 = 0.6*distanceFromTarget7;
  G4double flatheight7 = coneheight7 -0.01*mm;
  G4double coneangle7 = theta7;
  G4double crystal_rmina7 = -(coneheight7-flatheight7)*tan(coneangle7+halfTheta7);
  G4double crystal_rmaxa7 = -(coneheight7-flatheight7)*tan(coneangle7-halfTheta7);
  G4double crystal_rminb7 = -coneheight7*tan(coneangle7+halfTheta7);
  G4double crystal_rmaxb7 = -coneheight7*tan(coneangle7-halfTheta7);
  G4double crystal_hz7 = 0.5*flatheight7;//(crystal_rmaxb7-crystal_rmaxa7)/crystal_rmaxb7*coneheight7;
  G4double crystal_phimin7 = 0.*deg, crystal_phimax7 = 360.*deg/detNum7;
  G4RotationMatrix tip_rotm7 = G4RotationMatrix();
  G4ThreeVector tip_pos7 = G4ThreeVector(0,0,0.5*flatheight7+(coneheight7-flatheight7)); //last term is a fudge
  G4Transform3D tip_trans7 = G4Transform3D(tip_rotm7, tip_pos7);
  G4ThreeVector z7 = G4ThreeVector(0,0,1);
  G4ThreeVector d07 = G4ThreeVector(distanceFromTarget7*sin(coneangle7)*cos(0.5*360*deg/detNum7), 
				    distanceFromTarget7*sin(coneangle7)*sin(0.5*360*deg/detNum7), 
				    -distanceFromTarget7*cos(coneangle7));
  G4ThreeVector unitAxis7 = z7.cross(d07);
  unitAxis7 = unitAxis7/unitAxis7.mag();
  G4double ux7 = unitAxis7.x();
  G4double uy7 = unitAxis7.y();
  G4double uz7 = unitAxis7.z();
  G4ThreeVector top7 = G4ThreeVector(cos(-coneangle7)+pow(ux7,2)*(1-cos(-coneangle7)), ux7*uy7*(1-cos(-coneangle7))-uz7*sin(-coneangle7), ux7*uz7*(1-cos(-coneangle7))+uy7*sin(-coneangle7));
  G4ThreeVector mid7 = G4ThreeVector(uy7*ux7*(1-cos(-coneangle7))+uz7*sin(-coneangle7), cos(-coneangle7)+pow(uy7,2)*(1-cos(-coneangle7)), uy7*uz7*(1-cos(-coneangle7))-ux7*sin(-coneangle7));
  G4ThreeVector bot7 = G4ThreeVector(uz7*ux7*(1-cos(-coneangle7))-uy7*sin(-coneangle7), uz7*uy7*(1-cos(-coneangle7))+ux7*sin(-coneangle7), cos(-coneangle7)+pow(uz7,2)*(1-cos(-coneangle7)));
  G4RotationMatrix firstBox_rotm7 = G4RotationMatrix();
  firstBox_rotm7.setRows(top7,mid7,bot7);
  G4ThreeVector firstBox_pos7 = G4ThreeVector(0,0,0);
  G4Transform3D firstBox_trans7 = G4Transform3D(firstBox_rotm7, firstBox_pos7);
  G4RotationMatrix secondBox_rotm7 = G4RotationMatrix();
  secondBox_rotm7.setRows(top7,mid7,bot7);
  G4ThreeVector secondBox_pos7 = (3*distanceFromTarget7+detThickness7)/distanceFromTarget7*d07;
  G4Transform3D secondBox_trans7 = G4Transform3D(secondBox_rotm7, secondBox_pos7);
  
  G4Orb* solidTip7 =
    new G4Orb("tip7",
	      .0001*mm);
  G4Box* solidSubtractionBox7 = 
    new G4Box("SubtractionBox7",                  //its name
	      distanceFromTarget7,
	      distanceFromTarget7,
	      distanceFromTarget7);               //its size
  G4Box* solidSecondSubtractionBox7 = 
    new G4Box("SecondSubtractionBox7",            //its name
	      2*distanceFromTarget7,
	      2*distanceFromTarget7,
	      2*distanceFromTarget7);             //its size
  G4Cons* solidCone7 =    
    new G4Cons("Cone7",                           //its name
	       crystal_rmina7, crystal_rmaxa7, 
	       crystal_rminb7, crystal_rmaxb7, 
	       crystal_hz7,
	       crystal_phimin7, crystal_phimax7); //its size
  G4UnionSolid* solidTippedCone7 =
    new G4UnionSolid("TippedCone7",
		     solidTip7,
		     solidCone7,
		     tip_trans7);
  G4SubtractionSolid* solidIntermediate7 =
    new G4SubtractionSolid("Intermediate7",
			   solidTippedCone7,
			   solidSubtractionBox7,
			   firstBox_trans7);
  G4SubtractionSolid* solidCrystal7 =
    new G4SubtractionSolid("Crystal7",
			   solidIntermediate7,
			   solidSecondSubtractionBox7,
			   secondBox_trans7);
  
  G4LogicalVolume* logicCrystal7 =                         
    new G4LogicalVolume(solidCrystal7,            //its solid
                        CsI,                      //its material
                        "CsI Detector7");         //its name
  logicCrystal7->SetVisAttributes(crystalVisAtt);
  logicCrystal7->SetSensitiveDetector(m_MicroballScorer);
  //
  // Light Guide
  //       
  G4double lightGuide_px7 = 10*mm;
  G4double lightGuide_py7 = 10*mm;
  G4double lightGuide_pz7  = lgThickness7;      
  G4Box* solidLightGuide7 =    
    new G4Box("LightGuide7",                 //its name
              0.5*lightGuide_px7, 
	      0.5*lightGuide_py7,
	      0.5*lightGuide_pz7);           //its size
                
  G4LogicalVolume* logicLightGuide7 =                         
    new G4LogicalVolume(solidLightGuide7,    //its solid
                        glass,               //its material
                        "LightGuide7");      //its name
  logicLightGuide7->SetVisAttributes(lightGuideVisAtt);               
  
  
  for(int i = 1; i <= detNum7; i++)
    {
      G4RotationMatrix crystal_rotm = G4RotationMatrix();
      G4ThreeVector d0 = G4ThreeVector(distanceFromTarget7*sin(coneangle7)*cos(0.5*360*deg/detNum7), 
				       distanceFromTarget7*sin(coneangle7)*sin(0.5*360*deg/detNum7), 
				       distanceFromTarget7*cos(coneangle7));
      G4ThreeVector df = G4ThreeVector(distanceFromTarget7*sin(theta7)*sin((i-1)*360*deg/detNum7+.001*deg/*+0.5*360*deg/detNum7*/), 
				       distanceFromTarget7*sin(theta7)*cos((i-1)*360*deg/detNum7+.001*deg/*+0.5*360*deg/detNum7*/), 
				       -distanceFromTarget7*cos(theta7));
      df.setX(-df.x());
      G4ThreeVector axis = d0.cross(df);
      G4double ax = axis.x();
      G4double ay = axis.y();
      G4double az = axis.z();
      G4double ux = ax/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double uy = ay/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double uz = az/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double j = acos(d0.dot(df)/(d0.mag()*df.mag()));
      G4ThreeVector a = G4ThreeVector(cos(j)+pow(ux,2)*(1-cos(j)), ux*uy*(1-cos(j))-uz*sin(j), ux*uz*(1-cos(j))+uy*sin(j));
      G4ThreeVector b = G4ThreeVector(uy*ux*(1-cos(j))+uz*sin(j), cos(j)+pow(uy,2)*(1-cos(j)), uy*uz*(1-cos(j))-ux*sin(j));
      G4ThreeVector c = G4ThreeVector(uz*ux*(1-cos(j))-uy*sin(j), uz*uy*(1-cos(j))+ux*sin(j), cos(j)+pow(uz,2)*(1-cos(j)));
      G4RotationMatrix abc = G4RotationMatrix();
      abc.setRows(a,b,c);
      G4ThreeVector unitdf = df/df.mag();
      G4double udfx = unitdf.x();
      G4double udfy = unitdf.y();
      G4double udfz = unitdf.z();
      G4double k = 180.*deg;
      G4ThreeVector d = G4ThreeVector(cos(k)+pow(udfx,2)*(1-cos(k)), udfx*udfy*(1-cos(k))-udfz*sin(k), udfx*udfz*(1-cos(k))+udfy*sin(k));
      G4ThreeVector e = G4ThreeVector(udfy*udfx*(1-cos(k))+udfz*sin(k), cos(k)+pow(udfy,2)*(1-cos(k)), udfy*udfz*(1-cos(k))-udfx*sin(k));
      G4ThreeVector f = G4ThreeVector(udfz*udfx*(1-cos(k))-udfy*sin(k), udfz*udfy*(1-cos(k))+udfx*sin(k), cos(k)+pow(udfz,2)*(1-cos(k)));
      G4RotationMatrix def = G4RotationMatrix();
      def.setRows(d,e,f);
      crystal_rotm=def*abc;
      if (m_Flip == 1){
	crystal_rotm.rotateY(180*deg);
      }
      crystal_rotm.rotateZ(180*deg);
      G4ThreeVector crystal_pos = G4ThreeVector(0,0,0);
      G4Transform3D crystal_trans = G4Transform3D(crystal_rotm, crystal_pos);
      df.setX(-df.x());
      G4RotationMatrix lg_rotm = G4RotationMatrix();
      G4ThreeVector lg_Axis = df.cross(z7);
      lg_Axis = lg_Axis/lg_Axis.mag();
      lg_Axis.rotateZ(180*deg);
      G4double ulgx = lg_Axis.x();
      G4double ulgy = lg_Axis.y();
      G4double ulgz = lg_Axis.z();
      G4double lg_ang = theta7;
      G4ThreeVector p = G4ThreeVector(cos(lg_ang)+pow(ulgx,2)*(1-cos(lg_ang)), ulgx*ulgy*(1-cos(lg_ang))-ulgz*sin(lg_ang), ulgx*ulgz*(1-cos(lg_ang))+ulgy*sin(lg_ang));
      G4ThreeVector q = G4ThreeVector(ulgy*ulgx*(1-cos(lg_ang))+ulgz*sin(lg_ang), cos(lg_ang)+pow(ulgy,2)*(1-cos(lg_ang)), ulgy*ulgz*(1-cos(lg_ang))-ulgx*sin(lg_ang));
      G4ThreeVector r = G4ThreeVector(ulgz*ulgx*(1-cos(lg_ang))-ulgy*sin(lg_ang), ulgz*ulgy*(1-cos(lg_ang))+ulgx*sin(lg_ang), cos(lg_ang)+pow(ulgz,2)*(1-cos(lg_ang)));
      G4RotationMatrix pqr = G4RotationMatrix();
      pqr.setRows(p,q,r);
      lg_rotm.rotateZ((i-1)*360*deg/detNum7);
      lg_rotm.rotateX(180*deg);
      lg_rotm=pqr*lg_rotm;
      G4ThreeVector lg_pos = (distanceFromTarget7+detThickness7+0.5*lgThickness7+.0003*mm)/df.mag()*df;
      lg_pos.rotateZ(180*deg);
      if (m_Flip == 0){
	lg_pos.rotateY(180*deg);
	lg_rotm.rotateY(180*deg);
      }
      G4Transform3D lg_trans = G4Transform3D(lg_rotm, lg_pos);
      G4RotationMatrix delrin_rotm = G4RotationMatrix();
      G4ThreeVector w = G4ThreeVector(sin(theta7)*sin((i-1)*360*deg/detNum7+.001*deg), sin(theta7)*cos((i-1)*360*deg/detNum7+.001*deg), cos(theta7));
      G4ThreeVector delrin_Axis = w.cross(z7);
      delrin_Axis.rotateX(180*deg);
      delrin_Axis = delrin_Axis/delrin_Axis.mag();
      G4double udx = delrin_Axis.x();
      G4double udy = delrin_Axis.y();
      G4double udz = delrin_Axis.z();
      G4double delAng = -90*deg+theta7;
      G4ThreeVector pdel = G4ThreeVector(cos(delAng)+pow(udx,2)*(1-cos(delAng)), udx*udy*(1-cos(delAng))-udz*sin(delAng), udx*udz*(1-cos(delAng))+udy*sin(delAng));
      G4ThreeVector qdel = G4ThreeVector(udy*udx*(1-cos(delAng))+udz*sin(delAng), cos(delAng)+pow(udy,2)*(1-cos(delAng)), udy*udz*(1-cos(delAng))-udx*sin(delAng));
      G4ThreeVector rdel = G4ThreeVector(udz*udx*(1-cos(delAng))-udy*sin(delAng), udz*udy*(1-cos(delAng))+udx*sin(delAng), cos(delAng)+pow(udz,2)*(1-cos(delAng)));
      G4RotationMatrix pqrdel = G4RotationMatrix();
      pqrdel.setRows(pdel,qdel,rdel);
      delrin_rotm.rotateZ((-i+1)*360*deg/detNum7+.001*deg);
      delrin_rotm.rotateX(180*deg);
      delrin_rotm=pqrdel*delrin_rotm;
      delrin_rotm.rotateZ(0*deg);
      G4ThreeVector delrin_pos = (distanceFromTarget7+detThickness7+lgThickness7+delrinA7/2+.0005*mm)/df.mag()*df;  //the .0003 mm is a fudge factor to prevent overlapping
      delrin_pos.rotateZ(180*deg);
      delrin_pos.rotateY(180*deg);
      if (m_Flip == 1){
	delrin_pos.rotateY(180*deg);
	delrin_rotm.rotateY(180*deg);
      }
      G4Transform3D delrin_trans = G4Transform3D(delrin_rotm, delrin_pos);      
      bool placeCrystal = true;
      for (unsigned int i2=0; i2 < m_Array.size(); i2++){
	if (68 < m_Array[i2] && m_Array[i2] < 81){ 
	  /*if (m_Array[i2] == 69 && 68+i == 80){
	    placeCrystal = false;
	  }
	  else if(68+m_Array[i2]%69 == 68+i){
	    placeCrystal = false;
	  }
	  if (m_Array[i2] == 68+i){
	    placeDel = false;
	  }*/
	  if (m_Array[i2] == 68 +(i -detNum7/2)%detNum7){
	    placeCrystal = false;
	  }else if (m_Array[i2] == 68 +(i -detNum7/2)%detNum7 +detNum7){
	    placeCrystal = false;
	  }
	}
      }
      if (placeCrystal){
      if(crystalBool) {new G4PVPlacement(crystal_trans,       //rotation, position
                    logicCrystal7,           //its logical volume
                    "CsI Detector7",         //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
                    68+i,                     //copy number
                    checkOverlaps);          //overlaps checking
      }
      }
      if(placeCrystal){
      if(lgBool) {new G4PVPlacement(lg_trans,            //rotation, position
                    logicLightGuide7,        //its logical volume
                    "LightGuide7",           //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
		    68+i,                     //copy number
                    checkOverlaps);          //overlaps checking
      }
      if(delrinBool) {new G4PVPlacement(delrin_trans,          //rotation, position
                    logicDelrin7,               //its logical volume
                    "Delrin7",                  //its name
                    world,                 //its mother  volume
                    false,                      //no boolean operation
                    68+i,                        //copy number
                    checkOverlaps);             //overlaps checking
      }
      }
    }}
  

  if(m_Ring8) {
  // Ring 8 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  G4int detNum8 = 10;
  G4double distanceFromTargetBack8 = 47*mm; //Its really 47 mm from the target, but at 45.9 we avoid intersection the detectors in ring 9
  G4double theta8 = 135.0*deg;
  G4double halfTheta8 = 12.0*deg;// -0.1*deg;  //fudge factor
  G4double lgThickness8 = 7.5*mm;
  G4double detThickness8 = 3.6*mm;
  G4double distanceFromTarget8 = distanceFromTargetBack8 - detThickness8;

  //G4double distance8 = distanceFromTarget8*sin(theta8); //this gives the distance from the beam axis.
  /*
  //
  // Delrin Ring (for deletion)
  //
  G4double delrin_rmina8 = distance8+detThickness8+lgThickness8, delrin_rmaxa8 = delrin_rmina8+5*mm;
  G4double delrin_hz8 = 5*mm; //distance8*tan(halfTheta8);
  G4double delrin_phimin8 = 0.*deg, delrin_phimax8 = 360.*deg;
  G4ThreeVector delrin_pos8 = G4ThreeVector(0,0,distanceFromTarget8*cos(theta8)); //G4ThreeVector(0,0,distance8+lgThickness8+detThickness8*sin(theta8));

  G4Tubs* solidDelrin8 =    
    new G4Tubs("Delrin8",                       //its name
	       delrin_rmina8, delrin_rmaxa8, 
	       delrin_hz8,
	       delrin_phimin8, delrin_phimax8); //its size
      
  G4LogicalVolume* logicDelrin8 =                         
    new G4LogicalVolume(solidDelrin8,           //its solid
                        delrin,                 //its material
                        "Delrin8");             //its name
  logicDelrin8->SetVisAttributes(delrinVisAtt);
  
  if(delrinBool) {new G4PVPlacement(0,          //no rotation
                    delrin_pos8,                //its position
                    logicDelrin8,               //its logical volume
                    "Delrin8",                  //its name
                    world,                 //its mother  volume
                    false,                      //no boolean operation
                    0,                          //copy number
                    checkOverlaps);             //overlaps checking
  }   
  */
  //
  // Delrin Ring
  //
  G4double delrinA8 = 5*mm;
  G4double delrinB8 = 10*mm;
  G4double fdelrinHalfAngle8 = atan(delrinB8/2/(distanceFromTarget8+detThickness8+lgThickness8));
  G4double firs8 = pow(pow(distanceFromTarget8+detThickness8+lgThickness8,2)+pow(delrinB8/2,2),0.5)*sin(theta8-fdelrinHalfAngle8);
  G4double firb8 = pow(pow(distanceFromTarget8+detThickness8+lgThickness8,2)+pow(delrinB8/2,2),0.5)*sin(theta8+fdelrinHalfAngle8);
  G4double bdelrinHalfAngle8 = atan(delrinB8/2/(distanceFromTarget8+detThickness8+lgThickness8+delrinA8));
  G4double birs8 = pow(pow(distanceFromTarget8+detThickness8+lgThickness8+delrinA8,2)+pow(delrinB8/2,2),0.5)*sin(theta8-bdelrinHalfAngle8);
  G4double birb8 = pow(pow(distanceFromTarget8+detThickness8+lgThickness8+delrinA8,2)+pow(delrinB8/2,2),0.5)*sin(theta8+bdelrinHalfAngle8);

  G4double delrin_dz8 = delrinB8/2;
  G4double delrin_theta8 = 0;
  G4double delrin_phi8 = 0;
  G4double delrin_dyab8 = delrinA8/2;
  G4double delrin_dxa8 = firs8*tan(180*deg/detNum8);
  G4double delrin_dxb8 = birs8*tan(180*deg/detNum8);
  G4double delrin_dycd8 = delrinA8/2;
  G4double delrin_dxc8 = firb8*tan(180*deg/detNum8);
  G4double delrin_dxd8 = birb8*tan(180*deg/detNum8);
  G4double delrin_alpab8 = 0;
  G4double delrin_alpcd8 = delrin_alpab8;    
  
  G4ThreeVector delrin_pos8 = G4ThreeVector(0,0,distanceFromTarget8*cos(theta8));

  G4Trap* solidDelrin8 =    
    new G4Trap("Delrin8",                       //its name
	      delrin_dz8, 
	      delrin_theta8, delrin_phi8,
	      delrin_dyab8,
	      delrin_dxa8, delrin_dxb8,
	      delrin_alpab8,
	      delrin_dycd8,
	      delrin_dxc8, delrin_dxd8,
	      delrin_alpcd8);                   //its size
      
  G4LogicalVolume* logicDelrin8 =                         
    new G4LogicalVolume(solidDelrin8,           //its solid
                        delrin,                 //its material
                        "Delrin8");             //its name
  logicDelrin8->SetVisAttributes(delrinVisAtt);
    
  //
  // CsI Detector
  //
  G4double coneheight8 = 1.7*distanceFromTarget8;
  G4double flatheight8 = coneheight8 -0.01*mm;
  G4double coneangle8 = theta8;
  G4double crystal_rmina8 = -(coneheight8-flatheight8)*tan(coneangle8+halfTheta8);
  G4double crystal_rmaxa8 = -(coneheight8-flatheight8)*tan(coneangle8-halfTheta8);
  G4double crystal_rminb8 = -coneheight8*tan(coneangle8+halfTheta8);
  G4double crystal_rmaxb8 = -coneheight8*tan(coneangle8-halfTheta8);
  G4double crystal_hz8 = 0.5*flatheight8;//(crystal_rmaxb8-crystal_rmaxa8)/crystal_rmaxb8*coneheight8;
  G4double crystal_phimin8 = 0.*deg, crystal_phimax8 = 360.*deg/detNum8;
  G4RotationMatrix tip_rotm8 = G4RotationMatrix();
  G4ThreeVector tip_pos8 = G4ThreeVector(0,0,0.5*flatheight8+(coneheight8-flatheight8)); //last term is a fudge
  G4Transform3D tip_trans8 = G4Transform3D(tip_rotm8, tip_pos8);
  G4ThreeVector z8 = G4ThreeVector(0,0,1);
  G4ThreeVector d08 = G4ThreeVector(distanceFromTarget8*sin(coneangle8)*cos(0.5*360*deg/detNum8), 
				    distanceFromTarget8*sin(coneangle8)*sin(0.5*360*deg/detNum8), 
				    -distanceFromTarget8*cos(coneangle8));
  G4ThreeVector unitAxis8 = z8.cross(d08);
  unitAxis8 = unitAxis8/unitAxis8.mag();
  G4double ux8 = unitAxis8.x();
  G4double uy8 = unitAxis8.y();
  G4double uz8 = unitAxis8.z();
  G4ThreeVector top8 = G4ThreeVector(cos(-coneangle8)+pow(ux8,2)*(1-cos(-coneangle8)), ux8*uy8*(1-cos(-coneangle8))-uz8*sin(-coneangle8), ux8*uz8*(1-cos(-coneangle8))+uy8*sin(-coneangle8));
  G4ThreeVector mid8 = G4ThreeVector(uy8*ux8*(1-cos(-coneangle8))+uz8*sin(-coneangle8), cos(-coneangle8)+pow(uy8,2)*(1-cos(-coneangle8)), uy8*uz8*(1-cos(-coneangle8))-ux8*sin(-coneangle8));
  G4ThreeVector bot8 = G4ThreeVector(uz8*ux8*(1-cos(-coneangle8))-uy8*sin(-coneangle8), uz8*uy8*(1-cos(-coneangle8))+ux8*sin(-coneangle8), cos(-coneangle8)+pow(uz8,2)*(1-cos(-coneangle8)));
  G4RotationMatrix firstBox_rotm8 = G4RotationMatrix();
  firstBox_rotm8.setRows(top8,mid8,bot8);
  G4ThreeVector firstBox_pos8 = G4ThreeVector(0,0,0);
  G4Transform3D firstBox_trans8 = G4Transform3D(firstBox_rotm8, firstBox_pos8);
  G4RotationMatrix secondBox_rotm8 = G4RotationMatrix();
  secondBox_rotm8.setRows(top8,mid8,bot8);
  G4ThreeVector secondBox_pos8 = (2*distanceFromTarget8+detThickness8)/d08.mag()*d08;
  G4Transform3D secondBox_trans8 = G4Transform3D(secondBox_rotm8, secondBox_pos8);
  
  G4Orb* solidTip8 =
    new G4Orb("tip8",
	      .0001*mm);
  G4Box* solidSubtractionBox8 = 
    new G4Box("SubtractionBox8",                  //its name
	      distanceFromTarget8,
	      distanceFromTarget8,
	      distanceFromTarget8);               //its size
  G4Cons* solidCone8 =    
    new G4Cons("Cone8",                           //its name
	       crystal_rmina8, crystal_rmaxa8, 
	       crystal_rminb8, crystal_rmaxb8, 
	       crystal_hz8,
	       crystal_phimin8, crystal_phimax8); //its size
  G4UnionSolid* solidTippedCone8 =
    new G4UnionSolid("TippedCone8",
		     solidTip8,
		     solidCone8,
		     tip_trans8);
  G4SubtractionSolid* solidIntermediate8 =
    new G4SubtractionSolid("Intermediate8",
			   solidTippedCone8,
			   solidSubtractionBox8,
			   firstBox_trans8);
  G4SubtractionSolid* solidCrystal8 =
    new G4SubtractionSolid("Crystal8",
			   solidIntermediate8,
			   solidSubtractionBox8,
			   secondBox_trans8);
  
  G4LogicalVolume* logicCrystal8 =                         
    new G4LogicalVolume(solidCrystal8,            //its solid
                        CsI,                      //its material
                        "CsI Detector8");         //its name
  logicCrystal8->SetVisAttributes(crystalVisAtt);
  logicCrystal8->SetSensitiveDetector(m_MicroballScorer);
  //
  // Light Guide
  //       
  G4double lightGuide_px8 = 10*mm;
  G4double lightGuide_py8 = 10*mm;
  G4double lightGuide_pz8  = lgThickness8;      
  G4Box* solidLightGuide8 =    
    new G4Box("LightGuide8",                 //its name
              0.5*lightGuide_px8, 
	      0.5*lightGuide_py8,
	      0.5*lightGuide_pz8);           //its size
                
  G4LogicalVolume* logicLightGuide8 =                         
    new G4LogicalVolume(solidLightGuide8,    //its solid
                        glass,               //its material
                        "LightGuide8");      //its name
  logicLightGuide8->SetVisAttributes(lightGuideVisAtt);               
  
  
  for(int i = 1; i <= detNum8; i++)
    {
      G4RotationMatrix crystal_rotm = G4RotationMatrix();
      G4ThreeVector d0 = G4ThreeVector(distanceFromTarget8*sin(coneangle8)*cos(0.5*360*deg/detNum8), 
				       distanceFromTarget8*sin(coneangle8)*sin(0.5*360*deg/detNum8), 
				       distanceFromTarget8*cos(coneangle8));
      G4ThreeVector df = G4ThreeVector(distanceFromTarget8*sin(theta8)*sin((i-1)*360*deg/detNum8+.001*deg/*+0.5*360*deg/detNum8*/), 
				       distanceFromTarget8*sin(theta8)*cos((i-1)*360*deg/detNum8+.001*deg/*+0.5*360*deg/detNum8*/), 
				       -distanceFromTarget8*cos(theta8));
      df.setX(-df.x());
      G4ThreeVector axis = d0.cross(df);
      G4double ax = axis.x();
      G4double ay = axis.y();
      G4double az = axis.z();
      G4double ux = ax/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double uy = ay/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double uz = az/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double j = acos(d0.dot(df)/(d0.mag()*df.mag()));
      G4ThreeVector a = G4ThreeVector(cos(j)+pow(ux,2)*(1-cos(j)), ux*uy*(1-cos(j))-uz*sin(j), ux*uz*(1-cos(j))+uy*sin(j));
      G4ThreeVector b = G4ThreeVector(uy*ux*(1-cos(j))+uz*sin(j), cos(j)+pow(uy,2)*(1-cos(j)), uy*uz*(1-cos(j))-ux*sin(j));
      G4ThreeVector c = G4ThreeVector(uz*ux*(1-cos(j))-uy*sin(j), uz*uy*(1-cos(j))+ux*sin(j), cos(j)+pow(uz,2)*(1-cos(j)));
      G4RotationMatrix abc = G4RotationMatrix();
      abc.setRows(a,b,c);
      G4ThreeVector unitdf = df/df.mag();
      G4double udfx = unitdf.x();
      G4double udfy = unitdf.y();
      G4double udfz = unitdf.z();
      G4double k = 180.*deg;
      G4ThreeVector d = G4ThreeVector(cos(k)+pow(udfx,2)*(1-cos(k)), udfx*udfy*(1-cos(k))-udfz*sin(k), udfx*udfz*(1-cos(k))+udfy*sin(k));
      G4ThreeVector e = G4ThreeVector(udfy*udfx*(1-cos(k))+udfz*sin(k), cos(k)+pow(udfy,2)*(1-cos(k)), udfy*udfz*(1-cos(k))-udfx*sin(k));
      G4ThreeVector f = G4ThreeVector(udfz*udfx*(1-cos(k))-udfy*sin(k), udfz*udfy*(1-cos(k))+udfx*sin(k), cos(k)+pow(udfz,2)*(1-cos(k)));
      G4RotationMatrix def = G4RotationMatrix();
      def.setRows(d,e,f);
      crystal_rotm=def*abc;
      if (m_Flip == 1){
	crystal_rotm.rotateY(180*deg);
      }
      crystal_rotm.rotateZ(180*deg);
      G4ThreeVector crystal_pos = G4ThreeVector(0,0,0);
      G4Transform3D crystal_trans = G4Transform3D(crystal_rotm, crystal_pos);
      df.setX(-df.x());
      G4RotationMatrix lg_rotm = G4RotationMatrix();
      G4ThreeVector lg_Axis = df.cross(z8);
      lg_Axis = lg_Axis/lg_Axis.mag();
      lg_Axis.rotateZ(180*deg);
      G4double ulgx = lg_Axis.x();
      G4double ulgy = lg_Axis.y();
      G4double ulgz = lg_Axis.z();
      G4ThreeVector p = G4ThreeVector(cos(theta8)+pow(ulgx,2)*(1-cos(theta8)), ulgx*ulgy*(1-cos(theta8))-ulgz*sin(theta8), ulgx*ulgz*(1-cos(theta8))+ulgy*sin(theta8));
      G4ThreeVector q = G4ThreeVector(ulgy*ulgx*(1-cos(theta8))+ulgz*sin(theta8), cos(theta8)+pow(ulgy,2)*(1-cos(theta8)), ulgy*ulgz*(1-cos(theta8))-ulgx*sin(theta8));
      G4ThreeVector r = G4ThreeVector(ulgz*ulgx*(1-cos(theta8))-ulgy*sin(theta8), ulgz*ulgy*(1-cos(theta8))+ulgx*sin(theta8), cos(theta8)+pow(ulgz,2)*(1-cos(theta8)));
      G4RotationMatrix pqr = G4RotationMatrix();
      pqr.setRows(p,q,r);
      lg_rotm.rotateZ((i-1)*360*deg/detNum8);
      lg_rotm.rotateX(180*deg);
      lg_rotm=pqr*lg_rotm;
      G4ThreeVector lg_pos = (distanceFromTarget8+detThickness8+0.5*lgThickness8+.0003*mm)/df.mag()*df;
      lg_pos.rotateZ(180*deg);
      if (m_Flip == 0){
	lg_pos.rotateY(180*deg);
	lg_rotm.rotateY(180*deg);
      }
      G4Transform3D lg_trans = G4Transform3D(lg_rotm, lg_pos);
      G4RotationMatrix delrin_rotm = G4RotationMatrix();
      G4ThreeVector w = G4ThreeVector(sin(theta8)*sin((i-1)*360*deg/detNum8+.001*deg), sin(theta8)*cos((i-1)*360*deg/detNum8+.001*deg), cos(theta8));
      G4ThreeVector delrin_Axis = w.cross(z8);
      delrin_Axis.rotateX(180*deg);
      delrin_Axis = delrin_Axis/delrin_Axis.mag();
      G4double udx = delrin_Axis.x();
      G4double udy = delrin_Axis.y();
      G4double udz = delrin_Axis.z();
      G4double delAng = -90*deg+theta8;
      G4ThreeVector pdel = G4ThreeVector(cos(delAng)+pow(udx,2)*(1-cos(delAng)), udx*udy*(1-cos(delAng))-udz*sin(delAng), udx*udz*(1-cos(delAng))+udy*sin(delAng));
      G4ThreeVector qdel = G4ThreeVector(udy*udx*(1-cos(delAng))+udz*sin(delAng), cos(delAng)+pow(udy,2)*(1-cos(delAng)), udy*udz*(1-cos(delAng))-udx*sin(delAng));
      G4ThreeVector rdel = G4ThreeVector(udz*udx*(1-cos(delAng))-udy*sin(delAng), udz*udy*(1-cos(delAng))+udx*sin(delAng), cos(delAng)+pow(udz,2)*(1-cos(delAng)));
      G4RotationMatrix pqrdel = G4RotationMatrix();
      pqrdel.setRows(pdel,qdel,rdel);
      delrin_rotm.rotateZ((-i+1)*360*deg/detNum8+.001*deg);
      delrin_rotm.rotateX(180*deg);
      delrin_rotm=pqrdel*delrin_rotm;
      delrin_rotm.rotateZ(0*deg);
      G4ThreeVector delrin_pos = (distanceFromTarget8+detThickness8+lgThickness8+delrinA8/2+.0005*mm)/df.mag()*df;  //the .0003 mm is a fudge factor to prevent overlapping
      delrin_pos.rotateZ(180*deg);
      delrin_pos.rotateY(180*deg);
      if (m_Flip == 1){
	delrin_pos.rotateY(180*deg);
	delrin_rotm.rotateY(180*deg);
      }
      G4Transform3D delrin_trans = G4Transform3D(delrin_rotm, delrin_pos);     
      bool placeCrystal = true;
      for (unsigned int i2=0; i2 < m_Array.size(); i2++){
	if (80 < m_Array[i2] && m_Array[i2] < 91){ 
	  if (m_Array[i2] == 80 +(i -detNum8/2)%detNum8){
	    placeCrystal = false;
	  }else if (m_Array[i2] == 80 +(i -detNum8/2)%detNum8 +detNum8){
	    placeCrystal = false;
	  }
	}
      }
      if (placeCrystal){ 
      if(crystalBool) {new G4PVPlacement(crystal_trans,       //rotation, position
                    logicCrystal8,           //its logical volume
                    "CsI Detector8",         //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
                    80+i,                     //copy number
                    checkOverlaps);          //overlaps checking
      }
      }
      if (placeCrystal){
      if(lgBool) {new G4PVPlacement(lg_trans,            //rotation, position
                    logicLightGuide8,        //its logical volume
                    "LightGuide8",           //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
		    80+i,                     //copy number
		    checkOverlaps);          //overlaps checking
      }
      if(delrinBool) {new G4PVPlacement(delrin_trans,          //rotation, position
                    logicDelrin8,               //its logical volume
                    "Delrin8",                  //its name
                    world,                 //its mother  volume
                    false,                      //no boolean operation
                    80+i,                        //copy number
                    checkOverlaps);             //overlaps checking
      }
      }
    }}
  

  if(m_Ring9){
  // Ring 9 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  G4int detNum9 = 6;
  G4double distanceFromTargetBack9 = 50.*mm;
  G4double theta9 = 159.0*deg;
  G4double halfTheta9 = 12.0*deg;//-0.1*deg;  //fudge factor
  G4double lgThickness9 = 8.0*mm;
  G4double detThickness9 = 3.5*mm;
  G4double distanceFromTarget9 = distanceFromTargetBack9 - detThickness9;

  //G4double distance9 = distanceFromTarget9*sin(theta9); //this gives the distance from the beam axis.
  
  /*
  //
  // Delrin Ring (for deletion)
  //
  G4double delrin_rmina9 = distance9+detThickness9+lgThickness9, delrin_rmaxa9 = delrin_rmina9+5*mm;
  G4double delrin_hz9 = 5*mm; //distance9*tan(halfTheta9);
  G4double delrin_phimin9 = 0.*deg, delrin_phimax9 = 360.*deg;
  G4ThreeVector delrin_pos9 = G4ThreeVector(0,0,distanceFromTarget9*cos(theta9)); //G4ThreeVector(0,0,distance9+lgThickness9+detThickness9*sin(theta9));

  G4Tubs* solidDelrin9 =    
    new G4Tubs("Delrin9",                       //its name
	       delrin_rmina9, delrin_rmaxa9, 
	       delrin_hz9,
	       delrin_phimin9, delrin_phimax9); //its size
      
  G4LogicalVolume* logicDelrin9 =                         
    new G4LogicalVolume(solidDelrin9,           //its solid
                        delrin,                 //its material
                        "Delrin9");             //its name
  logicDelrin9->SetVisAttributes(delrinVisAtt);
  
  if(delrinBool) {new G4PVPlacement(0,          //no rotation
                    delrin_pos9,                //its position
                    logicDelrin9,               //its logical volume
                    "Delrin9",                  //its name
                    world,                 //its mother  volume
                    false,                      //no boolean operation
                    0,                          //copy number
                    checkOverlaps);             //overlaps checking
  }         
*/

  //
  // Delrin Ring
  //
  G4double delrinA9 = 5*mm;
  G4double delrinB9 = 10*mm;
  G4double fdelrinHalfAngle9 = atan(delrinB9/2/(distanceFromTarget9+detThickness9+lgThickness9));
  G4double firs9 = pow(pow(distanceFromTarget9+detThickness9+lgThickness9,2)+pow(delrinB9/2,2),0.5)*sin(theta9-fdelrinHalfAngle9);
  G4double firb9 = pow(pow(distanceFromTarget9+detThickness9+lgThickness9,2)+pow(delrinB9/2,2),0.5)*sin(theta9+fdelrinHalfAngle9);
  G4double bdelrinHalfAngle9 = atan(delrinB9/2/(distanceFromTarget9+detThickness9+lgThickness9+delrinA9));
  G4double birs9 = pow(pow(distanceFromTarget9+detThickness9+lgThickness9+delrinA9,2)+pow(delrinB9/2,2),0.5)*sin(theta9-bdelrinHalfAngle9);
  G4double birb9 = pow(pow(distanceFromTarget9+detThickness9+lgThickness9+delrinA9,2)+pow(delrinB9/2,2),0.5)*sin(theta9+bdelrinHalfAngle9);

  G4double delrin_dz9 = delrinB9/2;
  G4double delrin_theta9 = 0;
  G4double delrin_phi9 = 0;
  G4double delrin_dyab9 = delrinA9/2;
  G4double delrin_dxa9 = firs9*tan(180*deg/detNum9);
  G4double delrin_dxb9 = birs9*tan(180*deg/detNum9);
  G4double delrin_dycd9 = delrinA9/2;
  G4double delrin_dxc9 = firb9*tan(180*deg/detNum9);
  G4double delrin_dxd9 = birb9*tan(180*deg/detNum9);
  G4double delrin_alpab9 = 0;
  G4double delrin_alpcd9 = delrin_alpab9;    
  
  G4ThreeVector delrin_pos9 = G4ThreeVector(0,0,distanceFromTarget9*cos(theta9));

  G4Trap* solidDelrin9 =    
    new G4Trap("Delrin9",                       //its name
	      delrin_dz9, 
	      delrin_theta9, delrin_phi9,
	      delrin_dyab9,
	      delrin_dxa9, delrin_dxb9,
	      delrin_alpab9,
	      delrin_dycd9,
	      delrin_dxc9, delrin_dxd9,
	      delrin_alpcd9);                   //its size
      
  G4LogicalVolume* logicDelrin9 =                         
    new G4LogicalVolume(solidDelrin9,           //its solid
                        delrin,                 //its material
                        "Delrin9");             //its name
  logicDelrin9->SetVisAttributes(delrinVisAtt);
      
  //
  // CsI Detector
  //
  G4double coneheight9 = 2*distanceFromTarget9;
  G4double flatheight9 = coneheight9 -0.01*mm;
  G4double coneangle9 = theta9;
  G4double crystal_rmina9 = -(coneheight9-flatheight9)*tan(coneangle9+halfTheta9);
  G4double crystal_rmaxa9 = -(coneheight9-flatheight9)*tan(coneangle9-halfTheta9);
  G4double crystal_rminb9 = -coneheight9*tan(coneangle9+halfTheta9);
  G4double crystal_rmaxb9 = -coneheight9*tan(coneangle9-halfTheta9);
  G4double crystal_hz9 = 0.5*flatheight9;//(crystal_rmaxb9-crystal_rmaxa9)/crystal_rmaxb9*coneheight9;
  G4double crystal_phimin9 = 0.*deg, crystal_phimax9 = 360.*deg/detNum9;
  G4RotationMatrix tip_rotm9 = G4RotationMatrix();
  G4ThreeVector tip_pos9 = G4ThreeVector(0,0,0.5*flatheight9+(coneheight9-flatheight9)); //last term is a fudge
  G4Transform3D tip_trans9 = G4Transform3D(tip_rotm9, tip_pos9);
  G4ThreeVector z9 = G4ThreeVector(0,0,1);
  G4ThreeVector d09 = G4ThreeVector(distanceFromTarget9*sin(coneangle9)*cos(0.5*360*deg/detNum9), 
				    distanceFromTarget9*sin(coneangle9)*sin(0.5*360*deg/detNum9), 
				    -distanceFromTarget9*cos(coneangle9));
  G4ThreeVector unitAxis9 = z9.cross(d09);
  unitAxis9 = unitAxis9/unitAxis9.mag();
  G4double ux9 = unitAxis9.x();
  G4double uy9 = unitAxis9.y();
  G4double uz9 = unitAxis9.z();
  G4ThreeVector top9 = G4ThreeVector(cos(-coneangle9)+pow(ux9,2)*(1-cos(-coneangle9)), ux9*uy9*(1-cos(-coneangle9))-uz9*sin(-coneangle9), ux9*uz9*(1-cos(-coneangle9))+uy9*sin(-coneangle9));
  G4ThreeVector mid9 = G4ThreeVector(uy9*ux9*(1-cos(-coneangle9))+uz9*sin(-coneangle9), cos(-coneangle9)+pow(uy9,2)*(1-cos(-coneangle9)), uy9*uz9*(1-cos(-coneangle9))-ux9*sin(-coneangle9));
  G4ThreeVector bot9 = G4ThreeVector(uz9*ux9*(1-cos(-coneangle9))-uy9*sin(-coneangle9), uz9*uy9*(1-cos(-coneangle9))+ux9*sin(-coneangle9), cos(-coneangle9)+pow(uz9,2)*(1-cos(-coneangle9)));
  G4RotationMatrix firstBox_rotm9 = G4RotationMatrix();
  firstBox_rotm9.setRows(top9,mid9,bot9);
  G4ThreeVector firstBox_pos9 = G4ThreeVector(0,0,0);
  G4Transform3D firstBox_trans9 = G4Transform3D(firstBox_rotm9, firstBox_pos9);
  G4RotationMatrix secondBox_rotm9 = G4RotationMatrix();
  secondBox_rotm9.setRows(top9,mid9,bot9);
  G4ThreeVector secondBox_pos9 = (2*distanceFromTarget9+detThickness9)/d09.mag()*d09;
  G4Transform3D secondBox_trans9 = G4Transform3D(secondBox_rotm9, secondBox_pos9);
  
  G4Orb* solidTip9 =
    new G4Orb("tip9",
	      .0001*mm);
  G4Box* solidSubtractionBox9 = 
    new G4Box("SubtractionBox9",                  //its name
	      distanceFromTarget9,
	      distanceFromTarget9,
	      distanceFromTarget9);               //its size  
  G4Cons* solidCone9 =    
    new G4Cons("Cone9",                           //its name
	       crystal_rmina9, crystal_rmaxa9, 
	       crystal_rminb9, crystal_rmaxb9, 
	       crystal_hz9,
	       crystal_phimin9, crystal_phimax9); //its size
  G4UnionSolid* solidTippedCone9 =
    new G4UnionSolid("TippedCone9",
		     solidTip9,
		     solidCone9,
		     tip_trans9);
  G4SubtractionSolid* solidIntermediate9 =
    new G4SubtractionSolid("Intermediate9",
			   solidTippedCone9,
			   solidSubtractionBox9,
			   firstBox_trans9);
  G4SubtractionSolid* solidCrystal9 =
    new G4SubtractionSolid("Crystal9",
			   solidIntermediate9,
			   solidSubtractionBox9,
			   secondBox_trans9);
  
  G4LogicalVolume* logicCrystal9 =                         
    new G4LogicalVolume(solidCrystal9,            //its solid
                        CsI,                      //its material
                        "CsI Detector9");         //its name
  logicCrystal9->SetVisAttributes(crystalVisAtt);
  logicCrystal9->SetSensitiveDetector(m_MicroballScorer);
  
  //
  // Light Guide
  //       
  G4double lightGuide_px9 = 10*mm;
  G4double lightGuide_py9 = 10*mm;
  G4double lightGuide_pz9  = lgThickness9;      
  G4Box* solidLightGuide9 =    
    new G4Box("LightGuide9",                 //its name
              0.5*lightGuide_px9, 
	      0.5*lightGuide_py9,
	      0.5*lightGuide_pz9);           //its size
                
  G4LogicalVolume* logicLightGuide9 =                         
    new G4LogicalVolume(solidLightGuide9,    //its solid
                        glass,               //its material
                        "LightGuide9");      //its name
  logicLightGuide9->SetVisAttributes(lightGuideVisAtt);               
  
  
  for(int i = 1; i <= detNum9; i++)
    {
      G4RotationMatrix crystal_rotm = G4RotationMatrix();
      G4ThreeVector d0 = G4ThreeVector(distanceFromTarget9*sin(coneangle9)*cos(0.5*360*deg/detNum9), 
				       distanceFromTarget9*sin(coneangle9)*sin(0.5*360*deg/detNum9), 
				       distanceFromTarget9*cos(coneangle9));
      G4ThreeVector df = G4ThreeVector(distanceFromTarget9*sin(theta9)*sin((i-1)*360*deg/detNum9+.001*deg/*+0.5*360*deg/detNum9*/), 
				       distanceFromTarget9*sin(theta9)*cos((i-1)*360*deg/detNum9+.001*deg/*+0.5*360*deg/detNum9*/), 
				       -distanceFromTarget9*cos(theta9));
      df.setX(-df.x());
      G4ThreeVector axis = d0.cross(df);
      G4double ax = axis.x();
      G4double ay = axis.y();
      G4double az = axis.z();
      G4double ux = ax/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double uy = ay/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double uz = az/pow((pow(ax,2)+pow(ay,2)+pow(az,2)),0.5);
      G4double j = acos(d0.dot(df)/(d0.mag()*df.mag()));
      G4ThreeVector a = G4ThreeVector(cos(j)+pow(ux,2)*(1-cos(j)), ux*uy*(1-cos(j))-uz*sin(j), ux*uz*(1-cos(j))+uy*sin(j));
      G4ThreeVector b = G4ThreeVector(uy*ux*(1-cos(j))+uz*sin(j), cos(j)+pow(uy,2)*(1-cos(j)), uy*uz*(1-cos(j))-ux*sin(j));
      G4ThreeVector c = G4ThreeVector(uz*ux*(1-cos(j))-uy*sin(j), uz*uy*(1-cos(j))+ux*sin(j), cos(j)+pow(uz,2)*(1-cos(j)));
      G4RotationMatrix abc = G4RotationMatrix();
      abc.setRows(a,b,c);
      G4ThreeVector unitdf = df/df.mag();
      G4double udfx = unitdf.x();
      G4double udfy = unitdf.y();
      G4double udfz = unitdf.z();
      G4double k = 180.*deg;
      G4ThreeVector d = G4ThreeVector(cos(k)+pow(udfx,2)*(1-cos(k)), udfx*udfy*(1-cos(k))-udfz*sin(k), udfx*udfz*(1-cos(k))+udfy*sin(k));
      G4ThreeVector e = G4ThreeVector(udfy*udfx*(1-cos(k))+udfz*sin(k), cos(k)+pow(udfy,2)*(1-cos(k)), udfy*udfz*(1-cos(k))-udfx*sin(k));
      G4ThreeVector f = G4ThreeVector(udfz*udfx*(1-cos(k))-udfy*sin(k), udfz*udfy*(1-cos(k))+udfx*sin(k), cos(k)+pow(udfz,2)*(1-cos(k)));
      G4RotationMatrix def = G4RotationMatrix();
      def.setRows(d,e,f);
      crystal_rotm=def*abc;
      if (m_Flip == 1){
	crystal_rotm.rotateY(180*deg);
      }
      crystal_rotm.rotateZ(180*deg);
      G4ThreeVector crystal_pos = G4ThreeVector(0,0,0);
      G4Transform3D crystal_trans = G4Transform3D(crystal_rotm, crystal_pos);
      df.setX(-df.x());
      G4RotationMatrix lg_rotm = G4RotationMatrix();
      /*
      lg_rotm.rotateZ((i-1)*360*deg/detNum9);
      G4ThreeVector lg_pos = G4ThreeVector((distance9+detThickness9+0.5*lgThickness9)*cos(i*360*deg/detNum9),
					   (distance9+detThickness9+0.5*lgThickness9)*sin(i*360*deg/detNum9),
					   0)+delrin_pos9;
      */
      G4ThreeVector lg_Axis = df.cross(z9);
      lg_Axis = lg_Axis/lg_Axis.mag();
      lg_Axis.rotateZ(180*deg);
      G4double ulgx = lg_Axis.x();
      G4double ulgy = lg_Axis.y();
      G4double ulgz = lg_Axis.z();
      G4ThreeVector p = G4ThreeVector(cos(theta9)+pow(ulgx,2)*(1-cos(theta9)), ulgx*ulgy*(1-cos(theta9))-ulgz*sin(theta9), ulgx*ulgz*(1-cos(theta9))+ulgy*sin(theta9));
      G4ThreeVector q = G4ThreeVector(ulgy*ulgx*(1-cos(theta9))+ulgz*sin(theta9), cos(theta9)+pow(ulgy,2)*(1-cos(theta9)), ulgy*ulgz*(1-cos(theta9))-ulgx*sin(theta9));
      G4ThreeVector r = G4ThreeVector(ulgz*ulgx*(1-cos(theta9))-ulgy*sin(theta9), ulgz*ulgy*(1-cos(theta9))+ulgx*sin(theta9), cos(theta9)+pow(ulgz,2)*(1-cos(theta9)));
      G4RotationMatrix pqr = G4RotationMatrix();
      pqr.setRows(p,q,r);
      lg_rotm.rotateZ((i-1)*360*deg/detNum9);
      lg_rotm.rotateX(180*deg);
      lg_rotm=pqr*lg_rotm;
      G4ThreeVector lg_pos = (distanceFromTarget9+detThickness9+0.5*lgThickness9+.0003*mm)/df.mag()*df;
      lg_pos.rotateZ(180*deg);
      if (m_Flip == 0){
	lg_pos.rotateY(180*deg);
	lg_rotm.rotateY(180*deg);
      }
      G4Transform3D lg_trans = G4Transform3D(lg_rotm, lg_pos);
      G4RotationMatrix delrin_rotm = G4RotationMatrix();
      G4ThreeVector w = G4ThreeVector(sin(theta9)*sin((i-1)*360*deg/detNum9+.001*deg), sin(theta9)*cos((i-1)*360*deg/detNum9+.001*deg), cos(theta9));
      G4ThreeVector delrin_Axis = w.cross(z9);
      delrin_Axis.rotateX(180*deg);
      delrin_Axis = delrin_Axis/delrin_Axis.mag();
      G4double udx = delrin_Axis.x();
      G4double udy = delrin_Axis.y();
      G4double udz = delrin_Axis.z();
      G4double delAng = -90*deg+theta9;
      G4ThreeVector pdel = G4ThreeVector(cos(delAng)+pow(udx,2)*(1-cos(delAng)), udx*udy*(1-cos(delAng))-udz*sin(delAng), udx*udz*(1-cos(delAng))+udy*sin(delAng));
      G4ThreeVector qdel = G4ThreeVector(udy*udx*(1-cos(delAng))+udz*sin(delAng), cos(delAng)+pow(udy,2)*(1-cos(delAng)), udy*udz*(1-cos(delAng))-udx*sin(delAng));
      G4ThreeVector rdel = G4ThreeVector(udz*udx*(1-cos(delAng))-udy*sin(delAng), udz*udy*(1-cos(delAng))+udx*sin(delAng), cos(delAng)+pow(udz,2)*(1-cos(delAng)));
      G4RotationMatrix pqrdel = G4RotationMatrix();
      pqrdel.setRows(pdel,qdel,rdel);
      delrin_rotm.rotateZ((-i+1)*360*deg/detNum9+.001*deg);
      delrin_rotm.rotateX(180*deg);
      delrin_rotm=pqrdel*delrin_rotm;
      delrin_rotm.rotateZ(0*deg);
      G4ThreeVector delrin_pos = (distanceFromTarget9+detThickness9+lgThickness9+delrinA9/2+.0005*mm)/df.mag()*df;  //the .0003 mm is a fudge factor to prevent overlapping
      delrin_pos.rotateZ(180*deg);
      delrin_pos.rotateY(180*deg);
      if (m_Flip == 1){
	delrin_pos.rotateY(180*deg);
	delrin_rotm.rotateY(180*deg);
      }
      G4Transform3D delrin_trans = G4Transform3D(delrin_rotm, delrin_pos); 
      bool placeCrystal = true;
      for (unsigned int i2=0; i2 < m_Array.size(); i2++){
	if (90 < m_Array[i2] && m_Array[i2] < 97){ 
	  if (m_Array[i2] == 90 +(i -detNum9/2)%detNum9){
	    placeCrystal = false;
	  }else if (m_Array[i2] == 90 +(i -detNum9/2)%detNum9 +detNum9){
	    placeCrystal = false;
	  }
	}
      }
      if (placeCrystal){     
      if(crystalBool) {new G4PVPlacement(crystal_trans,       //rotation, position
                    logicCrystal9,           //its logical volume
                    "CsI Detector9",         //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
                    90+i,                     //copy number
                    checkOverlaps);          //overlaps checking
      }
      }
      if (placeCrystal){
      if(lgBool) {new G4PVPlacement(lg_trans,            //rotation, position
                    logicLightGuide9,        //its logical volume
                    "LightGuide9",           //its name
                    world,              //its mother  volume
                    false,                   //no boolean operation
		    90+i,                     //copy number
		    checkOverlaps);          //overlaps checking
      }
      if(delrinBool) {new G4PVPlacement(delrin_trans,          //rotation, position
                    logicDelrin9,               //its logical volume
                    "Delrin9",                  //its name
                    world,                 //its mother  volume
                    false,                      //no boolean operation
                    90+i,                        //copy number
                    checkOverlaps);             //overlaps checking
      }
      }
    }}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Microball::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("Microball")){
    pTree->Branch("Microball", "TMicroballData", &m_Event) ;
  }
  pTree->SetBranchAddress("Microball", &m_Event) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Microball::ReadSensitive(const G4Event* event){
  m_Event->Clear();

  ///////////
  // Calorimeter scorer
  G4THitsMap<G4double*>* CaloHitMap;
  std::map<G4int, G4double**>::iterator Calo_itr;

  G4int CaloCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("MicroballScorer/Calorimeter");
  CaloHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(CaloCollectionID));

  // Loop on the Calo map
  for (Calo_itr = CaloHitMap->GetMap()->begin() ; Calo_itr != CaloHitMap->GetMap()->end() ; Calo_itr++){

    G4double* Info = *(Calo_itr->second);
    //(Info[0]/2.35)*((Info[0]*1.02)*pow((Info[0]*1.8),.5))
    // double Energy = RandGauss::shoot(Info[0],((Info[0]*1000*1.02/2.35)*pow((Info[0]*1000*1.8),.5)) );
    double Energy = RandGauss::shoot(Info[0],Microball_NS::ResoEnergy);
    if(Energy>Microball_NS::EnergyThreshold){
      double Time = RandGauss::shoot(Info[1],Microball_NS::ResoTime);
      int DetectorNbr = (int) Info[7];
      m_Event->SetEnergy(DetectorNbr,Energy);
      m_Event->SetTime(DetectorNbr,Time); 

      ms_InterCoord->SetDetectedPositionX(Info[2]) ;
      ms_InterCoord->SetDetectedPositionY(Info[3]) ;
      ms_InterCoord->SetDetectedPositionZ(Info[4]) ;
      ms_InterCoord->SetDetectedAngleTheta(Info[5]/deg) ;
      ms_InterCoord->SetDetectedAnglePhi(Info[6]/deg) ;
    }
  }
  // clear map for next event
  CaloHitMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////   
void Microball::InitializeScorers() { 
  // This check is necessary in case the geometry is reloaded
  bool already_exist = false; 
  m_MicroballScorer = CheckScorer("MicroballScorer",already_exist) ;

  if(already_exist) 
    return ;

  // Otherwise the scorer is initialised
  vector<int> level; level.push_back(0);
  G4VPrimitiveScorer* Calorimeter= new CALORIMETERSCORERS::PS_CalorimeterWithInteraction("Calorimeter",level, 0) ;
  //and register it to the multifunctionnal detector
  m_MicroballScorer->RegisterPrimitive(Calorimeter);
  G4SDManager::GetSDMpointer()->AddNewDetector(m_MicroballScorer) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* Microball::Construct(){
  return  (NPS::VDetector*) new Microball();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
  class proxy_nps_plastic{
    public:
      proxy_nps_plastic(){
        NPS::DetectorFactory::getInstance()->AddToken("Microball","Microball");
        NPS::DetectorFactory::getInstance()->AddDetector("Microball",Microball::Construct);
      }
  };

  proxy_nps_plastic p_nps_plastic;
}
