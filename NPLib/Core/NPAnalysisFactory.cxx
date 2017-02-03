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
