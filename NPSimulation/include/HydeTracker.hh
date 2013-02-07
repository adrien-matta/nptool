#ifndef HydeTracker_h
#define HydeTracker_h 1

/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 10/06/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class manages different shapes of module for the Hyde    *
 *             tracker. It allows to have Hyde geometries with an            *
 *             heterogeneous set of modules                                  *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <vector>

// NPTool header
#include "VDetector.hh"
#include "HydeTrackerModule.hh"

using namespace std;



class HydeTracker : public VDetector
{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   HydeTracker();
   virtual ~HydeTracker();

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
   vector<HydeTrackerModule*> m_Modules;
};
#endif
