/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Sandra Giron  contact address: giron@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : febuary 2010                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class hold CATS raw data                                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *****************************************************************************/

#include <iostream>

#include "TCATSData.h"


ClassImp(TCATSData)

TCATSData::TCATSData()
{
   // Default constructor

   Clear();
}



TCATSData::~TCATSData()
{
}



void TCATSData::Clear()
{
   // X
   fCATS_DetX.clear();
   fCATS_StripX.clear();
   fCATS_ChargeX.clear();
   // Y
   fCATS_DetY.clear();
   fCATS_StripY.clear();
   fCATS_ChargeY.clear();
   // (Qfil)
   fCATS_DetQ.clear();
   fCATS_Charge.clear();
}



void TCATSData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   // X
   cout << "CATS_MultX = " << fCATS_DetX.size() << endl;
   for (UShort_t i = 0; i < fCATS_DetX.size(); i++)
      cout << "DetX: " << fCATS_DetX[i] << " StripX: " << fCATS_StripX[i] << " ChargeX: " << fCATS_ChargeX[i] << endl;
   // Y
   cout << "CATS_MultY = " << fCATS_DetY.size() << endl;
   for (UShort_t i = 0; i < fCATS_DetY.size(); i++)
      cout << "DetY: " << fCATS_DetY[i] << " StripY: " << fCATS_StripY[i] << " EnergyY: " << fCATS_ChargeY[i] << endl;
   // (Qfil)
   cout << "MM_MultQ = " << fCATS_DetQ.size() << endl;
   for (UShort_t i = 0; i < fCATS_DetQ.size(); i++)
      cout << "DetQ: " << fCATS_DetQ[i] << " Charge: " << fCATS_Charge[i] << endl;
}
