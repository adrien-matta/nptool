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
 *  This class hold Sharc Raw data                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *   
 *                                                                           *
 *****************************************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std; 

#include "TSharcData.h"

ClassImp(TSharcData)

/////////////////////////
TSharcData::TSharcData()
{
}

/////////////////////////
TSharcData::~TSharcData()
{
}

/////////////////////////
void TSharcData::Clear()
{
  fSharc_StripFront_DetectorNbr.clear();
  fSharc_StripFront_StripNbr.clear();
  fSharc_StripFront_Energy.clear();
  fSharc_StripFront_TimeCFD.clear();
  fSharc_StripFront_TimeLED.clear();

  fSharc_StripBack_DetectorNbr.clear();
  fSharc_StripBack_StripNbr.clear();
  fSharc_StripBack_Energy.clear();
  fSharc_StripBack_TimeCFD.clear();
  fSharc_StripBack_TimeLED.clear();
  
  fSharc_PAD_DetectorNbr.clear();
  fSharc_PAD_Energy.clear();
  fSharc_PAD_TimeCFD.clear();
  fSharc_PAD_TimeLED.clear();

}

/////////////////////////
void TSharcData::Dump() const
{
  // Energy
  cout << "Sharc_StripFront_Mult = " << fSharc_StripFront_DetectorNbr.size() << endl;
  
  // Front
  for (UShort_t i = 0; i < fSharc_StripFront_DetectorNbr.size(); i++){
    cout << "DetNbr: " << fSharc_StripFront_DetectorNbr[i]
         << " Strip: " << fSharc_StripFront_StripNbr[i]
         << " Energy: " << fSharc_StripFront_Energy[i]
         << " Time CFD: " << fSharc_StripFront_TimeCFD[i]
         << " Time LED: " << fSharc_StripFront_TimeLED[i] << endl;
  }
  
  // Back
  for (UShort_t i = 0; i < fSharc_StripFront_DetectorNbr.size(); i++){
    cout << "DetNbr: " << fSharc_StripFront_DetectorNbr[i]
    << " Strip: " << fSharc_StripFront_StripNbr[i]
    << " Energy: " << fSharc_StripFront_Energy[i]
    << " Time CFD: " << fSharc_StripBack_TimeCFD[i]
    << " Time LED: " << fSharc_StripBack_TimeLED[i] << endl;
  }
  
  // PAD
  for (UShort_t i = 0; i < fSharc_PAD_DetectorNbr.size(); i++){
    cout << "DetNbr: " << fSharc_PAD_DetectorNbr[i]
    << " Energy: " << fSharc_PAD_Energy[i]
    << " Time CFD: " << fSharc_PAD_TimeCFD[i]
    << " Time LED: " << fSharc_PAD_TimeLED[i] << endl;

  }
}
/////////////////////////
void TSharcData::ReadFSPCFile(string FSPCPath){
  ifstream FSPCFile;
  FSPCFile.open(FSPCPath.c_str());
  
  if(!FSPCFile.is_open()) {cout << "Error: FSPC File: " << FSPCPath << " not found " << endl ; exit(1);}
  else {cout << "Sharc is Reading FSPC file : " << FSPCPath << endl ;}
  
  string LineBuffer;
  string DataBuffer;
  string DetectorName;
  string FrontBack;
  string ChannelNumberStr;
  int    ChannelNumber;
  int    StripNumber;
  while(!FSPCFile.eof()){
    
    getline(FSPCFile,LineBuffer);
    // Typical line to analyse: #  960: Address 0x00100700, type 6, UmCD1 Front 00 [TIG64-SHARC Up CD 1 Front (UmCD1f 00) 0x0100700]
    stringstream myLine(LineBuffer);
    myLine >> DataBuffer >> ChannelNumberStr >> DataBuffer >> DataBuffer >> DataBuffer >> DataBuffer >> DetectorName >> FrontBack >> StripNumber ;
    
    ChannelNumberStr = ChannelNumberStr.substr(0,ChannelNumberStr.length()-1);
    stringstream(ChannelNumberStr) >> ChannelNumber;
    
    //cout << ChannelNumber << " " << DetectorName << " " << FrontBack << " " << StripNumber << endl ;

    // Key is the FSPC channel,
    // vector[0] is the Detector Nbr
    // vector[1] is 0: QQQ , 1: Box, 2: PAD
    // vector[2] the type: 0: Front , 1: Back1 or Back , 2: Back2
    // vector[3] strip nbr (FSPC ref)
    
    vector<int> FSPCInfo;
    if(DetectorName.length()==5 ) {
      if(DetectorName.compare(2,2,"CD")==0){
        string DetectorName2 = DetectorName.substr(4);
        int DetectorNbr;
        stringstream(DetectorName2) >> DetectorNbr;
        
        if(DetectorName.compare(0,1,"U")==0)      {/*do nothing*/;}
        else if(DetectorName.compare(0,1,"D")==0) DetectorNbr += 12;
        
        
        FSPCInfo.push_back(DetectorNbr);
        FSPCInfo.push_back(0);
        if(FrontBack == "Front")
          FSPCInfo.push_back(0);
        else if (FrontBack == "Back")
          FSPCInfo.push_back(1);
        else {cout << "Error: FSPC File Incorrectly formatted" << endl ; exit(1);}
        FSPCInfo.push_back(StripNumber);
        m_FSPC2Detector[ChannelNumber]= FSPCInfo;
      }
      
      else if(DetectorName.compare(2,2,"Bx")==0){
        string DetectorName2 = DetectorName.substr(4);
        int DetectorNbr;
        stringstream(DetectorName2) >> DetectorNbr;
        
             if(DetectorName.compare(0,1,"U")==0) DetectorNbr += 4;
        else if(DetectorName.compare(0,1,"D")==0) DetectorNbr += 8;
        
        
        FSPCInfo.push_back(DetectorNbr);
        FSPCInfo.push_back(1);
        if(FrontBack == "Front")
          FSPCInfo.push_back(0);
        else if (FrontBack == "Back1")
          FSPCInfo.push_back(1);
        else if (FrontBack == "Back2")
          FSPCInfo.push_back(2);
        else {cout << "Error: FSPC File Incorrectly formatted" << endl ; exit(1);}
        FSPCInfo.push_back(StripNumber);
        m_FSPC2Detector[ChannelNumber]= FSPCInfo;
      }
      
      else if(DetectorName=="SHARC"){
        if(FrontBack=="DoBx"){/*do nothing*/;}
        else {cout << "Error: FSPC File Incorrectly formatted" << endl ; exit(1);}
        int DetectorNbr=0;
        
             if(ChannelNumber==1984) DetectorNbr = 9;
        else if(ChannelNumber==1985) DetectorNbr = 10;
        else if(ChannelNumber==1986) DetectorNbr = 12;
        else if(ChannelNumber==1987) DetectorNbr = 11;
        
        if(DetectorNbr!=0){
          FSPCInfo.push_back(DetectorNbr);
          FSPCInfo.push_back(2);
          FSPCInfo.push_back(StripNumber);
          m_FSPC2Detector[ChannelNumber]= FSPCInfo;
        }
      }
      
    }
  }
  
  return;
}

