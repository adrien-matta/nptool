#ifndef TTIGRESSCRYSTAL_H
#define TTIGRESSCRYSTAL_H

#include <vector>
#include <string>
#include <math.h>
using namespace std;
// ROOT
#include"TVector3.h"
#include "TRotation.h"

// NPL
#include"TTigressData.h"

class TTigressCrystal  {

  public:
  TTigressCrystal(){Clear();};
  TTigressCrystal(int CrystalNbr, TVector3 CrystalPosition,TVector3 CrystalNormal);
  ~TTigressCrystal() {  };

  void Clear();
  TVector3 GetPosition();
  void SetPosition(TVector3 position);
  TVector3 GetCrystalNormal() {return m_Normal;};
    TVector3 GetCrystalPosition() {return m_Position;};

  private: 
    
    int m_CloverId;
    int m_CrystalId;
    std::string m_CrystalName;
    TVector3 m_Position;    
    TVector3 m_Normal;    
    
    void SetCloverId(int);
    void SetCrystalId(int);
    void SetCrystalPosition(TVector3 Position) {m_Position=Position;};
    void SetCrystalNormal(TVector3 Normal) {m_Normal=Normal.Unit();};
    
  public:
    double m_Core_Energy;
    vector<double> m_Segment_Energy;
    vector<double> m_Segment_Time;
    vector<int> m_Segment_Number;
};





#endif

