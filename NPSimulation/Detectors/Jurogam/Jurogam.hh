#ifndef Jurogam_h
#define Jurogam_h 1
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

// C++ header
#include <string>
#include <vector>
using namespace std;

// G4 headers
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4LogicalVolume.hh"
#include "G4MultiFunctionalDetector.hh"

// NPTool header
#include "NPSVDetector.hh"
#include "TJurogamData.h"
#include "NPInputParser.h"

class G4Tubs;
class G4Polycone;
class G4UnionSolid;
class Materials;


class Jurogam : public NPS::VDetector{
	////////////////////////////////////////////////////
	/////// Default Constructor and Destructor /////////
	////////////////////////////////////////////////////
	public:
		Jurogam() ;
		virtual ~Jurogam() ;

		////////////////////////////////////////////////////
		/////// Specific Function of this Class ///////////
		////////////////////////////////////////////////////
	public:
		// Cartesian
		void AddDetector(G4ThreeVector POS, string Shape, G4bool BGO);
		// Spherical
		void AddDetector(double R,double Theta,double Phi,string Shape, G4bool BGO);  
		// Spherical with offset
		void AddDetector(G4ThreeVector POS,double Theta,double Phi, string Shape, G4bool BGO);  

		//Generic positioning and rotating for all detectors
		void SetPosition( G4ThreeVector );
		void SetRotation( G4RotationMatrix );
		void PhaseISetAbsorber( G4bool );
		void CloverSetAbsorber( G4bool );

		//Generate solids
		void CreatePhaseISolids();
		void CreatePhaseIBGOSolids();
		void CreateCloverSolids();
		void CreateCloverBGOSolids();

		//Generating the logical volumes
		G4LogicalVolume* BuildPhaseI();
		G4LogicalVolume* BuildPhaseIBGO();
		G4LogicalVolume* BuildClover();
		G4LogicalVolume* BuildCloverBGO();

		//Placing physical volumes
		void PhaseIPlacement(G4int, G4LogicalVolume*, G4bool);
		void PhaseIBGOPlacement(G4int, G4LogicalVolume*, G4bool);
		void CloverPlacement(G4int, G4LogicalVolume*, G4bool);
		void CloverBGOPlacement(G4int, G4LogicalVolume*, G4bool);//TODO

		inline G4double GetTaperedEndCapL() {return fEndCapTaperL/mm;};

	private:

		////////////////////////////////////////////////////
		//////  Inherite from NPS::VDetector class /////////
		////////////////////////////////////////////////////
	public:
		// Read stream at Configfile to pick-up parameters of detector (Position,...)
		// Called in DetecorConstruction::ReadDetextorConfiguration Method
		void ReadConfiguration(NPL::InputParser) ;

		// Construct detector and inialise sensitive part.
		// Called After DetecorConstruction::AddDetector Method
		void ConstructDetector(G4LogicalVolume* world) ;

		// Add Detector branch to the EventTree.
		// Called After DetecorConstruction::AddDetector Method
		void InitializeRootOutput() ;

		// Read sensitive part and fill the Root tree.
		// Called at in the EventAction::EndOfEventAvtion
		void ReadSensitive(const G4Event* event) ;

	public:   // Scorer
		//   Initialize all Scorer used by the MUST2Array
		void InitializeScorers() ;

		//   Associated Scorer
		G4MultiFunctionalDetector* m_JurogamScorer ;
		G4MultiFunctionalDetector* m_JurogamBGOScorer ;
		////////////////////////////////////////////////////
		///////////Event class to store Data////////////////
		////////////////////////////////////////////////////
	private:
		TJurogamData* m_Event ;

		////////////////////////////////////////////////////
		///////////////Private intern Data//////////////////
		////////////////////////////////////////////////////
	private: // Geometry
		// Detector Coordinate 
		vector<double>  m_R; 
		vector<double>  m_Theta;
		vector<double>  m_Phi; 
		vector<double>  m_X;
		vector<double>  m_Y;
		vector<double>  m_Z;

		//   Shape type
		vector<string> m_Shape;

		// BGO placement flag
		vector<G4bool> m_BGO;
		//General materials....
		Materials*   fMat;

