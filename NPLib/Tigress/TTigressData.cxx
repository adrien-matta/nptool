/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : November 2012                                            *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold the Tigress  raw data (Made for TIG10 card)              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include "TTigressData.h"

ClassImp(TTigressData)

/////////////////////////
TTigressData::TTigressData()
{
}

/////////////////////////
TTigressData::~TTigressData()
{
}

/////////////////////////
void TTigressData::Clear()
{
  fTIG_Ge_CloverNbr.clear();
  fTIG_Ge_CrystalNbr.clear();
  fTIG_Ge_SegmentNbr.clear();
  fTIG_Ge_Energy.clear();
  fTIG_Ge_TimeCFD.clear();
  fTIG_Ge_TimeLED.clear();

  fTIG_BGO_CloverNbr.clear();
  fTIG_BGO_CrystalNbr.clear();
  fTIG_BGO_PmNbr.clear();
  fTIG_BGO_Energy.clear();
  fTIG_BGO_TimeCFD.clear();
  fTIG_BGO_TimeLED.clear();
}

/////////////////////////
void TTigressData::Dump() const
{
  // Energy
 // cout << "Tigress_Mult = " << fTIG_CloverNbr.size() << endl;
  
  // Front
 // for (UShort_t i = 0; i < fTIG_CloverNbr.size(); i++){
 //   cout << "Clover: " << fTIG_CloverNbr[i]
 //        << " Crystal: " << fTIG_CrystalNbr[i]
 //        << " Energy: " << fTIG_Energy[i]
 //        << " Time: " << fTIG_Time[i] << endl;
 // }
}

// Key is the FSPC channel,
// vector[0] is the Clover Nbr
// vector[1] is the crystal Nbr
// vector[2] is the segment Nbr
// vector[3] is 0: Ge , 1: BGO
void TTigressData::ReadFSPCFile(string FSPCPath){
  ifstream FSPCFile;
  FSPCFile.open(FSPCPath.c_str());
  
  if(!FSPCFile.is_open()) {cout << "Error: FSPC File: " << FSPCPath << " not found " << endl ; exit(1);}
  else {cout << "Tigress is Reading FSPC file : " << FSPCPath << endl ;}
  
  string LineBuffer;
  string DataBuffer;
  string DetectorName;
  string ChannelNumberStr;
  int    ChannelNumber;
  while(!FSPCFile.eof()){
    
    getline(FSPCFile,LineBuffer);
    /* Typical line to analyse: 1st for Ge, 2nd for BGO
     #    9: Address 0x00e00109, type 3, Ge01Bc0 [HPGe Pos01 Blue  Core 9 0x0e00109]
     #   10: Address 0x00e00200, type 4, Sup01Bs0 [Suppressor Pos01 Blue  Seg 0 0x0e00200]
    */
     
    stringstream myLine(LineBuffer);
    myLine >> DataBuffer >> ChannelNumberStr >> DataBuffer >> DataBuffer >> DataBuffer >> DataBuffer>> DetectorName;

    ChannelNumberStr = ChannelNumberStr.substr(0,ChannelNumberStr.length()-1);
    stringstream(ChannelNumberStr) >> ChannelNumber;
    
    vector<int> FSPCInfo;
    if(DetectorName.length()==7 || DetectorName.length()==8 ) {
      if(DetectorName.compare(0,2,"Ge")==0 && DetectorName.length()==7){ // Very specific to avoid conflict with possible future change
        string DetectorName2 = DetectorName.substr(2,2);
        int CloverNbr=0;
        stringstream(DetectorName2) >> CloverNbr;
        
        DetectorName2 = DetectorName.substr(4,1);
        int CrystalNbr=0;
             if (DetectorName2 == "B") CrystalNbr=1;
        else if (DetectorName2 == "G") CrystalNbr=2;
        else if (DetectorName2 == "R") CrystalNbr=3;
        else if (DetectorName2 == "W") CrystalNbr=4;
        else {cout << "ERROR: FSPC File incorrect" << endl ; exit(1);}

        DetectorName2 = DetectorName.substr(6,1);
        int SegmentNbr;
        stringstream(DetectorName2) >> SegmentNbr;
        
        FSPCInfo.push_back(CloverNbr);
        FSPCInfo.push_back(CrystalNbr);
        FSPCInfo.push_back(SegmentNbr);
        FSPCInfo.push_back(0);
        
        if(CloverNbr!=0) m_FSPC2Detector[ChannelNumber]= FSPCInfo;
    
      }
      
      else if(DetectorName.compare(0,3,"Sup")==0 && DetectorName.length()==8){ // Very specific to avoid conflict with possible future change
        string DetectorName2 = DetectorName.substr(3,2);
        int CloverNbr=0;
        stringstream(DetectorName2) >> CloverNbr;
        
        
        DetectorName2 = DetectorName.substr(5,1);
        int CrystalNbr=0;
        if (DetectorName2 == "B") CrystalNbr=1;
        else if (DetectorName2 == "G") CrystalNbr=2;
        else if (DetectorName2 == "R") CrystalNbr=3;
        else if (DetectorName2 == "W") CrystalNbr=4;
        else {cout << "ERROR: FSPC File incorrect" << endl ; exit(1);}
        
        DetectorName2 = DetectorName.substr(7,1);
        int PmNbr;
        stringstream(DetectorName2) >> PmNbr;
        
        FSPCInfo.push_back(CloverNbr);
        FSPCInfo.push_back(CrystalNbr);
        FSPCInfo.push_back(PmNbr);
        FSPCInfo.push_back(1);
        
        if(CloverNbr!=0) m_FSPC2Detector[ChannelNumber]= FSPCInfo;
        
      }
    }
  }
  
  return;
}


