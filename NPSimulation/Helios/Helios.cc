/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche    contact address: marc.labiche@stfc.ac.uk *
 *                                                                           *
 * Creation Date  : 31/01/12                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class manages different shapes of module for the Helios  *
 *             It allows to have Helios geometries with an                   *
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
#include "Helios.hh"
//#include "HeliosDetSquare.hh"
//#include "HeliosDetTrapezoid.hh"
//#include "HeliosDetAnnular.hh"
#include "HeliosDetDummyShape.hh"

using namespace std;


Helios::Helios()
{
}



Helios::~Helios()
{
}



// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Helios::ReadConfiguration(string Path)
{
   // open configuration file
   ifstream ConfigFile;
   ConfigFile.open(Path.c_str());

   //   bool HeliosSquare     = false;
   //   bool HeliosTrapezoid  = false;
   //   bool HeliosAnnular    = false;
   bool HeliosDummyShape = false;

   string LineBuffer;
   while (!ConfigFile.eof()) {
      getline(ConfigFile, LineBuffer);
      /*
      if (LineBuffer.compare(0, 12, "HeliosSquare") == 0  &&  HeliosSquare == false) {
         HeliosSquare = true;

         // instantiate a new "detector" corresponding to the Square elements
         HeliosModule* myDetector = new HeliosSquare();

         // read part of the configuration file corresponding to square elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from NPS::VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store HeliosSquare "detector"
         m_Modules.push_back(myDetector);
      }
      else if (LineBuffer.compare(0, 15, "HeliosTrapezoid") == 0  &&  HeliosTrapezoid == false) {
         HeliosTrapezoid = true;

         // instantiate a new "detector" corresponding to the Trapezoid elements
         HeliosModule* myDetector = new HeliosTrapezoid();

         // read part of the configuration file corresponding to trapezoid elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from NPS::VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store HeliosTrapezoid "detector"
         m_Modules.push_back(myDetector);
      }
      else if (LineBuffer.compare(0, 13, "HeliosAnnular") == 0  &&  HeliosAnnular == false) {
         HeliosAnnular = true;

         // instantiate a new "detector" corresponding to the Trapezoid elements
         HeliosModule* myDetector = new HeliosAnnular();

         // read part of the configuration file corresponding to trapezoid elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from NPS::VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store HeliosTrapezoid "detector"
         m_Modules.push_back(myDetector);
      }
      else 
      */
      if (LineBuffer.compare(0, 16, "HeliosDummyShape") == 0  &&  HeliosDummyShape == false) {
         HeliosDummyShape = true;

         // instantiate a new "detector" corresponding to the Shape elements
         // The HeliosSquare class should be replaced by the
         // HeliosShape class you need to define
         HeliosModule* myDetector = new HeliosDetDummyShape();

         // read part of the configuration file corresponding to shape elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from NPS::VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store HeliosShape "detector"
         m_Modules.push_back(myDetector);
      }
   }
}



// Construct detector and initialize sensitive part.
// Called After DetecorConstruction::AddDetector Method
void Helios::ConstructDetector(G4LogicalVolume* world)
{
   // loop on sub-detectors belonging to Helios
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->ConstructDetector(world);
}



// Connect the GaspardTrackingData class to the output TTree
// of the simulation
void Helios::InitializeRootOutput()
{
   // loop on sub-detectors belonging to Helios
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->InitializeRootOutput();
}



// Initialize all scorers necessary for each detector
void Helios::InitializeScorers()
{
   // loop on sub-detectors belonging to Helios
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->InitializeScorers();
}



// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAction
void Helios::ReadSensitive(const G4Event* event)
{
   // Before looping on each sub-detector, clear the static variable
   // ms_InterCoord
   // This is done on the first element of the m_Modules vector.
   // This should be done here since this variable (of type TIneractionCoordinates)
   // deals with multiplicity of events > 1.
   m_Modules[0]->GetInterCoordPointer()->Clear();

   // We do the same for the static variable ms_Event
   m_Modules[0]->GetEventPointer()->Clear();

   // loop on sub-detectors belonging to Helios
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->ReadSensitive(event);
}
