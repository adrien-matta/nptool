#ifndef GE_TAMU_Scorers_h
#define GE_TAMU_Scorers_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : February 2013                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  File old the scorer specific to the Silicon Detector                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * This new style of scorer is aim to become the standard way of doing scorer*
 * in NPTool.                                                                *
 *The index is build using the TrackID, Detector Number and Strip Number.    *
 *The scorer Hold Energy and time together                                   *
 *Only one scorer is needed for a detector                                   *
 *****************************************************************************/
#include "G4VPrimitiveScorer.hh"
#include "NPSHitsMap.hh"
#include "NPImage.h"
#include <map>


namespace GETAMUSCORERS {

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PS_GeTAMU : public G4VPrimitiveScorer{

public: // with description
	PS_GeTAMU(G4String name, G4int Level,G4int depth=0);
	~PS_GeTAMU();

protected: // with description
	G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
	void Initialize(G4HCofThisEvent*);
	void EndOfEvent(G4HCofThisEvent*);
	void clear();
	void DrawAll();
	void PrintAll();

private: // Geometry of the detector
#if 0
	G4double m_StripPlaneLength;
	G4double m_StripPlaneWidth;
	G4int    m_NumberOfStripLength;
	G4int    m_NumberOfStripWidth;
	G4double m_StripPitchLength;
	G4double m_StripPitchWidth;
	G4String m_Axis;
#endif
	// Level at which to find the copy number linked to the detector number
	G4int    m_Level;

private: // inherited from G4VPrimitiveScorer
	G4int HCID;
	NPS::HitsMap<G4double*>* EvtMap;

private: // Needed for intermediate calculation (avoid multiple instantiation in Processing Hit)
	G4ThreeVector m_Position  ;
	G4ThreeVector m_LocalPosition  ;
	G4int m_CloverNumber      ;
	G4int m_CrystalNumber     ;
	G4int m_Index             ;
};

}


#endif
