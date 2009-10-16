#ifndef ThinSi_h
#define ThinSi_h 1
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe a 20um Silicium detector                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
// C++ header
#include <string>
#include <vector>

// G4 header defining G4 types
#include "globals.hh"

// G4 headers
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4LogicalVolume.hh"

// NPTool header
#include "VDetector.hh"

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace THINSI
{
   // Energy and time Resolution
   const G4double ResoTime    = 0      ;
   const G4double ResoEnergy  = 0.064  ;// = 100keV of Resolution   //   Unit is MeV/2.35

   // Geometry
   const G4double DetectorSize   	= 68*mm           ;
   const G4double SiliconThickness  = 20*micrometer   ;
   const G4double FrameThickness    = 3*mm            ;
   const G4double SiliconSize    	= 50*mm           ;
   const G4double AluThickness      = 0.4*micrometer  ;
   const G4int  NumberOfStrip    	= 32       		  ;

   const G4double AluStripFront_PosZ 	= -0.5*SiliconThickness - 0.5*AluThickness ;
   const G4double Si_PosZ        		= 0                                 ;
   const G4double AluStripBack_PosZ  	= 0.5*SiliconThickness + 0.5*AluThickness  ;

}

using namespace THINSI ;

class ThinSi : public VDetector
{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   ThinSi() ;
   virtual ~ThinSi() ;

   ////////////////////////////////////////////////////
   //////// Specific Function of this Class ///////////
   ////////////////////////////////////////////////////
public:
   // By Position Method
   void AddTelescope(G4ThreeVector  TL       ,
         G4ThreeVector  BL       ,
         G4ThreeVector  BR       ,
         G4ThreeVector  TR       );
   // By Angle Method
   void AddTelescope(G4double    R        ,
         G4double    Theta    ,
         G4double    Phi         ,
         G4double    beta_u       ,
         G4double    beta_v       ,
         G4double    beta_w       );

   // Effectively construct Volume
   // Avoid to have two time same code for Angle and Point definition
   void VolumeMaker(G4int DetectorNumber     ,
         G4ThreeVector     MMpos ,
         G4RotationMatrix* MMrot ,
         G4LogicalVolume*  world );


   ////////////////////////////////////////////////////
   /////////  Inherite from VDetector class ///////////
   ////////////////////////////////////////////////////
public:
   // Read stream at Configfile to pick-up parameters of detector (Position,...)
   // Called in DetecorConstruction::ReadDetextorConfiguration Method
   void ReadConfiguration(string Path)          ;

   // Construct detector and inialise sensitive part.
   // Called After DetecorConstruction::AddDetector Method
   void ConstructDetector(G4LogicalVolume* world)  ;

   // Add Detector branch to the EventTree.
   // Called After DetecorConstruction::AddDetector Method
   void InitializeRootOutput()            ;

   // Read sensitive part and fill the Root tree.
   // Called at in the EventAction::EndOfEventAvtion
   void ReadSensitive(const G4Event* event)     ;


   ////////////////////////////////////////////////////
   ///////////Event class to store Data////////////////
   ////////////////////////////////////////////////////
private:
   G4double    m_Energy ;

	 ////////////////////////////////////////////////////
   ///////////////// Scorer Related ///////////////////
   ////////////////////////////////////////////////////

private:
		//	Initialize all Scorer used by the MUST2Array
		void InitializeScorers() ;
	 
		//	Scorer Associate to the 20um Silicon stage
		G4MultiFunctionalDetector*	m_StripScorer ; 



private:
		// 	Initialize mmaterial used in detector definition
		void InitializeMaterial();

		//	List of material
		G4Material* m_MaterialSilicon 	;
   	G4Material* m_MaterialAl 				;
   	G4Material* m_MaterialVacuum 		;

   ////////////////////////////////////////////////////
   ///////////////Private intern Data//////////////////
   ////////////////////////////////////////////////////
private:
   // True if Define by Position, False is Define by angle
   vector<bool>   m_DefinitionType  ;

   // Used for "By Point Definition"
   vector<G4ThreeVector>   m_TL     ; // Top Left Corner Position Vector
   vector<G4ThreeVector>   m_BL     ; // Bottom Left Corner Position Vector
   vector<G4ThreeVector>   m_BR     ; // Bottom Right Corner Position Vector
   vector<G4ThreeVector>   m_TR     ; // Center Corner Position Vector

   // Used for "By Angle Definition"
   vector<G4double>  m_R         ; //  |
   vector<G4double>  m_Theta     ; //  > Spherical coordinate of Strips Silicium Plate
   vector<G4double>  m_Phi    ; //  |

   vector<G4double>  m_beta_u ; //  |
   vector<G4double>  m_beta_v ; //  >Tilt angle of the Telescope
   vector<G4double>  m_beta_w ; //  |
   // Set to true if you want to see Telescope Frame in your visualisation
   bool           m_non_sensitive_part_visiualisation ;

};
#endif
