#ifndef GaspardTracker_h
#define GaspardTracker_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
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
 * Decription: This class manages different shapes of module for the Gaspard *
 *             tracker. It allows to have Gaspard geometries with an         *
 *             heterogeneous set of modules                                  *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <vector>

// NPTool header
#include "NPSVDetector.hh"
#include "GaspardTrackerModule.hh"
#include "NPInputParser.h"
// Geant4 Header
#include "G4AssemblyVolume.hh"

using namespace std;
using namespace CLHEP;

class GaspardTracker : public NPS::VDetector{
  ////////////////////////////////////////////////////
  /////// Default Constructor and Destructor /////////
  ////////////////////////////////////////////////////
  public:
    GaspardTracker();
    virtual ~GaspardTracker();

    ////////////////////////////////////////////////////
    /////////  Inherite from NPS::VDetector class ///////////
    ////////////////////////////////////////////////////
  public:
    // Read stream at Configfile to pick-up parameters of detector (Position,...)
    // Called in DetecorConstruction::ReadDetextorConfiguration Method
    void ReadConfiguration(NPL::InputParser);

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


  public:
    G4AssemblyVolume* BuildMugastChamber();

  private:
    vector<GaspardTrackerModule*> m_Modules;
    string m_Chamber;

  public:
    static NPS::VDetector* Construct();
};
#endif
