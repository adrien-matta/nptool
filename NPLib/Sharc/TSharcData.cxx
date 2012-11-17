/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : febuary 2009                                             *
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
using namespace std;

#include "TSharcData.h"

ClassImp(TSharcData)

/////////////////////////
TSharcData::TSharcData()
{
  // Default constructor
  Clear();
}

/////////////////////////
TSharcData::~TSharcData()
{
  Clear();
}

/////////////////////////
void TSharcData::Clear()
{
  fSharc_StripFront_DetectorNbr.clear();
  fSharc_StripFront_StripNbr.clear();
  fSharc_StripFront_Energy.clear();
  fSharc_StripFront_Time.clear();
  
  fSharc_StripBack_DetectorNbr.clear();
  fSharc_StripBack_StripNbr.clear();
  fSharc_StripBack_Energy.clear();
  fSharc_StripBack_Time.clear();
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
         << " Energy: " << fSharc_StripFront_Energy[i] << endl;
  }
  
  // Back
  for (UShort_t i = 0; i < fSharc_StripFront_DetectorNbr.size(); i++){
    cout << "DetNbr: " << fSharc_StripFront_DetectorNbr[i]
    << " Strip: " << fSharc_StripFront_StripNbr[i]
    << " Energy: " << fSharc_StripFront_Energy[i] << endl;
  }
}