/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 21/07/09                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class holds all the scorers needed by the AnnularS1      *
 *             object.                                                       *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef AnnularS1Scorers_h
#define AnnularS1Scorers_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"


class AnnularS1ScorerDetectorNumber : public G4VPrimitiveScorer
{
public: // with description
   AnnularS1ScorerDetectorNumber(G4String name, G4int depth = 0);
   virtual ~AnnularS1ScorerDetectorNumber();

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



class AnnularS1ScorerEnergy : public G4VPrimitiveScorer
{
public: // with description
   AnnularS1ScorerEnergy(G4String name, G4int depth = 0);
   virtual ~AnnularS1ScorerEnergy();

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



class AnnularS1ScorerTime : public G4VPrimitiveScorer
{
public: // with description
   AnnularS1ScorerTime(G4String name, G4int depth = 0);
   virtual ~AnnularS1ScorerTime();

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



class AnnularS1ScorerThetaStripNumber : public G4VPrimitiveScorer
{
public: // with description
   AnnularS1ScorerThetaStripNumber(G4String name, G4int depth = 0);
   virtual ~AnnularS1ScorerThetaStripNumber();

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



class AnnularS1ScorerPhiStripNumber : public G4VPrimitiveScorer
{
public: // with description
   AnnularS1ScorerPhiStripNumber(G4String name, G4int depth = 0);
   virtual ~AnnularS1ScorerPhiStripNumber();

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

#endif
