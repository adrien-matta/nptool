#ifndef GaspardTracker_h
#define GaspardTracker_h 1

// C++ headers
#include <vector>

// NPTool header
#include "VDetector.hh"
#include "GaspardTrackerModule.hh"

using namespace std;



class GaspardTracker : public VDetector
{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   GaspardTracker();
   virtual ~GaspardTracker();

   ////////////////////////////////////////////////////
   /////////  Inherite from VDetector class ///////////
   ////////////////////////////////////////////////////
public:
   // Read stream at Configfile to pick-up parameters of detector (Position,...)
   // Called in DetecorConstruction::ReadDetextorConfiguration Method
   void ReadConfiguration(string Path);

   // Construct detector and inialise sensitive part.
   // Called After DetecorConstruction::AddDetector Method
   void ConstructDetector(G4LogicalVolume* world);

   // Add Detector branch to the EventTree.
   // Called After DetecorConstruction::AddDetector Method
   void InitializeRootOutput();

   // Read sensitive part and fill the Root tree.
   // Called at in the EventAction::EndOfEventAvtion
   void ReadSensitive(const G4Event* event);

public:
   // Initialize all scorers necessary for each detector
   void InitializeScorers();

private:
   vector<GaspardTrackerModule*> m_Modules;
};
#endif
