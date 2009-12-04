#include <iostream>
#include "TParisData.h"
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 04/12/2009                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class described the raw data of the Paris detector                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

ClassImp(TParisData)

TParisData::TParisData()
{
}



TParisData::~TParisData()
{
}



void TParisData::Clear()
{
    fParis_Energy.clear();
    fParis_Number.clear();
    fParis_Time.clear();
}



void TParisData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   for (unsigned short i = 0; i<fParis_Energy.size(); i ++) {
      cout << "Paris Number " << fParis_Number[i] << " Energy: " << fParis_Energy[i]  << " Time: "<< fParis_Time[i] << endl;
   }
}
