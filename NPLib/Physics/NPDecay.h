#ifndef NPDECAY_H
#define NPDECAY_H
/*****************************************************************************
 * Copyright (C) 2009-2016    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author : Adrien Matta  contact: matta@lpccaen.in2p3.fr           *
 *                                                                           *
 * Creation Date   : Octobre 2017                                            *
 * Last update     :                                                         *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *   This Class hold data for all decay scheme of a given nuclei             *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
// C++ header
#include <string>
#include <vector>
#include <set>

// NPL Header
#include "NPInputParser.h"
#include "NPNucleus.h"

// Root
#include "TH1D.h"
#include "TGenPhaseSpace.h"
namespace NPL{
  // A given decay Path
  class SingleDecay{
    public: 
        SingleDecay(){};
        SingleDecay(std::string mother,double threshold, std::vector<std::string> daughter, 
        std::vector<double> Ex, TH1F* CrossSection);
        ~SingleDecay(){};
    
    private:
        std::string m_MotherName; 
        NPL::Nucleus m_Mother;
        double m_MotherMass;
        std::vector<std::string> m_DaughterName;
        std::vector<NPL::Nucleus> m_Daughter;
        bool m_GammaOnly;
        std::vector<double> m_DaughterMasses;
        std::vector<double> m_ExcitationEnergies;
        double m_Threshold;
        TH1F* m_CrossSectionHist; 
        TGenPhaseSpace m_TGenPhaseSpace; 
 
   public:
        // Given Energy and Momentum direction of Mother,
        // Send back Momemtum and Energy of Daugther
        // Return false if the decay is not allowed
        // true other wise
        bool GenerateEvent(double MEx,double MEK,double MPx, double MPy,double MPz,
        std::vector<double>& DEK,
        std::vector<double>& DPx,std::vector<double>& DPy,std::vector<double>& DPz);
   
   public:// Getter
        inline std::vector<std::string> GetDaughterName() {return m_DaughterName;};
        inline std::vector<double> GetExcitationEnergies() {return m_ExcitationEnergies;};
        inline std::vector<NPL::Nucleus> GetDaughter() {return m_Daughter;};
        inline NPL::Nucleus GetMother() {return m_Mother;};
        inline double GetThreshold(){return m_Threshold;};
    };
  
  // All decay Path for a given nuclei, with Branching Ratio
  class Decay{
    public:
      Decay(){};
      Decay(std::string MotherName,std::string path);
      Decay(std::string MotherName,NPL::InputParser parser);
      ~Decay(){};

    public:
      void ReadConfiguration(std::string MotherName, std::string path);
      void ReadConfiguration(std::string MotherName, NPL::InputParser parser);
      bool GenerateEvent(double MEx,double MEK,double MPx,double MPy,double MPz, 
      std::vector<NPL::Nucleus>& Daughter, std::vector<double>& Ex,
      std::vector<double>& DEK,
      std::vector<double>& DPx,std::vector<double>& DPy,std::vector<double>& DPz);
      bool AnyAboveThreshold(double MEx);


     private:
      std::string m_MotherName;  
      std::vector<int> m_Shoot;
      std::vector<NPL::SingleDecay> m_SingleDecay;
      std::vector<double> m_BranchingRatio;
      double m_BRTotal;
    }; 
 
  // All decay read from files
  class DecayStore{
    public:
      DecayStore(){};
      DecayStore(std::string path);
      DecayStore(NPL::InputParser parser);
      ~DecayStore(){};
    
     public:
      void ReadConfiguration(std::string path);
      void ReadConfiguration(NPL::InputParser parser);
      std::set<std::string> GetAllMotherName();  

    private:
      std::map<std::string, NPL::Decay> m_Store;
  
    public:
      void GenerateEvent(std::string MotherName, double MEx,
        double MEK,double MPx,double MPy,double MPz,
        std::vector<NPL::Nucleus>& Daughter, std::vector<double>& Ex,
        std::vector<double>& DEK,
        std::vector<double>& DPx,std::vector<double>& DPy,std::vector<double>& DPz);
      bool AnyAboveThreshold(std::string MotherName, double MEx);
    };
  }
#endif
