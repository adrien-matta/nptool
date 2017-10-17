#ifndef Foil_h
#define Foil_h 1
/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Mhd Moukaddam  contact address: m.moukaddam@surrey.ac.uk *
 *                                                                           *
 * Creation Date  : July 2017                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describes the simulation of a simple generic foil normally    *
 *  used in experiments as a catcher before/after target or a screen of      *
 *  other detectors e.g. the common Aluminum foil placed upstream the        *
 *  TRIFOIL with SHARC experiments                                           * 
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
#include "TFoilData.h"
#include "NPInputParser.h"

class Foil : public NPS::VDetector{
  ////////////////////////////////////////////////////
  /////// Default Constructor and Destructor /////////
  ////////////////////////////////////////////////////
  public:
    Foil() ;
    virtual ~Foil() ;

    ////////////////////////////////////////////////////
    /////// Specific Function of this Class ///////////
    ////////////////////////////////////////////////////
  public:
    // Cartesian
    void AddDetector(G4ThreeVector POS, string Shape, double Radius, double Thick, string Material);
    void AddDetector(G4ThreeVector POS, string Shape, double Height, double Width, double Thick, string Material);
   
    G4LogicalVolume* BuildSquareDetector( double FoilHeight, double FoilWidth, double FoilThickness, string FoilMaterial);
    G4LogicalVolume* BuildCylindricalDetector( double FoilRadius, double FoilThickness, string FoilMaterial);
  
  private:
    G4LogicalVolume* m_SquareDetector;
    G4LogicalVolume* m_CylindricalDetector;
    
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
    G4MultiFunctionalDetector* m_FoilScorer ;
    ////////////////////////////////////////////////////
    ///////////Event class to store Data////////////////
    ////////////////////////////////////////////////////
  private:
    TFoilData* m_Event ;

    ////////////////////////////////////////////////////
    ///////////////Private intern Data//////////////////
    ////////////////////////////////////////////////////
  private: // Geometry
    // Detector Coordinate 
    vector<double>  m_R; 
    vector<double>  m_Theta;
    vector<double>  m_Phi; 
   
   vector<double>   m_FoilThickness ;
   vector<double>   m_FoilRadius    ; // cylindrical shape
   vector<double>   m_FoilHeight    ; // squared shape
   vector<double>   m_FoilWidth     ; // squared shape
   vector<string>   m_FoilMaterial  ; 

    //   Shape type
    vector<string> m_Shape ;
   
    // Visualisation Attribute
    G4VisAttributes* m_VisSquare;
    G4VisAttributes* m_VisCylinder;

  // Needed for dynamic loading of the library
  public:
    static NPS::VDetector* Construct();
};
#endif
