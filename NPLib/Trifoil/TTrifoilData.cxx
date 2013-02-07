/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : December 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold Trifoil Raw data                                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/

// STL
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

// NPL
#include "TTrifoilData.h"

//ROOT
#include "TDirectory.h"

ClassImp(TTrifoilData)

/////////////////////////
TTrifoilData::TTrifoilData()
{

}

/////////////////////////
TTrifoilData::~TTrifoilData()
{
}

/////////////////////////
void TTrifoilData::Clear()
{
  fTrifoil_Waveform.clear();
  fTrifoil_TimeCFD.clear();
  fTrifoil_TimeLED.clear();
}
/////////////////////////
void TTrifoilData::Dump() const
{
}
/////////////////////////
void TTrifoilData::ReadFSPCFile(string FSPCPath){
  ifstream FSPCFile;
  FSPCFile.open(FSPCPath.c_str());
  
  if(!FSPCFile.is_open()) {cout << "Error: FSPC File: " << FSPCPath << " not found " << endl ; exit(1);}
  else {cout << "Trifoil is Reading FSPC file : " << FSPCPath << endl ;}
  
  string LineBuffer;
  string DataBuffer;
  string DetectorName;
  string ChannelNumberStr;
  int    ChannelNumber;
  while(!FSPCFile.eof()){
    getline(FSPCFile,LineBuffer);
    /* Typical line to analyse:
    # 2005: Address 0x00900800, type 8, Trifoil [Trifoil 0x0900800]
    */
    stringstream myLine(LineBuffer);
    myLine >> DataBuffer >> ChannelNumberStr >> DataBuffer >> DataBuffer >> DataBuffer >> DataBuffer >> DetectorName  ;
    ChannelNumberStr = ChannelNumberStr.substr(0,ChannelNumberStr.length()-1);
    stringstream(ChannelNumberStr) >> ChannelNumber;
    
    if(DetectorName=="Trifoil") m_FSPC_Channel=ChannelNumber;
  }
}


void TTrifoilData::FillData(TTigEventFragment* TigEvent){
  vector<int> channel_number = TigEvent->channel_number;
  for(unsigned int i = 0 ; i < channel_number.size() ; i++){
    if(channel_number[i] == m_FSPC_Channel){
      fTrifoil_Waveform.push_back(TigEvent->waveform[i]);
      fTrifoil_TimeCFD.push_back(TigEvent->cfd_value[i]);
      fTrifoil_TimeLED.push_back(TigEvent->led_value[i]);
    }
  }
}
