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

// ROOT headers
#include "TFile.h"



////////////////////////////////////////////////////////////////////////////////
Analysis::Analysis()
   : m_cutg_SP_p(new TCutG),
     fEnergy(-1),
     fTime(-1),
     fDetector(-1),
     fStripFront(-1),
     fStripBack(-1),
     fBrho(-1)
{
}



////////////////////////////////////////////////////////////////////////////////
Analysis::~Analysis()
{
}



////////////////////////////////////////////////////////////////////////////////
void Analysis::Init()
{
   // Load graphical cuts
   TFile *fcutg = new TFile("./cuts/cutg_SelectionProtons.root");
   m_cutg_SP_p = (TCutG*) fcutg->Get("run34_plasppos");

   // Get Physical objects
   m_W1 = (TW1Physics*)        m_DetectorManager->GetDetector("W1");
   m_SP = (TSplitPolePhysics*) m_DetectorManager->GetDetector("SplitPole");
}



////////////////////////////////////////////////////////////////////////////////
void Analysis::TreatEvent()
{
   // Clear data
   Clear();

   // select protons in SP (PlasticP vs Position)
   if (!m_cutg_SP_p->IsInside(m_SP->GetPlasticG(), m_SP->GetPlasticP())) {
      // select multiplicity 1 for W1's
      if (m_W1->GetEventMultiplicity() == 1) {
         // select good timing events
         // calculate relative time between SP and W1
         Double_t time = 0;
         // apply timing selection
         if (time > 1200 && time < 1400) {
         }
      }
   }
}



////////////////////////////////////////////////////////////////////////////////
void Analysis::End()
{
}



////////////////////////////////////////////////////////////////////////////////
void Analysis::Clear()
{
   // W1's
   fEnergy     = -1;
   fTime       = -1;
   fDetector   = -1;
   fStripFront = -1;
   fStripBack  = -1;
   // SP
   fBrho       = -1;
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

