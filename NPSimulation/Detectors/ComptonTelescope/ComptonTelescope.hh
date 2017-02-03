#ifndef ComptonTelescope_h
#define ComptonTelescope_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 10/06/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: Define a module of square shape for the Gaspard tracker       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/



// NPTool headers
#include "NPSVDetector.hh"
#include "TComptonTelescopeData.h"
#include "TComptonTelescopeProcessData.h"
#include "NPInputParser.h"
// Geant4 headers
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4Event.hh"

// C++ headers
#include <vector>
using namespace std;
using namespace CLHEP;



class ComptonTelescope : public NPS::VDetector
//class ComptonTelescope
{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   ComptonTelescope();
   virtual ~ComptonTelescope();

   ////////////////////////////////////////////////////
   //////// Specific Function of this Class ///////////
   ////////////////////////////////////////////////////
public:
   // By Position Method
   void AddModule(G4ThreeVector TL           ,
                  G4ThreeVector BL           ,
                  G4ThreeVector BR           ,
                  G4ThreeVector CT           ,
                  bool          wTracker  ,
                  bool          wCalorimeter);

   // By Angle Method
   void AddModule(G4double R            ,
                  G4double Theta        ,
                  G4double Phi          ,
                  G4double beta_u       ,
                  G4double beta_v       ,
                  G4double beta_w       ,
                  bool     wTracker  ,
                  bool     wCalorimeter);

   // Effectively construct Volume
   // Avoid to have two time same code for Angle and Point definition
   void VolumeMaker(G4int TelescopeNumber          ,
                    G4ThreeVector     CTpos        ,
                    G4RotationMatrix* CTrot        ,
                    bool              wTracker  ,
                    bool              wCalorimeter ,
                    G4LogicalVolume*  world);


   /////////////////////////////////////////
   ////  Inherite from NPS::VDetector class /////
   /////////////////////////////////////////
public:
   // Read stream at Configfile to pick-up parameters of detector (Position,...)
   // Called in DetecorConstruction::ReadDetextorConfiguration Method
   void ReadConfiguration(NPL::InputParser);

   // Construct detector and inialise sensitive part.
   // Called After DetecorConstruction::AddDetector Method
   void ConstructDetector(G4LogicalVolume* world);

   // Add Detector branch to the EventTree.
   // Called After DetecorConstruction::AddDetector Method
   void InitializeRootOutput();

   // Initialize all scorers necessary for the detector
   void InitializeScorers();

   // Read sensitive part and fill the Root tree.
   // Called at in the EventAction::EndOfEventAvtion
   void ReadSensitive(const G4Event* event);


   ////////////////////////////////////////////////////
   ///////////Event class to store Data////////////////
   ////////////////////////////////////////////////////
private:
   TComptonTelescopeData*        m_Event;
   TComptonTelescopeProcessData* m_ProcessEvent;


   ////////////////////////////////////////////////////
   ///////////////////// Scorer ///////////////////////
   ////////////////////////////////////////////////////
private:
   // Silicon Associate Scorer
   G4MultiFunctionalDetector* m_TrackerScorer;

   // CsI Associate Scorer 
   G4MultiFunctionalDetector* m_CalorimeterScorer;


   ////////////////////////////////////////////////////
   //////////////////// Material //////////////////////
   ////////////////////////////////////////////////////
private:
   //   Declare all material used by the MUST2Array
   void InitializeMaterial() ;
   // Si
   G4Material* m_MaterialSilicon;
   // Al
   G4Material* m_MaterialAluminium;
   // CsI
   G4Material* m_MaterialLaBr3;
   //  Vacuum
   G4Material* m_MaterialVacuum ;


   ////////////////////////////////////////////////////
   ///////////////Private intern Data//////////////////
   ////////////////////////////////////////////////////
private:
   // True if Define by Position, False is Define by angle
   vector<bool>   m_DefinitionType  ;

   // Used for "By Point Definition"
   vector<G4ThreeVector>   m_X1_Y1     ; // Top Left Corner Position Vector
   vector<G4ThreeVector>   m_X1_Y128   ; // Bottom Left Corner Position Vector
   vector<G4ThreeVector>   m_X128_Y1   ; // Bottom Right Corner Position Vector
   vector<G4ThreeVector>   m_X128_Y128 ; // Center Corner Position Vector

   // Used for "By Angle Definition"
   vector<G4double>  m_R      ; //  |
   vector<G4double>  m_Theta  ; //  > Spherical coordinate of Strips Silicium Plate
   vector<G4double>  m_Phi    ; //  |

   vector<G4double>  m_beta_u ; //  |
   vector<G4double>  m_beta_v ; //  > Tilt angle of the Telescope
   vector<G4double>  m_beta_w ; //  |

   // Set to true if you want this stage on you telescope
   vector<bool>      m_wTracker;
   vector<bool>      m_wCalorimeter;

   // Set to true if you want to see Telescope Frame in your visualisation
   bool m_non_sensitive_part_visiualisation ;

   G4int    m_NumberOfDSSSD;
   G4int    m_SizeOfDSSSD;
   G4int    m_NumberOfStrips;
   G4double m_ThicknessOfDSSSD;
   G4double m_DistanceInterDSSSD;
   G4double m_ThicknessOfCalorimeter;
   G4double m_DistanceTrackerCalorimeter;
   G4double m_TowerHeight;
public:
    static NPS::VDetector* Construct();
};



namespace COMPTONTELESCOPE
{
   // Resolution
   const G4double EnergyResolutionTracker    = 0.0064;       // = 15 keV of Resolution   //   Unit is MeV/2.35
   const G4double TimeResolutionTracker      = 0.212765957;  // = 500ps                 //   Unit is  ns/2.35
   const G4double EnergyReolutionCalorimeter = 0.021;        // = 52keV of Resolution   //   Unit is MeV/2.35
   const G4double TimeResolutionCalorimeter  = 0.212765957;  // = 500ps                 //   Unit is  ns/2.35
}

#endif
