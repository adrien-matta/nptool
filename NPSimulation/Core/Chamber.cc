/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche  contact address: marc.labiche@stfc.ac.uk   *
 *                                                                           *
 * Creation Date  : January 2010                                             *
 * Last update    : 19/07/2016                                               *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe standard Chamber. Derived fromNPS::VDetector         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment: last update added GREAT chamber                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// C++ header
#include <fstream>
#include <limits>

// G4 geometry header
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Box.hh"
#include "G4Trap.hh"
#include "G4Polycone.hh"
#include "G4SubtractionSolid.hh"

//G4 various headers
#include "G4Material.hh"
#include "G4RotationMatrix.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4EmCalculator.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"
using namespace CLHEP ;
// NPTool header
#include"Chamber.hh"
#include"MaterialManager.hh"
#include "NPOptionManager.h"
using namespace std;



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Specific Method of this class
Chamber::Chamber(): nStripsX(60),nStripsY(40)
{
  //m_ChamberType     = true;
  m_ChamberType     = 0   ;
  m_ChamberRmin     = 0   ;
  m_ChamberRmax     = 0   ;
  m_ChamberPhiMin   = 0   ;
  m_ChamberPhiMax   = 0   ;
  m_ChamberThetaMin = 0   ;
  m_ChamberThetaMax = 0   ;

  // For GREAT:
  DefineMaterials();
  //-----------------------------
  //defaults for Planar
  fPlanarGuard =   5.0*mm;
  fPlanarGe_W  = 120.0*mm;
  fPlanarGe_H  =  60.0*mm;
  fPlanarGe_T  =  15.0*mm;

  fEndCapThickness    =  2.7*mm; //Notes state a 2.2 mm thick rib supporting a 0.5 mm Be window
  fAlWindowThickness  =  1.1*mm;
  fBeWindowThickness  =  0.5*mm;

  fPlanarGeGapFront   = 14.0*mm; //Front face Al -> Front face Ge
  fPlanarGeGapBack    =  9.0*mm; //Back face Al  -> Back face Ge
  fPlanarGeGapFront   = 16.0*mm; //Front face Al -> Front face Ge
  fPlanarGeGapBack    =  7.0*mm; //Back face Al  -> Back face Ge

  fPlanarTotalL       = 2.*fEndCapThickness + fPlanarGeGapFront + fPlanarGeGapBack + fPlanarGe_T;

  //---------------------------------------
  // I measured 39.5 mm for fPlanarTotalL
  //  => put end cap thickness 2.75 mm
  //  => add 1 mm to one of the gaps (most likely the retards put fPlanarGeGapFront = 15 mm !)
  fPlanarTotalL       = fEndCapThickness + fPlanarGeGapFront + fPlanarGeGapBack + fPlanarGe_T + fAlWindowThickness;

  fdeltaZ_Support     =  0.0*mm;  //looks like there is a little gap between the cooling block and the planar cut-out
  fGap_PCB_2_Planar   =  6.0*mm;  //the gap between the downstrean part of the PCB and the upstream face of planar
  fGap_PCB_2_Planar  +=  fdeltaZ_Support;
  fLengthOfPlanarCut  = 55.0*mm; //the Z dimension of the cut out used to remove the planar
  //--------------------------------------------------------------
  // Cooling block dimensions
  fCoolingBlockL      =  79.0*mm;
  fCoolingBlockH      = 152.0*mm;
  fCoolingBlockW      = 176.7*mm;
  fCoolingBlockT      =   6.0*mm; //excellent idea for low energy transparency !

  // Size of cut-out for the tunnel (and DSSD)
  fCoolingBlockCutOutH    = 74.4*mm;
  fCoolingBlockCutOutW    = fCoolingBlockW - 2.*fCoolingBlockT;
  fCoolingBlockDSSDCentre = fCoolingBlockH - fCoolingBlockT - fCoolingBlockCutOutH/2.;
  fDSSD_BoardL            = 2.0*mm;  //thickness of Si implantation detector support board

  fCoolingBlockZ = fLengthOfPlanarCut + fDSSD_BoardL + fdeltaZ_Support; //distance between chamber inner wall and downstream side of detector support

  G4cout << "fCoolingBlockZ " << fCoolingBlockZ << G4endl;


  //-----------------------------
  //default DSSD
  fDSSD_H      = 40.0*mm; //active
  fDSSD_W      = 60.0*mm; //active
  fDSSD_T      =  0.3*mm;
  fDSSD_Guard  =  1.0*mm;


  //-----------------------------------------------------
  // Put these defaults here so I can easily compare them
  //-----------------------------------------------------
  //some default PhaseII detector parameters
  fTotalGeL_PhaseII     = 70.0 * mm;  //was 70
  fCrystalR_PhaseII     = 24.0 * mm;  //was 25
  fEndCap2Ge_PhaseII    = 20.0 * mm;  //Distance from Al outer face to Ge
  //added to fudge PTG's efficiency measurements for close-geometry STUK config.
  fFudge = 8.0*mm;
  fEndCap2Ge_PhaseII += fFudge;

  //modify it to make it 5 mm
  //fEndCap2Ge_PhaseII -= 23*mm;

  fGapBetweenLeaves_PhaseII = 0.8*mm;

  fHoleR_PhaseII        =  5.0 * mm; //was 5.0
  //fPassiveThick_PhaseII =  0.5 * mm;  
  fContactThick_PhaseII =  0.5 * mm;  

  //make the PhaseII solids
  CreateCloverIISolids();
  //-----------------------------------------------------


  //-----------------------------------------------------
  //some default GREAT Clover parameters
  fTotalGeL_GREAT     = 105.0 * mm;
  fTaperGeL_GREAT     =  30.0 * mm;
  fCrystalR_GREAT     =  35.0 * mm;

  fFrontFaceSquare_GREAT = 54.00 * mm;
  fBackFaceSquare_GREAT  = 61.50 * mm;
  fEndCap2Ge_GREAT       = 10.00 * mm;  //Distance from Al outer face to Ge

  fGapBetweenLeaves_GREAT =  0.8*mm;

  fHoleR_GREAT        =  5.0 * mm; //was 5.0
  //fPassiveThick_GREAT =  0.5 * mm;  
  fContactThick_GREAT =  0.5 * mm; 
  //gap between GREAT clover and chamber in "TOP" position
  fGeTopGap = 1.5 * mm;

  //make the GREAT Clover solids
  CreateGREATCloverSolids();

  //-----------------------------------------------------
  //Default position of Ges relative to DSSD
  geTopPosition  = 0. * mm;    //-1.5 mm puts the downstream endcap of the GREAT detector level with chamber
  geSidePosition = 0. * mm;
  //-----------------------------------------------------



  //Centre of Si Implantation detector [UPSTREAM]
  fDSSD_BoardZ = -fDSSD_BoardL/2.;
  fDSSD_BoardZ = 0.0 * mm;
  G4cout << "Default SiSuport Z " << fDSSD_BoardZ - fDSSD_BoardL/2. << G4endl;



}

G4Material* Chamber::GetMaterialFromLibrary(G4String MaterialName, G4double Temperature, G4double Pressure)
{
  // avoid compilation warnings
  Temperature *= 1;
  Pressure *= 1;

  if (MaterialName == "Alu") {
    return MaterialManager::getInstance()->GetMaterialFromLibrary("Al");
  }
  if (MaterialName == "Cu") {
    return MaterialManager::getInstance()->GetMaterialFromLibrary("Cu");
  }
  if (MaterialName == "C" || MaterialName == "12C") { // keeping legacy name
    return MaterialManager::getInstance()->GetMaterialFromLibrary("C");
  }

  else {
    G4cout << "No Matching Material in the Chamber Library Default is Vacuum" << G4endl;
    return  MaterialManager::getInstance()->GetMaterialFromLibrary("Vacuum");
  }
}


//------------------------------------------------------------------
//Materials Definitions
void Chamber::DefineMaterials()
{
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  G4String name, symbol;
  G4int    ncomponents, natoms;
  G4double fractionmass;

  //---------------------------------
  //some elements
  a = 1.0078*g/mole;
  G4Element* elH  = new G4Element(name="Hydrogen",   symbol="H",  z=1., a);

  a = 12.00*g/mole;
  G4Element* elC  = new G4Element(name="Carbon",     symbol="C",  z=6., a);

  a = 14.01*g/mole;
  G4Element* elN  = new G4Element(name="Nitrogen",   symbol="N",  z=7., a);

  a = 16.00*g/mole;
  G4Element* elO  = new G4Element(name="Oxygen",     symbol="O",  z=8., a);

  a = 24.305*g/mole;
  G4Element* elMg = new G4Element(name="Magnesium",  symbol="Mg", z=12., a);

  a = 26.98*g/mole;
  G4Element* elAl = new G4Element(name="Aluminium",  symbol="Al", z=13., a);

  a = 35.45*g/mole;
  G4Element* elCl = new G4Element(name="Chlorine",   symbol="Cl", z=17., a);

  a = 51.9961*g/mole;
  G4Element* elCr = new G4Element(name="Chromium",   symbol="Cr", z=24., a);

  a = 54.938*g/mole;
  G4Element* elMn = new G4Element(name="Manganse",   symbol="Mn", z=25., a);

  a = 55.845*g/mole;
  G4Element* elFe = new G4Element(name="Iron",       symbol="Fe", z=26., a);

  a = 58.6934*g/mole;
  G4Element* elNi = new G4Element(name="Nickel",     symbol="Ni", z=28., a);

  a = 63.54*g/mole;
  G4Element* elCu = new G4Element(name="Copper",     symbol="Cu", z=29., a);

  a = 65.39*g/mole;
  G4Element* elZn = new G4Element(name="Zinc",       symbol="Zn", z=30., a);

  a = 72.61*g/mole;
  G4Element* elGe = new G4Element(name="Germanium",  symbol="Ge", z=32., a);

  a =208.98*g/mole;
  G4Element* elBi = new G4Element(name="Bismuth",    symbol="Bi", z=83., a);

  //elements for physics....
  a =251.00*g/mole;
  G4Element* elEs = new G4Element(name="Einsteinium",symbol="Es", z=99., a);
  einsteinium = elEs;

  a =251.00*g/mole;
  G4Element* elFm = new G4Element(name="Fermium",    symbol="Fm", z=100., a);
  fermium = elFm;

  //---------------------------------
  //Define required materials
  a=6.941*g/mole;  density=0.534*g/cm3;
  G4Material* Li=new G4Material(name="Lithium", z=3., a, density);

  a=9.0122*g/mole;  density=1.85*g/cm3;
  G4Material* Be=new G4Material(name="Berilium", z=4., a, density);

  a=28.0855*g/mole;  density=2.33*g/cm3;
  G4Material* Si=new G4Material(name="Silicon", z=14., a, density);

  a=72.61*g/mole;  density=5.32*g/cm3;
  G4Material* Ge=new G4Material(name="Germanium", z=32., a, density);

  a=26.98*g/mole;  density=2.7*g/cm3;

  new G4Material(name="Aluminium", z=13., a, density);

  a=63.54*g/mole;  density=8.96*g/cm3;
  new G4Material(name="Copper", z=29., a, density);

  a=183.84*g/mole;  density=19.3*g/cm3;
  new G4Material(name="Tungsten", z=74., a, density);

  //---------------------------------
  // AIR
  density = 1.290*mg/cm3;
  G4Material* Air = new G4Material(name="Air", density, ncomponents=2);
  Air->AddElement(elN, fractionmass=70*perCent);
  Air->AddElement(elO, fractionmass=30*perCent);

  density  = 1.e-5*g/cm3;      //taken from ExN03
  G4double pressure    = 2.e-7*bar;
  G4double temperature = STP_Temperature;
  G4Material* Vacuum=new G4Material(name="Vacuum", density, ncomponents=1,
      kStateGas, temperature, pressure);
  Vacuum->AddMaterial(Air, fractionmass=1.);

  //---------------------------------
  // Pentane
  density     = 0.620*g/cm3;  //liquid !
  density     = 5.9e-6*g/cm3; //gas
  pressure    = 1.5/760.0*atmosphere;
  temperature = 310*kelvin;
  G4Material* Pentane=new G4Material(name="Pentane", density, ncomponents=2,
      kStateGas, temperature, pressure);
  Pentane->AddElement(elH, natoms=12);
  Pentane->AddElement(elC, natoms=5);

  //------------------------------------------------------------------
  //18-8 (stainless) steel 
  density = 8.00*g/cm3;
  G4Material* Steel = new G4Material(name="Steel", density, ncomponents=3);
  Steel->AddElement(elCr, natoms= 8);
  Steel->AddElement(elNi, natoms=18);
  Steel->AddElement(elFe, natoms=74);  

  //------------------------------------------------------------------
  //Brass (there are many compositions) 
  density =  8.87*g/cm3;
  G4Material* Brass = new G4Material(name="Brass", density, ncomponents=3);
  Brass->AddElement(elCu, fractionmass=70.0*perCent);
  Brass->AddElement(elZn, fractionmass=28.8*perCent);
  Brass->AddElement(elMn, fractionmass= 1.2*perCent);

  //------------------------------------------------------------------
  //DurAl [3-4% Cu, 0.5% Mg, 0.25-1%Mn, remainder Al] : use average val
  density = 2.8*g/cm3;
  G4Material* DurAl = new G4Material(name="DurAluminium",
      density, ncomponents=4);
  DurAl->AddElement(elCu, fractionmass= 3.5*perCent);
  DurAl->AddElement(elMg, fractionmass= 0.5*perCent);
  DurAl->AddElement(elMn, fractionmass= 0.6*perCent);
  DurAl->AddElement(elAl, fractionmass=95.4*perCent);

  //------------------------------------------------------------------
  //Epoxy for Si detector support
  density = 1.18*g/cm3;//cast
  density = 1.85*g/cm3;//molded
  G4Material* Epoxy = new G4Material(name="Epoxy", density, ncomponents=3);
  Epoxy->AddElement(elH, natoms=19);
  Epoxy->AddElement(elC, natoms=18);
  Epoxy->AddElement(elO, natoms= 3);  

  //------------------------------------------------------------------
  //Plastic support for the Si detector support
  density = 1.68*g/cm3;//PVC
  G4Material* Pvc = new G4Material(name="PVC", density, ncomponents=3);
  Pvc->AddElement(elH,  natoms=3);
  Pvc->AddElement(elC,  natoms=2);
  Pvc->AddElement(elCl, natoms=1);  

  //------------------------------------------------------------------
  //BGO
  density = 7.13*g/cm3;//cast
  G4Material* BGO = new G4Material(name="BGO", density, ncomponents=3);
  BGO->AddElement(elBi, natoms= 4);
  BGO->AddElement(elGe, natoms= 3);
  BGO->AddElement(elO,  natoms=12);  

  //---------------------------------
  //---------------------------------
  // Print all the materials defined.
  G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  //---------------------------------

  //Default materials
  WorldMaterial      = Air;
  chamberMaterial    = DurAl;
  //backplateMaterial  = DurAl;
  defaultMaterial    = Ge;
  eDetMat            = Si;
  boardMaterial      = Epoxy;
  supportMaterial    = Steel;
  metalbarMaterial   = Steel;
  geMaterial         = Ge;
  endCapMaterial     = DurAl;
  contactMaterial    = Li;
  //vacuumInVacChamMat = Pentane;
  vacuumMaterial     = Vacuum;
  beMaterial         = Be;
  //coolMaterial       = Cu;
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Inherite Method of Vdetector Class

// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Chamber::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithTokenAndValue("Chamber","Great-Karl");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  vector<string> token = {"Heightmin","Heightmax","Widthmin","Widthmax","Depthmin","Depthmax","Material"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Great chamber as defined by Karl " << i+1 <<  endl;
      m_ChamberType = 3 ;
      m_ChamberHmin = blocks[i]->GetDouble("Heightmin","mm");
      m_ChamberHmax = blocks[i]->GetDouble("Heightmax","mm");
      m_ChamberWmin = blocks[i]->GetDouble("Widthmin","mm");
      m_ChamberWmax = blocks[i]->GetDouble("Widthmax","mm");
      m_ChamberDmin = blocks[i]->GetDouble("Depthmin","mm");
      m_ChamberDmax = blocks[i]->GetDouble("Depthmax","mm");
      m_ChamberMaterial=GetMaterialFromLibrary(blocks[i]->GetString("Material"));
    }

    else{
      cout << "Warning: check your input file formatting " << endl;
    }
  }

  blocks.clear();
  blocks = parser.GetAllBlocksWithTokenAndValue("Chamber","Great");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 


  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Great standard chamber  " << i+1 <<  endl;
      m_ChamberType = 2 ;
      m_ChamberHmin = blocks[i]->GetDouble("Heightmin","mm");
      m_ChamberHmax = blocks[i]->GetDouble("Heightmax","mm");
      m_ChamberWmin = blocks[i]->GetDouble("Widthmin","mm");
      m_ChamberWmax = blocks[i]->GetDouble("Widthmax","mm");
      m_ChamberDmin = blocks[i]->GetDouble("Depthmin","mm");
      m_ChamberDmax = blocks[i]->GetDouble("Depthmax","mm");
      m_ChamberMaterial=GetMaterialFromLibrary(blocks[i]->GetString("Material"));
    }

    else{
      cout << "Warning: check your input file formatting " << endl;
    }
  }

  blocks.clear();
  blocks = parser.GetAllBlocksWithTokenAndValue("Chamber","");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 

  token = {"Heightmin","Heightmax","Widthmin","Widthmax","Depthmin","Depthmax","Material"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  Generic Chamber " << i+1 <<  endl;
      m_ChamberType = 0 ;
      m_ChamberRmin = blocks[i]->GetDouble("Rmin","mm");
      m_ChamberRmax = blocks[i]->GetDouble("Rmax","mm");
      m_ChamberPhiMin = blocks[i]->GetDouble("PhiMin","deg");
      m_ChamberPhiMax = blocks[i]->GetDouble("PhiMax","deg");
      m_ChamberThetaMin = blocks[i]->GetDouble("ThetaMin","deg");
      m_ChamberThetaMax = blocks[i]->GetDouble("ThetaMax","deg");
      m_ChamberMaterial=GetMaterialFromLibrary(blocks[i]->GetString("Material"));
    }

    else{
      cout << "Warning: check your input file formatting " << endl;
    }
  }
}
// Little trick to avoid warning in compilation: Use a PVPlacement "buffer".
// If don't you will have a Warning unused variable 'myPVP'
G4VPhysicalVolume* PVPBuffer ;




// Construct detector and inialise sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Chamber::ConstructDetector(G4LogicalVolume* world)
{
  //   if (m_ChamberType) {   // case of standard Chamber
  if (m_ChamberType==0) {   // case of standard Chamber

    G4Sphere* solidChamber
      = new G4Sphere("solidChamber", m_ChamberRmin, m_ChamberRmax, m_ChamberPhiMin, m_ChamberPhiMax, m_ChamberThetaMin, m_ChamberThetaMax );

    G4LogicalVolume* logicChamber = new G4LogicalVolume(solidChamber, m_ChamberMaterial, "logicChamber");

    // rotation of target
    //G4RotationMatrix *rotation = new G4RotationMatrix();
    //rotation->rotateY(m_ChamberAngle);

    new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicChamber, "Chamber", world, false, 0);

    G4VisAttributes* ChamberVisAtt = new G4VisAttributes(G4Colour(0., 1., 1.));
    logicChamber->SetVisAttributes(ChamberVisAtt);
    //  }
}

else if(m_ChamberType==1){   // case of cryogenic target
}

