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
#include "TSiResData.h"


ClassImp(TSiResData)

TSiResData::TSiResData()
{
}



TSiResData::~TSiResData()
{
}



void TSiResData::Clear()
{
   // Energy
   fSiRes_E_Number.clear();
   fSiRes_E_Channel.clear();
   fSiRes_E_Energy.clear();
   // Time
   fSiRes_T_Number.clear();
   fSiRes_T_Time.clear();
   fSiRes_E_EnergyBack.clear();
}



void TSiResData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX TSiRes: New Event XXXXXXXXXXXXXXXXX" << endl;

   // Energy
   for (unsigned short i = 0 ; i< fSiRes_E_Energy.size() ; i ++) {
         cout << "SiRes Number " << fSiRes_E_Number[i] << " Energy: " << fSiRes_E_Energy[i]  << endl;
   }
   
   // Time
   for (unsigned short i = 0 ; i< fSiRes_T_Time.size() ; i ++) {
         cout << "SiRes Number " << fSiRes_T_Number[i] << " Time: " << fSiRes_T_Time[i]  << endl;
   }
}
