/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  Adrien MATTA contact address: matta@lpccaen.in2p3.fr   *
 *                                                                           *
 * Creation Date   :                                                         *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
#include "TPlasticData.h"


ClassImp(TPlasticData)

TPlasticData::TPlasticData()
{
}



TPlasticData::~TPlasticData()
{
}



void TPlasticData::Clear()
{
    fPlastic_Energy.clear();
    fPlastic_Number.clear();
    fPlastic_Time.clear();
}



void TPlasticData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   for(unsigned short i = 0 ; i<fPlastic_Energy.size() ; i ++)
      {
         cout << "Plastic Number " << fPlastic_Number[i] << " Energy: " << fPlastic_Energy[i]  << " Time: "<< fPlastic_Time[i] << endl;
      
      }
   
}
