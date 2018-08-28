#ifndef Actar_h
#define Actar_h 1
/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre Morfouace  contact address: morfouac@nscl.msu.edu                        *
 *                                                                           *
 * Creation Date  : September 2017                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  Actar simulation                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// C++ header
#include <string>
#include <vector>
#include <map>
using namespace std;

// G4 headers
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4LogicalVolume.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4UserLimits.hh"
#include "G4VFastSimulationModel.hh"
#include "G4FastSimulationManager.hh"

// NPTool header
#include "NPSVDetector.hh"
#include "TActarData.h"
#include "MEventReduced.h"
#include "NPInputParser.h"
#include "Decay.hh"
#include "BeamReaction.hh"

#define NumberOfCobo 16
#define NumberOfASAD 4
#define NumberOfAGET 4
#define NumberOfChannel 68


class Actar : public NPS::VDetector{
    ////////////////////////////////////////////////////
    /////// Default Constructor and Destructor /////////
    ////////////////////////////////////////////////////
public:
    Actar() ;
    virtual ~Actar() ;

    ////////////////////////////////////////////////////
    /////// Specific Function of this Class ///////////
    ////////////////////////////////////////////////////
public:
    // Cartesian
    void AddDetector(G4ThreeVector POS, string Shape);
    // Spherical
    void AddDetector(double R,double Theta,double Phi,string Shape);


    G4LogicalVolume* BuildDetector();

private:
    G4LogicalVolume* m_SquareDetector;
    G4LogicalVolume* m_logicGas;
    bool m_build_Silicon;
    bool m_build_Vamos_Silicon;
    bool m_build_CsI;
    bool m_build_BeamDump;
    G4LogicalVolume* m_LogicSilicon;
    G4LogicalVolume* m_LogicVamosSilicon;
    G4LogicalVolume* m_LogicCsICrystal;
    G4LogicalVolume* m_LogicBeamDump;

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
    //   Initialize all Scorer
    void InitializeScorers() ;
     void SimulateDigitizer(vector<double> E, vector<double> T, double fallTime,double start,double stop,double step,double noise);

    //   Associated Scorer
    G4MultiFunctionalDetector* m_ActarScorer ;
    G4MultiFunctionalDetector* m_CsIScorer ;
    G4MultiFunctionalDetector* m_SiliconScorer ;
    //G4MultiFunctionalDetector* m_VamosSiliconScorer ;
    ////////////////////////////////////////////////////
    ///////////Event class to store Data////////////////
    ////////////////////////////////////////////////////
private:
    TActarData* m_Event ;
    MEventReduced* m_EventReduced;

    ////////////////////////////////////////////////////
    ///////////////Private intern Data//////////////////
    ////////////////////////////////////////////////////
private: // Geometry
    // Detector Coordinate
    vector<double>  m_R;
    vector<double>  m_Theta;
    vector<double>  m_Phi;

    //   Shape type
    vector<string> m_Shape;

    // map
    map<int, int> m_PadToXRow;
    map<int, int> m_PadToZColumn;
    int m_PadToCobo[128][128]; //!
    int m_PadToAsad[128][128]; //!
    int m_PadToAGET[128][128]; //!
    int m_PadToChannel[128][128]; //!
    
    // token
    vector<string> m_GasMaterial;
    vector<int> m_GasFraction;
    double m_Pressure; // bar
    double m_Temperature; // kelvin

    // Visualisation Attribute
    G4VisAttributes* m_VisChamber;
    G4VisAttributes* m_VisWindows;
    G4VisAttributes* m_VisMicromegas;
    G4VisAttributes* m_VisGas;
    G4VisAttributes* m_VisPads;
    G4VisAttributes* m_SiliconVisAtt;
    G4VisAttributes* m_CsIVisAtt;
    G4VisAttributes* m_BeamDumpVisAtt;
    
    // Needed for dynamic loading of the library
    
private:
    // Region were reaction can occure:
    G4Region* m_ReactionRegion;
    vector<G4VFastSimulationModel*> m_ReactionModel;

    
public:
    static NPS::VDetector* Construct();
};
#endif
