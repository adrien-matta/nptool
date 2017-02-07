//-------------------------------------------------------------------------
// BGO shield for a Phase-I detector
//
// Start: 07/04/2011 - Joonas Konki
//-------------------------------------------------------------------------
#include "PhaseIBGO.hh"
#include "Materials.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4LogicalVolume.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "globals.hh"
#include "G4AssemblyVolume.hh"
#include "G4Transform3D.hh"
#include "G4TwoVector.hh"

#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include <string>
#include <vector>
using namespace std;


PhaseIBGO::PhaseIBGO() {

  // Parameters
  BGOShieldLength = 251.75*mm;
  BGOShieldOuterRadius = 167.*mm/2.;
  BGOShieldInnerRadius = 0.*mm;
  subInnerConeL = 120.875*mm;
  BGOHevimetThickness = 35.0*mm; // real thickness 35.*mm, collar 1.5*mm
  //BGOHevimetDist = 138.0*mm; // CHECK
  BGOShieldDist = BGOHevimetThickness+1.5*mm;

  //BGOCrystalDist = -BGOShieldLength+0.7*mm;

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

  useAbsorber = true; // By default the absorbers are always in!

  //create the solids.....
  CreateSolids();

}

//Destructor
PhaseIBGO::~PhaseIBGO() {}

void PhaseIBGO::SetPosition(G4ThreeVector thisPos) { position = thisPos*mm; }

void PhaseIBGO::SetRotation(G4RotationMatrix thisRot) { rotation = thisRot; }

void PhaseIBGO::SetAbsorber(G4bool value) { useAbsorber = value; }


