/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche    contact address: marc.labiche@stfc.ac.uk *
 *                                                                           *
 * Creation Date  : 31/01/12                                                 *
 * Last update    : 31/08/15                                                 *
 *---------------------------------------------------------------------------*
 * Decription: This class is an Abstract Base Class (ABC) from which should  *
 *             derive all different modules from the Helios detector.        *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef HeliosModule_h
#define HeliosModule_h 1

// C++ headers
#include <string>
#include <vector>

// G4 headers
#include "G4LogicalVolume.hh"
#include "G4Event.hh"
#include "G4MultiFunctionalDetector.hh"

// NPTool - ROOT headers
#include "TInteractionCoordinates.h"
#include "THeliosData.h"
#include "NPInputParser.h"
using namespace std;
using namespace CLHEP;



class HeliosModule
{
public:
   HeliosModule();
   virtual ~HeliosModule();

public:
   // Read stream at Configfile to pick-up parameters of detector (Position,...)
   virtual void ReadConfiguration(NPL::InputParser) = 0;

   // Construct detector and inialise sensitive part.
   virtual void ConstructDetector(G4LogicalVolume* world) = 0;

   // Read sensitive part of a the HeliosModule detector and fill the Root tree.
   virtual void ReadSensitive(const G4Event* event) = 0;

   // Add Detector branch to the ROOT output tree
   virtual void InitializeRootOutput();

   // Initialize all scorers necessary for each detector
   virtual void InitializeScorers() = 0;

   // Give the static TInteractionCoordinates from NPS::VDetector to the classes
   // deriving from HeliosModule
   // This is mandatory since the Helios*** does not derive from NPS::VDetector
   virtual void SetInterCoordPointer(TInteractionCoordinates* interCoord) = 0;
   virtual TInteractionCoordinates* GetInterCoordPointer() = 0;

   // Initialize the Index map for the different modules of Gaspard
   void InitializeIndex();

public:
   THeliosData*		GetEventPointer() 	{return ms_Event;};

protected:
   // Class to store the result of simulation
   static THeliosData*		ms_Event;

   // Set to true if you want this stage on you telescope
   vector<bool>      m_wFirstStage;

   // Set to true if you want to see Telescope Frame in your visualisation
   bool              m_non_sensitive_part_visiualisation;

protected:
   // First stage Associate Scorer
   G4MultiFunctionalDetector* m_FirstStageScorer;


protected:
   map<string, int> m_index;
};

#endif
