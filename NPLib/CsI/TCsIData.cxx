/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  Adrien MATTA    contact address: a.matta@surrey.ac.uk  *
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
#include "TCsIData.h"


ClassImp(TCsIData)

TCsIData::TCsIData()
{
}



TCsIData::~TCsIData()
{
}



void TCsIData::Clear()
{
   // Energy
   fCsI_E_Number.clear();
   fCsI_E_Energy.clear();
   // Time
   fCsI_T_Number.clear();
   fCsI_T_Time.clear();
}



void TCsIData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX TCsI: New Event XXXXXXXXXXXXXXXXX" << endl;

   // Energy
   for (unsigned short i = 0 ; i< fCsI_E_Energy.size() ; i ++) {
         cout << "CsI Number " << fCsI_E_Number[i] << " Energy: " << fCsI_E_Energy[i]  << endl;
   }
   
   // Time
   for (unsigned short i = 0 ; i< fCsI_T_Time.size() ; i ++) {
         cout << "CsI Number " << fCsI_T_Number[i] << " Time: " << fCsI_T_Time[i]  << endl;
   }
}
