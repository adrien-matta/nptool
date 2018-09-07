#ifndef DSSDScorers_h
#define DSSDScorers_h 1
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
#include "NPImage.h"
#include <unordered_map>
using namespace std;
using namespace CLHEP;

namespace DSSDScorers {
  // Hold data for DSSD hit
  class DSSDData{
    public:
      DSSDData(){};
      DSSDData(const double& Energy, const double& Time, const unsigned int& Strip, const unsigned int& Detector){
        m_Energy=Energy;
        m_Time=Time;
        m_Strip=Strip;
        m_Detector=Detector;
        m_Index = CalculateIndex(m_Strip,m_Detector);
      }

      ~DSSDData(){};

    private:
      unsigned int m_Index;
      double m_Energy;
      double m_Time;
      unsigned int m_Strip;
      unsigned int m_Detector;

    public : // Modifier
      inline void Set(const double& Energy, const double& Time, const unsigned int& Strip, const unsigned int& Detector){
        m_Energy=Energy;
        m_Time=Time;
        m_Strip=Strip;
        m_Detector=Detector;
        m_Index = CalculateIndex(m_Strip,m_Detector);
      }

      inline void Add(const double& Energy) {m_Energy+=Energy;}; 

    public: // Accessor
      inline unsigned int GetIndex() const {return m_Index;};
      inline double GetEnergy() const {return m_Energy;};
      inline double GetTime() const {return m_Time;};
      inline unsigned int GetStrip() const {return m_Strip;};
      inline unsigned int GetDetector() const {return m_Detector;};

    public:
      static unsigned int CalculateIndex(const unsigned int& Strip,const unsigned int& Detector) {return Detector*1e6+Strip;}

  };

  // Manage a vector of DSSD hit
  class DSSDDataVector{
    public:
      DSSDDataVector(){};
      ~DSSDDataVector(){};

    private:
      vector<DSSDData> m_Data;