else if(m_ChamberType==2){  // case of GREAT chamber  

  G4Box* solidExtChamber
    = new G4Box("solidExtChamber", m_ChamberWmax/2, m_ChamberHmax/2, m_ChamberDmax/2 );
  G4Box* solidIntChamber
    = new G4Box("solidIntChamber", m_ChamberWmin/2, m_ChamberHmin/2, m_ChamberDmin/2 );

  G4SubtractionSolid* solidChamber=new G4SubtractionSolid("SolidChamber",solidExtChamber, solidIntChamber, 0, G4ThreeVector(0.,0.,-0.5*cm));


  G4LogicalVolume* logicChamber = new G4LogicalVolume(solidChamber, m_ChamberMaterial, "logicChamber");

  // rotation of target
  //G4RotationMatrix *rotation = new G4RotationMatrix();
  //rotation->rotateY(m_ChamberAngle);

  PVPBuffer =
    new G4PVPlacement(0, G4ThreeVector(0., -2.92325*cm/2, 0.), logicChamber, "Chamber", world, false, 0);

  G4VisAttributes* ChamberVisAtt = new G4VisAttributes(G4Colour(0., 1., 1.));
  logicChamber->SetVisAttributes(ChamberVisAtt);


  /*  Eleanor's additions: *//*

  // for the DSSSD (considered as passive here !!):

  G4Material* m_DSSSDMaterial = new G4Material("Si", 14, 28.0855*g/mole, 2.33*g/cm3);

  G4Box* solidDSSSD
  = new G4Box("solidDSSSD", 6.*cm/2, 4.*cm/2, 0.03*cm/2 );  // 

  G4LogicalVolume* logicDSSSD = new G4LogicalVolume(solidDSSSD, m_DSSSDMaterial, "logicDSSSD");

  PVPBuffer =
  new G4PVPlacement(0, G4ThreeVector(-3.2*cm, 0., 0.), logicDSSSD, "DSSSD", world, false, 0);
  //new G4PVPlacement(0, G4ThreeVector(-3.2*cm, 2.92325*cm/2, 0.), logicDSSSD, "DSSSD", logicChamber, false, 0);
  PVPBuffer =
  new G4PVPlacement(0, G4ThreeVector( 3.2*cm, 0., 0.), logicDSSSD, "DSSSD", world, false, 1);
  //new G4PVPlacement(0, G4ThreeVector( 3.2*cm, 2.92325*cm/2, 0.), logicDSSSD, "DSSSD", logicChamber, false, 1);

  G4VisAttributes* DSSSDVisAtt = new G4VisAttributes(G4Colour(0., 0., 1.));
  logicDSSSD->SetVisAttributes(DSSSDVisAtt);



  // for the solid aluminium support :

  G4Box* solid_alu_support_ext
  = new G4Box("solid_alu_support_ext", (18.32-0.65)*cm/2, (12.343-0.65)*cm/2, 8.5*cm/2 );  // 

  G4Box* solid_alu_support_int
  = new G4Box("solid_alu_support_int",16.27*cm/2, 10.295*cm/2, 8.6*cm/2 );  // 
  G4SubtractionSolid* solidsupport=new G4SubtractionSolid("Solidsupport",solid_alu_support_ext, solid_alu_support_int, 0, G4ThreeVector(0.,0.,0));	
  G4LogicalVolume* logicALU = new G4LogicalVolume(solidsupport, m_ChamberMaterial, "logicALU");

  PVPBuffer =
  new G4PVPlacement(0, G4ThreeVector(0., -2.92325*cm/2,-4.265*cm), logicALU, "Aluminium", world, false, 0);

  G4VisAttributes* alusupportVisAtt = new G4VisAttributes(G4Colour(0.5, 0., 0.3));
  logicALU->SetVisAttributes(alusupportVisAtt);


  G4Box* solid_alu_support_int1
  = new G4Box("solid_alu_support_int1",16.27*cm/2, 0.55*cm/2, 8.6*cm/2 );  // 

  G4LogicalVolume* logicALUint1 = new G4LogicalVolume(solid_alu_support_int1, m_ChamberMaterial, "logicALUint1");

  PVPBuffer =
  new G4PVPlacement(0, G4ThreeVector(0, -2.5215*cm-2.92325*cm/2, -4.265*cm), logicALUint1, "Aluminium", world, false, 0);

  G4VisAttributes* solid_alu_support_int1VisAtt = new G4VisAttributes(G4Colour(0.9, 0., 0.));
  logicALUint1->SetVisAttributes(solid_alu_support_int1VisAtt);


  G4Box* solid_alu_support_int2
  = new G4Box("solid_alu_support_int2",1.4*cm/2, 2.35*cm/2, 8.6*cm/2 );  // 

  G4LogicalVolume* logicALUint2 = new G4LogicalVolume(solid_alu_support_int2, m_ChamberMaterial, "logicALUint2");

  PVPBuffer =
  new G4PVPlacement(0, G4ThreeVector(0, -3.9715*cm-2.92325*cm/2, -4.265*cm), logicALUint2, "Aluminium", world, false, 0);


  G4VisAttributes* solid_alu_support_int2VisAtt = new G4VisAttributes(G4Colour(0.9, 0., 0.));
  logicALUint2->SetVisAttributes(solid_alu_support_int2VisAtt);


  G4Box* solid_DSSSD_coating1
  = new G4Box("solid_DSSSD_coating1",16.27*cm/2, 0.1*cm/2, 8.6*cm/2 );  // 

  G4LogicalVolume* logicSiliconCoating1 = new G4LogicalVolume(solid_DSSSD_coating1, m_DSSSDMaterial, "logicSiliconCoating1");
  PVPBuffer =
  new G4PVPlacement(0, G4ThreeVector(0, -2.5215*cm-2.92325*cm/2+0.05*cm+0.55*cm/2, -4.265*cm), logicSiliconCoating1, "PinDiode1", world, false, 0);

  G4VisAttributes* solid_DSSSD_coating1VisAtt = new G4VisAttributes(G4Colour(0., 0.2, 0.8));
  logicSiliconCoating1->SetVisAttributes(solid_DSSSD_coating1VisAtt);


  G4Box* solid_DSSSD_coating2
    = new G4Box("solid_DSSSD_coating2",16.27*cm/2, 0.1*cm/2, 8.6*cm/2 );  // 

  G4LogicalVolume* logicSiliconCoating2 = new G4LogicalVolume(solid_DSSSD_coating2, m_DSSSDMaterial, "logicSiliconCoating2");
  PVPBuffer =
    new G4PVPlacement(0, G4ThreeVector(0, 5.1055*cm-2.92325*cm/2-0.05*cm, -4.265*cm), logicSiliconCoating2, "PinDiode2", world, false, 0);

  G4VisAttributes* solid_DSSSD_coating2VisAtt = new G4VisAttributes(G4Colour(0., 0.2, 0.8));
  logicSiliconCoating2->SetVisAttributes(solid_DSSSD_coating2VisAtt);


  G4Box* solid_DSSSD_coating3
    = new G4Box("solid_DSSSD_coating3",0.1*cm/2, 7.393*cm/2, 8.6*cm/2 );  // 

  G4LogicalVolume* logicSiliconCoating3 = new G4LogicalVolume(solid_DSSSD_coating3, m_DSSSDMaterial, "logicSiliconCoating3");
  PVPBuffer =
    new G4PVPlacement(0, G4ThreeVector(8.085*cm, 1.4375*cm-2.92325*cm/2, -4.265*cm), logicSiliconCoating3, "PinDiode3", world, false, 0);
  PVPBuffer =
    new G4PVPlacement(0, G4ThreeVector(-8.085*cm, 1.4375*cm-2.92325*cm/2, -4.265*cm), logicSiliconCoating3, "PinDiode4", world, false, 1);

  G4VisAttributes* solid_DSSSD_coating3VisAtt = new G4VisAttributes(G4Colour(0., 0.2, 0.8));
  logicSiliconCoating3->SetVisAttributes(solid_DSSSD_coating3VisAtt);



  // Preamps and connectors board

  //  PCB


  G4Element* Si   = new G4Element("Silicon" , "Si"  , 14  , 28.0855  * g / mole);
  G4Element* C   = new G4Element("Carbon"   , "C"  , 6  , 12.011  * g / mole);
  G4Element* H   = new G4Element("Hydrogen"   , "H"  , 1  , 1.0079  * g / mole);
  G4Element* Br   = new G4Element("Bromine"   , "Br"  , 35  , 79.904 * g / mole);
  G4Element* O   = new G4Element("Oxigen"   , "O"  , 8  , 16.00  * g / mole);

  G4double density = 1.7 * g / cm3;
  G4int ncomponents;
  G4Material* PCB = new G4Material("PCB", density, ncomponents = 5);
  PCB->AddElement(Si, .181);
  PCB->AddElement(O, .406);
  PCB->AddElement(C, .278);
  PCB->AddElement(H, .068);
  PCB->AddElement(Br, .067);



  G4Box* pre_amp_board1
    = new G4Box("pre_amp_board1",6.756*cm/2, 1.675*cm/2, 7.976*cm/2 );  // 

  G4LogicalVolume* logicpre_amp1 = new G4LogicalVolume(pre_amp_board1, PCB, "logicpre_amp1");
  PVPBuffer =
    new G4PVPlacement(0, G4ThreeVector(4.0675*cm, 5.267875*cm, -4.265*cm), logicpre_amp1, "pre_amp_board1", world, false, 0);
  PVPBuffer =
    new G4PVPlacement(0, G4ThreeVector(-4.0675*cm, 5.267875*cm, -4.265*cm), logicpre_amp1, "pre_amp_board1", world, false, 1);


  G4VisAttributes* pre_amp_board1VisAtt = new G4VisAttributes(G4Colour(0., 0.7, 0.));
  logicpre_amp1->SetVisAttributes(pre_amp_board1VisAtt);

  G4Box* pre_amp_board2
    = new G4Box("pre_amp_board2",1.675*cm/2, 6.756*cm/2, 7.976*cm/2 );  // 

  G4LogicalVolume* logicpre_amp2 = new G4LogicalVolume(pre_amp_board2, PCB, "logicpre_amp2");
  PVPBuffer =
    new G4PVPlacement(0, G4ThreeVector(9.6625*cm, 2.413375*cm, -4.265*cm), logicpre_amp2, "pre_amp_board2", world, false, 0);
  PVPBuffer =
    new G4PVPlacement(0, G4ThreeVector(9.6625*cm, -5.336625*cm, -4.265*cm), logicpre_amp2, "pre_amp_board2", world, false, 1);


  G4VisAttributes* pre_amp_board2VisAtt = new G4VisAttributes(G4Colour(0., 0.7, 0.));
  logicpre_amp2->SetVisAttributes(pre_amp_board2VisAtt);


  G4Box* pre_amp_board3
    = new G4Box("pre_amp_board3",1.675*cm/2, 6.756*cm/2, 7.976*cm/2 );  // 

  G4LogicalVolume* logicpre_amp3 = new G4LogicalVolume(pre_amp_board3, PCB, "logicpre_amp3");
  PVPBuffer =
    new G4PVPlacement(0, G4ThreeVector(-9.6725*cm, 2.413375*cm, -4.265*cm), logicpre_amp3, "pre_amp_board3", world, false, 0);
  PVPBuffer =
    new G4PVPlacement(0, G4ThreeVector(-9.6725*cm, -5.336625*cm, -4.265*cm), logicpre_amp3, "pre_amp_board3", world, false, 1);


  G4VisAttributes* pre_amp_board3VisAtt = new G4VisAttributes(G4Colour(0., 0.7, 0.));
  logicpre_amp3->SetVisAttributes(pre_amp_board3VisAtt);

  */ /* end of Eleanor's additions */  





}


