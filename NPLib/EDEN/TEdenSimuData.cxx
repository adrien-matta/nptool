#include <iostream>
#include "TEdenSimuData.h"
/*****************************************************************************
 * Copyright (C) 2009   this file is part of the NPTool Project              *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: N. de Sereville  contact address: deserevi@ipno.in2p3.fr *
 *                                                                           *
 * Creation Date  : 11/02/2010                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *    This class described is used to store the results of the G4 simulation *
 *    concerning the EDEN neutron detector.                                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *    This class is mainly dedicated for the G4 simulation results and not   *
 *    for the analysis of real experiment.                                   *
 *                                                                           *
 *****************************************************************************/
ClassImp(TEdenSimuData)

TEdenSimuData::TEdenSimuData()
{
}



TEdenSimuData::~TEdenSimuData()
{
}



void TEdenSimuData::Clear()
{
    fEdenSimu_DetectorNumber.clear();
    fEdenSimu_Charge.clear();
    fEdenSimu_Time.clear();
}



void TEdenSimuData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   for (unsigned short i = 0; i < fEdenSimu_DetectorNumber.size(); i++) {
      cout << "EdenSimu Detector Number " << fEdenSimu_DetectorNumber[i] << " Charge: " << fEdenSimu_Charge[i]  << " Time: "<< fEdenSimu_Time[i] << endl;
   }
}
