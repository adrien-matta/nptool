#include"NPAnalysisFactory.h"
using namespace NPA;
#include<fstream>
#include<iostream>
AnalysisFactory* AnalysisFactory::m_Instance = 0;

////////////////////////////////////////////////////////////////////////////////
AnalysisFactory::AnalysisFactory(){
  m_Construct=NULL;
}

/////////////////////////////////////////////////////////////////////////////////
AnalysisFactory::~AnalysisFactory(){
  if(m_Instance!=0)
    delete m_Instance;

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
NPA::VAnalysis* AnalysisFactory::Construct(){
  if(m_Construct!=NULL)
    return  m_Construct();

  return NULL;
}
////////////////////////////////////////////////////////////////////////////////
void AnalysisFactory::SetConstructor(ClassAnalysisFactoryFn fn){
  m_Construct = fn;
}
