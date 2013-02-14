/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 11/07/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class holds all the scorers needed by the                *
 *             Hyde2Tracker*** objects.                                    *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef HYD2Scorer_h
#define HYD2Scorer_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

namespace HYD2SCORERS
{

class HYD2ScorerFirstStageEnergy : public G4VPrimitiveScorer
{
public: // with description
   HYD2ScorerFirstStageEnergy(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~HYD2ScorerFirstStageEnergy();

protected: // with description
   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
   virtual void Initialize(G4HCofThisEvent*);
   virtual void EndOfEvent(G4HCofThisEvent*);
   virtual void Clear();
   virtual void DrawAll();
   virtual void PrintAll();

private:
   G4String m_VolumeName;
   G4int HCID;
   G4THitsMap<G4double>* EvtMap;
};



class HYD2ScorerSecondStageEnergy : public G4VPrimitiveScorer
{
public: // with description
   HYD2ScorerSecondStageEnergy(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~HYD2ScorerSecondStageEnergy();

protected: // with description
   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
   virtual void Initialize(G4HCofThisEvent*);
   virtual void EndOfEvent(G4HCofThisEvent*);
   virtual void Clear();
   virtual void DrawAll();
   virtual void PrintAll();

private:
   G4String m_VolumeName;
   G4int HCID;
   G4THitsMap<G4double>* EvtMap;
};



class HYD2ScorerThirdStageEnergy : public G4VPrimitiveScorer
{
public: // with description
   HYD2ScorerThirdStageEnergy(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~HYD2ScorerThirdStageEnergy();

protected: // with description
   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
   virtual void Initialize(G4HCofThisEvent*);
   virtual void EndOfEvent(G4HCofThisEvent*);
   virtual void Clear();
   virtual void DrawAll();
   virtual void PrintAll();

private:
   G4String m_VolumeName;
   G4int HCID;
   G4THitsMap<G4double>* EvtMap;
};

class HYD2ScorerFourthStageEnergy : public G4VPrimitiveScorer
{
public: // with description
   HYD2ScorerFourthStageEnergy(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~HYD2ScorerFourthStageEnergy();

protected: // with description
   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
   virtual void Initialize(G4HCofThisEvent*);
   virtual void EndOfEvent(G4HCofThisEvent*);
   virtual void Clear();
   virtual void DrawAll();
   virtual void PrintAll();

private:
   G4String m_VolumeName;
   G4int HCID;
   G4THitsMap<G4double>* EvtMap;
};

class HYD2ScorerFifthStageEnergy : public G4VPrimitiveScorer
{
public: // with description
   HYD2ScorerFifthStageEnergy(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~HYD2ScorerFifthStageEnergy();

protected: // with description
   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
   virtual void Initialize(G4HCofThisEvent*);
   virtual void EndOfEvent(G4HCofThisEvent*);
   virtual void Clear();
   virtual void DrawAll();
   virtual void PrintAll();

private:
   G4String m_VolumeName;
   G4int HCID;
   G4THitsMap<G4double>* EvtMap;
};

class HYD2ScorerSixthStageEnergy : public G4VPrimitiveScorer
{
public: // with description
   HYD2ScorerSixthStageEnergy(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~HYD2ScorerSixthStageEnergy();

protected: // with description
   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
   virtual void Initialize(G4HCofThisEvent*);
   virtual void EndOfEvent(G4HCofThisEvent*);
   virtual void Clear();
   virtual void DrawAll();
   virtual void PrintAll();

private:
   G4String m_VolumeName;
   G4int HCID;
   G4THitsMap<G4double>* EvtMap;
};


class HYD2ScorerFirstStageFrontStripSquare1 : public G4VPrimitiveScorer
{
public: // with description
   HYD2ScorerFirstStageFrontStripSquare1(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~HYD2ScorerFirstStageFrontStripSquare1();

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



class HYD2ScorerFirstStageBackStripSquare1 : public G4VPrimitiveScorer
{
public: // with description
   HYD2ScorerFirstStageBackStripSquare1(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~HYD2ScorerFirstStageBackStripSquare1();

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



class HYD2ScorerFirstStageFrontStripTrapezoid1 : public G4VPrimitiveScorer
{
public: // with description
   HYD2ScorerFirstStageFrontStripTrapezoid1(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~HYD2ScorerFirstStageFrontStripTrapezoid1();

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



class HYD2ScorerFirstStageBackStripTrapezoid1 : public G4VPrimitiveScorer
{
public: // with description
   HYD2ScorerFirstStageBackStripTrapezoid1(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~HYD2ScorerFirstStageBackStripTrapezoid1();

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

class HYD2ScorerFirstStageFrontStripTrapezoid2 : public G4VPrimitiveScorer
{
public: // with description
   HYD2ScorerFirstStageFrontStripTrapezoid2(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~HYD2ScorerFirstStageFrontStripTrapezoid2();

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



class HYD2ScorerFirstStageBackStripTrapezoid2 : public G4VPrimitiveScorer
{
public: // with description
   HYD2ScorerFirstStageBackStripTrapezoid2(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~HYD2ScorerFirstStageBackStripTrapezoid2();

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

}

using namespace HYD2SCORERS;
#endif
