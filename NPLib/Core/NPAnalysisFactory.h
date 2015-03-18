#ifndef _DETECTORFACTORY_
#define _DETECTORFACTORY_
#include"NPVAnalysis.h"
#include"NPAnalysisFactory.h"
#include<map>
#include<string>

typedef NPA::VAnalysis* (*ClassAnalysisFactoryFn)();

namespace NPA{
  class AnalysisFactory{
    private:
      AnalysisFactory();
      ~AnalysisFactory();

    public:
      static AnalysisFactory* getInstance();
      NPA::VAnalysis* Construct();
      void SetConstructor(ClassAnalysisFactoryFn fn);

    private:
      static AnalysisFactory* m_Instance;
      // Map that Match the Token with Constructor, once the lib is loaded
      ClassAnalysisFactoryFn m_Construct;
  };
}
#endif
