#ifndef ThinSiScorers_h
#define ThinSiScorers_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : October 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  File old the scorer specific to the ThinSi Detector                       *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * Those Scorer use TrackID as map index. This way ones can rebuild energy   *
 *  deposit, time of flight or position,... particle by particle for each    *
 *  event.Because standard scorer provide by G4 don't work this way but using* 
 *  a global ID for each event you should not use those scorer with some G4  *
 *  provided ones or being very carefull doing so.                           *
 *****************************************************************************/
#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"
#include "CLHEP/Units/SystemOfUnits.h"
using namespace CLHEP;
namespace THINSI {
   //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
   //   This Threshold is used in the above scorer. Any energy deposit under this threshold will not create an entry.
   const double TriggerThreshold = 0.1*keV ;
   
   class PSStripNumber : public G4VPrimitiveScorer
   {

   public: // with description
      PSStripNumber(G4String name, G4int depth = 0, G4double StripPlaneSize = 50*mm, G4int NumberOfStrip = 16);
      virtual ~PSStripNumber();

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
      G4double  m_StripPitch    ;
      G4int HCID;
      G4THitsMap<G4int>* EvtMap;
};

}

#endif
