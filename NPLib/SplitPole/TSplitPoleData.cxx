/*****************************************************************************
 * Copyright (C) 2009-2013   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : january 2011                                             *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *     This class holds the raw data storage for the SplitPole focal plane   *
 *     detector at Orsay                                                     *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include "TSplitPoleData.h"

#include <iostream>
using namespace std;

ClassImp(TSplitPoleData)



TSplitPoleData::TSplitPoleData()
   : fPosition(-1),
     fDeltaE(-1),
     fWire(-1),
     fPlasticP(-1),
     fPlasticG(-1),
     fTime1(-1000),
     fTime2(-1000)
{
}



TSplitPoleData::~TSplitPoleData()
{
}



void TSplitPoleData::Clear()
{
   fPosition = -1;
   fDeltaE   = -1;
   fWire     = -1;
   fPlasticP = -1;
   fPlasticG = -1;
   fTime1    = -1000;
   fTime2    = -1000;
}



void TSplitPoleData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TSplitPoleData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

   cout << "Position: " << fPosition << endl;
   cout << "DeltaE:   " << fDeltaE   << endl;
   cout << "Wire:     " << fWire     << endl;
   cout << "PlasticP: " << fPlasticP << endl;
   cout << "PlasticG: " << fPlasticG << endl;
   cout << "Time1:    " << fTime1    << endl;
   cout << "Time2:    " << fTime2    << endl;
}
