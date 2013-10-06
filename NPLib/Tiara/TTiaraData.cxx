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
 *  This class hold Tiara Raw data                                           *
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

#include "TTiaraData.h"

ClassImp(TTiaraData)

/////////////////////////
TTiaraData::TTiaraData()
{
}

/////////////////////////
TTiaraData::~TTiaraData()
{
}

/////////////////////////
void TTiaraData::Clear()
{
  fTiara_StripFront_DetectorNbr.clear();
  fTiara_StripFront_StripNbr.clear();
  fTiara_StripFront_Energy.clear();
  fTiara_StripFront_TimeCFD.clear();
  fTiara_StripFront_TimeLED.clear();

  fTiara_StripBack_DetectorNbr.clear();
  fTiara_StripBack_StripNbr.clear();
  fTiara_StripBack_Energy.clear();
  fTiara_StripBack_TimeCFD.clear();
  fTiara_StripBack_TimeLED.clear();
  
  fTiara_PAD_DetectorNbr.clear();
  fTiara_PAD_Energy.clear();
  fTiara_PAD_TimeCFD.clear();
  fTiara_PAD_TimeLED.clear();

}

/////////////////////////
void TTiaraData::Dump() const
{
  // Energy
  cout << "Tiara_StripFront_Mult = " << fTiara_StripFront_DetectorNbr.size() << endl;
  
  // Front
  for (UShort_t i = 0; i < fTiara_StripFront_DetectorNbr.size(); i++){
    cout << "DetNbr: " << fTiara_StripFront_DetectorNbr[i]
         << " Strip: " << fTiara_StripFront_StripNbr[i]
         << " Energy: " << fTiara_StripFront_Energy[i]
         << " Time CFD: " << fTiara_StripFront_TimeCFD[i]
         << " Time LED: " << fTiara_StripFront_TimeLED[i] << endl;
  }
  
  // Back
  for (UShort_t i = 0; i < fTiara_StripFront_DetectorNbr.size(); i++){
    cout << "DetNbr: " << fTiara_StripFront_DetectorNbr[i]
    << " Strip: " << fTiara_StripFront_StripNbr[i]
    << " Energy: " << fTiara_StripFront_Energy[i]
    << " Time CFD: " << fTiara_StripBack_TimeCFD[i]
    << " Time LED: " << fTiara_StripBack_TimeLED[i] << endl;
  }
  
  // PAD
  for (UShort_t i = 0; i < fTiara_PAD_DetectorNbr.size(); i++){
    cout << "DetNbr: " << fTiara_PAD_DetectorNbr[i]
    << " Energy: " << fTiara_PAD_Energy[i]
    << " Time CFD: " << fTiara_PAD_TimeCFD[i]
    << " Time LED: " << fTiara_PAD_TimeLED[i] << endl;

  }
}