else if(m_ChamberType==3){  // case of GREAT chamber as defined by Karl  

  fChamberH = m_ChamberHmax;
  fChamberW = m_ChamberWmax;
  //fChamberL = m_ChamberDmax;
  fChamberThickness= m_ChamberHmax-m_ChamberHmin;

  fVacuumH = fChamberH - fChamberThickness;

  G4cout << "fChamberH=" << fChamberH << G4endl;
  G4cout << "fChamberW=" << fChamberW << G4endl;
  G4cout << "fChamberThickness=" << fChamberThickness << G4endl;

  //-----------------------------
  // more dimesions for chamber
  fChamberL       = fCoolingBlockL + fLengthOfPlanarCut + fDSSD_BoardL + fdeltaZ_Support + 2.*fChamberThickness + 10.0*mm;


  //---------------------------------------------------------------------------------
  // This shift in the position of the chamber is to put the DSSD centred at y = 0
  fShiftChamberY  = fCoolingBlockH - fCoolingBlockT - fCoolingBlockCutOutH/2.;   //height of centre wrt cooling block base
  fShiftChamberY -= fChamberH/2.;

  //Define the downstream face of the DSSD to be at 0 (ie the centre of the DSSD PCB board)
  fShiftChamberZ  = fCoolingBlockZ + fChamberThickness - fDSSD_BoardL/2.;  //a discrepency of 1 mm somewhere

  //the centre of the DSSD wrt the vacuum chamber
  fCentreOfDSSD_Y = -fVacuumH/2. + fCoolingBlockH - fCoolingBlockT - fCoolingBlockCutOutH/2.; //+ fChamberThickness
  cout << G4endl << " DSSD_Y at 0 mm , but wrt vacuum DSSD_Y @ " << fCentreOfDSSD_Y << G4endl;





  //--------------------------------------------
  //[partial] Vacuum Chamber (Walls+Vacuum) and end BackPlate
  //Set the Upstream side of the Chamber @ Z=0mm


  //G4Box* solidChamber = new G4Box("chamber",fChamberW/2., fChamberH/2., fChamberL/2.);
  // replaced by achamber with  a substraction of the Planar area:
  G4Box* solidChamberEnv = new G4Box("chamberEnv",fChamberW/2., fChamberH/2., fChamberL/2.);
  G4double PlanarCap_W = fPlanarGe_W + 2.*fPlanarGuard + 80.*mm; //A GUESS
  G4double PlanarCap_H = 150.*mm;
  PlanarCap_H = fPlanarGe_H + 70*mm;
  G4double PlanarCap_T = fPlanarTotalL;
  G4cout << G4endl << "Planar end-cap thickness " << PlanarCap_T << " cf 41. mm" << G4endl;
  fShiftPlanar      = fChamberL/2. - fChamberThickness +  PlanarCap_T/2. - fLengthOfPlanarCut + fGap_PCB_2_Planar;
  G4double Planar_z = fShiftPlanar;
  G4Box* solidPlanarCapEnv = new G4Box("planar-cap-Env", PlanarCap_W/2., PlanarCap_H/2., PlanarCap_T/2.);
  G4SubtractionSolid* solidChamber = 
    new G4SubtractionSolid("chamber", solidChamberEnv, solidPlanarCapEnv, 0, G4ThreeVector(0.0*mm, fCentreOfDSSD_Y, Planar_z));

  // end of replacement

  G4LogicalVolume* logicChamber = new G4LogicalVolume(solidChamber,   //it's solid
      m_ChamberMaterial,//it's material
      //chamberMaterial,//it's material
      //vacuumMaterial,//it's material
      "Chamber",      //it's name
      0, 0, 0); // field manager, sensitive det, user limits  

  PVPBuffer = new G4PVPlacement(0,	             //rotation
      G4ThreeVector(0.0*mm,-fShiftChamberY,-fChamberL/2.+fShiftChamberZ),
      logicChamber,     //its logical volume
      "Chamber",        //its name
      world,       //its mother
      false,            //no boolean operat
      0,                //copy number
      true);            //overlap check

  //viewing the chamber
  G4VisAttributes* visAttChamber = new G4VisAttributes(G4Colour(1.0,1.0,1.0) );
  visAttChamber->SetVisibility(true);
  visAttChamber->SetForceWireframe(true);
  logicChamber->SetVisAttributes(visAttChamber);
  //logicChamber->SetVisAttributes(G4VisAttributes::Invisible);



  //Vacuum within above partial chamber

  G4double vacH = fChamberH - fChamberThickness;
  G4double vacW = fChamberW - 2.*fChamberThickness;
  G4double vacL = fChamberL - 2.*fChamberThickness;

  //G4Box* solidChamVac = new G4Box("chamVac",vacW/2., vacH/2., vacL/2.);
  //Replaced by
  G4Box* solidChamVacEnv = new G4Box("chamVacEnV",vacW/2., vacH/2., vacL/2.);
  //G4Box* solidPlanarCapEnv = new G4Box("planar-cap-Env", PlanarCap_W/2., PlanarCap_H/2., PlanarCap_T/2.);
  G4SubtractionSolid* solidChamVac = 
    new G4SubtractionSolid("chamVac", solidChamVacEnv, solidPlanarCapEnv, 0, G4ThreeVector(0.0*mm, fCentreOfDSSD_Y+fChamberThickness/2., Planar_z));


  G4LogicalVolume* logicChamVac = new G4LogicalVolume(solidChamVac,   //it's solid
      vacuumMaterial, //it's material
      "chamVac",      //it's name
      0, 0, 0); // field manager, sensitive det, user limits  

  PVPBuffer = new G4PVPlacement(0,	             //rotation
      G4ThreeVector(0.0*cm, -fChamberThickness/2., 0.0*cm),
      logicChamVac,     //its logical volume
      "ChamVac",        //its name
      logicChamber,     //its mother
      false,            //no boolean operat
      0,                //copy number
      true);            //overlap check

  //logicChamVac->SetVisAttributes(G4VisAttributes::Invisible);
  logicChamVac->SetVisAttributes(visAttChamber);



  /* Uncomment if you want the planar in the geometry and you are not using GRTPlan */
  /*
  //----------------------------------------------------------------------
  // The Planar
  //----------------------------------------------------------------------
  G4double PlanarCap_W = fPlanarGe_W + 2.*fPlanarGuard + 80.*mm; //A GUESS
  G4double PlanarCap_H = 150.*mm;
  PlanarCap_H = fPlanarGe_H + 70*mm;
  G4double PlanarCap_T = fPlanarTotalL;
  G4cout << G4endl << "Planar end-cap thickness " << PlanarCap_T << " cf 41. mm" << G4endl;

  // - editted 07/11/2011
  fShiftPlanar      = fChamberL/2. - fChamberThickness +  PlanarCap_T/2. - fLengthOfPlanarCut + fGap_PCB_2_Planar;
  G4double Planar_z = fShiftPlanar;
  fPlanar_PosZ = Planar_z;

  //starting endCap sold
  G4Box* solidPlanarCap = new G4Box("planar-cap", PlanarCap_W/2., PlanarCap_H/2., PlanarCap_T/2.);

  //cut outs for Be window : planar end-cap is deliberately symmetric to make positioning more simple 
  //3 mm wide support rib that is 2.2 mm thick. Add the Be thickness => endCap thickness
  G4Box* solidBeCutOut = new G4Box("be-cut-out", fPlanarGe_W/4.-1.5*mm, fPlanarGe_H/2., fEndCapThickness/2.);//is it total 120 mm or 120+gap ? 
  G4ThreeVector  beCutOutTrans1(-fPlanarGe_W/4.-1.5*mm, 0.0*mm, -PlanarCap_T/2.+fEndCapThickness/2.);  
  G4ThreeVector  beCutOutTrans2(+fPlanarGe_W/4.+1.5*mm, 0.0*mm, -PlanarCap_T/2.+fEndCapThickness/2.);


  G4SubtractionSolid* solidPlanarCap_tmp1 = 
  new G4SubtractionSolid("planar-capo-1", solidPlanarCap, solidBeCutOut, 0, beCutOutTrans1);

  G4SubtractionSolid* solidPlanarCap_tmp2 = 
  new G4SubtractionSolid("planar-capo-2", solidPlanarCap_tmp1, solidBeCutOut, 0, beCutOutTrans2);


  //cut outs for Al window at rear
  G4Box* solidAlCutOut = new G4Box("Al-cut-out", fPlanarGe_W/2., fPlanarGe_H/2., fAlWindowThickness/2.);
  G4ThreeVector  alCutOutTrans(0.0*mm, 0.0*mm, PlanarCap_T/2.-fAlWindowThickness/2.); //ends up being 0.9 mm instead of 1.1 mm thick: CORRECT IT

  G4SubtractionSolid* solidPlanarCap_tmp3 = 
  new G4SubtractionSolid("planar-capo-3", solidPlanarCap_tmp2, solidAlCutOut, 0, alCutOutTrans);


  G4LogicalVolume* logicPlanarCap = new G4LogicalVolume(solidPlanarCap_tmp3, // it's solid
  //logicPlanarCap = new G4LogicalVolume(solidPlanarCap, // it's solid
  endCapMaterial, // it's material
  "planar-cap",       // it's name
  0, 0, 0); // field manager, sensitive det, user limits  

  PVPBuffer = new G4PVPlacement(0,               //rotation
  G4ThreeVector(0.0*mm, fCentreOfDSSD_Y, Planar_z),
  logicPlanarCap,  //its logical volume
  "PlanarCap",     //its name
  logicChamVac,    //its mother
  false,           //no boolean operat
  0,               //copy number
  true);           //overlap check




  G4VisAttributes* visAttPlanarCapo = new G4VisAttributes(G4Colour(1.0,0.8,1.0) );
  visAttPlanarCapo->SetVisibility(true);
  //visAttPlanarCapo->SetVisibility(false);
  visAttPlanarCapo->SetForceWireframe(true);
  logicPlanarCap->SetVisAttributes(visAttPlanarCapo);


  //-----------------------------------------------------------------------------------------------
  //add the Be window
  G4double PlanarBeWindow_z = -PlanarCap_T/2. + fBeWindowThickness/2.;

  G4Box* solidPlanarBeWindow = new G4Box("planar-be", fPlanarGe_W/2., fPlanarGe_H/2., fBeWindowThickness/2.);

  G4LogicalVolume* logicPlanarBeWindow = new G4LogicalVolume(solidPlanarBeWindow,// it's solid
      beMaterial,   // it's material
      "planar-be",   // it's name
      0, 0, 0); // field manager, sensitive det, user limits  

  PVPBuffer = new G4PVPlacement(0,            //rotation
      G4ThreeVector(0.0*mm, 0.0*mm, PlanarBeWindow_z),
      logicPlanarBeWindow, //its logical volume
      "Planar-Be",     //its name
      logicPlanarCap, //its mother
      false,        //no boolean operat
      0,            //copy number
      true);        //overlap check

  G4VisAttributes* visAttBeWindow = new G4VisAttributes(G4Colour(0.9,1.0,0.9) );
  visAttBeWindow->SetVisibility(true);
  //visAttBeWindow->SetVisibility(false);
  visAttBeWindow->SetForceWireframe(true);
  logicPlanarBeWindow->SetVisAttributes(visAttBeWindow);
  //-----------------------------------------------------------------------------------------------
  //special cut out for Vacuum....
  //fAlWindowThickness
  //fEndCapThickness 
  //PlanarCap_T - 2*fEndCapThickness



  //cut outs for windows and vacuum
  G4Box* solidPlanarVac = new G4Box("planar-vac", PlanarCap_W/2.-fEndCapThickness, PlanarCap_H/2.-fEndCapThickness, PlanarCap_T/2.-fEndCapThickness);

  G4LogicalVolume* logicPlanarVac = new G4LogicalVolume(solidPlanarVac,// it's solid
      vacuumMaterial,   // it's material
      "planar-vac",   // it's name
      0, 0, 0); // field manager, sensitive det, user limits  

  PVPBuffer = new G4PVPlacement(0,            //rotation
      G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm),
      logicPlanarVac, //its logical volume
      "Planar-Vac",     //its name
      logicPlanarCap, //its mother
      false,        //no boolean operat
      0,            //copy number
      true);        //overlap check

  G4cout << "Got Here" << G4endl;

  //visual attributes of the planar end-cap
  //G4VisAttributes* visAttPlanarCapo = new G4VisAttributes(G4Colour(1.0,0.8,1.0) );
  //visAttPlanarCapo->SetVisibility(true);
  //visAttPlanarCapo->SetVisibility(false);
  //visAttPlanarCapo->SetForceWireframe(true);
  //logicPlanarCap->SetVisAttributes(visAttPlanarCapo);
  logicPlanarVac->SetVisAttributes(visAttPlanarCapo);




  // the planar Ge
  G4double PlanarGe_z = -PlanarCap_T/2. + fPlanarGe_T/2. + fPlanarGeGapFront;

  //add in a slab of Ge to accomodate the guard-rail
  //and into this slab add the pixels
  G4Box* solidPlanarOuterGe = new G4Box("planar-outer-ge", fPlanarGe_W/2.+fPlanarGuard, fPlanarGe_H/2.+fPlanarGuard, fPlanarGe_T/2.);
  G4LogicalVolume* logicPlanarOuterGe = new G4LogicalVolume(solidPlanarOuterGe,      // it's solid
      geMaterial,              // it's material
      "planar-outer-ge",       // it's name
      0, 0, 0); // field manager, sensitive det, user limits  

  //G4PVPlacement* physiPlanarOuterGe = new G4PVPlacement(0,            //rotation
  PVPBuffer = new G4PVPlacement(0,            //rotation
      G4ThreeVector(0.*mm, 0.0*mm, PlanarGe_z),
      logicPlanarOuterGe, //its logical volume
      "Planar-Outer-Ge",           //its name
      logicPlanarVac,        //its mother
      false,                 //no boolean operat
      0, true);             //copy number  

  G4VisAttributes* visAttPlanarOuterGe = new G4VisAttributes(G4Colour(0.9,0.9,1.0) );
  visAttPlanarOuterGe->SetVisibility(true);
  //visAttPlanarOuterGe->SetVisibility(false);
  visAttPlanarOuterGe->SetForceWireframe(true);
  logicPlanarOuterGe->SetVisAttributes(visAttPlanarOuterGe);



  //segment the detector
  G4int nSegsH = 12; //(y)
  G4int nSegsW = 24; //(x)

  //nSegsH = 1;
  //nSegsW = 1;

  //common planar "pixel" shape
  G4double planarPixel_H = fPlanarGe_H/(G4double) nSegsH;
  G4double planarPixel_W = fPlanarGe_W/(G4double) nSegsW;

  G4Box* solidPlanarGe = new G4Box("planar-ge", planarPixel_W/2., planarPixel_H/2., fPlanarGe_T/2.);

  G4VisAttributes* visAttPlanarGe = new G4VisAttributes(G4Colour(1.0,1.0,1.0) );
  visAttPlanarGe->SetVisibility(true);
  //visAttPlanarGe->SetVisibility(false);
  visAttPlanarGe->SetForceWireframe(true);
  logicPlanarOuterGe->SetVisAttributes(visAttPlanarGe);

  //starting y-position for lowest row
  G4double thisY = -fPlanarGe_H/2. + planarPixel_H/2.;// + fDSSD_PosY;

  for(G4int ny = 0; ny < nSegsH; ny++) {
    //starting x-position
    G4double thisX = -fPlanarGe_W/2. + planarPixel_W/2.;
    //G4double posY = (2*ny-1)*planarPixel_H/2.;

    for(G4int nx = 0; nx < nSegsW; nx++) {
      //G4double posX = -fPlanarGe_W/2. + planarPixel_W/2. + nx*planarPixel_W;
      //G4int copyNo  = ny*nSegsW+nx;
      //G4cout << "Check Planar CopyNo " << copyNo << " @ " << posX << " " << posY << G4endl;
      G4int copyNo  = ny*100+nx;
      G4cout << "Check Planar CopyNo " << copyNo << " @ " << thisX << " " << thisY << G4endl;

      //logicPlanarGe[copyNo] = new G4LogicalVolume(solidPlanarGe,// it's solid
      logicPlanarGe[ny][nx] = new G4LogicalVolume(solidPlanarGe,// it's solid
          geMaterial,   // it's material
          "planar-ge",   // it's name
          0, 0, 0); // field manager, sensitive det, user limits  


      //physiPlanarGe[copyNo] = new G4PVPlacement(0,            //rotation
      //G4ThreeVector(posX, posY, 0.0*mm),
      //physiPlanarGe[ny][nx] = new G4PVPlacement(0,            //rotation
      PVPBuffer = new G4PVPlacement(0,            //rotation
          G4ThreeVector(thisX, thisY, 0.0*mm),
          //logicPlanarGe[copyNo], //its logical volume
          logicPlanarGe[ny][nx], //its logical volume
          "Planar-Ge",           //its name
          logicPlanarOuterGe,        //its mother
          false,                 //no boolean operat
          copyNo);             //copy number

      //set attributes
      logicPlanarGe[ny][nx]->SetVisAttributes(visAttPlanarGe);

      //move along....
      thisX += planarPixel_W;
    }
    thisY += planarPixel_H;
  }

  */

    //----------------------------------------------------------
    // Cooling Frame: Tunnel and DSSD Detector Support
    //----------------------------------------------------------
    G4VisAttributes* visAttCoolingBlock = new G4VisAttributes(G4Colour(0.8,0.8,0.8));
  visAttCoolingBlock->SetVisibility(true);
  //visAttCoolingBlock->SetVisibility(false);
  visAttCoolingBlock->SetForceWireframe(true);

  G4Box*   OuterBox = new G4Box("OuterBox",fCoolingBlockW/2., fCoolingBlockH/2., fCoolingBlockL/2.);

  //place the support on the bottom of the chamber
  G4double DSSD_SupPos_y = -fChamberH/2. + fCoolingBlockH/2. + fChamberThickness/2.;

  //position wrt BackPlate/VacChamber
  G4double DSSD_SupPos_z = fChamberL/2. - fChamberThickness - fCoolingBlockL/2. - fCoolingBlockZ;  

  //now cut a hole out for the DSSD's
  G4double Hole4DSSD_W = fCoolingBlockCutOutW;
  G4double Hole4DSSD_H = fCoolingBlockCutOutH;

  //from the above can determine the centre of the DSSD wrt centre of Cooling block
  fCoolingBlockCutOut_PosY = fCoolingBlockH/2. - fCoolingBlockT - fCoolingBlockCutOutH/2.; //save this frequently used position
  G4cout << "fCoolingBlockCutOut_PosY " << fCoolingBlockCutOut_PosY << " cf " << fCoolingBlockDSSDCentre << G4endl;

  G4Box*  hole1 = new G4Box("Hole #1",Hole4DSSD_W/2., Hole4DSSD_H/2., fCoolingBlockL/2.);

  G4double   dy = fCoolingBlockH/2. - fCoolingBlockT - Hole4DSSD_H/2.; 
  G4double   dx = 0.0*mm; 
  G4double   dz = 0.0*mm; 
  G4ThreeVector  hole1trans(dx, dy, dz);

  G4SubtractionSolid* solid_CB_Temp1 = 
    new G4SubtractionSolid("fCoolingBlockTemp1", OuterBox, hole1, 0, hole1trans);

  //now cut a hole out for preamps
  G4Box*  holePreAmp = new G4Box("Hole PreAmp",34.5/2.*mm, 22./2.*mm, fCoolingBlockL/2.);
  dx = -fCoolingBlockW/2.+fCoolingBlockT+34.5/2.*mm;
  dy = fCoolingBlockH/2. - fCoolingBlockT - Hole4DSSD_H - fCoolingBlockT - 22./2.*mm;
  G4ThreeVector  holePreAmpTrans1(dx, dy, dz);
  dx += (34.5*mm + fCoolingBlockT);
  G4ThreeVector  holePreAmpTrans2(dx, dy, dz);
  dx += (34.5*mm + 14.7*mm);  
  G4ThreeVector  holePreAmpTrans3(dx, dy, dz);
  dx += (34.5*mm + fCoolingBlockT);
  G4ThreeVector  holePreAmpTrans4(dx, dy, dz);

  G4SubtractionSolid* solid_CB_Temp2 = 
    new G4SubtractionSolid("fCoolingBlockTemp2", solid_CB_Temp1, holePreAmp, 0, holePreAmpTrans1);

  G4SubtractionSolid* solid_CB_Temp3 = 
    new G4SubtractionSolid("fCoolingBlockTemp3", solid_CB_Temp2, holePreAmp, 0, holePreAmpTrans2);

  G4SubtractionSolid* solid_CB_Temp4 = 
    new G4SubtractionSolid("fCoolingBlockTemp4", solid_CB_Temp3, holePreAmp, 0, holePreAmpTrans3);

  G4SubtractionSolid* solid_CB_Temp5 = 
    new G4SubtractionSolid("fCoolingBlockTemp5", solid_CB_Temp4, holePreAmp, 0, holePreAmpTrans4);

  G4LogicalVolume* logicDSSD_Sup = new G4LogicalVolume(solid_CB_Temp5,
      supportMaterial, "Cylinder-Box", 0, 0, 0);
  //vacuumMaterial, "Cylinder-Box", 0, 0, 0);

  logicDSSD_Sup->SetVisAttributes(visAttCoolingBlock);


  PVPBuffer = new G4PVPlacement(0,//rotation
      G4ThreeVector(0.0*cm, DSSD_SupPos_y, DSSD_SupPos_z),
      logicDSSD_Sup,   //its logical volume
      "fCoolingBlock",  //its name
      logicChamVac,      //its mother
      false,           //no boolean operat
      1,              //copy number
      true);            //overlap check





  //-------------------------------------------------------------------------------------------
  // DSSD detector Epoxy board....for some reason 2 cut outs are not working => make 2 PCB wafers
  G4double DSSD_BrdW = 106.0 * mm * 2.;  //Put the 2 boards in one plate
  G4double DSSD_BrdH = 157.0 * mm;       //SupportH;
  G4double DSSD_BrdL = fDSSD_BoardL;     //Thickness

  //recess to house DSSD's
  G4double siImpBrdCutOutL = fDSSD_BoardL/2.;
  // making 2 boards =>
  G4double DSSD_BrdL1 = DSSD_BrdL - siImpBrdCutOutL;
  G4double DSSD_BrdL2 = siImpBrdCutOutL;

  G4double CutOut_dY = 44.0*mm;     // distance from top of PCB to top of the first cut-out in the PCB
  G4double CutOut1_H = 43.5*mm;     // vertical dimension of the cut-out to half thickness
  G4double CutOut2_H = 41.0*mm;     // vertical dimension of total-thickness cut-ou ie a mounting lip of 1.25 mm

  G4double CutOut1_W = 63.5*mm;     //fDSSD_H-2.0*boardLipDSSD;
  G4double CutOut2_W = 61.0*mm;     //fDSSD_H;

  G4double dssdLip   = (CutOut1_W-CutOut2_W)/2.;
  G4double central_bar = 1.25*mm; //each side

  G4double DSSD_PCB_Pos_y = -fVacuumH/2. - DSSD_BrdH/2.;  //puts top of circuit board at bottom of chamber
  DSSD_PCB_Pos_y += (fCoolingBlockDSSDCentre + CutOut_dY + CutOut1_H/2.);

  G4double gapBetweenDSSD = 3.0 * mm;  //Gap between the two DSSD's

  //gapBetweenDSSD = 0.01*mm;

  //position wrt centre of vacuum in chamber
  G4double DSSD_BrdPos_z = fChamberL/2 - fChamberThickness - fCoolingBlockZ + DSSD_BrdL1/2 + DSSD_BrdL2;  

  G4Box* solidDSSD_Board1_tmp1 = 
    new G4Box("siImpBrd1_tmp", DSSD_BrdW/2., DSSD_BrdH/2., DSSD_BrdL1/2.);

  G4Box*         cutOut1a = new G4Box("Box R #1a",CutOut1_W/2., CutOut1_H/2., DSSD_BrdL1/2.);
  G4ThreeVector  cutOut1aTrans(CutOut1_W/2.,  DSSD_BrdH/2.-CutOut1_H/2.-CutOut_dY, 0.0*mm);

  G4Box*         cutOut1b = new G4Box("Box R #1b",CutOut1_W/2.,CutOut1_H/2.,DSSD_BrdL1/2.);
  G4ThreeVector  cutOut1bTrans(-CutOut1_W/2., DSSD_BrdH/2.-CutOut1_H/2.-CutOut_dY, 0.0*mm);


  G4SubtractionSolid* solidDSSD_Board1_a = 
    new G4SubtractionSolid("siImpBrd1_a", solidDSSD_Board1_tmp1, cutOut1a, 0, cutOut1aTrans);

  G4SubtractionSolid* solidDSSD_Board1 = 
    new G4SubtractionSolid("siImpBrd1", solidDSSD_Board1_a, cutOut1b, 0, cutOut1bTrans);

  G4LogicalVolume* logicDSSD_Board1 = new   G4LogicalVolume(solidDSSD_Board1,// it's solid : with cut-out
      boardMaterial,  // it's material
      //vacuumMaterial,
      "DSSD_Brd1",     // it's name
      0, 0, 0); // field manager, sensitive det, user limits  

  PVPBuffer = new G4PVPlacement(0,                //rotation
      G4ThreeVector(0.0*mm,DSSD_PCB_Pos_y,DSSD_BrdPos_z),
      logicDSSD_Board1,  //its logical volume
      "DSSD_Brd1",       //its name
      logicChamVac,     //its mother
      false,            //no boolean operat
      0,               //copy number
      true);            //overlap check


  DSSD_BrdPos_z -= DSSD_BrdL1/2;  
  DSSD_BrdPos_z -= DSSD_BrdL2/2;  

  G4Box* solidDSSD_Board2_tmp1 = 
    new G4Box("siImpBrd2_tmp", DSSD_BrdW/2., DSSD_BrdH/2., DSSD_BrdL2/2.);

  G4Box*         cutOut2a = new G4Box("Box R #2a",CutOut2_W/2.,CutOut2_H/2.,DSSD_BrdL2/2.);
  G4ThreeVector  cutOut2aTrans(CutOut2_W/2.+central_bar,  DSSD_BrdH/2.-CutOut2_H/2.-CutOut_dY-dssdLip, 0.0*mm);

  G4Box*         cutOut2b = new G4Box("Box R #2b",CutOut2_W/2.,CutOut2_H/2.,DSSD_BrdL2/2.);
  G4ThreeVector  cutOut2bTrans(-CutOut2_W/2.-central_bar,  DSSD_BrdH/2.-CutOut2_H/2.-CutOut_dY-dssdLip, 0.0*mm);

  G4SubtractionSolid* solidDSSD_Board2_a = 
    new G4SubtractionSolid("siImpBrd2_a", solidDSSD_Board2_tmp1, cutOut2a, 0, cutOut2aTrans);

  G4SubtractionSolid* solidDSSD_Board2 = 
    new G4SubtractionSolid("siImpBrd2", solidDSSD_Board2_a, cutOut2b, 0, cutOut2bTrans);

  G4LogicalVolume* logicDSSD_Board2 = new   G4LogicalVolume(solidDSSD_Board2,// it's solid : with cut-out
      boardMaterial,  // it's material
      //vacuumMaterial,
      "DSSD_Brd2",     // it's name
      0, 0, 0); // field manager, sensitive det, user limits  

  PVPBuffer = new G4PVPlacement(0,                //rotation
      G4ThreeVector(0.0*mm,DSSD_PCB_Pos_y,DSSD_BrdPos_z),
      logicDSSD_Board2,  //its logical volume
      "DSSD_Brd2",       //its name
      logicChamVac,     //its mother
      false,            //no boolean operat
      0,               //copy number
      true);            //overlap check



  //--------------------------------------------
  // Implantation detectors
  //first make two Si boards -> gauard ring...
  G4VisAttributes* visAttDSSD_1 = new G4VisAttributes(G4Colour(0.6,0.4,0.4) );
  visAttDSSD_1->SetVisibility(true);
  //visAttDSSD_1->SetVisibility(false);
  visAttDSSD_1->SetForceWireframe(true);

  G4double DSSD_z = DSSD_BrdPos_z - DSSD_BrdL/2. - 3.*fDSSD_T/2. + (DSSD_BrdL-siImpBrdCutOutL);
  DSSD_z = fChamberL/2 - fChamberThickness - fCoolingBlockZ + DSSD_BrdL2 - fDSSD_T/2.;
  G4cout << "DSSD Z wrt ChamberVac " << DSSD_z << G4endl;

  G4double totalDSSD_H = fDSSD_H+2.*fDSSD_Guard;
  G4double totalDSSD_W = fDSSD_W+2.*fDSSD_Guard;
  G4Box*           solidSi1 = new G4Box("Si1", totalDSSD_W/2., totalDSSD_H/2., fDSSD_T/2.);  //Si: 62x42
  G4LogicalVolume* logicSi1 = new G4LogicalVolume(solidSi1,   // it's solid
      eDetMat,     // it's material //vacuumMaterial,
      "dssd1",     // it's name
      0, 0, 0);    // field manager, sensitive det, user limits  

  logicSi1->SetVisAttributes(visAttDSSD_1);

  PVPBuffer = new G4PVPlacement(0,             //rotation
      G4ThreeVector(totalDSSD_W/2.+0.75*mm,fCentreOfDSSD_Y,DSSD_z),
      //G4ThreeVector(0.0*mm,fCentreOfDSSD_Y,DSSD_z),
      logicSi1,     //its logical volume
      "dssd1",     //its name
      logicChamVac, //its mother
      false,        //no boolean operat
      1);           //,       //copy number

  G4LogicalVolume* logicSi2 = new G4LogicalVolume(solidSi1,   // it's solid
      eDetMat,     // it's material //vacuumMaterial,
      "dssd2",     // it's name
      0, 0, 0);    // field manager, sensitive det, user limits  

  logicSi2->SetVisAttributes(visAttDSSD_1);

  PVPBuffer = new G4PVPlacement(0,             //rotation
      G4ThreeVector(-totalDSSD_W/2.-0.75*mm,fCentreOfDSSD_Y,DSSD_z),
      //G4ThreeVector(0.0*mm,fCentreOfDSSD_Y,DSSD_z),
      logicSi2,     //its logical volume
      "dssd2",     //its name
      logicChamVac, //its mother
      false,        //no boolean operat
      2);           //,       //copy number


  // The ChamberVac is located wrt centre of Chamber => z0 = -fChamberL/2.
  fDSSD_PosZ = DSSD_z - fChamberL/2. - fDSSD_T/2.;
  G4cout << "Upstream DSSD face @ Z = " << fDSSD_PosZ << G4endl;


  //common dssd "pixel" shape
  G4VisAttributes* visAttDSSD = new G4VisAttributes(G4Colour(0.8,0.3,0.3) );
  visAttDSSD->SetVisibility(true);
  //visAttDSSD->SetVisibility(false);
  visAttDSSD->SetForceWireframe(true);




  G4double fDSSD_Pixel_Lx = fDSSD_W/(1.0*nStripsX);
  G4double fDSSD_Pixel_Ly = fDSSD_H/(1.0*nStripsY);

  G4Box* solidDSSD = new G4Box("dssd", fDSSD_Pixel_Lx/2., fDSSD_Pixel_Ly/2., fDSSD_T/2.);


  //starting y-position for lowest row
  G4double thisy = -nStripsY/2.*fDSSD_Pixel_Ly + fDSSD_Pixel_Ly/2. + fCoolingBlockCutOut_PosY;
  thisy = -nStripsY/2.*fDSSD_Pixel_Ly + fDSSD_Pixel_Ly/2. + fCentreOfDSSD_Y;

  G4double thisy2 = -fDSSD_H/2. + fDSSD_Pixel_Ly/2.;

  for(G4int iy = 0; iy < nStripsY; iy++) {
    //starting x-position
    G4double thisx = -nStripsX*fDSSD_Pixel_Lx + fDSSD_Pixel_Lx/2. - gapBetweenDSSD/2.;
    G4double thisx2 = -fDSSD_W/2. + fDSSD_Pixel_Lx/2.;
    for(G4int ix = 0; ix < nStripsX; ix++) {

      //DSSD_A
      logicDSSD_A[iy][ix] = new G4LogicalVolume(solidDSSD,   // it's solid
          eDetMat,     // it's material
          //vacuumMaterial,
          "dssd-pixel",// it's name
          0, 0, 0);    // field manager, sensitive det, user limits  

      PVPBuffer = new G4PVPlacement(0,            //rotation
          //G4ThreeVector(thisx,thisy,DSSD_z),
          G4ThreeVector(thisx2,thisy2,0.0*mm),
          logicDSSD_A[iy][ix],//its logical volume
          "DSSD_A",     //its name
          logicSi2, //its mother
          false,        //no boolean operat
          iy*100+ix);//,       //copy number


      //DSSD_B
      logicDSSD_B[iy][ix] = new G4LogicalVolume(solidDSSD,   // it's solid
          eDetMat,     // it's material
          //vacuumMaterial,
          "dssd-pixel",// it's name
          0, 0, 0);    // field manager, sensitive det, user limits  

      PVPBuffer = new G4PVPlacement(0,            //rotation
          //G4ThreeVector(thisx+fDSSD_W+gapBetweenDSSD,thisy,DSSD_z),
          G4ThreeVector(thisx2,thisy2,0.0*mm),
          logicDSSD_B[iy][ix],//its logical volume
          "DSSD_B",     //its name
          logicSi1, //its mother
          false,        //no boolean operat
          iy*100+ix);//,       //copy number


      //visu
      logicDSSD_A[iy][ix]->SetVisAttributes(visAttDSSD);
      logicDSSD_B[iy][ix]->SetVisAttributes(visAttDSSD);

      //move along please
      //thisN++;
      thisx  += fDSSD_Pixel_Lx;
      thisx2 += fDSSD_Pixel_Lx;
    }
    thisy  += fDSSD_Pixel_Ly;
    thisy2 += fDSSD_Pixel_Ly;
  }


  //--------------------------------------------
  // The PINS
  //--------------------------------------------
  G4double PinL          = 30.0 * mm; //PinLxPinL
  G4double PinT          =  1.0 * mm; //0.5 * mm; //Thickness
  G4double PinGuard      =  1.0 * mm; //Guardrail
  G4double PinGap        =  1.0 * mm; //between PINS
  G4double PinToEdge     =  3.0 * mm; //gap from the end
  G4double PinEpoxyT     =  1.6 * mm; //thickness of pcb board
  //G4double PinSupportLip =  PinT+PinEpoxyT; //thickness of pcb board

  //horizontal-side dimensions [approxiamted as a single board]
  G4double PinBoard_H_W = PinL*5.;
  G4double PinBoard_H_H = PinEpoxyT;
  G4double PinBoard_H_L = PinToEdge + PinL + PinGap + PinL + 13.0*mm;

  //vertical-side dimensions  [approximated as a single board]
  G4double PinBoard_V_H = PinL*2.;
  G4double PinBoard_V_W = PinEpoxyT;
  G4double PinBoard_V_L = PinToEdge + PinL + PinGap + PinL + 13.0*mm;

  //the PCB board for the PINS sits on top of another board [oh yeah transparency !]
  G4double PinBoardSupport_T = 4.75*mm; //nominally 4.75*mm; 

  //positions wrt cahmaber vacuum : put it at downstream side of the support block
  G4double PinBoard_z = DSSD_SupPos_z  + fCoolingBlockL/2 - PinBoard_H_L/2.;
  PinBoard_z -= 1.5*mm; //move back due to support

  G4double PinBoard_H_x  = 0.0 * mm;
  G4double PinBoard_H_dy = Hole4DSSD_H/2. - PinEpoxyT/2. - PinBoardSupport_T;
  G4double PinBoard_V_x  = Hole4DSSD_W/2. - PinEpoxyT/2. - PinBoardSupport_T;
  fDSSD_PosY             = fCentreOfDSSD_Y;
  G4double PinBoard_V_y  = fDSSD_PosY;

  //The epoxy board for the pins
  G4VisAttributes* visAttPIN_Board = new G4VisAttributes( G4Colour(0.3,0.9,0.3) );
  visAttPIN_Board->SetVisibility(true);
  //visAttPIN_Board->SetVisibility(false);
  visAttPIN_Board->SetForceWireframe(true);

  //Pin PCB/Epoxy boards
  G4Box* solidPinBoard_H = new G4Box("pin-pcb", PinBoard_H_W/2., PinBoard_H_H/2., PinBoard_H_L/2.);
  G4Box* solidPinBoard_V = new G4Box("pin-pcb", PinBoard_V_W/2., PinBoard_V_H/2., PinBoard_V_L/2.);


  G4LogicalVolume* logicPinBoard_H = new G4LogicalVolume(solidPinBoard_H, // it's solid
      boardMaterial,   // it's material
      "pin-pcb",       // it's name
      0,0,0);          // field manager etc

  G4LogicalVolume* logicPinBoard_V = new G4LogicalVolume(solidPinBoard_V, // it's solid
      boardMaterial,   // it's material
      "pin-pcb",       // it's name
      0,0,0);          // field manager etc

  logicPinBoard_H->SetVisAttributes(visAttPIN_Board);
  logicPinBoard_V->SetVisAttributes(visAttPIN_Board);


  //G4PVPlacement* physiPinBoard_H[2];
  //G4PVPlacement* physiPinBoard_V[2];  

  for(G4int i = 0; i < 2; i++) {

    G4double dyPos = (2*i-1)*PinBoard_H_dy;

    PVPBuffer = new G4PVPlacement(0,                     //rotation
        G4ThreeVector(PinBoard_H_x, fCentreOfDSSD_Y + dyPos, PinBoard_z),
        logicPinBoard_H, //its logical volume
        "PinPCB_H",           //its name
        logicChamVac,        //its mother
        false,                 //no boolean operat
        i+1);               //copy number

    PVPBuffer = new G4PVPlacement(0,                     //rotation
        G4ThreeVector( PinBoard_V_x*(2*i-1), PinBoard_V_y, PinBoard_z),
        logicPinBoard_V, //its logical volume
        "PinPCB_V",           //its name
        logicChamVac,        //its mother
        false,                 //no boolean operat
        i+1);               //copy number

  }



  //-----------------------------------------
  //Horiz Support for Pin Epoxy : the PCB board for the PINS sits on top of another board [oh yeah transparency !]
  G4VisAttributes* visAttPIN_BoardSup = new G4VisAttributes( G4Colour(0.9,0.9,0.9) );
  visAttPIN_BoardSup->SetVisibility(true);
  //visAttPIN_BoardSup->SetVisibility(false);
  visAttPIN_BoardSup->SetForceWireframe(true);

  G4double PinBoardSupport_H_W = fCoolingBlockCutOutW;
  G4double PinBoardSupport_H_H = 3.75*mm;
  G4double PinBoardSupport_H_L = 80.*mm;


  G4Box* solidPinSupportBoard_H_temp = new G4Box("pin-pcb-support", PinBoardSupport_H_W/2., PinBoardSupport_H_H/2., PinBoardSupport_H_L/2.);

  G4double cutOutPinBoard_H_x = 28.6*mm;
  G4Box*         cutOutPinBoard_H = new G4Box("Box Pin Board #1",cutOutPinBoard_H_x/2.,PinBoardSupport_H_H/2.,70.*mm/2.);

  G4ThreeVector  cutOutPinBoard_H_centre(0.0*mm,    0.0*mm, 5.0*mm);
  G4ThreeVector  cutOutPinBoard_H_p1( PinL,         0.0*mm, 5.0*mm);
  G4ThreeVector  cutOutPinBoard_H_m1(-PinL,         0.0*mm, 5.0*mm);
  G4ThreeVector  cutOutPinBoard_H_p2( 2*PinL,       0.0*mm, 5.0*mm);
  G4ThreeVector  cutOutPinBoard_H_m2(-2*PinL,       0.0*mm, 5.0*mm);

  G4SubtractionSolid* solidPinSupportBoard_H0 = 
    new G4SubtractionSolid("pin-pcb-support_0", solidPinSupportBoard_H_temp, cutOutPinBoard_H, 0, cutOutPinBoard_H_centre);

  G4SubtractionSolid* solidPinSupportBoard_HA = 
    new G4SubtractionSolid("pin-pcb-support_a", solidPinSupportBoard_H0,     cutOutPinBoard_H, 0, cutOutPinBoard_H_p1);

  G4SubtractionSolid* solidPinSupportBoard_HB = 
    new G4SubtractionSolid("pin-pcb-support_b", solidPinSupportBoard_HA,     cutOutPinBoard_H, 0, cutOutPinBoard_H_m1);

  G4SubtractionSolid* solidPinSupportBoard_HC = 
    new G4SubtractionSolid("pin-pcb-support_b", solidPinSupportBoard_HB,     cutOutPinBoard_H, 0, cutOutPinBoard_H_p2);

  G4SubtractionSolid* solidPinSupportBoard_H = 
    new G4SubtractionSolid("pin-pcb-support_b", solidPinSupportBoard_HC,     cutOutPinBoard_H, 0, cutOutPinBoard_H_m2);



  G4LogicalVolume* logicPinSupportBoard_H = new G4LogicalVolume(solidPinSupportBoard_H, // it's solid
      m_ChamberMaterial,   // it's material
      "pin-pcb-support",       // it's name
      0,0,0);          // field manager etc
  logicPinSupportBoard_H->SetVisAttributes(visAttPIN_BoardSup);

  G4double PinBoardSupport_Z = DSSD_SupPos_z + fCoolingBlockL/2. - PinBoardSupport_H_L/2. - 1.0*mm;



  PVPBuffer = new G4PVPlacement(0,                     //rotation
      G4ThreeVector(PinBoard_H_x, fDSSD_PosY + fCoolingBlockCutOutH/2. - PinBoardSupport_H_H/2., PinBoardSupport_Z),
      logicPinSupportBoard_H, //its logical volume
      "PinPCB_Support",           //its name
      logicChamVac,        //its mother
      false,                 //no boolean operat
      1);               //copy number

  PVPBuffer = new G4PVPlacement(0,                     //rotation
      G4ThreeVector(PinBoard_H_x, fDSSD_PosY - fCoolingBlockCutOutH/2. + PinBoardSupport_H_H/2., PinBoardSupport_Z),
      logicPinSupportBoard_H, //its logical volume
      "PinPCB_Support",           //its name
      logicChamVac,        //its mother
      false,                 //no boolean operat
      2);               //copy number



  G4double PinBoardSupport_W_W = 3.75*mm;
  G4double PinBoardSupport_W_H = fCoolingBlockCutOutH - 2.*PinBoardSupport_H_H;
  G4double PinBoardSupport_W_L = 80.*mm;

  //Vertical Support Pin Epoxy
  G4Box* solidPinSupportBoard_V_temp = new G4Box("pin-pcb-support", PinBoardSupport_W_W/2., PinBoardSupport_W_H/2., PinBoardSupport_W_L/2.);

  G4Box*         cutOutPinBoard_V = new G4Box("Box Pin Board #1",PinBoardSupport_W_W/2.,cutOutPinBoard_H_x/2.,70.*mm/2.);
  G4double cutOutSpine_x          = (PinL + PinGap - cutOutPinBoard_H_x)/2.;

  G4ThreeVector  cutOutPinBoard_V_p1( 0.0*mm,  (cutOutSpine_x+cutOutPinBoard_H_x)/2., 5.0*mm);
  G4ThreeVector  cutOutPinBoard_V_m1( 0.0*mm, -(cutOutSpine_x+cutOutPinBoard_H_x)/2., 5.0*mm);

  G4SubtractionSolid* solidPinSupportBoard_V0 = 
    new G4SubtractionSolid("pin-pcb-support_0", solidPinSupportBoard_V_temp, cutOutPinBoard_V, 0, cutOutPinBoard_V_p1);

  G4SubtractionSolid* solidPinSupportBoard_V = 
    new G4SubtractionSolid("pin-pcb-support_b", solidPinSupportBoard_V0,     cutOutPinBoard_V, 0, cutOutPinBoard_V_m1);



  G4LogicalVolume* logicPinSupportBoard_V = new G4LogicalVolume(solidPinSupportBoard_V, // it's solid
      m_ChamberMaterial,   // it's material
      "pin-pcb-support",       // it's name
      0,0,0);          // field manager etc

  logicPinSupportBoard_V->SetVisAttributes(visAttPIN_BoardSup);

  PVPBuffer = new G4PVPlacement(0,                     //rotation
      G4ThreeVector( fCoolingBlockCutOutW/2. - PinBoardSupport_W_W/2., fDSSD_PosY, PinBoardSupport_Z),
      logicPinSupportBoard_V, //its logical volume
      "PinPCBSupport",           //its name
      logicChamVac,        //its mother
      false,                 //no boolean operat
      1);               //copy number

  PVPBuffer= new G4PVPlacement(0,                     //rotation
      G4ThreeVector(-fCoolingBlockCutOutW/2. + PinBoardSupport_W_W/2., fDSSD_PosY, PinBoardSupport_Z),
      logicPinSupportBoard_V, //its logical volume
      "PinPCBSupport",           //its name
      logicChamVac,        //its mother
      false,                 //no boolean operat
      2);               //copy number


  //add the fucking ridiculous flange....
  G4double PinFlange_Z = DSSD_SupPos_z + fCoolingBlockL/2. - 0.5*mm;
  G4Box*  PinFlange_outer = new G4Box("Box Pin Flange #1",fCoolingBlockCutOutW/2.,fCoolingBlockCutOutH/2.,1.0*mm/2.);  //probably even thicker just to make sure no low E gammas are seen !
  G4Box*  PinFlange_hole  = new G4Box("Box Pin Flange #2",fCoolingBlockCutOutW/2.-4.0*mm,fCoolingBlockCutOutH/2.-4.0*mm,1.0*mm/2.);

  G4SubtractionSolid* solidPinFlange = 
    new G4SubtractionSolid("pin-flange", PinFlange_outer, PinFlange_hole, 0, G4ThreeVector(0.*mm, 0.*mm, 0.*mm));


  G4LogicalVolume* logicPinFlange = new G4LogicalVolume(solidPinFlange, // it's solid
      m_ChamberMaterial,   // it's material
      "pin-flange",       // it's name
      0,0,0);          // field manager etc

  logicPinFlange->SetVisAttributes(visAttPIN_BoardSup);

  PVPBuffer = new G4PVPlacement(0,                    //rotation
      G4ThreeVector(0.0*mm, fDSSD_PosY, PinFlange_Z),
      logicPinFlange,       //its logical volume
      "PinFlange",          //its name
      logicChamVac,         //its mother
      false,                 //no boolean operat
      1);               //copy number  

  //now the PINS.....
  G4VisAttributes* visAttPIN = new G4VisAttributes( G4Colour(0.9,0.9,0.3) );
  visAttPIN->SetVisibility(true);
  //visAttPIN->SetVisibility(false);
  visAttPIN->SetForceWireframe(true);

  G4double Pin_z1 = PinBoard_z + PinBoard_V_L/2. - PinToEdge - PinL/2;
  //G4double Pin_z2 = Pin_z1 - PinGap - PinL;

  //total Si area
  G4Box* solidPINS_H = new G4Box("pins-passive", PinL/2., PinT/2., PinL/2);  //horizontal
  new G4Box("pins-passive", PinT/2., PinL/2., PinL/2);  //vertical

  //active Si
  G4Box* solidPINS_Active_H;
  solidPINS_Active_H = new G4Box("pins", PinL/2.-PinGuard, PinT/2., PinL/2-PinGuard);  //horizontal
  new G4Box("pins", PinT/2., PinL/2.-PinGuard, PinL/2-PinGuard);  //vertical

  //horizontal rows  could put an index[i] = copyNo to match position in real array !!
  for(G4int nH = 0; nH < 2; nH++) {  //up/down
    G4double posY = fDSSD_PosY + (2*nH - 1)*(PinBoard_H_dy - PinBoard_H_H/2. - PinT/2.);

    for(G4int i = 1; i <= 5; i++) {  //across in x
      G4double posX = -PinBoard_H_W/2. + PinL/2. + (i-1)*PinL;

      for(G4int r = 1; r <= 2; r++) { //2 rows (in z)
        G4double posZ = Pin_z1 - (r%2)*(PinGap + PinL);
        G4int copyNo  = nH*10+2*(i-1)+r;
        G4cout << "Checking PIN copyNo " << copyNo << G4endl;

        logicPINS[copyNo-1] = new G4LogicalVolume(solidPINS_H, // it's solid
            eDetMat,     // it's material
            "pin-pass-h",     // it's name
            0,0,0);      // field manager etc

        logicPINS[copyNo-1]->SetVisAttributes(visAttPIN);


        PVPBuffer = new G4PVPlacement(0,                     //rotation
            G4ThreeVector(posX, posY, posZ),
            logicPINS[copyNo-1],   //its logical volume
            "PinPass",             //its name
            logicChamVac,          //its mother
            false,                 //no boolean operat
            copyNo,                //copy number
            true);                 //check for overlaps


        logicPINS_Active[copyNo-1] = new G4LogicalVolume(solidPINS_Active_H, // it's solid
            eDetMat,     // it's material
            "pin-h",     // it's name
            0,0,0);      // field manager etc

        logicPINS_Active[copyNo-1]->SetVisAttributes(visAttPIN);


        PVPBuffer = new G4PVPlacement(0,                          //rotation
            G4ThreeVector(0.*mm,0.*mm,0.*mm),
            logicPINS_Active[copyNo-1], //its logical volume
            "Pin",                      //its name
            logicPINS[copyNo-1],        //its mother
            false,                      //no boolean operat
            copyNo);                    //copy number    

      }
    }
  }



  //--------------------------------------------
  // The DSSD pre-amp boards
  //--------------------------------------------
  //dimensions of the main support board
  G4double PA_SupportBoard_T =   3.0*mm;
  G4double PA_SupportBoard_W =  67.0*mm;
  G4double PA_SupportBoard_L =  fCoolingBlockL;  //along z

  //dimensions of the individual pcb boards
  G4double PA_Board_T =   0.8*mm;
  G4double PA_Board_W =  14.0*mm;
  G4double PA_Board_L =  33.0*mm;

  //fudge a capacitor in there.... 
  G4double PA_Capacitor_T = 2.0*mm;
  G4double PA_Capacitor_R = 4.0*mm;

  //and the connectors...
  G4double connectorW = 26.0*mm;
  G4double connectorL = 14.0*mm;
  G4double connectorT =  5.0*mm;


  //delta-pos for each pre-amp
  const G4int    nPA        =  20; //per support board
  G4double PA_dPos = PA_SupportBoard_W / ((G4double) (nPA+1));

  //positions wrt chamaber vacuum : put it at downstream side of the support block
  G4double PA_SupportBoard_z = DSSD_SupPos_z  + fCoolingBlockL/2. - PA_SupportBoard_L/2.;

  G4Box*   solidPA_SupportBoard = new G4Box("dssd-pa-support-pcb", PA_SupportBoard_T/2., PA_SupportBoard_W/2., PA_SupportBoard_L/2.);
  G4Box*   solidPA_Board        = new G4Box("dssd-pa-pcb",         PA_Board_W/2.,        PA_Board_T/2.,        PA_Board_L/2.);
  G4Tubs*  solidPA_Capacitor    = new G4Tubs("dssd-pa-cap",        0.0*mm, PA_Capacitor_R, PA_Capacitor_T/2., 0.*degree, 360.*degree); 
  G4Box*   solidDSSD_Connector  =  new G4Box("dssd-connector",     connectorW/2.,        connectorT/2.,        connectorL/2.);


  G4VisAttributes* visAtt_PA_SupportBoard = new G4VisAttributes(G4Colour(0.2,0.9,0.2) );
  visAtt_PA_SupportBoard->SetVisibility(true);
  // visAtt_PA_SupportBoard->SetVisibility(false);
  visAtt_PA_SupportBoard->SetForceWireframe(true);

  G4VisAttributes* visAtt_PA_Board = new G4VisAttributes(G4Colour(0.1,0.9,0.1) );
  visAtt_PA_Board->SetVisibility(true);
  // visAtt_PA_Board->SetVisibility(false);
  visAtt_PA_Board->SetForceWireframe(true);

  //visualise the capacitors
  G4VisAttributes* visCapacitor = new G4VisAttributes(G4Colour(0.3,0.1,0.9) );
  visCapacitor->SetVisibility(true);
  //visCapacitor->SetVisibility(false);
  visCapacitor->SetForceWireframe(true);


  G4LogicalVolume* logicPA_SupportBoard = new G4LogicalVolume(solidPA_SupportBoard, // it's solid
      boardMaterial,          // it's material
      "dssd-pa-support-pcb",  // it's name
      0,0,0);                 // field manager etc


  G4LogicalVolume* logicDSSD_Connector = new G4LogicalVolume(solidDSSD_Connector, // it's solid
      boardMaterial,          // it's material
      "dssd-connector",  // it's name
      0,0,0);                 // field manager etc


  G4LogicalVolume* logicPA_Board = new G4LogicalVolume(solidPA_Board, // it's solid
      boardMaterial,   // it's material
      "dssd-pa-pcb",   // it's name
      0,0,0);          // field manager etc


  G4LogicalVolume* logicPA_Capacitor = new G4LogicalVolume(solidPA_Capacitor, // it's solid
      boardMaterial,   // it's material
      "dssd-pa-cap",   // it's name
      0,0,0);          // field manager etc

  logicPA_SupportBoard->SetVisAttributes(visAtt_PA_SupportBoard);
  logicDSSD_Connector->SetVisAttributes(visAtt_PA_SupportBoard);
  logicPA_Board->SetVisAttributes(visAtt_PA_Board);
  logicPA_Capacitor->SetVisAttributes(visCapacitor);


  //G4PVPlacement* physiPA_SupportBoard[6];
  //G4PVPlacement* physiDSSD_Connector[12];

  G4double top_of_DSSD_Brd = DSSD_PCB_Pos_y + DSSD_BrdH/2.;
  G4double gapConnector_2_DSSD_Board_V = 15.0*mm; //Gap between the top of the top connector and top of DSSD board = 15.0 mm
  G4double gapConnector_2_DSSD_Board_H = 15.0*mm; //Gap between the end of the centre connector to the centre of the DSSD board = 15.0 mm
  G4double gapConnector_PA_Board       =  4.0*mm;


  G4RotationMatrix* rmCap = new G4RotationMatrix;
  rmCap->set(0,0,0);
  rmCap->rotateX(90.*degree);

  G4RotationMatrix* rot_90_Z = new G4RotationMatrix;
  rot_90_Z->set(0,0,0);
  rot_90_Z->rotateZ(90.*degree);

  G4RotationMatrix* rot_90_Y = new G4RotationMatrix;
  rot_90_Y->set(0,0,0);
  rot_90_Y->rotateY(90.*degree);

  //vertical boards....  
  //G4PVPlacement* physiPA_Board[6][nPA];
  //G4PVPlacement* physiPA_Capacitor[6][nPA];

  for(G4int i = 0; i < 4; i++) {

    G4double xPos = fCoolingBlockW/2. + PA_SupportBoard_T/2. + 3.0*mm;
    G4double yPos = top_of_DSSD_Brd - PA_SupportBoard_W/2. - gapConnector_2_DSSD_Board_V + gapConnector_PA_Board;

    if(i >= 2)   xPos *= -1.;
    if(i%2 != 0) yPos -= (PA_SupportBoard_W + 7.0*mm);

    PVPBuffer =
      new G4PVPlacement(0,                         //rotation
          G4ThreeVector(xPos, yPos, PA_SupportBoard_z),
          logicPA_SupportBoard,     //its logical volume
          "dssd-pa-support-pcb",    //its name
          logicChamVac,             //its mother
          false,                    //no boolean operat
          i+1);                     //copy number


    xPos = DSSD_BrdW/2. - 9.0*mm;
    yPos = top_of_DSSD_Brd - gapConnector_2_DSSD_Board_V - connectorW/2.;

    switch (i) {
      case 0 :
        break;
      case 1 : yPos -=  (7.0*mm + connectorW);
      case 2 : yPos -= (15.0*mm + connectorW); 
      case 3 : yPos -=  (7.0*mm + connectorW);
               break;
    }


    PVPBuffer =
      new G4PVPlacement(rot_90_Z,                         //rotation
          G4ThreeVector(xPos, yPos, PA_SupportBoard_z+PA_SupportBoard_L/2.-connectorL/2.),
          logicDSSD_Connector,     //its logical volume
          "dssd-pa-support-pcb",    //its name
          logicChamVac,             //its mother
          false,                    //no boolean operat
          i+1);                     //copy number

    PVPBuffer =
      new G4PVPlacement(rot_90_Z,                         //rotation
          G4ThreeVector(-xPos, yPos, PA_SupportBoard_z+PA_SupportBoard_L/2.-connectorL/2.),
          logicDSSD_Connector,     //its logical volume
          "dssd-pa-support-pcb",    //its name
          logicChamVac,             //its mother
          false,                    //no boolean operat
          i+1+4);                     //copy number

    //place the preamp boards and capacitors
    G4double PA_PosY = top_of_DSSD_Brd - gapConnector_2_DSSD_Board_V + gapConnector_PA_Board;
    G4double PA_PosX = fCoolingBlockW/2.+PA_SupportBoard_T+3.0*mm+PA_Board_W/2.;

    if(i >= 2)   PA_PosX *= -1.;
    if(i%2 != 0) PA_PosY -= (PA_SupportBoard_W + 7.0*mm);

    for(G4int j = 0; j < nPA; j++) {
      G4double thisY = PA_PosY - PA_dPos*(j+1);
      PVPBuffer =
        new G4PVPlacement(0,                         //rotation
            G4ThreeVector(PA_PosX, thisY, PA_SupportBoard_z-10*mm),
            logicPA_Board,    //its logical volume
            "dssd-pa-pcb",     //its name
            logicChamVac,              //its mother
            false,                     //no boolean operat
            j+1);                        //copy number

      PVPBuffer =
        new G4PVPlacement(rmCap,                         //rotation
            G4ThreeVector(PA_PosX, thisY,  PA_SupportBoard_z+20.*mm),
            logicPA_Capacitor,    //its logical volume
            "dssd-pa-cap",     //its name
            logicChamVac,              //its mother
            false,                     //no boolean operat
            j+1);                        //copy number

    }
  }


  //horizontal...
  for(G4int i = 4; i < 6; i++) {

    G4double xPos = PA_SupportBoard_W/2.+gapConnector_2_DSSD_Board_H-gapConnector_PA_Board;
    G4double yPos = top_of_DSSD_Brd-13.*mm-connectorT-PA_SupportBoard_T/2.+ 1.0*mm;

    if(i == 5)   xPos *= -1.;

    PVPBuffer=
      new G4PVPlacement(rot_90_Z,                         //rotation
          G4ThreeVector(xPos, yPos, PA_SupportBoard_z),
          logicPA_SupportBoard,     //its logical volume
          "dssd-pa-support-pcb",    //its name
          logicChamVac,             //its mother
          false,                    //no boolean operat
          i+1);                     //copy number

    for(G4int j = 0; j < 2; j++) {
      PVPBuffer =
        new G4PVPlacement(0,                         //rotation
            G4ThreeVector(xPos+((2*j-1)*(connectorW/2.+3.5*mm)), yPos+PA_SupportBoard_T/2.+connectorT/2., PA_SupportBoard_z+PA_SupportBoard_L/2.-connectorL/2.),
            logicDSSD_Connector,     //its logical volume
            "dssd-pa-support-pcb",    //its name
            logicChamVac,             //its mother
            false,                    //no boolean operat
            1+4+2*i+j);                     //copy number
    }

    //place the preamp boards and capacitors
    G4double PA_PosY = yPos + PA_Board_W/2. + PA_SupportBoard_T/2.;
    G4double PA_PosX = xPos + PA_SupportBoard_W/2.;


    //if(i >= 2)   PA_PosX *= -1.;
    //if(i%2 != 0) PA_PosY -= (PA_SupportBoard_W + 7.0*mm);

    for(G4int j = 0; j < nPA; j++) {
      G4double thisX = PA_PosX - PA_dPos*(j+1);
      PVPBuffer =
        new G4PVPlacement(rot_90_Z,                         //rotation
            G4ThreeVector(thisX, PA_PosY, PA_SupportBoard_z-10*mm),
            logicPA_Board,    //its logical volume
            "dssd-pa-pcb",     //its name
            logicChamVac,              //its mother
            false,                     //no boolean operat
            j+1);                        //copy number

      PVPBuffer =
        new G4PVPlacement(rot_90_Y,                         //rotation
            G4ThreeVector(thisX, PA_PosY,  PA_SupportBoard_z+20.*mm),
            logicPA_Capacitor,    //its logical volume
            "dssd-pa-cap",     //its name
            logicChamVac,              //its mother
            false,                     //no boolean operat
            j+1);                        //copy number

    }
  }


  //================================================
  G4cout << "Placing the clovers......." << G4endl;

  //by default place 2 PhaseII clovers on either side of the chamber

  // uncomment this to put clovers on the sides !!
  //  Place_PhaseIIs_Left_and_Right(world);    //usual

  Place_GREAT_On_the_Top(world);           //usual

  //Place_GREAT_Left_and_Right();     //compare
  //Place_PhaseII_LookingUpstream();  //usual

  G4cout << ".....placed" << G4endl;



}



}

