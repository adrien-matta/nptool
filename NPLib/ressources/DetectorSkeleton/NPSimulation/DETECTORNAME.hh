#ifndef DETECTORNAME_h
#define DETECTORNAME_h 1
/*****************************************************************************
 * Copyright (C) 2009-XYEARX   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: XAUTHORX  contact address: XMAILX                        *
 *                                                                           *
 * Creation Date  : XMONTHX XYEARX                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  DETECTORNAME simulation                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// C++ header
#include <string>
#include <vector>

// G4 header defining G4 types
#include "globals.hh"

// G4 headers
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4LogicalVolume.hh"
#include "G4MultiFunctionalDetector.hh"

// NPTool header
#include "NPSVDetector.hh"
#include "TDETECTORNAMEData.h"

using namespace std;
using namespace CLHEP;

class DETECTORNAME : public NPS::VDetector
{
  ////////////////////////////////////////////////////
  /////// Default Constructor and Destructor /////////
  ////////////////////////////////////////////////////
  public:
    DETECTORNAME() ;
    virtual ~DETECTORNAME() ;

    ////////////////////////////////////////////////////
    /////// Specific Function of this Class ///////////
    ////////////////////////////////////////////////////
  public:
    // Cylindric plastic
    void AddDETECTORNAME(  G4double   R,
        G4double   Theta,
        G4double   Phi,
        G4double   Thickness,
        G4double   Radius,
        G4String   Scintillator);  

    // Squared DETECTORNAME
    void AddDETECTORNAME(  G4double   R,
        G4double   Theta,
        G4double   Phi,
        G4double   Height,
        G4double   Width,
        G4double   Thickness,
        G4String   Scintillator);

    void VolumeMaker(G4ThreeVector Det_pos, int DetNumber,G4LogicalVolume* world) ;
    ////////////////////////////////////////////////////
    //////  Inherite from NPS::VDetector class /////////
    ////////////////////////////////////////////////////
  public:
    // Read stream at Configfile to pick-up parameters of detector (Position,...)
    // Called in DetecorConstruction::ReadDetextorConfiguration Method
    void ReadConfiguration(string Path) ;

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
    G4MultiFunctionalDetector* m_DETECTORNAMEScorer ;
    ////////////////////////////////////////////////////
    ///////////Event class to store Data////////////////
    ////////////////////////////////////////////////////
  private:
    TDETECTORNAMEData* m_Event ;

    ////////////////////////////////////////////////////
    ///////////////Private intern Data//////////////////
    ////////////////////////////////////////////////////
  private: // Geometry
    vector<double>      m_DETECTORNAME_Thickness ;
    vector<double>      m_DETECTORNAME_Radius    ; // cylindrical shape
    vector<double>      m_DETECTORNAME_Height    ; // squared shape
    vector<double>      m_DETECTORNAME_Width     ; // squared shape

    // Used for By Angle Definition
    vector<G4double>  m_R; 
    vector<G4double>  m_Theta;
    vector<G4double>  m_Phi; 

    // Used for By Position Definition
    vector<G4double>  m_X; 
    vector<G4double>  m_Y; 
    vector<G4double>  m_Z; 

    //   Scintillator type
    vector<G4String> m_Scintillator ;

  // Needed for dynamic loading of the library
  public:
    static NPS::VDetector* Construct();
};
#endif
