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
#include "RootOutput.h"
#include "TAsciiFile.h"
//   ROOT
#include "TChain.h"
///////////////////////////////////////////////////////////////////////////

ClassImp(TTrifoilPhysics)
///////////////////////////////////////////////////////////////////////////
TTrifoilPhysics::TTrifoilPhysics() 
   { 
      
   }
      
///////////////////////////////////////////////////////////////////////////
void TTrifoilPhysics::BuildSimplePhysicalEvent()
   { 
     BuildPhysicalEvent();
   }
   
///////////////////////////////////////////////////////////////////////////
   
void TTrifoilPhysics::BuildPhysicalEvent()
   { 
  
   }


///////////////////////////////////////////////////////////////////////////
void TTrifoilPhysics::Clear(){
  EventType.clear() ;
  Time.clear() ;
  m_EventData->Clear();

}
///////////////////////////////////////////////////////////////////////////

////   Innherited from VDetector Class   ////            
            
///////////////////////////////////////////////////////////////////////////
void TTrifoilPhysics::ReadConfiguration(string Path)  
{
 /*Trifoil has only one key word, so do nothing*/
  Path.clear();
}

///////////////////////////////////////////////////////////////////////////
void TTrifoilPhysics::AddParameterToCalibrationManager()   
   {
   
   }

///////////////////////////////////////////////////////////////////////////
void TTrifoilPhysics::InitializeRootInputRaw() 
   {
      TChain* inputChain = RootInput::getInstance()->GetChain()   ;
      inputChain->SetBranchStatus( "Trifoil" , true )               ;
      inputChain->SetBranchStatus( "fTrifoil_*" , true )               ;
      inputChain->SetBranchAddress( "Trifoil" , &m_EventData )      ;
   }
   
///////////////////////////////////////////////////////////////////////////
void TTrifoilPhysics::InitializeRootInputPhysics() 
   {
      TChain* inputChain = RootInput::getInstance()->GetChain();
      inputChain->SetBranchStatus( "EventType" , true );
      inputChain->SetBranchStatus( "Time" , true );
   }

///////////////////////////////////////////////////////////////////////////
void TTrifoilPhysics::InitializeRootOutput()    
   {
      TTree* outputTree = RootOutput::getInstance()->GetTree();
      outputTree->Branch( "Trifoil" , "TTrifoilPhysics" , &m_EventPhysics );
   }
