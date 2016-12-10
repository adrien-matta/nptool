#ifndef GaspardTrackerModule_h
#define GaspardTrackerModule_h 1
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

// NPTool - ROOT headers
#include "TGaspardTrackerData.h"
#include "NPInputParser.h"
using namespace std;



class GaspardTrackerModule
{
public:
   GaspardTrackerModule();
   virtual ~GaspardTrackerModule();

public:
   // Read stream at Configfile to pick-up parameters of detector (Position,...)
   virtual void ReadConfiguration(NPL::InputParser) = 0;

   // The goal of this method is to extract physical parameters from raw data
   // Method called at each event read from the Input Tree
   virtual void BuildPhysicalEvent() = 0;

   // Same as before but with a simpler treatment
   virtual void BuildSimplePhysicalEvent() = 0;

   // Initialize the Index map for the different modules of Gaspard
   void InitializeIndex();

   // Pass the TGaspardTrackerData object from TGaspardTrackerPhysics to the 
   // classes deriving from GaspardTrackerModule
   virtual void SetGaspardDataPointer(TGaspardTrackerData* gaspardData) = 0;

   //
   virtual double GetStripPositionX(Int_t DetectorNumber, Int_t stripX, Int_t stripY) = 0;
   virtual double GetStripPositionY(Int_t DetectorNumber, Int_t stripX, Int_t stripY) = 0;
   virtual double GetStripPositionZ(Int_t DetectorNumber, Int_t stripX, Int_t stripY) = 0;

protected:
   map<string, int> m_index;
};

#endif
