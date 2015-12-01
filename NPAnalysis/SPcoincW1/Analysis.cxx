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
     fDetector(-1),
     fStripFront(-1),
     fStripBack(-1),
     fBrhoSingle(-1),
     fBrhoCoinc(-1),
     fTime(-10000)
{
}



////////////////////////////////////////////////////////////////////////////////
Analysis::~Analysis()
{
}



////////////////////////////////////////////////////////////////////////////////
void Analysis::Init()
{
   // initialize branches with analysis information
   InitOutputBranch();

   // Load graphical cuts
   TFile *fcutg = new TFile("./cuts/cutg_SelectionProtons.root");
   m_cutg_SP_p = (TCutG*) fcutg->Get("run108_plasppos");

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
   if (m_cutg_SP_p->IsInside(m_SP->GetPlasticG(), m_SP->GetPlasticP())) {
      fBrhoSingle    = m_SP->GetBrho();
      // select multiplicity 1 for SP ref time
      if (m_SP->GetTime2Multiplicity() == 1) {
         // select multiplicity 1 for W1's
         if (m_W1->GetEventMultiplicity() == 1) {
            // select event in W1's
            if (m_W1->GetFrontEnergy(0) > 0) {
               // calculate relative time between SP and W1
               Double_t time = m_W1->GetFrontTime(0)- m_SP->GetTime2(0);
               fTime = time;
               // W1
               fEnergy     = m_W1->GetFrontEnergy(0);
               fDetector   = m_W1->GetDetectorNumber(0);
               fStripFront = m_W1->GetFrontStrip(0);
               fStripBack  = m_W1->GetBackStrip(0);
               // SP
               fBrhoCoinc  = m_SP->GetBrho();
               // apply timing selection
               if (time > 160 && time < 280) {
               }
            }
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
   fDetector   = -1;
   fStripFront = -1;
   fStripBack  = -1;
   // SP
   fBrhoSingle = -1;
   fBrhoCoinc  = -1;
   // time
   fTime       = -10000;
}



////////////////////////////////////////////////////////////////////////////////
void Analysis::InitOutputBranch()
{
   RootOutput::getInstance()->GetTree()->Branch("fEnergy",     &fEnergy,     "fEnergy/D");
   RootOutput::getInstance()->GetTree()->Branch("fTime",       &fTime,       "fTime/D");
   RootOutput::getInstance()->GetTree()->Branch("fDetector",   &fDetector,   "fDetector/I");
   RootOutput::getInstance()->GetTree()->Branch("fStripFront", &fStripFront, "fStripFront/I");
   RootOutput::getInstance()->GetTree()->Branch("fStripBack",  &fStripBack,  "fStripBack/I");
   RootOutput::getInstance()->GetTree()->Branch("fBrhoSingle", &fBrhoSingle, "fBrhoSingle/D");
   RootOutput::getInstance()->GetTree()->Branch("fBrhoCoinc",  &fBrhoSingle, "fBrhoCoinc/D");
}



////////////////////////////////////////////////////////////////////////////////
void Analysis::InitInputBranch()
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

