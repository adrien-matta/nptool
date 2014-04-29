/*****************************************************************************
 * Copyright (C) 2009-2014   this file is part of the NPTool Project         *
 *                                                                           *
 * For the licensing terms see $NPTOOL/Licence/NPTool_Licence                *
 * For the list of contributors see $NPTOOL/Licence/Contributors             *
 *****************************************************************************/

/*****************************************************************************
 * Original Author: M. Labiche    contact address: marc.labiche@stfc.ac.uk   *
 *                                                                           *
 * Creation Date  : 04/12/2009                                               *
 * Last update    :                                                          *
 *---------------------------------------------------------------------------*
 * Decription:                                                               *
 *  This class describes the raw data of the ChateauCristal detector         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Comment:                                                                  *
 *                                                                           *
 *                                                                           *
 *****************************************************************************/
#include <iostream>
#include "TChateauCristalData.h"

ClassImp(TChateauCristalData)

TChateauCristalData::TChateauCristalData()
{
    fChateauCristal_EDetectorNbr.clear();
    fChateauCristal_Energy.clear();
    fChateauCristal_TDetectorNbr.clear();
    fChateauCristal_Time.clear();
}

TChateauCristalData::~TChateauCristalData()
{
}

void TChateauCristalData::Clear()
{
    fChateauCristal_EDetectorNbr.clear();
    fChateauCristal_Energy.clear();
    fChateauCristal_TDetectorNbr.clear();
    fChateauCristal_Time.clear();
}

void TChateauCristalData::Dump() const
{
   cout << "XXXXXXXXXXXXXXXXXXXXXXXX New Event XXXXXXXXXXXXXXXXX" << endl;

   for (unsigned short i = 0; i<fChateauCristal_Energy.size(); i ++) {
     cout << "ChateauCristal Energy Number " << fChateauCristal_EDetectorNbr[i] << " Energy: " << fChateauCristal_Energy[i] << endl;
     cout << "ChateauCristal Time Number " << fChateauCristal_TDetectorNbr[i] << " Time: "<< fChateauCristal_Time[i] << endl;
   }
}