		//G4Material* eDetMat;
		G4Material* endCapMaterial;
		G4Material* vacuumMaterial;
		G4Material* geMaterial;
		G4Material* beMaterial;
		G4Material* contactMaterial;
		G4Material* BGOMaterial;
		G4Material* HevimetMaterial;
		G4Material* ShieldMaterial;
		G4Material* Absorber1Material;
		G4Material* Absorber2Material;

		//Clover building flags
		G4bool CloverConstructed = false;
		G4bool CloverBGOConstructed = false;

        G4bool PhaseI_placed = false;
        G4bool PhaseI_bgo_placed = false;
        G4bool Clover_placed = false;
        G4bool Clover_bgo_placed = false;

		G4ThreeVector        position;
		G4RotationMatrix     rotation;

		//Keep track of which detector is used
		G4String  theDetector;
		G4int     nDetectors;

		//-------------------------------------------
		// A Phase-I tapered Ge
		G4double            fGeTaperL_PhaseI;
		G4double            fTotalGeL_PhaseI;
		G4double            fGeOuterD_PhaseI;
		G4double            fGeInnerD_PhaseI;
		G4double            fAlCap2Ge_PhaseI;
		G4double            fEndCapThickness_PhaseI;
		G4double            fEndCapTaperL_PhaseI;
		G4double            fEndCapTubeL_PhaseI;
		G4double            fEndCapFrontR_PhaseI;
		G4double            fEndCapBackR_PhaseI;
		//
		G4Polycone*         solidAlCap_PhaseI;  //Al end-cap
		G4LogicalVolume*    logicAlCap_PhaseI;
		G4VPhysicalVolume*  physiAlCap_PhaseI;
		G4Polycone*         solidVacuum_PhaseI; //vacuum
		G4LogicalVolume*    logicVacuum_PhaseI;
		G4VPhysicalVolume*  physiVacuum_PhaseI;
		G4Polycone*         solidGe_PhaseI; //Ge
		G4LogicalVolume*    logicGe_PhaseI = 0;
		G4VPhysicalVolume*  physiGe_PhaseI;
		//G4String            sensitiveGeName;
		G4double           fContact_dZ_PhaseI;  //dZ to position Passivated Ge
		G4UnionSolid*      solidPassivated_PhaseI;
		G4LogicalVolume*   logicPassivated_PhaseI;
		G4VPhysicalVolume* physiPassivated_PhaseI;
		G4UnionSolid*      solidContact_PhaseI; //inner Li contact
		G4LogicalVolume*   logicContact_PhaseI;
		G4VPhysicalVolume* physiContact_PhaseI;
		G4UnionSolid*      solidBoreHole_PhaseI; //inner Li contact
		G4LogicalVolume*   logicBoreHole_PhaseI;
		G4VPhysicalVolume* physiBoreHole_PhaseI;


		//Specific Phase-I's
		G4double fAlCap2Ge_Ortec;
		G4double fGeTaperL_Ortec;
		G4double fTotalGeL_Ortec;
		G4double fGeOuterD_Ortec;
		G4double fGeInnerD_Ortec;
		G4double fEndCapFrontR_Ortec;
		G4double fEndCapBackR_Ortec;
		G4double fEndCapTaperL_Ortec;
		G4double fEndCapTubeL_Ortec;
		G4double fEndCapThickness_Ortec;

		//-------------------------------------------
		G4double            geRingZPos;

		//PhaseI BGO 
		G4bool   useAbsorber;

		G4double PhaseIBGOShieldLength;
		G4double PhaseIBGOHevimetThickness;
		G4double PhaseIBGOHevimetDist;
		G4double PhaseIBGOShieldDist;
		G4double PhaseIBGOCrystalDist;
		G4double PhaseIBGOShieldOuterRadius;
		G4double PhaseIBGOShieldInnerRadius;
		G4double PhaseIsubInnerConeL;
		G4double abs1_thickness;
		G4double abs2_thickness;

		G4VSolid*           PhaseISolidBGOCrystal;
		G4VSolid*           PhaseISolidBGOHevimet; //G4SubtractionSolid*
		G4VSolid*           PhaseISolidBGOShield;
		G4VSolid*           Phase1Absorber1;
		G4VSolid*           Phase1Absorber2;
		//G4UnionSolid*           solidBGOShield;

