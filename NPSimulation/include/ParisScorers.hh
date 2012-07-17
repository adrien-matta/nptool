/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche  contact address: marc.labiche@stfc.ac.uk     *
 *                                                                           *
 * Creation Date  : 30/02/10                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class holds all the scorers needed by the                *
 *             GaspardTracker*** objects.                                    *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef PARISScorer_h
#define PARISScorer_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

namespace PARISSCORERS
{


class PARISScorerLaBr3StageDetectorNumber : public G4VPrimitiveScorer
{
public: // with description
   PARISScorerLaBr3StageDetectorNumber(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~PARISScorerLaBr3StageDetectorNumber();

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
   G4THitsMap<G4int>* EvtMap;
};



class PARISScorerLaBr3StageEnergy : public G4VPrimitiveScorer
{
public: // with description
   PARISScorerLaBr3StageEnergy(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~PARISScorerLaBr3StageEnergy();

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



class PARISScorerLaBr3StageCrystal : public G4VPrimitiveScorer
{
public: // with description
   PARISScorerLaBr3StageCrystal(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~PARISScorerLaBr3StageCrystal();

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
   G4THitsMap<G4int>* EvtMap;
};


class PARISScorerLaBr3StageTOF : public G4VPrimitiveScorer
{
public: // with description
   PARISScorerLaBr3StageTOF(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~PARISScorerLaBr3StageTOF();

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



class PARISScorerCsIStageDetectorNumber : public G4VPrimitiveScorer
{
public: // with description
   PARISScorerCsIStageDetectorNumber(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~PARISScorerCsIStageDetectorNumber();

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
   G4THitsMap<G4int>* EvtMap;
};

// Added by Anna
class PARISScorerCsIStageCrystalNumber : public G4VPrimitiveScorer
{
public: // with description
   PARISScorerCsIStageCrystalNumber(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~PARISScorerCsIStageCrystalNumber();

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
   G4THitsMap<G4int>* EvtMap;
};


class PARISScorerCsIStageEnergy : public G4VPrimitiveScorer
{
public: // with description
   PARISScorerCsIStageEnergy(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~PARISScorerCsIStageEnergy();

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

}

using namespace PARISSCORERS;
#endif
