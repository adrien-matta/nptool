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

ClassImp(TModularLeafPhysics);
  ///////////////////////////////////////////////////////////////////////////
  TModularLeafPhysics::TModularLeafPhysics(){      
    m_NumberOfDetector = 0 ;
    m_EventPhysics = this ;
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
////////////////////////////////////////////////////////////////////////
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

  vector<std::string> token = {"Leafs","DefaultValue", "X", "Y"};

  for(unsigned int i = 0 ; i < blocks.size() ; i++){
    if(blocks[i]->HasTokenList(token)){
      if(NPOptionManager::getInstance()->GetVerboseLevel())
        cout << endl << "////  ModularLeaf " << i+1 <<  endl;

      m_DefaultValue = blocks[i]->GetDouble("DefaultValue", "void");
      m_leafs = blocks[i]->GetVectorString("Leafs"); 
      m_leafs_X = blocks[i]->GetVectorString("X");
      m_leafs_Y = blocks[i]->GetVectorString("Y");
      unsigned int size = m_leafs.size();

      for(unsigned int l = 0 ; l < size ; l++){
        m_CalibratedData[m_leafs[l]] = m_DefaultValue;
        m_RawData[m_leafs[l]] = m_DefaultValue;
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
  // std::map<std::string,double>::iterator it;
  // for(it = m_CalibratedData.begin() ; it!it->first=m_CalibratedData.end() ; it++)
  unsigned int leafs_size = m_leafs.size();
  for(unsigned int i = 0; i < leafs_size; i++){
    Cal->AddParameter(m_leafs[i], "", m_leafs[i]);
  }
}

///////////////////////////////////////////////////////////////////////////
void TModularLeafPhysics::InitializeRootInputRaw() {
  TChain* inputChain = RootInput::getInstance()->GetChain();
  std::map<std::string,short>::iterator it;
  for(it = m_RawData.begin() ; it!=m_RawData.end() ; it++){
    inputChain->SetBranchStatus ( (it->first).c_str(), true );
    inputChain->SetBranchAddress( (it->first).c_str(), &(it->second) )  ; 
    // cout << it->second << endl; 
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
cout << 1 << endl;
  static CalibrationManager* Cal = CalibrationManager::getInstance();
  static string name;
  std::map<std::string,short>::iterator it;

cout << 2 << endl;
  for(it = m_RawData.begin() ; it != m_RawData.end() ; it++){
    // cout << it->first << " " << it->second << endl;
    name = it->first + "/" +it->first; 
    cout << name << endl;
    if(it->second != m_DefaultValue){
      m_CalibratedData[it->first] = Cal->ApplyCalibration(name, it->second);
    }
  }
cout << 3 << endl;
}

///////////////////////////////////////////////////////////////////////////
void TModularLeafPhysics::BuildSimplePhysicalEvent(){
	TModularLeafPhysics::BuildPhysicalEvent();
}

/////////////////////////////////////////////////////////////////////////
// Spectra function needed by the spectra server 
//////////////////////////////////////////////////////////////////////////
void TModularLeafPhysics::InitSpectra(){
  m_Spectra = new TModularLeafSpectra(m_leafs, m_leafs_X, m_leafs_Y);
}

///////////////////////////////////////////////////////////////////////////
void TModularLeafPhysics::FillSpectra(){
  // cout << "TModularLeafPhysics::FillSpectra" << endl;
  m_Spectra->FillRawSpectra(m_RawData);
  m_Spectra->FillPreTreatedSpectra(m_CalibratedData);
  m_Spectra->FillPhysicsSpectra(m_leafs_X, m_leafs_Y, m_CalibratedData);
}
///////////////////////////////////////////////////////////////////////////
void TModularLeafPhysics::CheckSpectra(){
  m_Spectra->CheckSpectra();
}
///////////////////////////////////////////////////////////////////////////
void TModularLeafPhysics::ClearSpectra(){
  // To be done
}

///////////////////////////////////////////////////////////////////////////
void TModularLeafPhysics::WriteSpectra() {
  if(m_Spectra){
    m_Spectra->WriteSpectra();
  }
}

///////////////////////////////////////////////////////////////////////////
map< string , TH1*> TModularLeafPhysics::GetSpectra() {
  if(m_Spectra)
    return m_Spectra->GetMapHisto();
  else{
    map< string , TH1*> empty;
    return empty;
  }
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

