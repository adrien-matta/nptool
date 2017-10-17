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
 *  This class describe  Jurogam simulation                             *
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
#include "G4Sphere.hh"
#include "G4Trap.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4Cons.hh"
#include "G4Trd.hh"

#include "G4LogicalVolume.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
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
#include "Jurogam.hh"
#include "CalorimeterScorers.hh"
#include "RootOutput.h"
#include "MaterialManager.hh"
#include "NPSDetectorFactory.hh"
#include "NPOptionManager.h"
// CLHEP header
#include "CLHEP/Random/RandGauss.h"

#include "Materials.hh"

using namespace std;
using namespace CLHEP;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace Jurogam_NS{
	// Energy and time Resolution
	const double EnergyThreshold = 1*keV;
	const double ResoTime = 4.5*ns ;
	const double ResoEnergy = 1.1*keV ;
	const double BGOEnergyThreshold = 40*keV;
	const double BGOResoTime = 4.5*ns ;
	const double BGOResoEnergy = 2.5*keV ;
	//const double Radius = 50*mm ; 
	//const double Width = 100*mm ;
	//const double Thickness = 300*mm ;
	//const string Material = "BC400";
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Jurogam Specific Method
Jurogam::Jurogam()
{
	m_Event = new TJurogamData() ;
	m_JurogamScorer = 0;
	//Initialising volumes to be sensitive volumes
	logicGe_PhaseI = 0;
	PhaseILogicBGOCrystal = 0;
	CloverConstructed = false;
	CloverBGOConstructed = false;

	PhaseIBGOShieldLength = 251.75*mm;
	PhaseIBGOShieldOuterRadius = 167.*mm/2.;
	PhaseIBGOShieldInnerRadius = 0.*mm;
	PhaseIsubInnerConeL = 120.875*mm;
	PhaseIBGOHevimetThickness = 35.0*mm; // real thickness 35.*mm, collar 1.5*mm
	//PhaseIBGOHevimetDist = 138.0*mm; // CHECK
	PhaseIBGOShieldDist = PhaseIBGOHevimetThickness+1.5*mm;

	//---------------------------------
	//make the required materials and assign them
	fMat = Materials::GetInstance();

	//assign default materials.....
	vacuumMaterial     =  fMat->GetMaterial("Vacuum");
	BGOMaterial        =  fMat->GetMaterial("BGO");
	HevimetMaterial    =  fMat->GetMaterial("Hevimet");
	ShieldMaterial     =  fMat->GetMaterial("Aluminium"); // is it Al alloy?
	Absorber1Material  =  fMat->GetMaterial("Tin");
	Absorber2Material  =  fMat->GetMaterial("Copper");
	geMaterial         = fMat->GetMaterial("Germanium");
	endCapMaterial     = fMat->GetMaterial("DurAl");
	contactMaterial    = fMat->GetMaterial("Lithium");

	useAbsorber = false; // By default the absorbers are always in!

	//Default materials

	lappingSize = 3.0;

	//fTotalGeL     = 69.00 * mm;  //was 70
	fTotalGeL     = 70.00 * mm;  //playing around to fudge DC
	//fCrystalR     = 24.25 * mm;  //was 25
	//fCrystalR     = 22.0* mm;  //playing around to fudge DC
	fCrystalR     = (25-lappingSize)* mm;  //playing around to fudge DC
	fEndCap2Ge    = 20.00 * mm;  //Distance from Al outer face to Ge

	//added to fudge PTG's efficiency measurements for STUK (Karl)
	//fFudge = 5.0*mm;
	fFudge = 5.0*mm; // Fudge factor set to 0, 4.9.2013 JK
	//fFudge = 5.0*mm;
	fEndCap2Ge += fFudge;

	fHoleR            =  5.5 * mm; //was 5.0
	//fHoleR            =  6.5 * mm; //playing around to fudge DC
	fPassiveThick     =  0.5 * mm;
	fContactThick     =  0.5 * mm;
	//fContactThick     =  0.5 * mm;  //playing around to fudge DC
	fGapBetweenLeaves =  0.6 * mm;

	CloverBGOCrystalAngle = 7.1*deg;

	CloverBGOCrystalBoxD  = 18.86*mm; // approx
	CloverBGOCrystalBoxD2 = 3.0*mm;   // = ? (real size unknown)
	CloverBGOCrystalTrapL = 120.5*mm; //total length ~ 241 mm
	CloverBGOCrystalW     = 141.31*mm; // approx
	CloverBGOCrystalW2     = 82.0*mm; // approx
	CloverBGOCrystalW3     = (CloverBGOCrystalW+CloverBGOCrystalW2)/2.;
	CloverBGOHevimetThickness = 35.*mm;
	CloverBGOHevimetDist = 138.0*mm;
	CloverBGOShieldDist = CloverBGOHevimetDist+CloverBGOHevimetThickness;
	CloverBGOCrystalDist = 6.7*mm+CloverBGOCrystalTrapL/2.;
	CloverBGOCrystalH = (sin(7.1*deg)*CloverBGOCrystalTrapL/2.+2.+39.5)*mm;

	CreatePhaseISolids();
	CreatePhaseIBGOSolids();
	CreateCloverSolids();
	CreateCloverBGOSolids();

}
Jurogam::~Jurogam()
{
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Jurogam::AddDetector(G4ThreeVector POS, string  Shape, G4bool BGO)
{
	// Convert the POS value to R theta Phi as Spherical coordinate is easier in G4 
	m_R.push_back(POS.mag());
	m_Theta.push_back(POS.theta());
	m_Phi.push_back(POS.phi());
	m_Shape.push_back(Shape);
	m_BGO.push_back(BGO);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Jurogam::AddDetector(double  R, double  Theta, double  Phi, string  Shape, G4bool BGO)
{
	m_R.push_back(R);
	m_Theta.push_back(Theta);
	m_Phi.push_back(Phi);
	m_Shape.push_back(Shape);
	m_BGO.push_back(BGO);
	m_X.push_back(0);
	m_Y.push_back(0);
	m_Z.push_back(0);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Jurogam::AddDetector(G4ThreeVector POS, double  Theta, double  Phi, string  Shape, G4bool BGO)
{
	m_R.push_back(0);
	m_Theta.push_back(Theta);
	m_Phi.push_back(Phi);
	m_Shape.push_back(Shape);
	m_BGO.push_back(BGO);
	m_X.push_back(POS.getX());
	m_Y.push_back(POS.getY());
	m_Z.push_back(POS.getZ());
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Jurogam::SetPosition(G4ThreeVector thisPos)
{
	position = thisPos*mm;
}
void Jurogam::SetRotation(G4RotationMatrix thisRot)
{
	rotation = thisRot;
}
//---------------------------------------------------------------------
// Create the solids defining detectors
//---------------------------------------------------------------------
void Jurogam::CreatePhaseISolids()
{
	//--------------------------------------------------------
	//Define the dimensions used to construct an Ortec PhaseI
	fAlCap2Ge_Ortec        = 15.1 * mm; //Distance from Al outer face to Ge
	fGeTaperL_Ortec        = 30.0 * mm;
	fTotalGeL_Ortec        = 70.0 * mm;
	fGeOuterD_Ortec        = 70.0 * mm;
	fGeInnerD_Ortec        = 64.0 * mm; //dR = 10./100.*taperL

	fEndCapFrontR_Ortec    =  34.0*mm;
	fEndCapBackR_Ortec     =  44.0*mm;
	fEndCapTaperL_Ortec    = 100.0*mm; //=> tan(theta) = 10/100
	fEndCapTubeL_Ortec     = 260.0*mm;
	fEndCapThickness_Ortec =   1.2*mm;

	//default detector paremeters
	geRingZPos = 0.0*mm;

	fAlCap2Ge_PhaseI = 15.1 * mm; //fAlCap2Ge_Ortec;  //Distance from Al outer face to Ge
	fGeTaperL_PhaseI = 30.0 * mm; //fGeTaperL_Ortec;
	fTotalGeL_PhaseI = 70.0 * mm; //fTotalGeL_Ortec;
	fGeOuterD_PhaseI = 66.0 * mm; //fGeOuterD_Ortec;
	fGeInnerD_PhaseI = 64.0 * mm; //fGeInnerD_Ortec;   //dR = 10./100.*taperL

	fEndCapFrontR_PhaseI    =  34.0*mm;
	fEndCapBackR_PhaseI     =  44.0*mm;
	fEndCapTaperL_PhaseI    = 100.0*mm; //=> tan(theta) = 10/100
	fEndCapTubeL_PhaseI     = 260.0*mm;
	fEndCapThickness_PhaseI =   1.2*mm;

	//------------------------------------------
	// If it has been set up correctly then the
	// different dimesions for the Ortec, Eurisys
	// and Shorty have already been accounted for
	//-------------------------------------------
	G4double phiStartGe =   0.0*deg; // full circle...
	G4double phiTotalGe = 360.0*deg;

	G4double endCapFrontR    = fEndCapFrontR_PhaseI;
	G4double endCapBackR     = fEndCapBackR_PhaseI;
	G4double endCapTaperL    = fEndCapTaperL_PhaseI;
	G4double endCapCylinderL = fEndCapTubeL_PhaseI;

	//------------------------------
	// Al container tube
	const G4int numZPlanesAl=3;      // no. polycone planes
	G4double zPlaneAl[numZPlanesAl]; // positions of planes
	zPlaneAl[0] = 0.0*mm;
	zPlaneAl[1] = endCapTaperL;
	zPlaneAl[2] = endCapTaperL + endCapCylinderL;
	G4double rInnerAl[numZPlanesAl]; // interior radii
	rInnerAl[0] = 0.0*mm;
	rInnerAl[1] = 0.0*mm;
	rInnerAl[2] = 0.0*mm; // I think this is supposed to be 0, to avoid geometry warnings (JK) //was endCapBackR
	G4double rOuterAl[numZPlanesAl]; // exterior radii
	rOuterAl[0] = endCapFrontR;
	rOuterAl[1] = endCapBackR;  //=> tan(theta) = 10/100
	rOuterAl[2] = endCapBackR;

	solidAlCap_PhaseI = new G4Polycone("capAl", phiStartGe, phiTotalGe, numZPlanesAl, zPlaneAl, rInnerAl, rOuterAl);


	//------------------------------
	// Vacuum tube inside container : make Al walls 1.2 mm
	G4double AlThick = fEndCapThickness_PhaseI;

	const G4int numZPlanesVac=3;       // no. polycone planes
	G4double zPlaneVac[numZPlanesVac]; // positions of planes
	zPlaneVac[0] = zPlaneAl[0] + AlThick;                             //AlThick
	zPlaneVac[1] = zPlaneAl[1] + 10.*AlThick*(1./cos(atan(0.1)) - 1.);// AlThick;         //
	zPlaneVac[2] = zPlaneAl[2] - AlThick;
	G4double rInnerVac[numZPlanesVac]; // interior radii
	rInnerVac[0] = 0.0*cm;
	rInnerVac[1] = 0.0*cm;
	rInnerVac[2] = 0.0*cm;
	G4double rOuterVac[numZPlanesVac]; // exterior radii
	rOuterVac[0] = rOuterAl[0] + 0.1*AlThick - AlThick/cos(atan(0.1));
	rOuterVac[1] = rOuterAl[1] - AlThick;
	rOuterVac[2] = rOuterAl[2] - AlThick;

	//G4cout << "Vacuum " << rOuterVac[0] << " " << rOuterVac[1] << " " << rOuterVac[2] << G4endl;


	solidVacuum_PhaseI = new G4Polycone("vacuum", phiStartGe, phiTotalGe, numZPlanesVac, zPlaneVac, rInnerVac, rOuterVac);

	//could consider subtracting the above vacuum from the AlCap => one less solid to place

	//-------------------------------------------------------------------------------------
	// Germanium.....
	//G4double Gap     =  3.5 * mm;
	G4double delta   = 2.0 * mm;  //rounded edges

	const G4int numZPlanesGe=4;      // no. polycone planes
	G4double zPlaneGe[numZPlanesGe]; // positions of planes
	zPlaneGe[0] = 0.0 * mm;
	zPlaneGe[1] = delta;
	zPlaneGe[2] = fGeTaperL_PhaseI;
	zPlaneGe[3] = fTotalGeL_PhaseI;
	G4double rInnerGe[numZPlanesGe]; // interior radii
	rInnerGe[0] = 0.0*cm;
	rInnerGe[1] = 0.0*cm;
	rInnerGe[2] = 0.0*cm;
	rInnerGe[3] = 0.0*cm;
	G4double rOuterGe[numZPlanesGe]; // exterior radii
	rOuterGe[0] = fGeInnerD_PhaseI/2. - delta;     //a little rounding...
	rOuterGe[1] = fGeInnerD_PhaseI/2. + 0.1*delta; //tan(theta) = 0.1
	rOuterGe[2] = fGeOuterD_PhaseI/2.;
	rOuterGe[3] = fGeOuterD_PhaseI/2.;


	solidGe_PhaseI = new G4Polycone("germanium", phiStartGe, phiTotalGe, numZPlanesGe, zPlaneGe, rInnerGe, rOuterGe);

	//------------------------------------------------------------------
	// Inner bore hole + lithium contact + passivated Ge
	G4double GeDepth      =  5.2 * mm;           //treated => could make hole 0.2 mm longer than nominal 5.2 mm
	G4double contactThick =  0.50*mm;
	G4double passiveThick =  0.30*mm;

	//G4double zPasGeLength = fTotalGeL_PhaseI - GeDepth; // Passivated Ge : inner contact 0.3 mm thick

	//G4double innerRHole =  0.00*mm;
	//G4double holeR      =  5.00*mm;            //treated => could make it 0.5 mm bigger than the nominal 5.0*mm
	G4double holeR      =  5.50*mm;            //treated => could make it 0.5 mm bigger than the nominal 5.0*mm fudging the code DC
	G4double contactR   = holeR + contactThick;
	G4double passiveR   = contactR + passiveThick;
	G4double holeL      = fTotalGeL_PhaseI - GeDepth;
	G4double tubeL      = holeL - holeR;

	//save this for placement
	fContact_dZ_PhaseI = fTotalGeL_PhaseI - tubeL/2.;

	//the same translation works for all the following.
	G4ThreeVector transHole( 0.001*mm, 0.001*mm, -tubeL/2.-0.001*mm); //if offsets are 0. it does not display !!

	//now add a passivated layer
	G4Sphere* passivatedSphere = new G4Sphere("passSphere", 0.0*mm, passiveR,           0.*degree, 360.*degree, 0.*degree, 180.*degree);
	G4Tubs*   passivatedTube   = new G4Tubs(  "passTube",   0.0*mm, passiveR, tubeL/2., 0.*degree, 360.*degree);
	solidPassivated_PhaseI = new G4UnionSolid("passivatedGe",passivatedTube,passivatedSphere,0,transHole);

	//and the Li contact
	G4Sphere* contactSphere = new G4Sphere("sphere1", 0.0*mm, contactR,           0.*deg, 360.*deg, 0.*deg, 180.*deg);
	G4Tubs*   contactTube   = new G4Tubs(  "tube1",   0.0*mm, contactR, tubeL/2., 0.*deg, 360.*deg);
	solidContact_PhaseI = new G4UnionSolid("liContact",contactTube,contactSphere,0,transHole);

	//bore out a hole
	G4Sphere* boreSphere = new G4Sphere("boreSphere",0.0*mm, holeR,           0.*degree, 360.*degree, 0.*degree, 180.*degree);
	G4Tubs*   boreTube   = new G4Tubs(  "boreTube",  0.0*mm, holeR, tubeL/2., 0.*degree, 360.*degree);
	solidBoreHole_PhaseI = new G4UnionSolid("hole+sphere",boreTube,boreSphere,0,transHole);


}
void  Jurogam::CreatePhaseIBGOSolids()
{
	// -----------
	// BGO Crystal
	// -----------

	//  It's a 10-sided polyhedra with 2 z-sides in the beginning
	double* z = new double[2];  // 10 z-planes used later, ...
	double* ri = new double[2]; // only 2 z-planes here; same variables used
	double* ro = new double[2];

	z[0]  = 0.0;  z[1]  =  190.0;
	ri[0] = 33.0; ri[1] =  50.791; // ri[0] was 32.397
	ro[0] = 52.397; ro[1] =  70.992; // ro[0] was 43.102, ro[1] should be max 146mm/2.

	G4VSolid* CrystalBlock = new G4Polyhedra("CrystalBlock", 0.*deg, 360.*deg,10,2,z,ri,ro);

	G4VSolid* CrystalInnerTubsSub = new G4Tubs("CrystalInnerTubsSub",
			0.*mm,
			68.0*mm/2., // exactly
			50.*mm, // this length is not so important
			0.*deg,
			360.*deg);

	//Rotation of slicer cube
	G4RotationMatrix slicerot1;
	slicerot1.set(0,0,0);
	G4ThreeVector offvec(0,0,0);

	// Subtract the round hole (front face)
	G4VSolid* CrystalBlockSub
		= new G4SubtractionSolid ("CrystalBlockSub",CrystalBlock,CrystalInnerTubsSub,
				&slicerot1,offvec);



	// Create a solid box to be used as a ''slicer''
	G4VSolid* SliceBoxSub = new G4Box("SliceBoxSub",1001.*mm,100.5*mm,1001.*mm);
	G4VSolid* SliceBox    = new G4Box("SliceBox",1000.*mm,200.*mm,1000.*mm);

	// Slice the IceCreamCone with a big box from different angles
	G4VSolid* IceCreamSlicer = new G4SubtractionSolid("IceCreamSlicer",SliceBox,SliceBoxSub,0,G4ThreeVector(0,-100.5*mm,0*mm));

	// Start slicing !!
	G4double crystalangle = 18.*deg;
	G4RotationMatrix crysrotation; // Rotation to the crystal block coordinates
	crysrotation.set(0,0,0);
	crysrotation.rotateZ(crystalangle);
	crysrotation.invert();

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(-30.86*deg+crystalangle);
	slicerot1.rotateX(12.980212*deg);
	offvec = G4ThreeVector(-0.2*mm,43.102*mm,0);
	//offvec = G4ThreeVector(-19.034*mm,31.846*mm,0);
	offvec.transform(crysrotation);

	G4VSolid* SlicedCrystal1
		= new G4SubtractionSolid ("SlicedCrystal1",CrystalBlockSub,IceCreamSlicer,
				&slicerot1,offvec);
	slicerot1.set(0,0,0);
	slicerot1.rotateZ(30.86*deg+crystalangle);
	slicerot1.rotateX(12.980212*deg);
	offvec = G4ThreeVector(0.2*mm,43.102*mm,0);

	offvec.transform(crysrotation);

	G4VSolid* SlicedCrystal2
		= new G4SubtractionSolid ("SlicedCrystal1",SlicedCrystal1,IceCreamSlicer,
				&slicerot1,offvec);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(90.*deg+crystalangle);
	slicerot1.rotateX(18.*deg);
	offvec = G4ThreeVector(37.16*mm,0.0*mm,0);
	offvec.transform(crysrotation);

	G4VSolid* SlicedCrystal3
		= new G4SubtractionSolid ("SlicedCrystal1",SlicedCrystal2,IceCreamSlicer,
				&slicerot1,offvec);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(-90.*deg+crystalangle);
	slicerot1.rotateX(18.*deg);
	offvec = G4ThreeVector(-37.16*mm,0.0*mm,0);
	offvec.transform(crysrotation);

	G4VSolid* SlicedCrystal4
		= new G4SubtractionSolid ("SlicedCrystal1",SlicedCrystal3,IceCreamSlicer,
				&slicerot1,offvec);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(-123.9*deg+crystalangle);
	slicerot1.rotateX(13.*deg);
	offvec = G4ThreeVector(-36.57*mm,-12.093*mm,0);
	offvec.transform(crysrotation);

	G4VSolid* SlicedCrystal5
		= new G4SubtractionSolid ("SlicedCrystal1",SlicedCrystal4,IceCreamSlicer,
				&slicerot1,offvec);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(123.9*deg+crystalangle);
	slicerot1.rotateX(13.*deg);
	offvec = G4ThreeVector(36.57*mm,-12.093*mm,0);
	offvec.transform(crysrotation);

	G4VSolid* SlicedCrystal6
		= new G4SubtractionSolid ("SlicedCrystal1",SlicedCrystal5,IceCreamSlicer,
				&slicerot1,offvec);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(180.*deg+crystalangle);
	slicerot1.rotateX(16.*deg);
	offvec = G4ThreeVector(0.*mm,-36.*mm,0);
	offvec.transform(crysrotation);

	G4VSolid* SlicedCrystal7
		= new G4SubtractionSolid ("SlicedCrystal1",SlicedCrystal6,IceCreamSlicer,
				&slicerot1,offvec);

	PhaseISolidBGOCrystal = SlicedCrystal7;


	// -------------------------------------------
	// BGO Shield Heavy Metal Collimator (HEVIMET)
	// -------------------------------------------

	G4VSolid* BGOHevimetTubs = new G4Tubs("BGOHevimetTubs",
			50.0*mm/2.,
			50.*mm,
			PhaseIBGOHevimetThickness/2.*mm,
			0.*deg,
			360.*deg);

	//G4VSolid* subHevimetInnerTubs = new G4Tubs("subHevimetInnerTubs",
	//              0.*mm,
	//              55.*mm/2.,
	//              2.*mm, // 2mm off the hevimet front face
	//              0.*deg,
	//              360.*deg);

	//Rotation of slicer cube
	slicerot1.set(0,0,0);
	slicerot1.rotateZ(-30.9*deg);
	slicerot1.rotateX(13.*deg); //was 13.2

	// Location of slicer cube bottom center
	G4ThreeVector offvec2(0,34.089*mm,-PhaseIBGOHevimetThickness/2);

	// Start slicing !!
	G4VSolid* HevimetTubSlice1
		= new G4SubtractionSolid ("HevimetTubSlice1",BGOHevimetTubs,IceCreamSlicer,
				&slicerot1,offvec2);
	slicerot1.set(0,0,0);
	slicerot1.rotateZ(+30.9*deg);
	slicerot1.rotateX(13.*deg);

	G4VSolid* HevimetTubSlice2
		= new G4SubtractionSolid ("HevimetTubSlice2",HevimetTubSlice1,IceCreamSlicer,
				&slicerot1,offvec2);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(+90.*deg);
	slicerot1.rotateX(14.1*deg);

	offvec2 = G4ThreeVector(30.*mm,0,-PhaseIBGOHevimetThickness/2);
	G4VSolid* HevimetTubSlice3
		= new G4SubtractionSolid ("HevimetTubSlice3",HevimetTubSlice2,IceCreamSlicer,
				&slicerot1,offvec2);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(-90.*deg);
	slicerot1.rotateX(14.1*deg);

	offvec2 = G4ThreeVector(-30.*mm,0,-PhaseIBGOHevimetThickness/2);
	G4VSolid* HevimetTubSlice4
		= new G4SubtractionSolid ("HevimetTubSlice4",HevimetTubSlice3,IceCreamSlicer,
				&slicerot1,offvec2);


	slicerot1.set(0,0,0);
	slicerot1.rotateZ(123.9*deg);
	slicerot1.rotateX(13*deg); //was 12.952764513

	offvec2 = G4ThreeVector(15.58*mm,-29.26*mm,-PhaseIBGOHevimetThickness/2);
	G4VSolid* HevimetTubSlice5
		= new G4SubtractionSolid ("HevimetTubSlice5",HevimetTubSlice4,IceCreamSlicer,
				&slicerot1,offvec2);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(-123.9*deg);
	slicerot1.rotateX(13*deg);

	offvec2 = G4ThreeVector(-15.58*mm,-29.26*mm,-PhaseIBGOHevimetThickness/2);
	G4VSolid* HevimetTubSlice6
		= new G4SubtractionSolid ("HevimetTubSlice6",HevimetTubSlice5,IceCreamSlicer,
				&slicerot1,offvec2);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(180*deg);
	slicerot1.rotateX(14.68*deg);

	offvec2 = G4ThreeVector(0,-29.26*mm,-PhaseIBGOHevimetThickness/2);
	G4VSolid* HevimetTubSlice7
		= new G4SubtractionSolid("HevimetTubSlice7",HevimetTubSlice6,IceCreamSlicer,
				&slicerot1,offvec2);

	PhaseISolidBGOHevimet = HevimetTubSlice7;

	// ---------------------------------------
	// BGO Shield Aluminium part
	// ---------------------------------------

	G4VSolid* BGOShieldTubs = new G4Tubs("BGOShieldTubs",
			PhaseIBGOShieldInnerRadius,
			PhaseIBGOShieldOuterRadius,
			(PhaseIBGOShieldLength/2.)*mm,
			0.*deg,
			360.*deg);


	G4VSolid* subInnerCone = new G4Cons("subInnerCone",
			0.*mm, //	inside radius at -pDz
			64.*mm/2., // outside radius at -pDz
			0.*mm, // inside radius at +pDz
			88.1*mm/2., // outside radius at +pDz
			PhaseIsubInnerConeL/2.,    // half length in z
			0.*deg,  // starting angle of the segment in radians
			360.*deg); // the angle of the segment in radians

	G4VSolid* subInnerTubs = new G4Tubs("subInnerTubs",
			0.*mm,
			88.1*mm/2., // CHECK
			(PhaseIBGOShieldLength-PhaseIsubInnerConeL)/2.+10.*mm, // CHECK
			0.*deg,
			360.*deg);

	G4ThreeVector vec(0,0,(PhaseIBGOShieldLength)/2.+10.*mm); // CHECK
	G4UnionSolid* subInnerPart = new G4UnionSolid("subInnerPart",subInnerCone,subInnerTubs,0,vec);

	G4ThreeVector vec2(0,0,-(PhaseIBGOShieldLength-PhaseIsubInnerConeL+10.*um)/2.); // 10um offset for proper cut that shows in visualization?

	// Subtract the hole from the shield tubs = IceCreamCone1
	G4VSolid* IceCreamCone1 = new G4SubtractionSolid ("IceCreamCone1",BGOShieldTubs,subInnerPart,0,vec2);

	//Rotation of slicer cube
	//G4RotationMatrix slicerot1;
	slicerot1.set(0,0,0);
	slicerot1.rotateZ(-30.9*deg);
	slicerot1.rotateX(13.*deg); //14.98 in the .igs ?? cannot be right as it doesn't fit

	// Location of slicer cube bottom center
	G4ThreeVector vec3(0,44.153*mm,-PhaseIBGOShieldLength/2);

	// Start slicing !!
	G4VSolid* IceCreamCone2
		= new G4SubtractionSolid ("IceCreamCone2",IceCreamCone1,IceCreamSlicer,
				&slicerot1,vec3);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(+30.9*deg);
	slicerot1.rotateX(13.*deg);

	G4VSolid* IceCreamCone3
		= new G4SubtractionSolid ("IceCreamCone3",IceCreamCone2,IceCreamSlicer,
				&slicerot1,vec3);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(+90.*deg);
	slicerot1.rotateX(18.*deg); //18.42 .igs?? cannot be right, as it doesn't fit

	vec3 = G4ThreeVector(37.03,20.89*mm,-PhaseIBGOShieldLength/2);
	G4VSolid* IceCreamCone4
		= new G4SubtractionSolid ("IceCreamCone4",IceCreamCone3,IceCreamSlicer,
				&slicerot1,vec3);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(-90.*deg);
	slicerot1.rotateX(18.*deg);

	vec3 = G4ThreeVector(-37.03,20.89*mm,-PhaseIBGOShieldLength/2);
	G4VSolid* IceCreamCone5
		= new G4SubtractionSolid ("IceCreamCone5",IceCreamCone4,IceCreamSlicer,
				&slicerot1,vec3);


	slicerot1.set(0,0,0);
	slicerot1.rotateZ(123.9*deg);
	slicerot1.rotateX(13.*deg); // was 14.956711

	vec3 = G4ThreeVector(37.03,-11.29*mm,-PhaseIBGOShieldLength/2);
	G4VSolid* IceCreamCone6
		= new G4SubtractionSolid ("IceCreamCone6",IceCreamCone5,IceCreamSlicer,
				&slicerot1,vec3);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(-123.9*deg);
	slicerot1.rotateX(13.*deg); // was 14.956711

	vec3 = G4ThreeVector(-37.03,-11.29*mm,-PhaseIBGOShieldLength/2);
	G4VSolid* IceCreamCone7
		= new G4SubtractionSolid ("IceCreamCone7",IceCreamCone6,IceCreamSlicer,
				&slicerot1,vec3);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(180*deg);
	slicerot1.rotateX(16.5*deg);

	vec3 = G4ThreeVector(0,-37.86*mm,-PhaseIBGOShieldLength/2);
	G4VSolid* IceCreamCone8
		= new G4SubtractionSolid("IceCreamCone8",IceCreamCone7,IceCreamSlicer,
				&slicerot1,vec3);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(148.3*deg);
	slicerot1.rotateX(8.72*deg);// was 8.718867709, but crystal shows up then

	vec3 = G4ThreeVector(23.529*mm,-53.159*mm,-PhaseIBGOShieldLength/2+48.672*mm);
	G4VSolid* IceCreamCone9
		= new G4SubtractionSolid("IceCreamCone9",IceCreamCone8,IceCreamSlicer,
				&slicerot1,vec3);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(-148.3*deg);
	slicerot1.rotateX(8.5*deg); // was 8.72.... 8.5 (.igs)

	vec3 = G4ThreeVector(-23.529*mm,-53.159*mm,-PhaseIBGOShieldLength/2+48.672*mm);
	G4VSolid* IceCreamCone
		= new G4SubtractionSolid("IceCreamCone",IceCreamCone9,IceCreamSlicer,
				&slicerot1,vec3);

	PhaseISolidBGOShield = IceCreamCone;

	//------------------------------------------------------------------
	// Absorbers used in front of the detectors
	// 1. Sn 0.1 mm (closer to target)
	// 2. Cu 0.5 mm
	//------------------------------------------------------------------
	//abs1_thickness = 0.1*mm;
	abs1_thickness = 0.26*mm;
	//abs2_thickness = 0.5*mm;
	abs2_thickness = 0.65*mm;
	Phase1Absorber1 = new G4Tubs("Phase1Absorber1", 0.*mm, 54./2.*mm, abs1_thickness/2., 0., 360.*deg);
	Phase1Absorber2 = new G4Tubs("Phase1Absorber2", 0.*mm, 54./2.*mm, abs2_thickness/2., 0., 360.*deg);

}
void  Jurogam::CreateCloverSolids()
{
	//An approximate CloverII
	//G4cout << " ----> Constructing archetypal Clover" << G4endl;

	//---------------------------------------------------------
	//end-cap
	G4double endCapFrontThickness = 1.5*mm;//1.5 in the Duchene paper //1.2*mm; in another simulation
	G4double endCapTaperThickness = 1.5*mm;
	G4double endCapSideThickness  = 1.5*mm;

	G4double GeGap      =  fEndCap2Ge; //outsideface of endcap to Ge
	G4double taperAngle =  7.0*degree;

	G4double endCapTotalL = fTotalGeL + GeGap + endCapFrontThickness + 5.*mm; //+ Gap at rear end
	G4double endCapFrontD = 43.5*mm;
	G4double endCapBackD  = 50.5*mm;
	G4double endCapTaperL = 55.0*mm;

	G4double endCapBoxL   = endCapTotalL - endCapTaperL;

	//the tapered part
	G4Trap* solidTaperedCloverEC
		= new G4Trap("taperedCloverEC",
				endCapTaperL/2., //Half z-length [pDz]
				0.00*degree,     //Polar angle of line joining centres of the faces @ -/+pDz
				2.*taperAngle,   //aequivalent zimuthal angle
				endCapFrontD,    //pDy1 half y length at -pDz
				endCapFrontD,    //pDx1 half x length at -pDz, -pDy1
				endCapFrontD,    //pDx2 half x length at -pDz, +pDy1
				0.00*degree,//pAlpha1 wrt y-axis from the centre of the side (lower endcap)
				endCapBackD,    //pDy2 half y length at +pDz
				endCapBackD,    //pDx3 half x length at +pDz, -pDy2
				endCapBackD,    //pDx4 half x length at +pDz, +pDy2
				0.00*degree); //pAlpha2 wrt y-axis from the centre of the side (upper endcap)

	//the rectangular part.....
	G4Box*        endCapBox  = new G4Box("endCapBox",endCapBackD,endCapBackD,endCapBoxL/2.);
	G4ThreeVector transECBox(   0.*mm, 0.*mm, endCapTaperL/2.+endCapBoxL/2.);

	//add the two together
	solidEndCap = new G4UnionSolid("Box+Taper",solidTaperedCloverEC,endCapBox,0,transECBox);
	//need the taperL for placement
	fEndCapTaperL = endCapTaperL;


	//---------------------------------------------------------
	//end-cap inner vacuum
	G4double endCapDelta_1 = endCapTaperThickness/cos(taperAngle) - endCapFrontThickness*tan(taperAngle);
	//G4double endCapDelta_2 = ( endCapSideThickness - (endCapTaperThickness*sin(taperAngle)*tan(taperAngle) +
	//                         endCapTaperThickness*cos(taperAngle) ) )/tan(taperAngle);

	//G4cout << endCapDelta_1 << " " << endCapDelta_2 << G4endl;

	G4double endCapVacTaperL = endCapTaperL - endCapFrontThickness;// - endCapDelta_2;
	G4double endCapVacBoxL   = endCapBoxL   - endCapFrontThickness;
	G4double endCapVacTotalL = endCapVacBoxL + endCapVacTaperL;
	G4double endCapVacFrontD = endCapFrontD - endCapDelta_1;
	G4double endCapVacBackD  = endCapBackD  - endCapSideThickness;

	//position of vacuum wrt end-cap
	fVacuumPosZ = (-endCapTotalL + endCapVacTotalL )/2. + 1.5*endCapFrontThickness;

	//tapered part...
	G4Trap* solidTaperVac
		= new G4Trap("cloverTaperVac",
				endCapVacTaperL/2.,    //Half z-length [pDz]
				0.00*degree, //Polar angle of line joining centres of the faces @ -/+pDz
				14.0*degree,   //aequivalent zimuthal angle
				endCapVacFrontD,    //pDy1 half y length at -pDz
				endCapVacFrontD,    //pDx1 half x length at -pDz, -pDy1
				endCapVacFrontD,    //pDx2 half x length at -pDz, +pDy1
				0.00*degree,//pAlpha1 wrt y-axis from the centre of the side (lower endcap)
				endCapVacBackD,    //pDy2 half y length at +pDz
				endCapVacBackD,    //pDx3 half x length at +pDz, -pDy2
				endCapVacBackD,    //pDx4 half x length at +pDz, +pDy2
				0.00*degree); //pAlpha2 wrt y-axis from the centre of the side (upper endcap)

	//rectangular part
	G4Box*         endCapVacBox  = new G4Box("endCapBox",endCapVacBackD,endCapVacBackD,endCapVacBoxL/2.);
	G4ThreeVector transVacBox(   0.*mm, 0.*mm, (endCapVacTaperL/2.+endCapVacBoxL/2.-0.0001*mm));

	//add them together
	solidVacuum = new G4UnionSolid("Vac_Box+Taper",solidTaperVac,endCapVacBox,0,transVacBox);


	//---------------------------------------------------------
	//The Ge crystal...
	G4double GeTaperL    = 36.0*mm;
	//G4double GeTotalL    = fTotalGeL;
	//G4double smallSquare = 11.0*mm;
	//G4double largeSquare = 45.5*mm;
	G4double smallSquare = (41.0-lappingSize)*mm;
	G4double largeSquare = (45.5-lappingSize)*mm;

	G4double transX = (largeSquare-smallSquare)/2.;
	G4double transY = (largeSquare-smallSquare)/2.;
	fHole_dX = transX + 5.*mm;  //5 mm is a fudge
	fHole_dY = transY + 5.*mm;  //5 mm is a fudge

	//tapered part......
	G4Trap* solidTaper
		= new G4Trap("cloverTaper",
				GeTaperL/2.,    //Half ? z-length [pDz]
				5.05*degree,   //Polar angle of line joining centres of the faces @ -/+pDz
				45.*degree,   //equivalent azimuthal angle  //DOES NOT MAKE SENSE !!
				smallSquare/2., //pDy1 half y length at -pDz
				smallSquare/2., //pDx1 half x length at -pDz, -pDy1
				smallSquare/2., //pDx2 half x length at -pDz, +pDy1
				0.00*degree,//pAlpha1 wrt y-axis from the centre of the side (lower endcap)
				largeSquare/2.,    //pDy2 half y length at +pDz
				largeSquare/2.,    //pDx3 half x length at +pDz, -pDy2
				largeSquare/2.,    //pDx4 half x length at +pDz, +pDy2
				0.0*degree); //pAlpha2 wrt y-axis from the centre of the side (upper endcap)

	//HERE !!
	const G4int numZPlanesGe=4;      // no. polycone planes

	G4double zPlaneGe[numZPlanesGe]; // positions of planes
	zPlaneGe[0] =  0.00*mm;
	zPlaneGe[1] =  2.46*mm;
	zPlaneGe[2] =  5.00*mm;
	zPlaneGe[3] = GeTaperL;

	G4double rInnerGe[numZPlanesGe]; // interior radii
	rInnerGe[0] = rInnerGe[1] = rInnerGe[2] = rInnerGe[3] = 0.0*mm;
	G4double rOuterGe[numZPlanesGe]; // exterior radii
	//rOuterGe[0] = 20.5*mm;  rOuterGe[1] = 23.54*mm;
	rOuterGe[0] = (20.5/25.)*fCrystalR*mm;  rOuterGe[1] = (23.54/25.)*fCrystalR*mm;
	rOuterGe[2] = rOuterGe[3] = fCrystalR;


	G4Polycone* solidCone = new G4Polycone("cloverCone", 0.0*degree, 360.0*degree,
			numZPlanesGe,
			zPlaneGe,
			rInnerGe,
			rOuterGe);

	G4ThreeVector  transGeCone( -transX/2., -transY/2., -GeTaperL/2.);
	G4IntersectionSolid* taperedCone = new G4IntersectionSolid("Taper+Cone",solidTaper,solidCone,0,transGeCone);

	//back part....
	G4double geBoxL = fTotalGeL - GeTaperL;

	G4Box*    GeBox = new G4Box("GeBox",largeSquare/2.,largeSquare/2.,geBoxL/2.);
	G4Tubs*   GeCyl = new G4Tubs("GeCyl",0.0*mm,fCrystalR,geBoxL/2.,0.*degree,360.*degree);

	G4ThreeVector transGeBox( transX, transY, 0.0*mm);
	G4IntersectionSolid* backPart = new G4IntersectionSolid("Box+Cyl",GeCyl,GeBox,0,transGeBox);

	//add front and back
	G4ThreeVector transBack( -transX/2., -transY/2., (GeTaperL/2.+geBoxL/2.));
	solidGeLeaf = new G4UnionSolid("germanium",taperedCone,backPart,0,transBack);

	//z-position of Ge-leaf wrt vacuum
	fGeLeafPosZ = -endCapVacTaperL/2. + GeTaperL/2. + GeGap - endCapFrontThickness;

	//G4cout << "end-cap : box/2 " << endCapBoxL/2. << " taper/2 " << endCapTaperL/2. << " total/2 " << endCapTotalL << G4endl;
	//G4cout << "vacuum  : box/2 " << endCapVacBoxL/2. << " taper/2 " << endCapVacTaperL/2. << " total/2 " << endCapVacTotalL << G4endl;
	//G4cout << "ge      : box/2 " << geBoxL/2. << " taper/2 " << GeTaperL/2. << " total/2 " << GeTotalL << G4endl;



	//------------------------------------------------------------------
	// Inner bore hole + lithium contact + passivated Ge
	G4double GeDepth      = 15.00 * mm;  //Hole dirilled to this far from face
	G4double passiveThick = fPassiveThick;  //passivated Ge
	G4double contactThick = fContactThick;  //Li contact

	//G4double innerRHole =  0.00*mm;
	G4double holeR      = fHoleR;
	G4double contactR   = holeR + contactThick;
	G4double passiveR   = contactR + passiveThick;
	G4double holeL      = fTotalGeL - GeDepth;
	G4double tubeL      = holeL - holeR;

	//the same translation works for all the following rounded tubes
	G4ThreeVector transSphere(0.01*mm, 0.01*mm, -tubeL/2.-0.1*mm); //if offsets are 0. it does not display !!

	//now add a passivated layer
	G4Sphere* passivatedSphere = new G4Sphere("passSphere", 0.0*mm, passiveR,           0.*degree, 360.*degree, 0.*degree, 180.*degree);
	G4Tubs*   passivatedTube   = new G4Tubs(  "passTube",   0.0*mm, passiveR, tubeL/2., 0.*degree, 360.*degree);
	solidPassivated    = new G4UnionSolid("passivatedGe",passivatedTube,passivatedSphere,0,transSphere);

	//and the Li contact
	G4Sphere* contactSphere  = new G4Sphere("sphere1", 0.0*mm, contactR,           0.*deg, 360.*deg, 0.*deg, 180.*deg);
	G4Tubs*   contactTube    = new G4Tubs(  "tube1",   0.0*mm, contactR, tubeL/2., 0.*deg, 360.*deg);
	solidContact = new G4UnionSolid("liContact",contactTube,contactSphere,0,transSphere);

	//bore out a hole
	G4Sphere* boreSphere  = new G4Sphere(    "boreSphere", 0.0*mm, holeR,           0.*degree, 360.*degree, 0.*degree, 180.*degree);
	G4Tubs*   boreTube    = new G4Tubs(      "boreTube",   0.0*mm, holeR, tubeL/2., 0.*degree, 360.*degree);
	solidBoreHole = new G4UnionSolid("boreHole",boreTube,boreSphere,0,transSphere);

	//save this for placement
	fContact_dZ = holeL/2. - contactThick;// - passiveThick;

	//put corners @ (0,0)
	fGeLeaf_dX = largeSquare/2. - transX/2.;
}
void Jurogam::CreateCloverBGOSolids()
{
	// Form a slicer cube
	// Create a solid box to be used as a ''slicer''
	G4VSolid* SliceBoxSub = new G4Box("SliceBoxSub",1001.*mm,100.5*mm,1001.*mm);
	G4VSolid* SliceBox    = new G4Box("SliceBox",1000.*mm,200.*mm,1000.*mm);
	// Slice the IceCreamCone with a big box from different angles
	G4VSolid* IceCreamSlicer = new G4SubtractionSolid("IceCreamSlicer",SliceBox,SliceBoxSub,0,G4ThreeVector(0,-100.5*mm,0*mm));
	//Rotation of slicer cube
	G4RotationMatrix slicerot1;
	slicerot1.set(0,0,0);
	G4ThreeVector offvec(0,0,0);

	G4double heviangle = -45.*deg;
	G4RotationMatrix hevirotation;
	hevirotation.set(0,0,0);
	hevirotation.rotateZ(heviangle);
	hevirotation.invert();


	// -----------
	// BGO Crystal
	//  -measures were mostly obtained with a ruler from a design drawing
	//  -> approximate !! Actual crystal comprised of many smaller parts,
	//  very hard to model accurately...
	// -----------

	G4VSolid* BGOCrystalTrd1 = new G4Trd("BGOTrd1",
			CloverBGOCrystalW3/2.,     //G4double  dx1,
			CloverBGOCrystalW/2.,      //G4double  dx2,
			CloverBGOCrystalBoxD/2.,   //G4double  dy1,
			CloverBGOCrystalBoxD/2.,   //G4double  dy2,
			CloverBGOCrystalTrapL/2.); //G4double  dz

	G4VSolid* BGOCrystalTrd2 = new G4Trd("BGOTrd",
			CloverBGOCrystalW2/2., //G4double  dx1,
			CloverBGOCrystalW3/2., //G4double  dx2,
			CloverBGOCrystalBoxD2, //G4double  dy1,
			CloverBGOCrystalBoxD, //G4double  dy2,
			CloverBGOCrystalTrapL/2.); // G4double  dz

	G4VSolid* BGOCrystalTrdSubBox = new G4Box("BGOTrd2SubBox",
			CloverBGOCrystalW,
			CloverBGOCrystalBoxD,
			CloverBGOCrystalTrapL);

	G4ThreeVector trans1( 0.*mm, -CloverBGOCrystalBoxD, 0.*mm);
	G4SubtractionSolid* solidBGOTrap
		= new G4SubtractionSolid("solidBGOTrap",BGOCrystalTrd2,
				BGOCrystalTrdSubBox,0,trans1);

	//combine BGO crystal parts
	G4ThreeVector  trans2(0.*mm, CloverBGOCrystalBoxD/2.,
			CloverBGOCrystalTrapL/2.+CloverBGOCrystalTrapL/2.);
	CloverSolidBGOCrystal
		= new G4UnionSolid("solidBGOCrystal",solidBGOTrap,BGOCrystalTrd1,0,trans2);

	// Slice two of the crystals a bit
	slicerot1.set(0,0,0);
	slicerot1.rotateZ(2.35*deg);
	slicerot1.rotateX(7.6*deg);
	offvec = G4ThreeVector(CloverBGOCrystalW2/2.,CloverBGOCrystalBoxD2/2.-0.5*mm,-CloverBGOCrystalTrapL/2.);

	CloverSolidBGOCrystal2 = new G4SubtractionSolid("solidBGOCrystal2",CloverSolidBGOCrystal ,
			IceCreamSlicer,&slicerot1,offvec);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(-2.35*deg);
	slicerot1.rotateX(7.6*deg);
	offvec = G4ThreeVector(-CloverBGOCrystalW2/2.,CloverBGOCrystalBoxD2/2.-0.5*mm,-CloverBGOCrystalTrapL/2.);

	CloverSolidBGOCrystal3 = new G4SubtractionSolid("solidBGOCrystal3",CloverSolidBGOCrystal,
			IceCreamSlicer,&slicerot1,offvec);

	// -------------------------------------------
	// BGO Shield Heavy Metal Collimator (HEVIMET)
	// -------------------------------------------

	// start off with a symmetric part
	double* z = new double[10];  // 10 z-planes used later, ...
	double* ri = new double[10]; // only 2 z-planes here; same variables used
	double* ro = new double[10];

	z[0]  = 0.0;  z[1]  =  35.0;
	ri[0] = 22.5; ri[1] =  28.0;
	ro[0] = 37.5; ro[1] =  47.5;

	G4VSolid* HeviInit = new G4Polyhedra("BGOHevimet", 0.*deg, 360.*deg,4,2,z,ri,ro);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(3.84*deg+heviangle);
	slicerot1.rotateX(14.535875367*deg);
	offvec = G4ThreeVector(0,35.545*mm,0);
	offvec.transform(hevirotation);

	G4VSolid* SlicedHevimet1
		= new G4SubtractionSolid ("SlicedHevimet1",HeviInit,IceCreamSlicer,
				&slicerot1,offvec);

	CloverSolidBGOHevimet = SlicedHevimet1;

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(180.*deg-3.84*deg+heviangle);
	slicerot1.rotateX(14.535875367*deg);
	offvec = G4ThreeVector(0,-35.545*mm,0);
	offvec.transform(hevirotation);

	G4VSolid* SlicedHevimet2
		= new G4SubtractionSolid ("SlicedHevimet2",SlicedHevimet1,IceCreamSlicer,
				&slicerot1,offvec);
	slicerot1.set(0,0,0);
	slicerot1.rotateZ(-90.*deg+heviangle);
	slicerot1.rotateX(14.5*deg); // was 16.023309451
	offvec = G4ThreeVector(-35.5*mm,0,0);
	offvec.transform(hevirotation);

	G4VSolid* SlicedHevimet3
		= new G4SubtractionSolid ("SlicedHevimet3",SlicedHevimet2,IceCreamSlicer,
				&slicerot1,offvec);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(90.*deg+heviangle);
	slicerot1.rotateX(14.5*deg); //16.023309451
	offvec = G4ThreeVector(35.5*mm,0,0);
	offvec.transform(hevirotation);

	G4VSolid* SlicedHevimet4
		= new G4SubtractionSolid ("SlicedHevimet4",SlicedHevimet3,IceCreamSlicer,
				&slicerot1,offvec);

	CloverSolidBGOHevimet = SlicedHevimet4;


	// ****************************************
	// BGO Shield steel part
	// ****************************************

	/* old, whole symmetric shield... */
	// Define coordinates of z-planes and radii for a G4Polyhedra
	/* Old measures using a ruler
	   z[0]  = 0.0; z[1]  =  172.0; z[2] = 195.0; z[3] = 274.5; z[4] = 274.5;
	   z[5] = 297.5; z[6] = 297.5; z[7] = 356.0; z[8] = 356.0; z[9] = 377.5;

	   ri[0] = 39.115; ri[1] = 60.5; ri[2] = 60.5; ri[3] = 60.5; ri[4] = 60.5;
	   ro[0] = 47.5; ro[1] = 95.13; ro[2] = 102; ro[3] = 102.25; ro[4] = 105.5;

	   ri[5] = 60.5; ri[6] = 65.0; ri[7] = 65.0; ri[8] = 65.0; ri[9] = 65.0;
	   ro[5] = 106.5; ro[6] =106.5; ro[7] = 106.5; ro[8] = 122.0; ro[9] = 122.0;
	   */
	// new measures from .iges file (using Inventor)
	z[0]  = 0.0; z[1]  =  172.0; z[2] = 222.5; z[3] = 276.0; z[4] = 276.0;
	z[5] = 298.5; z[6] = 298.5; z[7] = 357.0; z[8] = 357.0; z[9] = 377.0;

	ri[0] = 39.115; ri[1] = 60.5; ri[2] = 60.5; ri[3] = 60.5; ri[4] = 60.5;
	ro[0] = 47.5; ro[1] = 95.13; ro[2] = 102.25; ro[3] = 102.25; ro[4] = 105.5;

	ri[5] = 60.5; ri[6] = 65.0; ri[7] = 65.0; ri[8] = 65.0; ri[9] = 65.0;
	ro[5] = 107.0; ro[6] =107.0; ro[7] = 107.0; ro[8] = 122.0; ro[9] = 122.0;

	G4VSolid* BGOShieldInit = new G4Polyhedra("BGOShieldInit", 0.*deg, 360.*deg,4,10,z,ri,ro);

	// Slice the shield !
	G4double shieldangle = -45.*deg;
	G4RotationMatrix shieldrotation;
	shieldrotation.set(0,0,0);
	shieldrotation.rotateZ(shieldangle);
	shieldrotation.invert();


	slicerot1.set(0,0,0);
	slicerot1.rotateZ(3.84*deg+shieldangle);
	slicerot1.rotateX(14.535875367*deg); // 14.291265703 from .igs, but results in more empty space
	offvec = G4ThreeVector(0,44.625*mm,0);
	offvec.transform(shieldrotation);

	G4VSolid* SlicedShield1
		= new G4SubtractionSolid ("SlicedShield1",BGOShieldInit,IceCreamSlicer,
				&slicerot1,offvec);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(180.*deg-3.84*deg+shieldangle);
	slicerot1.rotateX(14.535875367*deg); // 14.291265703 from .igs, but results in more empty space
	offvec = G4ThreeVector(0,-44.625*mm,0);
	offvec.transform(shieldrotation);

	G4VSolid* SlicedShield2
		= new G4SubtractionSolid ("SlicedShield2",SlicedShield1,IceCreamSlicer,
				&slicerot1,offvec);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(-90.*deg+shieldangle);
	slicerot1.rotateX(14.49*deg);
	offvec = G4ThreeVector(-44.625*mm,0,0);
	offvec.transform(shieldrotation);

	G4VSolid* SlicedShield3
		= new G4SubtractionSolid ("SlicedShield3",SlicedShield2,IceCreamSlicer,
				&slicerot1,offvec);

	slicerot1.set(0,0,0);
	slicerot1.rotateZ(90.*deg+shieldangle);
	slicerot1.rotateX(14.49*deg);
	offvec = G4ThreeVector(44.625*mm,0,0);
	offvec.transform(shieldrotation);

	G4VSolid* SlicedShield4
		= new G4SubtractionSolid ("SlicedShield4",SlicedShield3,IceCreamSlicer,
				&slicerot1,offvec);

	CloverSolidBGOShield = SlicedShield4;

	//------------------------------------------------------------------
	// Germanium absorbers
	// 1. Sn 0.1 mm (closer to target)
	// 2. Cu 0.5 mm
	//------------------------------------------------------------------
	//abs1_thickness = 0.1*mm;
	//abs1_thickness = 0.26*mm;
	abs1_thickness = 0.26*mm;
	//abs2_thickness = 0.5*mm;
	//abs2_thickness = 0.625*mm;
	abs2_thickness = 0.625*mm;
	AbsorberWidth  = 70.*mm;
	// The rotation of the ''box'' shapes will be 45degrees wrong, and changing it is difficult...
	// -> Quick solution: make them round and just cover the hole in the collimator
	//CloverAbsorber1 = new G4Box("CloverAbsorber1", AbsorberWidth/2., AbsorberWidth/2., abs1_thickness/2. );
	//CloverAbsorber2 = new G4Box("CloverAbsorber2", AbsorberWidth/2., AbsorberWidth/2., abs2_thickness/2. );
	CloverAbsorber1 = new G4Tubs("CloverAbsorber1", 0., AbsorberWidth/2., abs1_thickness/2. , 0., 360.*deg);
	CloverAbsorber2 = new G4Tubs("CloverAbsorber2", 0., AbsorberWidth/2., abs2_thickness/2. , 0., 360.*deg);

}
//---------------------------------------------------------------------
// Building logical volumes
//---------------------------------------------------------------------
G4LogicalVolume* Jurogam::BuildPhaseI()
{
	if(!logicGe_PhaseI){
		//Create physical instance of a Phase-I detector
		logicAlCap_PhaseI = new G4LogicalVolume(solidAlCap_PhaseI, endCapMaterial, "capAl", 0, 0, 0);
		//vacuum between AlCap and Ge
		logicVacuum_PhaseI = new G4LogicalVolume(solidVacuum_PhaseI, vacuumMaterial, "vacuum", 0, 0, 0);
		//the gremanium
		logicGe_PhaseI = new G4LogicalVolume(solidGe_PhaseI, geMaterial, "germanium", 0, 0, 0);
		//passivated rgion between Ge-dector and Li-contact
		logicPassivated_PhaseI = new G4LogicalVolume(solidPassivated_PhaseI, geMaterial, "passiveGe", 0, 0, 0);
		//the lithium contact
		logicContact_PhaseI = new G4LogicalVolume(solidContact_PhaseI, contactMaterial, "lihium", 0, 0, 0); //should be Li
		//the lithium contact
		logicBoreHole_PhaseI = new G4LogicalVolume(solidBoreHole_PhaseI, vacuumMaterial, "hole", 0, 0, 0); //should be Li
		logicGe_PhaseI->SetSensitiveDetector(m_JurogamScorer);
	}
	return logicGe_PhaseI;
}
G4LogicalVolume* Jurogam::BuildPhaseIBGO()
{
	if(!PhaseILogicBGOCrystal)
	{
		PhaseILogicBGOShield = new G4LogicalVolume(PhaseISolidBGOShield, ShieldMaterial,"PhaseIBGOShield",0,0,0);
		PhaseILogicBGOCrystal = new G4LogicalVolume(PhaseISolidBGOCrystal, BGOMaterial,
				"PhaseIBGOCrystal",0,0,0);
		PhaseILogicBGOHevimet = new G4LogicalVolume(PhaseISolidBGOHevimet, HevimetMaterial,
				"BGOHevimetColl", 0, 0, 0);

		Phase1Absorber1_log = new G4LogicalVolume(Phase1Absorber1, Absorber1Material,"Phase1Absorber1_log",0,0,0);
		Phase1Absorber2_log = new G4LogicalVolume(Phase1Absorber2, Absorber2Material,"Phase1Absorber2_log",0,0,0);
	PhaseILogicBGOCrystal->SetSensitiveDetector(m_JurogamBGOScorer);
	}
	return PhaseILogicBGOCrystal;
}
G4LogicalVolume* Jurogam::BuildClover()
{
	if(!CloverConstructed){
		logicEndCap = new G4LogicalVolume(solidEndCap, endCapMaterial,   "clover_EC",   0, 0, 0);
		logicVacuum = new G4LogicalVolume(solidVacuum, vacuumMaterial,   "clover_Vac",  0, 0, 0);

		for(G4int l = 0; l < 4; l++)
		{
			logicGeLeaf[l]     = new G4LogicalVolume(solidGeLeaf,     geMaterial,      "clover_Leaf",   0, 0, 0);
			logicGeLeaf[l]->SetSensitiveDetector(m_JurogamScorer);
			logicPassivated[l] = new G4LogicalVolume(solidPassivated,    geMaterial,      "passivatedGe",  0, 0, 0);
			logicContact[l]    = new G4LogicalVolume(solidContact, contactMaterial, "inner_contact", 0, 0, 0);
			logicBoreHole[l]   = new G4LogicalVolume(solidBoreHole,   vacuumMaterial,   "bore_hole",    0, 0, 0);
		}
		CloverConstructed = true;
	}
	G4bool checkOverlaps = false;
	G4int copyNo = 0;
	G4double vacuum_PosZ = fVacuumPosZ;
	G4double geLeaf_PosZ = fGeLeafPosZ;
	physiVacuum = new G4PVPlacement(0,                   //rotation
			G4ThreeVector(0.*mm,0.*mm,vacuum_PosZ),
			logicVacuum, //its logical volume
			"Clover_Vac",       //its name
			logicEndCap, //its mother
			false,                //no boolean operat
			copyNo,              //copy number
			checkOverlaps);               //overlap check

	//define these here
	G4VisAttributes* visAttAlCap = new G4VisAttributes( G4Colour(0.9,0.9,0.9,1.0) );
	visAttAlCap->SetVisibility(true);
	//visAttAlCap->SetForceWireframe(true);

	G4VisAttributes* visAttGeVac = new G4VisAttributes( G4Colour(0.9,1.0,0.9) );
	visAttGeVac->SetForceWireframe(true);
	visAttGeVac->SetVisibility(false);

	G4VisAttributes* visAttActive = new G4VisAttributes( G4Colour(1.0,1.0,0.0) );
	//visAttActive->SetForceWireframe(true);
	visAttActive->SetVisibility(true);
	G4VisAttributes* visAttActive0 = new G4VisAttributes( G4Colour(1.0,0.5,0.5) );
	//visAttActive->SetForceWireframe(true);
	visAttActive->SetVisibility(true);
	G4VisAttributes* visAttActive1 = new G4VisAttributes( G4Colour(0.5,1.0,0.5) );
	//visAttActive->SetForceWireframe(true);
	visAttActive->SetVisibility(true);
	G4VisAttributes* visAttActive2 = new G4VisAttributes( G4Colour(0.5,0.5,1.0) );
	//visAttActive->SetForceWireframe(true);
	visAttActive->SetVisibility(true);
	G4VisAttributes* visAttActive3 = new G4VisAttributes( G4Colour(0.0,1.0,1.0) );
	//visAttActive->SetForceWireframe(true);
	visAttActive->SetVisibility(true);

	G4VisAttributes* visAttPassive = new G4VisAttributes(G4Colour(0.0,1.0,1.0) );
	visAttPassive->SetForceWireframe(true);
	visAttPassive->SetVisibility(false);

	G4VisAttributes* visAttLiContact = new G4VisAttributes(G4Colour(1.0,0.0,1.0) );
	//visAttLiContact->SetVisibility(false);
	visAttLiContact->SetVisibility(true);

	G4VisAttributes* visAttHole = new G4VisAttributes( G4Colour(1.0,0.0,1.0) );
	//visAttHole->SetVisibility(false);
	visAttHole->SetVisibility(true);

	logicEndCap->SetVisAttributes(visAttAlCap);
	logicVacuum->SetVisAttributes(visAttGeVac);

	//Now for the placement of the leaves in each clover......
	G4RotationMatrix* rmC;
	G4double dPos = fGeLeaf_dX + fGapBetweenLeaves/2.;
	G4double leafX;
	G4double leafY;
	G4double leafZ;

	for(G4int l = 0; l < 4; l++) {
		//the rotation
		rmC = new G4RotationMatrix;
		rmC->set(0,0,0);
		rmC->rotateZ(90.*degree*(4-l));
		rmC->invert();
		//the x-translation
		if(l < 2) {
			leafX = dPos;
		} else {
			leafX = -dPos;
		}
		//the y-translation
		if(l == 0 || l == 3 ) {
			leafY = dPos;
		} else {
			leafY = -dPos;
		}
		//the z-translation
		leafZ = geLeaf_PosZ;


		physiGeLeaf[l] = new G4PVPlacement(rmC,                       //rotation
				G4ThreeVector(leafX,leafY,leafZ),
				"Clover",                 //its name
				logicGeLeaf[l],     //its logical volume
				physiVacuum,        //its mother
				true,                       //no boolean operat
				copyNo*4+l,                     //copy number
				checkOverlaps);                      //overlap check

		physiPassivated[l] = new G4PVPlacement(0,                   //rotation
				G4ThreeVector(-fHole_dX, -fHole_dY, fContact_dZ),
				"GePassivated",
				logicPassivated[l],
				physiGeLeaf[l],
				false,copyNo*4+l,checkOverlaps);

		physiContact[l] = new G4PVPlacement(0,                   //rotation
				G4ThreeVector(0.*mm,0.*mm, 0.0*mm),//-fContact_dZ),
			"LiContact",
			logicContact[l],
			physiPassivated[l],
			false,copyNo*4+l,checkOverlaps);


			physiBoreHole[l] = new G4PVPlacement(0,                   //rotation
					G4ThreeVector(0.*mm,0.*mm, 0.0*mm),//-fContact_dZ),
				"BoreHole",
				logicBoreHole[l],
				physiContact[l],
				false,copyNo*4+l,checkOverlaps);


				//visual attributes
				//logicGeLeaf[l]->SetVisAttributes(visAttActive);
				switch (l) {
					case 0:
						logicGeLeaf[l]->SetVisAttributes(visAttActive0);
						break;
					case 1:
						logicGeLeaf[l]->SetVisAttributes(visAttActive1);
						break;
					case 2:
						logicGeLeaf[l]->SetVisAttributes(visAttActive2);
						break;
					case 3:
						logicGeLeaf[l]->SetVisAttributes(visAttActive3);
						break;
					default:
						logicGeLeaf[l]->SetVisAttributes(visAttActive);
				}

				logicPassivated[l]->SetVisAttributes(visAttPassive);
				logicContact[l]->SetVisAttributes(visAttLiContact);
				logicBoreHole[l]->SetVisAttributes(visAttHole);
	}
	return *logicGeLeaf;
}
G4LogicalVolume* Jurogam::BuildCloverBGO()
{
	if(!CloverBGOConstructed){
		CloverLogicBGOCrystal[0] = new G4LogicalVolume(CloverSolidBGOCrystal2, BGOMaterial,
				"CloverBGOCrystal",0,0,0);
		CloverLogicBGOCrystal[1] = new G4LogicalVolume(CloverSolidBGOCrystal, BGOMaterial,
				"CloverBGOCrystal",0,0,0);
		CloverLogicBGOCrystal[2] = new G4LogicalVolume(CloverSolidBGOCrystal3, BGOMaterial,
				"CloverBGOCrystal",0,0,0);
		CloverLogicBGOCrystal[3] = new G4LogicalVolume(CloverSolidBGOCrystal, BGOMaterial,
				"CloverBGOCrystal",0,0,0);
		for(G4int l = 0; l < 4; l++)
		{
			CloverLogicBGOCrystal[l]->SetSensitiveDetector(m_JurogamBGOScorer);
		}


		CloverLogicBGOShield = new G4LogicalVolume(CloverSolidBGOShield, ShieldMaterial,
				"BGOShield", 0, 0, 0);

		CloverLogicBGOHevimet = new G4LogicalVolume(CloverSolidBGOHevimet, HevimetMaterial,
				"BGOHevimetColl", 0, 0, 0);

		CloverAbsorber1_log = new G4LogicalVolume(CloverAbsorber1,Absorber1Material,"CloverAbsorber1_log",0,0,0);
		CloverAbsorber2_log = new G4LogicalVolume(CloverAbsorber2,Absorber2Material,"CloverAbsorber2_log",0,0,0);
		CloverBGOConstructed = true;
	}
	return *CloverLogicBGOCrystal;
}
//---------------------------------------------------------------------
// Placing detectors
//---------------------------------------------------------------------
void Jurogam::PhaseIPlacement(G4int copyNo, G4LogicalVolume* logiMother, G4bool checkOverlaps)
{
	cout << "Placing phase I at\n";
	cout << "X " << position.getX() << " Y " << position.getY() << " Z " << position.getZ() << "\n";
	cout << "Theta " << rotation.getTheta() << " Phi " << rotation.getPhi() << " Psi " << rotation.getPsi() << "\n\n";
	physiAlCap_PhaseI = new G4PVPlacement(//&rotation,G4ThreeVector(position.x(),position.y(),position.z()),
			G4Transform3D(rotation, position),
			logicAlCap_PhaseI, //its logical volume
			"CapAl",           //its name
			logiMother,       //its mother
			false,             //no boolean operat
			copyNo,            //copy number
			checkOverlaps);             //overlap check

	physiVacuum_PhaseI = new G4PVPlacement(0, G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm),
			"Vacuum",           //its name
			logicVacuum_PhaseI, //its logical volume
			physiAlCap_PhaseI,  //its mother
			false,              //no boolean operat
			copyNo,            //copy number
			checkOverlaps);             //overlap check


	G4double geZ = fAlCap2Ge_PhaseI + fEndCapThickness_PhaseI;//-delta
	physiGe_PhaseI = new G4PVPlacement(0, G4ThreeVector(0.0*mm, 0.0*mm, geZ),
			//theDetector.data(),         //its name
			"Tapered",         //its name
			logicGe_PhaseI,        //its logical volume
			physiVacuum_PhaseI,    //its mother
			false,             //no boolean operat
			copyNo,            //copy number
			checkOverlaps);             //overlap check


	physiPassivated_PhaseI = new G4PVPlacement(0,
			G4ThreeVector(0.0*mm,0.0*mm,fContact_dZ_PhaseI),
			"PassiveGe",    //its name
			logicPassivated_PhaseI, //its logical volume
			physiGe_PhaseI,        //its mother
			false,          //no boolean operat
			copyNo,              //copy number
			checkOverlaps);          //overlap check


	physiContact_PhaseI = new G4PVPlacement(0, G4ThreeVector(0.0*mm,0.0*mm,0.0*mm), //dz = contact-thick ?
			"LiContact",         //its name
			logicContact_PhaseI, //its logical volume
			physiPassivated_PhaseI,        //its mother (I think it should be physiGe_PhaseI)
			false,          //no boolean operat
			copyNo,              //copy number
			checkOverlaps);          //overlap check



	physiBoreHole_PhaseI = new G4PVPlacement(0, G4ThreeVector(0.0*mm,0.0*mm,0.0*mm), //dz = contact-thick ?
			"BoreHole",         //its name
			logicBoreHole_PhaseI, //its logical volume
			physiContact_PhaseI,
			false,          //no boolean operat
			copyNo,              //copy number
			checkOverlaps);          //overlap check

	//a The active layer will appear in yellow colour.
	G4VisAttributes* visAttActive = new G4VisAttributes( G4Colour(1.0,1.0,0.0) );
	//visAttActive->SetForceWireframe(true);
	visAttActive->SetVisibility(true);

	G4VisAttributes* visAttHole = new G4VisAttributes( G4Colour(1.0,0.0,1.0) );
	visAttHole->SetVisibility(true);
	//visAttHole->SetVisibility(false);

	G4VisAttributes* visAttGeVac = new G4VisAttributes( G4Colour(0.9,1.0,0.9) );
	visAttGeVac->SetForceWireframe(true);
	//visAttGeVac->SetVisibility(true);
	visAttGeVac->SetVisibility(true);

	G4VisAttributes* visAttPassive = new G4VisAttributes(G4Colour(0.0,1.0,1.0) );
	//visAttPassive->SetVisibility(true);
	visAttPassive->SetVisibility(true);

	G4VisAttributes* visAttAlCap = new G4VisAttributes( G4Colour(0.9,1.0,1.0) );
	visAttAlCap->SetVisibility(true);
	//visAttAlCap->SetForceWireframe(true);

	G4VisAttributes* visAttBGO = new G4VisAttributes(G4Colour(0.5,1.0,1.0) );
	visAttBGO->SetVisibility(true);
	//visAttBGO->SetForceWireframe(true);

	logicGe_PhaseI->SetVisAttributes(visAttActive);
	logicBoreHole_PhaseI->SetVisAttributes(visAttPassive);
	logicPassivated_PhaseI->SetVisAttributes(visAttPassive);
	logicContact_PhaseI->SetVisAttributes(visAttHole);
	logicVacuum_PhaseI->SetVisAttributes(visAttGeVac);
	logicAlCap_PhaseI->SetVisAttributes(visAttAlCap);
}
void Jurogam::PhaseIBGOPlacement(G4int copyNo, G4LogicalVolume* logiMother, G4bool checkOverlaps)
{
	G4double hmetradius = position.mag()+PhaseIBGOHevimetThickness/2.;
	G4ThreeVector hevimetpos( (hmetradius)*sin(position.getTheta())*cos(position.getPhi()),
			(hmetradius)*sin(position.getTheta())*sin(position.getPhi()),
			(hmetradius)*cos(position.getTheta()) );


	PhaseIPhysiBGOHevimet = new G4PVPlacement(//&rotation, hevimetpos,
			G4Transform3D(rotation, hevimetpos),
			PhaseILogicBGOHevimet,
			"PhaseIBGOHevimetColl",
			logiMother,
			true,
			copyNo,
			checkOverlaps);

	if (useAbsorber) {
		G4double abs1radius = position.mag() - abs1_thickness/2. - abs2_thickness - 0.1*mm;
		G4ThreeVector abs1pos( (abs1radius)*sin(position.getTheta())*cos(position.getPhi()),
				(abs1radius)*sin(position.getTheta())*sin(position.getPhi()),
				(abs1radius)*cos(position.getTheta()) );

		Phase1Absorber1_phys = new G4PVPlacement(//&rotation, abs1pos,
				G4Transform3D(rotation,abs1pos),
				Phase1Absorber1_log,    //its logical volume
				"Phase1Absorber1_phys", //its name
				logiMother,            //its mother
				false,                  //no boolean operat
				copyNo,                 //copy number
				checkOverlaps);         //overlap check

		G4double abs2radius = position.mag() - abs2_thickness/2. - 0.05*mm;
		G4ThreeVector abs2pos( (abs2radius)*sin(position.getTheta())*cos(position.getPhi()),
				(abs2radius)*sin(position.getTheta())*sin(position.getPhi()),
				(abs2radius)*cos(position.getTheta()) );

		Phase1Absorber2_phys = new G4PVPlacement(//&rotation, abs2pos,
				G4Transform3D(rotation,abs2pos),
				Phase1Absorber2_log,    //its logical volume
				"Phase1Absorber2_phys", //its name
				logiMother,            //its mother
				false,                  //no boolean operat
				copyNo,                 //copy number
				checkOverlaps);         //overlap check
	} //if (useAbsorber)


	G4double shieldradius = position.mag()+PhaseIBGOShieldLength/2.+PhaseIBGOShieldDist;
	G4ThreeVector shieldpos((shieldradius)*sin(position.getTheta())*cos(position.getPhi()),
			(shieldradius)*sin(position.getTheta())*sin(position.getPhi()),
			(shieldradius)*cos(position.getTheta()));

	PhaseIPhysiBGOShield = new G4PVPlacement(//&rotation, //G4ThreeVector(position.x(),position.y(),position.z()), shieldpos,
			G4Transform3D(rotation,shieldpos),
			//G4ThreeVector(0,0,0),
			PhaseILogicBGOShield,
			"PhaseIBGOShield",
			logiMother,
			true,
			copyNo,
			checkOverlaps);


	G4RotationMatrix* crysrot = new G4RotationMatrix;
	crysrot->set(0,0,0);
	crysrot->rotateZ(-18.*deg);
	G4ThreeVector crystrans(0,0,-PhaseIBGOShieldLength/2.+0.7*mm);

	PhaseIPhysiBGOCrystal = new G4PVPlacement(crysrot,
			crystrans,
			"PhaseIBGOCrystal",
			PhaseILogicBGOCrystal,
			PhaseIPhysiBGOShield,
			false,
			copyNo,
			checkOverlaps);

	// Visualization attributes

	double hevimet_alpha = 0.5;
	double shield_alpha = 0.3;
	double bgo_alpha = 0.4;
	int usealpha = 0;
	if (!usealpha) {
		hevimet_alpha = shield_alpha = bgo_alpha = 1.0;
	}

	G4VisAttributes *grayVA = new G4VisAttributes(G4Colour(0.8,0.8,0.8,1.0));
	grayVA->SetVisibility(true);
	Phase1Absorber1_log->SetVisAttributes(grayVA);

	G4VisAttributes *orangeVA = new G4VisAttributes(G4Colour(0.8,0.4,0.));
	orangeVA->SetVisibility(true);
	Phase1Absorber2_log->SetVisAttributes(orangeVA);

	G4VisAttributes* visAttBGOHevimet =new G4VisAttributes(G4Colour(0.9,0.5,0.5,hevimet_alpha));
	visAttBGOHevimet->SetVisibility(true);
	visAttBGOHevimet->SetForceWireframe(false);
	PhaseILogicBGOHevimet->SetVisAttributes(visAttBGOHevimet);

	G4VisAttributes* visAttBGOShield =new G4VisAttributes(G4Colour(0.9,0.8,1.0,shield_alpha));
	visAttBGOShield->SetVisibility(true);
	visAttBGOShield->SetForceWireframe(false);
	PhaseILogicBGOShield->SetVisAttributes(visAttBGOShield);

	G4VisAttributes *visAttBGOCrystal = new G4VisAttributes(G4Colour(0.4,0.4,0.9,bgo_alpha));
	//visAttBGOCrystal->SetVisibility(false);
	visAttBGOCrystal->SetVisibility(true);
	visAttBGOCrystal->SetForceWireframe(false);
	PhaseILogicBGOCrystal->SetVisAttributes(visAttBGOCrystal);

}
void Jurogam::CloverPlacement(G4int copyNo, G4LogicalVolume* logiMother, G4bool checkOverlaps)
{
	//=================================================================================
	//Do not know why, but the positioning seems to be with respect to the Taper-part :
	//setting the z-position as endCapTaperL/2 puts the front face at z = 0 mm
	//=================================================================================
	//G4double vacuum_PosZ = fVacuumPosZ;
	//G4double geLeaf_PosZ = fGeLeafPosZ;

	physiEndCap = new G4PVPlacement(//&rotation, G4ThreeVector(position.x(),position.y(),position.z()),
			G4Transform3D(rotation, position),
			logicEndCap,//its logical volume
			"Clover_EC",       //its name
			logiMother,         //its mother
			true,               //no boolean operat
			copyNo,             //copy number
			checkOverlaps);              //overlap check

	//physiVacuum = new G4PVPlacement(0,                   //rotation
	//		G4ThreeVector(0.*mm,0.*mm,vacuum_PosZ),
	//		"Clover_Vac",       //its name
	//		logicVacuum, //its logical volume
	//		physiEndCap, //its mother
	//		false,                //no boolean operat
	//		copyNo,              //copy number
	//		checkOverlaps);               //overlap check

	////define these here
	//G4VisAttributes* visAttAlCap = new G4VisAttributes( G4Colour(0.9,0.9,0.9,1.0) );
	//visAttAlCap->SetVisibility(true);
	////visAttAlCap->SetForceWireframe(true);

	//G4VisAttributes* visAttGeVac = new G4VisAttributes( G4Colour(0.9,1.0,0.9) );
	//visAttGeVac->SetForceWireframe(true);
	//visAttGeVac->SetVisibility(false);

	//G4VisAttributes* visAttActive = new G4VisAttributes( G4Colour(1.0,1.0,0.0) );
	////visAttActive->SetForceWireframe(true);
	//visAttActive->SetVisibility(true);
	//G4VisAttributes* visAttActive0 = new G4VisAttributes( G4Colour(1.0,0.5,0.5) );
	////visAttActive->SetForceWireframe(true);
	//visAttActive->SetVisibility(true);
	//G4VisAttributes* visAttActive1 = new G4VisAttributes( G4Colour(0.5,1.0,0.5) );
	////visAttActive->SetForceWireframe(true);
	//visAttActive->SetVisibility(true);
	//G4VisAttributes* visAttActive2 = new G4VisAttributes( G4Colour(0.5,0.5,1.0) );
	////visAttActive->SetForceWireframe(true);
	//visAttActive->SetVisibility(true);
	//G4VisAttributes* visAttActive3 = new G4VisAttributes( G4Colour(0.0,1.0,1.0) );
	////visAttActive->SetForceWireframe(true);
	//visAttActive->SetVisibility(true);

	//G4VisAttributes* visAttPassive = new G4VisAttributes(G4Colour(0.0,1.0,1.0) );
	//visAttPassive->SetForceWireframe(true);
	//visAttPassive->SetVisibility(false);

	//G4VisAttributes* visAttLiContact = new G4VisAttributes(G4Colour(1.0,0.0,1.0) );
	////visAttLiContact->SetVisibility(false);
	//visAttLiContact->SetVisibility(true);

	//G4VisAttributes* visAttHole = new G4VisAttributes( G4Colour(1.0,0.0,1.0) );
	////visAttHole->SetVisibility(false);
	//visAttHole->SetVisibility(true);

	//logicEndCap->SetVisAttributes(visAttAlCap);
	//logicVacuum->SetVisAttributes(visAttGeVac);

	////Now for the placement of the leaves in each clover......
	//G4RotationMatrix* rmC;
	//G4double dPos = fGeLeaf_dX + fGapBetweenLeaves/2.;
	//G4double leafX;
	//G4double leafY;
	//G4double leafZ;

	//for(G4int l = 0; l < 4; l++) {
	//	//the rotation
	//	rmC = new G4RotationMatrix;
	//	rmC->set(0,0,0);
	//	rmC->rotateZ(90.*degree*(4-l));
	//	rmC->invert();
	//	//the x-translation
	//	if(l < 2) {
	//		leafX = dPos;
	//	} else {
	//		leafX = -dPos;
	//	}
	//	//the y-translation
	//	if(l == 0 || l == 3 ) {
	//		leafY = dPos;
	//	} else {
	//		leafY = -dPos;
	//	}
	//	//the z-translation
	//	leafZ = geLeaf_PosZ;


	//	physiGeLeaf[l] = new G4PVPlacement(rmC,                       //rotation
	//			G4ThreeVector(leafX,leafY,leafZ),
	//			"Clover",                 //its name
	//			logicGeLeaf[l],     //its logical volume
	//			physiVacuum,        //its mother
	//			true,                       //no boolean operat
	//			copyNo*4+l,                     //copy number
	//			checkOverlaps);                      //overlap check

	//	physiPassivated[l] = new G4PVPlacement(0,                   //rotation
	//			G4ThreeVector(-fHole_dX, -fHole_dY, fContact_dZ),
	//			"GePassivated",
	//			logicPassivated[l],
	//			physiGeLeaf[l],
	//			false,copyNo*4+l,checkOverlaps);

	//	physiContact[l] = new G4PVPlacement(0,                   //rotation
	//			G4ThreeVector(0.*mm,0.*mm, 0.0*mm),//-fContact_dZ),
	//		"LiContact",
	//		logicContact[l],
	//		physiPassivated[l],
	//		false,copyNo*4+l,checkOverlaps);


	//		physiBoreHole[l] = new G4PVPlacement(0,                   //rotation
	//				G4ThreeVector(0.*mm,0.*mm, 0.0*mm),//-fContact_dZ),
	//			"BoreHole",
	//			logicBoreHole[l],
	//			physiContact[l],
	//			false,copyNo*4+l,checkOverlaps);


	//			//visual attributes
	//			//logicGeLeaf[l]->SetVisAttributes(visAttActive);
	//			switch (l) {
	//				case 0:
	//					logicGeLeaf[l]->SetVisAttributes(visAttActive0);
	//					break;
	//				case 1:
	//					logicGeLeaf[l]->SetVisAttributes(visAttActive1);
	//					break;
	//				case 2:
	//					logicGeLeaf[l]->SetVisAttributes(visAttActive2);
	//					break;
	//				case 3:
	//					logicGeLeaf[l]->SetVisAttributes(visAttActive3);
	//					break;
	//				default:
	//					logicGeLeaf[l]->SetVisAttributes(visAttActive);
	//			}

	//			logicPassivated[l]->SetVisAttributes(visAttPassive);
	//			logicContact[l]->SetVisAttributes(visAttLiContact);
	//			logicBoreHole[l]->SetVisAttributes(visAttHole);
	//}
}
void Jurogam::CloverBGOPlacement(G4int copyNo, G4LogicalVolume* logiMother, G4bool checkOverlaps)
{
	G4RotationMatrix rms;
	rms.rotateZ(45.0*deg);
	rms.transform(rotation);
	//rms.set(0,0,0);
	//rms.setPhi(45.0*deg);

	//G4RotationMatrix hevirot;
	//hevirot.set(0,0,0); hevirot.rotateZ(90.0*deg);

	//G4double hradius = position.mag()+CloverBGOHevimetThickness/2.;
	//G4ThreeVector hevimetpos( (hradius)*sin(position.getTheta())*cos(position.getPhi()),
	//		(hradius)*sin(position.getTheta())*sin(position.getPhi()),
	//		(hradius)*cos(position.getTheta()) );
	G4ThreeVector hevimetpos( (position.mag())*sin(position.getTheta())*cos(position.getPhi()),
			(position.mag())*sin(position.getTheta())*sin(position.getPhi()),
			(position.mag())*cos(position.getTheta()) );

	//rotation.transform(rms);

	CloverPhysiBGOHevimet = new G4PVPlacement(//&rotation, //G4ThreeVector(0,0,-35.*mm), //hevimetpos,
			G4Transform3D(rms,hevimetpos),
			CloverLogicBGOHevimet,
			"CloverBGOHevimetColl",
			logiMother,
			true,
			copyNo,
			checkOverlaps);


	if (useAbsorber) {
		G4double abs1radius = position.mag() - abs1_thickness/2. - abs2_thickness - 0.1*mm;

		G4ThreeVector abs1pos( (abs1radius)*sin(position.getTheta())*cos(position.getPhi()),
				(abs1radius)*sin(position.getTheta())*sin(position.getPhi()),
				(abs1radius)*cos(position.getTheta()) );

		CloverAbsorber1_phys = new G4PVPlacement(//&rotation, abs1pos,
				G4Transform3D(rms,abs1pos),
				CloverAbsorber1_log,    //its logical volume
				"CloverAbsorber1_phys", //its name
				logiMother,            //its mother
				false,                  //no boolean operat
				copyNo,                 //copy number
				checkOverlaps);         //overlap check



		G4double abs2radius = position.mag() - abs2_thickness/2. - 0.05*mm;
		G4ThreeVector abs2pos( (abs2radius)*sin(position.getTheta())*cos(position.getPhi()),
				(abs2radius)*sin(position.getTheta())*sin(position.getPhi()),
				(abs2radius)*cos(position.getTheta()) );

		CloverAbsorber2_phys = new G4PVPlacement(//&rotation, abs2pos,
				G4Transform3D(rms,abs2pos),
				CloverAbsorber2_log,    //its logical volume
				"CloverAbsorber2_phys", //its name
				logiMother,            //its mother
				false,                  //no boolean operat
				copyNo,                 //copy number
				checkOverlaps);         //overlap check
	}


	G4double sradius = position.mag()+CloverBGOHevimetThickness;
	G4ThreeVector shieldpos((sradius)*sin(position.getTheta())*cos(position.getPhi()),
			(sradius)*sin(position.getTheta())*sin(position.getPhi()),
			(sradius)*cos(position.getTheta()));

	CloverPhysiBGOShield = new G4PVPlacement(//&rotation, //G4ThreeVector(0,0,0), //G4ThreeVector(position.x(),position.y(),position.z()), shieldpos,
			G4Transform3D(rms, shieldpos),
			CloverLogicBGOShield,
			"CloverBGOShield",
			logiMother,
			true,
			copyNo,
			checkOverlaps);


	G4RotationMatrix ShieldRotation; // shield coords 45*deg rotated wrt. world
	ShieldRotation.set(0,0,0);
	ShieldRotation.rotateZ(45.*deg);
	G4double rotoffset = 45.*deg;
	G4double DistOffset = 0.*mm;

	G4ThreeVector crystrans0(0,CloverBGOCrystalH,CloverBGOCrystalDist-DistOffset);
	crystrans0.transform(ShieldRotation);
	G4RotationMatrix* rmc0 = new G4RotationMatrix;
	rmc0->set(0,0,0);
	rmc0->setPhi(0.*deg+rotoffset);
	rmc0->rotateX(CloverBGOCrystalAngle);
	CloverPhysiBGOCrystal[0] = new G4PVPlacement(rmc0,
			crystrans0,
			"CloverBGOCrystal",
			CloverLogicBGOCrystal[0],
			CloverPhysiBGOShield,
			false,
			copyNo*4,
			checkOverlaps);

	G4ThreeVector crystrans1(CloverBGOCrystalH,0,CloverBGOCrystalDist-DistOffset);
	crystrans1.transform(ShieldRotation);
	G4RotationMatrix* rmc1 = new G4RotationMatrix;
	rmc1->set(0,0,0);
	rmc1->setPhi(270.*deg+rotoffset);
	rmc1->rotateX(CloverBGOCrystalAngle);
	CloverPhysiBGOCrystal[1] = new G4PVPlacement(rmc1,
			crystrans1,
			"CloverBGOCrystal",
			CloverLogicBGOCrystal[1],
			CloverPhysiBGOShield,
			false,
			copyNo*4+1,
			checkOverlaps);


	G4ThreeVector crystrans2(0,-CloverBGOCrystalH,CloverBGOCrystalDist-DistOffset);
	crystrans2.transform(ShieldRotation);
	G4RotationMatrix* rmc2 = new G4RotationMatrix;
	rmc2->set(0,0,0);
	rmc2->setPhi(180.*deg+rotoffset);
	rmc2->rotateX(CloverBGOCrystalAngle);
	CloverPhysiBGOCrystal[2] = new G4PVPlacement(rmc2,
			crystrans2,
			"CloverBGOCrystal",
			CloverLogicBGOCrystal[2],
			CloverPhysiBGOShield,
			false,
			copyNo*4+2,
			checkOverlaps);


	G4ThreeVector crystrans3(-CloverBGOCrystalH,0,CloverBGOCrystalDist-DistOffset);
	crystrans3.transform(ShieldRotation);
	G4RotationMatrix* rmc3 = new G4RotationMatrix;
	rmc3->set(0,0,0);
	rmc3->setPhi(90.*deg+rotoffset);
	rmc3->rotateX(CloverBGOCrystalAngle);
	CloverPhysiBGOCrystal[3] = new G4PVPlacement(rmc3,
			crystrans3,
			"CloverBGOCrystal",
			CloverLogicBGOCrystal[3],
			CloverPhysiBGOShield,
			false,
			copyNo*4+3,
			checkOverlaps);



	// Visualization attributes

	double hevimet_alpha = 0.5;
	double shield_alpha = 0.3;
	double bgo_alpha = 0.4;
	int usealpha = 0;
	if (!usealpha) {
		hevimet_alpha = shield_alpha = bgo_alpha = 1.0;
	}


	G4VisAttributes *grayVA = new G4VisAttributes(G4Colour(0.8,0.8,0.8,1.0));
	grayVA->SetVisibility(true);
	CloverAbsorber1_log->SetVisAttributes(grayVA);

	G4VisAttributes *orangeVA = new G4VisAttributes(G4Colour(0.8,0.4,0.));
	orangeVA->SetVisibility(true);
	CloverAbsorber2_log->SetVisAttributes(orangeVA);
	// R,   G,   B,   ALPHA
	G4VisAttributes* visAttBGOHevimet =new G4VisAttributes(G4Colour(0.9,0.5,0.5,hevimet_alpha));
	visAttBGOHevimet->SetVisibility(true);
	visAttBGOHevimet->SetForceWireframe(false);
	CloverLogicBGOHevimet->SetVisAttributes(visAttBGOHevimet);

	G4VisAttributes* visAttBGOShield =new G4VisAttributes(G4Colour(0.6,1.,0.6,shield_alpha));
	visAttBGOShield->SetVisibility(true);
	visAttBGOShield->SetForceWireframe(false);
	CloverLogicBGOShield->SetVisAttributes(visAttBGOShield);

	G4VisAttributes *visAttBGOCrystal = new G4VisAttributes(G4Colour(0.4,0.4,0.9,bgo_alpha));
	//visAttBGOCrystal->SetVisibility(false);
	visAttBGOCrystal->SetVisibility(true);
	visAttBGOCrystal->SetForceWireframe(false);

	for(int i = 0; i<4; i++)
	{
		CloverLogicBGOCrystal[i]->SetVisAttributes(visAttBGOCrystal);
	}

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Virtual Method of NPS::VDetector class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Jurogam::ReadConfiguration(NPL::InputParser parser)
{
	vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Jurogam");
	if(NPOptionManager::getInstance()->GetVerboseLevel())
		cout << "//// " << blocks.size() << " detectors found " << endl; 

	vector<string> cart = {"POS","Shape","BGO"};
	vector<string> sphe = {"R","Theta","Phi","Shape","BGO"};
	vector<string> combi = {"Loc","Theta","Phi","Shape","BGO"};

	for(unsigned int i = 0 ; i < blocks.size() ; i++){
		if(blocks[i]->HasTokenList(cart)){
			if(NPOptionManager::getInstance()->GetVerboseLevel())
				cout << endl << "////  Jurogam cart" << i+1 <<  endl;

			G4ThreeVector Pos = NPS::ConvertVector(blocks[i]->GetTVector3("POS","mm"));
			string Shape = blocks[i]->GetString("Shape");
			int BGO_int = blocks[i]->GetInt("BGO");
			G4bool BGO = false;
			if (BGO_int == 1) BGO = true;
			AddDetector(Pos,Shape,BGO);
		}
		else if(blocks[i]->HasTokenList(sphe)){
			if(NPOptionManager::getInstance()->GetVerboseLevel())
				cout << endl << "////  Jurogam sphe " << i+1 <<  endl;
			double R = blocks[i]->GetDouble("R","mm");
			double Theta = blocks[i]->GetDouble("Theta","deg");
			double Phi = blocks[i]->GetDouble("Phi","deg");
			string Shape = blocks[i]->GetString("Shape");
			int BGO_int = blocks[i]->GetInt("BGO");
			G4bool BGO = false;
			if (BGO_int == 1) BGO = true;
			AddDetector(R,Theta,Phi,Shape,BGO);
		}
		else if(blocks[i]->HasTokenList(combi)){
			if(NPOptionManager::getInstance()->GetVerboseLevel())
				cout << endl << "////  Jurogam combi" << i+1 <<  endl;
			G4ThreeVector Loc = NPS::ConvertVector(blocks[i]->GetTVector3("Loc","mm"));
			double Theta = blocks[i]->GetDouble("Theta","deg");
			double Phi = blocks[i]->GetDouble("Phi","deg");
			string Shape = blocks[i]->GetString("Shape");
			int BGO_int = blocks[i]->GetInt("BGO");
			G4bool BGO = false;
			if (BGO_int == 1) BGO = true;
			AddDetector(Loc,Theta,Phi,Shape,BGO);
		}
		else{
			cout << "ERROR: check your input file formatting foo" << endl;
			exit(1);
		}
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Jurogam::ConstructDetector(G4LogicalVolume* world)
{
	BuildPhaseI();
	BuildPhaseIBGO();
	BuildClover();
	BuildCloverBGO();
	for (unsigned short i = 0 ; i < m_R.size() ; i++)
	{
		G4double wX,wY,wZ;
		if (m_R[i] !=0)
		{
		wX = m_R[i] * sin(m_Theta[i] ) * cos(m_Phi[i] ) ;
		wY = m_R[i] * sin(m_Theta[i] ) * sin(m_Phi[i] ) ;
		wZ = m_R[i] * cos(m_Theta[i] ) ;
		}
		else
		{
		wX = m_X[i];
		wY = m_Y[i];
		wZ = m_Z[i];
		}
		G4ThreeVector Det_pos = G4ThreeVector(wX, wY, wZ) ;
		// So the face of the detector is at R instead of the middle
		//Det_pos+=Det_pos.unit()*Jurogam_NS::Thickness*0.5;
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

		if(m_Shape[i] == "PhaseI")
		{
			G4double radius = m_R[i]* mm;
			G4ThreeVector translation(radius*sin(m_Theta[i])*cos(m_Phi[i]),
					radius*sin(m_Theta[i])*sin(m_Phi[i]),
					radius*cos(m_Theta[i]));

			G4double sradius = radius - 23.80*mm-35.*mm;
			G4ThreeVector stranslation(sradius*sin(m_Theta[i])*cos(m_Phi[i]),
					sradius*sin(m_Theta[i])*sin(m_Phi[i]),
					sradius*cos(m_Theta[i]));
			SetRotation(*Rot);
			SetPosition(translation);

			if (m_R[i] < 1 )
			{
			SetPosition(Det_pos);
			translation.setX(m_X[i]*mm);
			translation.setY(m_Y[i]*mm);
			translation.setZ(m_Z[i]*mm);

			//G4RotationMatrix* tempRot= new G4RotationMatrix(m_Phi[i],m_Theta[i],0);
			G4RotationMatrix* tempRot= new G4RotationMatrix(0,0,0);
			//tempRot->setTheta(m_Theta[i]);
			tempRot->setPhi(m_Theta[i]);
			//tempRot->setPsi();
			SetRotation(*tempRot);
			}

			PhaseIPlacement(i,world,false);

			SetPosition(stranslation);
			if (m_BGO[i]) PhaseIBGOPlacement(i,world,false);


		}
		else if(m_Shape[i] == "Clover")
		{
			G4double radius = (m_R[i] +55./2)* mm;//To make radius the distance to endcap
			G4ThreeVector translation(radius*sin(m_Theta[i])*cos(m_Phi[i]),
					radius*sin(m_Theta[i])*sin(m_Phi[i]),
					radius*cos(m_Theta[i]));

			SetRotation(*Rot);
			SetPosition(translation);

			if (m_R[i] < 1 )
			{
			SetPosition(Det_pos);
			translation.setX(m_X[i]*mm);
			translation.setY(m_Y[i]*mm);
			translation.setZ(m_Z[i]*mm);

			//G4RotationMatrix* tempRot= new G4RotationMatrix(m_Phi[i],m_Theta[i],0);
			G4RotationMatrix* tempRot= new G4RotationMatrix(0,0,0);
			tempRot->setTheta(m_Theta[i]);
			tempRot->setPsi(m_Phi[i]);
			//tempRot->setPsi(0);
			SetRotation(*tempRot);
			}

			CloverPlacement(i,world,false);
			
			G4double sradius = (m_R[i] - 72.)* mm;
			G4ThreeVector stranslation(sradius*sin(m_Theta[i])*cos(m_Phi[i]),
					sradius*sin(m_Theta[i])*sin(m_Phi[i]),
					sradius*cos(m_Theta[i]));
			SetPosition(stranslation);
			if (m_BGO[i]) CloverBGOPlacement(i,world,false);

		}

	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Jurogam::InitializeRootOutput()
{
	RootOutput *pAnalysis = RootOutput::getInstance();
	TTree *pTree = pAnalysis->GetTree();
	if(!pTree->FindBranch("Jurogam")){
		pTree->Branch("Jurogam", "TJurogamData", &m_Event) ;
	}
	pTree->SetBranchAddress("Jurogam", &m_Event) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Jurogam::ReadSensitive(const G4Event* event)
{
	m_Event->Clear();

	///////////
	// Calorimeter scorer
	G4THitsMap<G4double*>* CaloHitMap;
	std::map<G4int, G4double**>::iterator Calo_itr;

	G4int CaloCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("JurogamScorer/Calorimeter");
	CaloHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(CaloCollectionID));

	// Loop on the Calo map
	for (Calo_itr = CaloHitMap->GetMap()->begin() ; Calo_itr != CaloHitMap->GetMap()->end() ; Calo_itr++){

		G4double* Info = *(Calo_itr->second);
		double Energy = RandGauss::shoot(Info[0],Jurogam_NS::ResoEnergy);
		if(Energy>Jurogam_NS::EnergyThreshold){
			double Time = RandGauss::shoot(Info[1],Jurogam_NS::ResoTime);
			int DetectorNbr = (int) Info[2];
			m_Event->SetEnergy(DetectorNbr,Energy);
			m_Event->SetTime(DetectorNbr,Time); 
			//for (int i =0; i< 3; i++) cout << Info[i]<< "\n";
			//cout << "\n";
		}
	}
	// clear map for next event
	CaloHitMap->clear();

	G4THitsMap<G4double*>* CaloBGOHitMap;
	std::map<G4int, G4double**>::iterator CaloBGO_itr;

	G4int CaloBGOCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("JurogamBGOScorer/CalorimeterBGO");
	CaloBGOHitMap = (G4THitsMap<G4double*>*)(event->GetHCofThisEvent()->GetHC(CaloBGOCollectionID));

	// Loop on the CaloBGO map
	for (CaloBGO_itr = CaloBGOHitMap->GetMap()->begin() ; CaloBGO_itr != CaloBGOHitMap->GetMap()->end() ; CaloBGO_itr++)
	{

		G4double* Info = *(CaloBGO_itr->second);
		double Energy = RandGauss::shoot(Info[0],Jurogam_NS::BGOResoEnergy);
		if(Energy>Jurogam_NS::BGOEnergyThreshold){
			double Time = RandGauss::shoot(Info[1],Jurogam_NS::BGOResoTime);
			int DetectorNbr = (int) Info[2];
			m_Event->Set_BGO_Energy(DetectorNbr,Energy);//TODO rewrite to differentiate from Jurogam entry compare to TREX method
			m_Event->Set_BGO_Time(DetectorNbr,Time);//TODO rewrite to differentiate from Jurogam entry JurogamDATA!
			//for (int i =0; i< 3; i++) cout << Info[i]<< "\n";
			//cout << "\n";
		}
	}
	// clear map for next event
	CaloBGOHitMap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////   
void Jurogam::InitializeScorers()
{ 
	// This check is necessary in case the geometry is reloaded
	bool already_exist = false; 
	m_JurogamScorer = CheckScorer("JurogamScorer",already_exist) ;
	m_JurogamBGOScorer = CheckScorer("JurogamBGOScorer",already_exist) ;

	if(already_exist) 
		return ;

	// Otherwise the scorer is initialised
	// PhaseI & Clover
	vector<int> level; level.push_back(2);
	G4VPrimitiveScorer* Calorimeter= new CALORIMETERSCORERS::PS_Calorimeter("Calorimeter",level, 0) ;
	// BGO Shields
	vector<int> levelBGO; levelBGO.push_back(1);
	G4VPrimitiveScorer* CalorimeterBGO= new CALORIMETERSCORERS::PS_Calorimeter("CalorimeterBGO",levelBGO, 0) ;

	//and register it to the multifunctionnal detector
	m_JurogamScorer->RegisterPrimitive(Calorimeter);
	m_JurogamBGOScorer->RegisterPrimitive(CalorimeterBGO);

	G4SDManager::GetSDMpointer()->AddNewDetector(m_JurogamScorer) ;
	G4SDManager::GetSDMpointer()->AddNewDetector(m_JurogamBGOScorer) ;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPS::VDetector* Jurogam::Construct()
{
	return  (NPS::VDetector*) new Jurogam();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern"C" {
	class proxy_nps_Jurogam{
		public:
			proxy_nps_Jurogam(){
				NPS::DetectorFactory::getInstance()->AddToken("Jurogam","Jurogam");
				NPS::DetectorFactory::getInstance()->AddDetector("Jurogam",Jurogam::Construct);
			}
	};

	proxy_nps_Jurogam p_nps_Jurogam;
}
