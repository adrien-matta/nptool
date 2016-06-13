#ifndef NeutronWall_h
#define NeutronWall_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
 *                                                                           *
 * Creation Date  : June 2016                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  NeutronWall simulation                             *
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
#include "TNeutronWallData.h"

class NeutronWall : public NPS::VDetector{
    ////////////////////////////////////////////////////
    /////// Default Constructor and Destructor /////////
    ////////////////////////////////////////////////////
public:
    NeutronWall() ;
    virtual ~NeutronWall() ;
    
    ////////////////////////////////////////////////////
    /////// Specific Function of this Class ///////////
    ////////////////////////////////////////////////////
public:
    // Cylindric plastic
    void AddNeutronWall(double R,
                        double Theta,
                        double Phi,
                        double X,
                        double Y,
                        double Z,
                        double Rotation);
    
    void BuildDetector();
    
private:
    G4LogicalVolume* m_NeutronWall_log;
    G4LogicalVolume* m_AlCase_log;
    G4LogicalVolume* m_Quartz_log;
    G4LogicalVolume* m_QuartzCap_log;
    G4LogicalVolume* m_PMTube_log;
    G4LogicalVolume* m_NE213_log;
    G4LogicalVolume* m_ShadowBar_log;
    
    G4VPhysicalVolume* m_NE213Tube_phys;
    G4VPhysicalVolume* m_Quartz_phys;
    G4VPhysicalVolume* m_AlCase_phys;
    G4VPhysicalVolume* m_NeutronWall_phys;
    
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
    G4MultiFunctionalDetector* m_NeutronWallScorer ;
    ////////////////////////////////////////////////////
    ///////////Event class to store Data////////////////
    ////////////////////////////////////////////////////
private:
    TNeutronWallData* m_Event ;
    
    ////////////////////////////////////////////////////
    ///////////////Private intern Data//////////////////
    ////////////////////////////////////////////////////
private: // Geometry
    // Detector Coordinate
    vector<double>  m_R;
    vector<double>  m_Theta;
    vector<double>  m_Phi;
    vector<double>  m_X;
    vector<double>  m_Y;
    vector<double>  m_Z;
    vector<double>  m_Rot;
    
    
    // Visualisation Attribute
    G4VisAttributes* m_VisNE213;
    G4VisAttributes* m_VisQuartz;
    G4VisAttributes* m_VisAl;
    G4VisAttributes* m_VisNW;
    
    // Needed for dynamic loading of the library
public:
    static NPS::VDetector* Construct();
};
#endif
