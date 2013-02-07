/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
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

#include "TChio_anData.h"


ClassImp(TChio_anData)

TChio_anData::TChio_anData()
{
   // Default constructor

   // (E)
   fChio_an_Energy.clear();
   fChio_an_Energy_pileup.clear();
}



TChio_anData::~TChio_anData()
{
}



void TChio_anData::Clear()
{
   // (E)
   fChio_an_Energy.clear();
   fChio_an_Energy_pileup.clear();
}



void TChio_anData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // Chio_an
   // (E)
   cout << "Chio_an_MultE = " << fChio_an_Energy.size() << endl;
   for (UShort_t i = 0; i < fChio_an_Energy.size(); i++)
      cout <<  " Energy: " << fChio_an_Energy[i] << endl;

   cout << "Chio_an_MultE pileup = " << fChio_an_Energy_pileup.size() << endl;
   for (UShort_t i = 0; i < fChio_an_Energy_pileup.size(); i++)
      cout <<  " Energy: " << fChio_an_Energy_pileup[i] << endl;
}
