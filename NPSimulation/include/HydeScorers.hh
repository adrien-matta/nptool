#ifndef HYDScorer_h
#define HYDScorer_h 1
/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
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
 *             HydeTracker*** objects.                                       *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

namespace HYDSCORERS
{
   // This Threshold is used in all scorers
   // Any energy deposit under this threshold will not create an entry
   const double TriggerThreshold = 0.1 * keV;

class HYDScorerFirstStageEnergy : public G4VPrimitiveScorer
{
public: // with description
   HYDScorerFirstStageEnergy(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~HYDScorerFirstStageEnergy();

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



class HYDScorerSecondStageEnergy : public G4VPrimitiveScorer
{
public: // with description
   HYDScorerSecondStageEnergy(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~HYDScorerSecondStageEnergy();

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



class HYDScorerThirdStageEnergy : public G4VPrimitiveScorer
{
public: // with description
   HYDScorerThirdStageEnergy(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~HYDScorerThirdStageEnergy();

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




class HYDScorerFirstStageFrontStripDummyShape : public G4VPrimitiveScorer
{
public: // with description
   HYDScorerFirstStageFrontStripDummyShape(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~HYDScorerFirstStageFrontStripDummyShape();

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



class HYDScorerFirstStageBackStripDummyShape : public G4VPrimitiveScorer
{
public: // with description
   HYDScorerFirstStageBackStripDummyShape(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~HYDScorerFirstStageBackStripDummyShape();

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



class HYDScorerFirstStageFrontStripSquare : public G4VPrimitiveScorer
{
public: // with description
   HYDScorerFirstStageFrontStripSquare(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~HYDScorerFirstStageFrontStripSquare();

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



class HYDScorerFirstStageBackStripSquare : public G4VPrimitiveScorer
{
public: // with description
   HYDScorerFirstStageBackStripSquare(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~HYDScorerFirstStageBackStripSquare();

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



class HYDScorerFirstStageFrontStripTrapezoid : public G4VPrimitiveScorer
{
public: // with description
   HYDScorerFirstStageFrontStripTrapezoid(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~HYDScorerFirstStageFrontStripTrapezoid();

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



class HYDScorerFirstStageBackStripTrapezoid : public G4VPrimitiveScorer
{
public: // with description
   HYDScorerFirstStageBackStripTrapezoid(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~HYDScorerFirstStageBackStripTrapezoid();

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



class HYDScorerFirstStageFrontStripAnnular : public G4VPrimitiveScorer
{
public: // with description
   HYDScorerFirstStageFrontStripAnnular(G4String name, G4int depth = 0, G4double StripPlaneSize = 98, G4int NumberOfStrip = 128);
   virtual ~HYDScorerFirstStageFrontStripAnnular();

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



class HYDScorerFirstStageBackStripAnnular : public G4VPrimitiveScorer
{
public: // with description
   HYDScorerFirstStageBackStripAnnular(G4String name, G4int depth = 0, G4double StripPlaneSize = 98, G4int NumberOfStrip = 128);
   virtual ~HYDScorerFirstStageBackStripAnnular();

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

#endif
