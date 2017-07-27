/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $GANIL2ROOT/Licence                           *
 * For the list of contributors see $GANIL2ROOT/README/CREDITS               *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: L. Lefebvre  contact address: lefebvrl@ipno.in2p3.fr     *
 *                    							                                         *
 *                                                                           *
 * Creation Date  : June 2011                                                *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription: This class holds EXL Raw data                                 *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

#include <iostream>
#include "TExlData.h"


ClassImp(TExlData)

TExlData::TExlData()
{
}



TExlData::~TExlData()
{
}



void TExlData::Clear()
{
    fExl_Energy.clear();
    fExl_Time.clear();
    fExl_Number.clear();
    fExl_Crystal.clear();
}



void TExlData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   for(unsigned short i = 0 ; i<fExl_Energy.size() ; i ++)
      {
         cout << "Exl Number " << fExl_Number[i] << " Crystal Number " << fExl_Crystal[i] << " Energy: " << fExl_Energy[i] << " Time: " << fExl_Time[i] << endl;

      }

}
