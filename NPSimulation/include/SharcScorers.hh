#ifndef SharcScorers_h
#define SharcScorers_h 1
/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
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
 *  File old the scorer specific to the Sharc Detector                       *
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
namespace SHARC {
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //   This Threshold is used in the above scorer. Any energy deposit under this threshold will not create an entry.
  const double TriggerThreshold = 0.1*keV ;
  
  class PS_Silicon_Rectangle : public G4VPrimitiveScorer{
    
  public: // with description
    PS_Silicon_Rectangle(G4String name, G4double StripPlaneLength, G4double StripPlaneWidth, G4int NumberOfStripLength,G4int NumberOfStripWidth,G4double TriggerThreshold,G4int depth=0);
    virtual ~PS_Silicon_Rectangle();
    
  protected: // with description
    virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    
  public:
    virtual void Initialize(G4HCofThisEvent*);
    virtual void EndOfEvent(G4HCofThisEvent*);
    virtual void clear();
    virtual void DrawAll();
    virtual void PrintAll();
  
  private: // Threshold
    G4double m_TriggerThreshold;
    
  private: // Geometry of the detector
    G4double m_StripPlaneLength;
    G4double m_StripPlaneWidth;
    G4int    m_NumberOfStripLength;
    G4int    m_NumberOfStripWidth;
    G4double m_StripPitchLength;
    G4double m_StripPitchWidth;
    
  private: // inherited from G4VPrimitiveScorer
    G4int HCID;
    G4THitsMap<G4double*>* EvtMap;
    
  private: // Needed for intermediate calculation (avoid multiple instantiation in Processing Hit)
    G4ThreeVector m_Position  ;
    G4int m_DetectorNumber    ;
    G4int m_StripLengthNumber ;
    G4int m_StripWidthNumber  ;
    G4int m_Index             ;
    
  };
}

#endif
