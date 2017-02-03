#ifndef Chamber2Array_h
#define Chamber2Array_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche    contact address: marc.labiche@atfc.ac.uk   *
 *                                                                           *
 * Creation Date  : January 2010                                             *
 * Last update    : 19/07/2016                                               *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe Cryogenic and standard Chamber.                      *
 *  Derived fromNPS::VDetector                                               *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// C++ headers
#include <string>
#include <cmath>

// G4 header defining G4 types
#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

// G4 headers
#include "G4Event.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"


// NPTool headers
#include "NPSVDetector.hh"
#include "NPInputParser.h"
using namespace std;
using namespace CLHEP;

class Chamber : public NPS::VDetector
{
   public:
      Chamber();
      ~Chamber(){};


   public:
      //   Read stream at Configfile to pick-up parameters of detector (Position,...)
      //   Called in DetecorConstruction::ReadDetextorConfiguration Method
      void ReadConfiguration(NPL::InputParser);

      //   Construct detector and inialise sensitive part.
      //   Called After DetecorConstruction::AddDetector Method
      void ConstructDetector(G4LogicalVolume* world);

      //   Add Detector branch to the EventTree.
      //   Called After DetecorConstruction::AddDetector Method
      void InitializeRootOutput();

      //   Read sensitive part and fill the Root tree.
      //   Called at in the EventAction::EndOfEventAvtion
      void ReadSensitive(const G4Event* event);


   public:
      //   Return Material from the Chamber Material Library
      G4Material* GetMaterialFromLibrary(G4String MaterialName, G4double Temperature = 0, G4double Pressure = 0);
      
      //   Generate a DEDX file table using the material used in the target
      void             WriteDEDXTable(G4ParticleDefinition* Particle,G4double Emin,G4double Emax);

   public:
      G4double    GetChamberRmin()     {return m_ChamberRmin;}
      G4double    GetChamberRmax()     {return m_ChamberRmax;}
      G4double    GetChamberPhiMin()   {return m_ChamberPhiMin;}
      G4double    GetChamberPhiMax()   {return m_ChamberPhiMax;}
      G4double    GetChamberThetaMin() {return m_ChamberThetaMin;}
      G4double    GetChamberThetaMax() {return m_ChamberThetaMax;}
      G4Material* GetChamberMaterial() {return m_ChamberMaterial;}
  //G4int       GetChamberNbLayers()      {return m_ChamberNbLayers;}


   private:
      // Chamber type : true = normal ; false = cryo
      //bool     m_ChamberType;
      G4int     m_ChamberType;

      // Standard parameter
      G4double    m_ChamberRmin;
      G4double    m_ChamberRmax;
      G4double    m_ChamberPhiMin;
      G4double    m_ChamberPhiMax;
      G4double    m_ChamberThetaMin;
      G4double    m_ChamberThetaMax;
      G4Material* m_ChamberMaterial;
      G4int       m_ChamberNbLayers;
  /*
      // For Cryo Chamber
      G4double    m_ChamberTemperature;
      G4double    m_ChamberPressure;
      G4double    m_WindowsThickness;
      G4Material* m_WindowsMaterial;

      // Positioning
      G4double    m_ChamberX;
      G4double    m_ChamberY;
      G4double    m_ChamberZ;
  */

  // GREAT chamber parameter
        G4double    m_ChamberHmin;
       	G4double    m_ChamberHmax;
       	G4double    m_ChamberWmin;
       	G4double    m_ChamberWmax;
       	G4double    m_ChamberDmin;
       	G4double    m_ChamberDmax;

  /* Karl's geometry of GREAT: */

  //Gap between BackPlate and SiSupport
  G4double fSiSupportThickness;
  G4double fCorrection;

  //distance from up-stream Si face to up-stream flange face
  G4double fSi2BackPlate;

  //distance from flange to Ge Al Cap
  G4double fBackPlate2AlCap;

  //------------------------------------------------------------------
  // Vacuum chamber walls
  G4double           fChamberL;
  G4double           fChamberH;
  G4double           fChamberW;
  G4double           fChamberThickness;
  G4double           fVacuumH;
  G4double           fShift;
  G4double           fShiftChamberZ;
  G4double           fShiftChamberY;

  //------------------------------------------------------------------
  //Support frame (Cooling Block) for SiImp and Tunnel
  G4double           fCoolingBlockL;
  G4double           fCoolingBlockH;
  G4double           fCoolingBlockW;
  G4double           fCoolingBlockT;
  G4double           fCoolingBlockZ;
  G4double           fdeltaZ_Support;

