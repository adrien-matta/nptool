#ifndef CalorimeterScorers_h
#define CalorimeterScorers_h 1
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
//#include "NPSecondaries.hh"

#include <map>
using namespace std;
using namespace CLHEP;

namespace CalorimeterScorers {
  // Hold One hit info
  class CalorimeterData{
      public:
        CalorimeterData(const double& Energy,const double& Time,const vector<unsigned int>& Nesting){
          m_Index=CalculateIndex(Nesting);
          m_Level=Nesting;
          m_Energy=Energy;
          m_Time=Time;
          };
        ~CalorimeterData(){};

      private:
        unsigned int m_Index;
        vector<unsigned int> m_Level;
        double m_Energy;
        double m_Time;

      public:
        static unsigned int CalculateIndex(const vector<unsigned int>& Nesting);
     
      public:
        inline unsigned int GetIndex() const {return m_Index;}
        inline vector<unsigned int> GetLevel() const {return m_Level;}; 
        inline double GetEnergy() const {return m_Energy;};
        inline double GetTime() const {return m_Time;};

      public:
        void Add(const double& Energy) {m_Energy+=Energy;};
      };

  // Manage a vector of DSSD hit
  class CalorimeterDataVector{
    public:
      CalorimeterDataVector(){};
      ~CalorimeterDataVector(){};

    private:
      vector<CalorimeterData> m_Data;

    public:
      vector<CalorimeterData>::iterator find(const unsigned int& index) ;
      inline void clear(){m_Data.clear();} ;
      inline vector<CalorimeterData>::iterator end() {return m_Data.end();};
      inline vector<CalorimeterData>::iterator begin() {return m_Data.begin();};
      inline unsigned int size() {return m_Data.size();};
      inline void Add(const unsigned int& index,const double& Energy) {find(index)->Add(Energy);};
      inline void Set(const double& Energy, const double& Time, const vector<unsigned int>& Nesting) {m_Data.push_back(CalorimeterData(Energy,Time,Nesting));};
      CalorimeterData* operator[](const unsigned int& i){return &m_Data[i];};
  };


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
        
    private: 
        CalorimeterDataVector m_Data;
        double t_Energy;
        double t_Time;
        vector<unsigned int> t_Level;
    public:
      inline unsigned int  GetMult() {return m_Data.size();};
      inline double GetEnergy(const unsigned int& i) {return m_Data[i]->GetEnergy();};
      inline double GetTime(const unsigned int& i) {return m_Data[i]->GetTime();};
      inline vector<unsigned int> GetLevel(const unsigned int& i) {return m_Data[i]->GetLevel();};
    };
}


#endif
