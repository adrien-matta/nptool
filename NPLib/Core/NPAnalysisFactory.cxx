#include"NPAnalysisFactory.h"
using namespace NPL;
#include<fstream>
#include<iostream>
AnalysisFactory* AnalysisFactory::m_Instance = 0;

////////////////////////////////////////////////////////////////////////////////
AnalysisFactory::AnalysisFactory(){
  m_Construct=NULL;
}

/////////////////////////////////////////////////////////////////////////////////
AnalysisFactory::~AnalysisFactory(){
  m_Instance = 0 ;
}

////////////////////////////////////////////////////////////////////////////////
AnalysisFactory* AnalysisFactory::getInstance(){
  if(!m_Instance){
    m_Instance = new AnalysisFactory();
  }

  return m_Instance;

}
////////////////////////////////////////////////////////////////////////////////
NPL::VAnalysis* AnalysisFactory::Construct(){
  if(m_Construct!=NULL)
    return  m_Construct();

  return NULL;
}
////////////////////////////////////////////////////////////////////////////////
void AnalysisFactory::SetConstructor(ClassAnalysisFactoryFn fn){
  m_Construct = fn;
}
