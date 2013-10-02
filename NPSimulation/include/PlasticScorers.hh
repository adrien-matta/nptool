#ifndef GeneralScorers_h
#define GeneralScorers_h 1
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : September 2009                                           *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class old some of the Platic Scorer, shared by different detector.  *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *****************************************************************************/
#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"
namespace PLASTIC {
class PSEnergy : public G4VPrimitiveScorer
	{

	public: // with description
	   PSEnergy(G4String name, G4int depth = 0);
	   virtual ~PSEnergy();

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
	   G4THitsMap<G4double>* EvtMap;
	};

class PSTOF : public G4VPrimitiveScorer
{

	public: // with description
	   PSTOF(G4String name, G4int depth = 0);
	   virtual ~PSTOF();

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
	   G4THitsMap<G4double>* EvtMap;
};


class PSDetectorNumber : public G4VPrimitiveScorer
	{

	public: // with description
	   PSDetectorNumber(G4String name, G4int depth = 0 );
	   virtual ~PSDetectorNumber();

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