  G4double fCoolingBlockCutOutH;
  G4double fCoolingBlockCutOutW;
  G4double fCoolingBlockDSSDCentre;
  G4double fCoolingBlockCutOut_PosY;
  G4double fCentreOfDSSD_Y;

  G4double fLengthOfPlanarCut;
  
  //------------------------------------------------------------------
  //Epoxy board for Si implantation detector
  G4double           fDSSD_BoardL;   //epoxy thickness
  G4double           fDSSD_BoardZ;   //epoxy position
  //------------------------------------------------------------------
  //Si DSSD detector
  G4double           fDSSD_W;     //detector width     (x)
  G4double           fDSSD_H;     //detector height    (y)
  G4double           fDSSD_T;     //detector thickness (z)
  G4double           fDSSD_Guard;
  G4double           fDSSD_PosY;  //centre of dets at this Y
  G4double           fDSSD_PosZ;  //upstream face at this Z


  const int nStripsX;
  const int nStripsY;

  //-------------------------------------
  // Planar
  //-------------------------------------
  // Al end cap
  G4double           fGap_PCB_2_Planar;
  G4double           fEndCapThickness;
  G4double           fBeWindowThickness;
  G4double           fAlWindowThickness;

  //Planar Ge
  G4double           fPlanarGuard;
  G4double           fPlanarGe_W;
  G4double           fPlanarGe_H;
  G4double           fPlanarGe_T;
  G4double           fPlanarGeGapFront;
  G4double           fPlanarGeGapBack;
  G4double           fPlanarTotalL;
  //Positioning
  G4double           fGapChamber2Planar;
  G4double           fShiftPlanar;
  G4double           fPlanar_PosZ;

  G4double           fGapPlanarDSSDBoard;





  // Materials:

  G4Material* defaultMaterial;
  G4Material* eDetMat;   
  G4Material* endCapMaterial;
  G4Material* vacuumMaterial;
  G4Material* geMaterial;   
  G4Material* contactMaterial;

  //add these in so that auger are tabulated
  G4Element* fermium;
  G4Element* einsteinium;

  G4Material*        WorldMaterial; // pointer to the detector  material
  G4Material*        chamberMaterial; 
  G4Material*        supportMaterial;
  G4Material*        boardMaterial; 
  G4Material*        beMaterial;
  G4Material*        metalbarMaterial;




  G4LogicalVolume*   logicPlanarGe[12][24];

  G4LogicalVolume*   logicDSSD_A[40][60];
  G4LogicalVolume*   logicDSSD_B[40][60];

  G4LogicalVolume*   logicPINS[28];
  G4LogicalVolume*   logicPINS_Active[28];

  //-------------------------------------
  // Ge Clovers.....
  //-------------------------------------
  //Keep track of which detectors are used
  //G4String  typeLookingUpstream;
  G4String  typeTop;
  G4String  typeLeft;
  G4String  typeRight;
  G4int nLeavesClover;

  //Up to 4 Clovers... left, right, top and looking-upstream
  G4double             fGeTopGap;
  G4double             geTopPosition;
  G4LogicalVolume*     logicEndCap_CloverT;  //on the Top
  //G4VPhysicalVolume*   physiEndCap_CloverT;
  G4LogicalVolume*     logicVacuum_CloverT;
  //G4VPhysicalVolume*   physiVacuum_CloverT;
  G4LogicalVolume*     logicGeLeaf_CloverT[4];
  //G4VPhysicalVolume*   physiGeLeaf_CloverT[4];
  G4LogicalVolume*     logicPassivated_CloverT[4];
  //G4VPhysicalVolume*   physiPassivated_CloverT[4];
  G4LogicalVolume*     logicContact_CloverT[4];
  //G4VPhysicalVolume*   physiContact_CloverT[4];
  G4LogicalVolume*     logicBoreHole_CloverT[4];
  //G4VPhysicalVolume*   physiBoreHole_CloverT[4];


  G4LogicalVolume*     logicEndCap_CloverU;  //looking upstream
  //G4VPhysicalVolume*   physiEndCap_CloverU;
  G4LogicalVolume*     logicVacuum_CloverU;
  //G4VPhysicalVolume*   physiVacuum_CloverU;
  G4LogicalVolume*     logicGeLeaf_CloverU[4];
  //G4VPhysicalVolume*   physiGeLeaf_CloverU[4];
  G4LogicalVolume*     logicPassivated_CloverU[4];
  //G4VPhysicalVolume*   physiPassivated_CloverU[4];
  G4LogicalVolume*     logicContact_CloverU[4];
  //G4VPhysicalVolume*   physiContact_CloverU[4];
  G4LogicalVolume*     logicBoreHole_CloverU[4];
  //G4VPhysicalVolume*   physiBoreHole_CloverU[4];

