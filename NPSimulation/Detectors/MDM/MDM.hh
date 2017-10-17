#ifndef MDM_h
#define MDM_h 1
/*****************************************************************************
 * Copyright (C) 2009-2017   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Greg Christian  contact address: gchristian@tamu.edu                        *
 *                                                                           *
 * Creation Date  : October 2017                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  MDM simulation                                      *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// G4 headers
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4LogicalVolume.hh"
#include "G4MultiFunctionalDetector.hh"

// NPTool header
#include "NPSVDetector.hh"
#include "TMDMData.h"
#include "NPInputParser.h"

class Rayin;
class MDMTrace;

class MDM : public NPS::VDetector{
  ////////////////////////////////////////////////////
  /////// Default Constructor and Destructor /////////
  ////////////////////////////////////////////////////
public:
  MDM() ;
  virtual ~MDM() ;

  ////////////////////////////////////////////////////
  /////// Specific Function of this Class ///////////
  ////////////////////////////////////////////////////
public:
  void AddDetector(double angle /*deg*/, double field /*Gauss*/, const std::string& rayin);


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
  G4MultiFunctionalDetector* m_MDMScorer ;
  ////////////////////////////////////////////////////
  ///////////Event class to store Data////////////////
  ////////////////////////////////////////////////////
private:
  TMDMData* m_Event ;

  ////////////////////////////////////////////////////
  ///////////////Private intern Data//////////////////
  ////////////////////////////////////////////////////
private: // Geometry
  // Detector Coordinate 
  double  m_Angle;
  double  m_Field;
  std::string m_Rayin_file;

  MDMTrace* m_Trace;
  Rayin* m_Rayin;

  G4VisAttributes* m_VisSquare;
	
  // Needed for dynamic loading of the library
public:
  static NPS::VDetector* Construct();
};
#endif

