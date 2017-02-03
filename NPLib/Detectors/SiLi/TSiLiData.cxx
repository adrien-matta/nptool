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
#include "TSiLiData.h"


ClassImp(TSiLiData)

TSiLiData::TSiLiData()
{
}



TSiLiData::~TSiLiData()
{
}



void TSiLiData::Clear()
{
   // Energy
   fSiLi_E_Number.clear();
   fSiLi_E_Energy.clear();
   // Time
   fSiLi_T_Number.clear();
   fSiLi_T_Time.clear();
}



void TSiLiData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX TSiLi: New Event XXXXXXXXXXXXXXXXX" << endl;

   // Energy
   for (unsigned short i = 0 ; i< fSiLi_E_Energy.size() ; i ++) {
         cout << "SiLi Number " << fSiLi_E_Number[i] << " Energy: " << fSiLi_E_Energy[i]  << endl;
   }
   
   // Time
   for (unsigned short i = 0 ; i< fSiLi_T_Time.size() ; i ++) {
         cout << "SiLi Number " << fSiLi_T_Number[i] << " Time: " << fSiLi_T_Time[i]  << endl;
   }
}
