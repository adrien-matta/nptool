#ifndef Must2Scorers_h
#define Must2Scorers_h 1
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : January 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  File old the scorer specific to the MUST2 Detector                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * Those Scorer use TrackID as map index. This way ones can rebuild energy   *
 *  deposit, time of flight or position,... particle by particle for each    *
 *  event.Because standard scorer provide by G4 don't work this way but using* 
 *  a global ID for each event you should not use those scorer with some G4  *
 *  provided ones or being very carefull doing so.                           *
 *****************************************************************************/
#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"
namespace MUST2 {
	
	class PSStripNumberX : public G4VPrimitiveScorer
	{

	public: // with description
	   PSStripNumberX(G4String name, G4int depth = 0, G4double StripPlaneSize = 98, G4int NumberOfStrip = 128);
	   virtual ~PSStripNumberX();

	protected: // with description
	   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

	public:
	   virtual void Initialize(G4HCofThisEvent*);
	   virtual void EndOfEvent(G4HCofThisEvent*);
	   virtual void clear();
	   virtual void DrawAll();
	   virtual void PrintAll();

	private:
	   G4double  m_StripPlaneSize;
	   G4int     m_NumberOfStrip ;
	   G4double  m_StripPitch		 ;
	   G4int HCID;
	   G4THitsMap<G4int>* EvtMap;
	};



	class PSStripNumberY : public G4VPrimitiveScorer
	{

	public: // with description
	   PSStripNumberY(G4String name, G4int depth = 0, G4double StripPlaneSize = 98, G4int NumberOfStrip = 128);
	   virtual ~PSStripNumberY();

	protected: // with description
	   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

	public:
	   virtual void Initialize(G4HCofThisEvent*);
	   virtual void EndOfEvent(G4HCofThisEvent*);
	   virtual void clear();
	   virtual void DrawAll();
	   virtual void PrintAll();

	private:
	   G4double  m_StripPlaneSize;
	   G4int     m_NumberOfStrip ;
	   G4double  m_StripPitch		 ;
	   G4int HCID;
	   G4THitsMap<G4int>* EvtMap;
	};
	
	
		class PSPadOrCristalNumber : public G4VPrimitiveScorer
	{

	public: // with description
	   PSPadOrCristalNumber(G4String name, G4int depth = 0);
	   virtual ~PSPadOrCristalNumber();

	protected: // with description
	   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

	public:
	   virtual void Initialize(G4HCofThisEvent*);
	   virtual void EndOfEvent(G4HCofThisEvent*);
	   virtual void clear();
	   virtual void DrawAll();
	   virtual void PrintAll();

	private:
	   G4int HCID;
	   G4THitsMap<G4int>* EvtMap;
	};
	
}

#endif
