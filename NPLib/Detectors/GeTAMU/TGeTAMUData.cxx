/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: a.matta@surrey.ac.uk      *
 *                                                                           *
 * Creation Date  : November 2012                                            *
 * Last update    : November 2016                                            *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold the GeTAMU  raw data                                     *
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

#include "TGeTAMUData.h"

ClassImp(TGeTAMUData)

  /////////////////////////
  TGeTAMUData::TGeTAMUData(){
  }

/////////////////////////
TGeTAMUData::~TGeTAMUData(){
}

/////////////////////////
void TGeTAMUData::Clear(){
  fGeTAMU_Core_CloverNbr.clear();
  fGeTAMU_Core_CrystalNbr.clear();
  fGeTAMU_Core_Energy.clear();
  fGeTAMU_Core_Time.clear();

  fGeTAMU_Segment_CloverNbr.clear();
  fGeTAMU_Segment_SegmentNbr.clear();
  fGeTAMU_Segment_Energy.clear();
  fGeTAMU_Segment_Time.clear();
}

/////////////////////////
void TGeTAMUData::Dump() const{

 cout << "Core Multiplicity = " << fGeTAMU_Core_CloverNbr.size() << endl;  
  for (unsigned int i = 0; i <  fGeTAMU_Core_CloverNbr.size(); i++){
    cout << " Clover: " <<  fGeTAMU_Core_CloverNbr[i] 
         << " Crystal: " << fGeTAMU_Core_CrystalNbr[i]  
         << " Energy: " <<  fGeTAMU_Core_Energy[i]  
         << " Time: " <<    fGeTAMU_Core_Time[i] << endl;
  }

  cout << "Segment Multiplicity = " <<  fGeTAMU_Segment_CloverNbr.size() << endl;  
  for (unsigned int i = 0; i < fGeTAMU_Segment_CloverNbr.size(); i++){
    cout << " Clover: " <<  fGeTAMU_Segment_CloverNbr[i]  
         << " Segment: " << fGeTAMU_Segment_SegmentNbr[i]  
         << " Energy: " <<  fGeTAMU_Segment_Energy[i]  
         << " Time: " <<    fGeTAMU_Segment_Time[i] << endl;
  }

}
