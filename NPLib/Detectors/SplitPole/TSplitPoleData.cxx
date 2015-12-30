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
     fTick(-1)
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
   fTick     = -1;
   fTime1.clear();
   fTime2.clear();
}



void TSplitPoleData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event [TSplitPoleData::Dump()] XXXXXXXXXXXXXXXXX" << endl;

   cout << "Position: " << fPosition << endl;
   cout << "DeltaE:   " << fDeltaE   << endl;
   cout << "Wire:     " << fWire     << endl;
   cout << "PlasticP: " << fPlasticP << endl;
   cout << "PlasticG: " << fPlasticG << endl;
   cout << "Tick:     " << fTick     << endl;

   cout << "Time1:";
   for (UInt_t i = 0; i < fTime1.size(); ++i) {   // loop on fTime1
      cout << "\t" << fTime1[i];
   } // end loop on fTime1
   cout << endl;

   cout << "Time2:";
   for (UInt_t i = 0; i < fTime2.size(); ++i) {   // loop on fTime2
      cout << "\t" << fTime2[i];
   } // end loop on fTime2
   cout << endl;
}
