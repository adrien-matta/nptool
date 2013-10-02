#ifndef VDectector_H
#define VDectector_H

// C++ header
#include <string>

// G4 header defining G4 types
#include "globals.hh"

// G4 headers
#include "G4LogicalVolume.hh"
#include "G4Event.hh"

// NPLib header
#include "TInteractionCoordinates.h"

using namespace std;


class VDetector
{
public:
   //  Default Constructor and destructor
   VDetector()   ;
   virtual ~VDetector()   ;

   // Read stream at Configfile to pick-up parameters of detector (Position,...)
   // Called in DetecorConstruction::ReadDetextorConfiguration Method
   virtual void ReadConfiguration(string) = 0;

   // Construct detector and inialise sensitive part.
   // Called After DetecorConstruction::AddDetector Method
   virtual void ConstructDetector(G4LogicalVolume*) = 0;

   // Read sensitive part and fill the Root tree.
   // Called at in the EventAction::EndOfEventAvtion
   virtual void ReadSensitive(const G4Event*) = 0;

   // Add Detector branch to the EventTree.
   // Called After DetecorConstruction::AddDetector Method
   virtual void InitializeRootOutput();
   
   // Add Detector branch to the EventTree.
   // Called After DetecorConstruction::AddDetector Method
   virtual void InitializeScorer() {};

public:
   TInteractionCoordinates*     GetInterCoordPointer()  {return ms_InterCoord;};

protected:
   // Class to store the interaction data (i.e. X,Y,Z,theta,phi)
   static TInteractionCoordinates*      ms_InterCoord;
};

#endif
