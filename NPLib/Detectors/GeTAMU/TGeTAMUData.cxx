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
 * Last update    : December 2016 m.moukaddam@suurey.ac.uk                   *
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
  fGeTAMU_Core_CloverNbr_E.clear();
  fGeTAMU_Core_CrystalNbr_E.clear();
  fGeTAMU_Core_Energy.clear();

  fGeTAMU_Core_CloverNbr_T.clear();
  fGeTAMU_Core_CrystalNbr_T.clear();
  fGeTAMU_Core_Time.clear();

  fGeTAMU_Segment_CloverNbr_E.clear();
  fGeTAMU_Segment_SegmentNbr_E.clear();
  fGeTAMU_Segment_Energy.clear();

  fGeTAMU_Segment_CloverNbr_T.clear();
  fGeTAMU_Segment_SegmentNbr_T.clear();
  fGeTAMU_Segment_Time.clear();
}

/////////////////////////
void TGeTAMUData::Dump() const{

 cout << "Core Energy Multiplicity = " << fGeTAMU_Core_CloverNbr_E.size() << endl;  
  for (unsigned int i = 0; i <  fGeTAMU_Core_CloverNbr_E.size(); i++){
    cout << " Clover: " <<  fGeTAMU_Core_CloverNbr_E[i] 
         << " Crystal: " << fGeTAMU_Core_CrystalNbr_E[i]  
         << " Energy: " <<  fGeTAMU_Core_Energy[i] << endl;  
  }

 cout << "Core Time Multiplicity = " << fGeTAMU_Core_CloverNbr_T.size() << endl;  
  for (unsigned int i = 0; i <  fGeTAMU_Core_CloverNbr_T.size(); i++){
    cout << " Clover: " <<  fGeTAMU_Core_CloverNbr_T[i] 
         << " Crystal: " << fGeTAMU_Core_CrystalNbr_T[i]  
         << " Time: " <<    fGeTAMU_Core_Time[i] << endl;
  }

  cout << "Segment Energy Multiplicity = " <<  fGeTAMU_Segment_CloverNbr_E.size() << endl;  
  for (unsigned int i = 0; i < fGeTAMU_Segment_CloverNbr_E.size(); i++){
    cout << " Clover: " <<  fGeTAMU_Segment_CloverNbr_E[i]  
         << " Segment: " << fGeTAMU_Segment_SegmentNbr_E[i]  
         << " Energy: " <<  fGeTAMU_Segment_Energy[i] << endl;   
  }

  cout << "Segment Time Multiplicity = " <<  fGeTAMU_Segment_CloverNbr_T.size() << endl;  
  for (unsigned int i = 0; i < fGeTAMU_Segment_CloverNbr_T.size(); i++){
    cout << " Clover: " <<  fGeTAMU_Segment_CloverNbr_T[i]  
         << " Segment: " << fGeTAMU_Segment_SegmentNbr_T[i]  
         << " Time: " <<    fGeTAMU_Segment_Time[i] << endl;
  }

}
