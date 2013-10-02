#ifndef Must2Scorers_h
#define Must2Scorers_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"
namespace MUST2 {
	class PSStripE : public G4VPrimitiveScorer
	{

	public: // with description
	   PSStripE(G4String name, G4int depth = 0);
	   virtual ~PSStripE();

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
	   G4int HCID;
	   G4THitsMap<G4double>* EvtMap;
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
}

#endif
