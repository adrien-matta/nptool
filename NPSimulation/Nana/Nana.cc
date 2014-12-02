/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche    contact address: marc.labiche@stfc.ac.uk   *
 *                                                                           *
 * Creation Date  : 04/01/13                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class manages different shapes of module for the Nana   *
 *             detector. It allows to have Nana geometries with an          *
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
#include "Nana.hh"
#include "NanaDetector.hh"

using namespace std;


Nana::Nana()
{
}



Nana::~Nana()
{
}



// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Nana::ReadConfiguration(string Path)
{
   // open configuration file
   ifstream ConfigFile;
   ConfigFile.open(Path.c_str());

   bool bNanaDetector     = false;

   string LineBuffer;
   while (!ConfigFile.eof()) {
      getline(ConfigFile, LineBuffer);
      if (LineBuffer.compare(0, 12, "NanaDetector") == 0  &&  bNanaDetector == false) {
         bNanaDetector = true;

         // instantiate a new "detector" corresponding to the Square elements
         NanaModule* myDetector = new NanaDetector();

         // read part of the configuration file corresponding to square elements
         ConfigFile.close();
         myDetector->ReadConfiguration(Path);
         ConfigFile.open(Path.c_str());

         // ms_InterCoord comes from VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store NanaDetector "detector"
         m_Modules.push_back(myDetector);
      }

   }
}



// Construct detector and initialize sensitive part.
// Called After DetectorConstruction::AddDetector Method
void Nana::ConstructDetector(G4LogicalVolume* world)
{
   // loop on sub-detectors belonging to Nana
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->ConstructDetector(world);
}



// Connect the NanaData class to the output TTree
// of the simulation
void Nana::InitializeRootOutput()
{
   // loop on sub-detectors belonging to Nana
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->InitializeRootOutput();
}



// Initialize all scorers necessary for each detector
void Nana::InitializeScorers()
{
   // loop on sub-detectors belonging to Nana
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->InitializeScorers();
}



// Read sensitive part and fill the Root tree.
// Called at in the EventAction::EndOfEventAction
void Nana::ReadSensitive(const G4Event* event)
{
   // Before looping on each sub-detector, clear the static variable
   // ms_InterCoord
   // This is done on the first element of the m_Modules vector.
   // This should be done here since this variable (of type TIneractionCoordinates)
   // deals with multiplicity of events > 1.
   m_Modules[0]->GetInterCoordPointer()->Clear();

   // We do the same for the static variable ms_Event
   m_Modules[0]->GetEventPointer()->Clear();

   // loop on sub-detectors belonging to Nana 
   int nbDetectors = m_Modules.size();
   for (int i = 0; i < nbDetectors; i++) m_Modules[i]->ReadSensitive(event);
}
