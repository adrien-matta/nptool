#ifndef Miniball_h
#define Miniball_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : January 2016                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  Miniball simulation                                 *
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
#include "G4AssemblyVolume.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4GDMLParser.hh"

// NPTool header
#include "NPSVDetector.hh"
#include "TMiniballData.h"
#include "NPInputParser.h"
class Miniball : public NPS::VDetector{
  ////////////////////////////////////////////////////
  /////// Default Constructor and Destructor /////////
  ////////////////////////////////////////////////////
  public:
    Miniball() ;
    virtual ~Miniball() ;

    ////////////////////////////////////////////////////
    /////// Specific Function of this Class ///////////
    ////////////////////////////////////////////////////
  public:
    // Cylindric plastic
    void AddMiniball(double R,
        double Theta,
        double Phi,
        double Alpha);  


    G4AssemblyVolume* BuildClusterDetector(double Alpha);
    G4AssemblyVolume* BuildClusterDetectorRotation();
  
  private:
    G4GDMLParser m_gdmlparser;
    G4AssemblyVolume* m_ClusterDetector;
    vector <G4AssemblyVolume*> ClusterDetectorHolder;
    int ClusterDetectorCounter;

    G4LogicalVolume* m_LogicalGDML;
    //G4VPhysicalVolume* m_PhysicalGDML;

    G4String m_GDMLPath;
    G4String m_GDMLName;
    G4String m_GDMLWorld;
    G4bool constructChamber;

    // Little trick to avoid warning in compilation: Use a PVPlacement "buffer".
    // If don't you will have a Warning unused variable 'myPVP'
    G4VPhysicalVolume* PVPBuffer ;

    double m_NumberOfPlacedVolume;    
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

    // Adding the "plunger" coulex chamber
    void BuildChamber(G4LogicalVolume*);

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
    G4MultiFunctionalDetector* m_MiniballScorer ;
    ////////////////////////////////////////////////////
    ///////////Event class to store Data////////////////
    ////////////////////////////////////////////////////
  private:
    TMiniballData* m_Event ;

    ////////////////////////////////////////////////////
    ///////////////Private intern Data//////////////////
    ////////////////////////////////////////////////////
  private: // Geometry
    // Detector Coordinate 
    vector<double>  m_R; 
    vector<double>  m_Theta;
    vector<double>  m_Phi; 
    vector<double>  m_Alpha; 

    //   Shape type
    vector<string> m_Shape ;
   
  // Needed for dynamic loading of the library
  public:
    static NPS::VDetector* Construct();
};
#endif