//---------------------------------------------------------------------
// Create the solids defining a Clover BGO shield
//---------------------------------------------------------------------
void  PhaseIBGO::CreateSolids() {

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

  solidBGOCrystal = SlicedCrystal7;


  // -------------------------------------------
  // BGO Shield Heavy Metal Collimator (HEVIMET)
  // -------------------------------------------

  G4VSolid* BGOHevimetTubs = new G4Tubs("BGOHevimetTubs",
                50.0*mm/2.,
                50.*mm,
                (BGOHevimetThickness/2.)*mm,
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
  G4ThreeVector offvec2(0,34.089*mm,-BGOHevimetThickness/2);

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

  offvec2 = G4ThreeVector(30.*mm,0,-BGOHevimetThickness/2);
  G4VSolid* HevimetTubSlice3
      = new G4SubtractionSolid ("HevimetTubSlice3",HevimetTubSlice2,IceCreamSlicer,
                                &slicerot1,offvec2);

  slicerot1.set(0,0,0);
  slicerot1.rotateZ(-90.*deg);
  slicerot1.rotateX(14.1*deg);

  offvec2 = G4ThreeVector(-30.*mm,0,-BGOHevimetThickness/2);
  G4VSolid* HevimetTubSlice4
      = new G4SubtractionSolid ("HevimetTubSlice4",HevimetTubSlice3,IceCreamSlicer,
                                &slicerot1,offvec2);


  slicerot1.set(0,0,0);
  slicerot1.rotateZ(123.9*deg);
  slicerot1.rotateX(13*deg); //was 12.952764513

  offvec2 = G4ThreeVector(15.58*mm,-29.26*mm,-BGOHevimetThickness/2);
  G4VSolid* HevimetTubSlice5
      = new G4SubtractionSolid ("HevimetTubSlice5",HevimetTubSlice4,IceCreamSlicer,
                                &slicerot1,offvec2);

  slicerot1.set(0,0,0);
  slicerot1.rotateZ(-123.9*deg);
  slicerot1.rotateX(13*deg);

  offvec2 = G4ThreeVector(-15.58*mm,-29.26*mm,-BGOHevimetThickness/2);
  G4VSolid* HevimetTubSlice6
      = new G4SubtractionSolid ("HevimetTubSlice6",HevimetTubSlice5,IceCreamSlicer,
                                &slicerot1,offvec2);

  slicerot1.set(0,0,0);
  slicerot1.rotateZ(180*deg);
  slicerot1.rotateX(14.68*deg);

  offvec2 = G4ThreeVector(0,-29.26*mm,-BGOHevimetThickness/2);
  G4VSolid* HevimetTubSlice7
      = new G4SubtractionSolid("HevimetTubSlice7",HevimetTubSlice6,IceCreamSlicer,
                               &slicerot1,offvec2);

  solidBGOHevimet = HevimetTubSlice7;

  // ---------------------------------------
  // BGO Shield Aluminium part
  // ---------------------------------------

  G4VSolid* BGOShieldTubs = new G4Tubs("BGOShieldTubs",
                BGOShieldInnerRadius,
                BGOShieldOuterRadius,
                (BGOShieldLength/2.)*mm,
                0.*deg,
                360.*deg);


  G4VSolid* subInnerCone = new G4Cons("subInnerCone",
                0.*mm, //	inside radius at -pDz
                64.*mm/2., // outside radius at -pDz
                0.*mm, // inside radius at +pDz
                88.1*mm/2., // outside radius at +pDz
                subInnerConeL/2.,    // half length in z
                0.*deg,  // starting angle of the segment in radians
                360.*deg); // the angle of the segment in radians

  G4VSolid* subInnerTubs = new G4Tubs("subInnerTubs",
                0.*mm,
                88.1*mm/2., // CHECK
                (BGOShieldLength-subInnerConeL)/2.+10.*mm, // CHECK
                0.*deg,
                360.*deg);

  G4ThreeVector vec(0,0,(BGOShieldLength)/2.+10.*mm); // CHECK
  G4UnionSolid* subInnerPart = new G4UnionSolid("subInnerPart",subInnerCone,subInnerTubs,0,vec);

  G4ThreeVector vec2(0,0,-(BGOShieldLength-subInnerConeL+10.*um)/2.); // 10um offset for proper cut that shows in visualization?

  // Subtract the hole from the shield tubs = IceCreamCone1
  G4VSolid* IceCreamCone1 = new G4SubtractionSolid ("IceCreamCone1",BGOShieldTubs,subInnerPart,0,vec2);

  //Rotation of slicer cube
  //G4RotationMatrix slicerot1;
  slicerot1.set(0,0,0);
  slicerot1.rotateZ(-30.9*deg);
  slicerot1.rotateX(13.*deg); //14.98 in the .igs ?? cannot be right as it doesn't fit

  // Location of slicer cube bottom center
  G4ThreeVector vec3(0,44.153*mm,-BGOShieldLength/2);

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

  vec3 = G4ThreeVector(37.03,20.89*mm,-BGOShieldLength/2);
  G4VSolid* IceCreamCone4
      = new G4SubtractionSolid ("IceCreamCone4",IceCreamCone3,IceCreamSlicer,
                                &slicerot1,vec3);

  slicerot1.set(0,0,0);
  slicerot1.rotateZ(-90.*deg);
  slicerot1.rotateX(18.*deg);

  vec3 = G4ThreeVector(-37.03,20.89*mm,-BGOShieldLength/2);
  G4VSolid* IceCreamCone5
      = new G4SubtractionSolid ("IceCreamCone5",IceCreamCone4,IceCreamSlicer,
                                &slicerot1,vec3);


  slicerot1.set(0,0,0);
  slicerot1.rotateZ(123.9*deg);
  slicerot1.rotateX(13.*deg); // was 14.956711

  vec3 = G4ThreeVector(37.03,-11.29*mm,-BGOShieldLength/2);
  G4VSolid* IceCreamCone6
      = new G4SubtractionSolid ("IceCreamCone6",IceCreamCone5,IceCreamSlicer,
                                &slicerot1,vec3);

  slicerot1.set(0,0,0);
  slicerot1.rotateZ(-123.9*deg);
  slicerot1.rotateX(13.*deg); // was 14.956711

  vec3 = G4ThreeVector(-37.03,-11.29*mm,-BGOShieldLength/2);
  G4VSolid* IceCreamCone7
      = new G4SubtractionSolid ("IceCreamCone7",IceCreamCone6,IceCreamSlicer,
                                &slicerot1,vec3);

  slicerot1.set(0,0,0);
  slicerot1.rotateZ(180*deg);
  slicerot1.rotateX(16.5*deg);

  vec3 = G4ThreeVector(0,-37.86*mm,-BGOShieldLength/2);
  G4VSolid* IceCreamCone8
      = new G4SubtractionSolid("IceCreamCone8",IceCreamCone7,IceCreamSlicer,
                               &slicerot1,vec3);

  slicerot1.set(0,0,0);
  slicerot1.rotateZ(148.3*deg);
  slicerot1.rotateX(8.72*deg);// was 8.718867709, but crystal shows up then

  vec3 = G4ThreeVector(23.529*mm,-53.159*mm,-BGOShieldLength/2+48.672*mm);
  G4VSolid* IceCreamCone9
      = new G4SubtractionSolid("IceCreamCone9",IceCreamCone8,IceCreamSlicer,
                               &slicerot1,vec3);

  slicerot1.set(0,0,0);
  slicerot1.rotateZ(-148.3*deg);
  slicerot1.rotateX(8.5*deg); // was 8.72.... 8.5 (.igs)

  vec3 = G4ThreeVector(-23.529*mm,-53.159*mm,-BGOShieldLength/2+48.672*mm);
  G4VSolid* IceCreamCone
      = new G4SubtractionSolid("IceCreamCone",IceCreamCone9,IceCreamSlicer,
                               &slicerot1,vec3);

  solidBGOShield = IceCreamCone;

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


//------------------------------------------------------------------
void PhaseIBGO::Placement(G4int copyNo, G4VPhysicalVolume* physiMother, G4bool checkOverlaps) {

  logicBGOShield = new G4LogicalVolume(solidBGOShield, ShieldMaterial,"PhaseIBGOShield",0,0,0);
  logicBGOCrystal = new G4LogicalVolume(solidBGOCrystal, BGOMaterial,
                                        "PhaseIBGOCrystal",0,0,0);
  logicBGOHevimet = new G4LogicalVolume(solidBGOHevimet, HevimetMaterial,
                                         "BGOHevimetColl", 0, 0, 0);

  Phase1Absorber1_log = new G4LogicalVolume(Phase1Absorber1, Absorber1Material,"Phase1Absorber1_log",0,0,0);
  Phase1Absorber2_log = new G4LogicalVolume(Phase1Absorber2, Absorber2Material,"Phase1Absorber2_log",0,0,0);

  G4double hmetradius = position.mag()+BGOHevimetThickness/2.;
  G4ThreeVector hevimetpos( (hmetradius)*sin(position.getTheta())*cos(position.getPhi()),
				                    (hmetradius)*sin(position.getTheta())*sin(position.getPhi()),
				                    (hmetradius)*cos(position.getTheta()) );


  physiBGOHevimet = new G4PVPlacement(&rotation,
           hevimetpos,
					 "PhaseIBGOHevimetColl",
					 logicBGOHevimet  ,
					 physiMother,
					 true,
					 copyNo,
					 checkOverlaps);

  if (useAbsorber) {
	  G4double abs1radius = position.mag() - abs1_thickness/2. - abs2_thickness - 0.1*mm;
    G4ThreeVector abs1pos( (abs1radius)*sin(position.getTheta())*cos(position.getPhi()),
				                   (abs1radius)*sin(position.getTheta())*sin(position.getPhi()),
				                   (abs1radius)*cos(position.getTheta()) );

    Phase1Absorber1_phys = new G4PVPlacement(&rotation,
               abs1pos,
					     "Phase1Absorber1_phys", //its name
					     Phase1Absorber1_log,    //its logical volume
					     physiMother,            //its mother
					     false,                  //no boolean operat
					     copyNo,                 //copy number
					     checkOverlaps);         //overlap check

    G4double abs2radius = position.mag() - abs2_thickness/2. - 0.05*mm;
    G4ThreeVector abs2pos( (abs2radius)*sin(position.getTheta())*cos(position.getPhi()),
				                   (abs2radius)*sin(position.getTheta())*sin(position.getPhi()),
				                   (abs2radius)*cos(position.getTheta()) );

    Phase1Absorber2_phys = new G4PVPlacement(&rotation,
               abs2pos,
					     "Phase1Absorber2_phys", //its name
					     Phase1Absorber2_log,    //its logical volume
					     physiMother,            //its mother
					     false,                  //no boolean operat
					     copyNo,                 //copy number
					     checkOverlaps);         //overlap check
	} //if (useAbsorber)


	G4double shieldradius = position.mag()+BGOShieldLength/2.+BGOShieldDist;
	G4ThreeVector shieldpos((shieldradius)*sin(position.getTheta())*cos(position.getPhi()),
				                  (shieldradius)*sin(position.getTheta())*sin(position.getPhi()),
				                  (shieldradius)*cos(position.getTheta()));

  physiBGOShield = new G4PVPlacement(&rotation,
           //G4ThreeVector(position.x(),position.y(),position.z()),
           shieldpos,
           //G4ThreeVector(0,0,0),
					 "PhaseIBGOShield",
					 logicBGOShield,
					 physiMother,
					 true,
					 copyNo,
					 checkOverlaps);


  G4RotationMatrix* crysrot = new G4RotationMatrix;
	crysrot->set(0,0,0);
	crysrot->rotateZ(-18.*deg);
	G4ThreeVector crystrans(0,0,-BGOShieldLength/2.+0.7*mm);

	physiBGOCrystal = new G4PVPlacement(crysrot,
           crystrans,
					 "PhaseIBGOCrystal",
					 logicBGOCrystal,
					 physiBGOShield,
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
  logicBGOHevimet->SetVisAttributes(visAttBGOHevimet);

  G4VisAttributes* visAttBGOShield =new G4VisAttributes(G4Colour(0.9,0.8,1.0,shield_alpha));
  visAttBGOShield->SetVisibility(true);
  visAttBGOShield->SetForceWireframe(false);
  logicBGOShield->SetVisAttributes(visAttBGOShield);

  G4VisAttributes *visAttBGOCrystal = new G4VisAttributes(G4Colour(0.4,0.4,0.9,bgo_alpha));
	//visAttBGOCrystal->SetVisibility(false);
	visAttBGOCrystal->SetVisibility(true);
  visAttBGOCrystal->SetForceWireframe(false);
  logicBGOCrystal->SetVisAttributes(visAttBGOCrystal);

}
