#ifndef GaspardTrackerModule_h
#define GaspardTrackerModule_h 1
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
 * Decription: This class is an Abstract Base Class (ABC) from which should  *
 *             derive all different modules from the Gaspard tracker.        *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/



// C++ headers
#include <string>
#include <vector>

// G4 headers
#include "G4LogicalVolume.hh"
#include "G4Event.hh"
#include "G4MultiFunctionalDetector.hh"

// NPTool - ROOT headers
#include "TInteractionCoordinates.h"
#include "TGaspardTrackerData.h"
#include "NPInputParser.h"
using namespace std;
using namespace CLHEP;

class GaspardTrackerModule
{
public:
   GaspardTrackerModule();
   virtual ~GaspardTrackerModule();

public:
   // Read stream at Configfile to pick-up parameters of detector (Position,...)
   virtual void ReadConfiguration(NPL::InputParser) = 0;

   // Construct detector and inialise sensitive part.
   virtual void ConstructDetector(G4LogicalVolume* world) = 0;

   // Read sensitive part of a the GaspardTrackerModule detector and fill the Root tree.
   virtual void ReadSensitive(const G4Event* event) = 0;

   // Add Detector branch to the ROOT output tree
   virtual void InitializeRootOutput();

   // Initialize all scorers necessary for each detector
   virtual void InitializeScorers() = 0;

   // Give the static TInteractionCoordinates from NPS::VDetector to the classes
   // deriving from GaspardTrackerModule
   // This is mandatory since the GaspardTracker*** does not derive from NPS::VDetector
   virtual void SetInterCoordPointer(TInteractionCoordinates* interCoord) = 0;
   virtual TInteractionCoordinates* GetInterCoordPointer() = 0;

   // Initialize the Index map for the different modules of Gaspard
   void InitializeIndex();

public:
   TGaspardTrackerData*      GetEventPointer()    {return ms_Event;};

protected:
   // Class to store the result of simulation
   static TGaspardTrackerData*      ms_Event;

   // Set to true if you want this stage on you telescope
   vector<bool>      m_wFirstStage;
   vector<bool>      m_wSecondStage;
   vector<bool>      m_wThirdStage;

   // Set to true if you want to see Telescope Frame in your visualisation
   bool              m_non_sensitive_part_visiualisation;

protected:
   ////////////////////////////////////////////////////
   //////////////////// Material //////////////////////
   ////////////////////////////////////////////////////
   //   Declare all material used by Gaspard 
   void InitializeMaterial() ;
   // Si
   G4Material* m_MaterialSilicon;
   //  Vacuum
   G4Material* m_MaterialVacuum ;

protected:
   // First stage Associate Scorer
   G4MultiFunctionalDetector* m_FirstStageScorer;

   // Second stage Associate Scorer
   G4MultiFunctionalDetector* m_SecondStageScorer;

   // Third stage Associate Scorer 
   G4MultiFunctionalDetector* m_ThirdStageScorer;

protected:
   map<string, int> m_index;
};

#endif
