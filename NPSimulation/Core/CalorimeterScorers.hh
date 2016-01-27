#ifndef CalorimeterScorers_h
#define CalorimeterScorers_h 1
/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
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
#include "G4THitsMap.hh"

#include <map>
using namespace std;
using namespace CLHEP;

namespace CALORIMETERSCORERS {
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
  class PS_Calorimeter : public G4VPrimitiveScorer{
    
  public: // with description
    PS_Calorimeter(G4String name, vector<G4int> NestingLevel,G4int depth=0);
     ~PS_Calorimeter();
    
  protected: // with description
     G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    
  public:
    void Initialize(G4HCofThisEvent*);
    void EndOfEvent(G4HCofThisEvent*);
    void clear();
    void DrawAll();
    void PrintAll();
  
  private: // How much level of volume nesting should be considered
   // Give the list of the nesting level at which the copy number should be return.
   // 0 is the lowest level possible (the actual volume copy number in which the interaction happen)
   vector<G4int> m_NestingLevel;
   G4int m_Index; 

  private: // inherited from G4VPrimitiveScorer
    G4int HCID;
    G4THitsMap<G4double*>* EvtMap;
  };

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
  class PS_CalorimeterWithInteraction : public G4VPrimitiveScorer{
    
  public: // with description
    PS_CalorimeterWithInteraction(G4String name, vector<G4int> NestingLevel,G4int depth=0);
     ~PS_CalorimeterWithInteraction();
    
  protected: // with description
     G4bool ProcessHits(G4Step*, G4TouchableHistory*);
     G4ThreeVector m_Position; 
  public:
    void Initialize(G4HCofThisEvent*);
    void EndOfEvent(G4HCofThisEvent*);
    void clear();
    void DrawAll();
    void PrintAll();
  
  private: // How much level of volume nesting should be considered
   // Give the list of the nesting level at which the copy number should be return.
   // 0 is the lowest level possible (the actual volume copy number in which the interaction happen)
   vector<G4int> m_NestingLevel;
   G4int m_Index; 

  private: // inherited from G4VPrimitiveScorer
    G4int HCID;
    G4THitsMap<G4double*>* EvtMap;
  };
}


#endif
