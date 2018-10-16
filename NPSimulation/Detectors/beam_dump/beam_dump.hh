#ifndef beam_dump_h
#define beam_dump_h 1
/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: valerian  contact address: girardalcindor@ganil.fr                        *
 *                                                                           *
 * Creation Date  : November 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  beam_dump simulation                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// C++ header
#include <string>
#include <vector>
using namespace std;

// G4 headers
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4LogicalVolume.hh"
#include "G4MultiFunctionalDetector.hh"

// NPTool header
#include "NPSVDetector.hh"
#include "Tbeam_dumpData.h"
#include "NPInputParser.h"

class beam_dump : public NPS::VDetector{
  ////////////////////////////////////////////////////
  /////// Default Constructor and Destructor /////////
  ////////////////////////////////////////////////////
  public:
    beam_dump() ;
    virtual ~beam_dump() ;

    ////////////////////////////////////////////////////
    /////// Specific Function of this Class ///////////
    ////////////////////////////////////////////////////
  public:
    // Spherical
    void AddDetector(double R,double Theta, double Phi, double Thickness);  

    G4LogicalVolume* BuildSquareDetector();
  
  private:
    G4LogicalVolume* m_SquareDetector;
    
    ////////////////////////////////////////////////////
    //////  Inherite from NPS::VDetector class /////////
    ////////////////////////////////////////////////////
  public:
    // Read stream at Configfile to pick-up parameters of detector (Position,...)
    // Called in DetecorConstruction::ReadDetextorConfiguration Method
    void ReadConfiguration(NPL::InputParser) ;

    // Construct detector and inialise sensitive part.
    // Called After DetecorConstruction::AddDetector Method
    void ConstructDetector(G4LogicalVolume* world) ;

    // Add Detector branch to the EventTree.
    // Called After DetecorConstruction::AddDetector Method
    void InitializeRootOutput() ;

    // Read sensitive part and fill the Root tree.
    // Called at in the EventAction::EndOfEventAvtion
    void ReadSensitive(const G4Event* event) ;

  public:   // Scorer
    //   Initialize all Scorer used by the MUST2Array
    void InitializeScorers() ;

    //   Associated Scorer
    G4MultiFunctionalDetector* m_beam_dumpScorer ;
    ////////////////////////////////////////////////////
    ///////////Event class to store Data////////////////
    ////////////////////////////////////////////////////
  private:
    Tbeam_dumpData* m_Event ;

    ////////////////////////////////////////////////////
    ///////////////Private intern Data//////////////////
    ////////////////////////////////////////////////////
  private: // Geometry
    // Detector Coordinate 
    vector<double>  m_R; 
    vector<double>  m_Theta;
    vector<double>  m_Phi; 
    vector<double>  m_Thickness;
    
    // Visualisation Attribute
    G4VisAttributes* m_VisSquare;

  // Needed for dynamic loading of the library
  public:
    static NPS::VDetector* Construct();
};
#endif
