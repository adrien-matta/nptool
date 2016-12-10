#ifndef GaspardTrackerRectangle_h
#define GaspardTrackerRectangle_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 15/07/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: Define a module of trapezoidal shape for the Gaspard tracker  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/



// C++ headers
#include <vector>

// NPTool header
#include "GaspardTrackerModule.hh"
#include "TInteractionCoordinates.h"
#include "NPInputParser.h"
using namespace std;
using namespace CLHEP;



class GaspardTrackerRectangle : public GaspardTrackerModule{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   GaspardTrackerRectangle();
   virtual ~GaspardTrackerRectangle();

   ////////////////////////////////////////////////////
   //////// Specific Function of this Class ///////////
   ////////////////////////////////////////////////////
public:
   // By Position Method
   void AddModule(G4ThreeVector TL           ,
                  G4ThreeVector BL           ,
                  G4ThreeVector BR           ,
                  G4ThreeVector CT           ,
                  bool          wFirstStage  ,
                  bool          wSecondStage ,
                  bool          wThirdStage);

   // By Angle Method
   void AddModule(G4double R            ,
                  G4double Theta        ,
                  G4double Phi          ,
                  G4double beta_u       ,
                  G4double beta_v       ,
                  G4double beta_w       ,
                  bool     wFirstStage  ,
                  bool     wSecondStage ,
                  bool     wThirdStage);

   // Effectively construct Volume
   // Avoid to have two time same code for Angle and Point definition
   void VolumeMaker(G4int             DetectorNumber,
                    G4ThreeVector     MMpos,
                    G4RotationMatrix* MMrot,
                    bool              wFirstStage,
                    bool              wSecondStage,
                    bool              wThirdStage,
                    G4LogicalVolume*  world);


   ////////////////////////////////////////////////////
   ////  Inherite from GaspardTrackerModule class /////
   ////////////////////////////////////////////////////
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

   // Give the static TInteractionCoordinates from NPS::VDetector to the classes
   // deriving from GaspardTrackerModule
   // This is mandatory since the GaspardTracker*** does not derive from NPS::VDetector
   void SetInterCoordPointer(TInteractionCoordinates* interCoord);
   TInteractionCoordinates* GetInterCoordPointer()      {return ms_InterCoord;};



   ////////////////////////////////////////////////////
   ///////////////Private intern Data//////////////////
   ////////////////////////////////////////////////////
private:
   // Interaction Coordinates coming from NPS::VDetector through the 
   // SetInteractionCoordinatesPointer method
   TInteractionCoordinates* ms_InterCoord;

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
};



namespace GPDRECT
{
   // Energy/Time resolutions for the different layers
   const G4double ResoFirstStage  = 0.021          ;// = 52keV of Resolution   //   Unit is MeV/2.35
   const G4double ResoSecondStage = 0.043          ;// = 130 keV of resolution //   Unit is MeV/2.35
   const G4double ResoThirdStage  = 0.043          ;// = 100 kev of resolution //   Unit is MeV/2.35
   const G4double ResoTimeGpd     = 0.212765957    ;// = 500ps                 //   Unit is  ns/2.35
   const G4double ResoTimePPAC    = 0.106382979    ;// = 250ps                 //   Unit is  ns/2.35


   // Threshold
   const G4double EnergyThreshold = 0.2*MeV;

   // Geometry
   const G4double Length    = 95*mm;
   const G4double Width     = 91.85*mm;
   const G4double Depth     =  20*mm;
   const G4double InterStageDistance = 7*mm;

   // First stage
   const G4double FirstStageLength    = 95*mm;
   const G4double FirstStageWidth     = 91.85*mm;
   const G4double FirstStageThickness = 500*micrometer;
   const G4int    NumberOfStrips      = 128;

   // Second stage
   const G4double SecondStageLength    = FirstStageLength;
   const G4double SecondStageWidth     = FirstStageWidth;
   const G4double SecondStageThickness = 1.5*mm;

   // Third stage
   const G4double ThirdStageLength    = FirstStageLength;
   const G4double ThirdStageWidth     = FirstStageWidth;
   const G4double ThirdStageThickness = 1.5*mm;

   // Starting at the front of the first stage and pointing to the third stage 
   const G4double FirstStage_PosZ  = Depth* -0.5 + 0.5*FirstStageThickness;
   const G4double SecondStage_PosZ = Depth* -0.5 + 0.5*SecondStageThickness + 1*InterStageDistance;
   const G4double ThirdStage_PosZ  = Depth* -0.5 + 0.5*ThirdStageThickness  + 2*InterStageDistance;
}

#endif
