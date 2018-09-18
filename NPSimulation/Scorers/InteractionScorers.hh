#ifndef InteractionScorers_h
#define InteractionScorers_h 1
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
 *  File old the scorer to record Hit energy,time and position               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include "G4VPrimitiveScorer.hh"
#include "TInteractionCoordinates.h"
#include "NPImage.h"
#include <map>
using namespace std;
using namespace CLHEP;

namespace InteractionScorers {

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  class InteractionData{
    public:
      InteractionData(){m_Index=0;};
      InteractionData(const unsigned int& Index ,const double& Energy, const double& Time , const double& PositionX, const double& PositionY, const double& PositionZ, const double& Theta, const double& Phi){
        m_Index = Index;
        m_Energy = Energy;
        m_Time = Time;
        m_PositionX = PositionX;;
        m_PositionY = PositionY;
        m_PositionZ = PositionZ;
        m_Theta = Theta;
        m_Phi = Phi;
      }

      ~InteractionData(){};

    private:
      unsigned int m_Index;
      double m_Energy;
      double m_Time;
      double m_PositionX;
      double m_PositionY;
      double m_PositionZ;
      double m_Theta;
      double m_Phi;

    public:
      unsigned int GetIndex() const{return m_Index;};
      double GetEnergy() const{return m_Energy;};
      double GetTime() const{return m_Time;};
      double GetPositionX() const{return m_PositionX;};
      double GetPositionY() const{return m_PositionY;};
      double GetPositionZ() const{return m_PositionZ;};
      double GetTheta() const{return m_Theta;};
      double GetPhi() const{return m_Phi;};


    public:
      void Set(const unsigned int& Index, const double& Energy, const double& Time , const double& PositionX, const double& PositionY, const double& PositionZ, const double& Theta, const double& Phi){
        m_Index = Index;
        m_Energy = Energy;
        m_Time = Time;
        m_PositionX = PositionX;;
        m_PositionY = PositionY;
        m_PositionZ = PositionZ;
        m_Theta = Theta;
        m_Phi = Phi;
      }
      void Add(const double& Energy){m_Energy+=Energy;};
      unsigned int GetIndex(){return m_Index;};
  };

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  // Manage a vector of Interaction hit
  class InteractionDataVector{
    public:
      InteractionDataVector(){};
      ~InteractionDataVector(){};

    private:
      vector<InteractionData> m_Data;

    public:
      vector<InteractionData>::iterator find(const unsigned int& index) ;
      void clear(){m_Data.clear();} ;
      vector<InteractionData>::iterator end() {return m_Data.end();};
      vector<InteractionData>::iterator begin() {return m_Data.begin();};
      unsigned int size() {return m_Data.size();};
      void Add(const unsigned int& index,const double& Energy) {find(index)->Add(Energy);};
      void Set(const unsigned int& index,const double& Energy, const double& Time , const double& PositionX, const double& PositionY, const double& PositionZ, const double& Theta, const double& Phi) {m_Data.push_back(InteractionData(index,Energy,Time,PositionX,PositionY,PositionZ,Theta,Phi));};
      InteractionData* operator[](const unsigned int& i){return &m_Data[i];};
  };


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  class PS_Interactions : public G4VPrimitiveScorer{

    public: // with description
      PS_Interactions(G4String name, TInteractionCoordinates* Inter,G4int depth=0);
      ~PS_Interactions(){};

    protected: // with description
      G4bool ProcessHits(G4Step*, G4TouchableHistory*);

    public:
      void Initialize(G4HCofThisEvent*);
      void EndOfEvent(G4HCofThisEvent*);
      void clear();
      void DrawAll(){};
      void PrintAll(){m_InterractionCoordinates->Dump();};

      // Level at which to find the copy number linked to the detector number
      G4int    m_Level;

    private:
      InteractionDataVector m_DataVector;
      TInteractionCoordinates* m_InterractionCoordinates; 
      unsigned int t_Index;
      double t_Energy;
      double t_Time;
      G4ThreeVector t_Position;
  };

}


#endif
