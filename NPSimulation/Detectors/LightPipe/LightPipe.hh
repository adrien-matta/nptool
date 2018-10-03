#ifndef LightPipe_h
#define LightPipe_h 1
/*****************************************************************************
 * Copyright (C) 2009-2018   this file is part of the NPTool Project       *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Greg Christian  contact address: gchristian@tamu.edu                        *
 *                                                                           *
 * Creation Date  : July 2018                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describe  LightPipe simulation                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

// C++ header
#include <string>
#include <vector>
#include <tuple>
#include <map>

// G4 headers
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4LogicalVolume.hh"
#include "G4MultiFunctionalDetector.hh"

// NPTool header
#include "NPSVDetector.hh"
#include "TLightPipeData.h"
#include "NPInputParser.h"

class G4OpticalSurface;

class LightPipe : public NPS::VDetector{
  ////////////////////////////////////////////////////
  /////// Default Constructor and Destructor /////////
  ////////////////////////////////////////////////////
public:
	LightPipe() ;
	virtual ~LightPipe() ;

	////////////////////////////////////////////////////
	/////// Specific Function of this Class ///////////
	////////////////////////////////////////////////////
public:
	// Cartesian
	void AddDetector(G4int nrow, G4int ncol, G4int nlayer, G4double width, G4double thickness, G4double pipe_width, G4double pipe_thickness);
    
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

private:
	G4Material* m_ScintillatorMaterial;
	G4Material* m_PipeMaterial;
	G4Material* m_WrappingMaterial;
	G4OpticalSurface* m_ReflectiveSurface;

	G4Material* CreateScintillatorMaterial() const;
	G4Material* CreatePipeMaterial() const;
	G4Material* CreateWrappingMaterial() const;
	G4OpticalSurface* CreateReflectiveSurface() const;
	
public:   // Scorer
	//   Initialize all Scorer used by the MUST2Array
	void InitializeScorers() ;

	//   Associated Scorer
	G4MultiFunctionalDetector* m_LightPipeScorer ;
	G4MultiFunctionalDetector* m_PDScorer;
	////////////////////////////////////////////////////
	///////////Event class to store Data////////////////
	////////////////////////////////////////////////////
private:
	TLightPipeData* m_Event ;
	std::map<int, std::tuple<int, int, int> > m_DetectorMap; // <detNo, <side, layer, row/col> >
	
public:
	G4int    GetNrow(size_t i)          const { return std::get<0>(m_Detector.at(i)); }
	G4int    GetNcol(size_t i)          const { return std::get<1>(m_Detector.at(i)); }
	G4int    GetNlayer(size_t i)        const { return std::get<2>(m_Detector.at(i)); }
	G4double GetWidth(size_t i)         const { return std::get<3>(m_Detector.at(i)); }
	G4double GetThickness(size_t i)     const { return std::get<4>(m_Detector.at(i)); }
	G4double GetPipeWidth(size_t i)     const { return std::get<5>(m_Detector.at(i)); }
	G4double GetPipeThickness(size_t i) const { return std::get<6>(m_Detector.at(i)); }
	////////////////////////////////////////////////////
	///////////////Private intern Data//////////////////
	////////////////////////////////////////////////////
private: // Geometry
	// Detector Coordinate
	std::vector<std::tuple<G4int, G4int, G4int, G4double, G4double, G4double, G4double> > m_Detector;

private:
	// Visualisation Attribute
	G4VisAttributes* m_VisSquare;
	G4VisAttributes* m_VisPipe;
	G4VisAttributes* m_VisPD;

  // Needed for dynamic loading of the library
public:
	static NPS::VDetector* Construct();
};
#endif
