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
#include "TFoilData.h"


ClassImp(TFoilData)

TFoilData::TFoilData()
{
}



TFoilData::~TFoilData()
{
}



void TFoilData::Clear()
{
    fFoil_Energy.clear();
    fFoil_Number.clear();
    fFoil_Time.clear();
}



void TFoilData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   for(unsigned short i = 0 ; i<fFoil_Energy.size() ; i ++)
      {
         cout << "Foil Number " << fFoil_Number[i] << " Energy: " << fFoil_Energy[i]  << " Time: "<< fFoil_Time[i] << endl;
      
      }
   
}
