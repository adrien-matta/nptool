#ifndef TGaspardTrackerModule_h
#define TGaspardTrackerModule_h 1

// C++ headers
#include <string>
#include <map>

// NPTool - ROOT headers
#include "TGaspardTrackerData.h"

using namespace std;



class TGaspardTrackerModule
{
public:
   TGaspardTrackerModule();
   virtual ~TGaspardTrackerModule();

public:
   // Read stream at Configfile to pick-up parameters of detector (Position,...)
   virtual void ReadConfiguration(string Path) = 0;

   // The goal of this method is to extract physical parameters from raw data
   // Method called at each event read from the Input Tree
   virtual void BuildPhysicalEvent() = 0;

   // Same as before but with a simpler treatment
   virtual void BuildSimplePhysicalEvent() = 0;

   // Initialize the Index map for the different modules of Gaspard
   void InitializeIndex();

protected:
   map<string, int> m_index;
};

#endif
