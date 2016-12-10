#ifndef GaspardTrackerTrapezoid_h
#define GaspardTrackerTrapezoid_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author:    contact address:                                      *
 *                                                                           *
 * Creation Date  :                                                          *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
// C++ headers
#include <string>
#include <map>
#include <vector>

// ROOT headers
#include "TVector3.h"

// Gaspard headers
#include "TGaspardTrackerData.h"
#include "TGaspardTrackerPhysics.h"
#include "GaspardTrackerModule.h"
#include "NPInputParser.h"
using namespace std;



class GaspardTrackerTrapezoid : public GaspardTrackerModule
{
public:
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
   GaspardTrackerTrapezoid(map<int, GaspardTrackerModule*> &Module, TGaspardTrackerPhysics* EventPhysics);
   virtual ~GaspardTrackerTrapezoid();

public:
   ////////////////////////////////////////////////////
   ////  Inherite from GaspardTrackerModule class /////
   ////////////////////////////////////////////////////
   // Read stream at Configfile to pick-up parameters of detector (Position,...)
   void ReadConfiguration(NPL::InputParser);

   // The goal of this method is to extract physical parameters from raw data
   // Method called at each event read from the Input Tree
   void BuildPhysicalEvent();

   // Same as before but with a simpler treatment
   void BuildSimplePhysicalEvent();

private:
   map<int, GaspardTrackerModule*> &m_ModuleTest;
   TGaspardTrackerPhysics*         m_EventPhysics;

public:
   void SetGaspardDataPointer(TGaspardTrackerData* gaspardData) {m_EventData = gaspardData;};
   void PreTreat();

private:
   // Gaspard data coming from TGaspardTrackerPhysics through the 
   // SetGaspardDataPointer method
   TGaspardTrackerData* m_EventData;
   TGaspardTrackerData* m_PreTreatData;

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
   double GetStripPositionX(int N ,int X ,int Y)        { return m_StripPositionX[N-1-m_index["Trapezoid"]][X-1][Y-1]; }
   double GetStripPositionY(int N ,int X ,int Y)        { return m_StripPositionY[N-1-m_index["Trapezoid"]][X-1][Y-1]; }
   double GetStripPositionZ(int N ,int X ,int Y)        { return m_StripPositionZ[N-1-m_index["Trapezoid"]][X-1][Y-1]; }
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
   double m_FirstStageBaseLarge;   // mm
   double m_FirstStageHeight;      // mm
   int    m_NumberOfStripsX;
   int    m_NumberOfStripsY;
   double m_StripPitchX;      // mm
   double m_StripPitchY;      // mm
};

#endif
