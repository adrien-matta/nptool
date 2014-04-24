/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
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
 * Decription: Define a module of trapezoidal shape for the Hyde2 tracker  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef Hyde2TrackerTrapezoid1_h
#define Hyde2TrackerTrapezoid1_h 1

// C++ headers
#include <vector>

// NPTool header
#include "Hyde2TrackerModule.hh"
#include "TInteractionCoordinates.h"

using namespace std;
using namespace CLHEP;



class Hyde2TrackerTrapezoid1 : public Hyde2TrackerModule
{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   Hyde2TrackerTrapezoid1();
   virtual ~Hyde2TrackerTrapezoid1();

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
                  bool          wThirdStage  ,
                  bool          wFourthStage ,
                  bool          wFifthStage  ,
                  bool          wSixthStage);

   // By Angle Method
   void AddModule(G4double R            ,
                  G4double Theta        ,
                  G4double Phi          ,
                  G4double beta_u       ,
                  G4double beta_v       ,
                  G4double beta_w       ,
                  bool     wFirstStage  ,
                  bool     wSecondStage ,
                  bool     wThirdStage  ,
                  bool     wFourthStage ,
                  bool     wFifthStage  ,
                  bool     wSixthStage);

   // Effectively construct Volume
   // Avoid to have two time same code for Angle and Point definition
   void VolumeMaker(G4int TelescopeNumber          ,
                    G4ThreeVector     MMpos        ,
                    G4RotationMatrix* MMrot        ,
                    bool              wFirstStage  ,
                    bool              wSecondStage ,
                    bool              wThirdStage  ,
                    bool              wFourthStage ,
                    bool              wFifthStage ,
                    bool              wSixthStage ,
                    G4LogicalVolume*  world);


   ////////////////////////////////////////////////////
   ////  Inherite from Hyde2TrackerModule class /////
   ////////////////////////////////////////////////////
public:
   // Read stream at Configfile to pick-up parameters of detector (Position,...)
   // Called in DetecorConstruction::ReadDetextorConfiguration Method
   void ReadConfiguration(string Path);

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

   // Give the static TInteractionCoordinates from VDetector to the classes
   // deriving from Hyde2TrackerModule
   // This is mandatory since the Hyde2Tracker*** does not derive from VDetector
   void SetInterCoordPointer(TInteractionCoordinates* interCoord);
   TInteractionCoordinates* GetInterCoordPointer()      {return ms_InterCoord;};


   ////////////////////////////////////////////////////
   ///////////////Private intern Data//////////////////
   ////////////////////////////////////////////////////
private:
   // Interaction Coordinates coming from VDetector through the 
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



namespace HYD2TRAP1
{
   // Resolution
   // If input resolution is Y in MeV units, what is X??
   //ResoFirstStage  = X <- X *2.35 = Y; X = Y/2.35
   // Energy/Time resolutions for the different layers
   const G4double ResoFirstStage  = 0.04255;   // = 100 keV of Resolution //   Unit is MeV/2.35
   const G4double ResoSecondStage = 0.01276;   // = 30 keV of resolution //   Unit is MeV/2.35
   const G4double ResoThirdStage  = 0.00851;   // = 20 keV of resolution //   Unit is MeV/2.35
   const G4double ResoFourthStage = 0.00851;   // = 20 keV of resolution //   Unit is MeV/2.35
   const G4double ResoFifthStage  = 0.00851;   // = 20 keV of resolution //   Unit is MeV/2.35
   const G4double ResoSixthStage  = 0.00851;   // = 20 keV of resolution //   Unit is MeV/2.35
   const G4double ResoTimeHyd2    = 0.42553;   // = 1000ps                 //   Unit is  ns/2.35
   const G4double ResoTimePPAC    = 0.106382979;   // = 250ps                 //   Unit is  ns/2.35

   //HYDE-TDR
   const G4double Height    =  97.68*mm;
   const G4double BaseLarge =  97.43*mm;
   const G4double BaseSmall =  62.54*mm;
   const G4double Length    =  20*mm;
   const G4double InterStageDistance = 3*mm;

   // First stage
   const G4double FirstStageThickness = 20*micrometer;
   const G4int    NumberOfStripsX     = 214;
   const G4int    NumberOfStripsY     = 220;
   //HYDE-TDR
   const G4double FirstStageHeight    =  87.68*mm;
   const G4double FirstStageBaseLarge =  85.48*mm;
   const G4double FirstStageBaseSmall =  54.17*mm;

   // Second stage
   const G4double SecondStageHeight    = FirstStageHeight;
   const G4double SecondStageBaseLarge = FirstStageBaseLarge;
   const G4double SecondStageBaseSmall = FirstStageBaseSmall;
   const G4double SecondStageThickness = 100*micrometer;


   // Third stage
   const G4double ThirdStageHeight    = FirstStageHeight;
   const G4double ThirdStageBaseLarge = FirstStageBaseLarge;
   const G4double ThirdStageBaseSmall = FirstStageBaseSmall;
   const G4double ThirdStageThickness = 500*micrometer;


   // Fourth stage
   const G4double FourthStageHeight    = FirstStageHeight;
   const G4double FourthStageBaseLarge = FirstStageBaseLarge;
   const G4double FourthStageBaseSmall = FirstStageBaseSmall;
   const G4double FourthStageThickness = 2000*micrometer;

   // Fifth stage
   const G4double FifthStageHeight    = FirstStageHeight;
   const G4double FifthStageBaseLarge = FirstStageBaseLarge;
   const G4double FifthStageBaseSmall = FirstStageBaseSmall;
   const G4double FifthStageThickness = 2000*micrometer;

   // Sixth stage
   const G4double SixthStageHeight    = FirstStageHeight;
   const G4double SixthStageBaseLarge = FirstStageBaseLarge;
   const G4double SixthStageBaseSmall = FirstStageBaseSmall;
   const G4double SixthStageThickness = 1.5*mm;

   // Starting at the front of the first stage and pointing to the third stage 
   const G4double FirstStage_PosZ  = Length* -0.5 + 0.5*FirstStageThickness;
   const G4double SecondStage_PosZ = Length* -0.5 + 0.5*SecondStageThickness + 1*InterStageDistance;
   const G4double ThirdStage_PosZ  = Length* -0.5 + 0.5*ThirdStageThickness  + 2*InterStageDistance;
   const G4double FourthStage_PosZ = Length* -0.5 + 0.5*FourthStageThickness + 3*InterStageDistance;
   const G4double FifthStage_PosZ  = Length* -0.5 + 0.5*FifthStageThickness  + 4*InterStageDistance;
   const G4double SixthStage_PosZ  = Length* -0.5 + 0.5*SixthStageThickness  + 5*InterStageDistance;
}

#endif