// Add Detector branch to the EventTree.
// Called After DetecorConstruction::AddDetector Method
void Chamber::InitializeRootOutput()
{}

// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAvtion
void Chamber::ReadSensitive(const G4Event*)
{}

//#################################################################################################################################################



//------------------------------------------------------------------
// Should add a ResetNumberOfCloversHere
//------------------------------------------------------------------

//------------------------------------------------------------------
// Should add a delta-Z so I can check effect of sliding the detectors up and down

//------------------------------------------------------------------
// Clover numbering scheme : 
//        Left     =  0 => Crystals  0, 1, 2, 3
//        Right    =  4 => Crystals  4, 5, 6, 7
//        Top      =  8 => Crystals  8, 9,10,11
//        Upstream = 12 => Crystals 12,13,14,15
//------------------------------------------------------------------
//------------------------------------------------------------------
void Chamber::Place_PhaseIIs_Left_and_Right(G4LogicalVolume* world)
{
  //=================================================================================
  //Do not know why, but the positioning seems to be with respect to the Taper-part :
  //setting the z-position as endCapTaperL/2 puts the front face at z = 0 mm
  //=================================================================================
  G4cout << "\t placing two Phase-II's" << G4endl;
  G4double endCap_PosX; 
  G4double vacuum_PosZ;
  G4double geLeaf_PosZ;

  //position is with respect to centre of tapered part (UnionSolid)
  endCap_PosX = fChamberW/2. + fEndCapTaperL_PhaseII/2. + 2.0*mm;
  vacuum_PosZ = fVacuumPosZ_PhaseII;
  geLeaf_PosZ = fGeLeafPosZ_PhaseII;


  G4RotationMatrix rm ;
  G4double theta = 90. * deg;
  G4double phi   =  0. * deg;


  //for(G4int det = prevNumber; det < numberOfClovers; det++) {
  for(G4int det = 0; det < 2; det++) {
    logicEndCap_CloverLR[det] = new G4LogicalVolume(solidEndCap_PhaseII, endCapMaterial,   "clover_EC",   0, 0, 0);
    logicVacuum_CloverLR[det] = new G4LogicalVolume(solidVacuum_PhaseII, vacuumMaterial,   "clover_Vac",  0, 0, 0);

    for(G4int l = 0; l < 4; l++) {
      logicGeLeaf_CloverLR[det][l]     = new G4LogicalVolume(solidGeLeaf_PhaseII,     geMaterial,      "clover_Leaf",   0, 0, 0);
      logicPassivated_CloverLR[det][l] = new G4LogicalVolume(solidPassivated_PhaseII, geMaterial,      "passivatedGe",  0, 0, 0); //should be Ge
      logicContact_CloverLR[det][l]    = new G4LogicalVolume(solidContact_PhaseII,    contactMaterial, "inner_contact", 0, 0, 0); //should be Li
      logicBoreHole_CloverLR[det][l]   = new G4LogicalVolume(solidBoreHole_PhaseII,   vacuumMaterial,  "bore-hole",     0, 0, 0);
    }


    //rotation for each clover.....
    rm.set(0,0,0);
    phi = 180.0 * deg * det;
    rm.rotateY(theta);
    rm.rotateX(phi);
    rm.invert();

    //traslate position: the centre of the DSSD has been placed @ Y = 0.0*mm
    G4ThreeVector translation(-endCap_PosX*sin(theta)*cos(phi),
        endCap_PosX*sin(theta)*sin(phi) + 0.0*mm, //fDSSD_PosY,
        geSidePosition);

    G4cout << "Clover " << det << " x " << -endCap_PosX*sin(theta)*cos(phi) << G4endl;

    //Physical placement of these solids......
    PVPBuffer = new G4PVPlacement(G4Transform3D(rm, translation),
        logicEndCap_CloverLR[det],//its logical volume
        "Clover_EC",       //its name
        world,         //its mother
        true,               //no boolean operat
        det*4,                  //copy number
        true);              //overlap check

    PVPBuffer = new G4PVPlacement(0,                   //rotation
        G4ThreeVector(0.*mm,0.*mm,vacuum_PosZ),
        logicVacuum_CloverLR[det], //its logical volume
        "Clover_Vac",       //its name
        logicEndCap_CloverLR[det], //its mother
        true,                //no boolean operat
        det*4,                   //copy number
        true);               //overlap check
  }

  //Now for the placement of the leaves in each clover......
  G4RotationMatrix* rmC;
  G4double leafX;
  G4double leafY;
  //G4double leafZ;

  //Keep track of which detectors are used
  //G4String  detName[2] = {"CloverR","CloverL"}; //Looking upstream

  for(G4int l = 0; l < 4; l++) {
    //the rotation
    rmC = new G4RotationMatrix;
    rmC->set(0,0,0);
    rmC->rotateZ(90.*degree*(4-l));
    rmC->invert();
    //the x-translation
    if(l < 2) {
      leafX = 22.15*mm;
    } else {
      leafX = -22.15*mm;
    }
    //the y-translation
    if(l == 0 || l == 3 ) {
      leafY = 22.15*mm;
    } else {
      leafY = -22.15*mm;
    }
    //the z-translation
    //leafZ = geLeaf_PosZ;


    //for(G4int det = prevNumber; det < numberOfClovers; det++) {
    for(G4int det = 0; det < 2; det++) {
      //physiGeLeaf_CloverLR[det][l] = new G4PVPlacement(rmC,                       //rotation
      PVPBuffer = new G4PVPlacement(rmC,                       //rotation
          G4ThreeVector(leafX,leafY,geLeaf_PosZ),
          logicGeLeaf_CloverLR[det][l], //its logical volume
          "Clover",//detName[det].data(),          //its name
          logicVacuum_CloverLR[det],    //its mother
          true,                       //no boolean operat
          det*4+l,                        //copy number
          true);                      //overlap check

      //physiPassivated_CloverLR[det][l] = new G4PVPlacement(0,                   //rotation
      PVPBuffer = new G4PVPlacement(0,                   //rotation
          G4ThreeVector(-fHole_dX_PhaseII, -fHole_dY_PhaseII, fContact_dZ_PhaseII),
          logicPassivated_CloverLR[det][l],
          "GePassivated",
          logicGeLeaf_CloverLR[det][l],
          false,det*4+l,true);

      //physiContact_CloverLR[det][l] = new G4PVPlacement(0,                   //rotation
      PVPBuffer = new G4PVPlacement(0,                   //rotation
          G4ThreeVector(0.*mm,0.*mm, 0.0*mm),//-fContact_dZ_PhaseII),
                logicContact_CloverLR[det][l],
                "LiContact",
                logicPassivated_CloverLR[det][l],
                false,det*4+l,true);

                //physiBoreHole_CloverLR[det][l] = new G4PVPlacement(0,                   //rotation
                PVPBuffer = new G4PVPlacement(0,                   //rotation
                    G4ThreeVector(0.*mm,0.*mm, 0.0*mm),//-fContact_dZ_PhaseII),
                          logicBoreHole_CloverLR[det][l],
                          "BoreHole",
                          logicContact_CloverLR[det][l],
                          false,det*4+l,true);

    }
  }

  //define the visual attributes
  G4VisAttributes* visAttAlCap = new G4VisAttributes( G4Colour(0.9,0.9,0.9) );
  visAttAlCap->SetForceWireframe(true);
  visAttAlCap->SetVisibility(true);
  //visAttAlCap->SetVisibility(false);

  G4VisAttributes* visAttGeVac = new G4VisAttributes( G4Colour(0.9,1.0,0.9) );
  visAttGeVac->SetForceWireframe(true);
  visAttGeVac->SetVisibility(true);
  //visAttGeVac->SetVisibility(false);

  G4VisAttributes* visAttActive = new G4VisAttributes( G4Colour(1.0,1.0,0.0) );
  visAttActive->SetForceWireframe(true);
  visAttActive->SetVisibility(true);
  //visAttActive->SetVisibility(false);

  G4VisAttributes* visAttPassive = new G4VisAttributes(G4Colour(0.0,1.0,1.0) );
  visAttPassive->SetForceWireframe(true);
  visAttPassive->SetVisibility(true);
  //visAttPassive->SetVisibility(false);

  G4VisAttributes* visAttLiContact = new G4VisAttributes(G4Colour(1.0,0.0,1.0) );
  visAttLiContact->SetVisibility(true);
  //visAttLiContact->SetVisibility(false);

  G4VisAttributes* visAttHole = new G4VisAttributes( G4Colour(0.0,0.0,1.0) );
  visAttHole->SetVisibility(true);
  //visAttHole->SetVisibility(false);

  for(G4int det = 0; det < 2; det++) {
    logicEndCap_CloverLR[det]->SetVisAttributes(visAttAlCap);
    logicVacuum_CloverLR[det]->SetVisAttributes(visAttGeVac);
    for(G4int l = 0; l < 4; l++) {
      logicGeLeaf_CloverLR[det][l]->SetVisAttributes(visAttActive);
      logicPassivated_CloverLR[det][l]->SetVisAttributes(visAttPassive);
      logicContact_CloverLR[det][l]->SetVisAttributes(visAttLiContact);
      logicBoreHole_CloverLR[det][l]->SetVisAttributes(visAttHole);
    }
  }
  }


  //------------------------------------------------------------------
  void Chamber::Place_PhaseII_On_the_Top(G4LogicalVolume* world)
  {
    //=================================================================================
    //Do not know why, but the positioning seems to be with respect to the Taper-part :
    //setting the z-position as endCapTaperL/2 puts the front face at z = 0 mm
    //=================================================================================
    G4double endCap_PosX; 
    G4double vacuum_PosZ;
    G4double geLeaf_PosZ;

    //reset scale
    endCap_PosX = fChamberH/2. + fEndCapTaperL_PhaseII/2. + fGeTopGap;
    vacuum_PosZ = fVacuumPosZ_PhaseII;
    geLeaf_PosZ = fGeLeafPosZ_PhaseII;


    G4RotationMatrix rm ;
    G4double theta = 90. * deg;
    G4double phi   =  0. * deg;


    logicEndCap_CloverT = new G4LogicalVolume(solidEndCap_PhaseII, endCapMaterial,   "clover_EC",   0, 0, 0);
    logicVacuum_CloverT = new G4LogicalVolume(solidVacuum_PhaseII, vacuumMaterial,   "clover_Vac",  0, 0, 0);

    for(G4int l = 0; l < 4; l++) {
      logicGeLeaf_CloverT[l]     = new G4LogicalVolume(solidGeLeaf_PhaseII,     geMaterial,      "clover_Leaf",   0, 0, 0);
      logicPassivated_CloverT[l] = new G4LogicalVolume(solidPassivated_PhaseII, geMaterial,      "passivatedGe",  0, 0, 0); //should be Ge
      logicContact_CloverT[l]    = new G4LogicalVolume(solidContact_PhaseII,    contactMaterial, "inner_contact", 0, 0, 0); //should be Li
      logicBoreHole_CloverT[l]   = new G4LogicalVolume(solidBoreHole_PhaseII,   vacuumMaterial,  "bore-hole",     0, 0, 0);
    }

    //rotate
    rm.set(0,0,0);
    phi = 90.0 * deg;
    rm.rotateY(theta);
    rm.rotateX(phi);
    rm.invert();

    //traslate position
    G4ThreeVector translation(-endCap_PosX*sin(theta)*cos(phi),
        endCap_PosX*sin(theta)*sin(phi),
        geTopPosition);

    G4cout << "Top Clover " << " x " << -endCap_PosX*sin(theta)*cos(phi) << G4endl;

    //Physical placement of these solids......
    //physiEndCap_CloverT = new G4PVPlacement(G4Transform3D(rm, translation),
    PVPBuffer = new G4PVPlacement(G4Transform3D(rm, translation),
        logicEndCap_CloverT,//its logical volume
        "Clover_EC",       //its name
        world,         //its mother
        true,               //no boolean operat
        8,                  //copy number
        true);              //overlap check

    //physiVacuum_CloverT = new G4PVPlacement(0,                   //rotation
    PVPBuffer = new G4PVPlacement(0,                   //rotation
        G4ThreeVector(0.*mm,0.*mm,vacuum_PosZ),
        logicVacuum_CloverT, //its logical volume
        "Clover_Vac",       //its name
        logicEndCap_CloverT, //its mother
        true,                //no boolean operat
        8,                   //copy number
        true);               //overlap check

    //Now for the placement of the leaves in each clover......
    G4RotationMatrix* rmC;
    G4double leafX;
    G4double leafY;
    //G4double leafZ;

    for(G4int l = 0; l < 4; l++) {
      //the rotation
      rmC = new G4RotationMatrix;
      rmC->set(0,0,0);
      rmC->rotateZ(90.*degree*(4-l));
      rmC->invert();
      //the x-translation
      if(l < 2) {
        leafX = 22.15*mm;
      } else {
        leafX = -22.15*mm;
      }
      //the y-translation
      if(l == 0 || l == 3 ) {
        leafY = 22.15*mm;
      } else {
        leafY = -22.15*mm;
      }
      //the z-translation
      //leafZ = geLeaf_PosZ;


      //physiGeLeaf_CloverT[l] = new G4PVPlacement(rmC,                       //rotation
      PVPBuffer = new G4PVPlacement(rmC,                       //rotation
          G4ThreeVector(leafX,leafY,geLeaf_PosZ),
          logicGeLeaf_CloverT[l],     //its logical volume
          "Clover",                 //its name
          logicVacuum_CloverT,        //its mother
          true,                       //no boolean operat
          8+l,                        //copy number
          true);                      //overlap check

      //physiPassivated_CloverT[l] = new G4PVPlacement(0,                   //rotation
      PVPBuffer = new G4PVPlacement(0,                   //rotation
          G4ThreeVector(-fHole_dX_PhaseII, -fHole_dY_PhaseII, fContact_dZ_PhaseII),
          logicPassivated_CloverT[l],
          "GePassivated",
          logicGeLeaf_CloverT[l],
          false,8+l,true);

      //physiContact_CloverT[l] = new G4PVPlacement(0,                   //rotation
      PVPBuffer = new G4PVPlacement(0,                   //rotation
          G4ThreeVector(0.*mm,0.*mm, 0.0*mm),//-fContact_dZ_PhaseII),
                logicContact_CloverT[l],
                "LiContact",
                logicPassivated_CloverT[l],
                false,8+l,true);

                //physiBoreHole_CloverT[l] = new G4PVPlacement(0,                   //rotation
                PVPBuffer = new G4PVPlacement(0,                   //rotation
                    G4ThreeVector(0.*mm,0.*mm, 0.0*mm),//-fContact_dZ_PhaseII),
                          logicBoreHole_CloverT[l],
                          "BoreHole",
                          logicContact_CloverT[l],
                          false,8+l,true);

    }

    //define the visual attributes
    G4VisAttributes* visAttAlCap = new G4VisAttributes( G4Colour(0.9,0.9,0.9) );
    visAttAlCap->SetVisibility(true);
    visAttAlCap->SetForceWireframe(true);

    G4VisAttributes* visAttGeVac = new G4VisAttributes( G4Colour(0.9,1.0,0.9) );
    visAttGeVac->SetForceWireframe(true);
    visAttGeVac->SetVisibility(true);

    G4VisAttributes* visAttActive = new G4VisAttributes( G4Colour(1.0,1.0,0.0) );
    visAttActive->SetForceWireframe(true);
    visAttActive->SetVisibility(true);

    G4VisAttributes* visAttPassive = new G4VisAttributes(G4Colour(0.0,1.0,1.0) );
    visAttPassive->SetForceWireframe(true);
    visAttPassive->SetVisibility(true);

    G4VisAttributes* visAttLiContact = new G4VisAttributes(G4Colour(1.0,0.0,1.0) );
    visAttLiContact->SetVisibility(true);

    G4VisAttributes* visAttHole = new G4VisAttributes( G4Colour(0.0,0.0,1.0) );
    visAttHole->SetVisibility(true);

    logicEndCap_CloverT->SetVisAttributes(visAttAlCap);
    logicVacuum_CloverT->SetVisAttributes(visAttGeVac);
    for(G4int l = 0; l < 4; l++) {
      logicGeLeaf_CloverT[l]->SetVisAttributes(visAttActive);
      logicPassivated_CloverT[l]->SetVisAttributes(visAttPassive);
      logicContact_CloverT[l]->SetVisAttributes(visAttLiContact);
      logicBoreHole_CloverT[l]->SetVisAttributes(visAttHole);
    }
  }


  //------------------------------------------------------------------
  void Chamber::Place_GREAT_On_the_Top(G4LogicalVolume* world)
  {
    //=================================================================================
    // The positioning for boolean solids is odd :  for this det it is with respect to the Taper-part :
    //setting the z-position as endCapTaperL/2 puts the front face at z = 0 mm
    //=================================================================================
    G4double endCap_PosX = fChamberH/2. - fShiftChamberY + fEndCapTaperL_GREAT/2. + fGeTopGap;
    G4double vacuum_PosZ = fVacuumPosZ_GREAT;
    G4double geLeaf_PosZ = fGeLeafPosZ_GREAT;


    G4RotationMatrix rm ;
    G4double theta = 90. * deg;
    G4double phi   =  0. * deg;


    logicEndCap_CloverT = new G4LogicalVolume(solidEndCap_GREAT, endCapMaterial,   "clover_EC",   0, 0, 0);
    logicVacuum_CloverT = new G4LogicalVolume(solidVacuum_GREAT, vacuumMaterial,   "clover_Vac",  0, 0, 0);

    for(G4int l = 0; l < 4; l++) {
      logicGeLeaf_CloverT[l]     = new G4LogicalVolume(solidGeLeaf_GREAT,     geMaterial,      "clover_Leaf",   0, 0, 0);
      logicPassivated_CloverT[l] = new G4LogicalVolume(solidPassivated_GREAT, geMaterial,      "passivatedGe",  0, 0, 0);
      logicContact_CloverT[l]    = new G4LogicalVolume(solidContact_GREAT,    contactMaterial, "inner_contact", 0, 0, 0);
      logicBoreHole_CloverT[l]   = new G4LogicalVolume(solidBoreHole_GREAT,   vacuumMaterial,  "bore_hole",     0, 0, 0);
    }

    //rotate
    rm.set(0,0,0);
    phi = 90.0 * deg;
    rm.rotateY(theta);
    rm.rotateX(phi);
    rm.invert();

    //traslate position
    G4ThreeVector translation(-endCap_PosX*sin(theta)*cos(phi),
        endCap_PosX*sin(theta)*sin(phi),
        geTopPosition +fCorrection);

    G4cout << "Top Clover " << " x " << -endCap_PosX*sin(theta)*cos(phi) << G4endl;

    //Physical placement of these solids......
    //physiEndCap_CloverT = new G4PVPlacement(G4Transform3D(rm, translation),
    PVPBuffer = new G4PVPlacement(G4Transform3D(rm, translation),
        logicEndCap_CloverT,//its logical volume
        "Clover_EC",       //its name
        world,         //its mother
        true,               //no boolean operat
        8,                  //copy number
        true);              //overlap check

    //physiVacuum_CloverT = new G4PVPlacement(0,                   //rotation
    PVPBuffer = new G4PVPlacement(0,                   //rotation
        G4ThreeVector(0.*mm,0.*mm,vacuum_PosZ),
        logicVacuum_CloverT, //its logical volume
        "Clover_Vac",       //its name
        logicEndCap_CloverT, //its mother
        true,                //no boolean operat
        8,                   //copy number
        true);               //overlap check

    //Now for the placement of the leaves in each clover......
    G4RotationMatrix* rmC;
    G4double dPos = fGeLeaf_dX_GREAT + fGapBetweenLeaves_GREAT/2.;
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


      //physiGeLeaf_CloverT[l] = new G4PVPlacement(rmC,                       //rotation
      PVPBuffer = new G4PVPlacement(rmC,                       //rotation
          G4ThreeVector(leafX, leafY, leafZ),
          logicGeLeaf_CloverT[l],     //its logical volume
          "Clover",                 //its name
          logicVacuum_CloverT,        //its mother
          true,                       //no boolean operat
          8+l,                        //copy number
          true);                      //overlap check

      //physiPassivated_CloverT[l] = new G4PVPlacement(0,                   //rotation
      PVPBuffer = new G4PVPlacement(0,                   //rotation
          G4ThreeVector(fHole_dX_GREAT, fHole_dY_GREAT, fContact_dZ_GREAT),
          logicPassivated_CloverT[l],
          "GePassivated",
          logicGeLeaf_CloverT[l],
          false,8+l,true);

      //physiContact_CloverT[l] = new G4PVPlacement(0,                   //rotation
      PVPBuffer = new G4PVPlacement(0,                   //rotation
          G4ThreeVector(0.*mm,0.*mm, 0.0*mm),//-fContact_dZ_GREAT),
                logicContact_CloverT[l],
                "LiContact",
                logicPassivated_CloverT[l],
                false,8+l,true);

                //physiBoreHole_CloverT[l] = new G4PVPlacement(0,                   //rotation
                PVPBuffer = new G4PVPlacement(0,                   //rotation
                    G4ThreeVector(0.*mm,0.*mm, 0.0*mm),
                    logicBoreHole_CloverT[l],
                    "BoreHole",
                    logicContact_CloverT[l],
                    false,8+l,true);

    }

    //define the visual attributes
    G4VisAttributes* visAttAlCap = new G4VisAttributes( G4Colour(0.9,0.9,0.9) );
    visAttAlCap->SetVisibility(true);
    //visAttAlCap->SetVisibility(false);
    visAttAlCap->SetForceWireframe(true);

    G4VisAttributes* visAttGeVac = new G4VisAttributes( G4Colour(0.9,1.0,0.9) );
    visAttGeVac->SetVisibility(true);
    //visAttGeVac->SetVisibility(false);
    visAttGeVac->SetForceWireframe(true);

    G4VisAttributes* visAttActive = new G4VisAttributes( G4Colour(1.0,1.0,0.0) );
    visAttActive->SetVisibility(true);
    //visAttActive->SetVisibility(false);
    visAttActive->SetForceWireframe(true);

    G4VisAttributes* visAttPassive = new G4VisAttributes(G4Colour(0.0,1.0,1.0) );
    visAttPassive->SetVisibility(true);
    //visAttPassive->SetVisibility(false);
    visAttPassive->SetForceWireframe(true);

    G4VisAttributes* visAttLiContact = new G4VisAttributes(G4Colour(1.0,0.0,1.0) );
    visAttLiContact->SetVisibility(true);
    //visAttLiContact->SetVisibility(false);

    G4VisAttributes* visAttHole = new G4VisAttributes( G4Colour(0.0,0.0,1.0) );
    visAttHole->SetVisibility(true);
    //visAttHole->SetVisibility(false);

    logicEndCap_CloverT->SetVisAttributes(visAttAlCap);
    logicVacuum_CloverT->SetVisAttributes(visAttGeVac);
    for(G4int l = 0; l < 4; l++) {
      logicGeLeaf_CloverT[l]->SetVisAttributes(visAttActive);
      logicPassivated_CloverT[l]->SetVisAttributes(visAttPassive);
      logicContact_CloverT[l]->SetVisAttributes(visAttLiContact);
      logicBoreHole_CloverT[l]->SetVisAttributes(visAttHole);
    }
  }

  //------------------------------------------------------------------
  // Clover numbering scheme : 
  //        Left     =  0 => Crystals  0, 1, 2, 3
  //        Right    =  4 => Crystals  4, 5, 6, 7
  //        Top      =  8 => Crystals  8, 9,10,11
  //        Upstream = 12 => Crystals 12,13,14,15
  //------------------------------------------------------------------
  //------------------------------------------------------------------
  void Chamber::Place_GREAT_Left_and_Right(G4LogicalVolume* world)
  {
    //=================================================================================
    // The positioning for boolean solids is odd :  for this det it is with respect to the Taper-part :
    //setting the z-position as endCapTaperL/2 puts the front face at z = 0 mm
    //=================================================================================
    G4double endCap_PosX = fChamberW/2. + fEndCapTaperL_GREAT/2. + 3.0*mm;
    G4double vacuum_PosZ = fVacuumPosZ_GREAT;
    G4double geLeaf_PosZ = fGeLeafPosZ_GREAT;


    G4RotationMatrix rm ;
    G4double theta = 90. * deg;
    G4double phi   =  0. * deg;


    //for(G4int det = prevNumber; det < numberOfClovers; det++) {
    for(G4int det = 0; det < 2; det++) {
      logicEndCap_CloverLR[det] = new G4LogicalVolume(solidEndCap_GREAT, endCapMaterial,   "clover_EC",   0, 0, 0);
      logicVacuum_CloverLR[det] = new G4LogicalVolume(solidVacuum_GREAT, vacuumMaterial,   "clover_Vac",  0, 0, 0);

      for(G4int l = 0; l < 4; l++) {
        logicGeLeaf_CloverLR[det][l]     = new G4LogicalVolume(solidGeLeaf_GREAT,     geMaterial,      "clover_Leaf",   0, 0, 0);
        logicPassivated_CloverLR[det][l] = new G4LogicalVolume(solidPassivated_GREAT,    geMaterial,      "passivatedGe",  0, 0, 0); //should be Ge
        logicContact_CloverLR[det][l]    = new G4LogicalVolume(solidContact_GREAT, contactMaterial, "inner_contact", 0, 0, 0); //should be Li
        logicBoreHole_CloverLR[det][l]   = new G4LogicalVolume(solidBoreHole_GREAT,   vacuumMaterial,  "bore-hole",     0, 0, 0);
      }


      //rotation for each clover.....
      rm.set(0,0,0);
      phi = 180.0 * deg * det;
      rm.rotateY(theta);
      rm.rotateX(phi);
      rm.invert();

      //traslate position: the centre of the DSSD has been placed @ Y = 0.0*mm
      G4ThreeVector translation(-endCap_PosX*sin(theta)*cos(phi),
          endCap_PosX*sin(theta)*sin(phi) + 0.0*mm, //fDSSD_PosY,
          0.0*mm);

      G4cout << "Clover " << det << " x " << -endCap_PosX*sin(theta)*cos(phi) << G4endl;

      //Physical placement of these solids......
      //physiEndCap_CloverLR[det] = new G4PVPlacement(G4Transform3D(rm, translation),
      PVPBuffer = new G4PVPlacement(G4Transform3D(rm, translation),
          logicEndCap_CloverLR[det],//its logical volume
          "Clover_EC",       //its name
          world,         //its mother
          true,               //no boolean operat
          det*4,                  //copy number
          true);              //overlap check

      //physiVacuum_CloverLR[det] = new G4PVPlacement(0,                   //rotation
      PVPBuffer = new G4PVPlacement(0,                   //rotation
          G4ThreeVector(0.*mm,0.*mm,vacuum_PosZ),
          logicVacuum_CloverLR[det], //its logical volume
          "Clover_Vac",       //its name
          logicEndCap_CloverLR[det], //its mother
          true,                //no boolean operat
          det*4,                   //copy number
          true);               //overlap check
    }

    //Now for the placement of the leaves in each clover......
    G4RotationMatrix* rmC;
    G4double dPos = fGeLeaf_dX_GREAT + fGapBetweenLeaves_GREAT/2.;
    G4double leafX;
    G4double leafY;
    //G4double leafZ;

    //Keep track of which detectors are used
    //G4String  detName[2] = {"CloverR","CloverL"}; //Looking upstream

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
      //leafZ = geLeaf_PosZ;


      //for(G4int det = prevNumber; det < numberOfClovers; det++) {
      for(G4int det = 0; det < 2; det++) {
        //physiGeLeaf_CloverLR[det][l] = new G4PVPlacement(rmC,                       //rotation
        PVPBuffer = new G4PVPlacement(rmC,                       //rotation
            G4ThreeVector(leafX,leafY,geLeaf_PosZ),
            logicGeLeaf_CloverLR[det][l], //its logical volume
            "Clover",//detName[det].data(),          //its name
            logicVacuum_CloverLR[det],    //its mother
            true,                       //no boolean operat
            det*4+l,                        //copy number
            true);                      //overlap check

        //physiPassivated_CloverLR[det][l] = new G4PVPlacement(0,                   //rotation
        PVPBuffer = new G4PVPlacement(0,                   //rotation
            G4ThreeVector(-fHole_dX_PhaseII, -fHole_dY_PhaseII, fContact_dZ_GREAT),
            logicPassivated_CloverLR[det][l],
            "GePassivated",
            logicGeLeaf_CloverLR[det][l],
            false,det*4+l,true);

        //physiContact_CloverLR[det][l] = new G4PVPlacement(0,                   //rotation
        PVPBuffer = new G4PVPlacement(0,                   //rotation
            G4ThreeVector(0.*mm,0.*mm, 0.0*mm),//-fContact_dZ_GREAT),
                  logicContact_CloverLR[det][l],
                  "LiContact",
                  logicPassivated_CloverLR[det][l],
                  false,det*4+l,true);

                  //physiBoreHole_CloverLR[det][l] = new G4PVPlacement(0,                   //rotation
                  PVPBuffer = new G4PVPlacement(0,                   //rotation
                      G4ThreeVector(0.*mm,0.*mm, 0.0*mm),//-fContact_dZ_GREAT),
                            logicBoreHole_CloverLR[det][l],
                            "BoreHole",
                            logicContact_CloverLR[det][l],
                            false,det*4+l,true);

      }
    }

    //define the visual attributes
    G4VisAttributes* visAttAlCap = new G4VisAttributes( G4Colour(0.9,0.9,0.9) );
    visAttAlCap->SetVisibility(true);
    visAttAlCap->SetForceWireframe(true);

    G4VisAttributes* visAttGeVac = new G4VisAttributes( G4Colour(0.9,1.0,0.9) );
    visAttGeVac->SetForceWireframe(true);
    visAttGeVac->SetVisibility(true);

    G4VisAttributes* visAttActive = new G4VisAttributes( G4Colour(1.0,1.0,0.0) );
    visAttActive->SetForceWireframe(true);
    visAttActive->SetVisibility(true);

    G4VisAttributes* visAttPassive = new G4VisAttributes(G4Colour(0.0,1.0,1.0) );
    visAttPassive->SetForceWireframe(true);
    visAttPassive->SetVisibility(true);

    G4VisAttributes* visAttLiContact = new G4VisAttributes(G4Colour(1.0,0.0,1.0) );
    visAttLiContact->SetVisibility(true);

    G4VisAttributes* visAttHole = new G4VisAttributes( G4Colour(0.0,0.0,1.0) );
    visAttHole->SetVisibility(true);

    for(G4int det = 0; det < 2; det++) {
      logicEndCap_CloverLR[det]->SetVisAttributes(visAttAlCap);
      logicVacuum_CloverLR[det]->SetVisAttributes(visAttGeVac);
      for(G4int l = 0; l < 4; l++) {
        logicGeLeaf_CloverLR[det][l]->SetVisAttributes(visAttActive);
        logicPassivated_CloverLR[det][l]->SetVisAttributes(visAttPassive);
        logicContact_CloverLR[det][l]->SetVisAttributes(visAttLiContact);
        logicBoreHole_CloverLR[det][l]->SetVisAttributes(visAttHole);
      }
    }
    }



    //------------------------------------------------------------------
    void Chamber::Place_PhaseII_LookingUpstream(G4LogicalVolume* world)
    {
      //=================================================================================
      // The positioning for boolean solids is odd :  for this det it is with respect to the Taper-part :
      //setting the z-position as endCapTaperL/2 puts the front face at z = 0 mm
      //=================================================================================
      G4double endCap_PosZ = fEndCapTaperL_PhaseII/2. + 3.0*mm;
      G4double vacuum_PosZ = fVacuumPosZ_PhaseII;
      G4double geLeaf_PosZ = fGeLeafPosZ_PhaseII;

      logicEndCap_CloverU = new G4LogicalVolume(solidEndCap_PhaseII, endCapMaterial,   "clover_EC",   0, 0, 0);
      logicVacuum_CloverU = new G4LogicalVolume(solidVacuum_PhaseII, vacuumMaterial,   "clover_Vac",  0, 0, 0);

      for(G4int l = 0; l < 4; l++) {
        logicGeLeaf_CloverU[l]     = new G4LogicalVolume(solidGeLeaf_PhaseII,     geMaterial,      "clover_Leaf",   0, 0, 0);
        logicPassivated_CloverU[l] = new G4LogicalVolume(solidPassivated_PhaseII, geMaterial,      "passivatedGe",  0, 0, 0);
        logicContact_CloverU[l]    = new G4LogicalVolume(solidContact_PhaseII,    contactMaterial, "inner_contact", 0, 0, 0);
        logicBoreHole_CloverU[l]   = new G4LogicalVolume(solidBoreHole_PhaseII,   vacuumMaterial,  "bore_hole",     0, 0, 0);
      }

      //Physical placement of these solids......
      //traslate position: the centre of the DSSD has been placed @ Y = 0.0*mm
      //physiEndCap_CloverU = new G4PVPlacement(0, //no rotation
      PVPBuffer = new G4PVPlacement(0, //no rotation
          G4ThreeVector(0.*mm, 0.0*mm, endCap_PosZ),
          logicEndCap_CloverU,//its logical volume
          "Clover_EC",       //its name
          world,         //its mother
          true,               //no boolean operat
          12,                  //copy number
          true);              //overlap check

      //physiVacuum_CloverU = new G4PVPlacement(0,                   //rotation
      PVPBuffer = new G4PVPlacement(0,                   //rotation
          G4ThreeVector(0.*mm,0.*mm,vacuum_PosZ),
          logicVacuum_CloverU, //its logical volume
          "Clover_Vac",       //its name
          logicEndCap_CloverU, //its mother
          true,                //no boolean operat
          12,                   //copy number
          true);               //overlap check

      //Now for the placement of the leaves in each clover......
      G4RotationMatrix* rmC;
      G4double dPos = fGeLeaf_dX_PhaseII + fGapBetweenLeaves_PhaseII/2.;
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


        //physiGeLeaf_CloverU[l] = new G4PVPlacement(rmC,                       //rotation
        PVPBuffer = new G4PVPlacement(rmC,                       //rotation
            G4ThreeVector(leafX, leafY, leafZ),
            logicGeLeaf_CloverU[l],     //its logical volume
            "Clover",                 //its name
            logicVacuum_CloverU,        //its mother
            true,                       //no boolean operat
            12+l,                        //copy number
            true);                      //overlap check

        //physiPassivated_CloverU[l] = new G4PVPlacement(0,                   //rotation
        PVPBuffer = new G4PVPlacement(0,                   //rotation
            G4ThreeVector(-fHole_dX_PhaseII, -fHole_dY_PhaseII, fContact_dZ_PhaseII),
            logicPassivated_CloverU[l],
            "GePassivated",
            logicGeLeaf_CloverU[l],
            false,12+l,true);

        //physiContact_CloverU[l] = new G4PVPlacement(0,                   //rotation
        PVPBuffer = new G4PVPlacement(0,                   //rotation
            G4ThreeVector(0.*mm,0.*mm, 0.0*mm),//-fContact_dZ_GREAT),
                  logicContact_CloverU[l],
                  "LiContact",
                  logicPassivated_CloverU[l],
                  false,12+l,true);

                  //physiBoreHole_CloverU[l] = new G4PVPlacement(0,                   //rotation
                  PVPBuffer = new G4PVPlacement(0,                   //rotation
                      G4ThreeVector(0.*mm,0.*mm, 0.0*mm),
                      logicBoreHole_CloverU[l],
                      "BoreHole",
                      logicContact_CloverU[l],
                      false,12+l,true);

      }

      //define the visual attributes
      G4VisAttributes* visAttAlCap = new G4VisAttributes( G4Colour(0.9,0.9,0.9) );
      visAttAlCap->SetVisibility(true);
      visAttAlCap->SetForceWireframe(true);

      G4VisAttributes* visAttGeVac = new G4VisAttributes( G4Colour(0.9,1.0,0.9) );
      visAttGeVac->SetForceWireframe(true);
      visAttGeVac->SetVisibility(true);

      G4VisAttributes* visAttActive = new G4VisAttributes( G4Colour(1.0,1.0,0.0) );
      visAttActive->SetForceWireframe(true);
      visAttActive->SetVisibility(true);

      G4VisAttributes* visAttPassive = new G4VisAttributes(G4Colour(0.0,1.0,1.0) );
      visAttPassive->SetForceWireframe(true);
      visAttPassive->SetVisibility(true);

      G4VisAttributes* visAttLiContact = new G4VisAttributes(G4Colour(1.0,0.0,1.0) );
      visAttLiContact->SetVisibility(true);

      G4VisAttributes* visAttHole = new G4VisAttributes( G4Colour(0.0,0.0,1.0) );
      visAttHole->SetVisibility(true);

      logicEndCap_CloverU->SetVisAttributes(visAttAlCap);
      logicVacuum_CloverU->SetVisAttributes(visAttGeVac);
      for(G4int l = 0; l < 4; l++) {
        logicGeLeaf_CloverU[l]->SetVisAttributes(visAttActive);
        logicPassivated_CloverU[l]->SetVisAttributes(visAttPassive);
        logicContact_CloverU[l]->SetVisAttributes(visAttLiContact);
        logicBoreHole_CloverU[l]->SetVisAttributes(visAttHole);
      }
    }

    //------------------------------------------------------------------
    // This is just as a comparison with the current position
    //------------------------------------------------------------------
    void Chamber::Place_GREAT_LookingUpstream(G4LogicalVolume* world)
    {
      //=================================================================================
      // The positioning for boolean solids is odd :  for this det it is with respect to the Taper-part :
      //setting the z-position as endCapTaperL/2 puts the front face at z = 0 mm
      //=================================================================================
      G4double endCap_PosZ = fEndCapTaperL_GREAT/2. + 3.0*mm;
      G4double vacuum_PosZ = fVacuumPosZ_GREAT;
      G4double geLeaf_PosZ = fGeLeafPosZ_GREAT;

      logicEndCap_CloverU = new G4LogicalVolume(solidEndCap_GREAT, endCapMaterial,   "clover_EC",   0, 0, 0);
      logicVacuum_CloverU = new G4LogicalVolume(solidVacuum_GREAT, vacuumMaterial,   "clover_Vac",  0, 0, 0);

      for(G4int l = 0; l < 4; l++) {
        logicGeLeaf_CloverU[l]     = new G4LogicalVolume(solidGeLeaf_GREAT,     geMaterial,      "clover_Leaf",   0, 0, 0);
        logicPassivated_CloverU[l] = new G4LogicalVolume(solidPassivated_GREAT, geMaterial,      "passivatedGe",  0, 0, 0);
        logicContact_CloverU[l]    = new G4LogicalVolume(solidContact_GREAT,    contactMaterial, "inner_contact", 0, 0, 0);
        logicBoreHole_CloverU[l]   = new G4LogicalVolume(solidBoreHole_GREAT,   vacuumMaterial,  "bore_hole",     0, 0, 0);
      }

      //Physical placement of these solids......
      //traslate position: the centre of the DSSD has been placed @ Y = 0.0*mm
      //physiEndCap_CloverU = new G4PVPlacement(0, //no rotation
      PVPBuffer = new G4PVPlacement(0, //no rotation
          G4ThreeVector(0.*mm, 0.0*mm, endCap_PosZ),
          logicEndCap_CloverU,//its logical volume
          "Clover_EC",       //its name
          world,         //its mother
          true,               //no boolean operat
          12,                  //copy number
          true);              //overlap check

      //physiVacuum_CloverU = new G4PVPlacement(0,                   //rotation
      PVPBuffer = new G4PVPlacement(0,                   //rotation
          G4ThreeVector(0.*mm,0.*mm,vacuum_PosZ),
          logicVacuum_CloverU, //its logical volume
          "Clover_Vac",       //its name
          logicEndCap_CloverU, //its mother
          true,                //no boolean operat
          12,                   //copy number
          true);               //overlap check

      //Now for the placement of the leaves in each clover......
      G4RotationMatrix* rmC;
      G4double dPos = fGeLeaf_dX_GREAT + fGapBetweenLeaves_GREAT/2.;
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


        //physiGeLeaf_CloverU[l] = new G4PVPlacement(rmC,                       //rotation
        PVPBuffer = new G4PVPlacement(rmC,                       //rotation
            G4ThreeVector(leafX, leafY, leafZ),
            logicGeLeaf_CloverU[l],     //its logical volume
            "Clover",                 //its name
            logicVacuum_CloverU,        //its mother
            true,                       //no boolean operat
            12+l,                        //copy number
            true);                      //overlap check

        //physiPassivated_CloverU[l] = new G4PVPlacement(0,                   //rotation
        PVPBuffer = new G4PVPlacement(0,                   //rotation
            G4ThreeVector(fHole_dX_GREAT, fHole_dY_GREAT, fContact_dZ_GREAT),
            logicPassivated_CloverU[l],
            "GePassivated",
            logicGeLeaf_CloverU[l],
            false,12+l,true);

        //physiContact_CloverU[l] = new G4PVPlacement(0,                   //rotation
        PVPBuffer = new G4PVPlacement(0,                   //rotation
            G4ThreeVector(0.*mm,0.*mm, 0.0*mm),//-fContact_dZ_GREAT),
                  logicContact_CloverU[l],
                  "LiContact",
                  logicPassivated_CloverU[l],
                  false,12+l,true);

                  //physiBoreHole_CloverU[l] = new G4PVPlacement(0,                   //rotation
                  PVPBuffer = new G4PVPlacement(0,                   //rotation
                      G4ThreeVector(0.*mm,0.*mm, 0.0*mm),
                      logicBoreHole_CloverU[l],
                      "BoreHole",
                      logicContact_CloverU[l],
                      false,12+l,true);

      }

      //define the visual attributes
      G4VisAttributes* visAttAlCap = new G4VisAttributes( G4Colour(0.9,0.9,0.9) );
      visAttAlCap->SetVisibility(true);
      visAttAlCap->SetForceWireframe(true);

      G4VisAttributes* visAttGeVac = new G4VisAttributes( G4Colour(0.9,1.0,0.9) );
      visAttGeVac->SetForceWireframe(true);
      visAttGeVac->SetVisibility(true);

      G4VisAttributes* visAttActive = new G4VisAttributes( G4Colour(1.0,1.0,0.0) );
      visAttActive->SetForceWireframe(true);
      visAttActive->SetVisibility(true);

      G4VisAttributes* visAttPassive = new G4VisAttributes(G4Colour(0.0,1.0,1.0) );
      visAttPassive->SetForceWireframe(true);
      visAttPassive->SetVisibility(true);

      G4VisAttributes* visAttLiContact = new G4VisAttributes(G4Colour(1.0,0.0,1.0) );
      visAttLiContact->SetVisibility(true);

      G4VisAttributes* visAttHole = new G4VisAttributes( G4Colour(0.0,0.0,1.0) );
      visAttHole->SetVisibility(true);

      logicEndCap_CloverU->SetVisAttributes(visAttAlCap);
      logicVacuum_CloverU->SetVisAttributes(visAttGeVac);
      for(G4int l = 0; l < 4; l++) {
        logicGeLeaf_CloverU[l]->SetVisAttributes(visAttActive);
        logicPassivated_CloverU[l]->SetVisAttributes(visAttPassive);
        logicContact_CloverU[l]->SetVisAttributes(visAttLiContact);
        logicBoreHole_CloverU[l]->SetVisAttributes(visAttHole);
      }
    }

    //---------------------------------------------------------------------
    // Create the solids defining Phase-II Clovers
    //---------------------------------------------------------------------
    void  Chamber::CreateCloverIISolids()
    {
      //An approximate CloverII
      G4cout << G4endl << "Constructing archetypal PhaseII Clover" << G4endl;

      //---------------------------------------------------------
      //end-cap
      G4double endCapFrontThickness = 1.2*mm; //was 1.5
      G4double endCapTaperThickness = 1.5*mm;
      G4double endCapSideThickness  = 1.5*mm;

      G4double GeGap      =  fEndCap2Ge_PhaseII;
      G4double taperAngle =  7.0*degree;

      G4double endCapTotalL = fTotalGeL_PhaseII + GeGap + endCapFrontThickness + 5.*mm; //+ Gap at rear end
      G4double endCapFrontD = 43.5*mm;
      G4double endCapBackD  = 50.5*mm;
      G4double endCapTaperL = 55.0*mm;

      G4double endCapBoxL   = endCapTotalL - endCapTaperL;

      //the tapered part
      G4Trap* solidTaperedCloverEC
        = new G4Trap("taperedCloverEC",
            endCapTaperL/2.,    //Half z-length [pDz]
            0.00*degree, //Polar angle of line joining centres of the faces @ -/+pDz
            14.0*degree,   //aequivalent zimuthal angle 
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
      solidEndCap_PhaseII = new G4UnionSolid("Box+Taper",solidTaperedCloverEC,endCapBox,0,transECBox);
      //need the taperL for placement
      fEndCapTaperL_PhaseII = endCapTaperL;


      //---------------------------------------------------------
      //end-cap inner vacuum
      G4double endCapDelta_1 = endCapTaperThickness/cos(taperAngle) - endCapFrontThickness*tan(taperAngle);
      G4double endCapDelta_2 = ( endCapSideThickness - (endCapTaperThickness*sin(taperAngle)*tan(taperAngle) +
            endCapTaperThickness*cos(taperAngle) ) )/tan(taperAngle);

      G4cout << endCapDelta_1 << " " << endCapDelta_2 << endl;

      G4double endCapVacTaperL = endCapTaperL - endCapFrontThickness;// - endCapDelta_2;
      G4double endCapVacBoxL   = endCapBoxL   - endCapFrontThickness;
      G4double endCapVacTotalL = endCapVacBoxL + endCapVacTaperL;
      G4double endCapVacFrontD = endCapFrontD - endCapDelta_1;
      G4double endCapVacBackD  = endCapBackD  - endCapSideThickness;

      //position of vacuum wrt end-cap
      fVacuumPosZ_PhaseII = (-endCapTotalL + endCapVacTotalL )/2. + 1.5*endCapFrontThickness;

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

      G4cout << endCapTotalL << " " << endCapVacTotalL << endl;

      //rectangular part
      G4Box*         endCapVacBox  = new G4Box("endCapBox",endCapVacBackD,endCapVacBackD,endCapVacBoxL/2.);
      G4ThreeVector transVacBox(   0.*mm, 0.*mm, (endCapVacTaperL/2.+endCapVacBoxL/2.-0.0001*mm));

      //add them together
      solidVacuum_PhaseII = new G4UnionSolid("Vac_Box+Taper",solidTaperVac,endCapVacBox,0,transVacBox);


      //---------------------------------------------------------
      //The Ge crystal...
      G4double GeTaperL    = 36.0*mm;
      G4double GeTotalL    = fTotalGeL_PhaseII; //70.0 * mm;
      G4double smallSquare = 41.0*mm; 
      G4double largeSquare = 45.5*mm;

      G4double transX = (largeSquare-smallSquare)/2.;
      G4double transY = (largeSquare-smallSquare)/2.;
      fHole_dX_PhaseII = transX;  //transX /= 2.;
      fHole_dY_PhaseII = transY;  //transY /= 2.;

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
      zPlaneGe[1] =  2.06*mm;
      zPlaneGe[2] =  5.00*mm;
      zPlaneGe[3] = GeTaperL;

      G4double rInnerGe[numZPlanesGe]; // interior radii
      rInnerGe[0] = rInnerGe[1] = rInnerGe[2] = rInnerGe[3] = 0.0*mm;
      G4double rOuterGe[numZPlanesGe]; // exterior radii
      rOuterGe[0] = 20.5*mm;  rOuterGe[1] = 23.54*mm;
      rOuterGe[2] = rOuterGe[3] = fCrystalR_PhaseII;


      G4Polycone* solidCone = new G4Polycone("cloverCone", 0.0*degree, 360.0*degree,
          numZPlanesGe,
          zPlaneGe,
          rInnerGe,
          rOuterGe);

      G4ThreeVector  transGeCone( -transX/2., -transY/2., -GeTaperL/2.);
      G4IntersectionSolid* taperedCone = new G4IntersectionSolid("Taper+Cone",solidTaper,solidCone,0,transGeCone);

      //back part....
      G4double geBoxL = fTotalGeL_PhaseII - GeTaperL;

      G4Box*    GeBox = new G4Box("GeBox",largeSquare/2.,largeSquare/2.,geBoxL/2.);
      G4Tubs*   GeCyl = new G4Tubs("GeCyl",0.0*mm,fCrystalR_PhaseII,geBoxL/2.,0.*degree,360.*degree); 

      G4ThreeVector transGeBox( transX, transY, 0.0*mm);          
      G4IntersectionSolid* backPart = new G4IntersectionSolid("Box+Cyl",GeCyl,GeBox,0,transGeBox);

      //add front and back
      G4ThreeVector transBack( -transX/2., -transY/2., (GeTaperL/2.+geBoxL/2.));
      solidGeLeaf_PhaseII = new G4UnionSolid("germanium",taperedCone,backPart,0,transBack);

      //z-position of Ge-leaf wrt vacuum
      fGeLeafPosZ_PhaseII = -endCapVacTaperL/2. + GeTaperL/2. + GeGap - endCapFrontThickness;

      G4cout << "end-cap : box/2 " << endCapBoxL/2. << " taper/2 " << endCapTaperL/2. << " total/2 " << endCapTotalL << G4endl;
      G4cout << "vacuum  : box/2 " << endCapVacBoxL/2. << " taper/2 " << endCapVacTaperL/2. << " total/2 " << endCapVacTotalL << G4endl;
      G4cout << "ge      : box/2 " << geBoxL/2. << " taper/2 " << GeTaperL/2. << " total/2 " << GeTotalL << G4endl;


      //------------------------------------------------------------------
      // Inner bore hole + lithium contact + passivated Ge
      G4double GeDepth      = 15.00 * mm;  //Hole dirilled to this far from face
      G4double passiveThick = 0.5 * mm;    //fPassiveThick_PhaseII;  //passivated Ge
      G4double contactThick = fContactThick_PhaseII;  //Li contact
      //G4double innerRHole =  0.00 * mm;
      G4double holeR      = fHoleR_PhaseII;
      G4double contactR   = holeR + contactThick;
      G4double passiveR   = contactR + passiveThick;
      G4double holeL      = fTotalGeL_PhaseII - GeDepth;
      G4double tubeL      = holeL - holeR;

      //the same translation works for all the following rounded tubes
      G4ThreeVector transSphere(0.01*mm, 0.01*mm, -tubeL/2.-0.1*mm); //if offsets are 0. it does not display !!

      //now add a passivated layer
      G4Sphere* passivatedSphere = new G4Sphere("passSphere", 0.0*mm, passiveR,           0.*degree, 360.*degree, 0.*degree, 180.*degree);
      G4Tubs*   passivatedTube   = new G4Tubs(  "passTube",   0.0*mm, passiveR, tubeL/2., 0.*degree, 360.*degree);
      solidPassivated_PhaseII    = new G4UnionSolid("passivatedGe",passivatedTube,passivatedSphere,0,transSphere);

      //and the Li contact
      G4Sphere* contactSphere  = new G4Sphere("sphere1", 0.0*mm, contactR,           0.*deg, 360.*deg, 0.*deg, 180.*deg);
      G4Tubs*   contactTube    = new G4Tubs(  "tube1",   0.0*mm, contactR, tubeL/2., 0.*deg, 360.*deg);
      solidContact_PhaseII = new G4UnionSolid("liContact",contactTube,contactSphere,0,transSphere);

      //bore out a hole
      G4Sphere* boreSphere  = new G4Sphere(    "boreSphere", 0.0*mm, holeR,           0.*degree, 360.*degree, 0.*degree, 180.*degree);
      G4Tubs*   boreTube    = new G4Tubs(      "boreTube",   0.0*mm, holeR, tubeL/2., 0.*degree, 360.*degree);
      solidBoreHole_PhaseII = new G4UnionSolid("boreHole",boreTube,boreSphere,0,transSphere);

      //save these for placements....
      fContact_dZ_PhaseII = holeL/2. - contactThick;// - passiveThick;

      //put corners @ (0,0)
      fGeLeaf_dX_PhaseII = largeSquare/2. - transX/2.;
    }




    //---------------------------------------------------------------------
    // Create the solids defining Phase-II Clovers
    //---------------------------------------------------------------------
    void  Chamber::CreateGREATCloverSolids()
    {
      //An approximate CloverII
      G4cout << G4endl << "Constructing archetypal GREAT Clover" << G4endl;

      //---------------------------------------------------------
      //end-cap
      G4double endCapFrontThickness = 2.0*mm;
      G4double endCapTaperThickness = 2.0*mm;
      G4double endCapSideThickness  = 2.0*mm;

      G4double GeGap      = fEndCap2Ge_GREAT - endCapFrontThickness;
      G4double taperAngle = 15.*degree;

      G4double endCapTotalL = fTotalGeL_GREAT + GeGap + 2.*endCapFrontThickness + 5.*mm; //+ Gap at rear end
      G4double endCapFrontD = 60.09*mm;
      G4double endCapBackD  = 70.00*mm;
      G4double endCapTaperL = 37.00*mm;

      G4double endCapBoxL   = endCapTotalL - endCapTaperL;

      //the tapered part....
      G4Trap* solidTaperedCloverEC  = new G4Trap("taperedCloverEC",
          endCapTaperL/2.,    //Half z-length [pDz]
          0.00*degree, //Polar angle of line joining centres of the faces @ -/+pDz
          45.0*degree,   //aequivalent zimuthal angle       //Does not make sense !
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
      solidEndCap_GREAT = new G4UnionSolid("Box+Taper",solidTaperedCloverEC,endCapBox,0,transECBox);
      //need the taperL for placement
      fEndCapTaperL_GREAT = endCapTaperL;


      //---------------------------------------------------------
      //end-cap inner vacuum
      G4double endCapDelta_1 = endCapTaperThickness/cos(taperAngle) - endCapFrontThickness*tan(taperAngle);
      G4double endCapDelta_2 = ( endCapSideThickness - (endCapTaperThickness*sin(taperAngle)*tan(taperAngle) +
            endCapTaperThickness*cos(taperAngle) ) )/tan(taperAngle);

      G4cout << endCapDelta_1 << " " << endCapDelta_2 << endl;

      G4double endCapVacTaperL = endCapTaperL - endCapFrontThickness - endCapDelta_2;
      G4double endCapVacBoxL   = endCapBoxL + endCapDelta_2;
      G4double endCapVacTotalL = endCapVacBoxL + endCapVacTaperL;
      G4double endCapVacFrontD = endCapFrontD - endCapDelta_1;
      G4double endCapVacBackD  = endCapBackD  - endCapSideThickness;

      //position of vacuum wrt end-cap
      fVacuumPosZ_GREAT = (-endCapTotalL + endCapVacTotalL - endCapDelta_2)/2. + endCapFrontThickness;

      //tapered part...
      G4Trap* solidTaperVac  = new G4Trap("cloverTaperVac",
          endCapVacTaperL/2.,    //Half z-length [pDz]
          0.00*degree, //Polar angle of line joining centres of the faces @ -/+pDz
          45.0*degree,   //aequivalent zimuthal angle 
          endCapVacFrontD,    //pDy1 half y length at -pDz
          endCapVacFrontD,    //pDx1 half x length at -pDz, -pDy1
          endCapVacFrontD,    //pDx2 half x length at -pDz, +pDy1
          0.00*degree,//pAlpha1 wrt y-axis from the centre of the side (lower endcap)
          endCapVacBackD,    //pDy2 half y length at +pDz
          endCapVacBackD,    //pDx3 half x length at +pDz, -pDy2
          endCapVacBackD,    //pDx4 half x length at +pDz, +pDy2
          0.00*degree); //pAlpha2 wrt y-axis from the centre of the side (upper endcap)

      G4cout << endCapTotalL << " " << endCapVacTotalL << endl;

      //rectangular part
      G4Box*         endCapVacBox  = new G4Box("endCapBox",endCapVacBackD,endCapVacBackD,endCapVacBoxL/2.);
      G4ThreeVector transVacBox(   0.*mm, 0.*mm, endCapVacTaperL/2.+endCapVacBoxL/2.);

      //add them together
      solidVacuum_GREAT = new G4UnionSolid("Vac_Box+Taper",solidTaperVac,endCapVacBox,0,transVacBox);


      //---------------------------------------------------------
      //The Ge crystal...
      G4double GeTaperL    = fTaperGeL_GREAT; //30.0*mm;
      G4double GeTotalL    = fTotalGeL_GREAT; //105 ? 140.0 * mm;

      G4double smallSquare = fFrontFaceSquare_GREAT;
      G4double largeSquare = fBackFaceSquare_GREAT;

      G4double transX = (largeSquare-smallSquare)/2.;
      G4double transY = (largeSquare-smallSquare)/2.;
      transX /= 2.;
      transY /= 2.;

      G4cout << "Got to the point of creating the clover leaf" << G4endl;

      //don't understand this G4Trap : expect an angle of 12.15 degrees !
      G4Trap* solidTaper = new G4Trap("cloverTaper",
          GeTaperL/2.,    //Half z-length [pDz]
          9.63*degree,   //This is a fudge angle and is diff from debug //Polar angle of line joining centres of the faces @ -/+pDz
          45.0*degree,    //equivalent zimuthal angle 
          smallSquare/2., //pDy1 half y length at -pDz
          smallSquare/2., //pDx1 half x length at -pDz, -pDy1
          smallSquare/2., //pDx2 half x length at -pDz, +pDy1
          0.00*degree,//pAlpha1 wrt y-axis from the centre of the side (lower endcap)
          largeSquare/2.,    //pDy2 half y length at +pDz
          largeSquare/2.,    //pDx3 half x length at +pDz, -pDy2
          largeSquare/2.,    //pDx4 half x length at +pDz, +pDy2
          0.0*degree); //pAlpha2 wrt y-axis from the centre of the side (upper endcap)

      /*
         This was used for a phaseII to make rounded edges
         const G4int numZPlanesGe=4;      // no. polycone planes
         G4double zPlaneGe[numZPlanesGe]; // positions of planes
         zPlaneGe[0] =  0.00*mm;
         zPlaneGe[1] =  1.46*mm;
         zPlaneGe[2] =  5.00*mm;
         zPlaneGe[3] = GeTaperL;

         G4double rInnerGe[numZPlanesGe]; // interior radii
         rInnerGe[0] = rInnerGe[1] = rInnerGe[2] = rInnerGe[3] = 0.0*mm;
         G4double rOuterGe[numZPlanesGe]; // exterior radii
         rOuterGe[0] = 20.5*mm;  rOuterGe[1] = 23.04*mm; //23.54*mm;//exagerate it more
         rOuterGe[2] = rOuterGe[3] = fCrystalR_PhaseII;


         G4Polycone* solidCone = new G4Polycone("cloverCone", 0.0*degree, 360.0*degree,numZPlanesGe, zPlaneGe, rInnerGe, rOuterGe);
         G4ThreeVector  transGeCone( -transX/2., -transY/2., -GeTaperL/2.);
         G4IntersectionSolid* taperedCone = new G4IntersectionSolid("Taper+Cone",solidTaper,solidCone,0,transGeCone);
         */

      //back part....
      G4double geBoxL = GeTotalL - GeTaperL;
      G4Box* GeBox = new G4Box("GeBox", largeSquare/2., largeSquare/2., geBoxL/2.);

      //add back box and front tapered parts
      G4ThreeVector transGeBox( transX, transX, GeTaperL/2.+geBoxL/2.);
      G4UnionSolid* newTaper  = new G4UnionSolid("Box+Taper",solidTaper,GeBox,0,transGeBox);


      //now make a cylinder 90x60 which needs to be displaced before intersection
      //the centres of the cylinders should be 56 mm apart, but the gap between leaves
      // is 0.8 mm => centre of cylinder should be at (27.6,27.6) wrt inner corner
      G4double dx1 = 27.60*mm;
      G4double dx2 = largeSquare/2.;
      G4cout << "transX " << transX << " dx2 " << dx2 << ".....hole_dX " << fHole_dX_GREAT << G4endl;

      fHole_dX_GREAT = transX + dx1 - dx2; //save the displacements for the bore-hole placements
      fHole_dY_GREAT = transY + dx1 - dx2;

      G4Tubs* GeCyl = new G4Tubs("GeCyl",0.0*mm, 35*mm, GeTotalL/2.,0.*degree,360.*degree);
      G4ThreeVector transGeCyl( fHole_dX_GREAT, fHole_dY_GREAT, (GeTotalL-GeTaperL)/2.);

      //add all of this together for the Ge....
      solidGeLeaf_GREAT  = new G4IntersectionSolid("Box+Taper+Cyl",newTaper,GeCyl,0,transGeCyl);


      //put corners @ (0,0)
      fGeLeaf_dX_GREAT = largeSquare/2. - transX;
      //fGeLeaf_dY_GREAT = largeSquare/2. - transY;
      G4cout << "shift leaves " << fGeLeaf_dX_GREAT << "....." << fHole_dX_GREAT << G4endl;

      //z-position of Ge-leaf wrt vacuum
      fGeLeafPosZ_GREAT = -endCapVacTaperL/2. + GeTaperL/2. + GeGap; //wrt clover Vacuum

      //1.875 28.875.....-1.575 27.6

      //-------------------------------------------------------
      // Inner bore hole + lithium contact + passivated Ge
      G4double GeDepth      = 15.00 * mm;
      G4double holeL        = GeTotalL - GeDepth;  //length of bore hole
      G4double passiveThick =  0.30 * mm;  //passivated Ge
      G4double contactThick =  0.50 * mm;  //Li contact

      //G4double innerRHole =  0.00 * mm;
      G4double holeR      =  5.00 * mm; //fHoleR_PhaseII;
      G4double contactR   = holeR + contactThick;
      G4double passiveR   = contactR + passiveThick;
      G4double tubeL      = holeL - holeR;

      //the same translation works for all the following rounded tubes
      G4ThreeVector transSphere(0.001*mm, 0.001*mm, -tubeL/2.-0.001*mm); //if offsets are 0. it does not display !!

      //now add a passivated layer
      G4Sphere* passivatedSphere = new G4Sphere("passSphere", 0.0*mm, passiveR,           0.*degree, 360.*degree, 0.*degree, 180.*degree);
      G4Tubs*   passivatedTube   = new G4Tubs(  "passTube",   0.0*mm, passiveR, tubeL/2., 0.*degree, 360.*degree);
      solidPassivated_GREAT    = new G4UnionSolid("passivatedGe",passivatedTube,passivatedSphere,0,transSphere);

      //and the Li contact
      G4Sphere* contactSphere  = new G4Sphere("sphere1", 0.0*mm, contactR,           0.*deg, 360.*deg, 0.*deg, 180.*deg);
      G4Tubs*   contactTube    = new G4Tubs(  "tube1",   0.0*mm, contactR, tubeL/2., 0.*deg, 360.*deg);
      solidContact_GREAT = new G4UnionSolid("liContact",contactTube,contactSphere,0,transSphere);

      //bore out a hole
      G4Sphere* boreSphere  = new G4Sphere(   "boreSphere", 0.0*mm, holeR,           0.*degree, 360.*degree, 0.*degree, 180.*degree);
      G4Tubs*   boreTube    = new G4Tubs(     "boreTube",   0.0*mm, holeR, tubeL/2., 0.*degree, 360.*degree);
      solidBoreHole_GREAT = new G4UnionSolid("boreHole",boreTube,boreSphere,0,transSphere);

      //save this for placement
      fContact_dZ_GREAT = -GeTaperL/2 + tubeL/2 + holeR + GeDepth;
      G4cout << "fContact_dZ_GREAT " << fContact_dZ_GREAT << G4endl;
      G4cout << "totalL/2 " << GeTotalL/2 << " taperL/2 " << GeTaperL/2 << "  tubeL/2 " << tubeL/2 << " : GeDepth " << GeDepth << G4endl;




    }
