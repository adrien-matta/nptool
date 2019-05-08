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
TSharcData::TSharcData(){
}

/////////////////////////
TSharcData::~TSharcData(){
}

/////////////////////////
void TSharcData::Clear(){
  fSharc_StripFront_DetectorNbr.clear();
  fSharc_StripFront_StripNbr.clear();
  fSharc_StripFront_Energy.clear();
  fSharc_StripFront_TimeCFD.clear();
  fSharc_StripFront_TimeLED.clear();
  fSharc_StripFront_Time.clear();

  fSharc_StripBack_DetectorNbr.clear();
  fSharc_StripBack_StripNbr.clear();
  fSharc_StripBack_Energy.clear();
  fSharc_StripBack_TimeCFD.clear();
  fSharc_StripBack_TimeLED.clear();
  fSharc_StripBack_Time.clear();
  
  fSharc_PAD_DetectorNbr.clear();
  fSharc_PAD_Energy.clear();
  fSharc_PAD_TimeCFD.clear();
  fSharc_PAD_TimeLED.clear();
  fSharc_PAD_Time.clear();

}

/////////////////////////
void TSharcData::Dump() const{
   
  // Front
  cout << "Sharc Strip Front Mult = " << fSharc_StripFront_DetectorNbr.size() << endl;
  for (UShort_t i = 0; i < fSharc_StripFront_DetectorNbr.size(); i++){
    cout << "DetNbr (Front): " << fSharc_StripFront_DetectorNbr[i]
         << "   Strip: " << fSharc_StripFront_StripNbr[i]
         << "   Energy: " << fSharc_StripFront_Energy[i]
         << "   Time CFD: " << fSharc_StripFront_TimeCFD[i]
         << "   Time LED: " << fSharc_StripFront_TimeLED[i] << endl;
  }

  // Back  
  cout << "Sharc Strip Back Mult  = " << fSharc_StripBack_DetectorNbr.size() << endl;
  for (UShort_t i = 0; i < fSharc_StripBack_DetectorNbr.size(); i++){
    cout << "DetNbr (Back): " << fSharc_StripBack_DetectorNbr[i]
    << "   Strip: " << fSharc_StripBack_StripNbr[i]
    << "   Energy: " << fSharc_StripBack_Energy[i]
    << "   Time CFD: " << fSharc_StripBack_TimeCFD[i]
    << "   Time LED: " << fSharc_StripBack_TimeLED[i] << endl;
  }
  
  // PAD
  cout << "Sharc Strip PAD Mult  = " << fSharc_PAD_DetectorNbr.size() << endl;
  for (UShort_t i = 0; i < fSharc_PAD_DetectorNbr.size(); i++){
    cout << "DetNbr (PAD): " << fSharc_PAD_DetectorNbr[i]
    << "   Energy: " << fSharc_PAD_Energy[i]
    << "   Time CFD: " << fSharc_PAD_TimeCFD[i]
    << "   Time LED: " << fSharc_PAD_TimeLED[i] << endl;

  }
}