void TSharcData::FillData(TTigEventFragment* TigEvent){
  vector<int> channel_number = TigEvent->channel_number;
  for(unsigned int i = 0 ; i < channel_number.size() ; i++){
    if(m_FSPC2Detector.find(channel_number[i])!=m_FSPC2Detector.end()){
      int DetNbr = m_FSPC2Detector[channel_number[i]][0];
      int type = m_FSPC2Detector[channel_number[i]][1];
      int FB = m_FSPC2Detector[channel_number[i]][2];
      if(type == 0 ){
             if (FB == 0 ) FillQQQFront(DetNbr , i , TigEvent);
        else if (FB == 1 ) FillQQQBack (DetNbr , i , TigEvent);
      }
      
      else if (type == 1 ){
             if (FB == 0 ) FillBoxFront (DetNbr , i , TigEvent);
        else if (FB == 1 ) FillBoxBack1 (DetNbr , i , TigEvent);
        else if (FB == 2 ) FillBoxBack2 (DetNbr , i , TigEvent);
      }
      
      else if (type == 2 ){
        FillPAD(DetNbr , i , TigEvent);
      }
      
      else{cout << "ERROR: FSPC map incorrectly instantiate" << endl ; exit(1);}
    }
  }
}

// Fill Box
void TSharcData::FillBoxFront(int DetNbr, int hit,TTigEventFragment* TigEvent){
  vector<int> channel_number = TigEvent->channel_number;
  int  StripNbr = m_FSPC2Detector[channel_number[hit]][3];

  if(StripNbr<25){
    SetFront_DetectorNbr(DetNbr);
    SetFront_StripNbr(StripNbr);
    SetFront_Energy(TigEvent->charge_raw[hit]);
    SetFront_TimeCFD(TigEvent->cfd_value[hit]);
    SetFront_TimeLED(TigEvent->led_value[hit]);

  }
}

