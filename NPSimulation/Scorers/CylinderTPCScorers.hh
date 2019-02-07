#ifndef DEAnodeScorers_h

/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Cyril Lenain  contact address: lenain@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date  : february 2019                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  Scorer for MINOS-TPC Drift Electron collector                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 * Count drift electrons per pads in MINOS                                    *
 *                                                                           *
 *****************************************************************************/

#include "G4VPrimitiveScorer.hh"
#include "NPSHitsMap.hh"

#include <map>
using namespace std;
using namespace CLHEP;

namespace CylinderTPCScorers{

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
  class AnodeData{
    public:
      AnodeData(){};
      AnodeData(const double& Charge, const double& DriftTime, const unsigned int& Pad, const double& X, const double& Y){
        m_Charge = Charge;
        m_DriftTime   = DriftTime;
        m_Pad   = Pad;
        m_X   = X;
        m_Y   = Y;
      }
      ~AnodeData(){};
    
   private:
     double m_Charge;
     double m_DriftTime;
     double m_X;
     double m_Y;
     unsigned int m_Pad;
   
   public:
     inline double GetCharge() const {return m_Charge;};
     inline double GetDriftTime() const {return m_DriftTime;};
     inline double GetX() const {return m_X;};
     inline double GetY() const {return m_Y;};
     inline unsigned int GetPad() const {return m_Pad;};
     void Add(const double& Charge){m_Charge+=Charge;};
  };

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
   // Manage a vector of Anode hit
  class AnodeDataVector{
    public:
      AnodeDataVector(){};
      ~AnodeDataVector(){};

    private:
      vector<AnodeData> m_Data;

    public:
      vector<AnodeData>::iterator find(const unsigned int& Pad) ;
      void clear(){m_Data.clear();} ;
      vector<AnodeData>::iterator end() {return m_Data.end();};
      vector<AnodeData>::iterator begin() {return m_Data.begin();};
      unsigned int size() {return m_Data.size();};
      void Add(const unsigned int& Pad,const double& Charge) {find(Pad)->Add(Charge);};
      void Set(const double& Charge, const double& DriftTime, const unsigned int& Pad, const double& X, const double& Y) {m_Data.push_back(AnodeData(Charge,DriftTime,Pad,X,Y));};
      AnodeData* operator[](const unsigned int& i){return &m_Data[i];};
  };


  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
  class PS_TPCAnode : public G4VPrimitiveScorer{

    public: // with description
      PS_TPCAnode(G4String name, vector<G4int>, G4int depth=0);
      ~PS_TPCAnode();

    protected: // with description
      G4bool ProcessHits(G4Step*, G4TouchableHistory*);

    public:
      void Initialize(G4HCofThisEvent*);
      void EndOfEvent(G4HCofThisEvent*);
      void clear();
      void DrawAll();
      void PrintAll();

   private: 
      AnodeDataVector m_Data;    

    private: // Needed for intermediate calculation (avoid multiple instantiation in Processing Hit)
     
      vector<G4int> m_NestingLevel;
      unsigned int m_Level;
      double t_Charge;
      double t_DriftTime;
      unsigned int t_Pad;
      double t_X;
      double t_Y;

    public:
      unsigned int GetMult(){return m_Data.size();};
      double GetCharge(const double& i) {return m_Data[i]->GetCharge();};
      double GetDriftTime(const double& i) {return m_Data[i]->GetDriftTime();};
      double GetX(const double& i) {return m_Data[i]->GetX();};
      double GetY(const double& i) {return m_Data[i]->GetY();};
      unsigned int GetPad(const unsigned int& i) {return m_Data[i]->GetPad();};


  };

}

#endif
