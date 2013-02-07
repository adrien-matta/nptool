/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
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
 * Decription: This class manages different shapes of module for the Hyde *
 *             tracker. It allows to have Hyde geometries with an         *
 *             heterogeneous set of modules                                  *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

// NPTool headers
#include "HydeTracker.hh"
#include "HydeTrackerSquare.hh"
#include "HydeTrackerTrapezoid.hh"
#include "HydeTrackerAnnular.hh"
#include "HydeTrackerDummyShape.hh"

using namespace std;


HydeTracker::HydeTracker()
{
}



HydeTracker::~HydeTracker()
{
}



// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void HydeTracker::ReadConfiguration(string Path)
{
   // open configuration file
   ifstream ConfigFile;
   ConfigFile.open(Path.c_str());

   bool HYDTrkSquare     = false;
   bool HYDTrkTrapezoid  = false;
   bool HYDTrkAnnular    = false;
   bool HYDTrkDummyShape = false;

   string LineBuffer;
   while (!ConfigFile.eof()) {
      getline(ConfigFile, LineBuffer);
      if (LineBuffer.compare(0, 9, "HYDSquare") == 0  &&  HYDTrkSquare == false) {
         HYDTrkSquare = true;

         // instantiate a new "detector" corresponding to the Square elements
         HydeTrackerModule* myDetector = new HydeTrackerSquare();

         // read part of the configuration file corresponding to square elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store HydeTrackerSquare "detector"
         m_Modules.push_back(myDetector);
      }
      else if (LineBuffer.compare(0, 12, "HYDTrapezoid") == 0  &&  HYDTrkTrapezoid == false) {
         HYDTrkTrapezoid = true;

         // instantiate a new "detector" corresponding to the Trapezoid elements
         HydeTrackerModule* myDetector = new HydeTrackerTrapezoid();

         // read part of the configuration file corresponding to trapezoid elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store HydeTrackerTrapezoid "detector"
         m_Modules.push_back(myDetector);
      }
      else if (LineBuffer.compare(0, 10, "HYDAnnular") == 0  &&  HYDTrkAnnular == false) {
         HYDTrkAnnular = true;

         // instantiate a new "detector" corresponding to the Trapezoid elements
         HydeTrackerModule* myDetector = new HydeTrackerAnnular();

         // read part of the configuration file corresponding to trapezoid elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store HydeTrackerTrapezoid "detector"
         m_Modules.push_back(myDetector);
      }
      else if (LineBuffer.compare(0, 13, "HYDDummyShape") == 0  &&  HYDTrkDummyShape == false) {
         HYDTrkDummyShape = true;

         // instantiate a new "detector" corresponding to the Shape elements
         // The HydeTrackerSquare class should be replaced by the
         // HydeTrackerShape class you need to define
         HydeTrackerModule* myDetector = new HydeTrackerDummyShape();

         // read part of the configuration file corresponding to shape elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store HydeTrackerShape "detector"
         m_Modules.push_back(myDetector);
      }
   }
}



// Construct detector and initialize sensitive part.
// Called After DetecorConstruction::AddDetector Method
void HydeTracker::ConstructDetector(G4LogicalVolume* world)
{
   // loop on sub-detectors belonging to HydeTracker
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->ConstructDetector(world);
}



// Connect the HydeTrackingData class to the output TTree
// of the simulation
void HydeTracker::InitializeRootOutput()
{
   // loop on sub-detectors belonging to HydeTracker
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->InitializeRootOutput();
}



// Initialize all scorers necessary for each detector
void HydeTracker::InitializeScorers()
{
   // loop on sub-detectors belonging to HydeTracker
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->InitializeScorers();
}



// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAction
void HydeTracker::ReadSensitive(const G4Event* event)
{
   // Before looping on each sub-detector, clear the static variable
   // ms_InterCoord
   // This is done on the first element of the m_Modules vector.
   // This should be done here since this variable (of type TIneractionCoordinates)
   // deals with multiplicity of events > 1.
   m_Modules[0]->GetInterCoordPointer()->Clear();

   // We do the same for the static variable ms_Event
   m_Modules[0]->GetEventPointer()->Clear();

   // loop on sub-detectors belonging to HydeTracker
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->ReadSensitive(event);
}
