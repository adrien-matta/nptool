#ifndef GaspardTrackerAnnular_h
#define GaspardTrackerAnnular_h 1
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
#include <iostream>
#include <string>
#include <map>
#include <vector>

// Gaspard headers
#include "TGaspardTrackerData.h"
#include "TGaspardTrackerPhysics.h"
#include "GaspardTrackerModule.h"
#include "NPInputParser.h"
using namespace std;



class GaspardTrackerAnnular : public GaspardTrackerModule
{
public:
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
   GaspardTrackerAnnular(map<int, GaspardTrackerModule*> &Module, TGaspardTrackerPhysics* EventPhysics);
   virtual ~GaspardTrackerAnnular();

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
   map<int, GaspardTrackerModule*>  &m_ModuleTest;
   TGaspardTrackerPhysics*          m_EventPhysics;

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
   // Add a Module 
   void AddModule(double zpos, double rmin, double rmax);

   // Getters to retrieve the (X,Y,Z) coordinates of a pixel defined by strips (X,Y)
   double GetStripPositionX(int N ,int X ,int Y)        {return m_StripPositionX[N-1-m_index["Annular"]][X-1][Y-1];}
   double GetStripPositionY(int N ,int X ,int Y)        {return m_StripPositionY[N-1-m_index["Annular"]][X-1][Y-1];}
   double GetStripPositionZ(int N ,int X ,int Y)        {return m_StripPositionZ[N-1-m_index["Annular"]][X-1][Y-1];}
   double GetNumberOfModule()                           { return m_NumberOfModule;}

private:
   // Spatial Position of Strip Calculated on basis of detector position
   int m_NumberOfModule;
   vector< vector < vector < double > > >    m_StripPositionX;
   vector< vector < vector < double > > >    m_StripPositionY;
   vector< vector < vector < double > > >    m_StripPositionZ;

private:
   //////////////////////////////
   // Geometry and stip number //
   //////////////////////////////
   int m_NumberOfStripsTheta;
   int m_NumberOfStripsPhi;
};

#endif
