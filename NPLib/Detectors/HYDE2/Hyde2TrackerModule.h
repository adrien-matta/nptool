#ifndef Hyde2TrackerModule_h
#define Hyde2TrackerModule_h 1

// C++ headers
#include <string>
#include <map>

// NPTool - ROOT headers
#include "THyde2TrackerData.h"

using namespace std;



class Hyde2TrackerModule
{
public:
   Hyde2TrackerModule();
   virtual ~Hyde2TrackerModule();

public:
   // Read stream at Configfile to pick-up parameters of detector (Position,...)
   virtual void ReadConfiguration(string Path) = 0;

   // The goal of this method is to extract physical parameters from raw data
   // Method called at each event read from the Input Tree
   virtual void BuildPhysicalEvent() = 0;

   // Same as before but with a simpler treatment
   virtual void BuildSimplePhysicalEvent() = 0;

   // Initialize the Index map for the different modules of Hyde2
   void InitializeIndex();

   // Pass the THyde2TrackerData object from THyde2TrackerPhysics to the 
   // classes deriving from Hyde2TrackerModule
   virtual void SetHyde2DataPointer(THyde2TrackerData* hyde2Data) = 0;

   //
   virtual double GetStripPositionX(Int_t DetectorNumber, Int_t stripX, Int_t stripY) = 0;
   virtual double GetStripPositionY(Int_t DetectorNumber, Int_t stripX, Int_t stripY) = 0;
   virtual double GetStripPositionZ(Int_t DetectorNumber, Int_t stripX, Int_t stripY) = 0;

protected:
   map<string, int> m_index;
};

#endif
