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
 * Decription: This class is an Abstract Base Class (ABC) from which should  *
 *             derive all different modules from the Hyde2 tracker.        *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef Hyde2TrackerModule_h
#define Hyde2TrackerModule_h 1

// C++ headers
#include <string>
#include <vector>

// G4 headers
#include "G4LogicalVolume.hh"
#include "G4Event.hh"
#include "G4MultiFunctionalDetector.hh"

// NPTool - ROOT headers
#include "TInteractionCoordinates.h"
#include "THyde2TrackerData.h"

using namespace std;



class Hyde2TrackerModule
{
public:
   Hyde2TrackerModule();
   virtual ~Hyde2TrackerModule();

public:
   // Read stream at Configfile to pick-up parameters of detector (Position,...)
   virtual void ReadConfiguration(string Path) = 0;

   // Construct detector and inialise sensitive part.
   virtual void ConstructDetector(G4LogicalVolume* world) = 0;

   // Read sensitive part of a the Hyde2TrackerModule detector and fill the Root tree.
   virtual void ReadSensitive(const G4Event* event) = 0;

   // Add Detector branch to the ROOT output tree
   virtual void InitializeRootOutput();

   // Initialize all scorers necessary for each detector
   virtual void InitializeScorers() = 0;

   // Give the static TInteractionCoordinates from VDetector to the classes
   // deriving from Hyde2TrackerModule
   // This is mandatory since the Hyde2Tracker*** does not derive from VDetector
   virtual void SetInterCoordPointer(TInteractionCoordinates* interCoord) = 0;
   virtual TInteractionCoordinates* GetInterCoordPointer() = 0;

   // Initialize the Index map for the different modules of Hyde2
   void InitializeIndex();

public:
   THyde2TrackerData*      GetEventPointer()    {return ms_Event;};

protected:
   // Class to store the result of simulation
   static THyde2TrackerData*      ms_Event;

   // Set to true if you want this stage on you telescope
   vector<bool>      m_wFirstStage;
   vector<bool>      m_wSecondStage;
   vector<bool>      m_wThirdStage;
   vector<bool>      m_wFourthStage;
   vector<bool>      m_wFifthStage;
   vector<bool>      m_wSixthStage;

   // Set to true if you want to see Telescope Frame in your visualisation
   bool              m_non_sensitive_part_visiualisation;

protected:
   // First stage Associate Scorer
   G4MultiFunctionalDetector* m_FirstStageScorer;

   // Second stage Associate Scorer
   G4MultiFunctionalDetector* m_SecondStageScorer;

   // Third stage Associate Scorer 
   G4MultiFunctionalDetector* m_ThirdStageScorer;

   // Fourth stage Associate Scorer 
   G4MultiFunctionalDetector* m_FourthStageScorer;

   // Fifth stage Associate Scorer 
   G4MultiFunctionalDetector* m_FifthStageScorer;

   // Sixth stage Associate Scorer 
   G4MultiFunctionalDetector* m_SixthStageScorer;

protected:
   map<string, int> m_index;
};

#endif
