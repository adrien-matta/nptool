#include <iostream>
#include "TDUMMYDetectorData.h"
/*****************************************************************************
 * Copyright (C) 2009-2010   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: Adrien MATTA  contact address: matta@ipno.in2p3.fr       *
 *                                                                           *
 * Creation Date  : October 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class described the raw data of a very simple DUMMYDetector         *
 *   Use it as a template for your own detector!                              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/

ClassImp(TDUMMYDetectorData)

TDUMMYDetectorData::TDUMMYDetectorData()
{
}



TDUMMYDetectorData::~TDUMMYDetectorData()
{
}



void TDUMMYDetectorData::Clear()
{
    fDUMMYDetector_Energy.clear();
    fDUMMYDetector_Number.clear();
    fDUMMYDetector_Time.clear();
}



void TDUMMYDetectorData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   for(unsigned short i = 0 ; i<fDUMMYDetector_Energy.size() ; i ++)
      {
         cout << "DUMMYDetector Number " << fDUMMYDetector_Number[i] << " Energy: " << fDUMMYDetector_Energy[i]  << " Time: "<< fDUMMYDetector_Time[i] << endl;
      
      }
   
}
