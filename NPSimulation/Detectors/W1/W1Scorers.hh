#ifndef W1Scorer_h
#define W1Scorer_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 12/01/11                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class holds all the scorers needed by the W1 object      *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

namespace W1SCORERS
{
   // This Threshold is used in all scorers
   // Any energy deposit under this threshold will not create an entry
   const double TriggerThreshold = 0.1 * keV;

class W1ScorerFrontStripNumber : public G4VPrimitiveScorer
{
public: // with description
   W1ScorerFrontStripNumber(G4String name, G4int depth = 0, G4int NumberOfStrip = 16);
   virtual ~W1ScorerFrontStripNumber();

protected: // with description
   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
   virtual void Initialize(G4HCofThisEvent*);
   virtual void EndOfEvent(G4HCofThisEvent*);
   virtual void Clear();
   virtual void DrawAll();
   virtual void PrintAll();

private:
   G4int m_NumberOfStrip;
   G4int HCID;
   G4THitsMap<G4double>* EvtMap;
};

class W1ScorerBackStripNumber : public G4VPrimitiveScorer
{
public: // with description
   W1ScorerBackStripNumber(G4String name, G4int depth = 0, G4int NumberOfStrip = 16);
   virtual ~W1ScorerBackStripNumber();

protected: // with description
   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

public:
   virtual void Initialize(G4HCofThisEvent*);
   virtual void EndOfEvent(G4HCofThisEvent*);
   virtual void Clear();
   virtual void DrawAll();
   virtual void PrintAll();

private:
   G4int m_NumberOfStrip;
   G4int HCID;
   G4THitsMap<G4double>* EvtMap;
};


}

using namespace W1SCORERS;
#endif
