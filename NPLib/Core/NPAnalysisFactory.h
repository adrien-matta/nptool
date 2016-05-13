#ifndef _ANALYSISFACTORY_
#define _ANALYSISFACTORY_
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
