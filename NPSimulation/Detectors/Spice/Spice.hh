#ifndef Spice_h
#define Spice_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: A. Matta  contact address: a.matta@surrey.ac.uk          *
 *                                                                           *
 * Creation Date  : 19/07/2015                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: Define the Spice electron spectrometer                        *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *   Use GMDL geometry from Triumf (M. Moukhaddam)                           *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// C++ headers
#include <vector>
using namespace std;
using namespace CLHEP;

// NPTool header
#include "NPSVDetector.hh"
#include "NPInputParser.h"
// Geant4
#include "G4MultiFunctionalDetector.hh"
#include "G4LogicalVolume.hh"
#include "G4GDMLParser.hh"


class Spice : public NPS::VDetector{
  ////////////////////////////////////////////////////
  /////// Default Constructor and Destructor /////////
  ////////////////////////////////////////////////////
  public:
    Spice();
    virtual ~Spice();

    ////////////////////////////////////////////////////
    //////// Specific Function of this Class ///////////
    ////////////////////////////////////////////////////
  public:
    // Produce the logical volume of the detector
    G4LogicalVolume* ConstructVolume();

  private:
    G4LogicalVolume* m_LogicalDetector;
    G4GDMLParser m_gdmlparser;
    ////////////////////////////////////////////////////
    ////  Inherite from GaspardTrackerModule class /////
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

    // Initialize all scorers necessary for the detector
    void InitializeScorers();

    // Read sensitive part and fill the Root tree.
    // Called at in the EventAction::EndOfEventAvtion
    void ReadSensitive(const G4Event* event);


    ////////////////////////////////////////////////////
    ///////////Event class to store Data////////////////
    ////////////////////////////////////////////////////
  private:
    // Scorer
    G4MultiFunctionalDetector* m_Scorer;


    ////////////////////////////////////////////////////
    ///////////////Private intern Data//////////////////
    ////////////////////////////////////////////////////
  private:

  public:
    static NPS::VDetector* Construct();
};

#endif
