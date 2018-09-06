#ifndef NPRANDOM
#define NPRANDOM
/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta   contact address: matta@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date  : 4 september 2017                                         *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  A singleton Random generator to be used overrall nptool                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

//STL
#include<iostream>

// Root
#include"TRandom1.h"


namespace NPL{
  class Random{
    private:
      Random(unsigned int seed=1){m_Random.SetSeed(seed);};
      ~Random(){std::cout << "NPL::Random current seed was :" <<  m_Random.GetSeed() << std::endl;};

    public:
      static Random* getInstance(unsigned int seed=1);
      void SetSeed(unsigned int seed){m_Random.SetSeed(seed);};
      inline double Uniform() {return m_Random.Uniform() ;};
      inline double Uniform(const double& x1, const double& x2){return m_Random.Uniform(x1,x2) ;};

      inline double Gaus(const double& mean,const double& sigma){return m_Random.Gaus(mean,sigma) ;};
      inline int    Poisson(const int&  mean){return m_Random.Poisson(mean) ;};
      inline double Poisson(const double& mean){return m_Random.Poisson(mean) ;};
      inline double BreitWigner(const double& mean, const double& gamma){return m_Random.BreitWigner(mean,gamma) ;};
      inline int    Integer(const unsigned int& max){return m_Random.Integer(max) ;};

    private:
      static Random* m_Instance;
      TRandom1 m_Random; 
  };

  namespace Rand{
    Random* MainRand= NPL::Random::getInstance();
    double Uniform() {return MainRand->Uniform() ;};
    double Uniform(const double& x1, const double& x2){return MainRand->Uniform(x1,x2) ;};

    double Gaus(const double& mean,const double& sigma){return MainRand->Gaus(mean,sigma) ;};
    int    Poisson(const int&  mean){return MainRand->Poisson(mean) ;};
    double Poisson(const double& mean){return MainRand->Poisson(mean) ;};
    double BreitWigner(const double& mean, const double& gamma){return MainRand->BreitWigner(mean,gamma) ;};
    int    Integer(const unsigned int& max){return MainRand->Integer(max) ;};

  }
}
#endif
