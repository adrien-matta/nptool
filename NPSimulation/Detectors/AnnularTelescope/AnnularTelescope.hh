#ifndef AnnularTELESCOPE_SIMULATION_HEADER
#define AnnularTELESCOPE_SIMULATION_HEADER 1
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
 *  This class describe  AnnularTelescope simulation                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// C++ header
#include <string>
#include <vector>
#include <utility>

// G4 headers
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4LogicalVolume.hh"
#include "G4MultiFunctionalDetector.hh"

// NPTool header
#include "NPSVDetector.hh"
#include "AnnularTelescope_Utils.h"
#include "TAnnularTelescopeData.h"
#include "NPInputParser.h"

namespace AnnularTelescope_Utils { struct Geometry; }

class AnnularTelescope : public NPS::VDetector{
  ////////////////////////////////////////////////////
  /////// Default Constructor and Destructor /////////
  ////////////////////////////////////////////////////
public:
	AnnularTelescope() ;
	virtual ~AnnularTelescope() ;

	////////////////////////////////////////////////////
	/////// Specific Function of this Class ///////////
	////////////////////////////////////////////////////
public:
	// Cartesian
	void AddDetector(AnnularTelescope_Utils::Geometry& geo);

	std::pair<G4LogicalVolume*, G4LogicalVolume*>
	BuildDetector(unsigned short i);
  
private:
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

	// Fill NPL CsI Data
	// Called from ReadSensitive
	void FillCsIData(int detector_number, double energy, double time, const G4ThreeVector& pos);

	// Fill NPL Si Data
	// Called from ReadSensitive
	void FillSiData(int detector_number, double energy, double time, const G4ThreeVector& pos);

public:   // Scorer
	//   Initialize all Scorer used by the MUST2Array
	void InitializeScorers() ;

	//   Associated Scorer
	G4MultiFunctionalDetector* m_AnnularTelescopeScorer ;
	////////////////////////////////////////////////////
	///////////Event class to store Data////////////////
	////////////////////////////////////////////////////
private:
	TAnnularTelescopeData* m_Event ;

	////////////////////////////////////////////////////
	///////////////Private intern Data//////////////////
	////////////////////////////////////////////////////
private: // Geometry
	// Detector Coordinates
	std::vector<AnnularTelescope_Utils::Geometry> m_Geo;
	
	// Visualisation Attribute
	G4VisAttributes* m_VisSquare;
	G4VisAttributes* m_VisCylinder;

  // Needed for dynamic loading of the library
public:
	static NPS::VDetector* Construct();
};
#endif
