/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@lpccaen.in2p3.fr    *
 *                                                                           *
 * Creation Date  : november 2009                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold ModularLeaf  Physics                                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

//   NPL
#include "TModularLeafPhysics.h"
#include "RootOutput.h"
#include "RootInput.h"
#include "NPDetectorFactory.h"
#include "NPOptionManager.h"

//   STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <stdlib.h>
using namespace std;

//   ROOT
#include "TChain.h"

//   tranform an integer to a string
string itoa(int value){
  char buffer [33];
  sprintf(buffer,"%d",value);
  return buffer;
}

ClassImp(TModularLeafPhysics)
  ///////////////////////////////////////////////////////////////////////////
TModularLeafPhysics::TModularLeafPhysics(){      
  NumberOfDetector = 0 ;
  //EventData = new TModularLeafData ;
  EventPhysics = this ;
}

///////////////////////////////////////////////////////////////////////////
TModularLeafPhysics::~TModularLeafPhysics(){
  }

///////////////////////////////////////////////////////////////////////////
void TModularLeafPhysics::Clear() {
 std::map<std::string,double>::iterator it ;
 for( it=m_CalibratedData.begin();it!=m_CalibratedData.end();it++){
   it->second=m_DefaultValue; 
   }
}
///////////////////////////////////////////////////////////////////////////
void TModularLeafPhysics::ClearEventData() {
 std::map<std::string,short>::iterator it ;
 for( it=m_RawData.begin();it!=m_RawData.end();it++){
   it->second=m_DefaultValue; 
   }
}


///////////////////////////////////////////////////////////////////////////
void TModularLeafPhysics::ReadConfiguration(NPL::InputParser parser) {
  vector<NPL::InputBlock*> blocks = parser.GetAllBlocksWithToken("ModularLeaf");
  if(NPOptionManager::getInstance()->GetVerboseLevel())
    cout << "//// " << blocks.size() << " modular leafs blocks found " << endl; 

  vector<string> token = {"Leafs","DefaultValue"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  ModularLeaf " << i+1 <<  endl;
      
      m_DefaultValue = blocks[i]->GetDouble("DefaultValue","void");
      std::vector<std::string> leafs = blocks[i]->GetVectorString("Leafs"); 
      unsigned int size = leafs.size();
      for(unsigned int l = 0 ; l < size ; l++){
        m_CalibratedData[leafs[l]] = m_DefaultValue;
        m_RawData[leafs[l]] = m_DefaultValue;
      }
    }

    else{
      cout << "ERROR: check your input file formatting " << endl;
      exit(1);
    }
  }
}

///////////////////////////////////////////////////////////////////////////
void TModularLeafPhysics::AddParameterToCalibrationManager(){
  CalibrationManager* Cal = CalibrationManager::getInstance();
  std::map<std::string,double>::iterator it;
  for(it = m_CalibratedData.begin() ; it!=m_CalibratedData.end() ; it++)
      Cal->AddParameter("ModularLeaf", it->first,it->first)   ;
}

///////////////////////////////////////////////////////////////////////////
void TModularLeafPhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  std::map<std::string,short>::iterator it;
  for(it = m_RawData.begin() ; it!=m_RawData.end() ; it++){
   inputChain->SetBranchStatus ( (it->first).c_str(), true );
   inputChain->SetBranchAddress( (it->first).c_str(), &(it->second) )  ; 
  }
}
///////////////////////////////////////////////////////////////////////////
void TModularLeafPhysics::InitializeRootInputPhysics(){
  TChain* inputChain = RootInput::getInstance()->GetChain();
  std::map<std::string,double>::iterator it;
  for(it = m_CalibratedData.begin() ; it!=m_CalibratedData.end() ; it++){
   inputChain->SetBranchStatus ( (it->first).c_str(), true );
   inputChain->SetBranchAddress( (it->first).c_str(), &(it->second) )  ; 
  }
}

///////////////////////////////////////////////////////////////////////////
void TModularLeafPhysics::InitializeRootOutput(){
  TTree* outputTree = RootOutput::getInstance()->GetTree();
  std::map<std::string,double>::iterator it;
  for(it = m_CalibratedData.begin() ; it!=m_CalibratedData.end() ; it++){
   outputTree->Branch( (it->first).c_str(),  &(m_CalibratedData[it->first]))  ; 
   }
}

///////////////////////////////////////////////////////////////////////////
void TModularLeafPhysics::BuildPhysicalEvent(){
  static CalibrationManager* Cal = CalibrationManager::getInstance();
  std::map<std::string,short>::iterator it;

  for(it = m_RawData.begin() ; it != m_RawData.end() ; it++){
    if(it->second!=m_DefaultValue){
      static string param;
      param="ModularLeaf/"+it->first;
      m_CalibratedData[it->first] = Cal->ApplyCalibration(param,it->second);
      }
    }
}

///////////////////////////////////////////////////////////////////////////
void TModularLeafPhysics::BuildSimplePhysicalEvent(){

}

////////////////////////////////////////////////////////////////////////////////
//            Construct Method to be pass to the DetectorFactory              //
////////////////////////////////////////////////////////////////////////////////
NPL::VDetector* TModularLeafPhysics::Construct(){
  return (NPL::VDetector*) new TModularLeafPhysics();
}

////////////////////////////////////////////////////////////////////////////////
//            Registering the construct method to the factory                 //
////////////////////////////////////////////////////////////////////////////////
extern "C"{
class proxy_modularleaf{
  public:
    proxy_modularleaf(){
      NPL::DetectorFactory::getInstance()->AddToken("ModularLeaf","ModularLeaf");
      NPL::DetectorFactory::getInstance()->AddDetector("ModularLeaf",TModularLeafPhysics::Construct);
    }
};

proxy_modularleaf p_modularleaf;
}

