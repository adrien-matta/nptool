#ifndef Hyde2TrackerSquare1_h
#define Hyde2TrackerSquare1_h 1

// C++ headers
#include <string>
#include <map>
#include <vector>

// ROOT headers
#include "TVector3.h"

// Hyde2 headers
#include "THyde2TrackerData.h"
#include "THyde2TrackerPhysics.h"
#include "Hyde2TrackerModule.h"

using namespace std;



class Hyde2TrackerSquare1 : public Hyde2TrackerModule
{
public:
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
   Hyde2TrackerSquare1(map<int, Hyde2TrackerModule*> &Module, THyde2TrackerPhysics* &EventPhysics);
   virtual ~Hyde2TrackerSquare1();

public:
   ////////////////////////////////////////////////////
   ////  Inherite from Hyde2TrackerModule class /////
   ////////////////////////////////////////////////////
   // Read stream at Configfile to pick-up parameters of detector (Position,...)
   void ReadConfiguration(string Path);

   // The goal of this method is to extract physical parameters from raw data
   // Method called at each event read from the Input Tree
   void BuildPhysicalEvent();

   // Same as before but with a simpler treatment
   void BuildSimplePhysicalEvent();

private:
   map<int, Hyde2TrackerModule*>  &m_ModuleTest;
   THyde2TrackerPhysics*          &m_EventPhysics;

public:
   void SetHyde2DataPointer(THyde2TrackerData* hyde2Data) {m_EventData = hyde2Data;};
   void PreTreat();

private:
   // Hyde2 data coming from THyde2TrackerPhysics through the 
   // SetHyde2DataPointer method
   THyde2TrackerData* m_EventData;
   THyde2TrackerData* m_PreTreatData;

public:
   ////////////////////////////////
   // Specific to Hyde2Tracker //
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
   double GetStripPositionX(int N ,int X ,int Y)        { return m_StripPositionX[N-1-m_index["Square1"]][X-1][Y-1]; }
   double GetStripPositionY(int N ,int X ,int Y)        { return m_StripPositionY[N-1-m_index["Square1"]][X-1][Y-1]; }
   double GetStripPositionZ(int N ,int X ,int Y)        { return m_StripPositionZ[N-1-m_index["Square1"]][X-1][Y-1]; }
   double GetNumberOfModule()                           { return m_NumberOfModule; }

private:
   // Spatial Position of Strip Calculated on basis of detector position
   int m_NumberOfModule;
   vector< vector < vector < double > > >       m_StripPositionX;
   vector< vector < vector < double > > >       m_StripPositionY;
   vector< vector < vector < double > > >       m_StripPositionZ;

private:
   //////////////////////////////
   // Geometry and stip number //
   //////////////////////////////
   double m_FirstStageFace;     // mm
   int    m_NumberOfStrips;
   double m_StripPitch;
};

#endif
