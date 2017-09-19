/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Daniel Cox  contact address: daniel.m.cox@jyu.fi         *
 *                                                                           *
 * Creation Date  : January 2017                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  Sage simulation                                     *
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
#include "G4Cons.hh"
#include "G4Polycone.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"

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
#include "Sage.hh"
#include "SiliconScorers.hh"
#include "RootOutput.h"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "NPOptionManager.h"
// CLHEP header
#include "CLHEP/Random/RandGauss.h"
// EM field
#include "SageHVField.hh"

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


using namespace std;
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace Sage_NS
{
	// Energy and time Resolution
	const double EnergyThreshold = 10.0*keV;
	const double ResoTime = 100.*ns ;
	const double ResoEnergy = 2.7*keV ;
	//const double Radius = 24*mm ;//Radius of SAGE detector
	//const double Width = 100*mm ;
	//const double Thickness = 1*mm ;//Thickness of SAGE detector
	const double SageRotation = 3.2*deg;
	//const string Material = "Silicon";
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Sage Specific Method
Sage::Sage()
{
	m_Event = new TSageData() ;
	m_SageScorer = 0;
	m_SiliconDetector = 0;

    fieldIsInitialized = false;
	G4String fieldFileName = "SageEMField.table";//TODO make this a user set variable
	fHVFieldStrength = 0.7;//TODO make this a user set variable
	//fField.Put(0);

	CarbonFoils = 0;
	InnerDiameter = 39.*mm;
	SiThickness = 1.*mm;

	// RGB Color + Transparency
	m_VisSilicon = new G4VisAttributes(G4Colour(0, 0.5, 0));   

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Sage::~Sage()
{
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Sage::AddDetector(G4ThreeVector POS, string  Shape)
{
	// Convert the POS value to R theta Phi as Spherical coordinate is easier in G4 
	m_R.push_back(POS.mag());
	m_Theta.push_back(POS.theta());
	m_Phi.push_back(POS.phi());
	m_Shape.push_back(Shape);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Sage::AddDetector(double  R, double  Theta, double  Phi, string  Shape)
{
	m_R.push_back(R);
	m_Theta.push_back(Theta);
	m_Phi.push_back(Phi);
	m_Shape.push_back(Shape);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* Sage::BuildSiliconDetector()
{
	if(!m_SiliconDetector)
	{
		G4Tubs* tub = new G4Tubs("SageSilicon",0,24.*mm,SiThickness*0.5,0,360*deg);

		G4Material* DetectorMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary("Si");
		m_SiliconDetector = new G4LogicalVolume(tub,DetectorMaterial,"logic_Sage_silicon",0,0,0);
		m_SiliconDetector->SetVisAttributes(m_VisSilicon);
		m_SiliconDetector->SetSensitiveDetector(m_SageScorer);

	}
	return m_SiliconDetector;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Sage::ConstructChamber(G4LogicalVolume* world)
{
	//Chamber
	G4ThreeVector ChamberPlacement (-sin(Sage_NS::SageRotation)*1.047*m,0.,cos(Sage_NS::SageRotation)*1.047*m);//Rotated
	//"magnet (don't acutally generate field)
	G4ThreeVector UpstreamCoilPlacement (-sin(Sage_NS::SageRotation)*85.5*mm,0.,cos(Sage_NS::SageRotation)*85.5*mm);//Rotated
	G4ThreeVector DownstreamCoilPlacement (0.,0., -39.0*mm);
	//Target chamber and wheel
	G4ThreeVector TargetChamberPlacement (0.,0.,2.*mm);
	G4ThreeVector TargetChamberBackPlatePlacement (0,0,-23.1*mm);
	G4ThreeVector TargetWheelPlacement (-33.75*mm,0,-0.86*mm);
	G4ThreeVector TWFPlacement (-(33.75+172/2-80)*mm,0.*mm,(-0.86+0.8+0.8)*mm);
	G4ThreeVector TWBPlacement ((-33.75+156/2-92)*mm,0.8*mm,(-0.86-0.7-0.6-14.)*mm);
	G4ThreeVector TWGearPlacement( -33.75*mm, 0., -0.86*mm-12.5/2*mm-0.8*mm);
	G4ThreeVector TWMotorPlacement( -105.*mm, -8.*mm, 5.*mm + 25./2*mm);
	//Support between target and main chamber
	G4ThreeVector ConnectorPlacement (-sin(Sage_NS::SageRotation)*91.*mm,0.,cos(Sage_NS::SageRotation)*91.*mm);//Rotated
	//Carbon foil unit
	G4ThreeVector CF1Placement (-sin(Sage_NS::SageRotation)*87.5*mm,0.,cos(Sage_NS::SageRotation)*87.5*mm);//Rotated
	G4ThreeVector CF2Placement (-sin(Sage_NS::SageRotation)*36.5*mm,0.,cos(Sage_NS::SageRotation)*36.5*mm);//Rotated
	G4ThreeVector CFTubePlacement (-sin(Sage_NS::SageRotation)*62.*mm,0.,cos(Sage_NS::SageRotation)*62.*mm);//Rotated  // TODO check distance
	//High voltagbe barrier (again doesn't generate the field)
	G4ThreeVector HVPlacement (-sin(Sage_NS::SageRotation)*605.04*mm,0.,cos(Sage_NS::SageRotation)*605.04*mm);//Rotated   // TODO check distance from OPERA3D

	G4RotationMatrix* yRot180deg = new G4RotationMatrix();
	yRot180deg->rotateY(180.*deg);

	G4RotationMatrix* yRot180degPlusSageAngle = new G4RotationMatrix();
	yRot180degPlusSageAngle->rotateY(180.*deg);
	yRot180degPlusSageAngle->rotateY(Sage_NS::SageRotation);

	G4RotationMatrix* SageAngle= new G4RotationMatrix();
	SageAngle->rotateY(Sage_NS::SageRotation);


	//G4Material* defaultMaterial             = MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
	G4Material* ChamberMaterial             = MaterialManager::getInstance()->GetMaterialFromLibrary("Al");//Al;
	//G4Material* TargetChamberVolumeMaterial = MaterialManager::getInstance()->GetMaterialFromLibrary("HeGas");//He;
	G4Material* CarbonFoilMaterial          = MaterialManager::getInstance()->GetMaterialFromLibrary("C");// C;
	G4Material* CarbonFoilTubeMaterial      = MaterialManager::getInstance()->GetMaterialFromLibrary("Al");// Al;
	//G4Material* HVInsulationMaterial        = MaterialManager::getInstance()->GetMaterialFromLibrary("Noryl");
	//G4Material* HVBarrierMaterial           = MaterialManager::getInstance()->GetMaterialFromLibrary("Stainless Steel");//Steel;
	G4Material* CoilMaterial                = MaterialManager::getInstance()->GetMaterialFromLibrary("Cu");//Cu;
	//G4Material* TargetMaterial              = MaterialManager::getInstance()->GetMaterialFromLibrary("Samarium");//Sm;
	//G4Material* TargetBackingMaterial       = MaterialManager::getInstance()->GetMaterialFromLibrary("Mylar");
	//G4Material* TargetFrontMaterial         = MaterialManager::getInstance()->GetMaterialFromLibrary("Acrylic");
	//G4Material* PECVDMaterial               = MaterialManager::getInstance()->GetMaterialFromLibrary("SiO2");
	G4int    ncomponents, natoms, z;
	G4double density, a;
	G4String name, symbol;

	a = 1.0078*g/mole;
	G4Element* elH  = new G4Element(name="Hydrogen",   symbol="H",  z=1., a);

	a = 12.00*g/mole;
	G4Element* elC  = new G4Element(name="Carbon",     symbol="C",  z=6., a);

	a = 51.9961*g/mole;
	G4Element* elCr = new G4Element(name="Chromium",   symbol="Cr", z=24., a);

	a = 55.845*g/mole;
	G4Element* elFe = new G4Element(name="Iron",       symbol="Fe", z=26., a);

	a = 58.6934*g/mole;
	G4Element* elNi = new G4Element(name="Nickel",     symbol="Ni", z=28., a);

	density = 8.00*g/cm3;
	G4Material* Steel = new G4Material(name="Steel", density, ncomponents=3);
	Steel->AddElement(elCr, natoms= 8);
	Steel->AddElement(elNi, natoms=18);
	Steel->AddElement(elFe, natoms=74);	
	G4Material* HVBarrierMaterial           = Steel;

	G4Material* CH2 						= new G4Material ("Polyethylene" , 1.08*g/cm3, 2);
	CH2->AddElement(elH,2);
	CH2->AddElement(elC,1);

	G4Material* HVInsulationMaterial = CH2;
	G4bool Overlap = false;
	/*------------------------------Shapes--------------------------------------*/
	/*----------------------------------Chamber----------------------------------*/

	G4Box* OuterChamber = new G4Box("Outer Chamber", 480.76/2*mm, 492.76/2*mm, 256.76/2*mm);
	G4Box* InnerChamber = new G4Box("Inner Chamber", 476./2*mm, 490./2*mm, 252./2*mm);
	G4Tubs* Aperature = new G4Tubs("Aperature", 0, 167./2*mm, 4.76/2*mm, 0., 360.*deg);
	G4Tubs* MagnetPipe = new G4Tubs("Magent Pipe", 148./2*mm, 167./2*mm, 567.2/2*mm, 0., 360.*deg);

	G4SubtractionSolid* SolidChamber = new G4SubtractionSolid ("OuterChamber-InnerChamber", OuterChamber, InnerChamber);
	G4ThreeVector AperaturePlacement(0.,0.,254.38/2*mm);
	G4SubtractionSolid* HoleChamber = new G4SubtractionSolid("Solidchamber-Aperature", SolidChamber,Aperature,0,AperaturePlacement);
	G4ThreeVector MagnetPipePlacement(0.,0.,411.98*mm);
	G4UnionSolid* Chamber = new G4UnionSolid("Chamber+Pipe",HoleChamber,MagnetPipe,0,MagnetPipePlacement);

	// --------------------------------- Upstream Coil -----------------------------------//

	// Define variables used for the polycone

	double* uscz = new double[8];  // 8 z-planes
	double* uscri = new double[8]; // inner radius of each z coord
	double* uscro = new double[8]; // outer radius of each z coord

	// Set dimensions, in mm
	uscz[0]  = 0.0; uscz[1] =  20.; uscz[2] = 20.0;  uscz[3] = 122.8; uscz[4] = 203.;
	uscz[5] = 227.; uscz[6] = 227.; uscz[7] = 245.;

	uscri[0] = 63./2;  uscri[1] = 63./2; uscri[2] = 63./2; uscri[3] = 63.2/2;
	uscro[0] = 117./2; uscro[1] = 117./2; uscro[2] = 113.5/2; uscro[3] = (((122.8-20.)/183)*(292-113.5)+113.5)/2.;

	uscri[4] = (((203.-122.8)/(227.-122.8))*(125.5-63.2)+63.2)/2.; uscri[5] = 125.5/2; uscri[6] = 221.5/2; uscri[7] = 221.5/2;
	uscro[4] = 292./2;                              uscro[5] = 292./2;  uscro[6] = 292./2;  uscro[7] = 292./2;


	G4VSolid* UpstreamCoil
		= new G4Polycone("UpstreamCoil", 0.*deg, 360.*deg,8,uscz,uscri,uscro);

	// --------------------------------- Downstream Coil -----------------------------------//

	// Define variables used for the polycone

	double* dscz = new double[7];  // 7 z-planes
	double* dscri = new double[7]; // inner radius of each z coord
	double* dscro = new double[7]; // outer radius of each z coord

	// Set dimensions, in mm
	dscz[0]  = 0.0;    dscz[1]  = 9.0;    dscz[2]  = 9.0;    dscz[3]  = 27.0;   dscz[4]  = 27.0;    dscz[5]  = 83.0;   dscz[6]  = 105.0;

	dscri[0] = 25.133; dscri[1] = 25.133; dscri[2] = 25.133; dscri[3] = 25.133; dscri[4] = 25.133;  dscri[5] = 25.133; dscri[6] = 25.133;
	dscro[0] = 49.54;  dscro[1] = 49.54;  dscro[2] = 67.54;  dscro[3] = 67.54;  dscro[4] = 102.343; dscro[5] = 182.46; dscro[6] = 182.46;

	G4VSolid* DownstreamCoil = new G4Polycone("DownstreamCoil", 0.*deg, 360.*deg,7,dscz,dscri,dscro);

	/*----------------------------------Carbon Foil Unit----------------------------------*/

	//CarbonFoil = new G4Tubs("CarbonFoil", 0.*mm, 30./2*mm, 0.217/2*um, 0., 360.*deg);//Had an outer radius of 30, changed to 31 though.
	G4Tubs* CarbonFoil = new G4Tubs("CarbonFoil", 0.*mm, 30./2*mm, 0.217/2*um, 0., 360.*deg);//Had an outer radius of 30, changed to 31 though.
	//CarbonFoil = new G4Tubs("CarbonFoil", 0.*mm, 31./2*mm, 0.00017/2*mm, 0., 360.*deg);//Had an outer radius of 30, changed to 31 though.
	G4Tubs* CarbonFoilTube = new G4Tubs("CarbonFoilTube",  31./2*mm, 38./2*mm, 50./2*mm, 0., 360.*deg);
	//G4Tubs* CarbonFoilTubeSolid = new G4Tubs("CarbonFoilTubeSolid",  0./2*mm, 38./2*mm, 500./2*mm, 0., 360.*deg);
	G4Tubs* CarbonFoilAlRing = new G4Tubs("CarbonFoilAlRing",        30./2*mm, 38./2*mm,1./2*mm,0.,360.*deg);
	//CarbonFoilAlRing = new G4Tubs("CarbonFoilAlRing",        30./2*mm, 38./2*mm,1./2*mm,0.,360.*deg);

	/*----------------------------------Target Chamber----------------------------------*/

	G4double TgtChamberL = (126.)*mm; // was 130 before, back plate takes 4 mm
	G4Tubs* TargetChamberBody = new G4Tubs("TargetChamberBody",194./2*mm,210./2*mm,TgtChamberL/2, 0., 360.*deg);
	G4VSolid* TargetChamberSub = new G4Box("TargetChamberSub",210/2*mm,(2*81)/2*mm,(112.5-16.5)/2*mm);
	G4Tubs* TargetChamberFarEnd = new G4Tubs("TargetChamberFarEnd",5./2*mm,199./2*mm,17./2*mm,0.,360.*deg);
	//TargetChamberVolume = new G4Tubs("TargetChamberVolume", 0.,200.02/2*mm,126.02/2*mm,0.,360.*deg);

	G4VSolid* TCSub = new G4Box("TCSub",40./2*mm, 6./2*mm, 95./2*mm);

	G4RotationMatrix* zRot14degW = new G4RotationMatrix();
	zRot14degW->rotateZ((14.5)*deg);

	G4RotationMatrix* zRotm14degW = new G4RotationMatrix();
	zRotm14degW->rotateZ((-14.5)*deg);

	G4ThreeVector TCSubPlacement1((53.5)/2*mm,(210.)/2-6.*mm,0.);
	G4ThreeVector TCSubPlacement2(-(53.5)/2*mm,(210.)/2-6.*mm,0.);
	G4ThreeVector TCSubPlacement3((53.5)/2*mm,-(210.)/2+6.*mm,0.);
	G4ThreeVector TCSubPlacement4(-(53.5)/2*mm,-(210.)/2+6.*mm,0.);

	G4ThreeVector TargetChamberFarEndPlacement(0.,0.,(126.)/2*mm);

	G4UnionSolid* TargetChamberClosed
		= new G4UnionSolid("TargetChamberClosed",TargetChamberBody,TargetChamberFarEnd,0,TargetChamberFarEndPlacement);

	G4SubtractionSolid* TargetChamber_doors
		= new G4SubtractionSolid("TargetChamber_doors",TargetChamberClosed,TargetChamberSub,0,G4ThreeVector(0.,0.,0.));

	G4SubtractionSolid* TC1
		= new G4SubtractionSolid("TC1",TargetChamber_doors,TCSub,zRot14degW,TCSubPlacement1);
	G4SubtractionSolid* TC2
		= new G4SubtractionSolid("TC2",TC1,TCSub,zRotm14degW,TCSubPlacement2);
	G4SubtractionSolid* TC3
		= new G4SubtractionSolid("TC3",TC2,TCSub,zRot14degW,TCSubPlacement3);
	G4SubtractionSolid* TC4
		= new G4SubtractionSolid("TC4",TC3,TCSub,zRotm14degW,TCSubPlacement4);

	G4VSolid* ConnectorTube = new G4Tubs("CarbonFoilTubeSolid",  0./2*mm, 49./2*mm, 500./2*mm, 0., 360.*deg);//Stops an overlap withe the start of the connector

	G4SubtractionSolid* TargetChamber
		= new G4SubtractionSolid("TargetChamber",TC4,ConnectorTube,SageAngle,G4ThreeVector(-3.6,0.,63.9));

	/*----------------------------------Target Chamber Doors----------------------------*/

	G4int nmbRZ = 8;

	G4double r[] = {0-64.,
		11.-64.,
		11.001-64.,
		54.5-64.,
		73.5-64.,
		117.-64.,
		117.001-64.,
		128.-64.};
	G4double z1[] = {105.1,
		105.1,
		115.,
		127.,
		127.,
		115.,
		105.1,
		105.1};
	G4double z2[] = {109.,
		109.,
		119.,
		131.,
		131.,
		119.,
		109.,
		109.};

	G4VSolid* TCDoorPlate = new G4Polycone(
			"TCDoorPlate",
			-54.7*deg,// start angle phi
			109.4*deg,// total angle phi
			nmbRZ,// Numbers of corners in the r,z space
			r,
			z1,
			z2);

	G4VSolid* DoorEnd1 = new G4Box(
			"DoorEnd1",
			26./2*mm,
			4./2*mm,
			106./2*mm);

	G4VSolid* DoorSub = new G4Box(
			"DoorSub",
			26./2*mm,
			5./2*mm,
			106./2*mm);

	G4VSolid* DoorEnd4 = new G4Box(
			"DoorEnd4",
			4./2*mm,
			12./2*mm,
			128./2*mm);

	G4RotationMatrix* DoorSubRot = new G4RotationMatrix();
	DoorSubRot->rotateY(15.*deg);
	G4RotationMatrix* DoorSubRot1 = new G4RotationMatrix();
	DoorSubRot1->rotateY(-15.*deg);

	G4RotationMatrix* DoorRot = new G4RotationMatrix();
	DoorRot->rotateZ(-54.7*deg);
	G4RotationMatrix* DoorRot1 = new G4RotationMatrix();
	DoorRot1->rotateZ(54.7*deg);

	G4SubtractionSolid* DoorEnd2
		= new G4SubtractionSolid("DoorEnd2",DoorEnd1,DoorSub, DoorSubRot, G4ThreeVector(13*mm,0.*mm,(53.+9.)*mm));

	G4SubtractionSolid* DoorEnd3
		= new G4SubtractionSolid("DoorEnd3",DoorEnd2,DoorSub, DoorSubRot1, G4ThreeVector(13*mm,0.*mm,-(53.+9.)*mm));

	G4UnionSolid* DoorEnd5
		= new G4UnionSolid("DoorEnd5",DoorEnd3,DoorEnd4,0,G4ThreeVector(-11*mm,(6.3)*mm,0.*mm));

	G4UnionSolid* DoorEnd6
		= new G4UnionSolid("DoorEnd6",DoorEnd3,DoorEnd4,0,G4ThreeVector(-11*mm,-(6.3)*mm,0.*mm));

	G4UnionSolid* TCDoor1
		= new G4UnionSolid("TCDoor1", TCDoorPlate, DoorEnd5, DoorRot, G4ThreeVector((67+.1)*mm,(110-13.+.1)*mm,0.*mm));

	G4UnionSolid* TCDoor
		= new G4UnionSolid("TCDoor", TCDoor1, DoorEnd6, DoorRot1, G4ThreeVector((67+.1)*mm,-(110-13.+.1)*mm,0.*mm));


	/* ----------Target chamber back plate - dwg part no. np47-01-22 -----------*/
	// Material: Al alloy

	// Define variables used for the polycone
	double* Z  = new double[14];  // 14 z-planes, simplified a bit
	double* ri = new double[14]; // inner radius of each z coord
	double* ro = new double[14]; // outer radius of each z coord

	// Set dimensions, in mm
	Z[0] =  0.0;  Z[1] =  7.0;  Z[2] =  7.0;  Z[3] = 12.0;  Z[4] = 12.0;  Z[5] = 19.0;  Z[6] = 19.0;  Z[7] = 22.0;  Z[8] = 22.0;  Z[9] = 25.0;  Z[10] = 25.0;  Z[11] = 38.0;  Z[12] = 38.0;  Z[13] = 42.0;
	ro[0] = 39.0; ro[1] = 39.0; ro[2] = 56.0; ro[3] = 64.0; ro[4] = 64.0; ro[5] = 64.0; ro[6] = 76.5; ro[7] = 76.5; ro[8] = 76.5; ro[9] = 76.5; ro[10] = 82.0; ro[11] = 82.0; ro[12] = 98.5; ro[13] = 98.5;
	ri[0] = 15.0; ri[1] = 15.0; ri[2] = 20.5; ri[3] = 20.5; ri[4] = 56.0; ri[5] = 56.;  ri[6] = 56.0; ri[7] = 56.0; ri[8] = 73.0; ri[9] = 73.0; ri[10] = 73.0; ri[11] = 73.0; ri[12] = 73.0; ri[13] = 73.0;

	G4VSolid* TgtChamberBackPlate
		= new G4Polycone("TargetChamberBackPlate", 0.*deg, 360.*deg,14,Z,ri,ro);


	/* -------------  Target wheel - dwg part no. np??????   --------------------*/
	// Material: ? set to Al alloy

	//G4VSolid* TgtWheelMain = new G4Tubs("TgtWheelMain", 30./2*mm, 105./2*mm, 14./2*mm, 0., 360.*deg);reduced thickness of the wheel by factor of ten
	G4VSolid* TgtWheelMain = new G4Tubs("TgtWheelMain", 30./2*mm, 105./2*mm, 1.6/2*mm, 0., 360.*deg);
	G4VSolid* TgtWheelHole = new G4Tubs("TgtWheelHole", 0*mm, 19./2*mm, 15./2*mm, 0., 360.*deg);
	G4VSolid* TgtWheelHole2Circle = new G4Tubs("TgtWheelHole2Circle", 0*mm, 10./2*mm, 15./2*mm, 0., 360.*deg);
	G4VSolid* TgtWheelHole2Box = new G4Box("TgtWheelHole2Box", 5./2*mm, 10./2*mm, 15./2*mm);

	// Target frame
	G4VSolid* TgtFrameBox = new G4Box("TgtFrameBox",20./2*mm, 25./2*mm, 0.6/2*mm);
	G4VSolid* TgtFrameCircle = new G4Tubs("TgtFrameCircle", 0*mm, 15./2*mm, 1./2*mm, 0., 360.*deg);
	G4SubtractionSolid* TgtFrame
		= new G4SubtractionSolid("TgtWheel", TgtFrameBox,TgtFrameCircle,0,G4ThreeVector(0,0,0));

	// Target wheel
	G4UnionSolid* TgtWheelHole2uni
		= new G4UnionSolid("TgtWheelHole2uni", TgtWheelHole2Box,TgtWheelHole2Circle,0,G4ThreeVector(2.6*mm,0,0));

	G4UnionSolid* TgtWheelHole2uni2
		= new G4UnionSolid("TgtWheelHole2uni2", TgtWheelHole2uni,TgtWheelHole2Circle,0,G4ThreeVector(-2.6*mm,0,0));

	G4double hole_radius = 33.75*mm;
	//G4ThreeVector TgtWheelHolePlacement(33.75*mm,0.,0);

	G4SubtractionSolid* TgtWheel
		= new G4SubtractionSolid("TgtWheel", TgtWheelMain,TgtWheelHole,0,
				G4ThreeVector(hole_radius*mm,0,0));
	G4SubtractionSolid* TgtWheel2
		= new G4SubtractionSolid("TgtWheel2", TgtWheel,TgtWheelHole,0,
				G4ThreeVector(-hole_radius*mm,0,0));
	G4SubtractionSolid* TgtWheel3
		= new G4SubtractionSolid("TgtWheel3", TgtWheel2,TgtWheelHole,0,
				G4ThreeVector(hole_radius*cos(60*deg)*mm,hole_radius*sin(60*deg)*mm,0));
	G4SubtractionSolid* TgtWheel4
		= new G4SubtractionSolid("TgtWheel4", TgtWheel3,TgtWheelHole,0,
				G4ThreeVector(hole_radius*cos(120*deg)*mm,hole_radius*sin(120*deg)*mm,0));
	G4SubtractionSolid* TgtWheel5
		= new G4SubtractionSolid("TgtWheel5", TgtWheel4,TgtWheelHole,0,
				G4ThreeVector(hole_radius*cos(240*deg)*mm,hole_radius*sin(240*deg)*mm,0));
	G4SubtractionSolid* TgtWheel6
		= new G4SubtractionSolid("TgtWheel6", TgtWheel5,TgtWheelHole,0,
				G4ThreeVector(hole_radius*cos(300*deg)*mm,hole_radius*sin(300*deg)*mm,0));

	G4RotationMatrix* zRot30degW = new G4RotationMatrix();
	zRot30degW->rotateZ((30.)*deg);
	G4RotationMatrix* zRot90degW = new G4RotationMatrix();
	zRot90degW->rotateZ((90.)*deg);
	G4RotationMatrix* zRot150degW = new G4RotationMatrix();
	zRot150degW->rotateZ((150.)*deg);
	G4RotationMatrix* zRot210degW = new G4RotationMatrix();
	zRot210degW->rotateZ((210.)*deg);
	G4RotationMatrix* zRot270degW = new G4RotationMatrix();
	zRot270degW->rotateZ((270.)*deg);
	G4RotationMatrix* zRot330degW = new G4RotationMatrix();
	zRot330degW->rotateZ((330.)*deg);


	G4SubtractionSolid* TgtWheel7
		= new G4SubtractionSolid("TgtWheel7", TgtWheel6,TgtWheelHole2uni2,zRot330degW,
				G4ThreeVector(hole_radius*cos(30*deg)*mm,hole_radius*sin(30*deg)*mm,0));
	G4SubtractionSolid* TgtWheel8
		= new G4SubtractionSolid("TgtWheel8", TgtWheel7,TgtWheelHole2uni2,zRot90degW,
				G4ThreeVector(hole_radius*cos(90*deg)*mm,hole_radius*sin(90*deg)*mm,0));
	G4SubtractionSolid* TgtWheel9
		= new G4SubtractionSolid("TgtWheel9", TgtWheel8,TgtWheelHole2uni2,zRot30degW,
				G4ThreeVector(hole_radius*cos(150*deg)*mm,hole_radius*sin(150*deg)*mm,0));
	G4SubtractionSolid* TgtWheel10
		= new G4SubtractionSolid("TgtWheel10", TgtWheel9,TgtWheelHole2uni2,zRot330degW,
				G4ThreeVector(hole_radius*cos(210*deg)*mm,hole_radius*sin(210*deg)*mm,0));
	G4SubtractionSolid* TgtWheel11
		= new G4SubtractionSolid("TgtWheel11", TgtWheel10,TgtWheelHole2uni2,zRot90degW,
				G4ThreeVector(hole_radius*cos(270*deg)*mm,hole_radius*sin(270*deg)*mm,0));
	G4SubtractionSolid* TgtWheel12
		= new G4SubtractionSolid("TgtWheel12", TgtWheel11,TgtWheelHole2uni2,zRot30degW,
				G4ThreeVector(hole_radius*cos(330*deg)*mm,hole_radius*sin(330*deg)*mm,0));

	/*-----------------------------Target Wheel support-----------------------------*/
	G4VSolid* TWF1 = new G4Box("TWF1", 172./2*mm, 150./2*mm, 1.6/2*mm);//Main plate
	G4VSolid* TWF2 = new G4Tubs("TWF2", 0*mm, 106.5/2*mm, 1.6*mm, 0., 360.*deg);//hole
	G4VSolid* TWF3 = new G4Box("TWF3", 100/2*mm, 132/2*mm, 1.6*mm);//subtraction for missing bits
	G4VSolid* TWF4 = new G4Box("TWF4", 150/2*mm, 100/2*mm, 1.6*mm);//subtraction for missing bits

	G4SubtractionSolid* TWF5
		= new G4SubtractionSolid("TWF4", TWF1,TWF2,0,
				G4ThreeVector((172/2-80)*mm,(0)*mm,0.));

	G4SubtractionSolid* TWF6
		= new G4SubtractionSolid("TWF5", TWF5,TWF3,0,
				G4ThreeVector((172/2+100/2-16)*mm,(0)*mm,0.));

	G4RotationMatrix* zRotm40degW = new G4RotationMatrix();
	zRotm40degW->rotateZ((-40.)*deg);

	G4RotationMatrix* zRot40degW = new G4RotationMatrix();
	zRot40degW->rotateZ((40.)*deg);

	G4RotationMatrix* zRotm20degW = new G4RotationMatrix();
	zRotm20degW->rotateZ((-20.)*deg);

	G4RotationMatrix* zRot20degW = new G4RotationMatrix();
	zRot20degW->rotateZ((20.)*deg);

	G4SubtractionSolid* TWF7
		= new G4SubtractionSolid("TWF7", TWF6,TWF4,zRotm40degW,
				G4ThreeVector((-172/2-100/2+42)*mm,(150/2)*mm,0.));

	G4SubtractionSolid* TWF8
		= new G4SubtractionSolid("TWF8", TWF7,TWF4,zRot40degW,
				G4ThreeVector((-172/2-100/2+42)*mm,(-150/2)*mm,0.));

	G4SubtractionSolid* TWF9
		= new G4SubtractionSolid("TWF9", TWF8,TWF4,zRot20degW,
				G4ThreeVector((-172/2-100/2-3)*mm,(158/2)*mm,0.));

	G4SubtractionSolid* TWF
		= new G4SubtractionSolid("TWB", TWF9,TWF4,zRotm20degW,
				G4ThreeVector((-172/2-100/2-3)*mm,(-158/2)*mm,0.));

	G4VSolid* TWB1 = new G4Box("TWB1",156./2*mm,123./2*mm,1.6/2*mm);//Main plate
	G4VSolid* TWB2 = new G4Tubs("TWF2", 0*mm, 85./2*mm, 1.6*mm, 0., 360.*deg);//hole
	G4VSolid* TWB3 = new G4Box("TWB3", 150/2*mm, 100/2*mm, 1.6*mm);//subtraction for missing bits

	G4SubtractionSolid* TWB4
		= new G4SubtractionSolid("TWB4", TWB1,TWB2,0,
				G4ThreeVector((156/2-64)*mm,(0)*mm,0.));

	G4SubtractionSolid* TWB5
		= new G4SubtractionSolid("TWB5", TWB4,TWB3,zRotm40degW,
				G4ThreeVector((-156/2-100/2+42)*mm,(150/2)*mm,0.));

	G4SubtractionSolid* TWB6
		= new G4SubtractionSolid("TWB6", TWB5,TWB3,zRot40degW,
				G4ThreeVector((-156/2-100/2+42)*mm,(-150/2)*mm,0.));

	G4SubtractionSolid* TWB7
		= new G4SubtractionSolid("TWB7", TWB6,TWB3,zRot20degW,
				G4ThreeVector((-156/2-100/2-3)*mm,(158/2)*mm,0.));

	G4SubtractionSolid* TWB8
		= new G4SubtractionSolid("TWB8", TWB7,TWB3,zRotm20degW,
				G4ThreeVector((-156/2-100/2-3)*mm,(-158/2)*mm,0.));

	G4SubtractionSolid* TWB
		= new G4SubtractionSolid("TWB", TWB8,TWB3,0,
				G4ThreeVector((-156/2-150/2+3)*mm,0.*mm,0.));

	// Ring gear between the plates where main wheel is attached
	// see assembly drawing no. NP47-01-755
	G4VSolid* TWGear = new G4Tubs("TWGear", 85./2*mm, 105.5/2*mm, 12.5*mm/2., 0., 360.*deg);

	// Target wheel motor
	G4VSolid* TWMotorCylinder = new G4Tubs("TWMotorCylinder", 0., 25./2*mm, 28.5*mm/2., 0., 360.*deg);
	G4VSolid* TWMotorAxle = new G4Tubs("TWMotorAxle", 0., 8./2*mm, 20./2.*mm, 0., 360.*deg);
	G4VSolid* TWMotorBase = new G4Box("TWMotorBase", 25./2.*mm, 25./2*mm, 2.5*mm/2.);

	G4ThreeVector TWMotorBasePlacement(0.,0.,-28.5*mm/2.-2.5/2.*mm);
	G4ThreeVector TWMotorAxlePlacement(0.,0.,-28.5*mm/2.-2.5/2.*mm-20./2.*mm);

	G4UnionSolid* TWMotor1 = new G4UnionSolid("TWMotor1", TWMotorCylinder, TWMotorBase, 0, TWMotorBasePlacement);
	G4UnionSolid* TWMotor = new G4UnionSolid("TWMotor", TWMotor1, TWMotorAxle, 0, TWMotorAxlePlacement);


	/*----------------------------------Target----------------------------------*/
	//G4Tubs* TargetBacking = new G4Tubs("TargetBacking", 0., 25.4*mm/2, 6.4*um/2.,0.,360.*deg);
	//G4Tubs* TargetFront = new G4Tubs("TargetFront", 0., 25.4*mm/2, 1.695*um/2, 0., 360.*deg);//200ug/cm^3 thick acrylic

	//G4Tubs* RVTargetBacking = new G4Tubs("RVTargetBacking", 0., 26*mm/2, 6.4*mm/2.,0.,360.*deg);
	//G4Tubs* RVTargetFront = new G4Tubs("RVTargetFront", 0., 26*mm/2, 1.695*mm/2, 0., 360.*deg);

	//taking density of Sm to be 7.52g/cm3 and a surface density of 1.5mg/cm2
	//Target= new G4Tubs("Target", 0., 25.4*mm/2, 1.99*um/2.,0.,360.*deg);
	//G4Tubs* Target= new G4Tubs("Target", 0., 25.4*mm/2, 0.3*um/2.,0.,360.*deg);
	//G4Tubs* RVTarget= new G4Tubs("RVTarget", 0., 26*mm/2, 0.58*um/2.,0.,360.*deg);
	/*----------------------------------Connector----------------------------------*/
	// ORIGINALS
	//G4Tubs* Connector1 = new G4Tubs("Connector1", 39./2*mm, 47./2*mm,48.5/2*mm,0.,360.*deg);
	//G4Tubs* Connector2 = new G4Tubs("Connector2", 44./2*mm, 63./2*mm,115.5/2*mm,0.,360.*deg);
	//G4Cons* Connector3 = new G4Cons("Connector3", 100./2*mm, 108./2*mm, 44./2*mm, 63./2*mm, 119.5/2*mm, 0., 360.*deg);
	//G4Tubs* Connector4 = new G4Tubs("Connector4", 109./2*mm, 148./2*mm, 2*mm, 0., 360.*deg);

	G4Tubs* Connector1 = new G4Tubs("Connector1", InnerDiameter/2, (InnerDiameter+8.*mm)/2,48.5/2*mm,0.,360.*deg);
	G4Tubs* Connector2 = new G4Tubs("Connector2", (InnerDiameter+5.*mm)/2, (InnerDiameter+24*mm)/2,115.5/2*mm,0.,360.*deg);
	G4Cons* Connector3 = new G4Cons("Connector3", 100./2*mm, 108./2*mm, (InnerDiameter+5.*mm)/2, (InnerDiameter+24.*mm)/2, 119.5/2*mm, 0., 360.*deg);
	G4Tubs* Connector4 = new G4Tubs("Connector4", 109./2*mm, 148./2*mm, 2*mm, 0., 360.*deg);

	G4ThreeVector Connector2Placement(0.,0.,-164./2*mm);
	G4ThreeVector Connector3Placement(0.,0.,-199.5*mm);
	G4ThreeVector Connector4Placement(0.,0.,-259.75*mm);
	G4UnionSolid* ConnectorPartOne = new G4UnionSolid("ConnectorPartOne", Connector1, Connector2, 0, Connector2Placement);
	G4UnionSolid* ConnectorPartTwo = new G4UnionSolid("ConnectorPartTwo", ConnectorPartOne, Connector3, 0, Connector3Placement);
	G4UnionSolid* Connector = new G4UnionSolid("Connector", ConnectorPartTwo, Connector4, 0, Connector4Placement);


	/*----------------------------------High Voltage Barrier----------------------------------*/

	G4Tubs* HVOuterSleeve = new G4Tubs("HVOuterSleeve", 142./2*mm, 146./2*mm, 150./2*mm, 0., 360.*deg);
	G4Tubs* HVInsulation = new G4Tubs("HVInsulation", 127./2*mm, 142./2*mm, 160./2*mm, 0., 360.*deg);
	G4Tubs* HVBarrier = new G4Tubs("HVBarrier", 103./2*mm, 127./2*mm, 110./2*mm, 0., 360.*deg);
	G4Tubs* HVHorseShoe = new G4Tubs("HVHorseShoe", 96./2*mm, 103./2*mm, 12./2*mm, -180.*deg, 259.*deg);
	//G4Tubs* HVFieldVolume = new G4Tubs("HVFieldVolume", 0./2*mm, 148./2*mm, 210./2*mm, 0.*deg, 360.*deg);


	/*----------------------------------High Voltage Field Volume----------------------------------*/
	//G4SubtractionSolid* HV1 = new G4SubtractionSolid("HV1", HVFieldVolume, HVOuterSleeve, 0, G4ThreeVector(0.,0.,0.));
	//G4SubtractionSolid* HV2 = new G4SubtractionSolid("HV2", HV1, HVInsulation, 0, G4ThreeVector(0.,0.,0.));
	//G4SubtractionSolid* HV3 = new G4SubtractionSolid("HV3", HV2, HVBarrier, 0, G4ThreeVector(0.,0.,0.));
	//G4SubtractionSolid* HVFieldVolumeTrimmed = new G4SubtractionSolid("HV4", HV3, HVHorseShoe, 0, G4ThreeVector(0.,0.,0.));

	/*-------------------------------Logical Volumes--------------------------------------*/

	/*----------------------------------Chamber----------------------------------*/

	G4LogicalVolume* Chamber_log = new G4LogicalVolume(Chamber, ChamberMaterial, "Chamber_log");

	// ---------------------------------Coils---------------------------------------//
	G4LogicalVolume* UpstreamCoil_log
		= new G4LogicalVolume(UpstreamCoil, CoilMaterial, "UpstreamCoil_log");

	G4LogicalVolume* DownstreamCoil_log
		= new G4LogicalVolume(DownstreamCoil, CoilMaterial, "DownstreamCoil_log");

	/*----------------------------------Target Chamber & Volume----------------------------------*/

	G4LogicalVolume* TargetChamber_log = new G4LogicalVolume(TargetChamber, ChamberMaterial, "TargetChamber_log");

	G4LogicalVolume* TgtChamberBackPlate_log
		= new G4LogicalVolume(TgtChamberBackPlate,ChamberMaterial,"TargetChamberBackPlate _log");

	G4LogicalVolume* TCDoor_log
		=new G4LogicalVolume(TCDoor,ChamberMaterial,"TCDoor_log");

	/* -------------  Target wheel - dwg part no. np??????   --------------------*/
	G4LogicalVolume* TgtWheel_log
		= new G4LogicalVolume(TgtWheel12, ChamberMaterial, "TgtWheel_log");

	G4LogicalVolume* TgtFrame_log
		= new G4LogicalVolume(TgtFrame, ChamberMaterial, "TgtFrame_log");

	G4LogicalVolume* TWF_log
		= new G4LogicalVolume(TWF, ChamberMaterial, "TWF_log");

	G4LogicalVolume* TWB_log
		= new G4LogicalVolume(TWB, ChamberMaterial, "TWB_log");

	G4LogicalVolume* TWGear_log
		= new G4LogicalVolume(TWGear, ChamberMaterial, "TWGear_log");

	G4LogicalVolume* TWMotor_log
		= new G4LogicalVolume(TWMotor, HVBarrierMaterial, "TWMotor_log");


	/*----------------------------------Target----------------------------------*/
	//G4LogicalVolume* TargetBacking_log = new G4LogicalVolume(TargetBacking, TargetBackingMaterial, "TargetBacking_log");
	//G4LogicalVolume* TargetFront_log = new G4LogicalVolume(TargetFront, TargetFrontMaterial, "TargetFront_log");
	//G4LogicalVolume* Target_log = new G4LogicalVolume(Target, TargetMaterial, "Target_log");


	/*----------------------------------Connector----------------------------------*/

	G4LogicalVolume* Connector_log = new G4LogicalVolume(Connector, ChamberMaterial, "Connector_log");

	/*----------------------------------Carbon Foil Unit----------------------------------*/

	G4LogicalVolume* CarbonFoil_log = new G4LogicalVolume(CarbonFoil, CarbonFoilMaterial, "CarbonFoil_log");
	G4LogicalVolume* CarbonFoilTube_log = new G4LogicalVolume(CarbonFoilTube, CarbonFoilTubeMaterial, "CarbonFoilTube_log");
	G4LogicalVolume* CarbonFoilAlRing_log = new G4LogicalVolume(CarbonFoilAlRing, CarbonFoilTubeMaterial, "CarbonFoilAlRing_log");

	/*----------------------------------High Voltage Barrier----------------------------------*/

	G4LogicalVolume* HVOuterSleeve_log = new G4LogicalVolume(HVOuterSleeve, ChamberMaterial, "HVOuterSleeve_log");
	G4LogicalVolume* HVInsulation_log = new G4LogicalVolume(HVInsulation, HVInsulationMaterial, "HVInsulation_log");
	G4LogicalVolume* HVBarrier_log = new G4LogicalVolume(HVBarrier, HVBarrierMaterial, "HVBarrier_log");
	G4LogicalVolume* HVHorseShoe_log = new G4LogicalVolume(HVHorseShoe, HVBarrierMaterial, "HVHorseShoe_log");

	/*--------------------------Physical Volumes-------------------------------*/

	/*----------------------------------Chamber----------------------------------*/
	new G4PVPlacement(yRot180degPlusSageAngle,
			ChamberPlacement,
			Chamber_log,
			"Chamber_phys",
			world,
			false, 0, Overlap);

	/*----------------------------------Coils----------------------------------*/

//	new G4PVPlacement(SageAngle,
//			UpstreamCoilPlacement,
//			UpstreamCoil_log,
//			"UpstreamCoil_phys",
//			world,
//			false, 0, Overlap);

	new G4PVPlacement(yRot180deg,
			DownstreamCoilPlacement,
			DownstreamCoil_log,
			"DownstreamCoil_phys",
			world,
			false, 0, Overlap);

	/*----------------------------------Target Chamber & Volume & wheel----------------------------------*/
	new G4PVPlacement(0,
			TargetChamberPlacement,
			TargetChamber_log,
			"TargetChamber_phys",
			world,
			false, 0, Overlap);

	new G4PVPlacement(yRot180deg,
			TargetChamberBackPlatePlacement,
			TgtChamberBackPlate_log,
			"TargetChamberBackPlate",
			world,
			false, 0,Overlap);

	new G4PVPlacement(0,
			TargetWheelPlacement,
			TgtWheel_log,
			"TgtWheel",
			world,
			false, 0,Overlap);

	new G4PVPlacement(0,
			TWFPlacement,
			TWF_log,
			"TWF",
			world,
			false, 0,Overlap);

	new G4PVPlacement(0,
			TWBPlacement,
			TWB_log,
			"TWB",
			world,
			false, 0,Overlap);

	new G4PVPlacement(0,
			TWGearPlacement,
			TWGear_log,
			"TWGear",
			world,
			false, 0,Overlap);

	G4RotationMatrix* TWMotorRot = new G4RotationMatrix();
	TWMotorRot->rotateX(-90.*deg);

	new G4PVPlacement(TWMotorRot,
			TWMotorPlacement,
			TWMotor_log,
			"TWMotor",
			world,
			false, 0,Overlap);

	new G4PVPlacement(0,
			TargetChamberPlacement,
			TCDoor_log,
			"TC_door_phys",
			world,
			false, 0, Overlap);

	new G4PVPlacement(yRot180deg,
			TargetChamberPlacement,
			TCDoor_log,
			"TC_door_phys",
			world,
			false, 1, Overlap);



	/*----------------------------------Connector----------------------------------*/

	new G4PVPlacement(yRot180degPlusSageAngle,
			ConnectorPlacement,
			Connector_log,
			"Connector",
			world,
			false, 0, Overlap);

	/*----------------------------------Carbon Foil Unit----------------------------------*/

	if (CarbonFoils==1) 
	{
		G4cout << "Carbon foils in" << endl;
		new G4PVPlacement(SageAngle,
				CF1Placement,
				CarbonFoil_log,
				"CarbonFoilNear",
				world,
				false, 0, Overlap);
		new G4PVPlacement(SageAngle,
				CF2Placement,
				CarbonFoil_log,
				"CarbonFoilFar",
				world,
				false, 0, Overlap);
		new G4PVPlacement(SageAngle,
				CF1Placement,
				CarbonFoilAlRing_log,
				"CarbonFoilAlRingNear",
				world,
				false, 0, Overlap);
		new G4PVPlacement(SageAngle,
				CF2Placement,
				CarbonFoilAlRing_log,
				"CarbonFoilAlRingFar",
				world,
				false, 0, Overlap);
		new G4PVPlacement(SageAngle,
				CFTubePlacement,
				CarbonFoilTube_log,
				"CarbonFoilTube",
				world,
				false, 0, Overlap);
	}

	/*----------------------------------High Voltage Barrier----------------------------------*/
	new G4PVPlacement(SageAngle,
			HVPlacement,
			HVOuterSleeve_log,
			"HVOuterSleeve",
			world,
			false, 0, Overlap);
	new G4PVPlacement(SageAngle,
			HVPlacement,
			HVInsulation_log,
			"HVInsulation",
			world,
			false, 0, Overlap);
	new G4PVPlacement(SageAngle,
			HVPlacement,
			HVBarrier_log,
			"HVBarrier",
			world,
			false, 0, Overlap);
	new G4PVPlacement(SageAngle,
			HVPlacement,
			HVHorseShoe_log,
			"HVHorseShoe",
			world,
			false, 0, Overlap);



	/*--------------------------------------------------------------------Colouring--------------------------------------------------------------------*/
	/*Setting the colour of Detectors*/

	G4VisAttributes *greyVA = new G4VisAttributes(G4Colour(0.94,1.0,1.0,0.3));
	G4VisAttributes *redVA = new G4VisAttributes(G4Colour(1.,0.5,0.5));
	G4VisAttributes *blackVA = new G4VisAttributes(G4Colour(0.,0.,0.));
	G4VisAttributes *blueVA = new G4VisAttributes(G4Colour(0.4,0.6,1.0));
	G4VisAttributes *orangeVA = new G4VisAttributes(G4Colour(0.8,0.4,3.));
	G4VisAttributes *cyanVA = new G4VisAttributes(G4Colour(0.5,1.0,1.0));
	G4VisAttributes *amberVA = new G4VisAttributes(G4Colour(1.0,0.49,0.3));


	/*Detector chamber up to the connecter to the target chamber*/
	Chamber_log -> SetVisAttributes(greyVA);
	//Chamber_log->SetVisAttributes (G4VisAttributes::Invisible);

	/*Magnet coils*/
	UpstreamCoil_log-> SetVisAttributes(amberVA);
	DownstreamCoil_log-> SetVisAttributes(amberVA);

	/*Target chamber*/
	//TargetChamber_log -> SetVisAttributes(greyVA);
	TargetChamber_log->SetVisAttributes (G4VisAttributes::Invisible);

	/*Back plate of the target chamber*/
	//TgtChamberBackPlate_log -> SetVisAttributes(blackVA);
	TgtChamberBackPlate_log -> SetVisAttributes(greyVA);
	//TgtChamberBackPlate_log->SetVisAttributes (G4VisAttributes::Invisible);

	/*Target wheel*/
	//TgtWheel_log->SetVisAttributes(blueVA);
	TgtWheel_log->SetVisAttributes (G4VisAttributes::Invisible);

	/*Target frame*/
	TgtFrame_log-> SetVisAttributes(redVA);
	//TgtFrame_log->SetVisAttributes (G4VisAttributes::Invisible);

	/*Connecter from detector chamber to target chamber*/
	Connector_log -> SetVisAttributes(greyVA);
	//Connector_log->SetVisAttributes (G4VisAttributes::Invisible);

	/*Carbon foil unit, foils, and Al rings*/
	CarbonFoil_log -> SetVisAttributes(blackVA);
	CarbonFoilTube_log -> SetVisAttributes(greyVA);
	//CarbonFoil_log->SetVisAttributes (G4VisAttributes::Invisible);
	//CarbonFoilTube_log->SetVisAttributes (G4VisAttributes::Invisible);
	CarbonFoilAlRing_log->SetVisAttributes(redVA);

	/*High voltage barrier volumes*/
	HVOuterSleeve_log -> SetVisAttributes(blackVA);
	HVInsulation_log -> SetVisAttributes(redVA);
	HVBarrier_log -> SetVisAttributes(blackVA);
	HVHorseShoe_log -> SetVisAttributes(greyVA);
	//HVOuterSleeve_log->SetVisAttributes (G4VisAttributes::Invisible);
	//HVInsulation_log->SetVisAttributes (G4VisAttributes::Invisible);
	//HVBarrier_log->SetVisAttributes (G4VisAttributes::Invisible);
	//HVHorseShoe_log->SetVisAttributes (G4VisAttributes::Invisible);

	/*Targer front and backing*/
	//TargetBacking_log -> SetVisAttributes(redVA);
	//TargetBacking_log -> SetVisAttributes(G4VisAttributes::Invisible);
	//TargetFront_log -> SetVisAttributes(orangeVA);
	//TargetFront_log -> SetVisAttributes(G4VisAttributes::Invisible);

	//Target_log -> SetVisAttributes(blueVA);

	TCDoor_log -> SetVisAttributes(redVA);

	TWF_log -> SetVisAttributes(blueVA);
	TWB_log -> SetVisAttributes(blueVA);
	TWGear_log -> SetVisAttributes(cyanVA);
	TWMotor_log-> SetVisAttributes(orangeVA);

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Sage::ConstructEMField(G4String fieldFileName)
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
	HVField = new SageHVField(fieldFileName, 0, fHVFieldStrength);
	//HVField = new SageHVField("SageHVField.table", 0, 1.);
	//G4ElectroMagneticField* foo;
	//foo = new SageHVField(0, 0, 0);

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
// Virtual Method of NPS::VDetector class
// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Sage::ReadConfiguration(NPL::InputParser parser)
{
	vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Sage");
	if(NPOptionManager::getInstance()->GetVerboseLevel())
		cout << "//// " << blocks.size() << " detectors found " << endl; 

	vector<string> cart = {"POS","Shape"};
	vector<string> sphe = {"R","Theta","Phi","Shape"};
	vector<string> foil = {"CarbonFoil","InnerDiameter","SiThickness"};

	for(unsigned int i = 0 ; i < blocks.size() ; i++){
		if(blocks[i]->HasTokenList(cart)){
			if(NPOptionManager::getInstance()->GetVerboseLevel())
				cout << endl << "////  Sage " << i+1 <<  endl;

			G4ThreeVector Pos = NPS::ConvertVector(blocks[i]->GetTVector3("POS","mm"));
			string Shape = blocks[i]->GetString("Shape");
			AddDetector(Pos,Shape);
		}
		else if(blocks[i]->HasTokenList(sphe)){
			if(NPOptionManager::getInstance()->GetVerboseLevel())
				cout << endl << "////  Sage " << i+1 <<  endl;
			double R = blocks[i]->GetDouble("R","mm");
			double Theta = blocks[i]->GetDouble("Theta","deg");
			double Phi = blocks[i]->GetDouble("Phi","deg");
			string Shape = blocks[i]->GetString("Shape");
			AddDetector(R,Theta,Phi,Shape);
		}
		else if(blocks[i]->HasTokenList(foil)){
			if(NPOptionManager::getInstance()->GetVerboseLevel())
				cout << endl << "////  Sage " << i+1 <<  endl;
			CarbonFoils = blocks[i]->GetInt("CarbonFoil");
			InnerDiameter = blocks[i]->GetDouble("InnerDiameter","mm");
			SiThickness = blocks[i]->GetDouble("SiThickness","mm");
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
void Sage::ConstructDetector(G4LogicalVolume* world)
{
	ConstructChamber(world);
	ConstructEMField("SageEMField.table");
	for (unsigned short i = 0 ; i < m_R.size() ; i++)
	{

		G4double wX = m_R[i] * sin(m_Theta[i] ) * cos(m_Phi[i] ) ;
		G4double wY = m_R[i] * sin(m_Theta[i] ) * sin(m_Phi[i] ) ;
		G4double wZ = m_R[i] * cos(m_Theta[i] ) ;
		G4ThreeVector Det_pos = G4ThreeVector(-sin(Sage_NS::SageRotation)*(wZ+wX), wY, cos(Sage_NS::SageRotation)*wZ) ;

		Det_pos+=Det_pos.unit()*SiThickness*0.5;

		if(m_Shape[i] == "Silicon")
		{
			new G4PVPlacement(G4Transform3D(G4RotationMatrix(0.,0.,0.),Det_pos),
					BuildSiliconDetector(),
					"Sage",world,false,i+1);
		}
		else if (m_Shape[i] == "Chamber")
		{
			//new G4PVPlacement(G4Transform3D(G4RotationMatrix(0.,0.,0.),G4ThreeVector(0.,0.,0.)),
			//		BuildChamber(),
			//		"Sage",world,false,i+1);

		}
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Sage::InitializeRootOutput()
{
	RootOutput *pAnalysis = RootOutput::getInstance();
	TTree *pTree = pAnalysis->GetTree();
	if(!pTree->FindBranch("Sage")){
		pTree->Branch("Sage", "TSageData", &m_Event) ;
	}
	pTree->SetBranchAddress("Sage", &m_Event) ;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
//TODO Make this behave more like annular detector readout, e.g. SharcQQQ
void Sage::ReadSensitive(const G4Event* event)
{
	m_Event->Clear();

	G4THitsMap<G4double*>* PixelHitMap;
	std::map<G4int, G4double**>::iterator Pixel_itr;

	G4int PixelCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("SageScorer/SagePixel");
	PixelHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(PixelCollectionID));

	//// Loop on the Pixel map
	for (Pixel_itr = PixelHitMap->GetMap()->begin() ; Pixel_itr != PixelHitMap->GetMap()->end() ; Pixel_itr++)
	{
		G4double* Info = *(Pixel_itr->second);
		double Energy = RandGauss::shoot(Info[0],Sage_NS::ResoEnergy);
		if(Energy>Sage_NS::EnergyThreshold){
			double Time = RandGauss::shoot(Info[1],Sage_NS::ResoTime);
			int DetectorNbr = (int) Info[7];
			int PixelNbr = (int) Info[8];
			m_Event->SetEnergy(DetectorNbr, PixelNbr, Energy);
			m_Event->SetTime(DetectorNbr, PixelNbr, Time); 
		}
	}
	// clear map for next event
	PixelHitMap->clear();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////   
void Sage::InitializeScorers()
{ 
	// This check is necessary in case the geometry is reloaded
	bool already_exist = false; 
	m_SageScorer = CheckScorer("SageScorer",already_exist) ;

	if(already_exist) 
		return ;
	//TODO make these read from a file
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
	}

	cout << PixelInnerRadius.size() << " pixels defined" << endl;

	vector<G4double>* PixelInnerRadius_p = &PixelInnerRadius;
	vector<G4double>* PixelOuterRadius_p = &PixelOuterRadius;
	vector<G4double>* PixelPhiStart_p = &PixelPhiStart;
	vector<G4double>* PixelPhiStop_p = &PixelPhiStop;


	// Otherwise the scorer is initialised
	vector<int> level; level.push_back(0);

	G4VPrimitiveScorer* PixelScorer =
		new SILICONSCORERS::PS_Silicon_Pixel("SagePixel",
				0,
				PixelInnerRadius_p,
				PixelOuterRadius_p,
				PixelPhiStart_p,
				PixelPhiStop_p,
				1);
	//G4VPrimitiveScorer* Calorimeter= new CALORIMETERSCORERS::PS_Calorimeter("Calorimeter",level, 0) ;
	//and register it to the multifunctionnal detector
	m_SageScorer->RegisterPrimitive(PixelScorer);
	G4SDManager::GetSDMpointer()->AddNewDetector(m_SageScorer) ;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* Sage::Construct()
{
	return  (NPS::VDetector*) new Sage();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C"
{
	class proxy_nps_Sage
	{
		public:
			proxy_nps_Sage()
			{
				NPS::DetectorFactory::getInstance()->AddToken("Sage","Sage");
				NPS::DetectorFactory::getInstance()->AddDetector("Sage",Sage::Construct);
			}
	};
	proxy_nps_Sage p_nps_Sage;
}