void TTigressData::FillData(TTigEventFragment* TigEvent){
  vector<int> channel_number = TigEvent->channel_number;
  for(unsigned int i = 0 ; i < channel_number.size() ; i++){
    if(m_FSPC2Detector.find(channel_number[i])!=m_FSPC2Detector.end()){
      int CloverNbr = m_FSPC2Detector[channel_number[i]][0];
      int CrystalNbr = m_FSPC2Detector[channel_number[i]][1];
      int SegmentNbr = m_FSPC2Detector[channel_number[i]][2];
      int type = m_FSPC2Detector[channel_number[i]][3];
      
      if (type == 0) {
        FillGe(CloverNbr,CrystalNbr,SegmentNbr,i,TigEvent);
      }
      
      else if(type == 1){
        FillBGO(CloverNbr,CrystalNbr,SegmentNbr,i,TigEvent);
      }
      
      else{cout << "ERROR: FSPC map incorrectly instantiate" << endl ; exit(1);}
    }
  }
}

void TTigressData::FillGe(unsigned short GeCloverNbr, unsigned short GeCrystalNbr , unsigned short GeSegmentNbr, int hit,TTigEventFragment* TigEvent){
  SetGeCloverNbr( GeCloverNbr);
  SetGeCrystalNbr( GeCrystalNbr);
  SetGeSegmentNbr( GeSegmentNbr);
  SetGeEnergy(TigEvent->charge_raw[hit]);
  SetGeTimeCFD( TigEvent->cfd_value[hit]);
  SetGeTimeLED( TigEvent->led_value[hit]);
}

void TTigressData::FillBGO(unsigned short BGOCloverNbr, unsigned short BGOCrystalNbr , unsigned short BGOPmNbr, int hit,TTigEventFragment* TigEvent){
  SetBGOCloverNbr( BGOCloverNbr);
  SetBGOCrystalNbr( BGOCrystalNbr);
  SetBGOPmNbr( BGOPmNbr);
  SetBGOEnergy( TigEvent->charge_raw[hit]);
  SetBGOTimeCFD( TigEvent->cfd_value[hit]);
  SetBGOTimeLED( TigEvent->led_value[hit]);
}

