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
#include "HeliosDetDummyShape.hh"
#include "NPSDetectorFactory.hh"
#include "NPOptionManager.h"
using namespace std;


Helios::Helios()
{
}



Helios::~Helios()
{
}



// Read stream at Configfile to pick-up parameters of detector (Position,...)
// Called in DetecorConstruction::ReadDetextorConfiguration Method
void Helios::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("HeliosDummyShape");
  if(blocks.size()>1){
    cout << "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww " << endl;
     // instantiate a new "detector" corresponding to the Shape elements
         // The HeliosSquare class should be replaced by the
         // HeliosShape class you need to define
         HeliosModule* myDetector = new HeliosDetDummyShape();

         // read part of the configuration file corresponding to shape elements
         myDetector->ReadConfiguration(parser);

         // ms_InterCoord comes from NPS::VDetector
         myDetector->SetInterCoordPointer(ms_InterCoord);

         // store HeliosShape "detector"
         m_Modules.push_back(myDetector);
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
 ////////////////////////////////////////////////////////////////////////////////
 //            Construct Method to be pass to the DetectorFactory              //
 ////////////////////////////////////////////////////////////////////////////////
 NPS::VDetector* Helios::Construct(){
  return  (NPS::VDetector*) new Helios();
 }

 ////////////////////////////////////////////////////////////////////////////////
 //            Registering the construct method to the factory                 //
 ////////////////////////////////////////////////////////////////////////////////
 extern"C" {
 class proxy{
   public:
    proxy(){
      NPS::DetectorFactory::getInstance()->AddToken("HeliosDummyShape","Helios");
      NPS::DetectorFactory::getInstance()->AddDetector("HeliosDummyShape",Helios::Construct);
    }
};

 proxy p;
 }
