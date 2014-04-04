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
 * Decription: This class manages different shapes of module for the ANU  *
 *             It allows to have ANU geometries with an                   *
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
#include "ANU.hh"
//#include "ANUDetSquare.hh"
//#include "ANUDetTrapezoid.hh"
//#include "ANUDetAnnular.hh"
#include "ANUDetDummyShape.hh"

using namespace std;


ANU::ANU()
{
}



ANU::~ANU()
{
}



// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void ANU::ReadConfiguration(string Path)
{
   // open configuration file
   ifstream ConfigFile;
   ConfigFile.open(Path.c_str());

   //   bool ANUSquare     = false;
   //   bool ANUTrapezoid  = false;
   //   bool ANUAnnular    = false;
   bool ANUDummyShape = false;

   string LineBuffer;
   while (!ConfigFile.eof()) {
      getline(ConfigFile, LineBuffer);
      /*
      if (LineBuffer.compare(0, 12, "ANUSquare") == 0  &&  ANUSquare == false) {
         ANUSquare = true;

         // instantiate a new "detector" corresponding to the Square elements
         ANUModule* myDetector = new ANUSquare();

         // read part of the configuration file corresponding to square elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store ANUSquare "detector"
         m_Modules.push_back(myDetector);
      }
      else if (LineBuffer.compare(0, 15, "ANUTrapezoid") == 0  &&  ANUTrapezoid == false) {
         ANUTrapezoid = true;

         // instantiate a new "detector" corresponding to the Trapezoid elements
         ANUModule* myDetector = new ANUTrapezoid();

         // read part of the configuration file corresponding to trapezoid elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store ANUTrapezoid "detector"
         m_Modules.push_back(myDetector);
      }
      else if (LineBuffer.compare(0, 13, "ANUAnnular") == 0  &&  ANUAnnular == false) {
         ANUAnnular = true;

         // instantiate a new "detector" corresponding to the Trapezoid elements
         ANUModule* myDetector = new ANUAnnular();

         // read part of the configuration file corresponding to trapezoid elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store ANUTrapezoid "detector"
         m_Modules.push_back(myDetector);
      }
      else 
      */
      if (LineBuffer.compare(0, 16, "ANUDummyShape") == 0  &&  ANUDummyShape == false) {
         ANUDummyShape = true;

         // instantiate a new "detector" corresponding to the Shape elements
         // The ANUSquare class should be replaced by the
         // ANUShape class you need to define
         ANUModule* myDetector = new ANUDetDummyShape();

         // read part of the configuration file corresponding to shape elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store ANUShape "detector"
         m_Modules.push_back(myDetector);
      }
   }
}



// Construct detector and initialize sensitive part.
// Called After DetecorConstruction::AddDetector Method
void ANU::ConstructDetector(G4LogicalVolume* world)
{
   // loop on sub-detectors belonging to ANU
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->ConstructDetector(world);
}



// Connect the GaspardTrackingData class to the output TTree
// of the simulation
void ANU::InitializeRootOutput()
{
   // loop on sub-detectors belonging to ANU
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->InitializeRootOutput();
}



// Initialize all scorers necessary for each detector
void ANU::InitializeScorers()
{
   // loop on sub-detectors belonging to ANU
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->InitializeScorers();
}



// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAction
void ANU::ReadSensitive(const G4Event* event)
{
   // Before looping on each sub-detector, clear the static variable
   // ms_InterCoord
   // This is done on the first element of the m_Modules vector.
   // This should be done here since this variable (of type TIneractionCoordinates)
   // deals with multiplicity of events > 1.
   m_Modules[0]->GetInterCoordPointer()->Clear();

   // We do the same for the static variable ms_Event
   m_Modules[0]->GetEventPointer()->Clear();

   // loop on sub-detectors belonging to ANU
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->ReadSensitive(event);
}
