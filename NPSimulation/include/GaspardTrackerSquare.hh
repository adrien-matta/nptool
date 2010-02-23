/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
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

#ifndef GaspardTrackerSquare_h
#define GaspardTrackerSquare_h 1

// C++ headers
#include <vector>

// NPTool header
#include "GaspardTrackerModule.hh"
#include "TInteractionCoordinates.h"

using namespace std;



class GaspardTrackerSquare : public GaspardTrackerModule
{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   GaspardTrackerSquare();
   virtual ~GaspardTrackerSquare();

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
   void VolumeMaker(G4int TelescopeNumber          ,
                    G4ThreeVector     MMpos        ,
                    G4RotationMatrix* MMrot        ,
                    bool              wFirstStage  ,
                    bool              wSecondStage ,
                    bool              wThirdStage  ,
                    G4LogicalVolume*  world);


   ////////////////////////////////////////////////////
   ////  Inherite from GaspardTrackerModule class /////
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
   // deriving from GaspardTrackerModule
   // This is mandatory since the GaspardTracker*** does not derive from VDetector
   void SetInterCoordPointer(TInteractionCoordinates* interCoord);
   TInteractionCoordinates* GetInterCoordPointer()	{return ms_InterCoord;};


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

   // for debugging purpose
   G4ThreeVector	MMpos;
   G4ThreeVector	MMu;
   G4ThreeVector	MMv;
   G4ThreeVector	MMw;
   G4ThreeVector	CT;
};



namespace GPDSQUARE
{
   // Resolution
   const G4double ResoFirstStage  = 0              ;// = 52keV of Resolution   //   Unit is MeV/2.35
//   const G4double ResoFirstStage  = 0.022          ;// = 52keV of Resolution   //   Unit is MeV/2.35
   const G4double ResoSecondStage = 0.055          ;// = 130 keV of resolution //   Unit is MeV/2.35
   const G4double ResoThirdStage  = 0              ;// = 100 keV of resolution //   Unit is MeV/2.35
//   const G4double ResoThirdStage  = 0.043          ;// = 100 kev of resolution //   Unit is MeV/2.35
   const G4double ResoTimeGpd     = 0.212765957    ;// = 500ps                 //   Unit is  ns/2.35
   const G4double ResoTimePPAC    = 0.106382979    ;// = 250ps                 //   Unit is  ns/2.35

   // Geometry
   const G4double FaceFront = 11.*cm   ;
   const G4double FaceBack = 16.5*cm   ;
//   const G4double Length  = 7.2*cm     ;
   const G4double Length  = 2.0*cm     ;

   // First stage
//   const G4double AluStripThickness = 0.00000001*micrometer;
   const G4double AluStripThickness = 0.4*micrometer ;
   const G4double SiliconThickness  = 300*micrometer ;
   const G4double SiliconFace       = 98*mm          ;
//   const G4double VacBoxThickness   = 3*cm           ;
   const G4double VacBoxThickness   = 0.5*cm           ;
   const G4int    NumberOfStrips    = 128;

   // Second stage
   const G4double SiLiThickness     = 5.1*mm                      ;  // Must be checked
   const G4double SiLiFaceX         = 48.25*mm                    ;
   const G4double SiLiFaceY         = 92*mm                       ;

   // Third stage
   const G4double MylarCsIThickness   = 3*micrometer;
//   const G4double ThirdStageThickness = 1.5*mm;
   const G4double ThirdStageThickness = 8.5*mm;
   const G4double ThirdStageFaceFront = FaceFront;
   const G4double ThirdStageFaceBack  = FaceBack;

   // Starting at the front and going to CsI
   const G4double AluStripFront_PosZ = Length* -0.5 + 0.5*AluStripThickness                              ;
   const G4double Silicon_PosZ       = AluStripFront_PosZ + 0.5*AluStripThickness + 0.5*SiliconThickness ;
   const G4double AluStripBack_PosZ  = Silicon_PosZ + 0.5*SiliconThickness + 0.5*AluStripThickness       ;
   const G4double VacBox_PosZ        = AluStripBack_PosZ + 0.5*AluStripThickness + 0.5* VacBoxThickness  ;
   const G4double ThirdStage_PosZ    = VacBox_PosZ + 0.5*VacBoxThickness + 0.5*ThirdStageThickness       ;
}

#endif
