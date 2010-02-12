#ifndef EdenScorers_h
#define EdenScorers_h 1
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 12/02/2010                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *    This file describes the specific scorers associated to the EDEN        *
 *    neutron detector                                                       *
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

namespace EDENSCORERS {

class EDENScorerCharge : public G4VPrimitiveScorer
{
  public: // with description
   EDENScorerCharge(G4String name, G4int depth = 0);
   virtual ~EDENScorerCharge();

  protected: // with description
   virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

  public:
   virtual void Initialize(G4HCofThisEvent*);
   virtual void EndOfEvent(G4HCofThisEvent*);
   virtual void Clear();
   virtual void DrawAll();
   virtual void PrintAll();

  private:
   G4int HCID;
   G4THitsMap<G4double>* EvtMap;
};

}
#endif
