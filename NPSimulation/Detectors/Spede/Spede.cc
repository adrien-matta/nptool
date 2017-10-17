/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Daniel Cox  contact address: daniel.m.cox@jyu.fi                        *
 *                                                                           *
 * Creation Date  : January 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  Spede simulation                             *
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
#include "G4Polycone.hh"
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
#include "G4THitsMap.hh"

// NPTool header
#include "Spede.hh"
#include "SiliconScorers.hh"
#include "RootOutput.h"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "NPOptionManager.h"
// CLHEP header
#include "CLHEP/Random/RandGauss.h"
// EM field
#include "SpedeHVField.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4EqMagElectricField.hh"

#include "G4ChordFinder.hh"
#include "G4UniformMagField.hh"
#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"

// CADMESH //
#include "CADMesh.hh"

using namespace std;
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace Spede_NS{
  // Energy and time Resolution
  const double EnergyThreshold = 10.*keV;
  const double ResoTime = 100*ns ;
  const double ResoEnergy = 2.7*keV ;//Sigma not actual resolution// 2.7*2.2 ~6keV
  //const double Radius = 50*mm ; 
  //const double Width = 100*mm ;
  const double Thickness = 1.0*mm ;
  //const string Material = "BC400";
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Spede Specific Method
Spede::Spede(){
  m_Event = new TSpedeData() ;
  m_SpedeScorer = 0;
  m_SiliconDetector = 0;

  // RGB Color + Transparency
  m_VisSilicon = new G4VisAttributes(G4Colour(0, 0.5, 0));   

  fieldIsInitialized = false;
  //G4String fieldFileName = "SpedeOldHVTable.table";//TODO make this a user set variable
  G4String fieldFileName = "130617SPEDE+CD.TABLE";//TODO make this a user set variable
  fHVFieldStrength = 1;//TODO make this a user set variable
}

Spede::~Spede(){
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Spede::AddDetector(G4ThreeVector POS, string  Shape){
  // Convert the POS value to R theta Phi as Spherical coordinate is easier in G4 
  m_R.push_back(POS.mag());
  m_Theta.push_back(POS.theta());
  m_Phi.push_back(POS.phi());
  m_Shape.push_back(Shape);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Spede::AddDetector(double  R, double  Theta, double  Phi, string  Shape){
  m_R.push_back(R);
  m_Theta.push_back(Theta);
  m_Phi.push_back(Phi);
  m_Shape.push_back(Shape);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Spede::BuildSiliconDetector()
{
	if(!m_SiliconDetector)
	{
		G4Tubs* tub = new G4Tubs("SpedeSilicon",20./2.*mm,47.6/2.*mm,Spede_NS::Thickness*0.5,0,360*deg);

		G4Material* DetectorMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary("Si");
		m_SiliconDetector = new G4LogicalVolume(tub,DetectorMaterial,"logic_Spede_silicon",0,0,0);
		m_SiliconDetector->SetVisAttributes(m_VisSilicon);
		m_SiliconDetector->SetSensitiveDetector(m_SpedeScorer);

	}
	return m_SiliconDetector;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct SPEDE absorber foil
//
//G4LogicalVolume* Spede::BuildFoil(G4double thickness,
void Spede::BuildFoil(G4double thickness,
    G4RotationMatrix *Rot,
    G4ThreeVector Det_pos,
    G4LogicalVolume *world)
{
    double foilInnerHole = 10.;
    double foilOuterRadius = 30.;

    G4Tubs* Foil = new G4Tubs("Foil ",
        foilInnerHole*mm,
        foilOuterRadius*mm,
        thickness/2*mm,
        0.*deg, 360.*deg);
    
    G4Material* Mylar= MaterialManager::getInstance()->GetMaterialFromLibrary("Mylar");
    m_Foil = new G4LogicalVolume(Foil, Mylar, "Foil_log");
    G4VisAttributes *foilVA = new G4VisAttributes(G4Colour(0.,0.5,0.5));
    m_Foil -> SetVisAttributes(foilVA);

	G4double fs_thickness = 2.;
	G4Tubs* spede_fs = new G4Tubs("SpedeFoilSupport",50./2.*mm,60./2.*mm,fs_thickness/2.*mm,0,360*deg);
	G4Material* FoilSupportMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary("Al");

	G4LogicalVolume* m_SPEDE_fs = new G4LogicalVolume(spede_fs, FoilSupportMaterial, "SPEDE_fs_log");

	G4VisAttributes *fsVA = new G4VisAttributes(G4Colour(0.8,0.8,0.8));
	m_SPEDE_fs -> SetVisAttributes(fsVA);

	G4ThreeVector fs_pos = Det_pos + Det_pos.unit()*(thickness+fs_thickness/2);

	new G4PVPlacement(G4Transform3D(*Rot,fs_pos),
			m_SPEDE_fs,
			"SPEDE_fs_back", world, false,0);

	fs_thickness = 1.;
	G4Tubs* spede_fs_front = new G4Tubs("SpedeFoilSupport",50./2.*mm,60./2.*mm,fs_thickness/2.*mm,0,360*deg);

	G4LogicalVolume* m_SPEDE_fs_front = new G4LogicalVolume(spede_fs_front, FoilSupportMaterial, "SPEDE_fs_log");

	m_SPEDE_fs_front -> SetVisAttributes(fsVA);

	G4ThreeVector fs_pos_front = Det_pos - Det_pos.unit()*(thickness+fs_thickness/2);

	new G4PVPlacement(G4Transform3D(*Rot,fs_pos_front),
			m_SPEDE_fs_front,
			"SPEDE_fs_front", world, false,0);

	new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
			m_Foil,
			"foil",world,false, 0);

    return;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Construct Spede ladder
G4LogicalVolume* Spede::BuildLadder(G4int)
{
    G4RotationMatrix* Target_Square_Rotation = new G4RotationMatrix();
    Target_Square_Rotation->rotateX(180.*deg);
    
    G4VSolid* TL1 = new G4Box("L1",22.*mm/2,95.*mm/2,3.5*mm/2);

    //G4VSolid* Target_Hole = new G4Tubs("L2", 0., 10.*mm/2, 4.*mm,0.*deg, 360.*deg);

    G4VSolid* Ladder_Holder = new G4Tubs("Ladder_Support", 0., 15.*mm/2,33./2*mm,0.*deg,360.*deg);

    G4VSolid* Target_Square = new G4Box("Target_Square",15./2*mm,15./2*mm,12./2*mm);

    //G4VSolid* Target_Pyramid = new G4Trd("Target_Pyramid", 16.01/2*mm,12.01/2*mm,16.010/2*mm,12.01/2*mm,4.010/2*mm);
    G4ThreeVector TargetPosition;

    TargetPosition.set(-1.*mm,(95./2-11.)*mm,(0.)*mm);
    G4SubtractionSolid* TLS2 =
        new G4SubtractionSolid("TLS2",TL1, Target_Square, Target_Square_Rotation, TargetPosition);

    TargetPosition.set(-1.*mm,(95./2-31.)*mm,(0.)*mm);
    G4SubtractionSolid* TLS3 =
        new G4SubtractionSolid("TLS3",TLS2, Target_Square, Target_Square_Rotation, TargetPosition);

    TargetPosition.set(-1.*mm,(95./2-51.)*mm,(0.)*mm);
    G4SubtractionSolid* TLS4 =
        new G4SubtractionSolid("TLS4",TLS3, Target_Square, Target_Square_Rotation, TargetPosition);

    TargetPosition.set(-1.*mm,(95./2-71.)*mm,(0.)*mm);
    G4SubtractionSolid* TLS5 =
        new G4SubtractionSolid("TLS5",TLS4, Target_Square, Target_Square_Rotation, TargetPosition);

    G4RotationMatrix* tRot90deg = new G4RotationMatrix();
    tRot90deg->rotateX(90.*deg);

    TargetPosition.set(2.5*mm,(-95./2-1.5)*mm,0.);
    G4UnionSolid* Target_Ladder_Square=
        new G4UnionSolid("Target_Ladder_Square", TLS5, Ladder_Holder, tRot90deg, TargetPosition);

    G4Material* Mylar= MaterialManager::getInstance()->GetMaterialFromLibrary("Mylar");
    m_Ladder = new G4LogicalVolume(Target_Ladder_Square,
      Mylar, "Target_Ladder_Square_log");
    G4VisAttributes *ladderVA = new G4VisAttributes(G4Colour(1.0,0.65,0.));
    m_Ladder -> SetVisAttributes(ladderVA);


    return m_Ladder;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Build PCB
void Spede::BuildPCB(G4double si_thickness,
    G4double pcb_thickness,
    G4RotationMatrix *Rot_old,
    G4ThreeVector Det_pos,
    G4LogicalVolume *world)
{
  /*G4Tubs* SPEDE_pcb= new G4Tubs("SPEDE pcb",
        8.5*mm,
        70.*mm,
        pcb_thickness/2*mm,
        0*deg,
        360*deg);
    G4Material* PCB= MaterialManager::getInstance()->GetMaterialFromLibrary("PCB");
    G4LogicalVolume* m_SPEDE_pcb = new G4LogicalVolume(SPEDE_pcb, PCB, "SPEDE_pcb_log");

    G4VisAttributes *pcbVA = new G4VisAttributes(G4Colour(0.0,0.3,0.0));
    m_SPEDE_pcb -> SetVisAttributes(pcbVA);
  
	//G4ThreeVector pcb_pos = Det_pos + Det_pos.unit()*(si_thickness/2+pcb_thickness/2);
//	new G4PVPlacement(G4Transform3D(*Rot,pcb_pos),
//			m_SPEDE_pcb,
//			"SPEDE pcb", world, false,0);

	//G4double cb_thickness = 3.;
	G4Tubs* spede_cb = new G4Tubs("SpedeCoolingBlock",20./2.*mm,74./2.*mm,cb_thickness/2*mm,0,360*deg);
	G4Material* CoolingBlockMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary("Cu");

	G4LogicalVolume* m_SPEDE_cb = new G4LogicalVolume(spede_cb, CoolingBlockMaterial, "SPEDE_cb_log");

	G4VisAttributes *cbVA = new G4VisAttributes(G4Colour(1.0,0.3,0.0));
	m_SPEDE_cb -> SetVisAttributes(cbVA);

	//G4ThreeVector cb_pos = Det_pos + Det_pos.unit()*(si_thickness/2+pcb_thickness+cb_thickness/2);

	//new G4PVPlacement(G4Transform3D(*Rot,cb_pos),
	//		m_SPEDE_cb,
	//		"SPEDE cb", world, false,0);
*/

	//TESTING for target interactions
	/*
	G4double target_thickness = 10.;//in um
	G4Tubs* spede_target = new G4Tubs("SpedeTarget",0.*mm,2./2.*mm,target_thickness/2*um,0,360*deg);
	G4Material* TargetMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary("Au");

	G4LogicalVolume* m_SPEDE_target = new G4LogicalVolume(spede_target, TargetMaterial, "SPEDE_target_log");

	G4VisAttributes *targetVA = new G4VisAttributes(G4Colour(1.0,0.3,0.0));
	m_SPEDE_target -> SetVisAttributes(targetVA);

	G4ThreeVector target_pos = G4ThreeVector(0,0,0);

	new G4PVPlacement(G4Transform3D(*Rot,target_pos),
			m_SPEDE_target,
			"SPEDE target", world, false,0);//*/

    G4RotationMatrix* rot = new G4RotationMatrix();
    rot->rotateY(180*deg);
    G4ThreeVector offset;
    offset = G4ThreeVector(0, 0, -21.5*mm);//Sets centre to 0 on Z axis

	string filename;
	CADMesh * mesh;
	G4VSolid* cad_solid;
	G4LogicalVolume* cad_logical;

	
	//Read in file
	filename = "/Users/dacox/work/nptool/NPSimulation/Detectors/Spede/spede_pcb.stl";
	//filename = "/Users/dacox/work/nptool/NPSimulation/Detectors/Spede/Untitled.stl";

	//Create the mesh
    mesh = new CADMesh((char*) filename.c_str());
    mesh->SetScale(mm);
    mesh->SetOffset(offset);
    mesh->SetReverse(false);
	
	//Attach to solid and logical
	cad_solid = mesh->TessellatedMesh();
	cad_logical = new G4LogicalVolume(cad_solid,
			MaterialManager::getInstance()->GetMaterialFromLibrary("PCB"),
			"Spede_pcb", 0, 0, 0);

	G4ThreeVector pcb_pos = Det_pos + Det_pos.unit()*(si_thickness/2);
	//Place
	new G4PVPlacement(rot, pcb_pos, cad_logical,
                                     "SPEDE_PCB", world, false, 0);
	//colour
    cad_logical->SetVisAttributes(G4Color(0.0, 0.3, 0., 1.0));

    offset = G4ThreeVector(0, 0, -20.6*mm);//Sets centre to 0 on Z axis
	//Read in file
	filename = "/Users/dacox/work/nptool/NPSimulation/Detectors/Spede/spede_preamps.stl";
	//filename = "/Users/dacox/work/nptool/NPSimulation/Detectors/Spede/Untitled.stl";

	//Create the mesh
    mesh = new CADMesh((char*) filename.c_str());
    mesh->SetScale(mm);
    mesh->SetOffset(offset);
    mesh->SetReverse(false);
	
	//Attach to solid and logical
	cad_solid = mesh->TessellatedMesh();
	cad_logical = new G4LogicalVolume(cad_solid,
			MaterialManager::getInstance()->GetMaterialFromLibrary("Cu"),
			"spede_preamps", 0, 0, 0);

	//Place
	new G4PVPlacement(rot, pcb_pos, cad_logical,
                                     "SPEDE_preamps", world, false, 0);
	//colour
    cad_logical->SetVisAttributes(G4Color(0.8, 0.7, 0., 1.0));

	G4double cb_thickness = 1.;
    offset = G4ThreeVector(0, 0, -23.1*mm);//Sets centre to 0 on Z axis
	G4ThreeVector cb_pos = Det_pos + Det_pos.unit()*(si_thickness+pcb_thickness+cb_thickness);
	//Read in file
	filename = "/Users/dacox/work/nptool/NPSimulation/Detectors/Spede/spede_cb.stl";
	//filename = "/Users/dacox/work/nptool/NPSimulation/Detectors/Spede/Untitled.stl";

	//Create the mesh
    mesh = new CADMesh((char*) filename.c_str());
    mesh->SetScale(mm);
    mesh->SetOffset(offset);
    mesh->SetReverse(false);
	
	//Attach to solid and logical
	cad_solid = mesh->TessellatedMesh();
	cad_logical = new G4LogicalVolume(cad_solid,
			MaterialManager::getInstance()->GetMaterialFromLibrary("Cu"),
			"spede_coolingblock", 0, 0, 0);

	//Place
	new G4PVPlacement(rot, cb_pos, cad_logical,
                                     "SPEDE_coolingblock", world, false, 0);
	//colour
    cad_logical->SetVisAttributes(G4Color(1.0, 0.7, 0., 1.0));

	G4ThreeVector connector_pos = cb_pos + Det_pos.unit()*(15.);
    offset = G4ThreeVector(0, 0, -38.1*mm);//Sets centre to 0 on Z axis
	//Read in file
	filename = "/Users/dacox/work/nptool/NPSimulation/Detectors/Spede/spede_connectors_al.stl";
	//filename = "/Users/dacox/work/nptool/NPSimulation/Detectors/Spede/Untitled.stl";

	//Create the mesh
    mesh = new CADMesh((char*) filename.c_str());
    mesh->SetScale(mm);
    mesh->SetOffset(offset);
    mesh->SetReverse(false);
	
	//Attach to solid and logical
	cad_solid = mesh->TessellatedMesh();
	cad_logical = new G4LogicalVolume(cad_solid,
			MaterialManager::getInstance()->GetMaterialFromLibrary("Al"),
			"spede_connectors_al", 0, 0, 0);

	//Place
	new G4PVPlacement(rot, connector_pos, cad_logical,
                                     "SPEDE_connectors_al", world, false, 0);
	//colour
    cad_logical->SetVisAttributes(G4Color(0.8, 0.8, 0.8, 1.0));

	connector_pos = Det_pos + Det_pos.unit()*(si_thickness/2+pcb_thickness+cb_thickness/2+10.5);
	//Read in file
	filename = "/Users/dacox/work/nptool/NPSimulation/Detectors/Spede/spede_connectors_plastic.stl";
	//filename = "/Users/dacox/work/nptool/NPSimulation/Detectors/Spede/Untitled.stl";

	//Create the mesh
    mesh = new CADMesh((char*) filename.c_str());
    mesh->SetScale(mm);
    //mesh->SetOffset(offset-pcb_pos);
    mesh->SetOffset(G4ThreeVector(0,0,0));
    mesh->SetReverse(false);
	
	//Attach to solid and logical
	cad_solid = mesh->TessellatedMesh();
	cad_logical = new G4LogicalVolume(cad_solid,
			MaterialManager::getInstance()->GetMaterialFromLibrary("PCB"),
			"spede_connectors_plastic", 0, 0, 0);

	connector_pos = Det_pos + Det_pos.unit()*(si_thickness/2+pcb_thickness+cb_thickness/2-20.5);
	//Place
	new G4PVPlacement(rot, connector_pos, cad_logical,
                                     "SPEDE_connectors_plastic", world, false, 0);
	//colour
    cad_logical->SetVisAttributes(G4Color(0.0, 0.3, 0., 1.0));

	//Read in file
	filename = "/Users/dacox/work/nptool/NPSimulation/Detectors/Spede/spede_brass.stl";



	//Create the mesh
    mesh = new CADMesh((char*) filename.c_str());
    mesh->SetScale(mm);
    mesh->SetOffset(offset);
    mesh->SetReverse(false);
	
	//Attach to solid and logical
	cad_solid = mesh->TessellatedMesh();
	cad_logical = new G4LogicalVolume(cad_solid,
			MaterialManager::getInstance()->GetMaterialFromLibrary("Brass"),
			"spede_brass", 0, 0, 0);

	G4ThreeVector brass_pos = Det_pos + Det_pos.unit()*(si_thickness/2+pcb_thickness+cb_thickness/2+10.);


	//Place
	new G4PVPlacement(rot, brass_pos, cad_logical,
                                     "SPEDE_brass", world, false, 0);
	//colour
    cad_logical->SetVisAttributes(G4Color(0.86, 0.78, 0., 1));

	//Read in file
	/*filename = "/Users/dacox/work/nptool/NPSimulation/Detectors/Spede/spede_cu.stl";

	//Create the mesh
    mesh = new CADMesh((char*) filename.c_str());
    mesh->SetScale(mm);
    mesh->SetOffset(offset);
    mesh->SetReverse(false);
	
	//Attach to solid and logical
	cad_solid = mesh->TessellatedMesh();
	cad_logical = new G4LogicalVolume(cad_solid,
			MaterialManager::getInstance()->GetMaterialFromLibrary("Cu"),
			"cad_logical", 0, 0, 0);

	//Place
	new G4PVPlacement(rot, G4ThreeVector(), cad_logical,
                                     "SPEDE_copper", world, false, 0);
	//colour
    cad_logical->SetVisAttributes(G4Color(1.0, 0.5, 0., 1));
	*/
	//Read in file
	filename = "/Users/dacox/work/nptool/NPSimulation/Detectors/Spede/spede_pb.stl";

	//Create the mesh
    mesh = new CADMesh((char*) filename.c_str());
    mesh->SetScale(mm);
    mesh->SetOffset(offset);
    mesh->SetReverse(false);
	
	//Attach to solid and logical
	cad_solid = mesh->TessellatedMesh();
	cad_logical = new G4LogicalVolume(cad_solid,
			MaterialManager::getInstance()->GetMaterialFromLibrary("Pb"),
			"spede_pb", 0, 0, 0);

	G4ThreeVector pb_pos = Det_pos + Det_pos.unit()*(si_thickness/2+pcb_thickness+cb_thickness/2+10.);

	//Place
	new G4PVPlacement(rot, pb_pos, cad_logical,
                                     "SPEDE_lead", world, false, 0);
	//colour
    cad_logical->SetVisAttributes(G4Color(0.4, 0.4, 0.4, 1));

	//Read in file
	filename = "/Users/dacox/work/nptool/NPSimulation/Detectors/Spede/spede_w.stl";

	//Create the mesh
    mesh = new CADMesh((char*) filename.c_str());
    mesh->SetScale(mm);
    mesh->SetOffset(offset);
    mesh->SetReverse(false);
	
	//Attach to solid and logical
	cad_solid = mesh->TessellatedMesh();
	cad_logical = new G4LogicalVolume(cad_solid,
			MaterialManager::getInstance()->GetMaterialFromLibrary("W"),
			"spede_w", 0, 0, 0);

	G4ThreeVector w_pos = Det_pos + Det_pos.unit()*(si_thickness/2+pcb_thickness+cb_thickness/2+10.);

	//Place
	new G4PVPlacement(rot, w_pos, cad_logical,
                                     "SPEDE_tungsten", world, false, 0);
	//colour
    cad_logical->SetVisAttributes(G4Color(0.6, 0.6, 0.6, 1));
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Build chamber
void Spede::BuildChamber(G4LogicalVolume *world)
{
    // =========================================================================
    // SPEDE chamber
    // =========================================================================
    G4int nmbRZ;

    //G4double TCInner = 154.*mm/2;
    //G4double TCOuter = 164.*mm/2;
    G4double TCWidth = 95.*mm/2;

    nmbRZ = 4;

    double TCr[] ={0.,
        0.+16.5,
        0.+16.5+62,
        0.+16.5+62+13.5};

    double TCz1[] ={132./2,
        159./2,
        159./2,
        140./2};

    double TCz2[] ={134.5/2,
        161.5/2,
        161.5/2,
        142.5/2};

    G4VSolid* TCBody = new G4Polycone(
            "TCBody",
            0.*deg,
            360.*deg,
            nmbRZ,
            TCr,
            TCz1,
            TCz2);

    nmbRZ = 3;

    double TCP1r[] = {0.,
        52.7/2,
        99./2};
    double TCP1z1[] = {150./2,
        60./2,
        60./2};
    double TCP1z2[] = {154./2,
        64./2,
        64./2};

    G4VSolid* TCPol1 = new G4Polycone(
            "TCPol1",
            0.*deg,// start angle phi
            360.*deg,// total angle phi
            nmbRZ,// Numbers of corners in the r,z space
            TCP1r,
            TCP1z1,
            TCP1z2);

    nmbRZ = 5;

    double TCP2r[] = {62.,
        62.-29.4,
        62.-47.,
        62.-47.00001,
        0.};
    double TCP2z1[] = {129./2,
        59./2,
        59./2,
        40./2,
        40./2};
    double TCP2z2[] = {142./2,
        61.5/2,
        61.5/2,
        61.5/2,
        61.5/2};

    G4VSolid* TCPol2 = new G4Polycone(
            "TCPol2",
            0.*deg,// start angle phi
            360.*deg,// total angle phi
            nmbRZ,// Numbers of corners in the r,z space
            TCP2r,
            TCP2z1,
            TCP2z2);

    G4ThreeVector position;

    G4Material* ChamberMaterial= MaterialManager::getInstance()->GetMaterialFromLibrary("Al");

/*
    G4LogicalVolume* Chamber_log
      = new G4LogicalVolume(TCBody, ChamberMaterial, "TargetChamber_log");

    position.set(0, 0, -44.5);
    new G4PVPlacement(0, position, Chamber_log, "TargetChamber", world, false, 0);

    G4LogicalVolume* TCPol1_log
        =new G4LogicalVolume(TCPol1, ChamberMaterial,"TCPol1_log");

    position.set(0, 0, TCWidth);
    new G4PVPlacement(0,position, TCPol1_log, "TC_pol1_phys", world, false, 0);

    G4LogicalVolume* TCPol2_log
        =new G4LogicalVolume(TCPol2, ChamberMaterial,"TCPol2_log");

    position.set(0, 0, -TCWidth-62.*mm);
    new G4PVPlacement(0,position, TCPol2_log, "TC_pol2_phys", world, false, 0);
*/
    // CAD model rotation and position
    G4RotationMatrix * rot = new G4RotationMatrix();
    rot->rotateY(180*deg);
    G4ThreeVector offset;
    offset = G4ThreeVector(0, 0, 0);//Places chamber front face of target is 0

	string filename;
	CADMesh * mesh;
	G4VSolid* cad_solid;
	G4LogicalVolume* cad_logical;

	
	//Read in file
	filename = "/Users/dacox/work/nptool/NPSimulation/Detectors/Spede/spede_al.stl";

	//Create the mesh
    mesh = new CADMesh((char*) filename.c_str());
    mesh->SetScale(mm);
    mesh->SetOffset(offset);
    mesh->SetReverse(false);
	
	//Attach to solid and logical
	cad_solid = mesh->TessellatedMesh();
	cad_logical = new G4LogicalVolume(cad_solid,
			MaterialManager::getInstance()->GetMaterialFromLibrary("Al"),
			"spede_chamber", 0, 0, 0);

	//Place
	new G4PVPlacement(rot, G4ThreeVector(), cad_logical,
                                     "SPEDE_chamber", world, false, 0);
	//colour
    //cad_logical->SetVisAttributes(G4Color(0.8, 0.8, 0.8, 0.7));
    cad_logical->SetVisAttributes(G4Color(0.8, 0.8, 0.8));

}
void Spede::ConstructEMField(G4String fieldFileName)
{

	G4ElectroMagneticField* HVField;
	G4FieldManager *HVFieldManager;
	G4MagIntegratorStepper * pStepper;
	G4EqMagElectricField * pEquation;
	G4MagInt_Driver * pIntgrDriver;
	G4ChordFinder *pChordFinder ;
	G4PropagatorInField *propInField;
	G4int nvar;
	/*Choose the electromagnetic field here, one day this should be read in from a file*/

	HVFieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();
	G4cout << "Reading in field file" << endl;
	HVField = new SpedeHVField(fieldFileName, 0, fHVFieldStrength);
	//HVField = new SpedeHVField("SpedeHVField.table", 0, 1.);
	//HVField = new SpedeHVField("SpedeHVField.table", 0, 1.);
	//G4ElectroMagneticField* foo;
	//foo = new SpedeHVField(0, 0, 0);

	pEquation = new G4EqMagElectricField(HVField);
	nvar = 8;

	/*Choose the stepper here*/
	pStepper = new G4CashKarpRKF45( pEquation, nvar );
	//pStepper = new G4ClassicalRK4( pEquation, nvar );
	//pStepper = new G4SimpleHeum( pEquation, nvar );
	//pStepper = new G4SimpleRunge( pEquation, nvar );
	//pStepper = new G4ExplicitEuler( pEquation, nvar );
	//pStepper = new G4ImplicitEuler( pEquation, nvar );


	G4double minStep  = 0.01*mm;
	pIntgrDriver = new G4MagInt_Driver(minStep,pStepper,pStepper->GetNumberOfVariables() );
	pChordFinder = new G4ChordFinder(pIntgrDriver);
	HVFieldManager->SetChordFinder(pChordFinder);

	HVFieldManager->GetChordFinder()->SetDeltaChord(1e-3*m);
	HVFieldManager->SetDeltaIntersection(1e-3*m);
	HVFieldManager->SetDeltaOneStep(1e-3*m);
	HVFieldManager->SetDetectorField(HVField);
	propInField =  G4TransportationManager::GetTransportationManager()->GetPropagatorInField();
	//propInField->SetMinimumEpsilonStep(1e-6);
	//propInField->SetMaximumEpsilonStep(1e-3);

	fieldIsInitialized = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Spede::ReadConfiguration(NPL::InputParser parser)
{
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Spede");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> cart = {"POS","Shape"};
  vector<string> sphe = {"R","Theta","Phi","Shape"};
  vector<string> field = {"Field"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(cart)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Spede " << i+1 <<  endl;
    
      G4ThreeVector Pos = NPS::ConvertVector(blocks[i]->GetTVector3("POS","mm"));
      string Shape = blocks[i]->GetString("Shape");
      AddDetector(Pos,Shape);
    }
    else if(blocks[i]->HasTokenList(sphe)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Spede " << i+1 <<  endl;
      double R = blocks[i]->GetDouble("R","mm");
      double Theta = blocks[i]->GetDouble("Theta","deg");
      double Phi = blocks[i]->GetDouble("Phi","deg");
      string Shape = blocks[i]->GetString("Shape");
      AddDetector(R,Theta,Phi,Shape);
    }
	else if(blocks[i]->HasTokenList(field)){
        cout << endl << "////  Spede " << i+1 <<  endl;
		string fieldName = blocks[i]->GetString("Field");
		ConstructEMField(fieldName);
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
void Spede::ConstructDetector(G4LogicalVolume* world){
  for (unsigned short i = 0 ; i < m_R.size() ; i++) {

    G4double wX = m_R[i] * sin(m_Theta[i] ) * cos(m_Phi[i] ) ;
    G4double wY = m_R[i] * sin(m_Theta[i] ) * sin(m_Phi[i] ) ;
    G4double wZ = m_R[i] * cos(m_Theta[i] ) ;
    G4ThreeVector Det_pos = G4ThreeVector(wX, wY, wZ) ;
    // So the face of the detector is at R instead of the middle
    Det_pos-=Det_pos.unit()*Spede_NS::Thickness*0.5;//minus because detector is in backwards angle
    // Building Detector reference frame
    G4double ii = cos(m_Theta[i]) * cos(m_Phi[i]);
    G4double jj = cos(m_Theta[i]) * sin(m_Phi[i]);
    G4double kk = -sin(m_Theta[i]);
    G4ThreeVector Y(ii,jj,kk);
    G4ThreeVector w = Det_pos.unit();
    G4ThreeVector u = w.cross(Y);
    G4ThreeVector v = w.cross(u);
    v = v.unit();
    u = u.unit();

    G4RotationMatrix* Rot = new G4RotationMatrix(u,v,w);

    if(m_Shape[i] == "Silicon")
    {
      double si_thickness = Spede_NS::Thickness;
      double pcb_thickness = 0.3;
     
      BuildPCB(si_thickness, pcb_thickness, Rot, Det_pos, world);
	  BuildChamber(world);

	  new G4PVPlacement(G4Transform3D(G4RotationMatrix(0.,0.,0.),Det_pos),
			  BuildSiliconDetector(),
			  "Spede",world,false,i+1);
    }
    else if(m_Shape[i] == "Foil")
    {
          //new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
          //    BuildFoil(0.012,Rot,Det_pos,world),"foil",world,false, i+1);
          
              BuildFoil(0.012,Rot,Det_pos,world);
    }
    else if(m_Shape[i] == "Ladder")
    {
          new G4PVPlacement(G4Transform3D(*Rot,Det_pos),
              BuildLadder(1),"ladder",world,false, i+1);
    }
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Spede::InitializeRootOutput(){
  RootOutput *pAnalysis = RootOutput::getInstance();
  TTree *pTree = pAnalysis->GetTree();
  if(!pTree->FindBranch("Spede")){
    pTree->Branch("Spede", "TSpedeData", &m_Event) ;
  }
  pTree->SetBranchAddress("Spede", &m_Event) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Spede::ReadSensitive(const G4Event* event)
{
	m_Event->Clear();

	G4THitsMap<G4double*>* PixelHitMap;
	std::map<G4int, G4double**>::iterator Pixel_itr;

	G4int PixelCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("SpedeScorer/SpedePixel");
	PixelHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(PixelCollectionID));

	//// Loop on the Pixel map
	for (Pixel_itr = PixelHitMap->GetMap()->begin() ; Pixel_itr != PixelHitMap->GetMap()->end() ; Pixel_itr++)
	{
		G4double* Info = *(Pixel_itr->second);
		double Energy = RandGauss::shoot(Info[0],Spede_NS::ResoEnergy);
		if(Energy>Spede_NS::EnergyThreshold){
			double Time = RandGauss::shoot(Info[1],Spede_NS::ResoTime);
			int DetectorNbr = (int) Info[7];
			int PixelNbr = (int) Info[8];
			m_Event->SetEnergy(DetectorNbr, PixelNbr, Energy);
			m_Event->SetTime(DetectorNbr, PixelNbr, Time); 

			// Interraction Coordinates
			ms_InterCoord->SetDetectedPositionX(Info[2]) ;
			ms_InterCoord->SetDetectedPositionY(Info[3]) ;
			ms_InterCoord->SetDetectedPositionZ(Info[4]) ;
			ms_InterCoord->SetDetectedAngleTheta(Info[5]/deg) ;
			ms_InterCoord->SetDetectedAnglePhi(Info[6]/deg) ;
		}
	}
	// clear map for next event
	PixelHitMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////   
void Spede::InitializeScorers() 
{ 
	// This check is necessary in case the geometry is reloaded
	bool already_exist = false; 
	m_SpedeScorer = CheckScorer("SpedeScorer",already_exist) ;

	if(already_exist) 
		return ;

	vector<G4double> PixelInnerRadius;
	vector<G4double> PixelOuterRadius;
	vector<G4double> PixelPhiStart;
	vector<G4double> PixelPhiStop;

	ifstream filename ("PixelLayout.txt");

	G4double innerRadius, outerRadius, phiStart, phiStop; 
	while (filename >> innerRadius >> outerRadius >> phiStart >> phiStop)
	{
		PixelInnerRadius.push_back(innerRadius);
		PixelOuterRadius.push_back(outerRadius);
		PixelPhiStart.push_back(phiStart);
		PixelPhiStop.push_back(phiStop);

		cout << innerRadius << " " << outerRadius << " " << phiStart << " " << phiStop << "\n";
	}

	cout << PixelInnerRadius.size() << " pixels defined" << endl;

	vector<G4double>* PixelInnerRadius_p = &PixelInnerRadius;
	vector<G4double>* PixelOuterRadius_p = &PixelOuterRadius;
	vector<G4double>* PixelPhiStart_p = &PixelPhiStart;
	vector<G4double>* PixelPhiStop_p = &PixelPhiStop;


	// Otherwise the scorer is initialised
	vector<int> level; level.push_back(0);

	G4VPrimitiveScorer* PixelScorer =
		new SILICONSCORERS::PS_Silicon_Pixel("SpedePixel",
				0,
				PixelInnerRadius_p,
				PixelOuterRadius_p,
				PixelPhiStart_p,
				PixelPhiStop_p,
				1);
	//G4VPrimitiveScorer* Calorimeter= new CALORIMETERSCORERS::PS_Calorimeter("Calorimeter",level, 0) ;
	//and register it to the multifunctionnal detector
	m_SpedeScorer->RegisterPrimitive(PixelScorer);
	G4SDManager::GetSDMpointer()->AddNewDetector(m_SpedeScorer) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* Spede::Construct(){
  return  (NPS::VDetector*) new Spede();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
  class proxy_nps_Spede{
    public:
      proxy_nps_Spede(){
        NPS::DetectorFactory::getInstance()->AddToken("Spede","Spede");
        NPS::DetectorFactory::getInstance()->AddDetector("Spede",Spede::Construct);
      }
  };

  proxy_nps_Spede p_nps_Spede;
}
