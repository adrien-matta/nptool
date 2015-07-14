#ifndef __NPCALIBRATIONSOURCE_H__
#define __NPCALIBRATIONSOURCE_H__

// STL
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
using namespace std;

// Root
#include <TF1.h>

// NPL
#include "NPEnergyLoss.h"

namespace NPL{
  class CalibrationSource{
    
  public:
    CalibrationSource();
    ~CalibrationSource();
    
  private: // Basic information
    // First Index is for main peak, second index for the relative sattellites peaks
    vector< vector<double> > m_Energies;
    vector< vector<double> > m_BranchingRatio;
    vector< vector<double> > m_ErrEnergies;
  
  private: // Computed source signal (a TF1 ready for fit use, set up with physical value)
    TF1* m_SourceSignal;
    void ComputeSourceSignal();
   
  public: // Getter and Setter
    TF1* GetSourceSignal();
    
  public: // Get and Set the energies
    // for particle like source (one gaussian per energies)
    void AddParticleContribution(vector<double> Energies ,  vector<double> ErrEnergies, vector<double> BranchingRatio);
    
    inline vector< vector<double> > GetEnergies(){return m_Energies;}
    inline vector< vector<double> > GetEnergiesErrors(){return m_ErrEnergies;}
    inline vector< vector<double> > GetBranchingRatio(){return m_BranchingRatio;}
    
 
  public: // Some Standard source
    void Set_ThreeAlphaSource();
    void Set_239Pu();
    void Set_241Am();
    void Set_244Cm();
    
  };
}
#endif
