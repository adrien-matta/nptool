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
 * Decription: This class manages different shapes of module for the Gaspard *
 *             tracker. It allows to have Gaspard geometries with an         *
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
#include "GaspardTracker.hh"
#include "GaspardTrackerSquare.hh"
#include "GaspardTrackerRectangle.hh"
#include "GaspardTrackerTrapezoid.hh"
#include "GaspardTrackerAnnular.hh"
#include "GaspardTrackerDummyShape.hh"
#include "NPSDetectorFactory.hh"
using namespace std;


GaspardTracker::GaspardTracker()
{
}



GaspardTracker::~GaspardTracker()
{
}



// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void GaspardTracker::ReadConfiguration(string Path)
{
   // open configuration file
   ifstream ConfigFile;
   ConfigFile.open(Path.c_str());

   bool GPDTrkSquare     = false;
   bool GPDTrkRectangle  = false;
   bool GPDTrkTrapezoid  = false;
   bool GPDTrkAnnular    = false;
   bool GPDTrkDummyShape = false;

   string LineBuffer;
   while (!ConfigFile.eof()) {
      getline(ConfigFile, LineBuffer);
      if (LineBuffer.compare(0, 9, "GPDSquare") == 0  &&  GPDTrkSquare == false) {
         GPDTrkSquare = true;

         // instantiate a new "detector" corresponding to the Square elements
         GaspardTrackerModule* myDetector = new GaspardTrackerSquare();

         // read part of the configuration file corresponding to square elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from NPS::VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store GaspardTrackerSquare "detector"
         m_Modules.push_back(myDetector);
      }
      else if (LineBuffer.compare(0, 12, "GPDRectangle") == 0  &&  GPDTrkRectangle == false) {
         GPDTrkRectangle = true;

         // instantiate a new "detector" corresponding to the Square elements
         GaspardTrackerModule* myDetector = new GaspardTrackerRectangle();

         // read part of the configuration file corresponding to square elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from NPS::VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store GaspardTrackerSquare "detector"
         m_Modules.push_back(myDetector);
      }
      else if (LineBuffer.compare(0, 12, "GPDTrapezoid") == 0  &&  GPDTrkTrapezoid == false) {
         GPDTrkTrapezoid = true;

         // instantiate a new "detector" corresponding to the Trapezoid elements
         GaspardTrackerModule* myDetector = new GaspardTrackerTrapezoid();

         // read part of the configuration file corresponding to trapezoid elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from NPS::VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store GaspardTrackerTrapezoid "detector"
         m_Modules.push_back(myDetector);
      }
      else if (LineBuffer.compare(0, 10, "GPDAnnular") == 0  &&  GPDTrkAnnular == false) {
         GPDTrkAnnular = true;

         // instantiate a new "detector" corresponding to the Trapezoid elements
         GaspardTrackerModule* myDetector = new GaspardTrackerAnnular();

         // read part of the configuration file corresponding to trapezoid elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from NPS::VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store GaspardTrackerTrapezoid "detector"
         m_Modules.push_back(myDetector);
      }
      else if (LineBuffer.compare(0, 13, "GPDDummyShape") == 0  &&  GPDTrkDummyShape == false) {
         GPDTrkDummyShape = true;

         // instantiate a new "detector" corresponding to the Shape elements
         // The GaspardTrackerSquare class should be replaced by the
         // GaspardTrackerShape class you need to define
         GaspardTrackerModule* myDetector = new GaspardTrackerDummyShape();

         // read part of the configuration file corresponding to shape elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from NPS::VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store GaspardTrackerShape "detector"
         m_Modules.push_back(myDetector);
      }
   }
}



// Construct detector and initialize sensitive part.
// Called After DetecorConstruction::AddDetector Method
void GaspardTracker::ConstructDetector(G4LogicalVolume* world)
{
   // loop on sub-detectors belonging to GaspardTracker
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->ConstructDetector(world);
}



// Connect the GaspardTrackingData class to the output TTree
// of the simulation
void GaspardTracker::InitializeRootOutput()
{
   // loop on sub-detectors belonging to GaspardTracker
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->InitializeRootOutput();
}



// Initialize all scorers necessary for each detector
void GaspardTracker::InitializeScorers()
{
   // loop on sub-detectors belonging to GaspardTracker
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->InitializeScorers();
}



// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAction
void GaspardTracker::ReadSensitive(const G4Event* event)
{
   // Before looping on each sub-detector, clear the static variable
   // ms_InterCoord
   // This is done on the first element of the m_Modules vector.
   // This should be done here since this variable (of type TIneractionCoordinates)
   // deals with multiplicity of events > 1.
   m_Modules[0]->GetInterCoordPointer()->Clear();

   // We do the same for the static variable ms_Event
   m_Modules[0]->GetEventPointer()->Clear();

   // loop on sub-detectors belonging to GaspardTracker
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->ReadSensitive(event);
}
 ////////////////////////////////////////////////////////////////////////////////
 //            Construct Method to be pass to the DetectorFactory              //
 ////////////////////////////////////////////////////////////////////////////////
 NPS::VDetector* GaspardTracker::Construct(){
  return  (NPS::VDetector*) new GaspardTracker();
 }

 ////////////////////////////////////////////////////////////////////////////////
 //            Registering the construct method to the factory                 //
 ////////////////////////////////////////////////////////////////////////////////
 extern"C" {
 class proxy_nps_gaspard{
   public:
    proxy_nps_gaspard(){
      NPS::DetectorFactory::getInstance()->AddToken("GaspardTracker","GaspardTracker");
      NPS::DetectorFactory::getInstance()->AddDetector("GaspardTracker",GaspardTracker::Construct);
    }
};

 proxy_nps_gaspard p_nps_gaspard;
 }
