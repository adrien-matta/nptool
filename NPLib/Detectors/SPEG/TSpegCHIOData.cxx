/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/
/*****************************************************************************
 * Original Author: L. Lefebvre  contact address: lefebvrl@ipno.in2p3.fr     *
 *                                                                           *
 * Creation Date  : June 2011                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class holds Speg Raw data                                          *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
using namespace std;

#include "TSpegCHIOData.h"


ClassImp(TSpegCHIOData)

TSpegCHIOData::TSpegCHIOData()
{
   // Default constructor
   Clear();
}



TSpegCHIOData::~TSpegCHIOData()
{
}



void TSpegCHIOData::Clear()
{
   fSpeg_CHIO_Energy.clear();
}



void TSpegCHIOData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;


   for (UShort_t i = 0; i < fSpeg_CHIO_Energy.size(); i++)
      cout << " Energy: " << fSpeg_CHIO_Energy[i] << endl;
}
