#ifndef DriftElectronScorers_h
#define SiliconScorers_h 1
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : February 2013                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  File old the scorer specific to the Silicon Detector                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * This new style of scorer is aim to become the standard way of doing scorer*
 * in NPTool.                                                                *
 *The index is build using the TrackID, Detector Number and Strip Number.    *
 *The scorer Hold Energy and time together                                   *
 *Only one scorer is needed for a detector                                   *
 *****************************************************************************/
#include "G4VPrimitiveScorer.hh"
#include "NPSHitsMap.hh"

#include <map>
using namespace std;
using namespace CLHEP;

namespace DRIFTELECTRONSCORERS {
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
  class PS_DriftElectron : public G4VPrimitiveScorer{
    
  public: // with description
    PS_DriftElectron(G4String name,G4int depth=0);
     ~PS_DriftElectron();
    
  protected: // with description
     G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    
  public:
    void Initialize(G4HCofThisEvent*);
    void EndOfEvent(G4HCofThisEvent*);
    void clear();
    void DrawAll();
    void PrintAll();
  
private: // inherited from G4VPrimitiveScorer
      G4int HCID;
      NPS::HitsMap<G4double*>* EvtMap;
    
  private: // Needed for intermediate calculation (avoid multiple instantiation in Processing Hit)
      G4int m_Index;
      G4int m_Level;
      G4int m_DetectorNumber;
      G4ThreeVector m_Position;
      G4double m_NumberOfDriftElectron;
    
  };
  

}

#endif
