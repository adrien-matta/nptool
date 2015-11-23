/*****************************************************************************
 * Copyright (C) 2009-2014    this file is part of the NPTool Project        *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : November 2015                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *    Analysis for the SP + W1 coincidence setup                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

// Analysis header
#include"Analysis.h"

// C++ headers
#include <iostream>
using namespace std;

// NPTool headers
#include"NPAnalysisFactory.h"
#include"NPDetectorManager.h"


////////////////////////////////////////////////////////////////////////////////
Analysis::Analysis()
{
}



////////////////////////////////////////////////////////////////////////////////
Analysis::~Analysis()
{
}



////////////////////////////////////////////////////////////////////////////////
void Analysis::Init()
{
   m_W1 = (TW1Physics*)        m_DetectorManager->GetDetector("W1");
   m_SP = (TSplitPolePhysics*) m_DetectorManager->GetDetector("SplitPole");
}



////////////////////////////////////////////////////////////////////////////////
void Analysis::TreatEvent()
{
   cout << "pipo" << endl;
}



////////////////////////////////////////////////////////////////////////////////
void Analysis::End()
{
}



////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VAnalysis* Analysis::Construct(){
  return (NPL::VAnalysis*) new Analysis();
}



////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy{
   public:
      proxy(){
         NPL::AnalysisFactory::getInstance()->SetConstructor(Analysis::Construct);
      }
};

proxy p;
}

