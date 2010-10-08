#ifndef GaspardTrackerTrapezoid_h
#define GaspardTrackerTrapezoid_h 1

// C++ headers
#include <string>
#include <map>
#include <vector>

// NPTool - ROOT headers
#include "TInteractionCoordinates.h"
#include "TGaspardTrackerData.h"
#include "TVector3.h"

// Gaspard headers
#include "GaspardTrackerModule.h"

using namespace std;



class GaspardTrackerTrapezoid : public GaspardTrackerModule
{
public:
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
   GaspardTrackerTrapezoid(map<int, GaspardTrackerModule*> &Module);
   virtual ~GaspardTrackerTrapezoid();

public:
   ////////////////////////////////////////////////////
   ////  Inherite from GaspardTrackerModule class /////
   ////////////////////////////////////////////////////
   // Read stream at Configfile to pick-up parameters of detector (Position,...)
   void ReadConfiguration(string Path);

   // The goal of this method is to extract physical parameters from raw data
   // Method called at each event read from the Input Tree
   void BuildPhysicalEvent();

   // Same as before but with a simpler treatment
   void BuildSimplePhysicalEvent();

private:
   map<int, GaspardTrackerModule*> &m_ModuleTest;

public:
   void SetGaspardDataPointer(TGaspardTrackerData* gaspardData) {m_EventData = gaspardData;};

private:
   // Gaspard data coming from TGaspardTrackerPhysics through the 
   // SetGaspardDataPointer method
   TGaspardTrackerData* m_EventData;

public:
   ////////////////////////////////
   // Specific to GaspardTracker //
   ////////////////////////////////
   // Add a Module using Corner Coordinate information
   void AddModule(TVector3 C_X1_Y1,
                  TVector3 C_X128_Y1,
                  TVector3 C_X1_Y128,
                  TVector3 C_X128_Y128);

   // Add a Module using R Theta Phi of Si center information
   void AddModule(double theta,
                  double phi,
                  double distance,
                  double beta_u,
                  double beta_v,
                  double beta_w);

   // Getters to retrieve the (X,Y,Z) coordinates of a pixel defined by strips (X,Y)
   double GetStripPositionX(int N ,int X ,int Y)        { return m_StripPositionX[N-1][X-1][Y-1]; }
   double GetStripPositionY(int N ,int X ,int Y)        { return m_StripPositionY[N-1][X-1][Y-1]; }
   double GetStripPositionZ(int N ,int X ,int Y)        { return m_StripPositionZ[N-1][X-1][Y-1]; }
   double GetNumberOfModule()                           { return m_NumberOfModule; }

private:
   // Spatial Position of Strip Calculated on basis of detector position
   int m_NumberOfModule;
   vector< vector < vector < double > > >       m_StripPositionX;
   vector< vector < vector < double > > >       m_StripPositionY;
   vector< vector < vector < double > > >       m_StripPositionZ;
};

#endif
