#ifndef GaspardTrackerAnnular_h
#define GaspardTrackerAnnular_h 1
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
 * Decription: Define a module of annular shape for the Gaspard tracker      *
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



class GaspardTrackerAnnular : public GaspardTrackerModule
{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   GaspardTrackerAnnular();
   virtual ~GaspardTrackerAnnular();

   ////////////////////////////////////////////////////
   //////// Specific Function of this Class ///////////
   ////////////////////////////////////////////////////
public:
   // By Position Method
   void AddModule(G4double PosZ,
                  G4double Rmin,
                  G4double Rmax,
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

   // Used for "By Point Definition"
   vector<G4double> m_PosZ;
   vector<G4double> m_Rmin;
   vector<G4double> m_Rmax;

   // for debugging purpose
   //G4ThreeVector   MMpos;
   G4ThreeVector   MMu;
   G4ThreeVector   MMv;
   G4ThreeVector   MMw;
   G4ThreeVector   CT;
};



namespace GPDANNULAR
{
   // Energy/Time resolutions for the different layers
   const G4double ResoFirstStage  = 0.021          ;// = 52keV of Resolution   //   Unit is MeV/2.35
   const G4double ResoSecondStage = 0.043          ;// = 100 keV of resolution //   Unit is MeV/2.35
   const G4double ResoThirdStage  = 0.043          ;// = 100 kev of resolution //   Unit is MeV/2.35
   const G4double ResoTimeGpd     = 0.212765957    ;// = 500ps                 //   Unit is  ns/2.35
   const G4double ResoTimePPAC    = 0.106382979    ;// = 250ps                 //   Unit is  ns/2.35

   // Threshold
   const G4double EnergyThreshold = 0.2*MeV;

   // Geometry
   const G4double Rmin   = 15*mm;
   const G4double Rmax   = 55*mm;
   const G4double Length = 10*mm;

   // First stage
//   const G4double AluStripThickness = 0.00000001*micrometer;
   const G4double AluStripThickness   = 0.4*micrometer;
   const G4double FirstStageThickness = 300*micrometer;
   const G4double FirstStageRmin = 16*mm;
   // MUGAST
   const G4double FirstStageRmax = 45*mm;
   // gaspHyde
//   const G4double FirstStageRmax = 52*mm;
//   const G4double VacBoxThickness   = 3*cm           ;
   const G4double VacBoxThickness     = 0.4*cm;

   // Second stage
   const G4double SecondStageThickness = 1.5*mm;

   // Third stage
   const G4double ThirdStageThickness = 1.5*mm;

   // Characteristics
   const G4int NbPhiStrips     = 16;
   const G4int NbThetaStrips   = 16;

   // Starting at the front and going in direction of third stage
   const G4double AluStripFront_PosZ = Length* -0.5 + 0.5*AluStripThickness;
   const G4double Silicon_PosZ       = AluStripFront_PosZ + 0.5*AluStripThickness + 0.5*FirstStageThickness;
   const G4double AluStripBack_PosZ  = Silicon_PosZ + 0.5*FirstStageThickness + 0.5*AluStripThickness;
   const G4double VacBox_PosZ        = AluStripBack_PosZ + 0.5*AluStripThickness + 0.5* VacBoxThickness;
   const G4double SecondStage_PosZ   = VacBox_PosZ + 0.5*VacBoxThickness + 0.5*SecondStageThickness;
   const G4double ThirdStage_PosZ    = SecondStage_PosZ + 0.5*SecondStageThickness + 0.5*VacBoxThickness + 0.5*ThirdStageThickness;
}

#endif
