#ifndef GeneralScorers_h
#define GeneralScorers_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"



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
#endif
