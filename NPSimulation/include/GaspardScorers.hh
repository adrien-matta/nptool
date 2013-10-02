#ifndef GPDScorer_h
#define GPDScorer_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

namespace GPDSCORERS
{

class GPDScorerFirstStageEnergy : public G4VPrimitiveScorer
{
public: // with description
   GPDScorerFirstStageEnergy(G4String name, G4int depth = 0);
   virtual ~GPDScorerFirstStageEnergy();

protected: // with description
   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
   virtual void Initialize(G4HCofThisEvent*);
   virtual void EndOfEvent(G4HCofThisEvent*);
   virtual void Clear();
   virtual void DrawAll();
   virtual void PrintAll();

private:
   G4int HCID;
   G4THitsMap<G4double>* EvtMap;
};



class GPDScorerSecondStageEnergy : public G4VPrimitiveScorer
{
public: // with description
   GPDScorerSecondStageEnergy(G4String name, G4int depth = 0);
   virtual ~GPDScorerSecondStageEnergy();

protected: // with description
   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
   virtual void Initialize(G4HCofThisEvent*);
   virtual void EndOfEvent(G4HCofThisEvent*);
   virtual void Clear();
   virtual void DrawAll();
   virtual void PrintAll();

private:
   G4int HCID;
   G4THitsMap<G4double>* EvtMap;
};



class GPDScorerThirdStageEnergy : public G4VPrimitiveScorer
{
public: // with description
   GPDScorerThirdStageEnergy(G4String name, G4int depth = 0);
   virtual ~GPDScorerThirdStageEnergy();

protected: // with description
   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
   virtual void Initialize(G4HCofThisEvent*);
   virtual void EndOfEvent(G4HCofThisEvent*);
   virtual void Clear();
   virtual void DrawAll();
   virtual void PrintAll();

private:
   G4int HCID;
   G4THitsMap<G4double>* EvtMap;
};



class GPDScorerDetectorNumber : public G4VPrimitiveScorer
{
public: // with description
   GPDScorerDetectorNumber(G4String name, G4int depth = 0 , G4String VolumeName = "xxx");
   virtual ~GPDScorerDetectorNumber();

protected: // with description
   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
   virtual void Initialize(G4HCofThisEvent*);
   virtual void EndOfEvent(G4HCofThisEvent*);
   virtual void Clear();
   virtual void DrawAll();
   virtual void PrintAll();

private:
   G4int HCID;
   G4THitsMap<G4int>* EvtMap;
   G4String m_VolumeName ;
};



class GPDScorerFirstStageFrontStripSquare : public G4VPrimitiveScorer
{
public: // with description
   GPDScorerFirstStageFrontStripSquare(G4String name, G4int depth = 0, G4double StripPlaneSize = 98, G4int NumberOfStrip = 128);
   virtual ~GPDScorerFirstStageFrontStripSquare();

protected: // with description
   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
   virtual void Initialize(G4HCofThisEvent*);
   virtual void EndOfEvent(G4HCofThisEvent*);
   virtual void Clear();
   virtual void DrawAll();
   virtual void PrintAll();

private:
   G4double  m_StripPlaneSize;
   G4int     m_NumberOfStrip ;
   G4int HCID;
   G4THitsMap<G4double>* EvtMap;
};



class GPDScorerFirstStageBackStripSquare : public G4VPrimitiveScorer
{
public: // with description
   GPDScorerFirstStageBackStripSquare(G4String name, G4int depth = 0, G4double StripPlaneSize = 98, G4int NumberOfStrip = 128);
   virtual ~GPDScorerFirstStageBackStripSquare();

protected: // with description
   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
   virtual void Initialize(G4HCofThisEvent*);
   virtual void EndOfEvent(G4HCofThisEvent*);
   virtual void Clear();
   virtual void DrawAll();
   virtual void PrintAll();

private:
   G4double  m_StripPlaneSize;
   G4int     m_NumberOfStrip ;
   G4int HCID;
   G4THitsMap<G4double>* EvtMap;
};



class GPDScorerFirstStageFrontStripTrapezoid : public G4VPrimitiveScorer
{
public: // with description
   GPDScorerFirstStageFrontStripTrapezoid(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~GPDScorerFirstStageFrontStripTrapezoid();

protected: // with description
   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
   virtual void Initialize(G4HCofThisEvent*);
   virtual void EndOfEvent(G4HCofThisEvent*);
   virtual void Clear();
   virtual void DrawAll();
   virtual void PrintAll();

private:
   G4int     m_NumberOfStrip ;
   G4int HCID;
   G4THitsMap<G4double>* EvtMap;
};



class GPDScorerFirstStageBackStripTrapezoid : public G4VPrimitiveScorer
{
public: // with description
   GPDScorerFirstStageBackStripTrapezoid(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~GPDScorerFirstStageBackStripTrapezoid();

protected: // with description
   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
   virtual void Initialize(G4HCofThisEvent*);
   virtual void EndOfEvent(G4HCofThisEvent*);
   virtual void Clear();
   virtual void DrawAll();
   virtual void PrintAll();

private:
   G4int     m_NumberOfStrip ;
   G4int HCID;
   G4THitsMap<G4double>* EvtMap;
};



class GPDScorerFirstStageFrontStripAnnular : public G4VPrimitiveScorer
{
public: // with description
   GPDScorerFirstStageFrontStripAnnular(G4String name, G4int depth = 0, G4double StripPlaneSize = 98, G4int NumberOfStrip = 128);
   virtual ~GPDScorerFirstStageFrontStripAnnular();

protected: // with description
   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
   virtual void Initialize(G4HCofThisEvent*);
   virtual void EndOfEvent(G4HCofThisEvent*);
   virtual void Clear();
   virtual void DrawAll();
   virtual void PrintAll();

private:
   G4double  m_StripPlaneSize;
   G4int     m_NumberOfStrip ;
   G4int HCID;
   G4THitsMap<G4double>* EvtMap;
};



class GPDScorerFirstStageBackStripAnnular : public G4VPrimitiveScorer
{
public: // with description
   GPDScorerFirstStageBackStripAnnular(G4String name, G4int depth = 0, G4double StripPlaneSize = 98, G4int NumberOfStrip = 128);
   virtual ~GPDScorerFirstStageBackStripAnnular();

protected: // with description
   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
   virtual void Initialize(G4HCofThisEvent*);
   virtual void EndOfEvent(G4HCofThisEvent*);
   virtual void Clear();
   virtual void DrawAll();
   virtual void PrintAll();

private:
   G4double  m_StripPlaneSize;
   G4int     m_NumberOfStrip ;
   G4int HCID;
   G4THitsMap<G4double>* EvtMap;
};

}

using namespace GPDSCORERS;
#endif
