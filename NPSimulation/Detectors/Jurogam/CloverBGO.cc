//-------------------------------------------------------------------------
// BGO shield for a Clover detector
//
// 29/03/2011 - Joonas Konki
//-------------------------------------------------------------------------
#include "CloverBGO.hh"
#include "Materials.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4LogicalVolume.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
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


CloverBGO::CloverBGO() {

  // Parameters
  BGOCrystalAngle = 7.1*deg;

  BGOCrystalBoxD  = 18.86*mm; // approx
  BGOCrystalBoxD2 = 3.0*mm;   // = ? (real size unknown)
  BGOCrystalTrapL = 120.5*mm; //total length ~ 241 mm
  BGOCrystalW     = 141.31*mm; // approx
  BGOCrystalW2     = 82.0*mm; // approx
  BGOCrystalW3     = (BGOCrystalW+BGOCrystalW2)/2.;
  BGOHevimetThickness = 35.*mm;
  BGOHevimetDist = 138.0*mm;
  BGOShieldDist = BGOHevimetDist+BGOHevimetThickness;
  BGOCrystalDist = 6.7*mm+BGOCrystalTrapL/2.;
  BGOCrystalH = (sin(7.1*deg)*BGOCrystalTrapL/2.+2.+39.5)*mm;

  //---------------------------------
  //make the required materials and assign them
  fMat = Materials::GetInstance();

  //assign default materials.....
  vacuumMaterial     =  fMat->GetMaterial("Vacuum");
  BGOMaterial        =  fMat->GetMaterial("BGO");
  HevimetMaterial    =  fMat->GetMaterial("Hevimet");
  ShieldMaterial     =  fMat->GetMaterial("Aluminium");
  Absorber1Material  =  fMat->GetMaterial("Tin");
  Absorber2Material  =  fMat->GetMaterial("Copper");

  useAbsorber = true; // By default the absorbers are always in!


  //create the solids.....
  CreateSolids();

}

//Destructor
CloverBGO::~CloverBGO() {}

void CloverBGO::SetPosition(G4ThreeVector thisPos) { position = thisPos*mm; }

void CloverBGO::SetRotation(G4RotationMatrix thisRot) { rotation = thisRot; }

void CloverBGO::SetAbsorber(G4bool value) { useAbsorber = value; }

