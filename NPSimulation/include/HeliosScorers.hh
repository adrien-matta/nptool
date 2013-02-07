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
 *             Helios*** objects.                                            *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef HELIOSScorer_h
#define HELIOSScorer_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

namespace HELIOSSCORERS
{

class HeliosScorerFirstStageEnergy : public G4VPrimitiveScorer
{
public: // with description
   HeliosScorerFirstStageEnergy(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~HeliosScorerFirstStageEnergy();

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


class HeliosScorerFirstStageFrontStripDummyShape : public G4VPrimitiveScorer
{
public: // with description
   HeliosScorerFirstStageFrontStripDummyShape(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~HeliosScorerFirstStageFrontStripDummyShape();

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



class HeliosScorerFirstStageBackStripDummyShape : public G4VPrimitiveScorer
{
public: // with description
   HeliosScorerFirstStageBackStripDummyShape(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~HeliosScorerFirstStageBackStripDummyShape();

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



class HeliosScorerFirstStageFrontStripSquare : public G4VPrimitiveScorer
{
public: // with description
   HeliosScorerFirstStageFrontStripSquare(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~HeliosScorerFirstStageFrontStripSquare();

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



class HeliosScorerFirstStageBackStripSquare : public G4VPrimitiveScorer
{
public: // with description
   HeliosScorerFirstStageBackStripSquare(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~HeliosScorerFirstStageBackStripSquare();

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



class HeliosScorerFirstStageFrontStripTrapezoid : public G4VPrimitiveScorer
{
public: // with description
   HeliosScorerFirstStageFrontStripTrapezoid(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~HeliosScorerFirstStageFrontStripTrapezoid();

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



class HeliosScorerFirstStageBackStripTrapezoid : public G4VPrimitiveScorer
{
public: // with description
   HeliosScorerFirstStageBackStripTrapezoid(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   virtual ~HeliosScorerFirstStageBackStripTrapezoid();

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



class HeliosScorerFirstStageFrontStripAnnular : public G4VPrimitiveScorer
{
public: // with description
   HeliosScorerFirstStageFrontStripAnnular(G4String name, G4int depth = 0, G4double StripPlaneSize = 98, G4int NumberOfStrip = 128);
   virtual ~HeliosScorerFirstStageFrontStripAnnular();

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



class HeliosScorerFirstStageBackStripAnnular : public G4VPrimitiveScorer
{
public: // with description
   HeliosScorerFirstStageBackStripAnnular(G4String name, G4int depth = 0, G4double StripPlaneSize = 98, G4int NumberOfStrip = 128);
   virtual ~HeliosScorerFirstStageBackStripAnnular();

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

using namespace HELIOSSCORERS;
#endif
