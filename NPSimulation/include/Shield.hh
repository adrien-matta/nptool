/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche  contact address: marc.labiche@atfc.ac.uk     *
 *                                                                           *
 * Creation Date  : 25/08/10                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class manages different shapes of module for the Paris   *
 *             Shield detector. It allows to have Paris geometries with an   *
 *             heterogeneous set of modules                                  *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef Shield_h
#define Shield_h 1

// C++ headers
#include <vector>

// NPTool header
#include "VDetector.hh"
#include "ShieldModule.hh"

using namespace std;



class Shield : public VDetector
{
   ////////////////////////////////////////////////////
   /////// Default Constructor and Destructor /////////
   ////////////////////////////////////////////////////
public:
   Shield();
   virtual ~Shield();

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
   vector<ShieldModule*> m_Modules;
};
#endif
