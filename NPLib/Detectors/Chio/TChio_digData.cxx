/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author:               contact address:                           *
 *                                                                           *
 * Creation Date  :                                                          *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
using namespace std;

#include "TChio_digData.h"


ClassImp(TChio_digData)
////////////////////////////////////////////////////////////////////////////////
TChio_digData::TChio_digData()
{
}


////////////////////////////////////////////////////////////////////////////////
TChio_digData::~TChio_digData()
{
}


////////////////////////////////////////////////////////////////////////////////
void TChio_digData::Clear()
{
   fChio_dig_Energy.clear();
   fChio_dig_Time.clear();

}


////////////////////////////////////////////////////////////////////////////////
void TChio_digData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // (E)
   cout << "Chio_dig_Esize = " << fChio_dig_Energy.size() << endl;
}
////////////////////////////////////////////////////////////////////////////////
TGraph* TChio_digData::GetEnergyAsGraph(){
  TGraph* res = new TGraph (fChio_dig_Energy.size(),&fChio_dig_Time[0],&fChio_dig_Energy[0]);
  res->Sort();
  return res;

}
