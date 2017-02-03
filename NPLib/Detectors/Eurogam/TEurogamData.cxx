/*****************************************************************************
 * Copyright (C) 2009-2016   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : October 2009                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class described the raw data of a Eurogam germanium detector        *
 *                                                                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
#include "TEurogamData.h"
ClassImp(TEurogamData)

TEurogamData::TEurogamData()
{
}



TEurogamData::~TEurogamData()
{
}



void TEurogamData::Clear()
{
   fEurogam_DetectorNumber.clear();
   fEurogam_Energy.clear();
   fEurogam_Time.clear();
}



void TEurogamData::Dump() const
{
   cout << "XXXXXXXXXXX TEurogamData: New Event XXXXXXXXXXXXXXXXX" << endl;

	for (UShort_t i = 0 ; i < fEurogam_DetectorNumber.size(); i ++) {
      cout << "Eurogam Number " << fEurogam_DetectorNumber[i] << " Energy: " << fEurogam_Energy[i]  << " Time: "<< fEurogam_Time[i] << endl;
   }
}