    public:
      vector<DSSDData>::iterator find(const unsigned int& index) ;
      void clear(){m_Data.clear();} ;
      vector<DSSDData>::iterator end() {return m_Data.end();};
      vector<DSSDData>::iterator begin() {return m_Data.begin();};
      unsigned int size() {return m_Data.size();};
      void Add(const unsigned int& index,const double& Energy) {find(index)->Add(Energy);};
      void Set(const double& Energy, const double& Time, const unsigned int& Strip, const unsigned int& Detector) {m_Data.push_back(DSSDData(Energy,Time,Strip,Detector));};
      DSSDData* operator[](const unsigned int& i){return &m_Data[i];};
  };

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  class PS_Images : public G4VPrimitiveScorer{

    public: // with description
      PS_Images(G4String name, string imageFront,string imageBack,double scalingFront, double scalingBack, double centerOffsetX,double centerOffsetY,unsigned int ignoreValue, G4int depth=0);
      ~PS_Images(){};

    protected: // with description
      G4bool ProcessHits(G4Step*, G4TouchableHistory*);

    public:
      void Initialize(G4HCofThisEvent*);
      void EndOfEvent(G4HCofThisEvent*);
      void clear();
      void DrawAll(){};
      void PrintAll(){};

    private: // Geometry of the detector
      NPL::Image* m_ImageFront; 
      NPL::Image* m_ImageBack; 
      double m_ScalingFront;
      double m_ScalingBack;
      double m_CenterOffsetX;
      double m_CenterOffsetY;
      unsigned int m_IgnoreValue;

      // Level at which to find the copy number linked to the detector number
      G4int    m_Level;

    private: // inherited from G4VPrimitiveScorer
      DSSDDataVector m_HitFront;
      DSSDDataVector m_HitBack;

    private: // Needed for intermediate calculation (avoid multiple instantiation in Processing Hit)
      G4ThreeVector t_Position;
      unsigned int t_Index;
      double t_Energy;
      double t_Time;
      unsigned int t_DetectorNbr;
      unsigned int t_PixelFront;
      unsigned int t_PixelBack;

    public: // information accessor
      unsigned int GetFrontMult() {return m_HitFront.size();};
      unsigned int GetStripFront(const unsigned int& i){return m_HitFront[i]->GetStrip();}; 
      unsigned int GetDetectorFront(const unsigned int& i){return m_HitFront[i]->GetDetector();};
      double GetEnergyFront(const unsigned int& i){return m_HitFront[i]->GetEnergy();};
      double GetTimeFront(const unsigned int& i){return m_HitFront[i]->GetTime();};
      unsigned int GetBackMult() {return m_HitBack.size();};
      unsigned int GetStripBack(const unsigned int& i){return m_HitBack[i]->GetStrip();}; 
      unsigned int GetDetectorBack(const unsigned int& i){return m_HitBack[i]->GetDetector();};
      double GetEnergyBack(const unsigned int& i){return m_HitBack[i]->GetEnergy();};
      double GetTimeBack(const unsigned int& i){return m_HitBack[i]->GetTime();};

      void GetARGBFront(unsigned int& i,unsigned int& a,unsigned int& r,unsigned int& g,unsigned int& b); 
      void GetARGBBack(unsigned int& i,unsigned int& a,unsigned int& r,unsigned int& g,unsigned int& b);
  };

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  class PS_Rectangle : public G4VPrimitiveScorer{

    public: // with description
      PS_Rectangle(G4String name, G4int Level, G4double StripPlaneLength, G4double StripPlaneWidth, G4int NumberOfStripLength,G4int NumberOfStripWidth,G4int depth=0,G4String axis="xy");
      ~PS_Rectangle();

    private:
      enum psAxis{ps_xy,ps_yz,ps_xz};
      psAxis m_Axis;

    protected: // with description
      G4bool ProcessHits(G4Step*, G4TouchableHistory*);

    public:
      void Initialize(G4HCofThisEvent*);
      void EndOfEvent(G4HCofThisEvent*);
      void clear();
      void DrawAll();
      void PrintAll();

    private: // Geometry of the detector
      double m_StripPlaneLength;
      double m_StripPlaneWidth;
      unsigned int    m_NumberOfStripLength;
      unsigned int    m_NumberOfStripWidth;
      double m_StripPitchLength;
      double m_StripPitchWidth;
      // Level at which to find the copy number linked to the detector number
      int    m_Level;

    private: // inherited from G4VPrimitiveScorer
      DSSDDataVector m_HitLength;
      DSSDDataVector m_HitWidth;
    private: // Needed for intermediate calculation (avoid multiple instantiation in Processing Hit)
      G4ThreeVector t_Position;
      unsigned int t_Index;
      double t_Energy;
      double t_Time;
      unsigned int t_DetectorNumber;
      unsigned int t_StripLengthNumber;
      unsigned int t_StripWidthNumber;

    public:
      unsigned int GetLengthMult() {return m_HitLength.size();};
      unsigned int GetStripLength(const unsigned int& i){return m_HitLength[i]->GetStrip();};
      unsigned int GetDetectorLength(const unsigned int& i){return m_HitLength[i]->GetDetector();};
      double GetEnergyLength(const unsigned int& i){return m_HitLength[i]->GetEnergy();};
      double GetTimeLength(const unsigned int& i){return m_HitLength[i]->GetTime();};
      unsigned int GetWidthMult() {return m_HitWidth.size();};
      unsigned int GetStripWidth(const unsigned int& i){return m_HitWidth[i]->GetStrip();};
      unsigned int GetDetectorWidth(const unsigned int& i){return m_HitWidth[i]->GetDetector();};
      double GetEnergyWidth(const unsigned int& i){return m_HitWidth[i]->GetEnergy();};
      double GetTimeWidth(const unsigned int& i){return m_HitWidth[i]->GetTime();};


    public:
      static unsigned int CalculateIndex(const unsigned int& Strip,const unsigned int& Detector) {return Detector*1e6+Strip;}


  };

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  class PS_Annular : public G4VPrimitiveScorer{

    public: // with description
      PS_Annular(G4String name,G4int Level, G4double StripPlaneInnerRadius, G4double StripPlaneOuterRadius, G4double PhiStart,G4double PhiStop, G4int NumberOfStripRing,G4int NumberOfStripSector=1, G4int NumberOfQuadrant=1,G4int depth=0);
      ~PS_Annular();

    protected: // with description
      G4bool ProcessHits(G4Step*, G4TouchableHistory*);

    public:
      void Initialize(G4HCofThisEvent*);
      void EndOfEvent(G4HCofThisEvent*);
      void clear();
      void DrawAll();
      void PrintAll();

    private: // Geometry of the detector
      double m_StripPlaneInnerRadius;
      double m_StripPlaneOuterRadius;
      double m_PhiStart;
      double m_PhiStop;
      unsigned int    m_NumberOfStripRing;
      unsigned int    m_NumberOfStripSector;
      unsigned int    m_NumberOfStripQuadrant;
      double m_StripPitchRing;
      double m_StripPitchSector;
      double m_StripPitchQuadrant; 
      string m_Axis;
      // Level at which to find the copy number linked to the detector number
      int    m_Level;


    private: // inherited from G4VPrimitiveScorer
      DSSDDataVector m_HitRing;
      DSSDDataVector m_HitSector;
      DSSDDataVector m_HitQuadrant;


    private: // Needed for intermediate calculation (avoid multiple instantiation in Processing Hit)
      G4ThreeVector t_Position;
      unsigned int t_Index;
      double t_Energy;
      double t_Time;
      unsigned int t_DetectorNumber;
      unsigned int t_StripRingNumber;
      unsigned int t_StripSectorNumber;
      unsigned int t_StripQuadrantNumber;

    public:
      unsigned int GetRingMult() {return m_HitRing.size();};
      unsigned int GetStripRing(const unsigned int& i){return m_HitRing[i]->GetStrip();}; 
      unsigned int GetDetectorRing(const unsigned int& i){return m_HitRing[i]->GetDetector();};
      double GetEnergyRing(const unsigned int& i){return m_HitRing[i]->GetEnergy();};
      double GetTimeRing(const unsigned int& i){return m_HitRing[i]->GetTime();};
      unsigned int GetSectorMult() {return m_HitSector.size();};
      unsigned int GetStripSector(const unsigned int& i){return m_HitSector[i]->GetStrip();}; 
      unsigned int GetDetectorSector(const unsigned int& i){return m_HitSector[i]->GetDetector();};
      double GetEnergySector(const unsigned int& i){return m_HitSector[i]->GetEnergy();};
      double GetTimeSector(const unsigned int& i){return m_HitSector[i]->GetTime();};
      unsigned int GetQuadrantMult() {return m_HitQuadrant.size();};
      unsigned int GetStripQuadrant(const unsigned int& i){return m_HitQuadrant[i]->GetStrip();}; 
      unsigned int GetDetectorQuadrant(const unsigned int& i){return m_HitQuadrant[i]->GetDetector();};
      double GetEnergyQuadrant(const unsigned int& i){return m_HitQuadrant[i]->GetEnergy();};
      double GetTimeQuadrant(const unsigned int& i){return m_HitQuadrant[i]->GetTime();};

    private: // Needed for intermediate calculation (avoid multiple instantiation in Processing Hit)
      G4ThreeVector m_uz ;

    public:
      static unsigned int CalculateIndex(const unsigned int& Strip,const unsigned int& Detector) {return Detector*1e6+Strip;}


  };

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  class PS_Resistive : public G4VPrimitiveScorer{

    public: // with description
      PS_Resistive(G4String name, G4int Level,
          G4double StripPlaneLength, G4double StripPlaneWidth,
          G4int NumberOfStripWidth,G4int depth=0);

      ~PS_Resistive();

    protected: // with description
      G4bool ProcessHits(G4Step*, G4TouchableHistory*);

    public:
      void Initialize(G4HCofThisEvent*);
      void EndOfEvent(G4HCofThisEvent*);
      void clear();
      void DrawAll();
      void PrintAll();

    private: // Geometry of the detector
      double m_StripPlaneLength;
      double m_StripPlaneWidth;
      unsigned int    m_NumberOfStripWidth;
      double m_StripPitchWidth;
      // Level at which to find the copy number linked to the detector number
      G4int    m_Level;

    private: 
      // Up and Down are each extremities of the resistive strip
      DSSDDataVector m_HitUp;
      DSSDDataVector m_HitDown;
      DSSDDataVector m_HitBack;
    
    private: // Needed for intermediate calculation (avoid multiple instantiation in Processing Hit)
      G4ThreeVector t_Position;
      double t_Energy;
      double t_EnergyUp;
      double t_EnergyDown;
      double t_Time;
      unsigned int t_DetectorNumber;
      unsigned int t_StripWidthNumber;
      unsigned int t_Index;
    public:
      unsigned int GetUpMult() {return m_HitUp.size();};
      unsigned int GetStripUp(const unsigned int& i){return m_HitUp[i]->GetStrip();}; 
      unsigned int GetDetectorUp(const unsigned int& i){return m_HitUp[i]->GetDetector();};
      double GetEnergyUp(const unsigned int& i){return m_HitUp[i]->GetEnergy();};
      double GetTimeUp(const unsigned int& i){return m_HitUp[i]->GetTime();};
      unsigned int GetDownMult() {return m_HitDown.size();};
      unsigned int GetStripDown(const unsigned int& i){return m_HitDown[i]->GetStrip();}; 
      unsigned int GetDetectorDown(const unsigned int& i){return m_HitDown[i]->GetDetector();};
      double GetEnergyDown(const unsigned int& i){return m_HitDown[i]->GetEnergy();};
      double GetTimeDown(const unsigned int& i){return m_HitDown[i]->GetTime();};
      unsigned int GetBackMult() {return m_HitBack.size();};
      unsigned int GetStripBack(const unsigned int& i){return m_HitBack[i]->GetStrip();}; 
      unsigned int GetDetectorBack(const unsigned int& i){return m_HitBack[i]->GetDetector();};
      double GetEnergyBack(const unsigned int& i){return m_HitBack[i]->GetEnergy();};
      double GetTimeBack(const unsigned int& i){return m_HitBack[i]->GetTime();};


  };

}


#endif
