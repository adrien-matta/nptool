/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
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
#include "NPOptionManager.h"

//   ROOT
#include "TChain.h"
#include "TSpectrum.h"
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
  double base,maxi,mytime; 
  for (unsigned int i = 0 ; i < mysize ; i++){
    TH1F* h = m_EventData->GetWaveform(i);
    base =  h->GetBinContent(h->GetMinimumBin());  
    maxi = h->GetBinContent(h->GetMaximumBin());
    if(maxi>2000 && base>-300){
      mytime = (h->GetMaximumBin());
      TimeStamp.push_back(m_EventData->GetTimeStamp(i)*10);
      Time.push_back(mytime);
      Energy.push_back(maxi);
    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TTrifoilPhysics::Clear(){
  Energy.clear() ;
  Time.clear() ;
  TimeStamp.clear();
}
///////////////////////////////////////////////////////////////////////////

////   Innherited from VDetector Class   ////            

///////////////////////////////////////////////////////////////////////////
void TTrifoilPhysics::ReadConfiguration(NPL::InputParser parser){
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("Trifoil");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " detectors found " << endl; 
}

///////////////////////////////////////////////////////////////////////////
void TTrifoilPhysics::AddParameterToCalibrationManager()   {
}

///////////////////////////////////////////////////////////////////////////
void TTrifoilPhysics::InitializeRootInputRaw(){
  TChain* inputChain = RootInput::getInstance()->GetChain();
  static UInt_t* found =  new UInt_t[100] ;
  inputChain->SetBranchStatus( "Trifoil" , true );
  if(inputChain->FindBranch( "fTrifoil_*")) 
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
class proxy_trifoil{
  public:
    proxy_trifoil(){
      NPL::DetectorFactory::getInstance()->AddToken("Trifoil","Trifoil");
      NPL::DetectorFactory::getInstance()->AddDetector("Trifoil",TTrifoilPhysics::Construct);
    }
};

proxy_trifoil p;
}