		G4LogicalVolume*     PhaseILogicBGOCrystal = 0;
		G4VPhysicalVolume*   PhaseIPhysiBGOCrystal;
		G4LogicalVolume*     PhaseILogicBGOShield;
		G4VPhysicalVolume*   PhaseIPhysiBGOShield;
		G4LogicalVolume*     PhaseILogicBGOHevimet;
		G4VPhysicalVolume*   PhaseIPhysiBGOHevimet;
		G4LogicalVolume*     Phase1Absorber1_log;
		G4LogicalVolume*     Phase1Absorber2_log;
		G4VPhysicalVolume*   Phase1Absorber1_phys;
		G4VPhysicalVolume*   Phase1Absorber2_phys;

		//Clover
		G4double             lappingSize;

		G4double             fCrystalR;
		G4double             fTotalGeL;
		G4double             fHoleR;
		G4double             fContactThick;
		G4double             fPassiveThick;
		G4double             fEndCapTaperL;
		G4double             fEndCapThickness;
		G4double             fEndCap2Ge;
		G4double             fFudge;
		G4double             fVacuumPosZ;
		G4double             fContact_dZ;
		G4double             fGeLeafPosZ;
		G4double             fGapBetweenLeaves;
		G4double             fGeLeaf_dX;
		G4double             fHole_dX;
		G4double             fHole_dY;

		G4UnionSolid*        solidEndCap;
		G4UnionSolid*        solidVacuum;
		G4UnionSolid*        solidGeLeaf;
		G4UnionSolid*        solidPassivated;
		G4UnionSolid*        solidContact;
		G4UnionSolid*        solidBoreHole;

		G4LogicalVolume*     logicEndCap;
		G4VPhysicalVolume*   physiEndCap;
		G4LogicalVolume*     logicVacuum;
		G4VPhysicalVolume*   physiVacuum;
		G4LogicalVolume*     logicGeLeaf[4];
		G4VPhysicalVolume*   physiGeLeaf[4];
		G4LogicalVolume*     logicPassivated[4];
		G4VPhysicalVolume*   physiPassivated[4];
		G4LogicalVolume*     logicContact[4];
		G4VPhysicalVolume*   physiContact[4];
		G4LogicalVolume*     logicBoreHole[4];
		G4VPhysicalVolume*   physiBoreHole[4];

		//Clover BGO
		G4double CloverBGOHevimetThickness;
		G4double CloverBGOHevimetDist;
		G4double CloverBGOShieldDist;
		G4double CloverBGOCrystalDist;
		G4double CloverBGOCrystalH;
		G4double CloverBGOCrystalAngle;
		G4double CloverBGOCrystalBoxD;
		G4double CloverBGOCrystalBoxD2;
		G4double CloverBGOCrystalTrapL;
		G4double CloverBGOCrystalW;
		G4double CloverBGOCrystalW2;
		G4double CloverBGOCrystalW3;
		//G4double abs1_thickness;
		//G4double abs2_thickness;
		G4double AbsorberWidth;

		G4UnionSolid*       CloverSolidBGOCrystal; // 1 side
		G4VSolid*           CloverSolidBGOCrystal2; // upper crystal, sliced
		G4VSolid*           CloverSolidBGOCrystal3; // lower crystal, sliced (mirrored)
		G4VSolid*           CloverSolidBGOHevimet;
		G4VSolid*           CloverSolidBGOShield;
		G4VSolid*           CloverAbsorber1;
		G4VSolid*           CloverAbsorber2;

		G4LogicalVolume*     CloverLogicBGOCrystal[4];
		G4VPhysicalVolume*   CloverPhysiBGOCrystal[4];
		G4LogicalVolume*     CloverLogicBGOShield;
		G4VPhysicalVolume*   CloverPhysiBGOShield;
		G4LogicalVolume*     CloverLogicBGOHevimet;
		G4VPhysicalVolume*   CloverPhysiBGOHevimet;
		G4LogicalVolume*     CloverAbsorber1_log;
		G4LogicalVolume*     CloverAbsorber2_log;
		G4VPhysicalVolume*   CloverAbsorber1_phys;
		G4VPhysicalVolume*   CloverAbsorber2_phys;


		//logicGe_PhaseI = 0;
		//PhaseILogicBGOCrystal = 0;
		//CloverConstructed = false;
		//CloverBGOConstructed = false;
		// Needed for dynamic loading of the library
	public:
		static NPS::VDetector* Construct();
};
#endif
