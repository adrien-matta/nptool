#ifndef ComptonTelescopeScorer_h
#define ComptonTelescopeScorer_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
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



#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"

namespace COMPTONTELESCOPESCORERS
{

   class ComptonTelescopeScorerProcess : public G4VPrimitiveScorer
   {
      public: // with description
         ComptonTelescopeScorerProcess(G4String name, G4String volumeName, G4String processName = "compt", G4int depth = 0);
         virtual ~ComptonTelescopeScorerProcess();

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
         G4String m_ProcessName;
         G4int HCID;
         G4THitsMap<G4double>* EvtMap;
};



   class ComptonTelescopeScorerTrackerTowerNumber : public G4VPrimitiveScorer
   {
      public: // with description
         ComptonTelescopeScorerTrackerTowerNumber(G4String name, G4String volumeName, G4int depth = 0);
         virtual ~ComptonTelescopeScorerTrackerTowerNumber();

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



   class ComptonTelescopeScorerTrackerDSSSDNumber : public G4VPrimitiveScorer
   {
      public: // with description
         ComptonTelescopeScorerTrackerDSSSDNumber(G4String name, G4String volumeName, G4int depth = 0);
         virtual ~ComptonTelescopeScorerTrackerDSSSDNumber();

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



   class ComptonTelescopeScorerTrackerEnergy : public G4VPrimitiveScorer
   {
      public: // with description
         ComptonTelescopeScorerTrackerEnergy(G4String name, G4String volumeName, G4int depth = 0);
         virtual ~ComptonTelescopeScorerTrackerEnergy();

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




   class ComptonTelescopeScorerTrackerTOF : public G4VPrimitiveScorer
   {
      public: // with description
         ComptonTelescopeScorerTrackerTOF(G4String name, G4String volumeName, G4int depth = 0);
         virtual ~ComptonTelescopeScorerTrackerTOF();

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



   class ComptonTelescopeScorerCalorimeterTowerNumber : public G4VPrimitiveScorer
   {
      public: // with description
         ComptonTelescopeScorerCalorimeterTowerNumber(G4String name, G4String volumeName, G4int depth = 0);
         virtual ~ComptonTelescopeScorerCalorimeterTowerNumber();

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



   class ComptonTelescopeScorerCalorimeterDetectorNumber : public G4VPrimitiveScorer
   {
      public: // with description
         ComptonTelescopeScorerCalorimeterDetectorNumber(G4String name, G4String volumeName, G4int depth = 0);
         virtual ~ComptonTelescopeScorerCalorimeterDetectorNumber();

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


   class ComptonTelescopeScorerCalorimeterEnergy : public G4VPrimitiveScorer
   {
      public: // with description
         ComptonTelescopeScorerCalorimeterEnergy(G4String name, G4String volumeName, G4int depth = 0);
         virtual ~ComptonTelescopeScorerCalorimeterEnergy();

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

using namespace COMPTONTELESCOPESCORERS;
#endif
