#ifndef AnnularCsI_SIMULATION_HEADER
#define AnnularCsI_SIMULATION_HEADER 1
/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Greg Christian  contact address: gchristian@tamu.edu     *
 *                                                                           *
 * Creation Date  : March 2018                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  AnnularCsI simulation                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// C++ header
#include <string>
#include <vector>

// G4 headers
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4LogicalVolume.hh"
#include "G4MultiFunctionalDetector.hh"

// NPTool header
#include "NPSVDetector.hh"
#include "AnnularCsI_Utils.h"
#include "TAnnularCsIData.h"
#include "NPInputParser.h"

class AnnularCsI : public NPS::VDetector{
  ////////////////////////////////////////////////////
  /////// Default Constructor and Destructor /////////
  ////////////////////////////////////////////////////
public:
	AnnularCsI() ;
	virtual ~AnnularCsI() ;

	////////////////////////////////////////////////////
	/////// Specific Function of this Class ///////////
	////////////////////////////////////////////////////
public:
	// Cartesian
	void AddDetector(double R_min, double R_max,
									 double Phi_min, double Phi_max,
									 double Z);

	G4LogicalVolume* BuildCylindricalDetector(unsigned short i);
  
private:
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
	G4MultiFunctionalDetector* m_AnnularCsIScorer ;
	////////////////////////////////////////////////////
	///////////Event class to store Data////////////////
	////////////////////////////////////////////////////
private:
	TAnnularCsIData* m_Event ;

	////////////////////////////////////////////////////
	///////////////Private intern Data//////////////////
	////////////////////////////////////////////////////
private: // Geometry
	// Detector Coordinates
	std::vector<AnnularCsI_Utils::Geometry> m_Geo;
	
	// Visualisation Attribute
	G4VisAttributes* m_VisSquare;
	G4VisAttributes* m_VisCylinder;

  // Needed for dynamic loading of the library
public:
	static NPS::VDetector* Construct();
};
#endif
