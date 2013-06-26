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
 * Decription: This class manages different shapes of module for the Hyde2 *
 *             tracker. It allows to have Hyde2 geometries with an         *
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
#include "Hyde2Tracker.hh"
#include "Hyde2TrackerSquare1.hh"
#include "Hyde2TrackerTrapezoid1.hh"
#include "Hyde2TrackerTrapezoid2.hh"

using namespace std;


Hyde2Tracker::Hyde2Tracker()
{
}



Hyde2Tracker::~Hyde2Tracker()
{
}



// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Hyde2Tracker::ReadConfiguration(string Path)
{
   // open configuration file
   ifstream ConfigFile;
   ConfigFile.open(Path.c_str());

   bool HYD2TrkSquare1     = false;
   bool HYD2TrkTrapezoid1  = false;
   bool HYD2TrkTrapezoid2  = false;

   string LineBuffer;
   while (!ConfigFile.eof()) {
      getline(ConfigFile, LineBuffer);
      if (LineBuffer.compare(0, 11, "HYD2Square1") == 0  &&  HYD2TrkSquare1 == false) {
         HYD2TrkSquare1 = true;

         // instantiate a new "detector" corresponding to the Square1 elements
         Hyde2TrackerModule* myDetector = new Hyde2TrackerSquare1();

         // read part of the configuration file corresponding to square elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store Hyde2TrackerSquare1 "detector"
         m_Modules.push_back(myDetector);
      }
      else if (LineBuffer.compare(0, 14, "HYD2Trapezoid1") == 0  &&  HYD2TrkTrapezoid1 == false) {
         HYD2TrkTrapezoid1 = true;

         // instantiate a new "detector" corresponding to the Trapezoid1 elements
         Hyde2TrackerModule* myDetector = new Hyde2TrackerTrapezoid1();

         // read part of the configuration file corresponding to trapezoid elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store Hyde2TrackerTrapezoid1 "detector"
         m_Modules.push_back(myDetector);
      }
      else if (LineBuffer.compare(0, 14, "HYD2Trapezoid2") == 0  &&  HYD2TrkTrapezoid2 == false) {
         HYD2TrkTrapezoid2 = true;

         // instantiate a new "detector" corresponding to the Trapezoid2 elements
         Hyde2TrackerModule* myDetector = new Hyde2TrackerTrapezoid2();

         // read part of the configuration file corresponding to trapezoid elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store Hyde2TrackerTrapezoid2 "detector"
         m_Modules.push_back(myDetector);
      }
   }
}



// Construct detector and initialize sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Hyde2Tracker::ConstructDetector(G4LogicalVolume* world)
{
   // loop on sub-detectors belonging to Hyde2Tracker
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->ConstructDetector(world);
}



// Connect the Hyde2TrackingData class to the output TTree
// of the simulation
void Hyde2Tracker::InitializeRootOutput()
{
   // loop on sub-detectors belonging to Hyde2Tracker
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->InitializeRootOutput();
}



// Initialize all scorers necessary for each detector
void Hyde2Tracker::InitializeScorers()
{
   // loop on sub-detectors belonging to Hyde2Tracker
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->InitializeScorers();
}



// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAction
void Hyde2Tracker::ReadSensitive(const G4Event* event)
{
   // Before looping on each sub-detector, clear the static variable
   // ms_InterCoord
   // This is done on the first element of the m_Modules vector.
   // This should be done here since this variable (of type TIneractionCoordinates)
   // deals with multiplicity of events > 1.
   m_Modules[0]->GetInterCoordPointer()->Clear();

   // We do the same for the static variable ms_Event
   m_Modules[0]->GetEventPointer()->Clear();

   // loop on sub-detectors belonging to Hyde2Tracker
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->ReadSensitive(event);
}
