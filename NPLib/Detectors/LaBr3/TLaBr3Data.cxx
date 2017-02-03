/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  Adrien MATTA  contact address: matta@lpccaen.in2p3.fr  *
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
#include "TLaBr3Data.h"


ClassImp(TLaBr3Data)

TLaBr3Data::TLaBr3Data()
{
}



TLaBr3Data::~TLaBr3Data()
{
}



void TLaBr3Data::Clear()
{
   // Energy
   fLaBr3_E_Number.clear();
   fLaBr3_E_Energy.clear();
   // Time
   fLaBr3_T_Number.clear();
   fLaBr3_T_Time.clear();
}



void TLaBr3Data::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX TLaBr3: New Event XXXXXXXXXXXXXXXXX" << endl;

   // Energy
   for (unsigned short i = 0 ; i< fLaBr3_E_Energy.size() ; i ++) {
         cout << "LaBr3 Number " << fLaBr3_E_Number[i] << " Energy: " << fLaBr3_E_Energy[i]  << endl;
   }
   
   // Time
   for (unsigned short i = 0 ; i< fLaBr3_T_Time.size() ; i ++) {
         cout << "LaBr3 Number " << fLaBr3_T_Number[i] << " Time: " << fLaBr3_T_Time[i]  << endl;
   }
}
