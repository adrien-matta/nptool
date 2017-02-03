#ifndef _ANALYSISFACTORY_
#define _ANALYSISFACTORY_

/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien Matta   contact address: matta@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date  :                                                          *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/


#include"NPVAnalysis.h"
#include"NPAnalysisFactory.h"
#include<map>
#include<string>

typedef NPL::VAnalysis* (*ClassAnalysisFactoryFn)();

namespace NPL{
  class AnalysisFactory{
    private:
      AnalysisFactory();
      ~AnalysisFactory();

    public:
      static AnalysisFactory* getInstance();
      NPL::VAnalysis* Construct();
      void SetConstructor(ClassAnalysisFactoryFn fn);

    private:
      static AnalysisFactory* m_Instance;
      // Map that Match the Token with Constructor, once the lib is loaded
      ClassAnalysisFactoryFn m_Construct;
  };
}
#endif