  G4double             geSidePosition;
  G4LogicalVolume*     logicEndCap_CloverLR[2];  //left and right
  //G4VPhysicalVolume*   physiEndCap_CloverLR[2];
  G4LogicalVolume*     logicVacuum_CloverLR[2];
  //G4VPhysicalVolume*   physiVacuum_CloverLR[2];
  G4LogicalVolume*     logicGeLeaf_CloverLR[2][4];
  //G4VPhysicalVolume*   physiGeLeaf_CloverLR[2][4];
  G4LogicalVolume*     logicPassivated_CloverLR[2][4];
  //G4VPhysicalVolume*   physiPassivated_CloverLR[2][4];
  G4LogicalVolume*     logicContact_CloverLR[2][4];
  //G4VPhysicalVolume*   physiContact_CloverLR[2][4];
  G4LogicalVolume*     logicBoreHole_CloverLR[2][4];
  //G4VPhysicalVolume*   physiBoreHole_CloverLR[2][4];

  
  //these clovers could be PhaseII's or the GREAT clover

  //PhaseII clover
  G4double             fCrystalR_PhaseII;
  G4double             fTotalGeL_PhaseII;

  G4double             fGapBetweenLeaves_PhaseII;

  G4double             fHoleR_PhaseII;
  G4double             fContactThick_PhaseII;

  G4double             fEndCapTaperL_PhaseII;
  G4double             fEndCap2Ge_PhaseII;
  G4double             fEndCapThickness_PhaseII;
  G4double             fFudge;

  G4double             fVacuumPosZ_PhaseII;
  G4double             fGeLeafPosZ_PhaseII;
  G4double             fContact_dZ_PhaseII;
  G4double             fHole_dX_PhaseII;
  G4double             fHole_dY_PhaseII;
  G4double             fGeLeaf_dX_PhaseII;

  G4UnionSolid*        solidEndCap_PhaseII;
  G4UnionSolid*        solidVacuum_PhaseII;
  G4UnionSolid*        solidVacuumIn_PhaseII;
  G4UnionSolid*        solidGeLeaf_PhaseII;
  G4UnionSolid*        solidPassivated_PhaseII;
  G4UnionSolid*        solidContact_PhaseII;
  G4UnionSolid*        solidBoreHole_PhaseII;

  //GREAT clover
  G4double             fCrystalR_GREAT;
  G4double             fTotalGeL_GREAT;
  G4double             fTaperGeL_GREAT;
  G4double             fFrontFaceSquare_GREAT;
  G4double             fBackFaceSquare_GREAT;

  G4double             fGapBetweenLeaves_GREAT;

  G4double             fHoleR_GREAT;
  G4double             fContactThick_GREAT;

  G4double             fEndCapTaperL_GREAT;
  G4double             fEndCap2Ge_GREAT;

  G4double             fVacuumPosZ_GREAT;
  G4double             fContact_dZ_GREAT;
  G4double             fHole_dX_GREAT;
  G4double             fHole_dY_GREAT;
  G4double             fGeLeafPosZ_GREAT;
  G4double             fGeLeaf_dX_GREAT;
  G4UnionSolid*        solidEndCap_GREAT;
  G4UnionSolid*        solidVacuum_GREAT;
  G4IntersectionSolid* solidGeLeaf_GREAT;
  G4UnionSolid*        solidPassivated_GREAT;
  G4UnionSolid*        solidContact_GREAT;
  G4UnionSolid*        solidBoreHole_GREAT;

private:
  void DefineMaterials();

  void CreateCloverIISolids();
  void CreateModifiedCloverIISolids();
  void CreateGREATCloverSolids();

  void Place_PhaseIIs_Left_and_Right(G4LogicalVolume* world);
  void Place_GREAT_Left_and_Right(G4LogicalVolume* world);

  void Place_PhaseII_On_the_Top(G4LogicalVolume* world);
  void Place_GREAT_On_the_Top(G4LogicalVolume* world);

  void Place_PhaseII_LookingUpstream(G4LogicalVolume* world);
  void Place_GREAT_LookingUpstream(G4LogicalVolume* world);




};

#endif
