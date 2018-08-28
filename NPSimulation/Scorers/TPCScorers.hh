#ifndef DECathodeScorers_h
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Pierre MORFOUACE  contact address: morfouace@ganil.fr    *
 *                                                                           *
 * Creation Date  : July 2018                                                *
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

namespace TPCSCORERS {
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
  class PS_TPCCathode : public G4VPrimitiveScorer{
    
  public: // with description
    PS_TPCCathode(G4String name,G4int depth=0);
     ~PS_TPCCathode();
    
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
    
  };
//////////////////////////////////////////////////////////////////////////////// 
  class PS_DEDigitizer : public G4VPrimitiveScorer{
    
  public: // with description
    PS_DEDigitizer(G4String name,G4int depth=0);
     ~PS_DEDigitizer();
    
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
    
  };
  

}

#endif
