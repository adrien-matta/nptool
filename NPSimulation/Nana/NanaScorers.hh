/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche  contact address: marc.labiche@stfc.ac.uk     *
 *                                                                           *
 * Creation Date  : 04/01/13                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class holds all the scorers needed by the                *
 *             GaspardTracker*** objects.                                    *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef NANAScorer_h
#define NANAScorer_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

namespace NANASCORERS
{


class NANAScorerLaBr3StageDetectorNumber : public G4VPrimitiveScorer
{
public: // with description
   NANAScorerLaBr3StageDetectorNumber(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~NANAScorerLaBr3StageDetectorNumber();

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



class NANAScorerLaBr3StageEnergy : public G4VPrimitiveScorer
{
public: // with description
   NANAScorerLaBr3StageEnergy(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~NANAScorerLaBr3StageEnergy();

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



class NANAScorerLaBr3StageCrystal : public G4VPrimitiveScorer
{
public: // with description
   NANAScorerLaBr3StageCrystal(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~NANAScorerLaBr3StageCrystal();

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


class NANAScorerLaBr3StageTOF : public G4VPrimitiveScorer
{
public: // with description
   NANAScorerLaBr3StageTOF(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~NANAScorerLaBr3StageTOF();

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



class NANAScorerCsIStageDetectorNumber : public G4VPrimitiveScorer
{
public: // with description
   NANAScorerCsIStageDetectorNumber(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~NANAScorerCsIStageDetectorNumber();

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
class NANAScorerCsIStageCrystalNumber : public G4VPrimitiveScorer
{
public: // with description
   NANAScorerCsIStageCrystalNumber(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~NANAScorerCsIStageCrystalNumber();

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


class NANAScorerCsIStageEnergy : public G4VPrimitiveScorer
{
public: // with description
   NANAScorerCsIStageEnergy(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~NANAScorerCsIStageEnergy();

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

using namespace NANASCORERS;
#endif
