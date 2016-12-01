#ifndef SiliconScorers_h
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
#include "G4THitsMap.hh"

#include <map>
using namespace std;
using namespace CLHEP;

namespace SILICONSCORERS {
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  struct RectangularOutput {
    G4double totalEnergy;
    G4double globalTime;
    G4double x;
    G4double y;
    G4double z;
    G4double theta;
    G4double phi;
    G4double detectorNumber;
    G4double stripLengthNumber;
    G4double stripWidthNumber;
  };

  class PS_Silicon_Rectangle : public G4VPrimitiveScorer{
    
  public: // with description
    PS_Silicon_Rectangle(G4String name, G4int Level, G4double StripPlaneLength, G4double StripPlaneWidth, G4int NumberOfStripLength,G4int NumberOfStripWidth,G4int depth=0,G4String axis="xy");
     ~PS_Silicon_Rectangle();

  protected: // with description
     G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    
  public:
    void Initialize(G4HCofThisEvent*);
    void EndOfEvent(G4HCofThisEvent*);
    void clear();
    void DrawAll();
    void PrintAll();
  
  private: // Geometry of the detector
    G4double m_StripPlaneLength;
    G4double m_StripPlaneWidth;
    G4int    m_NumberOfStripLength;
    G4int    m_NumberOfStripWidth;
    G4double m_StripPitchLength;
    G4double m_StripPitchWidth;
    G4String m_Axis;
    // Level at which to find the copy number linked to the detector number
    G4int    m_Level;

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
  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  struct AnnularOutput {
    G4double totalEnergy;
    G4double globalTime;
    G4double x;
    G4double y;
    G4double z;
    G4double theta;
    G4double phi;
    G4double detectorNumber;
    G4double stripRingNumber;
    G4double stripSectorNumber;
    G4double stripQuadrantNumber;
  };

  class PS_Silicon_Annular : public G4VPrimitiveScorer{
    
  public: // with description
    PS_Silicon_Annular(G4String name,G4int Level, G4double StripPlaneInnerRadius, G4double StripPlaneOuterRadius, G4double PhiStart,G4double PhiStop, G4int NumberOfStripRing,G4int NumberOfStripSector=1, G4int NumberOfQuadrant=1,G4int depth=0);
    ~PS_Silicon_Annular();
    
  protected: // with description
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    
  public:
    void Initialize(G4HCofThisEvent*);
    void EndOfEvent(G4HCofThisEvent*);
    void clear();
    void DrawAll();
    void PrintAll();
    
  private: // Geometry of the detector
    G4double m_StripPlaneInnerRadius;
    G4double m_StripPlaneOuterRadius;
    G4double m_PhiStart;
    G4double m_PhiStop;
    G4int    m_NumberOfStripRing;
    G4int    m_NumberOfStripSector;
    G4int    m_NumberOfStripQuadrant;
    G4double m_StripPitchRing;
    G4double m_StripPitchSector;
    G4double m_StripPitchQuadrant; 
    G4String m_Axis;
    // Level at which to find the copy number linked to the detector number
    G4int    m_Level;

  private: // inherited from G4VPrimitiveScorer
    G4int HCID;
    G4THitsMap<G4double*>* EvtMap;

  private: // Needed for intermediate calculation (avoid multiple instantiation in Processing Hit)
    G4ThreeVector m_Position    ;
    G4ThreeVector m_uz          ;
    G4int m_DetectorNumber      ;
    G4int m_StripRingNumber   ;
    G4int m_StripSectorNumber    ;
    G4int m_StripQuadrantNumber ;
    G4int m_Index               ;
    
  };
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  struct ResistiveOutput {
    G4double upstreamEnergy;
    G4double downstreamEnergy;
    G4double globalTime;
    G4double detectorNumber;
    G4double stripWidthNumber;
    G4double x;
    G4double y;
    G4double z;
    G4double theta;
    G4double phi;
  };

  class PS_Silicon_Resistive : public G4VPrimitiveScorer{
    
  public: // with description
    PS_Silicon_Resistive(G4String name, G4int Level,
                         G4double StripPlaneLength, G4double StripPlaneWidth,
                         G4int NumberOfStripWidth,G4int depth=0);
    
    ~PS_Silicon_Resistive();
    
  protected: // with description
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    
  public:
    void Initialize(G4HCofThisEvent*);
    void EndOfEvent(G4HCofThisEvent*);
    void clear();
    void DrawAll();
    void PrintAll();
    
  private: // Geometry of the detector
    G4double m_StripPlaneLength;
    G4double m_StripPlaneWidth;
    G4int    m_NumberOfStripWidth;
    G4double m_StripPitchWidth;
    // Level at which to find the copy number linked to the detector number
    G4int    m_Level;
   
  private: // inherited from G4VPrimitiveScorer
    G4int HCID;
    G4THitsMap<G4double*>* EvtMap;
    
  private: // Needed for intermediate calculation (avoid multiple instantiation in Processing Hit)
    G4ThreeVector m_Position  ;
    G4int m_DetectorNumber    ;
    G4int m_StripWidthNumber  ;
    G4int m_Index             ;
    
  };

}


#endif
