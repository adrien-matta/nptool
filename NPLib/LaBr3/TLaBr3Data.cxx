/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * Original Author :  Adrien MATTA    contact address: matta@ipno.in2p3.fr   *
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
    fLaBr3_Energy.clear();
    fLaBr3_Number.clear();
    fLaBr3_Time.clear();
}



void TLaBr3Data::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   for(unsigned short i = 0 ; i<fLaBr3_Energy.size() ; i ++)
      {
         cout << "LaBr3 Number " << fLaBr3_Number[i] << " Energy: " << fLaBr3_Energy[i]  << " Time: "<< fLaBr3_Time[i] << endl;
      
      }
   
}
