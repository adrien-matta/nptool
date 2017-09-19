#ifndef Sage_h
#define Sage_h 1
/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Daniel Cox  contact address: daniel.m.cox@jyu.fi         *
 *                                                                           *
 * Creation Date  : January 2017                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  Sage simulation                                     *
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
#include "G4Cache.hh"//TODO Why?

// NPTool header
#include "NPSVDetector.hh"
#include "TSageData.h"
#include "NPInputParser.h"

#include "G4ElectroMagneticField.hh"

#include "SageHVField.hh"


class Sage : public NPS::VDetector{
  ////////////////////////////////////////////////////
  /////// Default Constructor and Destructor /////////
  ////////////////////////////////////////////////////
  public:
    Sage() ;
    virtual ~Sage() ;

    ////////////////////////////////////////////////////
    /////// Specific Function of this Class ///////////
    ////////////////////////////////////////////////////
  public:
    // Cartesian
    void AddDetector(G4ThreeVector POS, string Shape);
    // Spherical
    void AddDetector(double R,double Theta,double Phi,string Shape);  

	void ConstructChamber(G4LogicalVolume* world);

	void ConstructEMField(G4String fieldFileName);

	G4LogicalVolume* BuildSiliconDetector();
  
  private:
	G4LogicalVolume* m_SiliconDetector;

	//Variables for electromagnetic field
	G4bool fieldIsInitialized;
	G4String fieldFileName;
	G4double fHVFieldStrength;

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
    G4MultiFunctionalDetector* m_SageScorer ;
    ////////////////////////////////////////////////////
    ///////////Event class to store Data////////////////
    ////////////////////////////////////////////////////
  private:
    TSageData* m_Event ;

    ////////////////////////////////////////////////////
    ///////////////Private intern Data//////////////////
    ////////////////////////////////////////////////////
  private: // Geometry
    // Detector Coordinate 
    vector<double>  m_R; 
    vector<double>  m_Theta;
    vector<double>  m_Phi; 
    
    //   Shape type
    vector<string> m_Shape ;

	// Specific flags
	int CarbonFoils;
	double InnerDiameter;
	double SiThickness;
   
    // Visualisation Attribute
    G4VisAttributes* m_VisSilicon;

  // Needed for dynamic loading of the library
  public:
    static NPS::VDetector* Construct();
};
#endif
