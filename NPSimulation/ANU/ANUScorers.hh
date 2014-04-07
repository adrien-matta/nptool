/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche    contact address: marc.labiche@stfc.ac.uk *
 *                                                                           *
 * Creation Date  : 31/01/12                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class holds all the scorers needed by the                *
 *             ANU*** objects.                                            *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef ANUScorer_h
#define ANUScorer_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

namespace ANUSCORERS
{

class ANUScorerSiLiEnergy : public G4VPrimitiveScorer
{
public: // with description
   ANUScorerSiLiEnergy(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~ANUScorerSiLiEnergy();

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


class ANUScorerSiLiFrontStripDummyShape : public G4VPrimitiveScorer
{
public: // with description
   ANUScorerSiLiFrontStripDummyShape(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~ANUScorerSiLiFrontStripDummyShape();

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



class ANUScorerSiLiBackStripDummyShape : public G4VPrimitiveScorer
{
public: // with description
   ANUScorerSiLiBackStripDummyShape(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~ANUScorerSiLiBackStripDummyShape();

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



class ANUScorerSiLiFrontStripSquare : public G4VPrimitiveScorer
{
public: // with description
   ANUScorerSiLiFrontStripSquare(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~ANUScorerSiLiFrontStripSquare();

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



class ANUScorerSiLiBackStripSquare : public G4VPrimitiveScorer
{
public: // with description
   ANUScorerSiLiBackStripSquare(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~ANUScorerSiLiBackStripSquare();

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



class ANUScorerSiLiFrontStripTrapezoid : public G4VPrimitiveScorer
{
public: // with description
   ANUScorerSiLiFrontStripTrapezoid(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~ANUScorerSiLiFrontStripTrapezoid();

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



class ANUScorerSiLiBackStripTrapezoid : public G4VPrimitiveScorer
{
public: // with description
   ANUScorerSiLiBackStripTrapezoid(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~ANUScorerSiLiBackStripTrapezoid();

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



class ANUScorerSiLiFrontStripAnnular : public G4VPrimitiveScorer
{
public: // with description
   ANUScorerSiLiFrontStripAnnular(G4String name, G4int depth = 0, G4double StripPlaneSize = 98, G4int NumberOfStrip = 128);
   virtual ~ANUScorerSiLiFrontStripAnnular();

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



class ANUScorerSiLiBackStripAnnular : public G4VPrimitiveScorer
{
public: // with description
   ANUScorerSiLiBackStripAnnular(G4String name, G4int depth = 0, G4double StripPlaneSize = 98, G4int NumberOfStrip = 128);
   virtual ~ANUScorerSiLiBackStripAnnular();

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

using namespace ANUSCORERS;
#endif
