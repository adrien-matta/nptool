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

namespace TPCScorers{

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
  class CathodeData{
    public:
      CathodeData(){};
      CathodeData(const double& Charge, const double& Time,const unsigned int& Detector, const unsigned int& PadX, const unsigned int& PadY){
        m_Charge = Charge;
        m_Time   = Time;
        m_Detector = Detector;
        m_PadX   = PadX;
        m_PadY   = PadY;
        m_Index = CalculateIndex(m_Detector,m_PadX,m_PadY);
      }
      ~CathodeData(){};
    
   private:
     unsigned int m_Index;
     double m_Charge;
     double m_Time;
     unsigned int m_Detector;
     unsigned int m_PadX;
     unsigned int m_PadY;
   
   public:
     static unsigned int CalculateIndex(const unsigned int& DetectorNumber,const unsigned int& PadX, const unsigned int& PadY){return DetectorNumber+PadX*1e3+PadY*3e6;};
     inline unsigned int GetIndex() const {return m_Index;};
     inline double GetCharge() const {return m_Charge;};
     inline double GetTime() const {return m_Time;};
     inline unsigned int GetDetector() const {return m_Detector;};
     inline unsigned int GetPadX() const {return m_PadX;};
     inline unsigned int GetPadY() const {return m_PadY;};
     void Add(const double& Charge){m_Charge+=Charge;};
  };

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
   // Manage a vector of Cathode hit
  class CathodeDataVector{
    public:
      CathodeDataVector(){};
      ~CathodeDataVector(){};

    private:
      vector<CathodeData> m_Data;

    public:
      vector<CathodeData>::iterator find(const unsigned int& index) ;
      void clear(){m_Data.clear();} ;
      vector<CathodeData>::iterator end() {return m_Data.end();};
      vector<CathodeData>::iterator begin() {return m_Data.begin();};
      unsigned int size() {return m_Data.size();};
      void Add(const unsigned int& index,const double& Charge) {find(index)->Add(Charge);};
      void Set(const double& Charge, const double& Time, const unsigned int& Detector, const unsigned int& PadX, const unsigned int& PadY) {m_Data.push_back(CathodeData(Charge,Time,Detector,PadX,PadY));};
      CathodeData* operator[](const unsigned int& i){return &m_Data[i];};
  };


  
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

   private: 
      CathodeDataVector m_Data;    

    private: // Needed for intermediate calculation (avoid multiple instantiation in Processing Hit)
      unsigned int m_Level;
      double t_Charge;
      double t_Time;
      unsigned int t_Detector;
      unsigned int t_PadX;
      unsigned int t_PadY;
      G4ThreeVector t_Position;
    
    public:
      unsigned int GetMult(){return m_Data.size();};
      double GetCharge(const double& i) {return m_Data[i]->GetCharge();};
      double GetTime(const double& i) {return m_Data[i]->GetTime();};
      unsigned int GetDetector(const unsigned int& i) {return m_Data[i]->GetDetector();};
      unsigned int GetPadX(const unsigned int& i) {return m_Data[i]->GetPadX();};
      unsigned int GetPadY(const unsigned int& i) {return m_Data[i]->GetPadY();};


  };
/*  //////////////////////////////////////////////////////////////////////////////// 
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

*/
}

#endif
