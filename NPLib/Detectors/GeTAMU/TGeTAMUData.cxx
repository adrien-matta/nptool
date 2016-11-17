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

 cout << "Core Multiplicity = " << GetMultiplicityCore() << endl;  
  for (UShort_t i = 0; i < GetMultiplicityCore(); i++){
    cout << " Clover: " <<  fGeTAMU_Core_CloverNbr[i] << endl; 
         << " Crystal: " << fGeTAMU_Core_CrystalNbr[i] << endl; 
         << " Energy: " <<  fGeTAMU_Core_Energy[i] << endl; 
         << " Time: " <<    fGeTAMU_Core_Time[i] << endl;
  }

  cout << "Segment Multiplicity = " << GetMultiplicityCore() << endl;  
  for (UShort_t i = 0; i < GetMultiplicityCore(); i++){
    cout << " Clover: " <<  fGeTAMU_Segment_CloverNbr[i] << endl; 
         << " Segment: " << fGeTAMU_Segment_SegmentNbr[i] << endl; 
         << " Energy: " <<  fGeTAMU_Segment_Energy[i] << endl; 
         << " Time: " <<    fGeTAMU_Segment_Time[i] << endl;
  }

}