// Converts a int to string
string intToString(int number) {
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

//---------------------------------------------------------------------
// Create the solids defining a Clover BGO shield
//---------------------------------------------------------------------
void  CloverBGO::CreateSolids() {

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
             BGOCrystalW3/2.,     //G4double  dx1,
             BGOCrystalW/2.,      //G4double  dx2,
             BGOCrystalBoxD/2.,   //G4double  dy1,
             BGOCrystalBoxD/2.,   //G4double  dy2,
             BGOCrystalTrapL/2.); //G4double  dz

  G4VSolid* BGOCrystalTrd2 = new G4Trd("BGOTrd",
             BGOCrystalW2/2., //G4double  dx1,
             BGOCrystalW3/2., //G4double  dx2,
             BGOCrystalBoxD2, //G4double  dy1,
             BGOCrystalBoxD, //G4double  dy2,
             BGOCrystalTrapL/2.); // G4double  dz

  G4VSolid* BGOCrystalTrdSubBox = new G4Box("BGOTrd2SubBox",
                                            BGOCrystalW,
                                            BGOCrystalBoxD,
                                            BGOCrystalTrapL);

  G4ThreeVector trans1( 0.*mm, -BGOCrystalBoxD, 0.*mm);
  G4SubtractionSolid* solidBGOTrap
      = new G4SubtractionSolid("solidBGOTrap",BGOCrystalTrd2,
                               BGOCrystalTrdSubBox,0,trans1);

  //combine BGO crystal parts
  G4ThreeVector  trans2(0.*mm, BGOCrystalBoxD/2.,
                        BGOCrystalTrapL/2.+BGOCrystalTrapL/2.);
  solidBGOCrystal
     = new G4UnionSolid("solidBGOCrystal",solidBGOTrap,BGOCrystalTrd1,0,trans2);

  // Slice two of the crystals a bit
  slicerot1.set(0,0,0);
  slicerot1.rotateZ(2.35*deg);
  slicerot1.rotateX(7.6*deg);
  offvec = G4ThreeVector(BGOCrystalW2/2.,BGOCrystalBoxD2/2.-0.5*mm,-BGOCrystalTrapL/2.);

  solidBGOCrystal2 = new G4SubtractionSolid("solidBGOCrystal2",solidBGOCrystal ,
                               IceCreamSlicer,&slicerot1,offvec);

  slicerot1.set(0,0,0);
  slicerot1.rotateZ(-2.35*deg);
  slicerot1.rotateX(7.6*deg);
  offvec = G4ThreeVector(-BGOCrystalW2/2.,BGOCrystalBoxD2/2.-0.5*mm,-BGOCrystalTrapL/2.);

  solidBGOCrystal3 = new G4SubtractionSolid("solidBGOCrystal3",solidBGOCrystal,
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

  solidBGOHevimet = SlicedHevimet1;

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

  solidBGOHevimet = SlicedHevimet4;


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

  solidBGOShield = SlicedShield4;

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


//------------------------------------------------------------------
void CloverBGO::Placement(G4int copyNo, G4VPhysicalVolume* physiMother, G4bool checkOverlaps) {


  logicBGOCrystal[0] = new G4LogicalVolume(solidBGOCrystal2, BGOMaterial,
                                           "CloverBGOCrystal",0,0,0);
  logicBGOCrystal[1] = new G4LogicalVolume(solidBGOCrystal, BGOMaterial,
                                           "CloverBGOCrystal",0,0,0);
  logicBGOCrystal[2] = new G4LogicalVolume(solidBGOCrystal3, BGOMaterial,
                                           "CloverBGOCrystal",0,0,0);
  logicBGOCrystal[3] = new G4LogicalVolume(solidBGOCrystal, BGOMaterial,
                                           "CloverBGOCrystal",0,0,0);

  logicBGOShield = new G4LogicalVolume(solidBGOShield, ShieldMaterial,
                                       "BGOShield", 0, 0, 0);


  logicBGOHevimet = new G4LogicalVolume(solidBGOHevimet, HevimetMaterial,
                                        "BGOHevimetColl", 0, 0, 0);

  CloverAbsorber1_log = new G4LogicalVolume(CloverAbsorber1,Absorber1Material,"CloverAbsorber1_log",0,0,0);
  CloverAbsorber2_log = new G4LogicalVolume(CloverAbsorber2,Absorber2Material,"CloverAbsorber2_log",0,0,0);

  G4RotationMatrix rms;
  rms.set(0,0,0); rms.rotateZ(45.0*deg);

  //G4RotationMatrix hevirot;
  //hevirot.set(0,0,0); hevirot.rotateZ(90.0*deg);

  //G4double hradius = position.mag()+BGOHevimetThickness/2.;
  G4ThreeVector hevimetpos( (position.mag())*sin(position.getTheta())*cos(position.getPhi()),
				                    (position.mag())*sin(position.getTheta())*sin(position.getPhi()),
				                    (position.mag())*cos(position.getTheta()) );

	rotation.transform(rms);

  physiBGOHevimet = new G4PVPlacement(&rotation,
           //G4ThreeVector(0,0,-35.*mm),
           hevimetpos,
					 "CloverBGOHevimetColl",
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

    CloverAbsorber1_phys = new G4PVPlacement(&rotation,
               abs1pos,
					     "CloverAbsorber1_phys", //its name
					     CloverAbsorber1_log,    //its logical volume
					     physiMother,            //its mother
					     false,                  //no boolean operat
					     copyNo,                 //copy number
					     checkOverlaps);         //overlap check



    G4double abs2radius = position.mag() - abs2_thickness/2. - 0.05*mm;
    G4ThreeVector abs2pos( (abs2radius)*sin(position.getTheta())*cos(position.getPhi()),
				                   (abs2radius)*sin(position.getTheta())*sin(position.getPhi()),
				                   (abs2radius)*cos(position.getTheta()) );

    CloverAbsorber2_phys = new G4PVPlacement(&rotation,
               abs2pos,
					     "CloverAbsorber2_phys", //its name
					     CloverAbsorber2_log,    //its logical volume
					     physiMother,            //its mother
					     false,                  //no boolean operat
					     copyNo,                 //copy number
					     checkOverlaps);         //overlap check
  }


	G4double sradius = position.mag()+BGOHevimetThickness;
	G4ThreeVector shieldpos((sradius)*sin(position.getTheta())*cos(position.getPhi()),
				                  (sradius)*sin(position.getTheta())*sin(position.getPhi()),
				                  (sradius)*cos(position.getTheta()));

  physiBGOShield = new G4PVPlacement(&rotation,
           //G4ThreeVector(0,0,0),
           //G4ThreeVector(position.x(),position.y(),position.z()),
           shieldpos,
					 "CloverBGOShield",
					 logicBGOShield,
					 physiMother,
					 true,
					 copyNo,
					 checkOverlaps);


  G4RotationMatrix ShieldRotation; // shield coords 45*deg rotated wrt. world
  ShieldRotation.set(0,0,0);
  ShieldRotation.rotateZ(45.*deg);
  G4double rotoffset = 45.*deg;
  G4double DistOffset = 0.*mm;

  G4ThreeVector crystrans0(0,BGOCrystalH,BGOCrystalDist-DistOffset);
  crystrans0.transform(ShieldRotation);
  G4RotationMatrix* rmc0 = new G4RotationMatrix;
  rmc0->set(0,0,0);
  rmc0->setPhi(0.*deg+rotoffset);
  rmc0->rotateX(BGOCrystalAngle);
  physiBGOCrystal[0] = new G4PVPlacement(rmc0,
          crystrans0,
          "CloverBGOCrystal",
          logicBGOCrystal[0],
          physiBGOShield,
          false,
          copyNo*4,
          checkOverlaps);

  G4ThreeVector crystrans1(BGOCrystalH,0,BGOCrystalDist-DistOffset);
  crystrans1.transform(ShieldRotation);
  G4RotationMatrix* rmc1 = new G4RotationMatrix;
  rmc1->set(0,0,0);
  rmc1->setPhi(270.*deg+rotoffset);
  rmc1->rotateX(BGOCrystalAngle);
  physiBGOCrystal[1] = new G4PVPlacement(rmc1,
          crystrans1,
          "CloverBGOCrystal",
          logicBGOCrystal[1],
          physiBGOShield,
          false,
          copyNo*4+1,
          checkOverlaps);


  G4ThreeVector crystrans2(0,-BGOCrystalH,BGOCrystalDist-DistOffset);
  crystrans2.transform(ShieldRotation);
  G4RotationMatrix* rmc2 = new G4RotationMatrix;
  rmc2->set(0,0,0);
  rmc2->setPhi(180.*deg+rotoffset);
  rmc2->rotateX(BGOCrystalAngle);
  physiBGOCrystal[2] = new G4PVPlacement(rmc2,
          crystrans2,
          "CloverBGOCrystal",
          logicBGOCrystal[2],
          physiBGOShield,
          false,
          copyNo*4+2,
          checkOverlaps);


  G4ThreeVector crystrans3(-BGOCrystalH,0,BGOCrystalDist-DistOffset);
  crystrans3.transform(ShieldRotation);
  G4RotationMatrix* rmc3 = new G4RotationMatrix;
  rmc3->set(0,0,0);
  rmc3->setPhi(90.*deg+rotoffset);
  rmc3->rotateX(BGOCrystalAngle);
  physiBGOCrystal[3] = new G4PVPlacement(rmc3,
          crystrans3,
          "CloverBGOCrystal",
          logicBGOCrystal[3],
          physiBGOShield,
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
  logicBGOHevimet->SetVisAttributes(visAttBGOHevimet);

  G4VisAttributes* visAttBGOShield =new G4VisAttributes(G4Colour(0.6,1.,0.6,shield_alpha));
  visAttBGOShield->SetVisibility(true);
  visAttBGOShield->SetForceWireframe(false);
  logicBGOShield->SetVisAttributes(visAttBGOShield);

  G4VisAttributes *visAttBGOCrystal = new G4VisAttributes(G4Colour(0.4,0.4,0.9,bgo_alpha));
  //visAttBGOCrystal->SetVisibility(false);
  visAttBGOCrystal->SetVisibility(true);
  visAttBGOCrystal->SetForceWireframe(false);

  for(int i = 0; i<4; i++) {
      logicBGOCrystal[i]->SetVisAttributes(visAttBGOCrystal);
  }



}
