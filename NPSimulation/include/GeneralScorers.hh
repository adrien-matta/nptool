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
 * Creation Date  : January 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class old some of the General Scorer, shared by different detector.
    Thos scorer could be a could basis for your own scorer                   *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * Those Scorer use TrackID as map index. This way ones can rebuild energy   *
 *  deposit, time of flight or position,... particle by particle for each    *
 *  event.Because standard scorer provide by G4 don't work this way but using* 
 *  a global ID for each event you should not use those scorer with some G4  *
 *  provided ones or being very carefull doing so.                           *                                            *
 *****************************************************************************/
#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

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


class PSInteractionCoordinatesX : public G4VPrimitiveScorer
	{
	public: // with description
	   PSInteractionCoordinatesX(G4String name, G4int depth = 0);
	   virtual ~PSInteractionCoordinatesX();

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

class PSInteractionCoordinatesY : public G4VPrimitiveScorer
	{
	public: // with description
	   PSInteractionCoordinatesY(G4String name, G4int depth = 0);
	   virtual ~PSInteractionCoordinatesY();

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



class PSInteractionCoordinatesZ : public G4VPrimitiveScorer
	{
	public: // with description
	   PSInteractionCoordinatesZ(G4String name, G4int depth = 0);
	   virtual ~PSInteractionCoordinatesZ();

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



class PSInteractionCoordinatesAngleTheta : public G4VPrimitiveScorer
{
public: // with description
   PSInteractionCoordinatesAngleTheta(G4String name, G4int depth = 0);
   virtual ~PSInteractionCoordinatesAngleTheta();

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



class PSInteractionCoordinatesAnglePhi : public G4VPrimitiveScorer
{
public: // with description
   PSInteractionCoordinatesAnglePhi(G4String name, G4int depth = 0);
   virtual ~PSInteractionCoordinatesAnglePhi();

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
	   PSDetectorNumber(G4String name, G4int depth = 0 , G4String VolumeName = "xxx");
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
	   G4String m_VolumeName ;
	};
#endif
