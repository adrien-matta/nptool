/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Marc Labiche    contact address: marc.labiche@stfc.ac.uk *
 *                                                                           *
 * Creation Date  : 31/01/12                                                 *
 * Last update    : 31/08/15                                                 *
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
  //HeliosScorerFirstStageFrontStripDummyShape(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   //HeliosScorerFirstStageFrontStripDummyShape(G4String name, G4int depth = 0, G4int NumberOfStrip = 250); // 0.5mm
   HeliosScorerFirstStageFrontStripDummyShape(G4String name, G4int depth = 0, G4int NumberOfStrip = 128); // 1mm
   //HeliosScorerFirstStageFrontStripDummyShape(G4String name, G4int depth = 0, G4int NumberOfStrip = 62);  // 2mm
   //HeliosScorerFirstStageFrontStripDummyShape(G4String name, G4int depth = 0, G4int NumberOfStrip = 25); // 5mm
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
   //HeliosScorerFirstStageBackStripDummyShape(G4String name, G4int depth = 0, G4int NumberOfStrip = 128);
   //HeliosScorerFirstStageBackStripDummyShape(G4String name, G4int depth = 0, G4int NumberOfStrip = 22); // 22x1mm
   //HeliosScorerFirstStageBackStripDummyShape(G4String name, G4int depth = 0, G4int NumberOfStrip = 10); // 11x2mm for helisol50.6
   HeliosScorerFirstStageBackStripDummyShape(G4String name, G4int depth = 0, G4int NumberOfStrip = 11); // 11x2mm for helisol54
   //HeliosScorerFirstStageBackStripDummyShape(G4String name, G4int depth = 0, G4int NumberOfStrip = 12); // 12x2mm for helisol57.5
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


}

using namespace HELIOSSCORERS;
#endif
