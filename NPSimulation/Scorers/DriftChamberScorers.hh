#ifndef DriftChamberScorers_h
#define DriftChamberScorers_h
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Cyril Lenain   contact adress: lenain@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date  : July 2018                                                *
 * Last update    : 09/01/19                                                 *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  Scorer for Drift Chamber based on cathode strips for X position and      *
 *  drift time for Y                                                         *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *****************************************************************************/
#include "G4VFastSimulationModel.hh"
#include "G4VPrimitiveScorer.hh"
#include "NPSHitsMap.hh"
#include <map>
using namespace std;
using namespace CLHEP;

namespace DriftChamberScorers {

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    class DCData {
        public:
            DCData(const double& DriftTime, const double& X,
                    const vector<unsigned int>& Nesting) {
                m_Index     = CalculateIndex(Nesting);
                m_Level     = Nesting;
                m_DriftTime = DriftTime;
                m_X         = X;
                m_Counter = 1;
            };
            ~DCData(){};

        private:
            unsigned int         m_Index;
            vector<unsigned int> m_Level;
            double               m_DriftTime;
            double               m_X;
            double               m_Counter;
        public:
            static unsigned int CalculateIndex(const vector<unsigned int>& Nesting);

        public:
            inline double               GetIndex() const { return m_Index;}
            inline vector<unsigned int> GetLevel() const {return m_Level;};
            inline double               GetDriftTime() const { return m_DriftTime; };
            inline double               GetX() const { return m_X; };
            inline double               GetCounter() {return m_Counter;};

        public:
            void Add(const double& DriftTime, const double& X) {
                m_DriftTime += DriftTime;
                m_X += X;
                m_Counter ++;
            };

            void Moy(const double& Counter){
                m_DriftTime /= Counter;
                m_X /= Counter;
            };
    };

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    class DCDataVector {
        public:
            DCDataVector(){};
            ~DCDataVector(){};

        private:
            vector<DCData> m_Data;

        public:
            vector<DCData>::iterator        find(const unsigned int& index);
            inline void                     clear() { m_Data.clear(); };
            inline vector<DCData>::iterator end() { return m_Data.end(); };
            inline vector<DCData>::iterator begin() { return m_Data.begin(); };
            inline unsigned int             size() { return m_Data.size(); };
            inline void Add(const unsigned int& index, const double& DriftTime, const double& X) {
                find(index)->Add(DriftTime, X);
            };

            inline void Moy( const unsigned int& index, const double& Counter){
                find(index)->Moy(Counter);
            };

            void Set(const double& DriftTime, const double& X,
                    const vector<unsigned int>& Nesting) {
                m_Data.push_back(DCData(DriftTime, X, Nesting));
            };
            DCData* operator[](const unsigned int& i) { return &m_Data[i]; };
    };

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    class PS_DriftChamber : public G4VPrimitiveScorer {

        public: // with description
            PS_DriftChamber(G4String name,vector<G4int> NestingLevel, const G4ThreeVector DriftDir,
                    const double DriftSpeed, G4int depth = 0);
            ~PS_DriftChamber();

        protected: // with description
            G4bool ProcessHits(G4Step*, G4TouchableHistory*);

        public:
            void Initialize(G4HCofThisEvent*);
            void EndOfEvent(G4HCofThisEvent*);
            void clear();
            void DrawAll();
            void PrintAll();

        private:
            vector<G4int> m_NestingLevel;
            G4int         m_Index;


        private: // Needed for intermediate calculation (avoid multiple instantiation
            // in Processing Hit)
            DCDataVector m_Data;
            vector<unsigned int> t_Level;
            unsigned int         t_Detector;
            G4ThreeVector        m_Dir;
            double               m_DriftSpeed;
            double               t_preout;
            double               t_postout;
            double               t_DriftTime;
            double               t_X;
            G4ThreeVector        t_PrePosition;
            G4ThreeVector        t_PostPosition;

        public:
            inline unsigned int GetMult() { return m_Data.size(); };
            inline double       GetDriftTime(const unsigned int& i) {
                return m_Data[i]->GetDriftTime();
            };
            inline double GetX(const unsigned int& i) { return m_Data[i]->GetX(); };
            inline double GetCounter(const unsigned int& i) { return m_Data[i]->GetCounter(); };
            inline vector<unsigned int> GetLevel(const unsigned int& i) {
                return m_Data[i]->GetLevel();
            };
    }; 

} // namespace DriftChamberScorers

#endif
