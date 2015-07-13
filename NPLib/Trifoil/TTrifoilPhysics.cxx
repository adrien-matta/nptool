/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : November 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Trifoil treated data                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include "TTrifoilPhysics.h"

//   STL
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <limits>

//   NPL
#include "RootInput.h"
#include "NPDetectorFactory.h"
#include "RootOutput.h"
#include "TAsciiFile.h"
//   ROOT
#include "TChain.h"
///////////////////////////////////////////////////////////////////////////

ClassImp(TTrifoilPhysics)
  ///////////////////////////////////////////////////////////////////////////
  TTrifoilPhysics::TTrifoilPhysics() { 
    m_EventPhysics = this;
    m_EventData = new TTrifoilData();
  }

///////////////////////////////////////////////////////////////////////////
void TTrifoilPhysics::BuildSimplePhysicalEvent(){ 
  BuildPhysicalEvent();
}

///////////////////////////////////////////////////////////////////////////
void TTrifoilPhysics::BuildPhysicalEvent(){ 
  unsigned int mysize = m_EventData->GetMultiplicity();

  for (unsigned int i = 0 ; i < mysize ; i++){
    TH1F h = m_EventData->GetWaveform(i);
    unsigned int bins = h.GetNbinsX();
    for(unsigned int b = 1 ; b < bins ; b++){
      double diff =  h.GetBinContent(b)-h.GetBinContent(b+1);
        if(diff<-1000){
          Time.push_back(b); 
          Energy.push_back(diff);
        }
      }
    }
  }


///////////////////////////////////////////////////////////////////////////
void TTrifoilPhysics::Clear(){
  Energy.clear() ;
  Time.clear() ;
}
///////////////////////////////////////////////////////////////////////////

////   Innherited from VDetector Class   ////            

///////////////////////////////////////////////////////////////////////////
void TTrifoilPhysics::ReadConfiguration(string Path)  {
  /*Trifoil has only one key word, so do nothing*/
  Path.clear();
}

///////////////////////////////////////////////////////////////////////////
void TTrifoilPhysics::AddParameterToCalibrationManager()   {
}

///////////////////////////////////////////////////////////////////////////
void TTrifoilPhysics::InitializeRootInputRaw(){
  TChain* inputChain = RootInput::getInstance()->GetChain();
  static UInt_t* found =  new UInt_t[100] ;
  inputChain->SetBranchStatus( "Trifoil" , true );
  inputChain->SetBranchStatus( "fTrifoil_*" , true , found);
  inputChain->SetBranchAddress( "Trifoil" , &m_EventData );
}

///////////////////////////////////////////////////////////////////////////
void TTrifoilPhysics::InitializeRootInputPhysics(){
  TChain* inputChain = RootInput::getInstance()->GetChain();
  inputChain->SetBranchStatus("EventType",true);
  inputChain->SetBranchStatus("Time",true);
  inputChain->SetBranchAddress("Trifoil",&m_EventPhysics);
}

///////////////////////////////////////////////////////////////////////////
void TTrifoilPhysics::InitializeRootOutput(){
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  outputTree->Branch( "Trifoil" , "TTrifoilPhysics" , &m_EventPhysics );
}
////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TTrifoilPhysics::Construct(){
  return (NPL::VDetector*) new TTrifoilPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy{
  public:
    proxy(){
      NPL::DetectorFactory::getInstance()->AddToken("Trifoil","Trifoil");
      NPL::DetectorFactory::getInstance()->AddDetector("Trifoil",TTrifoilPhysics::Construct);
    }
};

proxy p;
}

