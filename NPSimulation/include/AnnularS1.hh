/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 21/07/09                                                 *
 * Last update    : 11/10/09                                                 *
 *---------------------------------------------------------------------------*
 * Decription: Define the S1 detector from Micron                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *  + 11/10/09: Change scorer philosophy, one scorer for the detector number *
 *              added (N. de Sereville)                                      *
 *                                                                           *
 *****************************************************************************/

#ifndef AnnularS1_h
#define AnnularS1_h 1

// C++ headers
#include <vector>

// NPTool header
#include "VDetector.hh"

// NPTool - ROOT headers
#include "TS1Data.h"

using namespace std;



class AnnularS1 : public VDetector
{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   AnnularS1();
   virtual ~AnnularS1();

   ////////////////////////////////////////////////////
   //////// Specific Function of this Class ///////////
   ////////////////////////////////////////////////////
public:
   // By Position Method
   void AddModule(G4double PosZ);

   // Effectively construct Volume
   // Avoid to have two time same code for Angle and Point definition
   void VolumeMaker(G4int             DetecNumber,
                    G4ThreeVector     pos,
                    G4RotationMatrix* rot,
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


   ////////////////////////////////////////////////////
   ///////////Event class to store Data////////////////
   ////////////////////////////////////////////////////
private:
   TS1Data* m_Event;


private:
   // Scorer
   G4MultiFunctionalDetector* m_Scorer;


   ////////////////////////////////////////////////////
   ///////////////Private intern Data//////////////////
   ////////////////////////////////////////////////////
private:
   // Used for "By Point Definition"
   vector<G4double>	m_PosZ;

   // Set to true if you want to see Telescope Frame in your visualisation
   bool			m_non_sensitive_part_visiualisation;
};



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace ANNULARS1
{
   // Energy/Time resolutions for the different layers
   const G4double ResoFirstStage  = 0              ;// = 52keV of Resolution   //   Unit is MeV/2.35
//   const G4double ResoFirstStage  = 0.022          ;// = 52keV of Resolution   //   Unit is MeV/2.35
   const G4double ResoSecondStage = 0.055          ;// = 130 keV of resolution //   Unit is MeV/2.35
   const G4double ResoThirdStage  = 0              ;// = 100 keV of resolution //   Unit is MeV/2.35
//   const G4double ResoThirdStage  = 0.043          ;// = 100 kev of resolution //   Unit is MeV/2.35
   const G4double ResoTimeGpd     = 0.212765957    ;// = 500ps                 //   Unit is  ns/2.35
   const G4double ResoTimePPAC    = 0.106382979    ;// = 250ps                 //   Unit is  ns/2.35

   // Geometry
   const G4double Rmin   =  46/2*mm;
   const G4double Rmax   = 100/2*mm;
   const G4double Length = 10*mm;

   // First stage
//   const G4double AluStripThickness = 0.00000001*micrometer;
   const G4double AluStripThickness   = 0.4*micrometer;
   const G4double FirstStageThickness = 500*micrometer;
   const G4double FirstStageRmin      = 48/2*mm;
   const G4double FirstStageRmax      = 96/2*mm;

   // Characteristics
   const G4int NbPhiStrips     = 16;
   const G4int NbThetaStrips   = 16;
   const G4int NbThetaQuadrant = 4;

   // Starting at the front and going in direction of third stage
   const G4double AluStripFront_PosZ = Length* -0.5       + 0.5*AluStripThickness;
   const G4double Silicon_PosZ       = AluStripFront_PosZ + 0.5*AluStripThickness   + 0.5*FirstStageThickness;
   const G4double AluStripBack_PosZ  = Silicon_PosZ       + 0.5*FirstStageThickness + 0.5*AluStripThickness;
}

#endif
