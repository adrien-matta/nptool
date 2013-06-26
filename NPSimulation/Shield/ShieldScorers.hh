#ifndef SHIELDScorer_h
#define SHIELDScorer_h 1
/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche  contact address: marc.labiche@stfc.ac.uk     *
 *                                                                           *
 * Creation Date  : 26/08/10                                                 *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class holds all the scorers needed by the                *
 *             GaspardTracker*** objects.                                    *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/



#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

namespace SHIELDSCORERS
{


class SHIELDScorerCsIDetectorNumber : public G4VPrimitiveScorer
{
public: // with description
   SHIELDScorerCsIDetectorNumber(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~SHIELDScorerCsIDetectorNumber();

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



class SHIELDScorerCsIEnergy : public G4VPrimitiveScorer
{
public: // with description
   SHIELDScorerCsIEnergy(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~SHIELDScorerCsIEnergy();

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



class SHIELDScorerCsITOF : public G4VPrimitiveScorer
{
public: // with description
   SHIELDScorerCsITOF(G4String name, G4String volumeName, G4int depth = 0);
   virtual ~SHIELDScorerCsITOF();

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

using namespace SHIELDSCORERS;
#endif