void TSharcData::FillBoxBack1(int DetNbr, int hit,TTigEventFragment* TigEvent){
  vector<int> channel_number = TigEvent->channel_number;
  int StripNbr = m_FSPC2Detector[channel_number[hit]][3];
  if(StripNbr<25){
    SetBack_DetectorNbr(DetNbr);
    // Box Case, ordering is reversed
      StripNbr = 25 - StripNbr;
    SetBack_StripNbr(StripNbr);
  	SetBack_Energy(TigEvent->charge_raw[hit]);
  	SetBack_TimeCFD(TigEvent->cfd_value[hit]);
    SetBack_TimeLED(TigEvent->led_value[hit]);

  }
}

void TSharcData::FillBoxBack2(int DetNbr, int hit,TTigEventFragment* TigEvent){
  vector<int> channel_number = TigEvent->channel_number;
  int StripNbr = m_FSPC2Detector[channel_number[hit]][3];
  if(StripNbr<25){
    SetBack_DetectorNbr(DetNbr);
  	// Box Case, ordering is reversed
      StripNbr = 25 - StripNbr;
    SetBack_StripNbr(StripNbr+24);
  	SetBack_Energy(TigEvent->charge_raw[hit]);
  	SetBack_TimeCFD(TigEvent->cfd_value[hit]);
    SetBack_TimeLED(TigEvent->led_value[hit]);

  }
}


// Fill QQQ
void TSharcData::FillQQQFront(int DetNbr, int hit,TTigEventFragment* TigEvent){
  vector<int> channel_number = TigEvent->channel_number;
  int  StripNbr = m_FSPC2Detector[channel_number[hit]][3];
  
  if(StripNbr<25){
    SetFront_DetectorNbr(DetNbr);
    SetFront_StripNbr(StripNbr);
    SetFront_Energy(TigEvent->charge_raw[hit]);
    SetFront_TimeCFD(TigEvent->cfd_value[hit]);
    SetFront_TimeLED(TigEvent->led_value[hit]);

  }
}

void TSharcData::FillQQQBack(int DetNbr, int hit,TTigEventFragment* TigEvent){
  vector<int> channel_number = TigEvent->channel_number;
  int StripNbr = m_FSPC2Detector[channel_number[hit]][3];
  if(StripNbr<25){
    SetBack_DetectorNbr(DetNbr);
    SetBack_StripNbr(StripNbr);
  	SetBack_Energy(TigEvent->charge_raw[hit]);
  	SetBack_TimeCFD(TigEvent->cfd_value[hit]);
    SetBack_TimeLED(TigEvent->led_value[hit]);

  }
}

// Fill PAD
void TSharcData::FillPAD(int DetNbr,int hit,TTigEventFragment* TigEvent){
  SetPAD_DetectorNbr(DetNbr);
  SetPAD_Energy(TigEvent->charge_raw[hit]);
  SetPAD_TimeCFD(TigEvent->cfd_value[hit]);
  SetPAD_TimeLED(TigEvent->led_value[hit]);

}

map< int,vector<int> > TSharcData::GetFSPC2Detector()const{
  return m_FSPC2Detector;
}